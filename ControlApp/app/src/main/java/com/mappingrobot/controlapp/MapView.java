package com.mappingrobot.controlapp;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MapView extends SurfaceView {

    private Paint paint;
    private SurfaceHolder surfaceHolder;

    public MapView(Context context, AttributeSet attr) {
        super(context, attr);
        paint = new Paint();
        paint.setStyle(Paint.Style.FILL_AND_STROKE);
        surfaceHolder = getHolder();
    }

    public void updateDisplay() {

        Canvas canvas = surfaceHolder.lockCanvas();

        if(canvas == null)
            return;

        paint.setColor(Color.argb(255, 127, 127, 127));
        canvas.drawRect(0,0,RobotMap.SIZE*9,RobotMap.SIZE*9,paint);

        paint.setColor(Color.argb(255, 0, 0, 0));
        for(int y = 0; y < RobotMap.SIZE; y++)
            for(int x = 0; x < RobotMap.SIZE; x++)
                if(RobotMap.get(x, y))
                    canvas.drawRect(x*9, y*9, x*9+9, y*9+9, paint);

        // Draws the robot
        paint.setColor(Color.argb(255, 255, 0, 0));
        float[][] points = RobotTransform.getPoints();
        Path path = new Path();
        path.setFillType(Path.FillType.EVEN_ODD);
        path.moveTo(points[0][0], points[0][1]);
        path.lineTo(points[1][0], points[1][1]);
        path.lineTo(points[2][0], points[2][1]);
        path.close();
        canvas.drawPath(path, paint);
        /*
        paint.setColor(Color.argb(255, 255, 255, 255));
        canvas.drawRect(points[0][0]-2, points[0][1]-2,
                points[0][0]+2, points[0][1]+2, paint);
        canvas.drawRect((int)RobotTransform.x*9-2, (int)RobotTransform.y*9-2,
                (int)RobotTransform.x*9+2, (int)RobotTransform.y*9+2, paint);
        */

        surfaceHolder.unlockCanvasAndPost(canvas);
    }
}
