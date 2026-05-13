#include "wol.h"
#include <WiFiUdp.h>
#include <Arduino.h>

void wol_send(const char *mac_str) {
    uint8_t mac[6];
    sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

    uint8_t packet[102];
    for (int i = 0; i < 6; i++) packet[i] = 0xFF;
    for (int i = 1; i <= 16; i++) memcpy(&packet[i * 6], mac, 6);

    WiFiUDP udp;
    udp.begin(9);
    udp.beginPacket("255.255.255.255", 9);
    udp.write(packet, sizeof(packet));
    udp.endPacket();
    udp.stop();

    Serial.println("WoL magic packet sent");
}
