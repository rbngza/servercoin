#pragma once
#include <lvgl.h>
#include "../sse_client.h"

// ── Shared colour palette ─────────────────────────────────────────────────────
#define C_BG       0x0d0d0d
#define C_CARD     0x1e1e2e
#define C_CYAN     0x00d4ff
#define C_ORANGE   0xff6b35
#define C_PURPLE   0x7c4dff
#define C_GREEN    0x00ff88
#define C_RED      0xff4444
#define C_YELLOW   0xffaa00
#define C_WHITE    0xffffff
#define C_DIM      0x444444
#define C_MUTED    0x888888

/** Draw 5 nav dots centred near y=205. active_idx dot is highlighted with accent_col. */
void ui_draw_nav_dots(lv_obj_t *parent, int active_idx, lv_color_t accent_col);

/** Returns lv_color_hex(green/yellow/red) based on percentage. */
lv_color_t ui_gauge_color(float pct);

// ── Screen lifecycle ──────────────────────────────────────────────────────────
void ui_init();
void ui_next_screen();
void ui_prev_screen();

// ── Data update entry points ──────────────────────────────────────────────────
void ui_update(const Metrics &m);
void ui_update_processes(const ProcessInfo *procs, int count);
