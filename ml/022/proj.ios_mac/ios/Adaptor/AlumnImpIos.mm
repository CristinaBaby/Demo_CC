//
//  AlumnImpIos.cpp
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#import "AlumnImpIos.h"
#include "AlumnAdapter.h"
#include "Defines.h"
#include "Global.h"
#include "Dialog.h"

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
    UIImageWriteToSavedPhotosAlbum(apImg, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
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
        AlumnAdapter::finished(false);
        
	}else {
//		UIAlertView *alert = [[[UIAlertView  alloc] initWithTitle:nil
//							   message:@"Your image has been saved to Photos!"
//                                                         delegate:nil
//                                                cancelButtonTitle:nil
//                                                otherButtonTitles:@"OK",nil] autorelease];
//		[alert show];
        
        Dialog* dialog=Dialog::create(cocos2d::Size(425,290), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
        dialog->setContentText("Your image has been saved to your Camera Roll!");
        dialog->setPositiveBtnText("OK");
        CCDirector::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        
        dialog->setPosition(CCDirector::getInstance()->getVisibleOrigin());
        AlumnAdapter::finished(true);
	}
}


@end