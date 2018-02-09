//
//  MakeFavManager.h
//  MAKE05
//
//  Created by jordenwu on 11/2/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MakeFavItem.h"
/**收藏的数据管理*/
@interface MakeFavManager : NSObject
{
    NSMutableArray *favNames_;
}
/**收藏的所有名称组 */
@property(nonatomic,retain)NSMutableArray *favNames;
/**共享*/
+(MakeFavManager *)sharedFavmanager;
/**根据ID删除 */
-(void)deleteFavByID:(int)tag;
/**获取收藏项目组的个数*/
-(int)getFavCount;
/**获取收藏的名称*/
-(NSString *)getFavNameByID:(int)tag;
/**获取icon*/
-(UIImage *)getFavIconByID:(int)tag;
/**获取收藏做好东西的图片 */
-(UIImage *)getFavImageByID:(int)tag;
/*获取收藏的东西数据路径*/
-(NSString *)getFavDataPathByID:(int)tag;
/*保存数据*/
-(void)saveFavData;
/**添加收藏*/
-(void)addFavWithName:(NSString *)name Image:(UIImage *)image;

/**获取图片路径*/
-(NSString*)getFavImagePathByID:(int)tag;
-(NSString *)getFavIconPathByID:(int)tag;
@end
