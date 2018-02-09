//
//  DecorationConeLayer.hpp
//  IceCream
//
//  Created by huxixiang on 16/5/5.
//
//

#ifndef DecorationConeLayer_hpp
#define DecorationConeLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ToolSprite.h"
#include "KDGridView.h"
#include "KSSimpleAdapter.h"
#include "UnitScrollView.h"

USING_NS_CC;
class DecorationConeLayer : public BaseLayer, public ToolSpriteDelegate{
public:
    DecorationConeLayer();
    ~DecorationConeLayer();
    
    static Scene* scene();
    CREATE_FUNC(DecorationConeLayer);
    virtual bool init() override;
    
protected:
    bool initData();
    bool initLayer();
    void initBackground();
    void initScrollView();
    
    void onTypeItemClick(int tag,MenuItem* menuItem,int index);
    void onGridItemCallback(int tag,MenuItem* menuItem,int index);
    
    virtual void onNextCallback() override;
    void enterNextScene(float ft);
    
public:
    virtual void onNegativeClick(void* type) override;
    virtual void onPositiveClick(void* type) override;
    
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch) override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    Layer* m_pUILayer;
    Layer* m_pContentLayer;
    Layer* m_pBottomLayer;
    
    Node* m_pBg;

    // 糖的移动区域
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
    
    Sprite* m_pSyrup;
    Sprite* m_pSticker;
    
    Vec2 m_oTouchPos;
    Vec2 m_oListViewOffset;
};
#endif /* DecorationConeLayer_hpp */
