package com.example.control_unit;

import com.example.control_unit.enums.Mode;
import com.example.control_unit.enums.State;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class DataStorage {
    private static final int MAX_SIZE = 10;
    private final long alarmDelay = 5000;
    private final float temperatureThresholdFirst = 30;
    private final float temperatureThresholdSecond = 40;

    private final BlockingQueue<Float> temperatureQueue = new LinkedBlockingQueue<>(MAX_SIZE);
    private double temperatureSum;
    private double temperatureCount;
    private float max = 0;
    private float min = 50;
    private int windowPercentage = 0;
    private long tooHotStartTime;
    private State state = State.NORMAL;
    private Mode mode = Mode.AUTOMATIC;

    private static final DataStorage INSTANCE = new DataStorage();

    private DataStorage(){};

    public static DataStorage getInstance() {
        return INSTANCE;
    }

    public void addData(Float data) {
        if (temperatureQueue.size() == MAX_SIZE) {
            temperatureQueue.poll();
        }
        temperatureQueue.offer(data);
    }

    public List<Float> getDataAsList() {
        return new ArrayList<>(temperatureQueue);
    }

    public void addToSum(float temperature) {
        temperatureSum += (double) temperature;
    }

    public void increaseTemperatureCount() {
        temperatureCount++;
    }

    public double getAverageTemperature() {
        return temperatureCount == 0 ? 9.9 : temperatureSum / temperatureCount;
    }

    public float getMax() {
        return max;
    }

    public float getMin() {
        return min;
    }

    public void updateMinAndMax(float temperature) {
        if (temperature > max) {
            max = temperature;
        }
        if (temperature < min) {
            min = temperature;
        }
    }

    public int getWindowPercentage() {
        return windowPercentage;
    }

    public void setWindowPercentage(int windowPercentage) {
        this.windowPercentage = windowPercentage;
    }

    public void calculateWindowPercentage(float temperature) {
        float t = temperature;
        float t1 = temperatureThresholdFirst;
        float t2 = temperatureThresholdSecond;
        if (t < t1) {
            windowPercentage = 0;
        } else if (t1 <= t && t <= t2) {
            windowPercentage = (int) (((t - t1) / (t2 - t1)) * 99 + 1);
        } else if (t > t2){
            windowPercentage = 100;
        }
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    public void updateState(float temperature) {
        if (state == State.ALARM) {
            return;
        }
        if (temperature < temperatureThresholdFirst) {
            state = State.NORMAL;
        } else if (temperature >= temperatureThresholdFirst && temperature <= temperatureThresholdSecond) {
            state = State.HOT;
        } else if (temperature > temperatureThresholdSecond && state != State.TOO_HOT) {
            state = State.TOO_HOT;
            tooHotStartTime = System.currentTimeMillis();
        }
        if (state == State.TOO_HOT) {
            long elapsedTime = System.currentTimeMillis() - tooHotStartTime;
            if (elapsedTime >= alarmDelay) {
                state = State.ALARM;

            }
        }
    }

    public Mode getMode() {
        return mode;
    }

    public void setMode(Mode mode) {
        this.mode = mode;
    }

    public synchronized Mode toggleMode() {
        if (mode == Mode.MANUAL) {
            mode = Mode.AUTOMATIC;
        } else {
            mode = Mode.MANUAL;
        }
        return mode;
    }
}
