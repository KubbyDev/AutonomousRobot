package com.mappingrobot.controlapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Network.init(this);

        // Starts a thread that updates the map periodically
        new Thread(new Runnable() {
            @Override
            public void run() {
                sleep(2000);
                while (true) {
                    sleep(500);
                    RobotTransform.request();
                    ((MapView) findViewById(R.id.mapview)).updateDisplay();
                    sleep(250);
                    RobotMap.request();
                    ((MapView) findViewById(R.id.mapview)).updateDisplay();
                    sleep(250);
                    RobotTransform.request();
                    ((MapView) findViewById(R.id.mapview)).updateDisplay();

                }
            }
        }).start();

        // Touch events
        View view = findViewById(R.id.mapview);
        view.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                // The side length of a pixel of the map
                float pixelWidth = ((MapView)v).getPixelWidth();

                // Avoids triggering the target change twice (when the user stops pressing)
                if (event.getActionMasked() != MotionEvent.ACTION_DOWN)
                    return false;

                // The pixel the user touched
                int x = (int)event.getX();
                int y = (int)event.getY();
                if(x > 0 && x < RobotMap.SIZE*pixelWidth && y > 0 && y < RobotMap.SIZE*pixelWidth)
                    Network.requestTargetChange(Math.round(x/pixelWidth), Math.round(y/pixelWidth));

                return false;
            }
        });
    }

    private static void sleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) { e.printStackTrace(); }
    }
}
