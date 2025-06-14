import uvicorn
import sys
from pathlib import Path

# 將專案根目錄添加到 Python 路徑中
sys.path.append(str(Path(__file__).parent.parent))

if __name__ == "__main__":
    uvicorn.run("backend.main:app", host="0.0.0.0", port=8000, reload=True) 