import serial
from flask import Flask, render_template
import threading

app = Flask(__name__)

# Global variable to store serial data
serial_data = ''

def read_from_serial():
    global serial_data
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    while True:
        read_ser = ser.readline()
        serial_data = read_ser.decode().strip()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def data():
    return serial_data

if __name__ == '__main__':
    thread = threading.Thread(target=read_from_serial)
    thread.daemon = True
    thread.start()
    app.run(debug=True)
