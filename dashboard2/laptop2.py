import requests
import json
import eventlet
import os
import os.path
import pandas as pd
import sseclient
from flask import Flask
from flask_socketio import SocketIO
from flask_cors import CORS
from datetime import datetime

eventlet.monkey_patch()

TESTING = True # Toggle for test mode
CSV_MODE = False

test_keys = ['date_time', 
            'm_t',      # Master 3 in 1 Temperature
            'm_p',      # Master 3 in 1 Pressure 
            'm_h',      # Master 3 in 1 Humidity
            's_t',      # Slave 3 in 1 Temperature
            's_p',      # Slave 3 in 1 Pressure
            's_h',      # Slave 3 in 1 Humidity
            'g_so2',    # Gravity SO2
            'g_t',      # Gravity Temperature
            't_t',      # Thermocouple Temperature
            't_a',      # Thermocouple Ambient Temp
            't_adc',    # Thermocouple ADC
            'e_so2',    # ECSense SO2
            'e_tmp',    # ECSense Temperature
            'e_hum',    # ECSense Humidity
            'e_so2_db', # ECSense Debug Info
            'co2',      # CO2
            'gps_sat',  # GPS Satellites
            'gps_hdop', # GPS HDOP
            'gps_lat',  # GPS Latitude
            'gps_lng',  # GPS Longitude
            'gps_alt',  # GPS Altitude (Meters)
            'gps_deg',  # GPS Degrees
            'gps_mps',  # GPS Ground Speed (MPS)
            'gps_val',  # GPS Value
            'imu_tmp',  # IMU Temperature
            'imu_acx',  # IMU Acceleration X
            'imu_acy',  # IMU Acceleration Y
            'imu_acz',  # IMU Acceleration Z
            'imu_gyx',  # IMU Gyro X
            'imu_gyy',  # IMU Gyro Y
            'imu_gyz',  # IMU Gyro Z
            "tx_rssi",  # LoRa TX RSSI (Master)
            "rx_rssi",  # LoRa RX RSSI (Basestation)
            "m_bv",     # Master Battery Voltage
            "m_bp",     # Master Battery Percentage
            "s_bv",     # Slave Battery Voltage
            "s_bp"      # Slave Battery Percentage
            ]

app = Flask(__name__)
CORS(app,resources={r"/*":{"origins":"*"}})
socketio = SocketIO(app, cors_allowed_origins="*")

esp32_address = "192.168.4.1"
df = pd.DataFrame() # global dataframe to store data

# Set path of csv file (csv file will be saved in the same location laptop2.py)
path = os.getcwd()
newPath = path.replace(os.sep, '/')
csv_file = newPath + '/' + 'dashboard_data.csv'

@socketio.on('connect')
def handle_connect():
    print('Client connected')

def testing():
    global df
    datetime = 0
    print("TESTING MODE...")
    if CSV_MODE:
        test_df = pd.read_csv('dashboard_data_noheatpack.csv')  
        for row_dict in test_df.to_dict(orient="records"):
            print(row_dict)
            socketio.emit('data', row_dict) # send the data back to the client
            eventlet.sleep(1)
    else:
        output_data = parse_data()
        while True:
            datetime +=1
            for i in output_data.keys():
                output_data[i] = datetime*(datetime%2)
            output_data['date_time'] = datetime
            print("DATA: ", datetime, output_data)
            socketio.emit('data', output_data) # send the data back to the client
            df = pd.concat([df, pd.DataFrame.from_records([output_data])])
            # df.to_csv(csv_file)
            eventlet.sleep(5)
    
def begin_poll():
    global df
    print("Starting...")
    print("CSV FILE:", csv_file)
    url = "http://" + esp32_address + '/events'

    connected = False
    while connected == False:
        try:
            requests.get(url, stream=True)
            connected = True
        except:
            print("Failed to connect to basestation!")

    messages = sseclient.SSEClient(url)
    cnt = 0
    with app.app_context():
        for msg in messages:
            try:
                input_data = json.loads(msg.data)
                output_data = parse_data(input_data)
                print("Received data from basestation:\n", cnt, output_data)

                # TODO: use socketio.emit to send data to frontend (can refer to testing()) --> Test with actual ESP32 now
                socketio.emit('data', output_data)

                # TODO: add dictionary to pandas dataframe and append to CSV
                df = pd.concat([df, pd.DataFrame.from_records([output_data])])
                df.to_csv(csv_file) # data will be written to csv file
            except:
                print("Could not parse JSON:", msg.data)
            cnt += 1           

def parse_data(input_data={}):
    try:
        output_data = input_data
        output_data["date_time"] = (
            str(input_data["date_day"]).zfill(2) + "/" 
            + str(input_data["date_month"]).zfill(2) + "/" 
            + str(input_data["date_year"]).zfill(2) + " "
            + str(input_data["date_hour"]).zfill(2) + ":"
            + str(input_data["date_min"]).zfill(2) + ":"
            + str(input_data["date_sec"]).zfill(2)
            )
        output_data.pop("date_day", None)
        output_data.pop("date_month", None)
        output_data.pop("date_year", None)
        output_data.pop("date_hour", None)
        output_data.pop("date_min", None)
        output_data.pop("date_sec", None)
        timenow = datetime.now()
        output_data["laptop_time_format"] = timenow.strftime("%H:%M:%S")
    except Exception as e:
        for i in test_keys:
            output_data[i] = "--"
    return output_data

def http_resquest_json(addr):
    r = requests.get("http://" + addr + "/getdata")
    input_data = json.loads(r.text)
    print(input_data)
    return input_data

@app.before_first_request
def init_brew_controller():
    print("Starting threads...")
    if not TESTING: socketio.start_background_task(begin_poll)
    else: socketio.start_background_task(testing)

def start_react():
    print("Starting React...")
    file_path = os.path.realpath(__file__) # .replace("\\","/")
    file_path = os.path.dirname(file_path)
    file_path = os.path.join(file_path, "frontend")
    print(file_path)
    
    os.chdir(file_path)
    os.system("npm start") 

if __name__ == "__main__":
    socketio.start_background_task(start_react)
    print("Starting SocketIO...")
    socketio.run(app, debug=True, port=8090)
    eventlet.sleep(2)
    

    