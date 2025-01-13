
Deribit API Integration
This project demonstrates how to interact with the Deribit API for cryptocurrency trading. It provides functionalities to authenticate, place orders, cancel orders, retrieve the order book, and view account balances using C++, libcurl, and jsoncpp.

Features
Authenticate and obtain an access token.
Place limit orders for cryptocurrency trading.
Cancel existing orders.
Retrieve the order book for a specific instrument.
View account balance details.
Requirements
Before running the project, ensure you have the following:

Dependencies
libcurl
jsoncpp
API Credentials
Obtain your Client ID and Client Secret from the Deribit API portal.
Setup
1. Clone the Repository
bash
Copy code
git clone <repository_url>
cd <repository_directory>
2. Install Dependencies
Linux
Install libcurl and jsoncpp:

bash
Copy code
sudo apt update
sudo apt install libcurl4-openssl-dev libjsoncpp-dev
Windows
Download and install libcurl.
Download and build jsoncpp.
3. Compile the Code
Use the following command to compile the program:

bash
Copy code
g++ main.cpp -o DeribitAPI -lcurl -ljsoncpp
Usage
1. Authentication
Replace the placeholders in the code with your Client ID and Client Secret:

cpp
Copy code
std::string clientId = "your_client_id";
std::string clientSecret = "your_client_secret";
Run the program to fetch the access token:

bash
Copy code
./DeribitAPI
2. Available Operations
Place Order: Submit a limit order for a specific instrument.
Cancel Order: Cancel an existing order using the order ID.
Retrieve Order Book: Fetch the order book for a specific instrument.
Get Account Balance: Retrieve account balance details.
API Methods
Authentication
Endpoint: https://test.deribit.com/api/v2/public/auth
Method: POST
Payload:
json
Copy code
{
  "jsonrpc": "2.0",
  "method": "public/auth",
  "params": {
    "grant_type": "client_credentials",
    "client_id": "<client_id>",
    "client_secret": "<client_secret>"
  },
  "id": 0
}
Place Order
Endpoint: https://test.deribit.com/api/v2/private/buy
Method: POST
Payload:
json
Copy code
{
  "jsonrpc": "2.0",
  "method": "private/buy",
  "params": {
    "instrument_name": "ETH-PERPETUAL",
    "type": "limit",
    "price": "50000",
    "amount": "10"
  },
  "id": 1
}
Cancel Order
Endpoint: https://test.deribit.com/api/v2/private/cancel
Method: POST
Payload:
json
Copy code
{
  "jsonrpc": "2.0",
  "method": "private/cancel",
  "params": {
    "order_id": "29257473891"
  },
  "id": 6
}
Code Structure
main.cpp: The main entry point of the application containing all functionality.
Functions:
getAccessToken: Authenticate and retrieve the access token.
placeOrder: Place a limit order.
cancelOrder: Cancel an existing order.
getOrderBook: Retrieve the order book for an instrument.
getBalance: Retrieve account balance details.
Sample Output
Example output when running the program:

plaintext
Copy code
Access Token: abc123xyz

Placing Order...
Place Order Response: {"jsonrpc":"2.0","result":{"order_id":"123456"}}

Cancelling Order...
Cancel Order Response: {"jsonrpc":"2.0","result":"success"}

Fetching Order Book...
Best Bid Price: 50000, Amount: 10
Best Ask Price: 50010, Amount: 5

Fetching Account Balance...
Equity: 1.234 BTC
Available Margin: 0.987 BTC
Contributing
Contributions are welcome! If you find any issues or want to enhance the project, feel free to submit a pull request.

License
This project is licensed under the MIT License.

References
Deribit API Documentation
libcurl Documentation
jsoncpp Documentation
