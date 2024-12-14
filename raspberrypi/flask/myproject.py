from flask import Flask, jsonify, request
##from authlib.integrations.flask_client import OAuth
from flask_cors import CORS

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

@app.route("/")
def hello():
    return "<h1 style='color:blue'>Hello There!</h1>"

@app.route("/api/data")
def get_data():
    print(request.headers)
    return jsonify({"message": "Hello from Flask!"})
    
if __name__ == "__main__":
    app.run(host='0.0.0.0')
