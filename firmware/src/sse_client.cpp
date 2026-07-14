#include "sse_client.h"
#include "config.h"
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

Metrics     g_metrics;
ProcessInfo g_processes[MAX_PROCESSES];
int         g_process_count = 0;

static WiFiClient _client;
static bool       _connected          = false;
static String     _lineBuf            = "";
static uint32_t   _lastConnectAttempt = 0;

// Reconnect backoff: start at 5 s, double on each failure, cap at 30 s.
static uint32_t   _reconnectInterval  = 5000;

// ─── SSE parsing ─────────────────────────────────────────────────────────────

static bool parse_metrics(const String &data) {
    JsonDocument doc;
    if (deserializeJson(doc, data) != DeserializationError::Ok) return false;

    g_metrics.cpu          = doc["cpu"]             | 0.0f;
    g_metrics.ram_percent  = doc["ram"]["percent"]  | 0.0f;
    g_metrics.ram_used_gb  = doc["ram"]["used_gb"]  | 0.0f;
    g_metrics.ram_total_gb = doc["ram"]["total_gb"] | 0.0f;
    g_metrics.net_sent_kbs = doc["net"]["sent_kbs"] | 0.0f;
    g_metrics.net_recv_kbs = doc["net"]["recv_kbs"] | 0.0f;
    g_metrics.cpu_temp_c   = doc["temp"]["cpu_c"]   | -1.0f;  // null -> -1
    g_metrics.nvme_temp_c  = doc["temp"]["nvme_c"]  | -1.0f;  // null -> -1
    g_metrics.uptime_s     = doc["uptime_s"]        | 0u;
    g_metrics.online       = (strcmp(doc["status"] | "", "online") == 0);
    g_metrics.valid        = true;
    return true;
}

// ─── Connection management ────────────────────────────────────────────────────

static bool connect_sse() {
    // Hard 2-second connect timeout — never hangs the UI loop.
    if (!_client.connect(SERVER_HOST, SERVER_PORT, 2000)) {
        Serial.println("SSE: connect failed");
        // Exponential backoff: 5 s → 10 s → 20 s → 30 s (cap)
        _reconnectInterval = min(_reconnectInterval * 2, (uint32_t)30000);
        return false;
    }
    _client.printf(
        "GET /stream HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Accept: text/event-stream\r\n"
        "Cache-Control: no-cache\r\n"
        "Connection: keep-alive\r\n\r\n",
        SERVER_HOST, SERVER_PORT);
    _lineBuf          = "";
    _connected        = true;
    _reconnectInterval = 5000;  // reset backoff on success
    Serial.println("SSE: connected");
    return true;
}

// ─── Public API ──────────────────────────────────────────────────────────────

void sse_begin() {
    _lastConnectAttempt = millis();
    connect_sse();
}

bool sse_tick() {
    if (!_connected) {
        uint32_t now = millis();
        if (now - _lastConnectAttempt >= _reconnectInterval) {
            _lastConnectAttempt = now;
            connect_sse();  // max 2 s block, then returns
        }
        return false;
    }

    if (!_client.connected()) {
        _connected       = false;
        g_metrics.online = false;
        _client.stop();
        Serial.println("SSE: disconnected");
        return false;
    }

    bool got_frame = false;

    // Read available bytes non-blocking — budget keeps loop responsive
    int budget = 512;
    while (_client.available() && budget-- > 0) {
        char c = static_cast<char>(_client.read());
        if (c == '\n') {
            if (_lineBuf.startsWith("data: ")) {
                if (parse_metrics(_lineBuf.substring(6))) got_frame = true;
            }
            _lineBuf = "";
        } else if (c != '\r') {
            _lineBuf += c;
            if (_lineBuf.length() > 1024) _lineBuf = "";
        }
    }

    return got_frame;
}

void fetch_processes() {
    // Only attempt when the server is known to be reachable
    if (!g_metrics.online) return;

    HTTPClient http;
    WiFiClient client;
    String url = "http://" SERVER_HOST ":" + String(SERVER_PORT) + "/processes";

    http.begin(client, url);
    http.setTimeout(2000);  // 2 s max — won't hang the UI for long
    int code = http.GET();

    if (code == HTTP_CODE_OK) {
        String body = http.getString();
        JsonDocument doc;
        if (deserializeJson(doc, body) == DeserializationError::Ok) {
            JsonArray arr   = doc.as<JsonArray>();
            g_process_count = 0;
            for (JsonObject proc : arr) {
                if (g_process_count >= MAX_PROCESSES) break;
                ProcessInfo &p = g_processes[g_process_count++];
                strlcpy(p.name, proc["name"] | "?", sizeof(p.name));
                p.cpu = proc["cpu_percent"]    | 0.0f;
                p.mem = proc["memory_percent"] | 0.0f;
            }
        }
    } else {
        Serial.printf("fetch_processes HTTP %d\n", code);
    }

    http.end();
}
