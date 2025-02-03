package com.example.control_unit;

import com.example.control_unit.enums.Mode;
import com.example.control_unit.enums.State;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class DataStorage {
    private static final int QUEUE_SIZE = 10;

    private final BlockingQueue<Float> temperatureQueue = new LinkedBlockingQueue<>(QUEUE_SIZE);
    private double temperatureSum;
    private double temperatureCount;
    private float max = -50;
    private float min = 50;
    private int windowPercentage = 0;
    private State state = State.NORMAL;
    private Mode mode = Mode.AUTOMATIC;

    private static final DataStorage INSTANCE = new DataStorage();

    private DataStorage(){};

    public static DataStorage getInstance() {
        return INSTANCE;
    }

    public void addTemperatureToQueue(Float data) {
        if (temperatureQueue.size() == QUEUE_SIZE) {
            temperatureQueue.poll();
        }
        temperatureQueue.offer(data);
    }

    public double getTemperatureSum() {
        return temperatureSum;
    }

    public void setTemperatureSum(double temperatureSum) {
        this.temperatureSum = temperatureSum;
    }

    public double getTemperatureCount() {
        return temperatureCount;
    }

    public void setTemperatureCount(double temperatureCount) {
        this.temperatureCount = temperatureCount;
    }

    public float getMax() {
        return max;
    }

    public void setMax(float max) {
        this.max = max;
    }

    public float getMin() {
        return min;
    }

    public void setMin(float min) {
        this.min = min;
    }

    public int getWindowPercentage() {
        return windowPercentage;
    }

    public void setWindowPercentage(int windowPercentage) {
        this.windowPercentage = windowPercentage;
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    public Mode getMode() {
        return mode;
    }

    public void setMode(Mode mode) {
        this.mode = mode;
    }

    public BlockingQueue<Float> getTemperatureQueue() {
        return temperatureQueue;
    }
}