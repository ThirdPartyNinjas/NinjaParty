package com.thirdpartyninjas.NinjaPartyTest;

public class NinjaJni
{
	static
	{
		System.loadLibrary("NinjaPartyTest");
	}
	
	public static native void init(int width, int height, String apkPath);
	public static native void destroy();

	public static native void setResolution(int width, int height);

	public static native void update(float deltaSeconds);
	public static native void draw();

	public static native void pause();
	public static native void resume();
	
	public static native void touchDown(int id, int x, int y);
	public static native void touchUp(int id, int x, int y);
	public static native void touchMove(int id, int x, int y);
	public static native void touchCancel(int id);
}
