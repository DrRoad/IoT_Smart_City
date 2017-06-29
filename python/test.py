import paho.mqtt.client as mqtt
import time

host="iot.eclipse.org"
msg = ""

client = mqtt.Client()
client.connect(host, 1883, 60)

print("Publishing")
msg="Work"
client.publish("sutd/2.601", "Working")
print("###Published###")

#print("Pulbishing again")
#msg="No Work"
#client.publish("/sutd/2.601", msg, 2)
#print("###Published###")

client.disconnect()
