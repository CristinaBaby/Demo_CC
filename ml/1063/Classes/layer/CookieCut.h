//
//  CookieCut.hpp
//  WeddingFoodMaker
//
//  Created by luotianqiang1 on 16/2/26.
//
//

#ifndef CookieCut_hpp
#define CookieCut_hpp
#include "BaseStepLayer.h"
#include "MyScollView.h"
#include "WeddingFoodData.h"
class CookieCut:public BaseStepLayer {
public:
    CREATE_SECENE(CookieCut);
    CookieCut();
protected:
    bool init();
    void onEnterTransitionDidFinish();
     virtual void touchEnd(ui::Widget* widget);
protected:
    KDS::MyScollView* _listView;
    vector<Rect> limits;
    decltype(WeddingFoodData::getCookieShapes()) cookieIndexs;
};


#endif /* CookieCut_hpp */
