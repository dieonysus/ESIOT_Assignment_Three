package com.example.control_unit.Tasks;

import com.example.control_unit.serial.CommChannel;
import com.example.control_unit.services.ArduinoService;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

@Service
public class ArduinoTask {

    private final ArduinoService arduinoService;

    public ArduinoTask(ArduinoService arduinoService) {
        this.arduinoService = arduinoService;
    }

    @Scheduled(fixedRate = 100)
    public void monitorArduino() {
        try {
            CommChannel channel = arduinoService.getChannel();
            if (channel.isMsgAvailable()) {
                String msg = channel.receiveMsg();
                if (msg.startsWith("M:")) {
                    arduinoService.receiveMode(msg);
                } else if (msg.startsWith("P:")) {
                    arduinoService.receiveWindowPercentage(msg);
                } else {
                    System.out.println(msg);
                }
            }
        } catch (Exception e) {
            System.err.println("Error communicating with Arduino: " + e.getMessage());
        }
    }
}
