//
//  UIUnitScrollView.h
//  Make
//
//  Created by liji on 15/8/6.
//
//

#ifndef __Make__UIUnitScrollView__
#define __Make__UIUnitScrollView__

#include <stdio.h>
#include <iostream>
#include "cocos2d.h"
#include "./extensions/cocos-ext.h"
#include "ui/cocosGUI.h"
#include "RETableView.h"
USING_NS_CC;
USING_NS_CC_EXT;



class UIUnitScrollView : public Layer
{
public:
    
    static UIUnitScrollView* createWithTypeName(std::string pTypeName);
    bool initWithTypeName(std::string pTypeName);
    
    //按照指定 列 来生成
    
    static UIUnitScrollView* createWithTypeNameAndColumn(std::string pTypeName,int Column);
    bool initWithTypeNameAndColumn(std::string pTypeName,int Column);
    
    
    //按照给定的数组来生成
    
    static UIUnitScrollView* createWithNodeVector(Vector<Node*> vec_node, int Column);
    bool initWithNodeVector(Vector<Node*> vec_node,int Column);


    CREATE_FUNC(UIUnitScrollView);
    bool init();
    
    void loadUnitList(std::string pTypeName);

    void setMarginX(int x){_margin = x;};
    void setBackgroundPath(std::string path){_frameBg = path;};
    void setUnitBg(std::string unitBg){_unitBg = unitBg;};
    void loadUnitListColumn(std::string pTypeName,int column );

    void loadUnitListFromTo(std::string pTypeName,int idxFrom,int idxTo );
    
    void enableTouch(bool enable);

    ui::ScrollView* getUnitScrollView(){return unitScrollView_;};
    
    void checkUnlock();

    std::function<void(std::string pIconImageName, int idx)> freeUnitClicked = nullptr;
    std::function<void()> lockedUnitClicked = nullptr;
    
private:
    void touchEvent(Ref*, ui::Widget::TouchEventType);
    int getRowNumber();
    int getRowNumber(int total ,int column);
    void loadScrollList(Vector<Node*>, int column, int row);

private:
    ui::ScrollView* unitScrollView_ = nullptr;
    
    int _columnNumber = 0;
    int _margin = 20;
    
    bool _isClicked = false; //防止连续点击
    
    Vector<ui::Button*> vec_Button;
    
    std::string _frameBg;
    std::string _unitBg;

    

};
#endif /* defined(__Make__UIUnitScrollView__) */
