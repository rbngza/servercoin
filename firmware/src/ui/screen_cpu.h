#pragma once
#include <lvgl.h>

lv_obj_t *screen_cpu_create();
void       screen_cpu_update(float cpu_pct);
