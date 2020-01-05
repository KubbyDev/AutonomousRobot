package com.mappingrobot.controlapp;

public class RobotTransform {

    public static double x, y, rotation = 0;
    private static final double SIZE = 1.5; // The size of the display of the robot

    public static void request() {
        Network.requestPosition();
    }

    public static void update(String receivedData) {
        String[] numbers = receivedData.split(";");
        x = Double.parseDouble(numbers[0]);
        y = Double.parseDouble(numbers[1]);
        rotation = Double.parseDouble(numbers[2]);
    }

    /** Returns the position of the 3 points forming the triangle that represents the robot */
    public static float[][] getPoints() {

        float[][] res = new float[3][2];
        double cos = Math.cos(rotation);
        double sin = Math.sin(rotation);

        //Calculates the position of the points
        //Front
        res[0][0] = (float) (cos*SIZE);
        res[0][1] = (float) (sin*SIZE);
        //Top Back
        res[1][0] = (float) ((-cos + sin)*SIZE);
        res[1][1] = (float) ((-sin - cos)*SIZE);
        //Bottom Back
        res[2][0] = (float) ((-cos - sin)*SIZE);
        res[2][1] = (float) ((-sin + cos)*SIZE);

        //Converts the positions in canvas coordinates
        for(float[] point : res) {
            point[0] = (float) (point[0] + x)*9;
            point[1] = (float) (point[1] + y)*9;
        }

        return res;
    }
}
