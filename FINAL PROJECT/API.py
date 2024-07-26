from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///C:/Users/User/GladysAmelia/FINAL PROJECT/database.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

class SensorData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    timestamp = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
    ppm = db.Column(db.Float, nullable=False)

@app.route('/data', methods=['POST'])
def add_data():
    try:
        data = request.get_json()
        ppm = data.get('ppm')

        if ppm is None:
            return jsonify({"error": "Missing 'ppm' in request data"}), 400

        new_data = SensorData(ppm=ppm)
        db.session.add(new_data)
        db.session.commit()
        return jsonify({"message": "Data added successfully"}), 201
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/data', methods=['GET'])
def get_data():
    try:
        all_data = SensorData.query.all()
        result = []
        for data in all_data:
            result.append({
                "timestamp": data.timestamp.isoformat(),
                "ppm": data.ppm
            })
        return jsonify(result)
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/data', methods=['DELETE'])
def delete_data():
    try:
        num_rows_deleted = db.session.query(SensorData).delete()
        db.session.commit()
        return jsonify({"message": f"Deleted {num_rows_deleted} rows"}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

def create_app():
    with app.app_context():
        db.create_all()
    return app

if __name__ == '__main__':
    app = create_app()
    app.run(host='0.0.0.0', port=5000, debug=True)