package com.example.control_unit.services;

import com.example.control_unit.pojo.DataStorage;
import com.example.control_unit.enums.Mode;
import com.example.control_unit.enums.State;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class DataStorageService {

    DataStorage dataStorage = DataStorage.getInstance();

    private static final float TEMP_THRESHOLD_LOW = 30;
    private static final float TEMP_THRESHOLD_HIGH = 40;
    private static final long ALARM_DELAY = 5000;
    private long tooHotStartTime;


    public void processTemperature(float temperature) {
        addTemperatureToQueue(temperature);
        increaseTemperatureSum(temperature);
        increaseTemperatureCount();
        updateMinAndMax(temperature);
        updateState(temperature);
    }


    public synchronized Mode toggleMode() {
        Mode currentMode = dataStorage.getMode();
        if (currentMode == Mode.MANUAL) {
            currentMode = Mode.AUTOMATIC;
        } else {
            currentMode = Mode.MANUAL;
        }
        return currentMode;
    }


    public State resetState() {
        State normalState = State.NORMAL;
        dataStorage.setState(normalState);
        return normalState;
    }


    private void updateState(float temperature) {
        State state = dataStorage.getState();
        if (state == State.ALARM) {
            return;
        }
        if (temperature < TEMP_THRESHOLD_LOW) {
            state = State.NORMAL;
        } else if (temperature >= TEMP_THRESHOLD_LOW && temperature <= TEMP_THRESHOLD_HIGH) {
            state = State.HOT;
        } else if (temperature > TEMP_THRESHOLD_HIGH && state != State.TOO_HOT) {
            state = State.TOO_HOT;
            tooHotStartTime = System.currentTimeMillis();
        }
        if (state == State.TOO_HOT) {
            long elapsedTime = System.currentTimeMillis() - tooHotStartTime;
            if (elapsedTime >= ALARM_DELAY) {
                state = State.ALARM;
            }
        }
        dataStorage.setState(state);
    }


    public int calculateWindowPercentage(float temperature) {
        int windowPercentage = 0;
        float t = temperature;
        float t1 = TEMP_THRESHOLD_LOW;
        float t2 = TEMP_THRESHOLD_HIGH;
        if (t < t1) {
            windowPercentage = 0;
        } else if (t1 <= t && t <= t2) {
            windowPercentage = (int) (((t - t1) / (t2 - t1)) * 99 + 1);
        } else {
            windowPercentage = 100;
        }
        dataStorage.setWindowPercentage(windowPercentage);
        return windowPercentage;
    }


    private void updateMinAndMax(float temperature) {
        float max = dataStorage.getMax();
        float min = dataStorage.getMin();
        if (temperature > max) {
            dataStorage.setMax(temperature);
        }
        if (temperature < min) {
            dataStorage.setMin(temperature);
        }
    }


    private void increaseTemperatureCount() {
        double newTemperatureCount = dataStorage.getTemperatureCount() + 1;
        dataStorage.setTemperatureCount(newTemperatureCount);
    }


    private void increaseTemperatureSum(float temperature) {
        double newTemperatureSum = dataStorage.getTemperatureSum() + (double) temperature;
        dataStorage.setTemperatureSum(newTemperatureSum);
    }


    public double calculateAverageTemperature() {
        double temperatureCount = dataStorage.getTemperatureCount();
        double temperatureSum = dataStorage.getTemperatureSum();
        return temperatureCount == 0 ? 0 : temperatureSum / temperatureCount;
    }


    private void addTemperatureToQueue(float temperature) {
        dataStorage.addTemperatureToQueue(temperature);
    }

    public Mode getMode() {
        return dataStorage.getMode();
    }

    public List<Float> getTemperatureAsList() {
        return new ArrayList<>(dataStorage.getTemperatureQueue());
    }

    public float getMax() {
        return dataStorage.getMax();
    }

    public float getMin() {
        return dataStorage.getMin();
    }

    public State getState() {
        return dataStorage.getState();
    }

    public int getWindowPercentage() {
        return dataStorage.getWindowPercentage();
    }

    public void setWindowPercentage(int windowPercentage) {
        dataStorage.setWindowPercentage(windowPercentage);
    }
}
