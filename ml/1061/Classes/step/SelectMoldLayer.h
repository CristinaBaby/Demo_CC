//
//  SelectMoldLayer.hpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/21.
//
//

#ifndef SelectMoldLayer_hpp
#define SelectMoldLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MySpriteButton.h"

USING_NS_CC;
using namespace std;

/*
 *  选择mold的Layer
 */

class SelectMoldLayer : public Layer{

public:
    SelectMoldLayer();
    ~SelectMoldLayer();
    
    virtual bool init();
    CREATE_FUNC(SelectMoldLayer);
private:
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();

    void clickBtn(Ref *sender);
    void showBtn();
    void selectBtnAction(MySpriteButton* btn);
    void addLock();
    void showFont();
public:
    //
    std::function<void()> showPourLayer = nullptr;
    
    //选中之后的回掉
    std::function<void()> isSelectCallback = nullptr;
    //更换背景
    std::function<void()> changeBgCallback = nullptr;
    //开始倒
    std::function<void(int tag)> startPourCallback = nullptr;
    void hideSelectBtn();
    void _hideSelectBtn();
private:
//    Vec2 btnPos[6];
    Vector<MySpriteButton*> m_vBtn;
    bool m_bIsSelect;
    MySpriteButton* m_pSelectBtn;
    Sprite* m_pFont;
    Sprite* m_pBg;
    Vector<Sprite*> m_vLock;
};
#endif /* SelectMoldLayer_hpp */
