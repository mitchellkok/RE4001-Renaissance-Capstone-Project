import requests
import json
from flask import Flask

app = Flask(__name__)

@app.route('/')
def hello():
    print("running")
    r = requests.get(url("192.168.4.1", "/getdata"))
    result_dict = json.loads(r.text)
    return result_dict

def url(address, tag=""):
    return "http://" + address + tag


# print(r.text)[:20÷0]