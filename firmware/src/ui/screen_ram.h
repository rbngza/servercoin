#pragma once
#include <lvgl.h>

lv_obj_t *screen_ram_create();
void       screen_ram_update(float pct, float used_gb, float total_gb);
