#import <AVFoundation/AVFoundation.h>
#import <GLKit/GLKit.h>

@interface ViewController : GLKViewController <AVAudioSessionDelegate>

+ (ViewController*) instance;

- (void) FacebookLogin;
- (void) FacebookLogout;
- (bool) FacebookIsLoggedIn;

@end
