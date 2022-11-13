import random
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
        data = [result_dict["atm_master_temp"],result_dict["atm_master_pressure"],result_dict["atm_master_humidity"]]
        print("INJECT", data)
    except Exception as e:
        data = ["--", "--", "--"]
    
    return {'load1': data[0], 'load5': data[1], 'load15': data[2]}

def http_resquest_json(addr):
    r = requests.get(url(addr, "/getdata"))
    result_dict = json.loads(r.text)
    print(result_dict)
    return result_dict

def url(address, tag=""):
    return "http://" + address + tag

if __name__ == "__main__":
    app.run(debug=True, port=8090, threaded=True)