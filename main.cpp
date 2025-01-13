#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <sstream>


size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string sendRequest(const std::string& url, const Json::Value& payload, const std::string& accessToken = "") {
    std::string readBuffer;
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

    
        Json::StreamWriterBuilder writer;
        std::string jsonStr = Json::writeString(writer, payload);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

        // Set headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!accessToken.empty()) {
            headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

       
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }

      
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return readBuffer;
}


std::string getAccessToken(const std::string& clientId, const std::string& clientSecret) {
    Json::Value payload;
    payload["id"] = 0;
    payload["method"] = "public/auth";
    payload["params"]["grant_type"] = "client_credentials";
    payload["params"]["scope"] = "session:apiconsole-c5i26ds6dsr expires:2592000";
    payload["params"]["client_id"] = clientId;
    payload["params"]["client_secret"] = clientSecret;
    payload["jsonrpc"] = "2.0";

    std::string response = sendRequest("https://test.deribit.com/api/v2/public/auth", payload);

    Json::CharReaderBuilder reader;
    Json::Value responseJson;
    std::string errs;

    std::istringstream stream(response);
    if (!Json::parseFromStream(reader, stream, &responseJson, &errs)) {
        std::cerr << "Failed to parse JSON response: " << errs << std::endl;
        return "";
    }

    if (responseJson.isMember("result") && responseJson["result"].isMember("access_token")) {
        return responseJson["result"]["access_token"].asString();
    } else {
        std::cerr << "Failed to retrieve access token." << std::endl;
        return "";
    }
}

void placeOrder(const std::string& price, const std::string& accessToken, const std::string& amount, const std::string& instrument) {
    Json::Value payload;
    payload["jsonrpc"] = "2.0";
    payload["method"] = "private/buy";
    payload["params"]["instrument_name"] = instrument;
    payload["params"]["type"] = "limit";
    payload["params"]["price"] = price;
    payload["params"]["amount"] = amount;
    payload["id"] = 1;

    std::string response = sendRequest("https://test.deribit.com/api/v2/private/buy", payload, accessToken);
    std::cout << "Place Order Response: " << response << std::endl;
}


void cancelOrder(const std::string& accessToken, const std::string& orderID) {
    Json::Value payload;
    payload["jsonrpc"] = "2.0";
    payload["method"] = "private/cancel";
    payload["params"]["order_id"] = orderID;
    payload["id"] = 6;

    std::string response = sendRequest("https://test.deribit.com/api/v2/private/cancel", payload, accessToken);
    std::cout << "Cancel Order Response: " << response << std::endl;
}

// Function to fetch account balance
void getAccountBalance(const std::string& accessToken) {
    Json::Value payload;
    payload["jsonrpc"] = "2.0";
    payload["method"] = "private/get_account_summary";
    payload["params"]["currency"] = "BTC";
    payload["id"] = 7;

    std::string response = sendRequest("https://test.deribit.com/api/v2/private/get_account_summary", payload, accessToken);
    std::cout << "Account Balance Response: " << response << std::endl;
}

// Function to fetch order book
void getOrderBook(const std::string& instrument) {
    Json::Value payload;
    payload["jsonrpc"] = "2.0";
    payload["method"] = "public/get_order_book";
    payload["params"]["instrument_name"] = instrument;
    payload["id"] = 8;

    std::string response = sendRequest("https://test.deribit.com/api/v2/public/get_order_book", payload);
    std::cout << "Order Book Response: " << response << std::endl;
}

// Main function
int main() {
    // Replace with your actual client credentials
    std::string clientId = "bmJhkj6X";
    std::string clientSecret = "FxdxhVWYr_ka6JqAwVIy59JRJptukCFiOtKYOg286KE";

    // Retrieve the access token
    std::string accessToken = getAccessToken(clientId, clientSecret);

    if (!accessToken.empty()) {
        // Example operations
        placeOrder("50000", accessToken, "10", "ETH-PERPETUAL");
        cancelOrder(accessToken, "29257473891");
        getAccountBalance(accessToken);
        getOrderBook("ETH-PERPETUAL");
    } else {
        std::cerr << "Unable to obtain access token, aborting operations." << std::endl;
    }

    return 0;
}
