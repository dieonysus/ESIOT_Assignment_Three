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
    public ResponseEntity<List<Float>> getTemperatureQueue() {
        return ResponseEntity.ok(dataStorage.getTemperatureAsList());
    }

    @GetMapping("/averageTemperature")
    public ResponseEntity<Double> getAverageTemperature() {
        return ResponseEntity.ok(dataStorage.getAverageTemperature());
    }

    @GetMapping("/maxTemperature")
    public ResponseEntity<Float> getMaxTemperature() {
        return ResponseEntity.ok(dataStorage.getMax());
    }

    @GetMapping("/minTemperature")
    public ResponseEntity<Float> getMinTemperature() {
        return ResponseEntity.ok(dataStorage.getMin());
    }

    @GetMapping("/state")
    public ResponseEntity<State> getState() {
        return ResponseEntity.ok(dataStorage.getState());
    }

    @GetMapping("/mode")
    public ResponseEntity<Mode> getMode() {
        return ResponseEntity.ok(dataStorage.getMode());
    }

    @GetMapping("/windowPercentage")
    public ResponseEntity<Integer> getWindowPercentage() {
        return ResponseEntity.ok(dataStorage.getWindowPercentage());
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

