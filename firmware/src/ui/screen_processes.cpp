#include "screen_processes.h"
#include "ui.h"

static lv_obj_t *_screen;
static lv_obj_t *_table;

// Circular 240×240 display: safe content width at y=50 is ~184 px, at y=200 ~178 px.
// Table at 160 px wide centred stays within the circle at all vertical positions.
#define TABLE_W  160
#define COL0_W    82   // process name (truncated at 11 chars)
#define COL1_W    39   // CPU %
#define COL2_W    39   // MEM %

lv_obj_t *screen_processes_create() {
    _screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(_screen, lv_color_hex(C_BG), 0);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    // Title
    lv_obj_t *title = lv_label_create(_screen);
    lv_label_set_text(title, "PROCESSES");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(C_GREEN), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 18);

    // Table centred — 160 px wide fits within the circular display at all rows
    _table = lv_table_create(_screen);
    lv_obj_set_size(_table, TABLE_W, 170);
    lv_obj_align(_table, LV_ALIGN_CENTER, 0, 8);
    lv_obj_clear_flag(_table, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(_table, LV_OBJ_FLAG_SCROLLABLE);

    lv_table_set_col_cnt(_table, 3);
    lv_table_set_col_width(_table, 0, COL0_W);
    lv_table_set_col_width(_table, 1, COL1_W);
    lv_table_set_col_width(_table, 2, COL2_W);

    // Style — dark rows, thin dividers
    lv_obj_set_style_border_width(_table, 0, 0);
    lv_obj_set_style_bg_color(_table, lv_color_hex(C_BG), 0);
    lv_obj_set_style_pad_all(_table, 0, 0);

    lv_obj_set_style_text_font(_table, &lv_font_montserrat_12, LV_PART_ITEMS);
    lv_obj_set_style_text_color(_table, lv_color_hex(C_WHITE), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(_table, lv_color_hex(C_BG), LV_PART_ITEMS);
    lv_obj_set_style_border_color(_table, lv_color_hex(C_CARD), LV_PART_ITEMS);
    lv_obj_set_style_border_width(_table, 1, LV_PART_ITEMS);
    lv_obj_set_style_border_side(_table, LV_BORDER_SIDE_BOTTOM, LV_PART_ITEMS);
    lv_obj_set_style_pad_top(_table, 3, LV_PART_ITEMS);
    lv_obj_set_style_pad_bottom(_table, 3, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(_table, 3, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(_table, 2, LV_PART_ITEMS);

    // Header row
    lv_table_set_row_cnt(_table, 1);
    lv_table_set_cell_value(_table, 0, 0, "Name");
    lv_table_set_cell_value(_table, 0, 1, "CPU");
    lv_table_set_cell_value(_table, 0, 2, "MEM");

    ui_draw_nav_dots(_screen, 4, lv_color_hex(C_GREEN));
    return _screen;
}

void screen_processes_update(const ProcessInfo *procs, int count) {
    lv_table_set_row_cnt(_table, (uint16_t)(count + 1));

    for (int i = 0; i < count; i++) {
        char cpu_buf[8], mem_buf[8], name_buf[12];

        // Truncate names to 11 chars so they fit COL0_W at font size 12
        strlcpy(name_buf, procs[i].name, sizeof(name_buf));
        snprintf(cpu_buf, sizeof(cpu_buf), "%.1f%%", (double)procs[i].cpu);
        snprintf(mem_buf, sizeof(mem_buf), "%.1f%%", (double)procs[i].mem);

        lv_table_set_cell_value(_table, (uint16_t)(i + 1), 0, name_buf);
        lv_table_set_cell_value(_table, (uint16_t)(i + 1), 1, cpu_buf);
        lv_table_set_cell_value(_table, (uint16_t)(i + 1), 2, mem_buf);
    }
}
