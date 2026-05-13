import psutil
import time

_last_net_io = None
_last_net_time = None


def get_cpu_percent() -> float:
    return round(psutil.cpu_percent(interval=None), 1)


def get_ram() -> dict:
    mem = psutil.virtual_memory()
    return {
        "used_gb": round(mem.used / (1024 ** 3), 2),
        "total_gb": round(mem.total / (1024 ** 3), 2),
        "percent": round(mem.percent, 1),
    }


def get_net_io() -> dict:
    global _last_net_io, _last_net_time

    net = psutil.net_io_counters()
    now = time.time()

    if _last_net_io is None or _last_net_time is None:
        _last_net_io = net
        _last_net_time = now
        return {"sent_kbs": 0.0, "recv_kbs": 0.0}

    dt = now - _last_net_time
    if dt <= 0:
        return {"sent_kbs": 0.0, "recv_kbs": 0.0}

    sent_kbs = round((net.bytes_sent - _last_net_io.bytes_sent) / dt / 1024, 2)
    recv_kbs = round((net.bytes_recv - _last_net_io.bytes_recv) / dt / 1024, 2)

    _last_net_io = net
    _last_net_time = now

    return {
        "sent_kbs": max(0.0, sent_kbs),
        "recv_kbs": max(0.0, recv_kbs),
    }


def get_uptime_s() -> int:
    return int(time.time() - psutil.boot_time())


def get_all_metrics() -> dict:
    return {
        "status": "online",
        "cpu": get_cpu_percent(),
        "ram": get_ram(),
        "net": get_net_io(),
        "uptime_s": get_uptime_s(),
    }


def get_processes(n: int = 8) -> list:
    procs = []
    for p in psutil.process_iter(["name", "cpu_percent", "memory_percent"]):
        try:
            procs.append(p.info)
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            pass
    procs.sort(key=lambda x: x.get("cpu_percent") or 0.0, reverse=True)
    return procs[:n]
