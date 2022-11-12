import random
import requests
import json
import sseclient
import threading

from flask import Flask, render_template
from turbo_flask import Turbo

PRELOAD = False 
# connect to internet and set preload to True if js fails offline
# once loaded, set preload to False and reload

app = Flask(__name__)
turbo = Turbo(app)

ipaddress = "192.168.4.1"

def begin_poll():
    print("Starting...")
    url = "http://" + ipaddress + '/events'
    requests.get(url, stream=True)
    messages = sseclient.SSEClient(url)
    cnt = 0
    with app.app_context():
        for msg in messages:
            print(msg, cnt, "TRYING TURBO")
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
        r = requests.get(url(ipaddress, "/getdata"))
        result_dict = json.loads(r.text)
        print(result_dict)
    return render_template('index.html')

def url(address, tag=""):
    return "http://" + address + tag

@app.context_processor
def inject_load():
    load = [int(random.random() * 100) / 100 for _ in range(3)]
    print("INJECT", load)
    return {'load1': load[0], 'load5': load[1], 'load15': load[2]}


# print(r.text)[:20÷0]
if __name__ == "__main__":
    app.run(debug=True, port=8090, threaded=True)