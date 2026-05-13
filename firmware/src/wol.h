#pragma once

/** Send a Wake-on-LAN magic packet to the given MAC address string (e.g. "AA:BB:CC:DD:EE:FF"). */
void wol_send(const char *mac_str);
