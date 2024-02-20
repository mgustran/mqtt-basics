import logging
import os
import random
import ssl
import time

import paho.mqtt.client as mqtt

class MqttFunctions:
    def __init__(self):
        self.broker_address = "domotich.es"
        self.broker_port_secure = 8883
        self.broker_port = 1083
        self.client_id = "domodeck-client-" + str(random.randint(1000, 9999))
        self.subscribe_topic = "ham/domotichdeck/out"
        self.subscribe_topic_2 = "ham/domotichdeck/out2"
        self.publish_topic = "ham/domotichdeck/in"

        self.mqtt_client = None

    def publish_mqtt(self, topic, message):
        print("publishing into: " + topic + " with msg: " + message)
        self.mqtt_client.publish(topic, message)

    def on_message(self, client, userdata, message):
        print("payload type is: " + (message if type(message.payload) != bytes else "byte array"))
        msg = message.payload
        topic = str(message.topic)
        if type(message.payload) != bytes:
            print("message received ", str(msg.decode("utf-8")), " from topic ", topic)
            # logging.debug("message received " + str(msg.decode("utf-8")) + " from topic " + topic)
        else:
            print("message received (byte array)[", str(len(msg)), "] from topic ", topic)
            # logging.debug("message received (byte array)[" + str(len(msg)) + "] from topic " + topic)

    def create_mqtt_client(self, use_tls=False):

        # logging.debug("creating new mqtt instance with tls")
        print("creating new mqtt instance")
        self.mqtt_client = mqtt.Client(client_id=self.client_id, protocol=mqtt.MQTTv5)
        # self.mqtt_client.username_pw_set(os.getenv('M_MQTT_USERNAME'), password=os.getenv('M_MQTT_PASSWORD'))

        if use_tls:

            self.mqtt_client.tls_set(ca_certs="certs" + os.sep + "ca.crt",
                                     certfile="certs" + os.sep + "client.crt",
                                     keyfile="certs" + os.sep + "client.key",
                                     tls_version=ssl.PROTOCOL_TLS_CLIENT)
            # self.mqtt_client.tls_insecure_set(True)
            self.mqtt_client.tls_insecure_set(False)

        # self.mqtt_client.enable_logging(self.logging)
        self.mqtt_client.reconnect_delay_set(min_delay=1, max_delay=180)

        self.mqtt_client.on_connect = self.on_m_connect
        self.mqtt_client.on_disconnect = self.on_m_disconnect

        self.mqtt_client.on_message = self.on_message  # attach function to callback

        # logging.debug("connecting to broker")
        print("connecting to broker")
        if use_tls:
            self.mqtt_client.connect(self.broker_address, self.broker_port_secure)
        else:
            self.mqtt_client.connect(self.broker_address, self.broker_port)

        # logging.debug("Subscribing to topics " + self.subscribe_topic + ", " + self.subscribe_topic_2)
        print("Subscribing to topics " + self.subscribe_topic + ", " + self.subscribe_topic_2)
        self.mqtt_client.subscribe(self.subscribe_topic)
        self.mqtt_client.subscribe(self.subscribe_topic_2)

        self.mqtt_client.loop_start()

    def on_m_connect(self, client, userdata, flags, rc, aux):
        if str(rc) == "Success":
            # logging.info("Connected with Mqtt broker!")
            print("Connected with Mqtt broker!")
        else:
            # logging.warning(f"Error connecting with Mqtt broker! {rc}")
            print(f"Error connecting with Mqtt broker! {rc}")

    def on_m_disconnect(self, client, userdata, reason_code, ns):
        # logging.warning(f"Mqtt broker Disconnection error {reason_code}")
        print(f"Mqtt broker Disconnection error {reason_code}")

if __name__ == "__main__":
    functions = MqttFunctions()
    functions.create_mqtt_client(use_tls=True)
    # functions.create_mqtt_client(use_tls=False)

    functions.publish_mqtt(functions.publish_topic, "test message")

    while True:
        time.sleep(1)
