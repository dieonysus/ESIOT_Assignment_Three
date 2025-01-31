package com.example.control_unit.services;

import com.example.control_unit.DataStorage;
import com.example.control_unit.enums.Mode;
import com.example.control_unit.mqtt.MqttConfig;
import org.springframework.stereotype.Service;

@Service
public class TemperatureService {

    private final MqttConfig.MyGateway myGateway;
    private final DataStorage dataStorage = DataStorage.getInstance();
    private final ArduinoService arduinoService;

    public TemperatureService(MqttConfig.MyGateway myGateway, ArduinoService arduinoService) {
        this.myGateway = myGateway;
        this.arduinoService = arduinoService;
    }

    public void processTemperature(String payload) {
        try {
            float temperature = Float.parseFloat(payload);
            dataStorage.addData(temperature);
            dataStorage.addToSum(temperature);
            dataStorage.increaseTemperatureCount();
            dataStorage.updateMinAndMax(temperature);
            dataStorage.updateState(temperature);
            sendStateToEsp32();
            if (dataStorage.getMode() == Mode.AUTOMATIC) {
                dataStorage.calculateWindowPercentage(temperature);
                sendPercentageToArduino();
            } else {
                sendTemperatureToArduino(temperature);
            }

        } catch (NumberFormatException e) {
            System.err.println("Invalid payload: " + payload);
        }
    }


    public void sendStateToEsp32() {
        myGateway.sendToMqtt(dataStorage.getState().toString());
    }

    public void sendTemperatureToArduino(float temperature) {
        try {
            arduinoService.sendTemperature(temperature);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public void sendPercentageToArduino() {
        try {
            arduinoService.sendWindowPercentage(dataStorage.getWindowPercentage());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
