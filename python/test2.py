import paho.mqtt.client as mqtt
import time

host = "10.19.52.64"
msg = ""
ticks = 0

prev_time = int(round(time.time()*1000))

def on_message(client, userdata, message):
	global prev_time	
	msg = str(message.payload.decode("utf-8"))
	print("Incomming message ", msg)
	ticks = int(round(time.time()*1000))	
	count(ticks)		

def count(time):
	global prev_time	
	print(time-prev_time)	
	prev_time = time	
		

client = mqtt.Client()
client.on_message = on_message

client.connect(host)
client.subscribe("sutd/2.601",2)

client.loop_forever()
