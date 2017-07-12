import paho.mqtt.client as mqtt
import time
import math
import json

host = "192.168.1.253"
msg = ""
cli = "Pi1"
def on_connect(client, userdata, flags, rc):
        print("Client Connected "+str(rc))
        client.subscribe("sutd/2.601")

def on_message(client, userdata, message):
        msg = str(message.payload.decode("utf-8"))
        write_message("/home/pi/Desktop/DataGather/Data/KTPH_", msg)
        print("Incomming message", msg)

def write_message(path, message):
        message = message.replace("n_t", "noise_t")
        message = message.replace("n_h", "noise_h")
	jsonString = json.loads(message)
        ticks = int(round(time.time()*1000))
        file = path+jsonString["id"]+"_"+str(ticks)+".txt"
        foo = open(file, "w")
        foo.write(json.dumps(jsonString))
        foo.close();

client = mqtt.Client(cli)
client.connect(host)

client.on_message = on_message
client.on_connect = on_connect


client.loop_forever()
