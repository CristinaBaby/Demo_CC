//
//  CicleLoadingBar.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/28.
//
//

#ifndef CicleLoadingBar_hpp
#define CicleLoadingBar_hpp

#include "ShadeSprite.h"
class CicleLoadingBar: public KDS::ShadeSprite{
public:
    CicleLoadingBar();
    static CicleLoadingBar*create(string scr,string mask="");
public:
    void setStartRote(float);
    void setEndRote(float);
    void setPercent(float p);
protected:
    float _starRote,endRote;
    void resumAction();
};
#endif /* CicleLoadingBar_hpp */
