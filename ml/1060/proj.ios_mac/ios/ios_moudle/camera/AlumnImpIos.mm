//
//  AlumnImpIos.cpp
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "AlumnImpIos.h"
#import "RootViewController.h"
#import "AppController.h"
#include "AlumnAdapter.h"
#include "cocos2d.h"
USING_NS_CC;

static AlumnImpIos* _alumnImpIos = nil;

@implementation AlumnImpIos
+ (AlumnImpIos*)shared
{
    if(!_alumnImpIos)
       _alumnImpIos = [[AlumnImpIos alloc] init];
    return _alumnImpIos;
}

+ (void)purge
{
    if(_alumnImpIos)
        [_alumnImpIos release];
}

- (void)saveToAlumn:(UIImage *)apImg
{
    if(false == isTakingPhoto) {        
        isTakingPhoto = true;
        UIImageWriteToSavedPhotosAlbum(apImg, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
    }
}

//保存到本地回调
- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
{
	if (error != NULL)
	{
        if ([@"Data unavailable" isEqualToString:[error localizedDescription]]) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"This app does not have access to your photos.\nYou can enable access in Privacy Setting." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil, nil];
            [alert show];
            [alert release];
        }
        
	}else {
		UIAlertView *alert = [[[UIAlertView  alloc] initWithTitle:nil
							   message:@"Your image has been saved to Photos!"
                                                         delegate:nil
                                                cancelButtonTitle:nil
                                                otherButtonTitles:@"OK",nil] autorelease];
		[alert show];
	}
    
    isTakingPhoto = false;
}


-(void)navigationController:(UINavigationController *)navigationController
     willShowViewController:(UIViewController *)viewController
                   animated:(BOOL)animated
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
}

-(void) openAlumn {
    
    RootViewController *_viewController = [AppController sharedAppController].viewController;
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary])
    {
    
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    picker.wantsFullScreenLayout = YES;
    picker.delegate = self;
     //   picker.navigationBarHidden = YES;
    //设置选择后的图片可被编辑
   //  picker.allowsEditing = YES;
    
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        photoPop=[[UIPopoverController alloc] initWithContentViewController:picker];
        photoPop.delegate = self;
        [photoPop presentPopoverFromRect:CGRectMake(_viewController.view.frame.size.width/2,_viewController.view.frame.size.height/4,0, 0) inView:_viewController.view permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
        
    } else {
        [_viewController presentModalViewController:picker animated:YES];
    }
    [picker release];
    } else {
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:@"error"
                              message:@""
                              delegate:nil
                              cancelButtonTitle:@"OK!"
                              otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
}

#pragma mark UIImagePickerControllerDelegate
- (void)imagePickerController:(UIImagePickerController *)picker
        didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo
{
     if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
         [photoPop dismissPopoverAnimated:YES];
         [photoPop release];
         photoPop = nil;
     } else {
           [picker dismissModalViewControllerAnimated:YES];
     }
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    if(image != nil) {
        
        Texture2D* tex = new Texture2D();
        tex->autorelease();
        
        CGImageRef imageRef = [image CGImage];
        NSUInteger width = CGImageGetWidth(imageRef);
        NSUInteger height = CGImageGetHeight(imageRef);
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        unsigned char* rawData = (unsigned char*) calloc(height * width * 4, sizeof(unsigned char));
        NSUInteger bytesPerPixel = 4;
        NSUInteger bytesPerRow = bytesPerPixel * width;
        NSUInteger bitsPerComponent = 8;
        CGContextRef context = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
        CGContextRelease(context);
        //    tex->initWithData(rawData, kCCTexture2DPixelFormat_RGBA8888, width, height, CCSizeMake(width, height));
        tex->initWithData(rawData, sizeof(rawData), Texture2D::PixelFormat::RGBA8888, width, height, cocos2d::Size(width, height));
        free(rawData);
        auto callback = AlumnAdapter::getInstance()->chooseCallback;
        if(nullptr != callback)
            callback(tex);
    }
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{

    [picker dismissModalViewControllerAnimated:YES];
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    [photoPop release];
    photoPop = nil;
}

@end

#endif