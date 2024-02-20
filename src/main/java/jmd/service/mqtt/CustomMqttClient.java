package jmd.service.mqtt;


import lombok.Getter;
import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.nio.charset.StandardCharsets;
import java.util.UUID;

@Getter
public class CustomMqttClient {

    private static final String HOST = "ssl://domotich.es:8883";
    private static final String SUBSCRIBE_TOPIC = "ham/domotichdeck/out";
    private static final String PUBLISH_TOPIC = "ham/domotichdeck/in";

    private static final String PUBLISHER_ID_0 = UUID.randomUUID().toString();
    private static final String PUBLISHER_ID = "publisher-client-" + PUBLISHER_ID_0.substring(0, 4);
    private static final String SUBSCRIBER_ID = "subscriber-client-" + PUBLISHER_ID_0.substring(0, 4);
    private static final IMqttClient publisher;
    private static final IMqttClient subscriber;

    private static final MqttConnectOptions options = new MqttConnectOptions();

    static {
        try {
            publisher = new MqttClient(HOST, PUBLISHER_ID, new MemoryPersistence());
            subscriber = new MqttClient(HOST, SUBSCRIBER_ID, new MemoryPersistence());
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

    public CustomMqttClient() throws Exception {
        System.out.println("Starting Mqtt Client");

        options.setAutomaticReconnect(true);
        options.setCleanSession(true);
        options.setConnectionTimeout(10);
        options.setSocketFactory(SslUtil.getSocketFactory("certs/ca.crt",
                "certs/client.crt",
                "certs/client.key", ""));

        this.connectSubscriber();
        publisher.connect(options);
    }

    private void connectSubscriber() throws MqttException {
        subscriber.connect(options);

        subscriber.subscribe(SUBSCRIBE_TOPIC, (topic, msg) -> {
            byte[] payload = msg.getPayload();
            System.out.println(String.format("[I46] Message received: topic=%s, payload=%s", topic, new String(payload)));
        });
    }

    public boolean sendMessage(String topic, String message) throws MqttException {
        if ( !publisher.isConnected() || message == null) {
            return false;
        }
        MqttMessage msg = new MqttMessage(message.getBytes(StandardCharsets.UTF_8));
        msg.setQos(0);
        msg.setRetained(false);
        publisher.publish(topic,msg);
        return true;
    }

//    @PreDestroy
    public void close() throws MqttException {
        publisher.disconnect();
        subscriber.disconnect();
        publisher.close();
        subscriber.close();
    }

    public static void main(String[] args) throws Exception {
        CustomMqttClient client = new CustomMqttClient();

        boolean res = client.sendMessage(PUBLISH_TOPIC, "ChikitoLaCalzá");
        System.out.println("Publish result: " + res);

        boolean res2 = client.sendMessage(SUBSCRIBE_TOPIC, "ChikitoLaCalzá makina");
        System.out.println("Publish 2 result: " + res2);

        Thread.sleep(10000);
        client.close();
    }


}
