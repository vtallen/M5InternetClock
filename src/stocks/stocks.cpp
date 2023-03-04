#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <M5Core2.h>
#include <WiFi.h>
#include <vector>

#include <conf_parser.h>

namespace Stocks {
char *ALPHA_VANTAGE_KEY;
std::vector<String> STOCK_TICKERS;

void loadStocksFile() {
  File file = SD.open("/Stocks.txt");
  if (file) {
    while (file.available()) {
      String c = file.readStringUntil('\n');
      STOCK_TICKERS.push_back(c);
      Serial.println("Loaded stock " + c);
    }
  } else {
    Serial.println("Failed to open Stocks.txt.");
  }

  file.close();
}

// Do not call before the config file has been fully read and parsed, it will
// segfault/be caught by the assert statement
void init() {
  ALPHA_VANTAGE_KEY = Conf::getALPHA_VANTAGE_KEY();
  loadStocksFile();
}

std::vector<String> &getSTOCK_TICKERS() { return STOCK_TICKERS; }

StaticJsonDocument<2000> getStockQuote(const char *stock_symbol) {

  HTTPClient http;
  String url =
      "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" +
      String(stock_symbol) + "&apikey=" + String(ALPHA_VANTAGE_KEY);

  http.begin(url);
  int httpCode = http.GET();

  // Parse the JSON response
  StaticJsonDocument<2000> doc;
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    deserializeJson(doc, payload);
  } else {
    Serial.println("Error getting stock data from Alpha Vantage API");
  }

  http.end();

  return doc;
}

StaticJsonDocument<2000> getStockOverview(const char *stock_symbol) {

  HTTPClient http;
  String url = "https://www.alphavantage.co/query?function=OVERVIEW&symbol=" +
               String(stock_symbol) + "&apikey=" + String(ALPHA_VANTAGE_KEY);

  http.begin(url);
  int httpCode = http.GET();

  // Parse the JSON response
  StaticJsonDocument<2000> doc;
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    deserializeJson(doc, payload);
  } else {
    Serial.println("Error getting stock data from Alpha Vantage API");
  }

  http.end();

  return doc;
}

String getName(const char *stock_symbol) {
  StaticJsonDocument<2000> jsonDoc{getStockOverview(stock_symbol)};

  String name = jsonDoc["Name"];

  return name;
}

String getName(StaticJsonDocument<2000> jsonDoc) {
  String name = jsonDoc["Name"];
  return name;
}

String getQuoteString(const char *stock_symbol) {
  StaticJsonDocument<2000> jsonDoc{getStockQuote(stock_symbol)};

  String quote = jsonDoc["Global Quote"]["05. price"];

  float price = quote.toFloat();

  return String(price, 2);
}

double getQuote(const char *stock_symbol) {
  return std::stod(getQuoteString(stock_symbol).c_str());
}

} // namespace Stocks