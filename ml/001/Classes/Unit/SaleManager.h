
#ifndef __SaleManager__
#define __SaleManager__


#include "SingleTon.h"
#include "DressUpData.h"
#include <iostream>
#include <vector>
#include "StringHelp.h"

#include "cocos2d.h"
#include "RoleModel.h"

USING_NS_CC;
using namespace std;
class SaleManager : public SingleTon<SaleManager>
{
public:
    
    RoleModel* produceGuest();
    RoleModel* getRole(){
        return m_pRole;
    }
    int getRoleIndex(){
        return m_nIndex;
    }
    void guestCome();
    void guestStandby();
    void guestStandby2();
    void guestStandbyPay();
    void guestBack();
    void guestGetPizza(Node* pizza,float scale,int type);
    void guestGetCoint(Node* pCoint);
    void guestPay(Node* pCoint);
    void guestSad();
    
    void showDialog(Node* pTip);
    void removeDialog();
protected:
    int m_nIndex;
    RoleModel* m_pRole;
    
    Sprite* m_pDialog;
};

#endif