#include "strategy.h"
#include "../util/util.h"
#include <iostream>
#include <thread>
#include <cmath>

Strategy::Strategy(std::string ticker, double max_qty, double trade_qty, double spread, std::optional<double> cancel_buffer)
: ticker_(std::move(ticker)), max_qty_(max_qty), trade_qty_(trade_qty), spread_(spread), cancel_buffer_(cancel_buffer.value_or(0.0)), start_time_(now_ms()) {}

void Strategy::run(std::shared_ptr<Exchange> maker, std::shared_ptr<Exchange> taker) {
    if (!maker || !taker) return;
    if (!maker->has_orderbook(ticker_) || !taker->has_orderbook(ticker_)) return;
    if ( (now_ms() - start_time_) < 3000 ) return; // warmup

    double ask_price = taker->get_ask_price(ticker_);
    double target_ratio = spread_ + cancel_buffer_/2.0;
    double tick = maker->get_tick_size(ticker_);
    double target_price = ceil_by_tick(ask_price * (1.0 + target_ratio), tick);
    if (target_price < maker->get_best_ask(ticker_)) target_price = maker->get_best_ask(ticker_);

    // hedge check in background
    std::thread([=]() {
        hedge_if_needed(maker, taker, ask_price);
    }).detach();

    if (maker->waiting_list_empty() && maker->get_balance(ticker_) > -max_qty_) {
        std::thread([=]() {
            maker->order(ticker_, "sell", trade_qty_, target_price);
        }).detach();
    }

    for (const auto& o : maker->get_waiting_orders()) {
        const double prev_ratio = o.price / ask_price - 1.0;
        const double low_ratio = spread_;
        const double high_ratio = spread_ + cancel_buffer_;
        if (cancel_buffer_ == 0.0) {
            if (o.price != target_price) {
                maker->cancel_order(o.ticker, o.uuid);
            }
        } else {
            if (!(low_ratio <= prev_ratio && prev_ratio <= high_ratio) && target_price != maker->get_best_ask(ticker_)) {
                maker->cancel_order(o.ticker, o.uuid);
            }
        }
        double bal = std::fabs(maker->get_balance(ticker_));
        if (max_qty_*0.999 < bal && bal < max_qty_*1.0001) {
            maker->cancel_order(o.ticker, o.uuid);
        }
    }

    trade_print(maker, taker);
}

void Strategy::hedge_if_needed(std::shared_ptr<Exchange> maker, std::shared_ptr<Exchange> taker, double taker_price) {
    if (taker_price <= 0) return;
    if ((now_ms() - maker->get_account_timestamp(ticker_) < 1000) &&
        (now_ms() - taker->get_account_timestamp(ticker_) < 1000)) {
        const double sum_qty = -(maker->get_balance(ticker_) + taker->get_balance(ticker_));
        const double minq = taker->get_min_qty(ticker_);
        if (sum_qty > minq) {
            double hp = ceil_by_tick(taker_price * 1.005, taker->get_tick_size(ticker_));
            taker->order(ticker_, "buy", sum_qty, hp);
        } else if (std::fabs(sum_qty) > minq) {
            double hp = floor_by_tick(taker_price * 0.995, taker->get_tick_size(ticker_));
            taker->order(ticker_, "sell", std::fabs(sum_qty), hp);
        }
    }
}

void Strategy::trade_print(std::shared_ptr<Exchange> maker, std::shared_ptr<Exchange> taker) {
    double cur_maker = maker->get_balance(ticker_);
    double cur_taker = taker->get_balance(ticker_);
    if ((last_maker_balance_ - cur_maker != 0) || (last_taker_balance_ - cur_taker != 0)) {
        double taker_avg = taker->get_avg_price(ticker_, cur_taker);
        double maker_avg = maker->get_avg_price(ticker_);
        std::cout << ((last_maker_balance_ - cur_maker != 0) ? "[메이커 체결]\n" : "[테이커 체결]\n");
        std::cout << "maker " << maker->get_name() << " 수량: " << cur_maker << " 평단: " << maker_avg << "\n";
        std::cout << "taker " << taker->get_name() << " 수량: " << cur_taker << " 평단: " << taker_avg << "\n\n";
    }
    last_maker_balance_ = cur_maker;
    last_taker_balance_ = cur_taker;
}
