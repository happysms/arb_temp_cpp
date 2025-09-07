#pragma once
#include <string>
#include <memory>
#include "api.h"

class WebSocketManager {
public:
    explicit WebSocketManager(std::shared_ptr<API> api);
    void connect();
    void subscribe(const std::string& symbol);
    void run();
private:
    std::shared_ptr<API> api_;
};
