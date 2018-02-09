//
//  FaceBook.m
//  MAKE05
//
//  Created by jordenwu on 11/10/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "FaceBookManager.h"
#import "AppDelegate.h"

#define kFacebookAppID			@"595301420503441"
#define kFBAccessTokenKey		@"FBAccessTokenKey"
#define kFBExpirationDateKey	@"213506278831213"

@implementation FacebookManager


static FacebookManager *instance=nil;
+(FacebookManager *)sharedManager
{
    @synchronized(self)
    {
        if (instance==nil) {
            instance=[[FacebookManager alloc] init];
        }
    }
    return instance;
}
-(BOOL)handleOpenURL:(NSURL*)url
{
    return [facebook_ handleOpenURL:url];
}
- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        facebook_ = [[Facebook alloc] initWithAppId:kFacebookAppID andDelegate:self];
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        if ([defaults objectForKey:kFBAccessTokenKey]  && [defaults objectForKey:kFBExpirationDateKey]) {
            facebook_.accessToken = [defaults objectForKey:kFBAccessTokenKey];
            facebook_.expirationDate = [defaults objectForKey:kFBExpirationDateKey];
        }
        NSLog(@"\naccess token: %@\nexpire in: %@", facebook_.accessToken, facebook_.expirationDate);
        
        if (![facebook_ isSessionValid]) {
            isNeedLogIn_=YES;
        }
        
        NSArray *cookies = [[NSHTTPCookieStorage sharedHTTPCookieStorage] cookies];
        for (NSHTTPCookie* cookie in cookies) {
            if ([cookie.domain hasSuffix:@"facebook.com"]) {
                NSLog(@"cookie domain: %@, common url: %@", cookie.domain, cookie.commentURL);
            }
        }
        //
        tempArray_=[[NSMutableArray arrayWithCapacity:1] retain];
    }
    return self;
}

-(void)logIn
{
    NSArray *permissions = [NSArray arrayWithObjects:@"offline_access", @"read_stream", @"publish_stream",
							@"user_likes", @"email", nil];
    [facebook_ authorize:permissions];
}

-(void)logOut
{	
    if ([facebook_ isSessionValid]) {
		[facebook_ logout:self];
	}
}

-(void)postContent:(NSString *)caption Content:(NSString*) shareContent
{
    if (isNeedLogIn_) {
        [self logIn];
        //添加一句话
        [tempArray_ removeAllObjects];
        [tempArray_ addObject:shareContent];
    }else {
        NSArray *actionLinks = [NSArray arrayWithObjects:[NSDictionary dictionaryWithObjectsAndKeys:@"Find Chupa Chups",@"name",@"https://itunes.apple.com/app/id744746963",@"link", nil],nil];
        
        SBJSON *jsonWriter = [[SBJSON new]autorelease];
        NSString *actionLinkStr = [jsonWriter stringWithObject:actionLinks];
        
        NSMutableString *_shareCaption = [NSMutableString stringWithString:caption];
        NSMutableString *_shareContent = [NSMutableString stringWithString:shareContent];
        [_shareContent appendString:@"\nhttps://itunes.apple.com/app/id744746963"];
        NSMutableDictionary * params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                        @"Chupa Chups - Spot the Difference" ,@"name",
                                        _shareCaption, @"caption",
                                        _shareContent, @"description",
                                        @"https://itunes.apple.com/app/id744746963",@"link",
                                        actionLinkStr,@"actions",
                                        @"http://img0.bdstatic.com/img/image/shouye/mnqc-11681551470.jpg",@"picture",
                                        nil];
        [facebook_ dialog:@"feed" andParams:params andDelegate:self];
    }
}

-(void)_doPostImage:(UIImage *)image
{
// NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
// @"http://www.tech.com/", @"link",
// image, @"picture",
// @"Test post something to Facebook", @"name",
// @"Simple Test", @"caption",
// @"Posting  Media....", @"description",
// @"Test post something to Facebook",  @"message",
// nil];
// [facebook_ dialog:@"feed" andParams:params andDelegate:self];
 
NSMutableDictionary * params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                image, @"picture",
                                @"Download the coolest game. Let's play together.", @"caption",
                                @"Sweet Discovery" ,@"name",
                                @"Sweet Discovery", @"description",
                                nil];

[facebook_ requestWithMethodName: @"photos.upload"
                       andParams: params
                   andHttpMethod: @"POST"
                     andDelegate: self];
}
-(void)postImage:(UIImage *)image
{
    if (isNeedLogIn_) {
        [self logIn];
        //只添加一张图片
        [tempArray_ removeAllObjects];
        [tempArray_ addObject:image];
    }else {
		[self _doPostImage:image];
    }
}

#pragma mark -- Facebook Session elegate --

- (void)fbDidLogin
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setObject:facebook_.accessToken forKey:kFBAccessTokenKey];
	[defaults setObject:facebook_.expirationDate forKey:kFBExpirationDateKey];
	[defaults synchronize];
	
	isNeedLogIn_=NO;
    if ([tempArray_ count]>0) {
        NSLog(@"%d", [tempArray_ count]);
        for (NSString *content in tempArray_) {
			[self postContent:@"Spot Chupa Chups!" Content:content];
        }
        [tempArray_ removeAllObjects];
    }
}

- (void)fbDidNotLogin:(BOOL)cancelled
{
    //	NSLog(@"Did Not Login");
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Facebook login failed!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

- (void)fbDidLogout
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	if ([defaults objectForKey:kFBAccessTokenKey]) {
		[defaults removeObjectForKey:kFBAccessTokenKey];
		[defaults removeObjectForKey:kFBExpirationDateKey];
		[defaults synchronize];
	}
	isNeedLogIn_=YES;
    
}

#pragma mark -- Facebook Request delegate --

/**
 * Called just before the request is sent to the server.
 */
- (void)requestLoading:(FBRequest *)request
{
    if (!alert_) {
        alert_ = [[UIAlertView alloc] initWithTitle:@"Posting Image\nPlease Wait..." message:nil delegate:self cancelButtonTitle:nil otherButtonTitles: nil];
    }
    if (!_indicator) {
        _indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
//        if (DeviceTool::isIphoneTarget()) {
            _indicator.center = CGPointMake(147, 85);
//        }else {
//            _indicator.center = CGPointMake(147, 85);
//        }
        [_indicator startAnimating];
        [alert_ addSubview:_indicator];
    }
    //    indicator.center = CGPointMake(alert_.bounds.size.width / 2, alert_.bounds.size.height - 50);
    [alert_ show];
//    NSLog(@"%@", NSStringFromCGRect([AppDelegate sharedDelegate].window.frame));
}

/**
 * Called when the server responds and begins to send back data.
 */
- (void)request:(FBRequest *)request didReceiveResponse:(NSURLResponse *)response
{
	//NSLog(@"%@",_cmd);
    //     NSLog(@"fffffffffff");
}

/**
 * Called when a request returns a response.
 *
 * The result object is the raw response from the server of type NSData
 */
- (void)request:(FBRequest *)request didLoadRawResponse:(NSData *)data
{
	//NSLog(@"%@",_cmd);
    //    NSLog(@"fffffffffff");
    
    
}

/**
 * Called when a request returns and its response has been parsed into
 * an object.
 *
 * The resulting object may be a dictionary, an array, a string, or a number,
 * depending on thee format of the API response.
 */
- (void)request:(FBRequest *)request didLoad:(id)result
{
    //	NSLog(@"request: %@\nresponse: %@", [request url], result);
	/*
     if ([[request url] hasSuffix:@"me"]) {
     [self simpleAlertWithTitle:@"My Facebook Profile" andMessage:[(NSDictionary *)result description]];
     [_myInfoButton setTitle:@"My Info" forState:UIControlStateNormal];
     _myInfoButton.enabled = YES;
     }
     */
    
    [alert_ dismissWithClickedButtonIndex:0 animated:YES];
    
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your image has been uploaded to your album." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

/**
 * Called when an error prevents the request from completing successfully.
 */
- (void)request:(FBRequest *)request didFailWithError:(NSError *)error
{
    [alert_ dismissWithClickedButtonIndex:0 animated:YES];
    
    [self logOut];
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your image has not been uploaded to your album." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

#pragma mark -- Facebook Dialog delegate --

/**
 * Called when the dialog succeeds and is about to be dismissed.
 */
- (void)dialogDidComplete:(FBDialog *)dialog
{
//    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your highscore has been uploaded to your Facebook." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//    [alert show];
//    [alert release];
}

/**
 * Called when the dialog succeeds with a returning url.
 */
- (void)dialogCompleteWithUrl:(NSURL *)url
{
    if([url.absoluteString rangeOfString:@"post_id"].length > 0)
    {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your highscore has been uploaded to your Facebook." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
        [alert release];
    }else if([url.absoluteString isEqualToString:@"fbconnect://success"])
    {
            UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your highscore has not been uploaded to your facebook." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
            [alert show];
            [alert release];
    }

}

/**
 * Called when the dialog get canceled by the user.
 */
- (void)dialogDidNotCompleteWithUrl:(NSURL *)url
{
//    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your highscore has not been uploaded to your facebook." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
//    [alert show];
//    [alert release];
}

/**
 * Called when the dialog is cancelled and is about to be dismissed.
 */
- (void)dialogDidNotComplete:(FBDialog *)dialog
{
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your highscore has not been uploaded to your facebook." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

/**
 * Called when dialog failed to load due to an error.
 */
- (void)dialog:(FBDialog*)dialog didFailWithError:(NSError *)error
{
	[self logOut];
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil message:@"Your highscore has not been uploaded to your facebook." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
    [alert release];
}

/**
 * Asks if a link touched by a user should be opened in an external browser.
 *
 * If a user touches a link, the default behavior is to open the link in the Safari browser,
 * which will cause your app to quit.  You may want to prevent this from happening, open the link
 * in your own internal browser, or perhaps warn the user that they are about to leave your app.
 * If so, implement this method on your delegate and return NO.  If you warn the user, you
 * should hold onto the URL and once you have received their acknowledgement open the URL yourself
 * using [[UIApplication sharedApplication] openURL:].
 */
- (BOOL)dialog:(FBDialog*)dialog shouldOpenURLInExternalBrowser:(NSURL *)url
{
	return NO;
}

@end
