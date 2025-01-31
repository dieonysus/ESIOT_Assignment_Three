package com.example.control_unit;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

import javax.xml.crypto.Data;
import java.util.Random;

@SpringBootApplication
@EnableScheduling
public class ControlUnitApplication {
	public static void main(String[] args) {
		SpringApplication.run(ControlUnitApplication.class, args);
	}
}


