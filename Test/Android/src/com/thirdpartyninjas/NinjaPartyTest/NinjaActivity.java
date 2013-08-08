package com.thirdpartyninjas.NinjaPartyTest;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import com.facebook.*;
import com.facebook.model.*;

import android.util.Log;

public class NinjaActivity extends Activity
{
	NinjaView ninjaView;

	protected static NinjaActivity instance = null;

	private UiLifecycleHelper lifecycleHelper;

	public static void FacebookLogin()
	{
		Log.i("NinjaPartyTest", "FacebookLogin");
		instance.facebookLogin();
	}

	public static void FacebookLogout()
	{
		Log.i("NinjaPartyTest", "FacebookLogout");
		instance.facebookLogout();
	}

	public static boolean FacebookIsLoggedIn()
	{
		Log.i("NinjaPartyTest", "FacebookIsLoggedIn");
		return instance.facebookIsLoggedIn();
	}

	public void facebookLogin()
	{
		if(facebookIsLoggedIn())
		{
			NinjaJni.facebookLogin(true, Session.getActiveSession().getAccessToken());
			return;
		}

		Session.openActiveSession(this, true, new Session.StatusCallback()
		{
			@Override
			public void call(Session session, SessionState state, Exception exception)
			{
				onSessionStateChanged(session, state, exception);
			}
		});
	}

	public void facebookLogout()
	{
		if(!facebookIsLoggedIn())
		{
			NinjaJni.facebookLogout();
			return;
		}

		Session.getActiveSession().closeAndClearTokenInformation();
	}

	public boolean facebookIsLoggedIn()
	{
		return (Session.getActiveSession() != null && Session.getActiveSession().isOpened());
	}

	@Override
	public void onBackPressed()
	{
		// do nothing on back button
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		lifecycleHelper.onPause();
		ninjaView.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		lifecycleHelper.onResume();
		ninjaView.onResume();
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		lifecycleHelper = new UiLifecycleHelper(this, new Session.StatusCallback()
		{
			@Override
			public void call(Session session, SessionState state, Exception exception)
			{
				onSessionStateChanged(session, state, exception);
			}
		});
		lifecycleHelper.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		ninjaView = new NinjaView(getApplication(), this);
		setContentView(ninjaView);
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		lifecycleHelper.onDestroy();
		ninjaView.Destroy();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
		lifecycleHelper.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		lifecycleHelper.onSaveInstanceState(outState);
	}

	private void onSessionStateChanged(Session session, SessionState state, Exception exception)
	{
		Log.i("NinjaPartyTest", "onSessionStateChanged");
		switch(state)
		{
			case CLOSED:
				Log.i("NinjaPartyTest", "CLOSED");
				NinjaJni.facebookLogout();
				break;

			case OPENED:
				Log.i("NinjaPartyTest", "OPENED");
				NinjaJni.facebookLogin(true, session.getAccessToken());
				break;

			case CLOSED_LOGIN_FAILED:
				Log.i("NinjaPartyTest", "CLOSED_LOGIN_FAILED");
				NinjaJni.facebookLogin(false, "");
				break;
		}
	}
}
