import serial
from flask import Flask, render_template
import threading

app = Flask(__name__)

serial_data = ''
ser = None
monitoring = False

def read_from_serial():
    global serial_data, ser, monitoring
    while True:
        if ser and ser.is_open and monitoring:
            read_ser = ser.readline()
            serial_data = read_ser.decode().strip()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def data():
    return serial_data

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

if __name__ == '__main__':
    thread = threading.Thread(target=read_from_serial)
    thread.daemon = True
    thread.start()
    app.run(debug=True)
