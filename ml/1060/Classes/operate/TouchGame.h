//
//  TouchGame.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/7.
//
//

#ifndef TouchGame_hpp
#define TouchGame_hpp

#include "BaseStepLayer.h"
#include "MoveForeverComponent.h"
#include "ImageLabel.h"
class TouchGame : public BaseStepLayer{
    DECLARE_DYNCLASS(TouchGame);
public:
    CREATE_FUNC(TouchGame);
    TouchGame();
protected:
    bool init();
       virtual void onExitTransitionDidStart();
    virtual void onEnterTransitionDidFinish();
    void createSchdue();
    void createDonut(int _index);
       virtual void touchEnd(ui::Widget* widget);
    void moveCallBack(Node*,Component*,OperateListner*);
private:
    Vector<MoveForeverComponent*> _moves;
    Vector<OperateListner*> _moveForeverLis;
    vector<Vector<ui::Widget*>> _dounts;
    vector<Rect> limits;
    ImageLabel* _lable;
    vector<int> numContanier;
    void showLogo(string path = "");
    Sprite* _logo;
    Node* dountContainer;
      bool isEnd;
    Vec2 _faidPos;
    static const string _eventName;
};


#endif /* TouchGame_hpp */
