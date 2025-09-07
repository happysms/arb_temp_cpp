#include <iostream>
#include <cstdlib>
#include <string>
#include <memory>
#include <thread>
#include "strategy/strategy.h"
#include "exchange/exchange.h"
#include "binance_f_module/exchange.h"
#include "binance_s_module/exchange.h"
#include "gateio_f_module/exchange.h"
#include "gateio_s_module/exchange.h"

static std::string env(const char* k, const std::string& def = "") {
    const char* v = std::getenv(k);
    return v ? std::string(v) : def;
}
static double envd(const char* k, double def = 0.0) {
    const char* v = std::getenv(k);
    return v ? std::stod(v) : def;
}

int main() {
    std::string TICKER = env("TICKER", "BTC_USDT");
    double MAX_QTY   = envd("MAX_QTY", 1.0);
    double TRADE_QTY = envd("TRADE_QTY", 0.01);
    double SPREAD    = envd("SPREAD", 0.001);
    bool has_cancel_buffer = std::getenv("CANCEL_BUFFER") != nullptr;
    double CANCEL_BUFFER = has_cancel_buffer ? envd("CANCEL_BUFFER", 0.0) : 0.0;
    std::string MAKER_EXCHANGE = env("MAKER_EXCHANGE", "binance_f");
    std::string TAKER_EXCHANGE = env("TAKER_EXCHANGE", "gateio_f");

    std::shared_ptr<Exchange> maker, taker;
    if (MAKER_EXCHANGE == "binance_f") maker = std::make_shared<BinanceFutureExchange>();
    else if (MAKER_EXCHANGE == "binance_s") maker = std::make_shared<BinanceSpotExchange>();
    else if (MAKER_EXCHANGE == "gateio_f") maker = std::make_shared<GateioFutureExchange>();
    else if (MAKER_EXCHANGE == "gateio_s") maker = std::make_shared<GateioSpotExchange>();

    if (TAKER_EXCHANGE == "binance_f") taker = std::make_shared<BinanceFutureExchange>();
    else if (TAKER_EXCHANGE == "binance_s") taker = std::make_shared<BinanceSpotExchange>();
    else if (TAKER_EXCHANGE == "gateio_f") taker = std::make_shared<GateioFutureExchange>();
    else if (TAKER_EXCHANGE == "gateio_s") taker = std::make_shared<GateioSpotExchange>();

    Strategy strategy(TICKER, MAX_QTY, TRADE_QTY, SPREAD, has_cancel_buffer ? std::optional<double>(CANCEL_BUFFER) : std::nullopt);

    // simulate event listeners
    std::thread([&]() {
        while (true) {
            strategy.run(maker, taker);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }).join();

    return 0;
}
