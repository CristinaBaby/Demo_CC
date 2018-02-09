//
//  ChangeLoading.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/20.
//
//

#ifndef ChangeLoading_hpp
#define ChangeLoading_hpp

#include "cocos2d.h"
//#include "CocosHelper.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
class ChangeLoading : public Layer {
public:
    CREATE_FUNC(ChangeLoading);
    ChangeLoading();
    static void loading(std::function<void()>);
protected:
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();
    virtual void onExit();
    bool init();
    bool isEnd;
    void show();
  
private:
    Sprite* m_pBg;
    Sprite* m_pCurtain;
    Sprite* m_pBeads;
    cocostudio::Armature * m_pArmature;
public:
    std::function<void()> endFunc;
};

#endif /* ChangeLoading_hpp */
