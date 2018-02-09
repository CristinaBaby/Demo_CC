//
//  MakeFavManager.m
//  MAKE05
//
//  Created by jordenwu on 11/2/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "MakeFavManager.h"
#import  <QuartzCore/QuartzCore.h>
#import "FileHelper.h"

#define keyForFavFileName @"fav.data"
#define DOCUMENTS_FOLDER [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]

@implementation MakeFavManager
@synthesize favNames=favNames_;

static MakeFavManager *instance=nil;

+(MakeFavManager *)sharedFavmanager
{
    @synchronized(self)
    {
        if (instance==nil) {
            instance=[[MakeFavManager alloc] init];
        }
    }
    return instance;
}

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        NSString *path=[DOCUMENTS_FOLDER stringByAppendingPathComponent:keyForFavFileName];
        NSLog(@"%@",path);
        if ([FileHelper fileIsExistWithPath:path]) {
            NSArray *tempArray=[NSKeyedUnarchiver unarchiveObjectWithFile:path];
            favNames_=[[NSMutableArray arrayWithArray:tempArray] retain];
        }else{
            favNames_=[[NSMutableArray arrayWithCapacity:10] retain];
        }
    }
    return self;
}

-(void)addFavWithName:(NSString *)name Image:(UIImage *)image
{
    MakeFavItem *item=[[MakeFavItem alloc] initWithFavName:name];
    [favNames_ addObject:item];
    [item release];
    
    //保存做好的图片
    NSData *imgData = UIImagePNGRepresentation(image);
    NSString  *path = [NSString stringWithFormat:@"%@/%@.png", DOCUMENTS_FOLDER,name];
    [imgData writeToFile:path atomically:YES];
    
    
    CGSize iconSize= CGSizeMake(image.size.width*0.5, image.size.height*0.5);
    NSLog(@"---%f,%f",iconSize.width,iconSize.height);
    //缩小
    if([[UIDevice currentDevice].systemVersion floatValue]>=4){
        UIGraphicsBeginImageContextWithOptions(iconSize, NO, 1);
    }
    else
        UIGraphicsBeginImageContext(iconSize);
    [image drawInRect:CGRectMake(2, 2, iconSize.width-4, iconSize.height-4)];
    UIImage* smallImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
//
    NSData *imgData2 = UIImagePNGRepresentation(smallImage);
    NSString *name2 = [NSString stringWithFormat:@"%@Icon",name];
    NSString  *path2 = [NSString stringWithFormat:@"%@/%@.png", DOCUMENTS_FOLDER,name2];
    [imgData2 writeToFile:path2 atomically:YES];
}

-(void)saveFavData
{
    NSString *path=[DOCUMENTS_FOLDER stringByAppendingPathComponent:keyForFavFileName];
    NSArray *tempArray=[NSArray arrayWithArray:favNames_];
    [NSKeyedArchiver archiveRootObject:tempArray toFile:path];
}

-(void)deleteFavByID:(int)tag
{  
    [favNames_ removeObject:(MakeFavItem *)[favNames_ objectAtIndex:tag]];
}

-(int)getFavCount
{
    return [favNames_ count];
}

-(NSString *)getFavNameByID:(int)tag
{
    return ((MakeFavItem *)[favNames_ objectAtIndex:tag]).favName;
}

-(UIImage *)getFavImageByID:(int)tag
{
    UIImage *img=[UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%@/%@.png", DOCUMENTS_FOLDER,[self getFavNameByID:tag]]];
    return img;  
}
-(NSString*)getFavIconPathByID:(int)tag
{
    
    return [NSString stringWithFormat:@"%@/%@Icon.png", DOCUMENTS_FOLDER,[self getFavNameByID:tag]];
}
-(NSString*)getFavImagePathByID:(int)tag
{
    return [NSString stringWithFormat:@"%@/%@.png", DOCUMENTS_FOLDER,[self getFavNameByID:tag]];
}

-(UIImage *)getFavIconByID:(int)tag
{  
    UIImage *img=[UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%@/%@Icon.png", DOCUMENTS_FOLDER,[self getFavNameByID:tag]]];
//    CGSize iconSize=CGSizeZero;
//    if ([DeviceHelper isIphoneTarget]) 
//    {
//        iconSize=CGSizeMake(112, 85);
//    }else
//    {
//        iconSize=CGSizeMake(224, 170);
//    }
//    //缩小
//    UIGraphicsBeginImageContext(iconSize); 
//    CGContextRef ctx = UIGraphicsGetCurrentContext();
//    CGContextSetFillColorWithColor(ctx, [UIColor whiteColor].CGColor);
//    CGContextFillRect(ctx, CGRectMake(0, 0, iconSize.width,iconSize.height));
//    [img drawInRect:CGRectMake(2, 2, iconSize.width-4, iconSize.height-4)]; 
//    UIImage* scaledImage = UIGraphicsGetImageFromCurrentImageContext(); 
//    UIGraphicsEndImageContext();
    return img;//scaledImage;
}
//
-(NSString *)getFavDataPathByID:(int)tag
{
    return [NSString stringWithFormat:@"%@/%@.dat", DOCUMENTS_FOLDER,[self getFavNameByID:tag]];
}

-(void)dealloc
{    
    [favNames_ release],favNames_=nil;
    [super dealloc];
}


@end
