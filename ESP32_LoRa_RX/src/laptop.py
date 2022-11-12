import requests

def url(address, tag=""):
    return "http://" + address + tag

r = requests.get(url("192.168.4.1", "/getdata"))
print(r)
print(r.text)[:200]