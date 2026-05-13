#pragma once
#include <lvgl.h>
#include "../sse_client.h"

lv_obj_t *screen_processes_create();
void       screen_processes_update(const ProcessInfo *procs, int count);
