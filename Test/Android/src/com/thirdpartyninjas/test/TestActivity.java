package com.thirdpartyninjas.test;

import android.os.Bundle;
import android.app.Activity;

public class TestActivity extends Activity
{
	TestView testView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		testView = new TestView(getApplication());
		setContentView(testView);
	}
}
