#include <Arduino.h>

#ifndef CONF_H
#define CONF_H
namespace Conf {
char *getSSID();
char *getPASSWORD();
char *getALPHA_VANTAGE_KEY();

void loadConfig();

// The file that contains the tickers of stocks that the user wants to be
// tracked
void loadStocksFile();
} // namespace Conf
#endif