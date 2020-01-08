package com.mappingrobot.controlapp;

// Contains and manages the values of the intern map of the robot (gets and stores them)
public class RobotMap {

    public static int SIZE = 72;
    private static boolean[] values = new boolean[SIZE * SIZE];

    public static void fill(boolean val) {
        for(int i = 0; i < SIZE*SIZE; i++)
            values[i] = val;
    }

    public static boolean get(int x, int y) {
        return values[y*SIZE + x];
    }

    public static void set(int x, int y, boolean val) {
        values[y*SIZE + x] = val;
    }

    public static void request() {
        Network.requestMap();
    }

    public static void updateMap(String receivedData) {

        // For each hexadecimal value
        for(int i = 0; i < 72*9*2; i++) {
            // Updates the 4 pixels this value represents
            int value = Integer.parseInt(receivedData.charAt(i)+"", 16);
            values[i*4    ] =  value   /8 == 1;
            values[i*4 + 1] = (value%8)/4 == 1;
            values[i*4 + 2] = (value%4)/2 == 1;
            values[i*4 + 3] =  value%2    == 1;
        }
    }
}
