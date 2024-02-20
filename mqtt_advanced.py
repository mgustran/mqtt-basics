import logging
import os
import random
import ssl
import threading
import time
from collections import deque

import paho.mqtt.client as mqtt

logger = logging.getLogger(__name__)


class MqttMessage:

    DEFAULT_SLEEP = 1.0

    def __init__(self, topic, message, sleep=DEFAULT_SLEEP):
        self.topic = topic
        self.message = message
        self.sleep = sleep


class MqttFunctions:

    DEFAULT_SLEEP = 1.0
    def __init__(self):
        self.stop = False

        self.broker_address = "domotich.es"
        self.broker_port_secure = 8883
        self.broker_port = 1083
        self.client_id = "domodeck-client-" + str(random.randint(1000, 9999))
        self.subscribe_topic = "ham/domotichdeck/out"
        self.subscribe_topic_finish = "ham/domotichdeck/finish"
        self.publish_topic = "ham/domotichdeck/in"
        self.publish_topic_with_reply = "ham/domotichdeck/reply-test"

        self.mqtt_client = None

        self.mqtt_publish_queue = deque()
        self.mqtt_reply_cache = {}

    def publish_mqtt_get_reply(self, topic, message, timeout=10) -> bytes | str:
        msg_id = random.randint(100000, 999999)
        print("publishing into: " + topic + "/" + str(msg_id) + " and waiting " + str(timeout) + " sec. for reply")
        self.mqtt_client.publish(topic + "/" + str(msg_id), message)
        start_time = time.time()
        result = None
        while self.stop is not True and (time.time() - start_time) < timeout:
            if msg_id in self.mqtt_reply_cache:
                result = self.mqtt_reply_cache[msg_id]
                del self.mqtt_reply_cache[msg_id]
                break
            time.sleep(0.1)
        return result

    def publish_mqtt(self, topic, message):
        print("publishing into: " + topic + " with msg: " + message)
        self.mqtt_client.publish(topic, message)

    def publish_mqtt_into_queue(self, topic, message, sleep=DEFAULT_SLEEP, skip_queue=False):
        print("publishing queued message into: " + topic + " - sleep: " + str(sleep) + ", skip:" + str(skip_queue) + ", msg: " + message)
        if skip_queue:
            self.mqtt_publish_queue.appendleft(MqttMessage(topic, message, sleep))
        else:
            self.mqtt_publish_queue.append(MqttMessage(topic, message, sleep))

    def on_message(self, client, userdata, message):
        msg = message.payload
        topic = str(message.topic)
        if type(message.payload) != bytes:
            print("message received ", str(msg.decode("utf-8")), " from topic ", topic)
        else:
            print("message received (byte array)[", str(len(msg)), "] from topic ", topic)

        if topic.startswith(self.subscribe_topic_finish):
            msg_id = int(topic[topic.rindex("/") + 1:])
            self.mqtt_reply_cache[msg_id] = msg

        elif topic.startswith(self.publish_topic_with_reply):
            print("reply to finish simulating other client")
            msg_id = int(topic[topic.rindex("/") + 1:])
            self.publish_mqtt(self.subscribe_topic_finish + '/' + str(msg_id), "this is a reply for id " + str(msg_id))

    def mqtt_publish_queue_func(self):
        while self.stop is not True:
            if len(self.mqtt_publish_queue) > 0:
                item = self.mqtt_publish_queue.popleft()
                self.mqtt_client.publish(item.topic, item.message)
                time.sleep(item.sleep)
            else:
                time.sleep(0.1)

    def create_mqtt_client(self, use_tls=False, username=None, password=None):

        print("creating blocking queue for mqtt publishing")
        threading.Thread(target=self.mqtt_publish_queue_func, daemon=True).start()

        print("creating new mqtt instance" + (" with tls" if use_tls else ""))
        self.mqtt_client = mqtt.Client(client_id=self.client_id, protocol=mqtt.MQTTv5)

        if username is not None and password is not None:
            self.mqtt_client.username_pw_set(username=username, password=password)

        if use_tls:

            self.mqtt_client.tls_set(ca_certs="certs" + os.sep + "ca.crt",
                                     certfile="certs" + os.sep + "client.crt",
                                     keyfile="certs" + os.sep + "client.key",
                                     tls_version=ssl.PROTOCOL_TLS_CLIENT)
            # self.mqtt_client.tls_insecure_set(True)
            self.mqtt_client.tls_insecure_set(False)

        self.mqtt_client.reconnect_delay_set(min_delay=1, max_delay=180)

        self.mqtt_client.on_connect = self.on_m_connect
        self.mqtt_client.on_disconnect = self.on_m_disconnect

        self.mqtt_client.on_message = self.on_message

        print("connecting to broker")
        if use_tls:
            self.mqtt_client.connect(self.broker_address, self.broker_port_secure)
        else:
            self.mqtt_client.connect(self.broker_address, self.broker_port)

        print("Subscribing to topics " + self.subscribe_topic + ", " + self.subscribe_topic_finish)
        self.mqtt_client.subscribe(self.subscribe_topic)
        self.mqtt_client.subscribe(self.subscribe_topic_finish + '/+')

        # Test Topic to simulate other client replying message
        self.mqtt_client.subscribe(self.publish_topic_with_reply + '/+')

        self.mqtt_client.loop_start()

    def on_m_connect(self, client, userdata, flags, rc, aux):
        if str(rc) == "Success":
            print("Connected with Mqtt broker!")
        else:
            print(f"Error connecting with Mqtt broker! {rc}")

    def on_m_disconnect(self, client, userdata, reason_code, ns):
        print(f"Mqtt broker Disconnection error {reason_code}")

if __name__ == "__main__":
    functions = MqttFunctions()

    # Create Client
    functions.create_mqtt_client(use_tls=True)
    # functions.create_mqtt_client(use_tls=True, username="domotich", password="some-password")
    # functions.create_mqtt_client(use_tls=False)

    # Publish simple message
    functions.publish_mqtt(functions.publish_topic, "test message 0")

    # Publish message using threaded queue
    functions.publish_mqtt_into_queue(functions.publish_topic, "test message 1", sleep=2)
    functions.publish_mqtt_into_queue(functions.publish_topic, "test message 2", skip_queue=True)

    # Publish message and get reply, (in a real scenario, reply will be responded by other client)
    reply = functions.publish_mqtt_get_reply(functions.publish_topic_with_reply, "test message", timeout=5)
    print("reply is: \"" + reply.decode('utf-8') + "\"")

    while True:
        time.sleep(1)