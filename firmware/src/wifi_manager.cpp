#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>

static uint32_t _lastReconnect = 0;

void wifi_start() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("WiFi: connecting (non-blocking)...");
}

void wifi_init() {
    wifi_start();
    uint32_t t = millis();
    while (!WiFi.isConnected() && millis() - t < 10000) {
        delay(500);
        Serial.print('.');
    }
    Serial.println();
    if (WiFi.isConnected())
        Serial.printf("WiFi OK  IP: %s\n", WiFi.localIP().toString().c_str());
    else
        Serial.println("WiFi timeout — will retry in loop");
}

void wifi_tick() {
    if (!WiFi.isConnected()) {
        uint32_t now = millis();
        if (now - _lastReconnect >= 5000) {
            _lastReconnect = now;
            WiFi.reconnect();
        }
    }
}

bool wifi_connected() {
    return WiFi.isConnected();
}

String wifi_ip() {
    return WiFi.localIP().toString();
}
