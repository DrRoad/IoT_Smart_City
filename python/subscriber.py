import paho.mqtt.client as mqtt
import json
import Queue	

host = '192.168.1.253'
topic = 'g1'
msgQ = Queue.Queue()

def on_connect(client, userdata, flags, rc): #When this cline connect to the broker this method runs
        print("Client Connected "+str(rc))
        client.subscribe(topic)

def on_message(client, userdata, message): #On incomming msg to the subscriberd topic, this method runs
        msg = str(message.payload.decode("utf-8"))
        global msgQ
        msgQ.put(msg)

def command(message):
	if '{' in message:
		jsonStr = json.loads(message)
		print 'Light Value:', jsonStr['l']

	elif 'wifi' in message:
		msgList = message.split('_')
		print 'Power Usage:', msgList[1],'W'

	elif 'hue' in message:
		msgList = message.split['_']
		if 'bri' in msgList[2]:
			print 'Philips Hue Brightness:', msgList[3]
		else:
			print 'Philips Hue Status:',msgList[2]

client = mqtt.Client()
client.connect(host)

client.on_message = on_message
client.on_connect = on_connect

client.loop_start()

while (True):
	if not msgQ.empty():
		try:
			command(msgQ.get())
		except Exception as e:
			print e
