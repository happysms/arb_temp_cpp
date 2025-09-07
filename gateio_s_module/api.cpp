#include "api.h"
#include <iostream>

API::API(const std::string& k, const std::string& s) : api_key_(k), api_secret_(s) {}
void API::authenticate() { std::cout << "[API] auth with key: " << api_key_ << "\n"; }
void API::send_order(const std::string& symbol, const std::string& side, double qty, double price) {
    std::cout << "[API] send order " << side << " " << qty << " " << symbol << " @ " << price << "\n";
}
std::string API::api_key() const { return api_key_; }
