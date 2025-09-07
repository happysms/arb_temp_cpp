#include "exchange.h"
#include "../util/util.h"
#include <iostream>
#include <sstream>
#include <algorithm>

BinanceSpotExchange::BinanceSpotExchange() {{
    name_ = "binance_s";
    q_ = std::make_shared<std::queue<int>>();
    ob_["BTC_USDT"] = {{27000.0, 27010.0}};
    tk_["BTC_USDT"] = {{0.1, 0.001}};
    ac_["BTC_USDT"] = {{0.0, 0.0, now_ms()}};
}}

std::shared_ptr<std::queue<int>> BinanceSpotExchange::register_listener() {{ return q_; }}
bool BinanceSpotExchange::has_orderbook(const std::string& t) const {{ return ob_.count(t) > 0; }}
double BinanceSpotExchange::get_ask_price(const std::string& t) const {{ return ob_.at(t).ask; }}
double BinanceSpotExchange::get_best_ask(const std::string& t) const {{ return ob_.at(t).best_ask; }}
double BinanceSpotExchange::get_tick_size(const std::string& t) const {{ return tk_.at(t).tick; }}
double BinanceSpotExchange::get_min_qty(const std::string& t) const {{ return tk_.at(t).minq; }}

void BinanceSpotExchange::order(const std::string& t, const std::string& side, double qty, double price) {{
    std::ostringstream oss; oss << "order-" << now_ms();
    waiting_.push_back({{t, oss.str(), price}});
    ac_[t].balance += (side == "buy" ? qty : -qty);
    ac_[t].avg = price;
    ac_[t].ts = now_ms();
    q_->push(1);
    std::cout << "[binance_s] order " << side << " " << qty << " @ " << price << "\n";
}}

void BinanceSpotExchange::cancel_order(const std::string& t, const std::string& uuid) {{
    waiting_.erase(std::remove_if(waiting_.begin(), waiting_.end(), [&](const OrderInfo& o){{return o.ticker==t && o.uuid==uuid;}}), waiting_.end());
    std::cout << "[binance_s] cancel " << uuid << "\n";
}}

bool BinanceSpotExchange::waiting_list_empty() const {{ return waiting_.empty(); }}
std::vector<OrderInfo> BinanceSpotExchange::get_waiting_orders() const {{ return waiting_; }}
double BinanceSpotExchange::get_balance(const std::string& t) const {{ return ac_.at(t).balance; }}
long long BinanceSpotExchange::get_account_timestamp(const std::string& t) const {{ return ac_.at(t).ts; }}
double BinanceSpotExchange::get_avg_price(const std::string& t, double) const {{ return ac_.at(t).avg; }}
std::string BinanceSpotExchange::get_name() const {{ return name_; }}
