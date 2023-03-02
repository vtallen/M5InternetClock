#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include <conf_parser.h>

namespace Stocks {
char *ALPHA_VANTAGE_KEY;

void init() { ALPHA_VANTAGE_KEY = Conf::getALPHA_VANTAGE_KEY(); }

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

  return quote;
}

double getQuote(const char *stock_symbol) {
  return std::stod(getQuoteString(stock_symbol).c_str());
}

} // namespace Stocks