package com.thirdpartyninjas.NinjaPartyTest;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import android.os.Handler;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.view.View;
import android.os.Looper;

import android.opengl.GLUtils;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

public class NinjaActivity extends Activity
{
	NinjaView ninjaView;

	@Override
	public void onBackPressed()
	{
		// do nothing on back button
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		ninjaView.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		ninjaView.onResume();
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		ninjaView = new NinjaView(getApplication(), this);
		setContentView(ninjaView);
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		ninjaView.Destroy();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
	}
}
