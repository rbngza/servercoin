#pragma once
#include <lvgl.h>

lv_obj_t *screen_network_create();
void       screen_network_update(float sent_kbs, float recv_kbs);
