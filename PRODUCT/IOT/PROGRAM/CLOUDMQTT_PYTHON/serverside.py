'''
Sampel penggunaan property private https://www.programiz.com/python-programming/methods/built-in/property

Script untuk publish, subscribe multi connection
'''

import paho.mqtt.client as mqtt
import os,sys
from urllib.parse import urlparse
import threading
import time

stop_all_thread = False

class MQTT(threading.Thread) :

	def __init__(self, userid,server,port,username, password,topic):
		threading.Thread.__init__(self)
		self.userid = userid
		self.server = server
		self.port = port
		self.username = username
		self.password = password
		self.topic = topic
		self.mqttc = None

	def on_connect(self, client,userdata, flags, rc):
		print("rc: " + str(rc))

	def on_message(self, client, obj,msg):
		print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

	def on_publish(self, client, obj, mid):
		print("mid: "+ str(mid))

	def on_subscribe(self,client, obj, mid, granted_qos):
		print("Subscribed: " + str(mid) + " " + str(granted_qos))

	def on_log(self,client, obj, level, string):
		print(string)

	def on_disconnect (self, client,userdata, flags, rc):
		print("disconnect")

	def publish_message(self, message):
		if(self.mqttc != None):
			try :
				self.mqttc.publish(self.topic, message)
			except Exception as e :
				print(e)

	def new_topic(self, new_topic) :
		if(self.mqttc != None):
			try :
				self.mqttc.subscribe(new_topic, 0)
			except Exception as e :
				print(e)

	def connect_mqtt(self,connectedMessage):
		self.mqttc = mqtt.Client()
		# Assign event callbacks
		self.mqttc.on_message = self.on_message
		self.mqttc.on_connect = self.on_connect
		self.mqttc.on_publish = self.on_publish
		self.mqttc.on_subscribe = self.on_subscribe
		self.mqttc.on_disconnect = self.on_disconnect

		# Parse CLOUDMQTT_URL (or fallback to localhost)
		url_str = os.environ.get('CLOUDMQTT_URL', 'mqtt://'+self.username+':'+self.password+'@'+self.server+':'+self.port)
		url = urlparse(url_str)
		
		# Connect
		self.mqttc.username_pw_set(url.username, url.password)
		self.mqttc.connect(url.hostname, url.port)
		
		# Start subscribe, with QoS level 0
		self.mqttc.subscribe(self.topic, 0)
		
		# Publish a message
		self.mqttc.publish(self.topic, connectedMessage)
		
		# Continue the network loop, exit when an error occurs
		global stop_all_thread
		rc = 0
		while rc == 0:
			if stop_all_thread == True :
				break
			rc = self.mqttc.loop()
		#print("rc: " + str(rc))
		print('Disconnect '+self.userid+' mqtt server!') 


	def run(self):
		self.connect_mqtt('Ready')

'''
Simulasi data device user dari database 
'''
stack_threads = []
stack_threads.append(MQTT('device-6e5c4b13-357f-4a56-abb9-a22a23f2e505','m15.cloudmqtt.com','15944','hhrblzkj','5fDMVfZA3RM3','data-6e5c4b13-357f-4a56-abb9-a22a23f2e505'))
stack_threads.append(MQTT('device-8c3a6f39-ae11-4bd3-aa3e-849798682b5b','m15.cloudmqtt.com','12453','cdftjynm','Wh1y-lcwyjOt','data-8c3a6f39-ae11-4bd3-aa3e-849798682b5b'))
stack_threads.append(MQTT('device-ac40bacd-e959-408c-85c7-462cbb6b9af7','m15.cloudmqtt.com','15462','flrftgzs','R6sS1_dnYO-R','data-ac40bacd-e959-408c-85c7-462cbb6b9af7'))
stack_threads.append(MQTT('device-b391a4a4-77ff-46a9-9660-ae52b8e616d7','m15.cloudmqtt.com','12557','ugvhidfy','ZAuR8m0KZckW','data-b391a4a4-77ff-46a9-9660-ae52b8e616d7'))
stack_threads.append(MQTT('device-b7899de8-04c0-4789-aff6-0e699bbd6ea9','m15.cloudmqtt.com','18559','lqzgplpu','mCPjb5QJ7TVu','data-b7899de8-04c0-4789-aff6-0e699bbd6ea9'))
stack_threads.append(MQTT('device-ba216b9c-0ec0-4ef7-9955-ef7253d423ad','m15.cloudmqtt.com','11178','eggmgodl','vhskMSr8a0_o','data-ba216b9c-0ec0-4ef7-9955-ef7253d423ad'))
stack_threads.append(MQTT('device-d6970362-6a09-4327-a23a-db703155fb3a','m15.cloudmqtt.com','11966','vaexvvak','dlPZTJivz6w0','data-d6970362-6a09-4327-a23a-db703155fb3a'))
stack_threads.append(MQTT('device-de0603b4-5969-4598-bcca-64e3e11f22ae','m15.cloudmqtt.com','11184','louibnsk','5CiazAOgsT7y','data-de0603b4-5969-4598-bcca-64e3e11f22ae'))

if len(stack_threads) > 0 :
	try :
		for t in stack_threads :
			# Bukan pagar untuk mengaktifkan fungsi daemon
			# daemon berfungsi agar semua thread non-aktif ketika ditekan ctrl+c
			#t.daemon = True
			t.start()
	except Exception as e :
		print(e)

try:
	i = 0
	while 1:
		try :
			i += 1
			time.sleep(1)
			if(i > 5):
				stack_threads[0].publish_message('Hello World')
				i = 0
		except Exception as e :
			print(e)

except (KeyboardInterrupt, SystemExit):
	stop_all_thread = True # fungsi untuk keluar dari infinity While pada loop, dan proses selesai
	sys.exit()