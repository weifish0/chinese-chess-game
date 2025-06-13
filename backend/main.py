from datetime import datetime, timedelta
from typing import Optional
from fastapi import FastAPI, Depends, HTTPException, status
from fastapi.security import OAuth2PasswordBearer
from sqlalchemy.orm import Session
from pydantic import BaseModel
import models
import database
import auth
from jose import JWTError, jwt

app = FastAPI(title="Chess Game API")

# 創建資料庫表
models.Base.metadata.create_all(bind=database.engine)

# Pydantic 模型
class UserCreate(BaseModel):
    username: str
    email: str
    password: str

class User(BaseModel):
    username: str
    email: str
    created_at: datetime

    class Config:
        from_attributes = True

class Token(BaseModel):
    access_token: str
    token_type: str
    user: User

# 登入請求模型
class LoginRequest(BaseModel):
    email: str
    password: str

# 路由
@app.post("/register")
def register(user: UserCreate, db: Session = Depends(database.get_db)):
    try:
        db_user = auth.create_user(db, user.username, user.email, user.password)
        access_token = auth.create_access_token(data={"sub": user.email})
        return {
            "access_token": access_token,
            "token_type": "bearer",
            "user": {
                "username": db_user.username,
                "email": db_user.email,
                "created_at": db_user.created_at
            }
        }
    except HTTPException as e:
        raise e
    except Exception as e:
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail=f"註冊過程中發生錯誤：{str(e)}"
        )

@app.post("/token")
def login(login_data: LoginRequest, db: Session = Depends(database.get_db)):
    try:
        print("login")
        user = auth.authenticate_user(db, login_data.email, login_data.password)
        access_token = auth.create_access_token(data={"sub": user.email})
        return {
            "access_token": access_token,
            "token_type": "bearer",
            "user": {
                "username": user.username,
                "email": user.email,
                "created_at": user.created_at
            }
        }
    except HTTPException as e:
        raise e
    except Exception as e:
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail=f"登入過程中發生錯誤：{str(e)}"
        )

@app.get("/users/me", response_model=User)
async def read_users_me(current_user: models.User = Depends(auth.get_current_user)):
    return current_user
