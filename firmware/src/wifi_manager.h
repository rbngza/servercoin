#pragma once
#include <Arduino.h>

/** Non-blocking: starts the WiFi connection and returns immediately. */
void   wifi_start();
/** Legacy blocking init (kept for compatibility). */
void   wifi_init();
void   wifi_tick();
bool   wifi_connected();
String wifi_ip();
