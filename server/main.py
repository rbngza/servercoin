import asyncio
import json
import os

from dotenv import load_dotenv
from fastapi import Depends, FastAPI, Header, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from sse_starlette.sse import EventSourceResponse

from metrics import get_all_metrics, get_processes

load_dotenv()
_API_KEY = os.getenv("API_KEY", "")

app = FastAPI(title="Nexus Health API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["GET"],
    allow_headers=["*", "Authorization"],
)


async def verify_key(authorization: str = Header(None)):
    if not _API_KEY or authorization != f"Bearer {_API_KEY}":
        raise HTTPException(status_code=401, detail="Unauthorized")


@app.get("/health", dependencies=[Depends(verify_key)])
async def health():
    return get_all_metrics()


@app.get("/stream", dependencies=[Depends(verify_key)])
async def stream():
    async def event_generator():
        while True:
            data = get_all_metrics()
            yield {"event": "metrics", "data": json.dumps(data)}
            await asyncio.sleep(2)

    return EventSourceResponse(event_generator())


@app.get("/processes", dependencies=[Depends(verify_key)])
async def processes():
    return get_processes(n=8)


if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=False)
