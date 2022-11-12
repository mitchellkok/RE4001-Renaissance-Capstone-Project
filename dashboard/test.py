from flask import Flask, render_template, request
import os
import requests
import json
import pandas as pd

app=Flask(__name__)
app.secret_key="123"

app.config["UPLOAD_FOLDER1"]="csv"

def url(address, tag=""):
    return "http://" + address + tag

@app.route("/",methods=['GET','POST'])
def home():
    # try:
    #     r = requests.get(url("192.168.4.1", "/getdata"))
    #     result_dict = json.loads(r.text)
    # except Exception as e:
    #     print(e)
    return render_template("frontend.html")

@app.route("/upload",methods=['GET','POST'])
def upload():
    return render_template("frontend.html")

@app.route("/display",methods=["GET","POST"])
def display():
    upload_file = request.files['upload_csv']
    if upload_file.filename != '':
            file_path = os.path.join(app.config["UPLOAD_FOLDER1"], upload_file.filename)
            upload_file.save(file_path)
            data=pd.read_csv(file_path,sep=",")
            return render_template("frontend.html",data=data.to_html(index=False))

if __name__=='__main__':
    app.run(debug=True)