#include "screen_network.h"
#include "ui.h"
#include <stdio.h>

// Arc scale: 100 % = NET_MAX_KBS. Raise if your link is faster than 10 MB/s.
#define NET_MAX_KBS 10240.0f

static lv_obj_t *_screen;
static lv_obj_t *_up_arc;
static lv_obj_t *_dn_arc;
static lv_obj_t *_up_val;
static lv_obj_t *_dn_val;

static void make_net_arc(lv_obj_t *parent, lv_obj_t **out,
                         lv_color_t col, lv_align_t align, int16_t x_ofs) {
    lv_obj_t *a = lv_arc_create(parent);
    lv_obj_set_size(a, 92, 92);
    lv_arc_set_rotation(a, 135);
    lv_arc_set_bg_angles(a, 0, 270);
    lv_arc_set_range(a, 0, 100);
    lv_arc_set_value(a, 0);
    lv_obj_remove_style(a, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(a, lv_color_hex(C_CARD), LV_PART_MAIN);
    lv_obj_set_style_arc_color(a, col, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(a, 10, LV_PART_MAIN);
    lv_obj_set_style_arc_width(a, 10, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(a, LV_OPA_TRANSP, 0);
    lv_obj_align(a, align, x_ofs, 12);
    lv_obj_clear_flag(a, LV_OBJ_FLAG_CLICKABLE);
    *out = a;
}

static void format_throughput(char *buf, size_t len, float kbs) {
    if (kbs >= 1024.0f)
        snprintf(buf, len, "%.1f MB/s", (double)(kbs / 1024.0f));
    else
        snprintf(buf, len, "%.0f KB/s", (double)kbs);
}

lv_obj_t *screen_network_create() {
    _screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(_screen, lv_color_hex(C_BG), 0);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t *title = lv_label_create(_screen);
    lv_label_set_text(title, "NETWORK");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(C_PURPLE), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 24);

    // Upload arc (left, purple) — icon + value as children
    make_net_arc(_screen, &_up_arc, lv_color_hex(C_PURPLE), LV_ALIGN_CENTER, -55);

    lv_obj_t *up_icon = lv_label_create(_up_arc);
    lv_label_set_text(up_icon, LV_SYMBOL_UP);
    lv_obj_set_style_text_color(up_icon, lv_color_hex(C_PURPLE), 0);
    lv_obj_set_style_text_font(up_icon, &lv_font_montserrat_12, 0);
    lv_obj_align(up_icon, LV_ALIGN_CENTER, 0, -8);

    _up_val = lv_label_create(_up_arc);
    lv_label_set_text(_up_val, "0 KB/s");
    lv_obj_set_style_text_font(_up_val, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_up_val, lv_color_hex(C_WHITE), 0);
    lv_obj_set_style_text_align(_up_val, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(_up_val, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(_up_val, 80);
    lv_obj_align(_up_val, LV_ALIGN_CENTER, 0, 8);

    // Download arc (right, cyan) — icon + value as children
    make_net_arc(_screen, &_dn_arc, lv_color_hex(C_CYAN), LV_ALIGN_CENTER, 55);

    lv_obj_t *dn_icon = lv_label_create(_dn_arc);
    lv_label_set_text(dn_icon, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_color(dn_icon, lv_color_hex(C_CYAN), 0);
    lv_obj_set_style_text_font(dn_icon, &lv_font_montserrat_12, 0);
    lv_obj_align(dn_icon, LV_ALIGN_CENTER, 0, -8);

    _dn_val = lv_label_create(_dn_arc);
    lv_label_set_text(_dn_val, "0 KB/s");
    lv_obj_set_style_text_font(_dn_val, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_dn_val, lv_color_hex(C_WHITE), 0);
    lv_obj_set_style_text_align(_dn_val, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(_dn_val, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(_dn_val, 80);
    lv_obj_align(_dn_val, LV_ALIGN_CENTER, 0, 8);

    ui_draw_nav_dots(_screen, 3, lv_color_hex(C_PURPLE));
    return _screen;
}

void screen_network_update(float sent_kbs, float recv_kbs) {
    int up_pct = (int)((sent_kbs / NET_MAX_KBS) * 100.0f);
    int dn_pct = (int)((recv_kbs / NET_MAX_KBS) * 100.0f);
    if (up_pct > 100) up_pct = 100;
    if (dn_pct > 100) dn_pct = 100;

    lv_arc_set_value(_up_arc, up_pct);
    lv_arc_set_value(_dn_arc, dn_pct);

    char buf[16];
    format_throughput(buf, sizeof(buf), sent_kbs);
    lv_label_set_text(_up_val, buf);
    format_throughput(buf, sizeof(buf), recv_kbs);
    lv_label_set_text(_dn_val, buf);
}
