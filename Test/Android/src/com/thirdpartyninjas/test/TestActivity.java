package com.thirdpartyninjas.test;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.util.Log;

public class TestActivity extends Activity implements OnTouchListener
{
	TestView testView;
	
	@Override
	protected void onPause()
	{
		super.onPause();
		testView.onPause();
		TestJni.pause();
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		testView.onResume();
		TestJni.resume();
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		testView = new TestView(getApplication());
		testView.setOnTouchListener(this);
		setContentView(testView);
	}
	
	@Override
	public boolean onTouch(View v, MotionEvent event)
	{
		int action = event.getAction() & MotionEvent.ACTION_MASK;
		int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
		int pointerId = event.getPointerId(pointerIndex);
		int pointerCount = event.getPointerCount();
		
		switch(action)
		{
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_POINTER_DOWN:
			TestJni.touchDown(pointerId, (int)event.getX(pointerIndex), (int)event.getY(pointerIndex));
			break;
			
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_POINTER_UP:
			TestJni.touchUp(pointerId, (int)event.getX(pointerIndex), (int)event.getY(pointerIndex));
			break;

		case MotionEvent.ACTION_CANCEL:
			for (int i = 0; i < pointerCount; i++)
			{
				pointerIndex = i;
				pointerId = event.getPointerId(pointerIndex);
				TestJni.touchCancel(pointerId);
			}
			break;
			
		case MotionEvent.ACTION_MOVE:
			for (int i = 0; i < pointerCount; i++)
			{
				pointerIndex = i;
				pointerId = event.getPointerId(pointerIndex);
				TestJni.touchMove(pointerId, (int)event.getX(pointerIndex), (int)event.getY(pointerIndex));
			}			
			break;
			
		default:
			break;
		}
		
		return true;
	}
}
