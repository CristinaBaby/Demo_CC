//
//  ShareUtilFunction
//  MysteryGuardians
//
//  Created by liyang on 13-7-9.
//
//

#ifndef __MysteryGuardians__ShareUtilFunction__
#define __MysteryGuardians__ShareUtilFunction__
#include <string>
#include "cocos2d.h"

class GoPhotoCallBack{
public:
    virtual void goPhotoCallBack(int code) = 0;
    
};



class ShareUtilFunction
{
//    static ShareUtilFunction *sharefuncion;
public:
	ShareUtilFunction();
	~ShareUtilFunction();
    
    static ShareUtilFunction* getInstance();
    
    GoPhotoCallBack* m_delegate;
public:
    /**
     *  @brief go to system image page
     *
     *  @param subject email subject.
     *  @param content email subject.
     */
    virtual void goSystemImageEs();

    void setDelegate(GoPhotoCallBack* delegate);
};



#endif /* defined(__MysteryGuardians__ShareUtilFunction__) */
