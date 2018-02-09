
#import "GameDataAdapter.h"
#include "AlumnAdapter.h"
#include "Defines.h"
#include "Global.h"
#include "Dialog.h"

#import <Foundation/Foundation.h>
@interface GameDataAdapterIOS : NSObject

+ (BOOL) getBoolValue:(NSString*) str;
+ (int) getIntValue:(NSString*) str;
+ (NSString*) getStringValue:(NSString*) str;

@end

@implementation GameDataAdapterIOS

+ (BOOL) getBoolValue:(NSString*) str
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:str];
}

+ (int) getIntValue:(NSString*) str
{
    return [[NSUserDefaults standardUserDefaults] integerForKey:str];
}
+ (NSString*) getStringValue:(NSString*) str
{
    return [[NSUserDefaults standardUserDefaults] stringForKey:str];
}
@end


bool GameDataAdapter::getBoolValue(const std::string str)
{
    return [GameDataAdapterIOS getBoolValue:[NSString stringWithUTF8String:str.c_str()]];
}

int GameDataAdapter::getIntValue(const std::string str)
{
    return [GameDataAdapterIOS getIntValue:[NSString stringWithUTF8String:str.c_str()]];
}

std::string GameDataAdapter::getStringValue(const std::string str)
{
    return std::string([[GameDataAdapterIOS getStringValue:[NSString stringWithUTF8String:str.c_str()]] cStringUsingEncoding:NSUTF8StringEncoding]);
}
