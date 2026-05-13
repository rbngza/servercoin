#include "screen_home.h"
#include "ui.h"
#include "../config.h"
#include "../wol.h"
#include "../wifi_manager.h"

static lv_obj_t *_screen;
static lv_obj_t *_status_dot;
static lv_obj_t *_status_label;
static lv_obj_t *_uptime_label;
static lv_obj_t *_ip_label;
static lv_obj_t *_wol_btn;

static void format_uptime(char *buf, size_t len, uint32_t s) {
    uint32_t d = s / 86400;
    uint32_t h = (s % 86400) / 3600;
    uint32_t m = (s % 3600) / 60;
    if      (d > 0) snprintf(buf, len, "%lud %luh %lum", (unsigned long)d, (unsigned long)h, (unsigned long)m);
    else if (h > 0) snprintf(buf, len, "%luh %lum",      (unsigned long)h, (unsigned long)m);
    else             snprintf(buf, len, "%lum",           (unsigned long)m);
}

static void wol_btn_cb(lv_event_t *) {
    wol_send(WOL_MAC);
}

lv_obj_t *screen_home_create() {
    _screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(_screen, lv_color_hex(C_BG), 0);
    lv_obj_set_style_bg_opa(_screen, LV_OPA_COVER, 0);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    // Server name
    lv_obj_t *name = lv_label_create(_screen);
    lv_label_set_text(name, "NEXUS");
    lv_obj_set_style_text_font(name, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(name, lv_color_hex(C_CYAN), 0);
    lv_obj_align(name, LV_ALIGN_TOP_MID, 0, 32);

    // Decorative divider line
    lv_obj_t *line = lv_obj_create(_screen);
    lv_obj_set_size(line, 80, 2);
    lv_obj_set_style_bg_color(line, lv_color_hex(C_CARD), 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_set_style_radius(line, 1, 0);
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 60);

    // Status dot
    _status_dot = lv_obj_create(_screen);
    lv_obj_set_size(_status_dot, 12, 12);
    lv_obj_set_style_radius(_status_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(_status_dot, 0, 0);
    lv_obj_set_style_bg_color(_status_dot, lv_color_hex(C_MUTED), 0);
    lv_obj_align(_status_dot, LV_ALIGN_CENTER, -38, -28);

    // Status text
    _status_label = lv_label_create(_screen);
    lv_label_set_text(_status_label, "OFFLINE");
    lv_obj_set_style_text_font(_status_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_status_label, lv_color_hex(C_MUTED), 0);
    lv_obj_align(_status_label, LV_ALIGN_CENTER, 16, -28);

    // Uptime
    _uptime_label = lv_label_create(_screen);
    lv_label_set_text(_uptime_label, "Uptime: --");
    lv_obj_set_style_text_font(_uptime_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_uptime_label, lv_color_hex(C_MUTED), 0);
    lv_obj_align(_uptime_label, LV_ALIGN_CENTER, 0, 0);

    // IP label
    _ip_label = lv_label_create(_screen);
    lv_label_set_text(_ip_label, "IP: --");
    lv_obj_set_style_text_font(_ip_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_ip_label, lv_color_hex(C_MUTED), 0);
    lv_obj_align(_ip_label, LV_ALIGN_CENTER, 0, 20);

    // Wake-on-LAN button — visible by default, hidden only when server is online
    _wol_btn = lv_btn_create(_screen);
    lv_obj_set_size(_wol_btn, 150, 38);
    lv_obj_align(_wol_btn, LV_ALIGN_CENTER, 0, 60);
    lv_obj_set_style_bg_color(_wol_btn, lv_color_hex(C_CYAN), 0);
    lv_obj_set_style_radius(_wol_btn, 19, 0);
    lv_obj_add_event_cb(_wol_btn, wol_btn_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *wol_lbl = lv_label_create(_wol_btn);
    lv_label_set_text(wol_lbl, LV_SYMBOL_POWER "  WAKE SERVER");
    lv_obj_set_style_text_color(wol_lbl, lv_color_hex(C_BG), 0);
    lv_obj_set_style_text_font(wol_lbl, &lv_font_montserrat_12, 0);
    lv_obj_center(wol_lbl);

    ui_draw_nav_dots(_screen, 0, lv_color_hex(C_CYAN));
    return _screen;
}

void screen_home_update(const Metrics &m) {
    if (m.online) {
        lv_obj_set_style_bg_color(_status_dot, lv_color_hex(C_GREEN), 0);
        lv_label_set_text(_status_label, "ONLINE");
        lv_obj_set_style_text_color(_status_label, lv_color_hex(C_GREEN), 0);
        lv_obj_add_flag(_wol_btn, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_set_style_bg_color(_status_dot, lv_color_hex(C_RED), 0);
        lv_label_set_text(_status_label, "OFFLINE");
        lv_obj_set_style_text_color(_status_label, lv_color_hex(C_RED), 0);
        lv_obj_clear_flag(_wol_btn, LV_OBJ_FLAG_HIDDEN);
    }

    char uptime_buf[32];
    format_uptime(uptime_buf, sizeof(uptime_buf), m.uptime_s);
    lv_label_set_text_fmt(_uptime_label, "Uptime: %s", uptime_buf);
    lv_label_set_text_fmt(_ip_label, "IP: %s", wifi_ip().c_str());
}
