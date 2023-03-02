#include <Arduino.h>

namespace Stocks {
void init();
String getName(const char *stock_symbol);
String getQuoteString(const char *stock_symbol);
double getQuote(const char *stock_symbol);
} // namespace Stocks