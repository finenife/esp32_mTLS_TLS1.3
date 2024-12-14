# config.py
import os
from dotenv import load_dotenv

load_dotenv()

class Config:
    CLIENT_ID = os.getenv("CLIENT_ID")
    CLIENT_SECRET = os.getenv("CLIENT_SECRET")
    KEYCLOAK_URL = os.getenv("KEYCLOAK_URL")
    REALM = os.getenv("REALM")