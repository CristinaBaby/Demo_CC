//
//  BaseShopLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/30.
//
//

#ifndef BaseShopLayer_hpp
#define BaseShopLayer_hpp

#include "BaseStepLayer.h"
class BaseShopLayer:public BaseStepLayer{
public:
    BaseShopLayer();
    virtual ~BaseShopLayer();
protected:
    std::vector<std::string> _packgeName;
    std::string _restoreBtn,closeBtn;
    std::string _showAction,closeAction;
protected:
      virtual void touchEnd(ui::Widget* widget);
    virtual bool onBack();
    virtual void onEnterTransitionDidFinish();
    virtual void onExitTransitionDidStart();
    void close();
    virtual void restoreCall(bool isScucess, bool isHasData);
    virtual void  purchaseCall(bool flag);
    virtual void purchse(int index);
public:
    std::function<void()> _callBack;
};

#endif /* BaseShopLayer_hpp */
