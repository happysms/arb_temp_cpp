#pragma once
#include <string>
#include <vector>
#include <queue>
#include <memory>

struct OrderInfo {
    std::string ticker;
    std::string uuid;
    double price;
};

class Exchange {
public:
    virtual ~Exchange() = default;

    virtual std::shared_ptr<std::queue<int>> register_listener() = 0;

    virtual bool has_orderbook(const std::string& ticker) const = 0;
    virtual double get_ask_price(const std::string& ticker) const = 0;
    virtual double get_best_ask(const std::string& ticker) const = 0;
    virtual double get_tick_size(const std::string& ticker) const = 0;
    virtual double get_min_qty(const std::string& ticker) const = 0;

    virtual void order(const std::string& ticker, const std::string& side, double qty, double price) = 0;
    virtual void cancel_order(const std::string& ticker, const std::string& uuid) = 0;

    virtual bool waiting_list_empty() const = 0;
    virtual std::vector<OrderInfo> get_waiting_orders() const = 0;

    virtual double get_balance(const std::string& ticker) const = 0;
    virtual long long get_account_timestamp(const std::string& ticker) const = 0;
    virtual double get_avg_price(const std::string& ticker, double balance = -1) const = 0;
    virtual std::string get_name() const = 0;
};
