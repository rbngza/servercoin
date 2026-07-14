#pragma once
#include <Arduino.h>

struct Metrics {
    float    cpu          = 0.0f;
    float    ram_percent  = 0.0f;
    float    ram_used_gb  = 0.0f;
    float    ram_total_gb = 0.0f;
    float    net_sent_kbs = 0.0f;
    float    net_recv_kbs = 0.0f;
    float    cpu_temp_c   = -1.0f;   // -1 = sensor unavailable
    float    nvme_temp_c  = -1.0f;   // -1 = sensor unavailable
    uint32_t uptime_s     = 0;
    bool     online       = false;
    bool     valid        = false;
};

#define MAX_PROCESSES 8

struct ProcessInfo {
    char  name[32];
    float cpu;
    float mem;
};

extern Metrics     g_metrics;
extern ProcessInfo g_processes[MAX_PROCESSES];
extern int         g_process_count;

/** Open persistent HTTP/SSE connection to SERVER_HOST:/stream. */
void sse_begin();

/**
 * Non-blocking tick — reads available SSE data and updates g_metrics.
 * Returns true if a fresh metrics frame was parsed this call.
 */
bool sse_tick();

/** One-shot HTTP GET /processes, updates g_processes + g_process_count. */
void fetch_processes();
