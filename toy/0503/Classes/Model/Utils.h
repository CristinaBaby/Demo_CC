//
//  Utils.h
//  BedtimeStory
//
//  Created by maxiang on 9/15/15.
//
//

#ifndef __BedtimeStory__Utils__
#define __BedtimeStory__Utils__

#include <cocos2d.h>
#include "cocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace std;

class Utils
{
public:

    static Button* addEventToButton(Widget *ui,
                                    const string& buttonname,
                                    Widget::TouchEventType type,
                                    const std::function<void()>& callback);
    
    static bool hitTest(Node *node, const Vec2& point);
    
    static Sprite* createSpriteWithNoCache(const std::string& file);
    static LayerColor* createMaskLayer(const float alpha);

};

#endif /* defined(__BedtimeStory__Utils__) */
