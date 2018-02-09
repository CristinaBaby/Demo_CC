//
//  MakeFavItem.m
//  MAKE05
//
//  Created by jordenwu on 11/2/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "MakeFavItem.h"
#define kFavName @"FavName"

@implementation MakeFavItem
@synthesize favName=favName_;
- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}
-(id)initWithFavName:(NSString*)name
{
    self = [super init];
    if (self) {
        // Initialization code here.
        self.favName=name;
    }
    
    return self;
}
#pragma mark NSCoding

- (void) encodeWithCoder:(NSCoder *)encoder {
    
    [encoder encodeObject:favName_ forKey:kFavName];
}

- (id)initWithCoder:(NSCoder *)decoder {
    
    return [self initWithFavName:(NSString*)[decoder decodeObjectForKey:kFavName]];
}
-(void)dealloc
{
    [super dealloc];
}

@end
