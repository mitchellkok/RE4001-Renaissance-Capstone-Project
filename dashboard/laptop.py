import requests
import json
import sseclient
import threading

from flask import Flask, render_template
from turbo_flask import Turbo

PRELOAD = False # Not needed anymore; keep just in case
# connect to internet and set preload to True if js fails offline
# once loaded, set preload to False and reload

app = Flask(__name__)
turbo = Turbo(app)

ipaddress = "192.168.4.1"
result_dict = {}

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
            turbo.push(turbo.replace(render_template('loadavg.html'), 'load'))

@app.before_first_request
def before_first_request():
    if PRELOAD: return
    threading.Thread(target=begin_poll).start()

@app.route('/')
def hello():
    print("running")
    if not PRELOAD:
        pass
        # http_resquest_json(ipaddress)
    return render_template('index.html')

@app.context_processor
def inject_load():
    try:
        datetime = (
            str(result_dict["date_day"]) + "/" 
            + str(result_dict["date_month"]) + "/" 
            + str(result_dict["date_year"]) + " "
            + str(result_dict["date_hour"]) + ":"
            + str(result_dict["date_min"]) + ":"
            + str(result_dict["date_sec"])
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
                "{:.2f}".format(result_dict["thermo_thermo"])
            ]
        print("INJECT", data)
    except Exception as e:
        datetime = "NO DATA"
        data = ["--" for i in range(10)]
    
    return {
        'date_time' : datetime,
        'm_t': data[0], 
        'm_p': data[1], 
        'm_h': data[2],
        's_t': data[3], 
        's_p': data[4], 
        's_h': data[5],
        'g_so2': data[6], 
        'g_t': data[7], 
        't_t': data[8]
        }

def http_resquest_json(addr):
    r = requests.get(url(addr, "/getdata"))
    result_dict = json.loads(r.text)
    print(result_dict)
    return result_dict

def url(address, tag=""):
    return "http://" + address + tag

if __name__ == "__main__":
    app.run(debug=True, port=8090, threaded=True)