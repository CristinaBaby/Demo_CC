//
//  BaseDecorationLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef BaseDecorationLayer_hpp
#define BaseDecorationLayer_hpp

#include "BaseStepLayer.h"
#include "StudioLayer.h"
#include "MyScollView.h"
class BaseDecorationLayer: public BaseStepLayer{
public:
    BaseDecorationLayer();
    virtual ~BaseDecorationLayer();
protected:
    virtual void creatIcon(size_t _index);
    void showCatagore();
    void hideCatagore();
    virtual void showEnd(){}
    virtual void hideEnd(){}
    virtual void reset(){};
    virtual void gotoShop();
    virtual void touchEnd(ui::Widget* widget);
    virtual void catagoreClick(KDS::EventSprite*,size_t,KDS::MyScollView*);
    virtual void freeClick(KDS::EventSprite*,size_t,KDS::MyScollView*) =0;
    void saveImage(Node*,Size _size,string _name = "imageCache.png");
    void hideUi(Node* _root);
    void onEnterTransitionDidFinish();
protected:
    string _showIconAciton,_hideIconAction,resetName,resetMessage,_lockImage;
    vector<string> categoryNames;
    KDS::MyScollView* _decoView;
    string _currentcategory;
    string shopClassName;
    static const string _lockName;
    Sprite* _chooseBg;
};

#endif /* BaseDecorationLayer_hpp */
