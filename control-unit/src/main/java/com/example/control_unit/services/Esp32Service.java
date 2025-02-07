package com.example.control_unit.services;

import com.example.control_unit.pojo.DataStorage;
import com.example.control_unit.mqtt.MqttConfig;
import org.springframework.stereotype.Service;

@Service
public class Esp32Service {

    private final DataStorage dataStorage = DataStorage.getInstance();
    private final MqttConfig.MyGateway myGateway;

    public Esp32Service(MqttConfig.MyGateway myGateway) {
        this.myGateway = myGateway;
    }

    public void sendState() {
        myGateway.sendToMqtt(dataStorage.getState().toString());
    }
}