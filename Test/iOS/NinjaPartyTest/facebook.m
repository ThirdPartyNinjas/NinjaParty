#import "ViewController.h"

void FacebookLogin()
{
	[[ViewController instance] FacebookLogin];
}

void FacebookLogout()
{
	[[ViewController instance] FacebookLogout];
}

bool FacebookIsLoggedIn()
{
	return [[ViewController instance] FacebookIsLoggedIn];
}
