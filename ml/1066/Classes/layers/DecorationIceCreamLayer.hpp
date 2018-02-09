//
//  DecorationIceCreamLayer.hpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#ifndef DecorationIceCreamLayer_hpp
#define DecorationIceCreamLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "UnitScrollView.h"

USING_NS_CC;
class DecorationIceCreamLayer : public BaseLayer, public ToolSpriteDelegate{
public:
    DecorationIceCreamLayer();
    ~DecorationIceCreamLayer();
    
    static Scene* scene();
    CREATE_FUNC(DecorationIceCreamLayer);
    virtual bool init() override;
    
protected:
    bool initData();
    bool initLayer();
    void initBackground();
    void initScrollView();
    
    void onTypeItemClick(int tag,MenuItem* menuItem,int index);
    void onGridItemCallback(int tag,MenuItem* menuItem,int index);
 
public:
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch) override;
    virtual void onNegativeClick(void* type) override;
    virtual void onPositiveClick(void* type) override;
    
    virtual void onNextCallback() override;
    void enterNextScene(float ft);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    Layer* m_pBottomLayer;
    Layer* m_pUILayer;
    Layer* m_pDecBottomLayer;
    Layer* m_pIceCreamLayer;
    Layer* m_pDecTopLayer;
    
    Node* m_pBg;
    Sprite* m_pWafer;
    Sprite* m_pSticker;
    // 移动限制区域
    Node* m_pMoveRect;
    
    Sprite* m_pListviewBg;
    KDGridView* m_pListView;
    KSSimpleAdapter* m_pAdapter;
    
    Vector<Sprite*> m_vTypes;
    
    Sprite* m_pUnitViewBg;
    UnitScrollView* m_pUnitScrollView;
    
    MenuItemSprite* m_pLastItem;
    
    int m_iLastIndex;
    int m_iCurrtIndex;
    
    Vec2 m_oTouchPos;
    Vec2 m_oListViewOffset;
    
};
#endif /* DecorationIceCreamLayer_hpp */
