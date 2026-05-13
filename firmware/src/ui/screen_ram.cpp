#include "screen_ram.h"
#include "ui.h"

static lv_obj_t *_screen;
static lv_obj_t *_arc;
static lv_obj_t *_pct_label;   // child of _arc
static lv_obj_t *_gb_label;    // child of _arc

lv_obj_t *screen_ram_create() {
    _screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(_screen, lv_color_hex(C_BG), 0);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t *title = lv_label_create(_screen);
    lv_label_set_text(title, "RAM");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(C_ORANGE), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 24);

    // Arc gauge
    _arc = lv_arc_create(_screen);
    lv_obj_set_size(_arc, 148, 148);
    lv_arc_set_rotation(_arc, 135);
    lv_arc_set_bg_angles(_arc, 0, 270);
    lv_arc_set_range(_arc, 0, 100);
    lv_arc_set_value(_arc, 0);
    lv_obj_remove_style(_arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(_arc, lv_color_hex(C_CARD), LV_PART_MAIN);
    lv_obj_set_style_arc_color(_arc, lv_color_hex(C_ORANGE), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(_arc, 14, LV_PART_MAIN);
    lv_obj_set_style_arc_width(_arc, 14, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(_arc, LV_OPA_TRANSP, 0);
    lv_obj_align(_arc, LV_ALIGN_CENTER, 0, -10);
    lv_obj_clear_flag(_arc, LV_OBJ_FLAG_CLICKABLE);

    // Percentage — child of arc, centred, then offset up slightly
    _pct_label = lv_label_create(_arc);
    lv_label_set_text(_pct_label, "0%");
    lv_obj_set_style_text_font(_pct_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(_pct_label, lv_color_hex(C_ORANGE), 0);
    lv_obj_align(_pct_label, LV_ALIGN_CENTER, 0, -10);

    // GB used / total — child of arc, below the percentage
    _gb_label = lv_label_create(_arc);
    lv_label_set_text(_gb_label, "0 / 0 GB");
    lv_obj_set_style_text_font(_gb_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(_gb_label, lv_color_hex(C_MUTED), 0);
    lv_obj_align(_gb_label, LV_ALIGN_CENTER, 0, 14);

    ui_draw_nav_dots(_screen, 2, lv_color_hex(C_ORANGE));
    return _screen;
}

void screen_ram_update(float pct, float used_gb, float total_gb) {
    lv_color_t col = ui_gauge_color(pct);
    lv_arc_set_value(_arc, (int32_t)pct);
    lv_obj_set_style_arc_color(_arc, col, LV_PART_INDICATOR);
    lv_label_set_text_fmt(_pct_label, "%.0f%%", (double)pct);
    lv_obj_set_style_text_color(_pct_label, col, 0);
    lv_label_set_text_fmt(_gb_label, "%.1f / %.1f GB", (double)used_gb, (double)total_gb);
}
