# Allegro Xiangqi Game - 明棋暗棋冒險之旅

## 🎯 專案願景

我們致力於為程式設計學習者打造一個輕鬆有趣的象棋遊戲空間。透過結合傳統象棋（明棋、暗棋）與現代遊戲元素，創造出一個既能放鬆心情，又能體驗程式設計樂趣的遊戲環境。遊戲中融入了 I2P 程式設計課程的元素，讓玩家在遊玩過程中也能感受到程式設計的趣味性。

## 🎮 遊戲特色

### 核心玩法
- **明棋模式**：完整實現中國象棋規則，支援合法走法判斷
- **暗棋模式**：隨機翻牌機制，獨特的吃子規則
- **RPG 元素**：探索式地圖場景，NPC 互動對話
- **多人對戰**：支援本地雙人、AI 對戰及線上多人模式

### 技術亮點
- **完整的遊戲系統**：從基礎走子到進階 AI 對戰
- **豐富的互動體驗**：角色動畫、特效、音效
- **穩定的線上功能**：帳號系統、房間管理、即時對戰
- **開源友善**：提供完整的建置文件與開發指南

## 🎯 功能分級

### 基礎功能 [Easy]
- Xiangqi: 明棋基礎規則（走法合法判斷）
- Anqi: 造型更換
- Anqi: 暗棋隨機翻牌與走子規則
- Anqi: 回合時間限制、玩家回合提示
- Anqi: 求和、投降、延長時間
- Playground: NPC 對話功能（空白鍵觸發）
- Playground: 切換背景音樂
- Playground: 基礎碰撞判斷
- 本地雙人遊戲（PVP 模式）

### 進階功能 [Normal]
- 悔棋、空步判和
- 特效動畫（~~吃子動畫~~、音效）
- Playgorund: 中間場景角色移動動畫 + 地圖指引互動
- Playground: 彩蛋功能：找到郭教授可以與之聊天（支援 chatGPT 請求）
- ~~關卡達成任務紀錄與提示~~
- ~~計分系統（依照棋子階級與條件加權）~~


### 高階功能 [Hard]
- Anqi: 長捉禁手規則
- Playground: JWT Token
- Playground: SQLite 儲存玩家資訊與成就
- Playground: 帳號密碼提示、有效檢查
- Playground: 鏡頭跟隨功能
- Playground: Mini Map
- ~~FastAPI 登入系統（LINE、GitHub、Google OAuth 整合）~~
- ~~線上對戰房間系統（建房 / 加入 / 同步棋步）~~
- ~~支線任務模擬課程教授測驗（RPG 敘事互動）~~
- ~~AI 對戰邏輯（明棋與暗棋）~~

## 🎲 遊戲流程

1. **探索階段**
   - 開場進入中間地圖場景
   - 與 NPC 互動獲取任務
   - 自由探索遊戲世界

2. **對戰選擇**
   - 進入明棋宮殿或暗棋宮殿
   - 選擇遊戲模式（單人/AI/雙人/線上）
   - 開始對戰

3. **成就系統**
   - 完成棋局獲得積分
   - 解鎖支線任務成就
   - 收集特殊獎勵

## 👥 團隊分工

### [薛彥彬](https://github.com/faaberling-0st1d) (33%)
- 明棋模式：棋盤建立、走法合法判斷、AI 對戰邏輯
- 玩家計分與結果呈現模組
- 本地雙人模式切換功能

### [蘇緯泓](https://github.com/Su-Wei-Hong) (33%)
- 暗棋模式邏輯：棋子隨機翻牌、吃子規則、悔棋功能
- 吃子動畫與特效整合
- 計分與勝負條件實作

### [鄭名緯](https://github.com/weifish0) (33%)
- 中間場景建構：角色移動、地圖事件、NPC 對話邏輯
- FastAPI + SQLite 帳號登入與房間建立系統
- 線上對戰通訊（棋步交換API）、房間狀態同步


## 🛠️ 技術架構

### 前端
- C++ with Allegro5
- 遊戲邏輯與渲染系統
- 使用者介面設計

### 後端
- Python with FastAPI
- SQLite3 資料庫
- OAuth 整合（LINE、GitHub、Google）

### 開發工具
- nlohmann/json.hpp
- cpr (C++ HTTP client)
- CMakeLists.txt
- Makefile

## 🚀 建置與執行

### 前置需求
- Allegro5 函式庫
- C++ 編譯器
- Python 3.8+
- SQLite3

### 遊戲端建置 (Client - Allegro5)
```bash
# 使用 CMake
mkdir build && cd build
cmake ..
make

# 或使用 Makefile
make
```

### 伺服器端建置 (Server - FastAPI)
```bash
# 安裝依賴
pip install -r requirements.txt

# 啟動伺服器
uvicorn main:app --reload
```

## 📅 專案資訊
- 課程：Introduction to Programming II（I2P II）
- 指導老師：郭柏志 教授
- 提交日期：2025/06/14

## 🤝 開源貢獻
我們歡迎所有開發者參與專案改進！請參考我們的貢獻指南，一起讓這個專案變得更好。
