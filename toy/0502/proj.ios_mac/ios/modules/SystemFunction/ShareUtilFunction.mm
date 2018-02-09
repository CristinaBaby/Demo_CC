//
//  ShareUtilFunction
//  MysteryGuardians
//
//  Created by liyang on 13-7-9.
//
//

#include <string>
#include "ShareUtilFunction.h"
#include "cocos2d.h"
#include "ShareFunctionIOS.h"
#include "AppController.h"
#include "RootViewController.h"

static ShareUtilFunction* sharefuncion = 0;

ShareUtilFunction* ShareUtilFunction::getInstance()
{
    if(sharefuncion == NULL)
        sharefuncion = new ShareUtilFunction;
    
    return sharefuncion;
};

    ShareUtilFunction::ShareUtilFunction(){
    
    }
ShareUtilFunction::~ShareUtilFunction(){

}

   void ShareUtilFunction::goSystemImageEs(){
       [[AppController sharedAppController].viewController goImageES];
    
    }


   void ShareUtilFunction::setDelegate(GoPhotoCallBack *delegate)
{
    m_delegate = NULL;
    m_delegate = delegate;
}

void GoPhotoCallBack::goPhotoCallBack(int code)
{

}

