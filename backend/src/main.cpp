#include <iostream>
#include <vector>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <sstream>
#include "../inlcude/json.hpp"
#include "../inlcude/core/Product.h"

#pragma comment(lib, "ws2_32.lib")

using json = nlohmann::json;

std::vector<Product> products;
CRITICAL_SECTION products_mutex;

struct UserData {
    std::string id;
    std::string username;
    std::string password;
    std::string token;
    std::string role; // "customer" or "shopkeeper"
};
std::vector<UserData> users;
CRITICAL_SECTION users_mutex;

struct OrderData {
    std::string order_id;
    std::string username;
    double total_amount;
    std::string status; 
};
std::vector<OrderData> orders;
CRITICAL_SECTION orders_mutex;

void init_db() {
    // Database is now initially empty so shopkeepers must list items.
}

std::string make_response(int status_code, const std::string& status_text, const std::string& body) {
    std::string response = "HTTP/1.1 " + std::to_string(status_code) + " " + status_text + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    response += "Access-Control-Allow-Headers: Content-Type, Authorization\r\n";
    response += "Content-Type: application/json\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
    response += "\r\n";
    response += body;
    return response;
}

std::string handle_request(const std::string& method, const std::string& path, const std::string& body) {
    if (method == "OPTIONS") {
        return make_response(204, "No Content", "");
    }

    if (method == "GET" && path == "/products") {
        EnterCriticalSection(&products_mutex);
        json j_products = json::array();
        for (const auto& p : products) {
            j_products.push_back({
                {"id", p.getSku()}, 
                {"name", p.getName()}, 
                {"price", p.getPrice()},
                {"stock", p.getStock()},
                {"owner", p.getOwner()},
                {"img", "https://via.placeholder.com/200"}
            });
        }
        std::string res_body = j_products.dump();
        LeaveCriticalSection(&products_mutex);
        return make_response(200, "OK", res_body);
    }
    
    if (method == "POST" && path == "/register") {
        try {
            auto j = json::parse(body);
            std::string username = j["username"];
            std::string password = j["password"];
            std::string role = j.value("role", "customer");
            
            EnterCriticalSection(&users_mutex);
            UserData newUser{"U" + std::to_string(users.size()+1), username, password, "", role};
            users.push_back(newUser);
            LeaveCriticalSection(&users_mutex);
            
            return make_response(200, "OK", "{\"status\": \"registered\"}");
        } catch(...) {
            return make_response(400, "Bad Request", "{\"error\": \"invalid json\"}");
        }
    }

    if (method == "POST" && path == "/login") {
        try {
            auto j = json::parse(body);
            std::string username = j["username"];
            std::string password = j["password"];
            
            EnterCriticalSection(&users_mutex);
            std::string token = "";
            std::string role = "";
            for (auto& u : users) {
                if (u.username == username && u.password == password) {
                    u.token = "token_" + u.username;
                    token = u.token;
                    role = u.role;
                    break;
                }
            }
            LeaveCriticalSection(&users_mutex);
            
            if (token != "") {
                return make_response(200, "OK", "{\"token\": \"" + token + "\", \"role\": \"" + role + "\"}");
            } else {
                return make_response(401, "Unauthorized", "{\"error\": \"invalid credentials\"}");
            }
        } catch(...) {
            return make_response(400, "Bad Request", "{\"error\": \"invalid json\"}");
        }
    }

    if (method == "POST" && path == "/checkout") {
        try {
            auto j = json::parse(body);
            std::string token = j.value("token", "");
            double total = j.value("total", 0.0);
            
            if (token == "") return make_response(401, "Unauthorized", "{\"error\": \"missing token\"}");
            
            EnterCriticalSection(&products_mutex);
            if (j.contains("skus") && j["skus"].is_array()) {
                for (const auto& sku_val : j["skus"]) {
                    std::string sku_str = sku_val.get<std::string>();
                    for (auto& p : products) {
                        if (p.getSku() == sku_str && p.getStock() > 0) {
                            p.reduceStock(1);
                            break;
                        }
                    }
                }
            }
            LeaveCriticalSection(&products_mutex);

            EnterCriticalSection(&orders_mutex);
            OrderData order{"O" + std::to_string(orders.size()+1), token, total, "Paid"};
            orders.push_back(order);
            LeaveCriticalSection(&orders_mutex);
            
            return make_response(200, "OK", "{\"status\": \"payment successful\", \"order_id\": \"" + order.order_id + "\"}");
        } catch(...) {
            return make_response(400, "Bad Request", "{\"error\": \"invalid json\"}");
        }
    }

    if (method == "POST" && path == "/products") {
        try {
            auto j = json::parse(body);
            std::string token = j.value("token", "");
            std::string name = j.value("name", "");
            double price = j.value("price", 0.0);
            
            if (token == "") return make_response(401, "Unauthorized", "{\"error\": \"missing token\"}");
            
            bool is_shopkeeper = false;
            std::string shopkeeper_username = "";
            EnterCriticalSection(&users_mutex);
            for (auto& u : users) {
                if (u.token == token && u.role == "shopkeeper") {
                    is_shopkeeper = true;
                    shopkeeper_username = u.username;
                    break;
                }
            }
            LeaveCriticalSection(&users_mutex);

            if (!is_shopkeeper) {
                return make_response(403, "Forbidden", "{\"error\": \"Only shopkeepers can add products.\"}");
            }

            EnterCriticalSection(&products_mutex);
            std::string sku = "SKU" + std::to_string(products.size() + 1);
            products.push_back(Product(sku, name, price, 100, shopkeeper_username)); // Default stock 100
            LeaveCriticalSection(&products_mutex);

            return make_response(200, "OK", "{\"status\": \"product added\", \"sku\": \"" + sku + "\"}");
        } catch(...) {
            return make_response(400, "Bad Request", "{\"error\": \"invalid json\"}");
        }
    }

    return make_response(404, "Not Found", "{\"error\": \"Not Found\"}");
}

DWORD WINAPI ClientHandler(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string request(buffer, bytesReceived);
        
        // Parse request line
        size_t first_space = request.find(' ');
        size_t second_space = request.find(' ', first_space + 1);
        if (first_space != std::string::npos && second_space != std::string::npos) {
            std::string method = request.substr(0, first_space);
            std::string path = request.substr(first_space + 1, second_space - first_space - 1);
            
            // Extract body (very naive)
            std::string body = "";
            size_t body_pos = request.find("\r\n\r\n");
            if (body_pos != std::string::npos) {
                body = request.substr(body_pos + 4);
                // In a real server, we should read 'Content-Length' and keep recv'ing if body is incomplete.
                // For this demo, assuming 4096 bytes is enough for the entire request + body.
            }

            std::string response = handle_request(method, path, body);
            send(clientSocket, response.c_str(), response.length(), 0);
        }
    }
    closesocket(clientSocket);
    return 0;
}

int main() {
    InitializeCriticalSection(&products_mutex);
    InitializeCriticalSection(&users_mutex);
    InitializeCriticalSection(&orders_mutex);
    init_db();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(18080);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error: Port 18080 is already in use or bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error: Listen failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Starting Bazarly Server on http://localhost:18080..." << std::endl;
    std::cout << "Automatically launching the frontend in your browser..." << std::endl;
    system("start ..\\frontend\\index.html");

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            CreateThread(NULL, 0, ClientHandler, (LPVOID)clientSocket, 0, NULL);
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    DeleteCriticalSection(&products_mutex);
    DeleteCriticalSection(&users_mutex);
    DeleteCriticalSection(&orders_mutex);
    return 0;
}
