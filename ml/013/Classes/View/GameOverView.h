//
//  GameOverView.h
//  Make
//
//  Created by QAMAC01 on 15-4-14.
//
//

#ifndef __Make__GameOverView__
#define __Make__GameOverView__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;

class GameOverView : public BaseLayer
{
public:

    static GameOverView* create(bool isWin);
    bool init(bool isWin);
    
private:
    virtual void nextBtnClicked();
    virtual void resetGameBtnClicked();
    
    void initWinGame();
    void initLoseGame();

};

#endif /* defined(__Make__GameOverView__) */
