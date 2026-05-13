import asyncio
import json

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from sse_starlette.sse import EventSourceResponse

from metrics import get_all_metrics, get_processes

app = FastAPI(title="Nexus Health API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["GET"],
    allow_headers=["*"],
)


@app.get("/health")
async def health():
    """One-shot snapshot of all current metrics."""
    return get_all_metrics()


@app.get("/stream")
async def stream():
    """SSE endpoint — pushes a 'metrics' event every 2 seconds."""
    async def event_generator():
        while True:
            data = get_all_metrics()
            yield {"event": "metrics", "data": json.dumps(data)}
            await asyncio.sleep(2)

    return EventSourceResponse(event_generator())


@app.get("/processes")
async def processes():
    """Top 8 processes by CPU usage."""
    return get_processes(n=8)


if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=False)
