package com.example.control_unit.controllers;

import com.example.control_unit.DataStorage;
import com.example.control_unit.enums.Mode;
import com.example.control_unit.enums.State;
import com.example.control_unit.services.ArduinoService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/data")
public class GraphController {

    private final DataStorage dataStorage = DataStorage.getInstance();
    private final ArduinoService arduinoService;

    public GraphController(ArduinoService arduinoService) {
        this.arduinoService = arduinoService;
    }

    @GetMapping("/temperatureQueue")
    public List<Float> getTemperatureQueue() {
        List<Float> latestTemperatureValues = DataStorage.getInstance().getDataAsList();
        return latestTemperatureValues;
    }

    @GetMapping("/averageTemperature")
    public double getAverageTemperature() {
        return dataStorage.getAverageTemperature();
    }

    @GetMapping("/maxTemperature")
    public double getMaxTemperature() {
        return dataStorage.getMax();
    }

    @GetMapping("/minTemperature")
    public double getMinTemperature() {
        return dataStorage.getMin();
    }

    @GetMapping("/state")
    public State getState() {
        return dataStorage.getState();
    }

    @PostMapping("/toggleMode")
    public ResponseEntity<Mode> toggleMode() {
        Mode newMode = dataStorage.toggleMode();
        arduinoService.sendMode(newMode.toString().toLowerCase());
        return ResponseEntity.ok(newMode);
    }

    @PostMapping("/resetState")
    public State resetState() {
        State normalState = State.NORMAL;
        dataStorage.setState(normalState);
        return normalState;
    }

    @GetMapping("/mode")
    public ResponseEntity<String> getMode() {
        return ResponseEntity.ok(dataStorage.getMode().toString());
    }

    @GetMapping("/windowPercentage")
    public int getWindowPercentage() {
        return dataStorage.getWindowPercentage();
    }

    @PostMapping("/setWindowPercentage")
    public ResponseEntity<String> setWindowPercentage(@RequestBody Map<String, Integer> request) throws InterruptedException {
        if (request.containsKey("percentage")) {
            int newPercentage = request.get("percentage");
            dataStorage.setWindowPercentage(newPercentage);
            arduinoService.sendWindowPercentage(newPercentage);
            return ResponseEntity.ok("Window percentage updated to " + newPercentage);
        } else {
            return ResponseEntity.badRequest().body("Missing 'percentage' field");
        }
    }
}

