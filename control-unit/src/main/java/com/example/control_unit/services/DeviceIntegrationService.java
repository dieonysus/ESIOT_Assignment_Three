package com.example.control_unit.services;

import com.example.control_unit.enums.Mode;
import org.springframework.stereotype.Service;

import static com.example.control_unit.enums.Mode.AUTOMATIC;

@Service
public class DeviceIntegrationService {

    private final DataStorageService dataStorageService;
    private final ArduinoService arduinoService;
    private final Esp32Service esp32Service;

    public DeviceIntegrationService(ArduinoService arduinoService, Esp32Service esp32Service, DataStorageService dataStorageService) {
        this.dataStorageService = dataStorageService;
        this.arduinoService = arduinoService;
        this.esp32Service = esp32Service;
    }

    public void processTemperature(String payload) {
        float temperature = Float.parseFloat(payload);
        dataStorageService.processTemperature(temperature);
        esp32Service.sendState();
        Mode currentMode = dataStorageService.getMode();
        if (currentMode == AUTOMATIC) {
            int windowPercentage = dataStorageService.calculateWindowPercentage(temperature);
            arduinoService.sendWindowPercentage(windowPercentage);
        } else {
            arduinoService.sendTemperature(temperature);
        }
    }
}