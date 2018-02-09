//
//  STFacebook.mm
//  SweetDiscovery
//
//  Created by zhangguangzong1 on 9/3/13.
//
//

#include "STFacebook.h"
#include "FaceBookManager.h"


STFacebook::STFacebook()
{
}

STFacebook::~STFacebook()
{
}

void STFacebook::postContent(const char * caption, const char* shareContent)
{
    [[FacebookManager sharedManager] postContent:[NSString stringWithCString:caption encoding:NSUTF8StringEncoding]
                                         Content:[NSString stringWithCString:shareContent encoding:NSUTF8StringEncoding]];
}

void STFacebook::postImage(const char* imgPath)
{
    [[FacebookManager sharedManager] postImage:[UIImage imageWithContentsOfFile:[NSString stringWithCString:imgPath encoding:NSUTF8StringEncoding]]];
}
