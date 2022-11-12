import requests
import json
from flask import Flask, Response

app = Flask(__name__)

@app.route('/')
def hello():
    print("running")
    r = requests.get(url("192.168.4.1", "/getdata"))
    result_dict = json.loads(r.text)
    return result_dict

# @app.route("/events")
# def stream():
#     def eventStream():
#         while True:
#             # Poll data from the database
#             # and see if there's a new message
#             if len(messages) > len(previous_messages):
#                 yield "data: {}\n\n".format(messages[len(messages)-1)])"
    
#     return Response(eventStream(), mimetype="text/event-stream")

def url(address, tag=""):
    return "http://" + address + tag


# print(r.text)[:20÷0]