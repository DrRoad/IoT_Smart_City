import paho.mqtt.client as mqtt
import time

host="192.168.1.162"
msg = ""

client = mqtt.Client()
client.username_pw_set("vikum","sutdmqtt")
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
