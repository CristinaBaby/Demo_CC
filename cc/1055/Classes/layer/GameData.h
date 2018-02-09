//
//  GameData.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#ifndef GameData_hpp
#define GameData_hpp

#include <stdio.h>
#include "SingleTon.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;



class GameData :public SingleTon<GameData> {
private:
    friend class SingleTon<GameData>;
    GameData();
    virtual ~GameData(){}
public:
    
    CC_SYNTHESIZE(string, Flavor, Flavor);
    CC_SYNTHESIZE(string, Mold, Mold);
    
    CC_SYNTHESIZE(string, PopDialogType, PopDialogType);
    
    CC_SYNTHESIZE(string, GameType, GameType);
    
    CC_SYNTHESIZE(string, EatPath, EatPath);
    
    CC_SYNTHESIZE(string, stepOnePath, stepOnePath);
    
    CC_SYNTHESIZE(string, stepTwoPath, stepTwoPath);
    
    CC_SYNTHESIZE(string, StepOtherPath, StepOtherPath);
    
    CC_SYNTHESIZE(int, BowlIndex, BowlIndex);
    
    CC_SYNTHESIZE(bool, IsShowAcg, IsShowAcg);
    
    CC_SYNTHESIZE(bool, IsMoreGame, IsMoreGame);
};

#define GameDataInstance GameData::getInstance()

//游戏弹出对话框的类型
#define PopDialogTypeNewFlavor "newFlavor"
#define PopDialogTypeAllFlavor "unlockAll"
#define PopDialogTypeNo        "noDialog"

//游戏的六种口味
#define FlavorTypeChololateChip "chocolatechips"
#define FlavorTypePeabutbutter  "peanutbutter"
#define FlavorTypeChololate     "chocolate"
#define FlavorTypeVanilla       "vanilla"
#define FlavorTypeMint          "mint"
#define FlavorTypeStrawberry    "strawberry"

//游戏的三种状态
#define GameState      "GameState"
#define GameStateOne   "OneState"
#define GameStateTwo   "TwoState"
#define GameStateThree "ThreeState"

//Btn Tag
#define TAGButtonNext      100
#define TAGButtonBack      101
#define TAGButtonHome      102
#define TAGButtonPhoto     103
#define TAGButtonEatAgain  104
//游戏的模具
#define MoldCat         "cat"
#define MoldPeople      "ginger"
#define MoldHeart       "heart"
#define MoldRound       "round"
#define MoldStar        "star"
#define MoldTree        "tree"

#define GameTypeOne    "GameOne"
#define GameTypeOther  "GameOther"

#define AppResume       "appResume"

#endif /* GameData_hpp */
