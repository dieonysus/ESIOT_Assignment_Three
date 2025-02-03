package com.example.control_unit.controllers;

import com.example.control_unit.enums.Mode;
import com.example.control_unit.enums.State;
import com.example.control_unit.services.ArduinoService;
import com.example.control_unit.services.DataStorageService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/data")
public class DashboardController {

    private final ArduinoService arduinoService;
    private final DataStorageService dataStorageService;

    public DashboardController(ArduinoService arduinoService, DataStorageService dataStorageService) {
        this.arduinoService = arduinoService;
        this.dataStorageService = dataStorageService;
    }

    @GetMapping("/temperatureQueue")
    public ResponseEntity<List<Float>> getTemperatureQueue() {
        return ResponseEntity.ok(dataStorageService.getTemperatureAsList());
    }

    @GetMapping("/averageTemperature")
    public ResponseEntity<Double> getAverageTemperature() {
        return ResponseEntity.ok(dataStorageService.calculateAverageTemperature());
    }

    @GetMapping("/maxTemperature")
    public ResponseEntity<Float> getMaxTemperature() {
        return ResponseEntity.ok(dataStorageService.getMax());
    }

    @GetMapping("/minTemperature")
    public ResponseEntity<Float> getMinTemperature() {
        return ResponseEntity.ok(dataStorageService.getMin());
    }

    @GetMapping("/state")
    public ResponseEntity<State> getState() {
        return ResponseEntity.ok(dataStorageService.getState());
    }

    @GetMapping("/mode")
    public ResponseEntity<Mode> getMode() {
        return ResponseEntity.ok(dataStorageService.getMode());
    }

    @GetMapping("/windowPercentage")
    public ResponseEntity<Integer> getWindowPercentage() {
        return ResponseEntity.ok(dataStorageService.getWindowPercentage());
    }

    @PostMapping("/toggleMode")
    public ResponseEntity<Mode> toggleMode() {
        Mode newMode = dataStorageService.toggleMode();
        arduinoService.sendMode(newMode.toString().toLowerCase());
        return ResponseEntity.ok(newMode);
    }

    @PostMapping("/resetState")
    public ResponseEntity<State> resetState() {
        return ResponseEntity.ok(dataStorageService.resetState());
    }

    @PostMapping("/setWindowPercentage")
    public ResponseEntity<String> setWindowPercentage(@RequestBody Map<String, Integer> request) throws InterruptedException {
        if (request.containsKey("percentage")) {
            int newPercentage = request.get("percentage");
            dataStorageService.setWindowPercentage(newPercentage);
            arduinoService.sendWindowPercentage(newPercentage);
            return ResponseEntity.ok("Window percentage updated to " + newPercentage);
        } else {
            return ResponseEntity.badRequest().body("Missing 'percentage' field");
        }
    }
}

