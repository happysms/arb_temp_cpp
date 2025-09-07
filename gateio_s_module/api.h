#pragma once
#include <string>

class API {
public:
    API(const std::string& api_key, const std::string& api_secret);
    void authenticate();
    void send_order(const std::string& symbol, const std::string& side, double qty, double price);
    std::string api_key() const;
private:
    std::string api_key_;
    std::string api_secret_;
};
