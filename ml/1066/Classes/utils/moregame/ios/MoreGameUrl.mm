//
//  MoreGameUrl.cpp
//  JapaneseFood
//
//  Created by youjinchao1 on 15-3-10.
//
//

#include "MoreGameUrl.h"
#import "UIDevice-Hardware.h"
#include <sstream>
#import <AdSupport/AdSupport.h>
using namespace std;
string MoreGameUrl::getMoreGameUrl(){
   // return "http://services.crazycatsmedia.com/more/bakerychef?&format=json";
    
    NSString *appId = [[[[NSBundle mainBundle] bundleIdentifier] componentsSeparatedByString:@"."] lastObject];
    NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
    NSString *suffix = [[bundleID componentsSeparatedByString:@"."] objectAtIndex:0];
    NSString *kurl_ = [NSString stringWithFormat:@"https://services.%@.%@/more/",[[bundleID componentsSeparatedByString:@"."] objectAtIndex:1],suffix];
    NSMutableString *url = [NSMutableString stringWithString:kurl_];
    [url appendFormat:@"%@/",appId];
    
    // get current language
    NSUserDefaults* defs = [NSUserDefaults standardUserDefaults];
    NSArray* languages = [defs objectForKey:@"AppleLanguages"];
    NSString* preferredLang = [languages objectAtIndex:0];
    [url appendFormat:@"%@/",preferredLang];
    
    UIDevice *_device = [UIDevice currentDevice];
    // append udid - changed to OpenUDID at 2.1.0rc
    NSString* idfa = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    if(0 == [idfa length])
        idfa = @"";
    [url appendFormat:@"%@/",idfa];
    // append model
    [url appendFormat:@"?model=%@",[[_device platformString] stringByReplacingOccurrencesOfString:@" " withString:@"_"]];
    // append systemVersion
    [url appendFormat:@"&sysVer=%@",[_device systemVersion]];
    
    // append bundleVersion
    NSString *bundleVer = [[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleVersion"];
    [url appendFormat:@"&bundleVer=%@",bundleVer];
    
    // append full bundleID
    [url appendFormat:@"&bundleID=%@",bundleID];
    
    [url appendString:@"&format=json"];
    
    
    stringstream tempS("");
    tempS<<[url UTF8String];
    
    
    return tempS.str();
}
