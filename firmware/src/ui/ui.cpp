#include "ui.h"
#include "screen_home.h"
#include "screen_cpu.h"
#include "screen_ram.h"
#include "screen_network.h"
#include "screen_processes.h"

#define NUM_SCREENS 5
#define NAV_DOT_SIZE   8
#define NAV_DOT_GAP   12
#define NAV_DOT_Y    205

static lv_obj_t *_screens[NUM_SCREENS];
static int        _current = 0;

// ─── Shared helpers ───────────────────────────────────────────────────────────

void ui_draw_nav_dots(lv_obj_t *parent, int active_idx, lv_color_t accent_col) {
    int total_w = NUM_SCREENS * NAV_DOT_GAP - (NAV_DOT_GAP - NAV_DOT_SIZE);
    int start_x = 120 - total_w / 2;

    for (int i = 0; i < NUM_SCREENS; i++) {
        lv_obj_t *dot = lv_obj_create(parent);
        lv_obj_set_size(dot, NAV_DOT_SIZE, NAV_DOT_SIZE);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(dot, 0, 0);
        lv_obj_set_style_pad_all(dot, 0, 0);
        lv_obj_set_pos(dot, start_x + i * NAV_DOT_GAP, NAV_DOT_Y);
        lv_obj_set_style_bg_color(dot,
            (i == active_idx) ? accent_col : lv_color_hex(C_DIM), 0);
    }
}

lv_color_t ui_gauge_color(float pct) {
    if (pct < 60.0f) return lv_color_hex(C_GREEN);
    if (pct < 80.0f) return lv_color_hex(C_YELLOW);
    return lv_color_hex(C_RED);
}

// ─── Public API ───────────────────────────────────────────────────────────────

void ui_init() {
    _screens[0] = screen_home_create();
    _screens[1] = screen_cpu_create();
    _screens[2] = screen_ram_create();
    _screens[3] = screen_network_create();
    _screens[4] = screen_processes_create();

    lv_scr_load(_screens[0]);
    _current = 0;
}

void ui_next_screen() {
    int next = (_current + 1) % NUM_SCREENS;
    lv_scr_load_anim(_screens[next], LV_SCR_LOAD_ANIM_MOVE_LEFT, 250, 0, false);
    _current = next;
}

void ui_prev_screen() {
    int prev = (_current - 1 + NUM_SCREENS) % NUM_SCREENS;
    lv_scr_load_anim(_screens[prev], LV_SCR_LOAD_ANIM_MOVE_RIGHT, 250, 0, false);
    _current = prev;
}

void ui_update(const Metrics &m) {
    screen_home_update(m);
    screen_cpu_update(m.cpu, m.cpu_temp_c, m.nvme_temp_c);
    screen_ram_update(m.ram_percent, m.ram_used_gb, m.ram_total_gb);
    screen_network_update(m.net_sent_kbs, m.net_recv_kbs);
}

void ui_update_processes(const ProcessInfo *procs, int count) {
    screen_processes_update(procs, count);
}
