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

eventlet.monkey_patch()

TESTING = True # Toggle for test mode
test_keys = ['date_time', 'm_t', 'm_p', 'm_h', 's_t', 's_p', 's_h', 'g_so2', 'g_t', 't_t', 'e_so2',
                'co2', 'gps_sat', 'gps_hdop', 'gps_lat', 'gps_lng', 'gps_alt', 'gps_deg',
                'gps_mps', 'gps_val', 'imu_tmp', 'imu_acx', 'imu_acy', 'imu_acz', 'imu_gyx',
                'imu_gyy', 'imu_gyz', "tx_rssi", "rx_rssi", "m_bv", "m_bp", "s_bv", "s_bp"]

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
    requests.get(url, stream=True)
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

if __name__ == "__main__":
    socketio.run(app, debug=True, port=8090)