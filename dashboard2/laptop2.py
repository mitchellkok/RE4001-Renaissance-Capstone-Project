import requests
import json
import pandas as pd
import sseclient
import time
from flask import Flask
from flask_socketio import SocketIO
from flask_cors import CORS
import eventlet
eventlet.monkey_patch()

# TODO: Add in IMU

PRELOAD = False # Not needed anymore; keep just in case
# connect to internet and set preload to True if js fails offline
# once loaded, set preload to False and reload
TESTING = True # Toggle for test mode

app = Flask(__name__)
CORS(app,resources={r"/*":{"origins":"*"}})
socketio = SocketIO(app, cors_allowed_origins="*")

ipaddress = "192.168.4.1"
result_dict = {}
datetime = 0
data = [] 
df = pd.DataFrame() # global dataframe to store data

@socketio.on('connect')
def handle_connect():
    print('Client connected')

def testing():
    global datetime, data, df
    while True:
        datetime +=1
        data = [datetime*(datetime%2) for i in range(27)]
        json_dict = get_data()
        print("DATA: ", datetime, json_dict)
        socketio.emit('data', json_dict) # send the data back to the client
        df = pd.concat([df, pd.DataFrame.from_records([json_dict])]) 
        eventlet.sleep(5)
    
def begin_poll():
    global result_dict
    print("Starting...")
    url = "http://" + ipaddress + '/events'
    requests.get(url, stream=True)
    messages = sseclient.SSEClient(url)
    cnt = 0
    with app.app_context():
        for msg in messages:
            try:
                result_dict = json.loads(msg.data)
                print(cnt, result_dict)
            except:
                print("Could not parse JSON:", msg.data)
            cnt += 1           
    
@app.route('/data')
def get_data():
    global datetime, data
    return {
        'date_time': datetime,
        'm_t': data[0], 
        'm_p': data[1], 
        'm_h': data[2],
        's_t': data[3], 
        's_p': data[4], 
        's_h': data[5],
        'g_so2': data[6], 
        'g_t': data[7], 
        't_t': data[8],
        'e_so2': data[9],
        'co2': data[10],
        'rssi': data[11],
        'gps_sat': data[12],
        'gps_hdop': data[13],
        'gps_lat': data[14],
        'gps_lng': data[15],
        'gps_alt': data[16],
        'gps_deg': data[17],
        'gps_mps': data[18],
        'gps_val': data[19],
        'imu_tmp': data[20],
        'imu_acx': data[21],
        'imu_acy': data[22],
        'imu_acz': data[23],
        'imu_gyx': data[24],
        'imu_gyy': data[25],
        'imu_gyz': data[26]
        }

@app.context_processor
def inject_load():
    try:
        datetime = (
            str(result_dict["date_day"]).zfill(2) + "/" 
            + str(result_dict["date_month"]).zfill(2) + "/" 
            + str(result_dict["date_year"]).zfill(2) + " "
            + str(result_dict["date_hour"]).zfill(2) + ":"
            + str(result_dict["date_min"]).zfill(2) + ":"
            + str(result_dict["date_sec"]).zfill(2)
            )
        data = [
            "{:.2f}".format(result_dict["atm_master_temp"]),
            "{:.2f}".format(result_dict["atm_master_pressure"]),
            "{:.2f}".format(result_dict["atm_master_humidity"]),
            "{:.2f}".format(result_dict["atm_slave_temp"]),
            "{:.2f}".format(result_dict["atm_slave_pressure"]),
            "{:.2f}".format(result_dict["atm_slave_humidity"]),
            "{:.2f}".format(result_dict["gravity_so2conc"]),
            "{:.2f}".format(result_dict["gravity_so2temp"]),
            "{:.2f}".format(result_dict["thermo_thermo"]),
            "",
            "",
            ]
        print("INJECT", data)
    except Exception as e:
        datetime = "NO DATA"
        data = ["--" for i in range(27)]
    
    return {
        'date_time': datetime,
        'm_t': data[0], 
        'm_p': data[1], 
        'm_h': data[2],
        's_t': data[3], 
        's_p': data[4], 
        's_h': data[5],
        'g_so2': data[6], 
        'g_t': data[7], 
        't_t': data[8],
        'e_so2': data[9],
        'co2': data[10],
        'rssi': data[11],
        'gps_sat': data[12],
        'gps_hdop': data[13],
        'gps_lat': data[14],
        'gps_lng': data[15],
        'gps_alt': data[16],
        'gps_deg': data[17],
        'gps_mps': data[18],
        'gps_val': data[19],
        'imu_tmp': data[20],
        'imu_acx': data[21],
        'imu_acy': data[22],
        'imu_acz': data[23],
        'imu_gyx': data[24],
        'imu_gyy': data[25],
        'imu_gyz': data[26]
        }

def http_resquest_json(addr):
    r = requests.get("http://" + addr + "/getdata")
    result_dict = json.loads(r.text)
    print(result_dict)
    return result_dict

@app.before_first_request
def init_brew_controller():
    if not TESTING: socketio.start_background_task(begin_poll)
    else: socketio.start_background_task(testing)

if __name__ == "__main__":
    socketio.run(app, debug=True, port=8090)