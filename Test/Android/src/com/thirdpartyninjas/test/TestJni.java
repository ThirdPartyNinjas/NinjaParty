package com.thirdpartyninjas.test;

public class TestJni
{
	static
	{
		System.loadLibrary("Test");
	}
	
	public static native void init(int width, int height, String apkPath);
	public static native void update(float deltaSeconds);
	public static native void draw();
}
