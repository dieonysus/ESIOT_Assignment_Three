package com.example.control_unit.services;

import com.example.control_unit.DataStorage;
import org.springframework.stereotype.Service;

import static com.example.control_unit.enums.Mode.AUTOMATIC;

@Service
public class DeviceIntegrationService {

    private final DataStorage dataStorage = DataStorage.getInstance();
    private final ArduinoService arduinoService;
    private final Esp32Service esp32Service;

    public DeviceIntegrationService(ArduinoService arduinoService, Esp32Service esp32Service) {
        this.arduinoService = arduinoService;
        this.esp32Service = esp32Service;
    }

    public void processTemperature(String payload) {
        float temperature = Float.parseFloat(payload);
        processTemperature(temperature);
        esp32Service.sendState();
        if (dataStorage.getMode() == AUTOMATIC) {
            dataStorage.calculateWindowPercentage(temperature);
            arduinoService.sendWindowPercentage(dataStorage.getWindowPercentage());
        } else {
            arduinoService.sendTemperature(temperature);
        }
    }

    private void processTemperature(float temperature) {
        dataStorage.addData(temperature);
        dataStorage.addToSum(temperature);
        dataStorage.increaseTemperatureCount();
        dataStorage.updateMinAndMax(temperature);
        dataStorage.updateState(temperature);
    }
}
