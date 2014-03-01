#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAudioSession.h>
#import <Foundation/NSNotification.h>
#import <UIKit/UIKit.h>

#import <FacebookSDK/FacebookSDK.h>

#import "ViewController.h"

#include <NinjaParty/TouchEvents.hpp>
#include <NinjaParty/GestureEvents.hpp>

#define TEST_SPINE

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
#elif defined(TEST_FONT)
#include "../../Tests/Font.hpp"
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
    
    // todo: disable if you don't want multitouch
    view.multipleTouchEnabled = true;
    
    // todo: register for the gestures we care about
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(respondToTapGesture:)];
    tapRecognizer.numberOfTapsRequired = 1;
    tapRecognizer.numberOfTouchesRequired = 2;
    [view addGestureRecognizer:tapRecognizer];
    
    tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(respondToTapGesture:)];
    tapRecognizer.numberOfTapsRequired = 1;
    tapRecognizer.numberOfTouchesRequired = 1;
    [view addGestureRecognizer:tapRecognizer];
    
    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(respondToPinchGesture:)];
    [view addGestureRecognizer:pinchRecognizer];

    UISwipeGestureRecognizer *swipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(respondToSwipeGesture:)];
    swipeRecognizer.direction = UISwipeGestureRecognizerDirectionLeft;
    [view addGestureRecognizer:swipeRecognizer];

    swipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(respondToSwipeGesture:)];
    swipeRecognizer.direction = UISwipeGestureRecognizerDirectionRight;
    [view addGestureRecognizer:swipeRecognizer];

    swipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(respondToSwipeGesture:)];
    swipeRecognizer.direction = UISwipeGestureRecognizerDirectionUp;
    [view addGestureRecognizer:swipeRecognizer];
    
    swipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(respondToSwipeGesture:)];
    swipeRecognizer.direction = UISwipeGestureRecognizerDirectionDown;
    [view addGestureRecognizer:swipeRecognizer];
    
    UIRotationGestureRecognizer *rotationRecognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(respondToRotationGesture:)];
    [view addGestureRecognizer:rotationRecognizer];

//    UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(respondToPanGesture:)];
//    [view addGestureRecognizer:panRecognizer];
    
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
//	game->BeginAudioInterruption();
}

- (void)endInterruption
{
//	game->EndAudioInterruption();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	float scale = [UIScreen mainScreen].scale;
	for(UITouch *touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
        
        auto event = std::make_shared<NinjaParty::TouchBeganEvent>(touch,
                                                      static_cast<int>(touchPoint.x * scale),
                                                      static_cast<int>(touchPoint.y * scale),
                                                      static_cast<int>(touch.tapCount));
        game->PostEvent(event);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	float scale = [UIScreen mainScreen].scale;
	for(UITouch *touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];

        auto event = std::make_shared<NinjaParty::TouchEndedEvent>(touch,
                                                                   static_cast<int>(touchPoint.x * scale),
                                                                   static_cast<int>(touchPoint.y * scale));
        game->PostEvent(event);
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	float scale = [UIScreen mainScreen].scale;
	for(UITouch *touch in touches)
	{
		CGPoint touchPoint = [touch locationInView:self.view];
        
        auto event = std::make_shared<NinjaParty::TouchMovedEvent>(touch,
                                                                   static_cast<int>(touchPoint.x * scale),
                                                                   static_cast<int>(touchPoint.y * scale));
        game->PostEvent(event);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for(UITouch *touch in touches)
	{
        auto event = std::make_shared<NinjaParty::TouchCancelledEvent>(touch);
        game->PostEvent(event);
	}
}

- (IBAction)respondToTapGesture:(UITapGestureRecognizer*)recognizer
{
	float scale = [UIScreen mainScreen].scale;
    CGPoint touchPoint = [recognizer locationInView:self.view];
    
    int x = (int)(touchPoint.x * scale);
    int y = (int)(touchPoint.y * scale);
    
    auto event = std::make_shared<NinjaParty::GestureTapEvent>(x, y, recognizer.numberOfTouches);
    game->PostEvent(event);
}

- (IBAction)respondToPinchGesture:(UIPinchGestureRecognizer*)recognizer
{
    float scale = [UIScreen mainScreen].scale;
    CGPoint touchPoint = [recognizer locationInView:recognizer.view];

    int x = (int)(touchPoint.x * scale);
    int y = (int)(touchPoint.y * scale);
    
    auto event = std::make_shared<NinjaParty::GesturePinchEvent>(x, y, recognizer.scale);
    game->PostEvent(event);
    
    recognizer.scale = 1;
}

- (IBAction)respondToSwipeGesture:(UISwipeGestureRecognizer*)recognizer
{
    NinjaParty::SwipeDirection direction;

    switch(recognizer.direction)
    {
        case UISwipeGestureRecognizerDirectionLeft:
            direction = NinjaParty::SwipeDirection::Left;
            break;
        case UISwipeGestureRecognizerDirectionRight:
            direction = NinjaParty::SwipeDirection::Right;
            break;
        case UISwipeGestureRecognizerDirectionUp:
            direction = NinjaParty::SwipeDirection::Up;
            break;
        case UISwipeGestureRecognizerDirectionDown:
            direction = NinjaParty::SwipeDirection::Down;
            break;
    }
    
    auto event = std::make_shared<NinjaParty::GestureSwipeEvent>(direction);
    game->PostEvent(event);
}

- (IBAction)respondToPanGesture:(UIPanGestureRecognizer*)recognizer
{
    float scale = [UIScreen mainScreen].scale;
    CGPoint translation = [recognizer translationInView:self.view];

    int x = (int)(translation.x * scale);
    int y = (int)(translation.y * scale);
    
    auto event = std::make_shared<NinjaParty::GesturePanEvent>(x, y);
    game->PostEvent(event);
    
    [recognizer setTranslation:{0, 0} inView:self.view];
}

- (IBAction)respondToRotationGesture:(UIRotationGestureRecognizer*)recognizer
{
    auto event = std::make_shared<NinjaParty::GestureRotationEvent>(recognizer.rotation);
    game->PostEvent(event);
    recognizer.rotation = 0;
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
