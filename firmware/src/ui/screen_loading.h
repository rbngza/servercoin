#pragma once
#include <lvgl.h>

lv_obj_t  *screen_loading_create();
void        screen_loading_set_status(const char *msg);
