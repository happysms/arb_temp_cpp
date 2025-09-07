#include "websocket_manager.h"
#include <iostream>

WebSocketManager::WebSocketManager(std::shared_ptr<API> api) : api_(std::move(api)) {}
void WebSocketManager::connect() { std::cout << "[WS] connect with key: " << api_->api_key() << "\n"; }
void WebSocketManager::subscribe(const std::string& symbol) { std::cout << "[WS] subscribe " << symbol << "\n"; }
void WebSocketManager::run() { std::cout << "[WS] loop running..." << "\n"; }
