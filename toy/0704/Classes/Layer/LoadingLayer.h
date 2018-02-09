//
//  LoadingLayer.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/14.
//
//

#ifndef __ColorBook0818__SettingLayer__
#define __ColorBook0818__SettingLayer__

#include "UITools.h"

USING_NS_CC;

class LoadingLayer : public Layer{
public:
    CREATE_FUNC(LoadingLayer);
    virtual bool init();
    
    void showTapToStart();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void setListView(ListView * listview);
    
private:
    bool            _tapToStart = false;
    ImageView *     _tapImage;
    ListView *       _listView;
};

#endif /* defined(__ColorBook0818__SettingLayer__) */
