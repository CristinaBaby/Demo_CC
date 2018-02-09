//
//  KDKeyboardDispatcher.h
//  DKG-BeautySalonFashion
//
//  Created by hujie2 on 14-12-3.
//
//

#ifndef __DKG_BeautySalonFashion__KDKeyboardDispatcher__
#define __DKG_BeautySalonFashion__KDKeyboardDispatcher__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class KDKeyboardDispatcherDelegate
{
public:
    virtual bool onKeyReleased(){return false;};
};

class KDKeyboardDispatcher : public Ref
{
public:
    static KDKeyboardDispatcher* getInstance();
    
    KDKeyboardDispatcher();
    ~KDKeyboardDispatcher();
    
    void addDelegate(KDKeyboardDispatcherDelegate* apDelegate);
    void removeDelegate(KDKeyboardDispatcherDelegate* apDelegate);
    
    virtual void onKeyBackClicked();
    

protected:
    std::vector<KDKeyboardDispatcherDelegate*> m_pDelegates;
    EventListenerKeyboard* m_pKeyBoardListener;
};

#endif /* defined(__DKG_BeautySalonFashion__KDKeyboardDispatcher__) */
