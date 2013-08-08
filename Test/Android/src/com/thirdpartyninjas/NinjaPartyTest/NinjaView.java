package com.thirdpartyninjas.NinjaPartyTest;

import java.util.ArrayList;
import java.util.List;

import java.nio.ByteBuffer;

import android.graphics.Bitmap;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;

import android.opengl.GLUtils;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

class NinjaView extends GLSurfaceView implements GLSurfaceView.Renderer
{
	protected long previousTime = 0;
	protected String apkPath;

    protected Object syncObject = new Object();
    protected boolean gamePaused = false;
    protected boolean lostWindowFocus = false;
    protected NinjaActivity ninjaActivity = null;

    protected static NinjaView instance = null;    

    public NinjaView(Context context, NinjaActivity ninjaActivity)
    {
        super(context);
        setEGLContextClientVersion(2);
        setRenderer(this);
        apkPath = context.getPackageCodePath();

        setFocusable(true);
        setFocusableInTouchMode(true);

        this.ninjaActivity = ninjaActivity;
        instance = this;
    }

    public void Destroy()
    {
        synchronized(syncObject)
        {
            NinjaJni.destroy();
        }
    }

    @Override // GLSurfaceView.Renderer
    public void onDrawFrame(GL10 gl)
    {
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
        gl.glClearColor(1, 0, 0, 1);

    	long currentTime = SystemClock.elapsedRealtime();
    	float deltaSeconds = (currentTime - previousTime) / 1000.0f;
    	previousTime = currentTime;

    	if(deltaSeconds > 1 / 30.0f)
    		deltaSeconds = 1 / 30.0f;
    	
        synchronized(syncObject)
        {
            if(!gamePaused)
                NinjaJni.update(deltaSeconds);
            NinjaJni.draw();
        }
    }

    @Override // GLSurfaceView.Renderer
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        synchronized(syncObject)
        {
            NinjaJni.setResolution(width, height);
        }
    }

    @Override // GLSurfaceView.Renderer
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        synchronized(syncObject)
        {
        	NinjaJni.init(getWidth(), getHeight(), apkPath);
        }
    }

    @Override
    public void onPause()
    {
        super.onPause();

        if(!gamePaused)
        {
            gamePaused = true;
            synchronized(syncObject)
            {
                NinjaJni.pause();
            }
        }
    }

    @Override
    public void onResume()
    {
        super.onResume();

        if(gamePaused && !lostWindowFocus)
        {
            gamePaused = false;
            synchronized(syncObject)
            {
                NinjaJni.resume();
            }
        }
    }

    @Override
    public void onWindowFocusChanged(boolean gainFocus)
    {
        super.onWindowFocusChanged(gainFocus);

        lostWindowFocus = !gainFocus;

        if(gainFocus && gamePaused)
        {
            gamePaused = false;
            synchronized(syncObject)
            {
                NinjaJni.resume();
            }
        }
    }
	
    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        super.onTouchEvent(event);

        int action = event.getAction() & MotionEvent.ACTION_MASK;
        int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
        int pointerId = event.getPointerId(pointerIndex);
        int pointerCount = event.getPointerCount();
        
        synchronized(syncObject)
        {
            switch(action)
            {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                NinjaJni.touchDown(pointerId, (int)event.getX(pointerIndex), (int)event.getY(pointerIndex));
                break;
                
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                NinjaJni.touchUp(pointerId, (int)event.getX(pointerIndex), (int)event.getY(pointerIndex));
                break;

            case MotionEvent.ACTION_CANCEL:
                for (int i = 0; i < pointerCount; i++)
                {
                    pointerIndex = i;
                    pointerId = event.getPointerId(pointerIndex);
                    NinjaJni.touchCancel(pointerId);
                }
                break;
                
            case MotionEvent.ACTION_MOVE:
                for (int i = 0; i < pointerCount; i++)
                {
                    pointerIndex = i;
                    pointerId = event.getPointerId(pointerIndex);
                    NinjaJni.touchMove(pointerId, (int)event.getX(pointerIndex), (int)event.getY(pointerIndex));
                }           
                break;
                
            default:
                break;
            }
        }
        
        return true;
    }
}
