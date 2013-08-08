#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAudioSession.h>
#import <Foundation/NSNotification.h>
#import <UIKit/UIKit.h>

#import <FacebookSDK/FacebookSDK.h>

#import "ViewController.h"

#define TEST_FACEBOOK

#if defined(TEST_EMPTYGAME)
#include "../../Tests/EmptyGame.hpp"
#elif defined(TEST_SPRITEBATCH)
#include "../../Tests/SpriteBatch.hpp"
#elif defined(TEST_SPINE)
#include "../../Tests/Spine.hpp"
#elif defined(TEST_DEMINA)
#include "../../Tests/Demina.hpp"
#elif defined(TEST_RENDERTEXTURE)
#include "../../Tests/RenderTexture.hpp"
#elif defined(TEST_AUDIO)
#include "../../Tests/Audio.hpp"
#elif defined(TEST_TOUCH)
#include "../../Tests/Touch.hpp"
#elif defined(TEST_ASSETS)
#include "../../Tests/Assets.hpp"
#elif defined(TEST_HTTPREQUEST)
#include "../../Tests/HttpRequest.hpp"
#elif defined(TEST_FACEBOOK)
#include "../../Tests/Facebook.hpp"
#endif

static ViewController *globalInstance = nil;

@interface ViewController ()
{
	EAGLContext *context;
    Tests::TestGame *game;
}
@end

@implementation ViewController

+ (ViewController*)instance
{
	return globalInstance;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	NSError *setCategoryError = nil;
	
	AVAudioSession *audioSession = [AVAudioSession sharedInstance];
	[audioSession setCategory: AVAudioSessionCategoryAmbient error: &setCategoryError];

	float osVersion = [[UIDevice currentDevice].systemVersion floatValue];
	if(osVersion >= 6.0f)
	{
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(interruptionHandler:) name:AVAudioSessionInterruptionNotification object:nil];
	}
	else
	{
        // note: left here for osVersion < 6.0
		audioSession.delegate = self;
	}

	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if(!context)
	{
		NSLog(@"Failed to create OpenGLES2 context");
	}

	globalInstance = self;
	
	GLKView *view = (GLKView*)self.view;
	view.context = context;
	view.delegate = self;
	self.preferredFramesPerSecond = 60;
	
    // todo: if you are running in landscape mode, change this
	// these are reversed because we are only in landscape mode and bounds returns portrait dimensions
	int screenWidth = (int)[UIScreen mainScreen].bounds.size.height * [UIScreen mainScreen].scale;
	int screenHeight = (int)[UIScreen mainScreen].bounds.size.width * [UIScreen mainScreen].scale;
	view.contentScaleFactor = [UIScreen mainScreen].scale;
	
	[EAGLContext setCurrentContext:context];
	[view bindDrawable];
	glEnable(GL_BLEND);
    
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(respondToTapGesture:)];
    tapRecognizer.numberOfTapsRequired = 1;
    [view addGestureRecognizer:tapRecognizer];
	
	game = new Tests::TestGame(screenWidth, screenHeight);
	game->LoadContent("Assets/", "Assets/Assets.zip");
	
	[self openFacebookSession:NO];
}

// This function was deprecated in iOS 6.0, but we need it for versions < 6.0
// todo: Update this to the correct orientations for your app!
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
	if(toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
	   toInterfaceOrientation == UIInterfaceOrientationLandscapeRight)
		return YES;
	return NO;
}

- (void)interruptionHandler:(NSNotification*)notification
{
	NSUInteger interruptionType = [[[notification userInfo] objectForKey:AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
	
    if (interruptionType == AVAudioSessionInterruptionTypeBegan)
        [self beginInterruption];
    else if (interruptionType == AVAudioSessionInterruptionTypeEnded)
        [self endInterruption];
}

- (void)viewDidUnload
{
	game->UnloadContent();
	delete game;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)update
{
	game->Update((float)self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	game->Draw();
}

- (void)beginInterruption
{
	game->BeginAudioInterruption();
}

- (void)endInterruption
{
	game->EndAudioInterruption();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	float scale = [UIScreen mainScreen].scale;
	for(UITouch *touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
		game->TouchBegan(touch, touch.tapCount, (int)(touchPoint.x * scale), (int)(touchPoint.y * scale));
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	float scale = [UIScreen mainScreen].scale;
	for(UITouch *touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
		game->TouchEnded(touch, (int)(touchPoint.x * scale), (int)(touchPoint.y * scale));
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	float scale = [UIScreen mainScreen].scale;
	for(UITouch *touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
		game->TouchMoved(touch, (int)(touchPoint.x * scale), (int)(touchPoint.y * scale));
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(UITouch *touch in touches)
	{
		game->TouchCancelled(touch);
	}
}

- (IBAction)respondToTapGesture:(UITapGestureRecognizer*)recognizer
{
	float scale = [UIScreen mainScreen].scale;
    CGPoint touchPoint = [recognizer locationInView:self.view];
    game->HandleTap((int)(touchPoint.x * scale), (int)(touchPoint.y * scale));
}

- (void) facebookSessionStateChanged:(FBSession*)session state:(FBSessionState)state error:(NSError*)error
{
	switch(state)
	{
		case FBSessionStateClosed:
			// logout
			[FBSession.activeSession closeAndClearTokenInformation];
			[FBSession setActiveSession:nil];
			game->FacebookLogout();
			break;
			
		case FBSessionStateOpen:
			// login
			game->FacebookLogin(true, [[[[FBSession activeSession] accessTokenData] accessToken] UTF8String]);
			break;
			
		case FBSessionStateClosedLoginFailed:
			// login failed/cancelled
			game->FacebookLogin(false, "");
			break;
			
		default:
			break;
	}
}

- (void) openFacebookSession:(BOOL)allowUI
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[FBSession openActiveSessionWithReadPermissions:nil allowLoginUI:allowUI completionHandler:^(FBSession *session, FBSessionState state, NSError *error)
		 {
			 [self facebookSessionStateChanged:session state:state error:error];
		 }];
	});
}

- (void) FacebookLogin
{
	if([self FacebookIsLoggedIn])
		game->FacebookLogin(true, [[[[FBSession activeSession] accessTokenData] accessToken] UTF8String]);
	else
		[self openFacebookSession:YES];
}

- (void) FacebookLogout
{
	if([self FacebookIsLoggedIn])
	{
		[FBSession.activeSession closeAndClearTokenInformation];
		[FBSession setActiveSession:nil];
	}
	else
	{
		game->FacebookLogout();
	}
}

- (bool) FacebookIsLoggedIn
{
	return [FBSession.activeSession isOpen];
}

@end
