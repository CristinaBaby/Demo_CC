//
//  ESScrollView.h
//  Make
//
//  Created by liji on 15/9/18.
//
//

#ifndef __Make__ESScrollView__
#define __Make__ESScrollView__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "ESUnitAdapter.h"
USING_NS_CC;

class  ESScrollView : public ui::ScrollView
{
public:
    CREATE_FUNC(ESScrollView);
    
    void setMarginX(int x){_marginX = x;};
    void setMarginY(int y){_marginY = y;};
    void setMargin(float margin){_margin = margin;};
    void setMarginLeft(int left){_marginLeft = left;};
    void setMarginRight(int right){_marginRight = right;};
    void setDisplayCounts(float c){_disPlayCount = c;};
    void setItemAdapter(ESUnitAdapter* base);
    ESUnitAdapter* getItemAdapter(){return _adapter;};
    virtual void loadScrollView(int column = 1);//每一列行的个数
    void initGrayLayer();
    ESScrollView();
    ~ESScrollView();
    
    Vector<ui::Widget*> vec_Item;
    
    Point scrollToIdx(int idx, float duration = 0.5f);
    
    void reload();
    
    //pageflow
    void setEnablePageflow();
    
    void removeItem(ui::Widget* image);
    
    void show(std::function<void()> callback);
protected:
    int _marginX = 0;
    int _marginLeft = 0;
    int _marginRight = 0;
    int _marginY = 0;
    int _margin = 0;
    float _disPlayCount = 3.f;
    ESUnitAdapter* _adapter;
    
    
protected:
    virtual int getRowNumber(int column);
    void scrolled(Ref*, EventType);
};

#endif /* defined(__Make__ESScrollView__) */
