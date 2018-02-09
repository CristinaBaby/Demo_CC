//
//  MakeFavItem.h
//  MAKE05
//
//  Created by jordenwu on 11/2/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
/**搜藏的项目*/
@interface MakeFavItem : NSObject
{
    NSString *favName_;
}
/**收藏的名称*/
@property(nonatomic,retain)NSString *favName;
/**根据名称初始化 */
-(id)initWithFavName:(NSString*)name;
@end
