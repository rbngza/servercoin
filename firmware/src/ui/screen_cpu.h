#pragma once
#include <lvgl.h>

lv_obj_t *screen_cpu_create();
void       screen_cpu_update(float cpu_pct, float cpu_temp_c, float nvme_temp_c);
