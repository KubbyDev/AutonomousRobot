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

        float pixelWidth = (float)Math.min(canvas.getWidth(), canvas.getHeight()) / RobotMap.SIZE;

        int i = canvas.getWidth();
        int j = canvas.getHeight();

        paint.setColor(Color.argb(255, 0, 0, 0));
        canvas.drawRect(0, 0, canvas.getWidth(), canvas.getHeight(), paint);
        paint.setColor(Color.argb(255, 127, 127, 127));
        canvas.drawRect(0, 0, RobotMap.SIZE*pixelWidth, RobotMap.SIZE*pixelWidth, paint);

        paint.setColor(Color.argb(255, 0, 0, 0));
        for(int y = 0; y < RobotMap.SIZE; y++)
            for(int x = 0; x < RobotMap.SIZE; x++)
                if(RobotMap.get(x, y))
                    canvas.drawRect(x*pixelWidth, y*pixelWidth,
                                    x*pixelWidth+pixelWidth, y*pixelWidth+pixelWidth,
                                    paint);

        // Draws the robot
        paint.setColor(Color.argb(255, 255, 0, 0));
        float[][] points = RobotTransform.getPoints(pixelWidth);
        Path path = new Path();
        path.setFillType(Path.FillType.EVEN_ODD);
        path.moveTo(points[0][0], points[0][1]);
        path.lineTo(points[1][0], points[1][1]);
        path.lineTo(points[2][0], points[2][1]);
        path.close();
        canvas.drawPath(path, paint);

        surfaceHolder.unlockCanvasAndPost(canvas);
    }
}
