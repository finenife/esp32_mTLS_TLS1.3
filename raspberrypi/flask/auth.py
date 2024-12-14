# auth.py
import requests
from cachetools import TTLCache
import jwt
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
from config import Config
from flask import request, jsonify
from functools import wraps

# Initialize cache for JWKs
jwk_cache = TTLCache(maxsize=100, ttl=3600)  # Cache for 1 hour

def get_jwks(kid):
    if kid in jwk_cache:
        print("Using cached keys")
        return jwk_cache[kid]
    
    jwks_url = f'http://{Config.KEYCLOAK_URL}/realms/{Config.REALM}/protocol/openid-connect/certs'
    response = requests.get(jwks_url)
    response.raise_for_status()  # Raise an error for bad responses
    
    jwks = response.json()
    for key in jwks['keys']:
        jwk_cache[key['kid']] = key  # Cache each JWK
    
    return jwk_cache.get(kid)

def jwk_to_pem(jwk):
    n = int.from_bytes(jwt.utils.base64url_decode(jwk['n']), 'big')
    e = int.from_bytes(jwt.utils.base64url_decode(jwk['e']), 'big')
    
    public_key = rsa.RSAPublicNumbers(e, n).public_key(default_backend())
    
    pem = public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo,
    )
    
    return pem

def verify_jwt(token):
    try:
        header = jwt.get_unverified_header(token)
        kid = header.get('kid')
        
        key = get_jwks(kid)
        
        rsa_key = {}
        if key and key['kty'] == 'RSA':
            rsa_key = jwk_to_pem(key)  # Convert JWK to PEM format
        
        if not rsa_key:
            print("No valid RSA key found.")
            return None
        
        decoded_token = jwt.decode(
            token,
            rsa_key,
            algorithms=['RS256'],
            audience=Config.CLIENT_ID,
            claims_options={'iss': {'essential': True, 'values': [f'http://{Config.KEYCLOAK_URL}/realms/{Config.REALM}']}}
        )
        
        return decoded_token
    except jwt.ExpiredSignatureError:
        print("Token has expired")
        return None
    except Exception as e:
        print(f"Unable to parse token: {e}")
        return None
    
def jwt_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        auth_header = request.headers.get('Authorization', None)
        
        if not auth_header:
            return jsonify({"msg": "Missing Authorization Header"}), 401
        
        token = auth_header.split(" ")[1]  # Extract token from "Bearer <token>"
        
        # Verify the JWT token
        claims = verify_jwt(token)
        
        if not claims:
            return jsonify({"msg": "Invalid token"}), 401
        
        # Attach claims to the request context if needed
        request.user_claims = claims
        
        return f(*args, **kwargs)  # Call the original function
    
    return decorated_function