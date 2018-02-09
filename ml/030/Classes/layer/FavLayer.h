//
//  FavLayer.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#ifndef __OreoMaker__FavLayer__
#define __OreoMaker__FavLayer__

#include <iostream>
#include "BaseLayer.h"
#include "KDGridView.h"
#include "KFavoriteAdapter.h"

using namespace std;

class FavLayer:public BaseLayer,public KDGridViewScrollCallback{
    
private:
    FavLayer();
    ~FavLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(FavLayer);
    
public:
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
public:
    void initUI();
    void initData();
    void touchEvent(Ref *obj , Widget::TouchEventType type);
    virtual void onScrolling(float offsetX,float offsetY);
    virtual void onScrollEnd(float offsetX,float offsetY);
    void onDeleteCallback(int tag,cocos2d::ui::Button* button);
    
    virtual void onNegativeClick(void* type);
    virtual void onPositiveClick(void* type);

private:
    vector<string> m_vFavoriteScales;
//    vector<string> m_vFavorites;
    Vector<Sprite*> m_vTypes;

    KDGridView* m_pGirdView;
    KFavoriteAdapter* m_pAdapter;
    bool isFirstIn;
    int m_pDeleteIndex;

};
#endif /* defined(__OreoMaker__FavLayer__) */
