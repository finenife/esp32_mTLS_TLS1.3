
from flask import Flask, request, jsonify
from flask_cors import CORS
from auth import jwt_required

app = Flask(__name__)
CORS(app)




@app.route('/api/data', methods=['GET'])
@jwt_required                                   #this decorator is where all of our JWT authorization happens, claims checking and scoping can be confirmed in auth.py
def get_data():
    claims = request.user_claims
    print(claims)
    return jsonify({"message": f"Hello, {claims['name']}!"})


@app.route("/")
def hello():
    return "<h1 style='color:blue'>Hello There!</h1>"
    
if __name__ == "__main__":
    app.run(host='0.0.0.0')
