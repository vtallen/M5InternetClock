#include <Arduino.h>

#ifndef CONF_H
#define CONF_H
namespace Conf {
char *getSSID();
char *getPASSWORD();
char *getALPHA_VANTAGE_KEY();
void loadConfig();
} // namespace Conf
#endif