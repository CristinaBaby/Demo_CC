//
//  Pricelayer.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/15.
//
//

#ifndef Pricelayer_hpp
#define Pricelayer_hpp

#include "BaseStepLayer.h"
#include "NumEditBox.h"
#include "EventLayer.h"
#include "TouchMoveComponent.h"
class Pricelayer: public BaseStepLayer{
    DECLARE_DYNCLASS(Pricelayer);
public:
    CREATE_FUNC(Pricelayer);
    Pricelayer();
    ~Pricelayer();
protected:
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
       virtual void onExitTransitionDidStart();
    NumEditBox* _numBox;
    void showText(Vec2 pos);
    void hideText(Vec2 pos);
    virtual void touchEnd(ui::Widget* widget);
    void moveEnd(Node*,Component*,OperateListner*);
    Vector<LQPolygon*> limits;
    Vector<TouchMoveComponent*> _dountOperats;
    Vector<Node*> layers;
    Node* _currentLayer;
    Vec2 addPos;
    int crruntTarget;
    void saveToMenu();
    std::string gameFoodPath;
    void touchAction(Node* _node);
    vector<string> dountPath;
    vector<string> dountPrice;
    vector<string> paths;
    int curentIndex;
    
    static const string _eventName;
};

#endif /* Pricelayer_hpp */
