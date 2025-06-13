from datetime import datetime, timedelta
from typing import Optional
from fastapi import FastAPI, Depends, HTTPException, status
from fastapi.security import OAuth2PasswordRequestForm
from sqlalchemy.orm import Session
from pydantic import BaseModel
import models
import database
import auth

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
@app.post("/register", response_model=Token)
def register_user(user: UserCreate, db: Session = Depends(database.get_db)):
    # 檢查用戶名是否已存在
    db_user = auth.get_user_by_username(db, username=user.username)
    if db_user:
        raise HTTPException(
            status_code=400,
            detail="Username already registered"
        )
    
    # 檢查郵箱是否已存在
    db_user = auth.get_user_by_email(db, email=user.email)
    if db_user:
        raise HTTPException(
            status_code=400,
            detail="Email already registered"
        )
    
    # 創建新用戶
    hashed_password = auth.get_password_hash(user.password)
    db_user = models.User(
        username=user.username,
        email=user.email,
        hashed_password=hashed_password
    )
    db.add(db_user)
    db.commit()
    db.refresh(db_user)
    
    # 生成 token
    access_token_expires = timedelta(minutes=auth.ACCESS_TOKEN_EXPIRE_MINUTES)
    access_token = auth.create_access_token(
        data={"sub": db_user.username}, expires_delta=access_token_expires
    )
    
    # 返回 token 和用戶資料
    return {
        "access_token": access_token,
        "token_type": "bearer",
        "user": {
            "username": db_user.username,
            "email": db_user.email,
            "created_at": db_user.created_at
        }
    }

@app.post("/token", response_model=Token)
async def login_for_access_token(
    login_data: LoginRequest,
    db: Session = Depends(database.get_db)
):
    user = auth.authenticate_user(db, login_data.email, login_data.password)
    if not user:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Incorrect email or password",
            headers={"WWW-Authenticate": "Bearer"},
        )
    
    # 更新最後登入時間
    user.last_login = datetime.utcnow()
    db.commit()
    
    access_token_expires = timedelta(minutes=auth.ACCESS_TOKEN_EXPIRE_MINUTES)
    access_token = auth.create_access_token(
        data={"sub": user.username}, expires_delta=access_token_expires
    )
    
    # 返回 token 和用戶資料
    return {
        "access_token": access_token,
        "token_type": "bearer",
        "user": {
            "username": user.username,
            "email": user.email,
            "created_at": user.created_at
        }
    }

@app.get("/users/me", response_model=User)
async def read_users_me(current_user: models.User = Depends(auth.get_current_user)):
    return current_user
