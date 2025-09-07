#pragma once
#include <string>
#include <optional>
#include <memory>
#include "../exchange/exchange.h"

class Strategy {
public:
    Strategy(std::string ticker, double max_qty, double trade_qty, double spread, std::optional<double> cancel_buffer);
    void run(std::shared_ptr<Exchange> maker, std::shared_ptr<Exchange> taker);
private:
    std::string ticker_;
    double max_qty_;
    double trade_qty_;
    double spread_;
    double cancel_buffer_;
    long long start_time_;
    double last_maker_balance_ = 0.0;
    double last_taker_balance_ = 0.0;

    void hedge_if_needed(std::shared_ptr<Exchange> maker, std::shared_ptr<Exchange> taker, double taker_price);
    void trade_print(std::shared_ptr<Exchange> maker, std::shared_ptr<Exchange> taker);
};
