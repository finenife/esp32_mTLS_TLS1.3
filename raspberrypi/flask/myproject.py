import os
import requests
from flask import Flask, request, jsonify
from flask_cors import CORS
from authlib.integrations.flask_client import OAuth
from dotenv import load_dotenv
import jwt
load_dotenv()


app = Flask(__name__)
CORS(app)

'''oauth = OAuth(App)
keycloak = oauth.register(
    'keycloak',
    client_id='flask-app',
    client_secret='iff8ZxskbupWULbxP9Lb09kDl1ENHaCO',
    server_metadata_url='http://localhost:8080/realms/Test/.well-known/openid-configuration',
    client_kwargs={'scope': 'openid profile email'}
)'''



# Initialize OAuth
oauth = OAuth(app)

# Register Keycloak as an OAuth provider
oauth.register(
    'keycloak',
    client_id=os.getenv("CLIENT_ID"),
    client_secret=os.getenv("CLIENT_SECRET"),
    server_metadata_url=f'http://{os.getenv("KEYCLOAK_URL")}/realms/{os.getenv("REALM")}/.well-known/openid-configuration',
    client_kwargs={'scope': 'openid profile email'},
)

def get_jwks():
    """Fetch the JWKS from Keycloak."""
    jwks_url = f'http://{os.getenv("KEYCLOAK_URL")}/realms/{os.getenv("REALM")}/protocol/openid-connect/certs'
    print(jwks_url)
    response = requests.get(jwks_url)
    response.raise_for_status()  # Raise an error for bad responses
    return response.json()['keys']

# Function to convert JWK to PEM format
def jwk_to_pem(jwk):
    from cryptography.hazmat.backends import default_backend
    from cryptography.hazmat.primitives.asymmetric import rsa
    from cryptography.hazmat.primitives import serialization

    # Extract modulus and exponent from JWK
    n = int.from_bytes(jwt.utils.base64url_decode(jwk['n']), 'big')
    e = int.from_bytes(jwt.utils.base64url_decode(jwk['e']), 'big')

    # Create RSA public key object
    public_key = rsa.RSAPublicNumbers(e, n).public_key(default_backend())

    # Convert to PEM format
    pem = public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo,
    )
    
    return pem

def verify_jwt(token):
    try:
        # Get public keys from Keycloak
        jwks = get_jwks()
        
        # Loop through keys to find the correct one for verification
        rsa_key = {}
        for key in jwks:
            if key['kty'] == 'RSA':
                rsa_key = jwk_to_pem(key)  # Convert JWK to PEM format
                break
        
        if not rsa_key:
            print("No valid RSA key found.")
            return None
        
        # Decode and verify the token using the public key and expected audience
        decoded_token = jwt.decode(
            token,
            rsa_key,
            algorithms=['RS256'],
            audience=os.getenv('CLIENT_ID'),
            claims_options={'iss': {'essential': True, 'values': [f'http://{os.getenv("KEYCLOAK_URL")}/realms/{os.getenv("REALM")}']}})
        return decoded_token
    except jwt.ExpiredSignatureError:
        print("Token has expired")
        return None
    except Exception as e:
        print(f"Unable to parse token: {e}")
        return None

@app.route('/api/data', methods=['GET'])
def get_data():
    auth_header = request.headers.get('Authorization', None)

    if not auth_header:
        return jsonify({"msg": "Missing Authorization Header"}), 401
    print(auth_header)
    token = auth_header.split(" ")[1]  # Extract token from "Bearer <token>"
    
    # Verify the JWT token
    claims = verify_jwt(token)
    print(claims)
    if not claims:
        return jsonify({"msg": "Invalid token"}), 401
    
    # If verification is successful, return user info
    return jsonify({"message": f"Hello, {claims['preferred_username']}!"})


@app.route("/")
def hello():
    return "<h1 style='color:blue'>Hello There!</h1>"
    
if __name__ == "__main__":
    app.run(host='0.0.0.0')
