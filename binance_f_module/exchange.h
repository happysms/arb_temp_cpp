#pragma once
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <memory>
#include "../exchange/exchange.h"

class BinanceFutureExchange : public Exchange {
public:
    BinanceFutureExchange();
    std::shared_ptr<std::queue<int>> register_listener() override;

    bool has_orderbook(const std::string& ticker) const override;
    double get_ask_price(const std::string& ticker) const override;
    double get_best_ask(const std::string& ticker) const override;
    double get_tick_size(const std::string& ticker) const override;
    double get_min_qty(const std::string& ticker) const override;

    void order(const std::string& ticker, const std::string& side, double qty, double price) override;
    void cancel_order(const std::string& ticker, const std::string& uuid) override;

    bool waiting_list_empty() const override;
    std::vector<OrderInfo> get_waiting_orders() const override;

    double get_balance(const std::string& ticker) const override;
    long long get_account_timestamp(const std::string& ticker) const override;
    double get_avg_price(const std::string& ticker, double balance = -1) const override;
    std::string get_name() const override;
private:
    std::string name_;
    std::shared_ptr<std::queue<int>> q_;

    struct OB { double ask=0, best_ask=0; };
    struct TK { double tick=0.1, minq=0.001; };
    struct AC { double balance=0, avg=0; long long ts=0; };

    std::map<std::string, OB> ob_;
    std::map<std::string, TK> tk_;
    std::map<std::string, AC> ac_;
    std::vector<OrderInfo> waiting_;
};
