//
//  KDKeyboardDispatcher.cpp
//  DKG-BeautySalonFashion
//
//  Created by hujie2 on 14-12-3.
//
//

#include "KDKeyboardDispatcher.h"


KDKeyboardDispatcher* KDKeyboardDispatcher::getInstance()
{
    static KDKeyboardDispatcher __instance;
    return &__instance;
}

KDKeyboardDispatcher::KDKeyboardDispatcher()
{
    m_pKeyBoardListener = EventListenerKeyboard::create();
    m_pKeyBoardListener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event*){
        
        switch (code) {
            case EventKeyboard::KeyCode::KEY_BACK:
                onKeyBackClicked();
                break;
                
            default:
                break;
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_pKeyBoardListener, 1);
}

KDKeyboardDispatcher::~KDKeyboardDispatcher()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_pKeyBoardListener);
}

void KDKeyboardDispatcher::addDelegate(KDKeyboardDispatcherDelegate *apDelegate)
{
    m_pDelegates.push_back(apDelegate);
}

void KDKeyboardDispatcher::removeDelegate(KDKeyboardDispatcherDelegate *apDelegate)
{
    for (auto itr = m_pDelegates.begin(); itr!= m_pDelegates.end();  itr++) {
        if ((*itr) == apDelegate) {
            m_pDelegates.erase(itr);
            break;
        }
    }
}

void KDKeyboardDispatcher::onKeyBackClicked()
{
    
    
    for (int i = m_pDelegates.size() - 1; i >= 0; --i) {
        KDKeyboardDispatcherDelegate* lpDelegate = m_pDelegates.at(i);
        if(lpDelegate)
        {
            if(lpDelegate->onKeyReleased())
                break;
        }
    }
}
