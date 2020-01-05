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

        // Starts a thread that calls update() every 200 milliseconds (2 seconds after startup)
        new Thread(new Runnable() {
            @Override
            public void run() {
                try { Thread.sleep(2000); } catch (InterruptedException e) { e.printStackTrace(); }
                while (true) {
                    RobotMap.requestNextChunk();
                    try { Thread.sleep(500); } catch (InterruptedException e) { e.printStackTrace(); }
                    RobotTransform.request();
                    try { Thread.sleep(500); } catch (InterruptedException e) { e.printStackTrace(); }
                    ((MapView) findViewById(R.id.mapview)).updateDisplay();
                }
            }
        }).start();

        View view = findViewById(R.id.mapview);
        view.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getActionMasked() != MotionEvent.ACTION_DOWN)
                    return false;
                int x = (int)event.getX();
                int y = (int)event.getY();
                if(x > 0 && x < RobotMap.SIZE*9 && y > 0 && y < RobotMap.SIZE*9)
                    Network.requestTargetChange(x/9, y/9);
                return false;
            }
        });
    }
}
