//
//  OCHelp.cpp
//  KidsBook
//
//  Created by lidong on 14-10-10.
//
//

#include "OCHelp.h"
#import "AppController.h"
#import <UIKit/UIKit.h>
void OCHelp::loadSupport()
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.toyboxmediainc.com"]];
}

void OCHelp::loadTermsOfUse()
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://www.toyboxmediainc.com/terms.html"]];
}

void OCHelp::loadRateUs(const char* appleID,const char* info)
{
    NSString* lAppleID=[NSString stringWithCString:appleID encoding:NSUTF8StringEncoding];
      [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@", lAppleID]]];
}