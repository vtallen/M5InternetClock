#include <Arduino.h>

#ifndef CONF_H
#define CONF_H
namespace Conf {
char *getSSID();
char *getPASSWORD();
char *getALPHA_VANTAGE_KEY();
int *getTIME_OFFSET();

void loadConfig();

} // namespace Conf
#endif