//
//  SettingView.h
//  Make
//
//  Created by QAMAC01 on 15-4-14.
//
//

#ifndef __Make__SettingView__
#define __Make__SettingView__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"

USING_NS_CC;

class SettingView : public Layer
{
public:
    CREATE_FUNC(SettingView);
    bool init();

private:
    void initBtn();
    void touchEvent(Ref *obj , cocos2d::ui::Widget::TouchEventType type);


};

#endif /* defined(__Make__SettingView__) */
