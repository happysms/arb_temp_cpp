#include "exchange.h"
#include "../util/util.h"
#include <iostream>
#include <sstream>
#include <algorithm>

GateioFutureExchange::GateioFutureExchange() {{
    name_ = "gateio_f";
    q_ = std::make_shared<std::queue<int>>();
    ob_["BTC_USDT"] = {{27000.0, 27010.0}};
    tk_["BTC_USDT"] = {{0.1, 0.001}};
    ac_["BTC_USDT"] = {{0.0, 0.0, now_ms()}};
}}

std::shared_ptr<std::queue<int>> GateioFutureExchange::register_listener() {{ return q_; }}
bool GateioFutureExchange::has_orderbook(const std::string& t) const {{ return ob_.count(t) > 0; }}
double GateioFutureExchange::get_ask_price(const std::string& t) const {{ return ob_.at(t).ask; }}
double GateioFutureExchange::get_best_ask(const std::string& t) const {{ return ob_.at(t).best_ask; }}
double GateioFutureExchange::get_tick_size(const std::string& t) const {{ return tk_.at(t).tick; }}
double GateioFutureExchange::get_min_qty(const std::string& t) const {{ return tk_.at(t).minq; }}

void GateioFutureExchange::order(const std::string& t, const std::string& side, double qty, double price) {{
    std::ostringstream oss; oss << "order-" << now_ms();
    waiting_.push_back({{t, oss.str(), price}});
    ac_[t].balance += (side == "buy" ? qty : -qty);
    ac_[t].avg = price;
    ac_[t].ts = now_ms();
    q_->push(1);
    std::cout << "[gateio_f] order " << side << " " << qty << " @ " << price << "\n";
}}

void GateioFutureExchange::cancel_order(const std::string& t, const std::string& uuid) {{
    waiting_.erase(std::remove_if(waiting_.begin(), waiting_.end(), [&](const OrderInfo& o){{return o.ticker==t && o.uuid==uuid;}}), waiting_.end());
    std::cout << "[gateio_f] cancel " << uuid << "\n";
}}

bool GateioFutureExchange::waiting_list_empty() const {{ return waiting_.empty(); }}
std::vector<OrderInfo> GateioFutureExchange::get_waiting_orders() const {{ return waiting_; }}
double GateioFutureExchange::get_balance(const std::string& t) const {{ return ac_.at(t).balance; }}
long long GateioFutureExchange::get_account_timestamp(const std::string& t) const {{ return ac_.at(t).ts; }}
double GateioFutureExchange::get_avg_price(const std::string& t, double) const {{ return ac_.at(t).avg; }}
std::string GateioFutureExchange::get_name() const {{ return name_; }}
