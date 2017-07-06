import paho.mqtt.client as mqtt
import time
import math
import json

host = "10.19.52.64"
msg = ""

def on_message(client, userdata, message):
	msg = str(message.payload.decode("utf-8"))
	write_message("Desktop/Project Work/python/", msg)
	print("Incomming message", msg)

def write_message(path, message):
	message = message.replace("n_t", "noise_t")
	message = message.replace("n_h", "noise_h")
	jsonString = json.loads(message)
	#jsonString = jsonString.replace("n_t", "noise_t");
	#jsonString = jsonString.replace("n_h", "noise_h");
	ticks = int(round(time.time()*1000))
	file = path+jsonString["id"]+"_"+str(ticks)+".txt"
	foo = open(file, "w")
	foo.write(json.dumps(jsonString))
	foo.close();

client = mqtt.Client()

client.on_message = on_message

client.connect(host)
client.subscribe("sutd/2.601")

client.loop_forever()