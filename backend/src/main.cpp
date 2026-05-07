#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#define ASIO_STANDALONE
#include "../inlcude/asio.hpp"
#include "../inlcude/core/Product.h"
#include "../inlcude/core/User.h"
#include "../inlcude/core/Customer.h"
#include "../inlcude/core/Order.h"
#include "../inlcude/core/OrderService.h"
#include "../inlcude/core/ShopkeeperService.h"

using namespace std;
using asio::ip::tcp;

// --- CUSTOM PLAIN-TEXT PARSER ---
vector<string> split_string(const string& str, char delim) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}


ShopkeeperService globalShopService;
CRITICAL_SECTION products_mutex;

vector<User*> users;
CRITICAL_SECTION users_mutex;

OrderService globalOrderService;
CRITICAL_SECTION orders_mutex;


string make_response(int status_code, const string& status_text, const string& body) {
    string response = "HTTP/1.1 " + to_string(status_code) + " " + status_text + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    response += "Access-Control-Allow-Headers: Content-Type, Authorization\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: " + to_string(body.length()) + "\r\n";
    response += "\r\n";
    response += body;
    return response;
}

string handle_request(const string& method, const string& path, const string& body) {
    if (method == "OPTIONS") {
        return make_response(204, "No Content", "");
    }

    if (method == "GET" && path == "/products") {
        EnterCriticalSection(&products_mutex);
        string res_body = "";
        const auto& inventory = globalShopService.getInventory();
        for (size_t i = 0; i < inventory.size(); ++i) {
            auto* p = inventory[i];
            // Format: sku;name;price;stock;owner
            res_body += p->getSku() + ";" + p->getName() + ";" + 
                        to_string(p->getPrice()) + ";" + 
                        to_string(p->getStock()) + ";" + p->getOwner();
            if (i < inventory.size() - 1) res_body += "|";
        }
        LeaveCriticalSection(&products_mutex);
        return make_response(200, "OK", res_body);
    }
    
    if (method == "POST" && path == "/register") {
        vector<string> parts = split_string(body, ';');
        if (parts.size() < 3) return make_response(400, "Bad Request", "ERROR");
        
        string username = parts[0];
        string password = parts[1];
        string role = parts[2];
        
        EnterCriticalSection(&users_mutex);
        User* newUser;
        if (role == "customer") {
            newUser = new Customer(username, username + "@example.com", password);
        } else {
            newUser = new User(username, username + "@example.com", password);
        }
        newUser->setRole(role);
        users.push_back(newUser);
        LeaveCriticalSection(&users_mutex);
        
        return make_response(200, "OK", "SUCCESS");
    }

    if (method == "POST" && path == "/login") {
        vector<string> parts = split_string(body, ';');
        if (parts.size() < 2) return make_response(400, "Bad Request", "ERROR");
        
        string username = parts[0];
        string password = parts[1];
        
        EnterCriticalSection(&users_mutex);
        string token = "";
        string role = "";
        for (auto* u : users) {
            if (u->getName() == username && u->getPassword() == password) {
                u->setToken("token_" + u->getName());
                token = u->getToken();
                role = u->getRole();
                break;
            }
        }
        LeaveCriticalSection(&users_mutex);
        
        if (token != "") {
            return make_response(200, "OK", "SUCCESS;" + token + ";" + role);
        } else {
            return make_response(401, "Unauthorized", "ERROR");
        }
    }

    if (method == "POST" && path == "/checkout") {
        vector<string> parts = split_string(body, ';');
        if (parts.size() < 3) return make_response(400, "Bad Request", "ERROR");
        
        string token = parts[0];
        // parts[1] is total
        vector<string> skus = split_string(parts[2], ',');
        
        EnterCriticalSection(&products_mutex);
        if (skus.size() > 0) {
            for (const auto& sku_str : skus) {
                if (sku_str.empty()) continue;
                for (auto* p : globalShopService.getInventory()) {
                    if (p->getSku() == sku_str && p->getStock() > 0) {
                        p->reduceStock(1);
                        break;
                    }
                }
            }
        }
        LeaveCriticalSection(&products_mutex);

        EnterCriticalSection(&orders_mutex);
        Order* newOrder = new Order("O" + to_string(globalOrderService.getActiveOrders().size()+1), token);
        
        Customer* buyingCustomer = nullptr;
        EnterCriticalSection(&users_mutex);
        for(auto* u : users) {
            if(u->getToken() == token) { buyingCustomer = dynamic_cast<Customer*>(u); break; }
        }
        LeaveCriticalSection(&users_mutex);

        if (!buyingCustomer) {
            delete newOrder;
            LeaveCriticalSection(&orders_mutex);
            return make_response(401, "Unauthorized", "ERROR");
        }
        
        globalOrderService.createOrder(newOrder, buyingCustomer);
        string created_order_id = newOrder->getOrderId();
        LeaveCriticalSection(&orders_mutex);
        
        return make_response(200, "OK", "SUCCESS;" + created_order_id);
    }

    if (method == "POST" && path == "/products") {
        vector<string> parts = split_string(body, ';');
        if (parts.size() < 3) return make_response(400, "Bad Request", "ERROR");
        
        string token = parts[0];
        string name = parts[1];
        double price = stod(parts[2]);
        
        bool is_shopkeeper = false;
        string shopkeeper_username = "";
        EnterCriticalSection(&users_mutex);
        for (auto* u : users) {
            if (u->getToken() == token && u->getRole() == "shopkeeper") {
                is_shopkeeper = true;
                shopkeeper_username = u->getName();
                break;
            }
        }
        LeaveCriticalSection(&users_mutex);

        if (!is_shopkeeper) {
            return make_response(403, "Forbidden", "ERROR");
        }

        EnterCriticalSection(&products_mutex);
        string sku = "SKU" + to_string(globalShopService.getInventory().size() + 1);
        Product* newProduct = new Product(sku, name, price, 100, shopkeeper_username);
        globalShopService.addProductToInventory(newProduct);
        LeaveCriticalSection(&products_mutex);

        return make_response(200, "OK", "SUCCESS;" + sku);
    }

    return make_response(404, "Not Found", "{\"error\": \"Not Found\"}");
}

DWORD WINAPI handle_session(LPVOID lpParam) {
    tcp::socket* socket = (tcp::socket*)lpParam;
    try {
        char buffer[4096];
        asio::error_code error;
        size_t length = socket->read_some(asio::buffer(buffer), error);
        
        if (error == asio::error::eof) {
            delete socket;
            return 0;
        } else if (error) {
            throw asio::system_error(error);
        }

        if (length > 0) {
            string request(buffer, length);
            
            size_t first_space = request.find(' ');
            size_t second_space = request.find(' ', first_space + 1);
            if (first_space != string::npos && second_space != string::npos) {
                string method = request.substr(0, first_space);
                string path = request.substr(first_space + 1, second_space - first_space - 1);
                
                string body = "";
                size_t body_pos = request.find("\r\n\r\n");
                if (body_pos != string::npos) {
                    body = request.substr(body_pos + 4);
                }

                string response = handle_request(method, path, body);
                asio::write(*socket, asio::buffer(response));
            }
        }
    } catch (exception& e) {
        cerr << "Exception in thread: " << e.what() << "\n";
    }
    delete socket;
    return 0;
}

int main() {
    InitializeCriticalSection(&products_mutex);
    InitializeCriticalSection(&users_mutex);
    InitializeCriticalSection(&orders_mutex);

    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 18080));

        cout << "Starting Bazarly Server with ASIO on http://localhost:18080..." << endl;
        cout << "Automatically launching the frontend in your browser..." << endl;
        system("start ..\\frontend\\index.html");

        while (true) {
            tcp::socket* socket = new tcp::socket(io_context);
            acceptor.accept(*socket);
            CreateThread(NULL, 0, handle_session, (LPVOID)socket, 0, NULL);
        }
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    DeleteCriticalSection(&products_mutex);
    DeleteCriticalSection(&users_mutex);
    DeleteCriticalSection(&orders_mutex);
    return 0;
}
