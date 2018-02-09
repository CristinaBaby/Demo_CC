//
//  FruitSprite.hpp
//  test-cocos2dx
//
//  Created by wusonglin on 15/12/12.
//
//

#ifndef FruitSprite_hpp
#define FruitSprite_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

//定义一个水果方块
class FruitSprite:public Sprite{

public:
    FruitSprite();
    ~FruitSprite();
public:
    static FruitSprite* create(int color);
    bool init(int color);
    //是否被选中
    bool isSelected();
    inline void setSelected(bool b){selected = b;}
    
    //目标点
    void setDesPosition(const Vec2 p);
    inline Point getDesPosition(){return desPosition;}
    
    void updatePosition();
    int getColor();
    
    bool isCheckPosAndDes();
    //设置行列
    inline int  getIndexRow(){return index_row;}
    inline int  getIndexCol(){return index_col;}
    inline void setIndex_RowCol(int row,int col){index_row = row;index_col = col;}
private:
    __String* getImage(int color);
public:
    //一些参数
    const static int MOVE_SPEED = 16;
    static const int COLOR_MAX_NUM = 5;
    static const int WIDTH  = 76;
    static const int HEIGHT = 76;
private:
    //定义行列
    int index_row;
    int index_col;
    int color;
    //目标点
    Vec2 desPosition;
    bool selected;
    bool isAdjust;
    int  state;
    bool m_bAdjustX;
    bool m_bAdjustY;
};


#endif /* FruitSprite_hpp */
