package com.example.control_unit.services;

import com.example.control_unit.pojo.DataStorage;
import com.example.control_unit.enums.Mode;
import com.example.control_unit.serial.CommChannel;
import com.example.control_unit.serial.SerialCommChannel;
import jakarta.annotation.PostConstruct;
import org.springframework.stereotype.Service;

@Service
public class ArduinoService {

    private final DataStorage dataStorage = DataStorage.getInstance();
    private CommChannel channel;

    @PostConstruct
    public void init() throws Exception {
        this.channel = new SerialCommChannel("COM4", 9600);
        System.out.println("Waiting Arduino for rebooting...");
        Thread.sleep(4000);
        System.out.println("Arduino is ready.");
    }

    public void sendTemperature(float temperature) {
        channel.sendMsg("T:".concat(Float.toString(temperature)));
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public void sendMode(String mode) {
        channel.sendMsg("M:".concat(mode));
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public void receiveMode(String msg) throws Exception {
        String currentMode = dataStorage.getMode().toString().toLowerCase();
        String newMode = msg.split(":")[1];
        if (!newMode.equals(currentMode)) {
            if (newMode.equals("manual")) {
                dataStorage.setMode(Mode.MANUAL);
            } else {
                dataStorage.setMode(Mode.AUTOMATIC);
            }
        }
    }

    public void sendWindowPercentage(int windowPercentage) {
        channel.sendMsg("P:".concat(String.valueOf(windowPercentage)));
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public void receiveWindowPercentage(String msg) throws Exception {
        String newWindowPercentage = msg.split(":")[1];
        dataStorage.setWindowPercentage(Integer.parseInt(newWindowPercentage));
    }

    public CommChannel getChannel() {
        return channel;
    }
}
