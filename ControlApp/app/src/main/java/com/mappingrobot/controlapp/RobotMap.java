package com.mappingrobot.controlapp;

// Contains and manages the values of the intern map of the robot (gets and stores them)
public class RobotMap {

    public static int SIZE = 72;
    private static boolean[] values = new boolean[SIZE * SIZE];
    private static int nextChunkIndex = 0;

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

    public static void requestNextChunk() {
        Network.requestMapChunk(nextChunkIndex);
        nextChunkIndex = (nextChunkIndex+1)%24;
    }

    public static void updateMapChunk(int chunkIndex, String receivedData) {

        //Gets all the byte values. Each values takes 3 digits and their are 3 lines of 9 bytes
        int[] bytes = new int[3*9];
        for(int i = 0; i < 3*9; i++)
            bytes[i] = Integer.parseInt(receivedData.substring(3*i, 3*i+3));

        int offset = chunkIndex * 3 * 72;
        for(int i = 0; i < 3*9; i++) {

            int b = bytes[i];
            for(int valIndex = 7; valIndex >= 0; valIndex--) {
                values[offset + i*8 + valIndex] = b%2 == 1;
                b /= 2;
            }
        }
    }
}
