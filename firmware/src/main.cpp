#include <Arduino.h>
#include <lvgl.h>

#include "config.h"
#include "wifi_manager.h"
#include "sse_client.h"
#include "wol.h"
#include "drivers/display.h"
#include "ui/ui.h"
#include "ui/screen_loading.h"

static uint32_t lastTickUpdate = 0;
static uint32_t lastProcPoll   = 0;

// Pump LVGL for a given number of milliseconds, keeping tick accurate.
static void lvgl_delay(uint32_t ms) {
    uint32_t deadline = millis() + ms;
    while (millis() < deadline) {
        uint32_t now = millis();
        lv_tick_inc(now - lastTickUpdate);
        lastTickUpdate = now;
        lv_timer_handler();
        delay(5);
    }
}

void setup() {
    Serial.begin(115200);

    display_init();   // GC9A01 + LVGL + touch — must come first

    // Show loading screen immediately so user sees something
    lv_obj_t *loading = screen_loading_create();
    lv_scr_load(loading);
    lastTickUpdate = millis();
    lv_timer_handler();  // render the loading screen before blocking

    wifi_start();  // non-blocking — returns immediately

    // Animate spinner while waiting for WiFi (up to 12 s)
    uint32_t deadline = millis() + 12000;
    while (!wifi_connected() && millis() < deadline) {
        uint32_t now = millis();
        lv_tick_inc(now - lastTickUpdate);
        lastTickUpdate = now;
        lv_timer_handler();
        delay(10);
    }

    if (wifi_connected()) {
        screen_loading_set_status("Connected!");
        lvgl_delay(400);
        sse_begin();
        screen_loading_set_status("Fetching data...");
    } else {
        screen_loading_set_status("No WiFi — check config.h");
        lvgl_delay(2000);
    }

    // Wait for first SSE frame (up to 8 s) so home screen shows live data
    deadline = millis() + 8000;
    while (!g_metrics.valid && millis() < deadline) {
        uint32_t now = millis();
        lv_tick_inc(now - lastTickUpdate);
        lastTickUpdate = now;
        lv_timer_handler();
        sse_tick();
        delay(10);
    }

    ui_init();  // build all 5 screens and load home screen
    lastTickUpdate = millis();  // reset tick so loop() doesn't get a huge jump
}

void loop() {
    uint32_t now = millis();
    lv_tick_inc(now - lastTickUpdate);
    lastTickUpdate = now;

    lv_timer_handler();
    wifi_tick();

    if (wifi_connected()) {
        bool fresh = sse_tick();
        if (fresh) ui_update(g_metrics);

        if (now - lastProcPoll >= 5000) {
            lastProcPoll = now;
            fetch_processes();
            ui_update_processes(g_processes, g_process_count);
        }
    }
}
