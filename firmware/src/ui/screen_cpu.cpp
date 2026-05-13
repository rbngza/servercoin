#include "screen_cpu.h"
#include "ui.h"

static lv_obj_t         *_screen;
static lv_obj_t         *_arc;
static lv_obj_t         *_pct_label;  // child of _arc — always drawn on top
static lv_obj_t         *_chart;
static lv_chart_series_t *_series;

lv_obj_t *screen_cpu_create() {
    _screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(_screen, lv_color_hex(C_BG), 0);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t *title = lv_label_create(_screen);
    lv_label_set_text(title, "CPU");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(C_CYAN), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 24);

    // Arc gauge — 270° sweep, gap at the bottom (classic speedometer)
    _arc = lv_arc_create(_screen);
    lv_obj_set_size(_arc, 148, 148);
    lv_arc_set_rotation(_arc, 135);
    lv_arc_set_bg_angles(_arc, 0, 270);
    lv_arc_set_range(_arc, 0, 100);
    lv_arc_set_value(_arc, 0);
    lv_obj_remove_style(_arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(_arc, lv_color_hex(C_CARD), LV_PART_MAIN);
    lv_obj_set_style_arc_color(_arc, lv_color_hex(C_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(_arc, 14, LV_PART_MAIN);
    lv_obj_set_style_arc_width(_arc, 14, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(_arc, LV_OPA_TRANSP, 0);
    lv_obj_align(_arc, LV_ALIGN_CENTER, 0, -18);
    // Disable touch interaction — arc must not be draggable
    lv_obj_clear_flag(_arc, LV_OBJ_FLAG_CLICKABLE);

    // Percentage label — child of arc so it is guaranteed to render on top
    _pct_label = lv_label_create(_arc);
    lv_label_set_text(_pct_label, "0%");
    lv_obj_set_style_text_font(_pct_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(_pct_label, lv_color_hex(C_GREEN), 0);
    lv_obj_center(_pct_label);  // centred within the arc bounding box

    // Sparkline chart — 30 readings, no axes/dots/dividers
    _chart = lv_chart_create(_screen);
    lv_obj_set_size(_chart, 160, 34);
    lv_chart_set_type(_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(_chart, 30);
    lv_chart_set_range(_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_div_line_count(_chart, 0, 0);
    lv_obj_set_style_border_width(_chart, 0, 0);
    lv_obj_set_style_bg_opa(_chart, LV_OPA_TRANSP, 0);
    lv_obj_set_style_size(_chart, 0, LV_PART_INDICATOR);  // hide point dots
    lv_obj_align(_chart, LV_ALIGN_CENTER, 0, 62);
    lv_obj_clear_flag(_chart, LV_OBJ_FLAG_CLICKABLE);

    _series = lv_chart_add_series(_chart, lv_color_hex(C_CYAN), LV_CHART_AXIS_PRIMARY_Y);
    for (int i = 0; i < 30; i++) lv_chart_set_next_value(_chart, _series, 0);

    ui_draw_nav_dots(_screen, 1, lv_color_hex(C_CYAN));
    return _screen;
}

void screen_cpu_update(float cpu_pct) {
    lv_color_t col = ui_gauge_color(cpu_pct);
    lv_arc_set_value(_arc, (int32_t)cpu_pct);
    lv_obj_set_style_arc_color(_arc, col, LV_PART_INDICATOR);
    lv_label_set_text_fmt(_pct_label, "%.0f%%", (double)cpu_pct);
    lv_obj_set_style_text_color(_pct_label, col, 0);
    lv_chart_set_next_value(_chart, _series, (lv_coord_t)cpu_pct);
    // Only trigger chart redraw when this screen is actually visible
    if (lv_scr_act() == _screen) lv_chart_refresh(_chart);
}
