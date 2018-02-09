//
//  TipsUIController.h
//  BHUG1062
//
//  Created by tanshoumei on 5/16/16.
//
//

#ifndef TipsUIController_hpp
#define TipsUIController_hpp

#include <stdio.h>
#include "SingleTon.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class TipsUIController: public SingleTon<TipsUIController>
{
public:
    Node* showClickTip(Node* parent, const Vec2& pointLocal);
    void removeClickTip();
    
    Node* showAccelerateTip(Node* parent, const Vec2& pointLocal, bool isRight = false);
    void removeAccelerateTip();
    
    //垂直视角
    Node* showMixTip90Degree(Node* parent, const Vec2& pointLocal);
    //45度视角的旋转
    Node* showMixTip45Degree(Node* parent, const Vec2& pointLocal);
    void removeRotateTip();
    
    Node* showMoveTip(Node* parent, const Vec2& startPos , const Vec2& endPos);
    Node* showMoveTipBezel(Node* parent, const Vec2& startPos , const Vec2& endPos);
    Node* showPressTip(Node* parent, const Vec2& startPos , const Vec2& endPos);
    void removeMoveTip();
    
    Node* showDownTip(Node* parent, const Vec2& pointLocal);

    void removeAllTips();
    
private:
    void removeTipNode(const int tag);
    
private:
    string _rootPathRes = "ui/other/";
    
    Map<int, Node*> _mapTipsNode;
};

#endif /* TipsUIController_hpp */
