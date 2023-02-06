import requests
import json
import pandas as pd
import sseclient
from flask import Flask
from flask_socketio import SocketIO
from flask_cors import CORS
import eventlet
eventlet.monkey_patch()

TESTING = True # Toggle for test mode
test_keys = ['date_time', 'm_t', 'm_p', 'm_h', 's_t', 's_p', 's_h', 'g_so2', 'g_t', 't_t', 'e_so2',
                'co2', 'rssi', 'gps_sat', 'gps_hdop', 'gps_lat', 'gps_lng', 'gps_alt', 'gps_deg',
                'gps_mps', 'gps_val', 'imu_tmp', 'imu_acx', 'imu_acy', 'imu_acz', 'imu_gyx',
                'imu_gyy', 'imu_gyz']

app = Flask(__name__)
CORS(app,resources={r"/*":{"origins":"*"}})
socketio = SocketIO(app, cors_allowed_origins="*")

esp32_address = "192.168.4.1"
input_data = {}
output_data = {}
datetime = 0
df = pd.DataFrame() # global dataframe to store data

@socketio.on('connect')
def handle_connect():
    print('Client connected')

def testing():
    global datetime, df
    print("TESTING MODE...")
    parse_data()
    while True:
        datetime +=1
        for i in output_data.keys():
            output_data[i] = datetime*(datetime%2)
        output_data['date_time'] = datetime
        print("DATA: ", datetime, output_data)
        socketio.emit('data', output_data) # send the data back to the client
        df = pd.concat([df, pd.DataFrame.from_records([output_data])]) 
        eventlet.sleep(5)
    
def begin_poll():
    global input_data, output_data
    print("Starting...")
    url = "http://" + esp32_address + '/events'
    requests.get(url, stream=True)
    messages = sseclient.SSEClient(url)
    cnt = 0
    with app.app_context():
        for msg in messages:
            try:
                input_data = json.loads(msg.data)
                print(cnt, input_data)
                parse_data()
                # TODO: use socketio.emit to send data to frontend (can refer to testing())

                # TODO: add dictionary to pandas dataframe and append to CSV
            except:
                print("Could not parse JSON:", msg.data)
            cnt += 1           

def parse_data():
    global datetime, input_data, output_data, data
    try:
        datetime = (
            str(input_data["date_day"]).zfill(2) + "/" 
            + str(input_data["date_month"]).zfill(2) + "/" 
            + str(input_data["date_year"]).zfill(2) + " "
            + str(input_data["date_hour"]).zfill(2) + ":"
            + str(input_data["date_min"]).zfill(2) + ":"
            + str(input_data["date_sec"]).zfill(2)
            )
        output_data = {
            'date_time': datetime,
            'm_t':      input_data["atm_master_temp"], 
            'm_p':      input_data["atm_master_pressure"], 
            'm_h':      input_data["atm_master_humidity"],
            's_t':      input_data["atm_slave_temp"],
            's_p':      input_data["atm_slave_pressure"], 
            's_h':      input_data["atm_slave_humidity"],
            'g_so2':    input_data["gravity_so2conc"],
            'g_t':      input_data["gravity_so2temp"],
            't_t':      input_data["thermo_thermo"],
            'e_so2':    "",
            'co2':      "",
            'rssi':     "",
            'gps_sat':  "",
            'gps_hdop': "",
            'gps_lat':  "",
            'gps_lng':  "",
            'gps_alt':  "",
            'gps_deg':  "",
            'gps_mps':  "",
            'gps_val':  "",
            'imu_tmp':  "",
            'imu_acx':  "",
            'imu_acy':  "",
            'imu_acz':  "",
            'imu_gyx':  "",
            'imu_gyy':  "",
            'imu_gyz':  ""
            }
    except Exception as e:
        datetime = 0
        for i in test_keys:
            output_data[i] = "--"

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