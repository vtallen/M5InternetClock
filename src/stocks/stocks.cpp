#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

namespace Stocks
{
    StaticJsonDocument<2000> getStockQuote(const char *alpha_vantage_key, const char *stock_symbol)
    {

        HTTPClient http;
        String url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" +
                     String(stock_symbol) +
                     "&apikey=" + String(alpha_vantage_key);

        http.begin(url);
        int httpCode = http.GET();

        // Parse the JSON response
        StaticJsonDocument<2000> doc;
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            deserializeJson(doc, payload);
        }
        else
        {
            Serial.println("Error getting stock data from Alpha Vantage API");
        }

        http.end();

        return doc;
    }

    StaticJsonDocument<2000> getStockOverview(const char *alpha_vantage_key, const char *stock_symbol)
    {

        HTTPClient http;
        String url = "https://www.alphavantage.co/query?function=OVERVIEW&symbol=" +
                     String(stock_symbol) +
                     "&apikey=" + String(alpha_vantage_key);

        http.begin(url);
        int httpCode = http.GET();

        // Parse the JSON response
        StaticJsonDocument<2000> doc;
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            deserializeJson(doc, payload);
        }
        else
        {
            Serial.println("Error getting stock data from Alpha Vantage API");
        }

        http.end();

        return doc;
    }

    String getName(const char *alpha_vantage_key, const char *stock_symbol)
    {
        StaticJsonDocument<2000> jsonDoc{getStockOverview(alpha_vantage_key, stock_symbol)};

        String name = jsonDoc["Name"];

        return name;
    }

    String getName(StaticJsonDocument<2000> jsonDoc)
    {
        String name = jsonDoc["Name"];
        return name;
    }
}