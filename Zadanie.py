import serial
from flask import Flask, render_template, jsonify
import threading
from collections import deque

app = Flask(_name_)

serial_data = '0'
ser = None
monitoring = False
data_history = deque(maxlen=10)  

def read_from_serial():
    global serial_data, ser, monitoring, data_history
    while True:
        if ser and ser.is_open and monitoring:
            read_ser = ser.readline()
            serial_data = read_ser.decode().strip()
            data_history.append(serial_data)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def data():
    return jsonify({
        'current': serial_data,
        'history': list(data_history)
    })

@app.route('/open')
def open_system():
    global ser
    try:
        ser = serial.Serial('/dev/ttyUSB0', 9600)  
        return "System initialized and sensors activated"
    except Exception as e:
        return str(e)

@app.route('/start')
def start_monitoring():
    global monitoring
    monitoring = True
    return "Monitoring started"

@app.route('/stop')
def stop_monitoring():
    global monitoring
    monitoring = False
    return "Monitoring stopped"

@app.route('/close')
def close_system():
    global ser, monitoring
    monitoring = False
    if ser:
        ser.close()  
        ser = None
    return "System deactivated and connection closed"

if _name_ == '_main_':
    thread = threading.Thread(target=read_from_serial)
    thread.daemon = True
    thread.start()
    app.run(debug=True)