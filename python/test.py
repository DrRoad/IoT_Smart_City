import paho.mqtt.client as mqtt
import time

host="iot.eclipse.org"
msg = ""

def on_message(client, userdata, message):
	print("message: ", str(message.payload.decode("utf-8")))
	print("topic: ",message.topic)

client = mqtt.Client()

client.on_message = on_message
client.connect(host, 1883, 60)

print("Publishing")
msg="Work"
client.publish("sutd/2.601", msg, 2)
print("###Published###")

print("Pulbishing again")
msg="No Work"
client.publish("sutd/2.601", msg, 2)
print("###Published###")

