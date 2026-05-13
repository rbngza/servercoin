#include "screen_loading.h"
#include "ui.h"

static lv_obj_t *_screen;
static lv_obj_t *_status_label;

lv_obj_t *screen_loading_create() {
    _screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(_screen, lv_color_hex(C_BG), 0);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t *title = lv_label_create(_screen);
    lv_label_set_text(title, "NEXUS");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(C_CYAN), 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -65);

    lv_obj_t *sub = lv_label_create(_screen);
    lv_label_set_text(sub, "MONITOR");
    lv_obj_set_style_text_font(sub, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(sub, lv_color_hex(C_MUTED), 0);
    lv_obj_align(sub, LV_ALIGN_CENTER, 0, -40);

    // Animated spinner (60° arc, 1 s rotation)
    lv_obj_t *spinner = lv_spinner_create(_screen, 1000, 60);
    lv_obj_set_size(spinner, 70, 70);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 8);
    lv_obj_set_style_arc_color(spinner, lv_color_hex(C_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(spinner, lv_color_hex(C_CARD), LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner, 6, LV_PART_MAIN);

    // Status text below spinner
    _status_label = lv_label_create(_screen);
    lv_label_set_text(_status_label, "Connecting...");
    lv_obj_set_style_text_font(_status_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_status_label, lv_color_hex(C_MUTED), 0);
    lv_obj_align(_status_label, LV_ALIGN_CENTER, 0, 62);

    return _screen;
}

void screen_loading_set_status(const char *msg) {
    if (_status_label) lv_label_set_text(_status_label, msg);
}
