#pragma once
#include <lvgl.h>
#include "../sse_client.h"

lv_obj_t *screen_home_create();
void       screen_home_update(const Metrics &m);
