import json
import sseclient
import requests

if __name__ == '__main__':
    print("Starting...")

    url = 'http://192.168.4.1/events'
    response = requests.get(url, stream=True)
    messages = sseclient.SSEClient(url)

    for msg in messages:
        print(msg)  # call print(dir(msg)) to see the available attributes