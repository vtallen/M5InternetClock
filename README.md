# M5InternetClock
---
An internet clock application for the M5Stack Core2 ESP32 micro controler. It features an LCD clock style with a stock price tracking widget.



# Usage
---
Place M5Clock.txt on the root of your sd card containing the WIFI SSID and password as well as your Alpha Vantage API key and time offset in seconds from UTC
```
SSID = SSID_HERE
PASSWORD = PASSWORD_HERE
ALPHA_VANTAGE_KEY = API_KEY_HERE
TIME_OFFSET_UTC_SECONDS = -18000
```

Stocks.txt contains a list of all the stocks you would like to rotate. Each ticker should be placed on it's own line.
```
VTI
VOO
VYM
```

