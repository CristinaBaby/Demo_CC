
#include "StickFoodHelp.h"

static std::string gName[4] = {"meat","vegetable","seafood","other"};
static std::string gMeatName[2] = {"marinade","sausage"};
StickFoodHelp::StickFoodHelp()
{
    
}

StickFoodHelp::~StickFoodHelp()
{
    
}

Sprite* StickFoodHelp::createStick(vector<string> dataVector,bool roasted)
{
    Sprite* pStick = Sprite::create("content/make/skewer/stick.png");
    for_each(dataVector.begin(), dataVector.end(), [=](string str){
        std::ostringstream ostr;
        std::vector<std::string> vec = StringHelp::split(str, ",");
        int type = stoi(vec.at(0));
        int index = stoi(vec.at(1));
        if (roasted) {
            ostr<<"content/make/skewer/"<<gName[type]<<"/"<<gName[type]<<index<<"_3.png";
        }else{
            
            ostr<<"content/make/skewer/"<<gName[type]<<"/"<<gName[type]<<index<<"_1.png";
        }
        Sprite* pFood = Sprite::create(ostr.str());
        
        int childCount = pStick->getChildren().size();
        pFood->setTag(childCount);
        pStick->addChild(pFood);
        pFood->setAnchorPoint(Vec2(0, 0.5));
        Vec2 position = Vec2::ZERO;
        if (childCount==0) {
            position = Vec2(150, pStick->getContentSize().height/2);
        }else{
            Node* pNode = pStick->getChildByTag(childCount-1);
            if (pNode) {
                position = Vec2(pNode->getBoundingBox().getMaxX()+8, pNode->getPositionY());
            }
        }
        pFood->setPosition(position);
        
    });
    return pStick;
}

vector<Sprite*> StickFoodHelp::createBBQMeat(vector<string> dataVector,bool roasted)
{
    vector<Sprite*> meatVector;
    for_each(dataVector.begin(), dataVector.end(), [=,&meatVector](string str){
        std::ostringstream ostr;
        std::vector<std::string> vec = StringHelp::split(str, ",");
        int type = stoi(vec.at(0));
        int index = stoi(vec.at(1));
        if (roasted) {
            if (type==0) {
                ostr<<"content/make/bbq/"<<gMeatName[type]<<"/"<<gMeatName[type]<<index<<"_4.png";
            }else{
                
                ostr<<"content/make/bbq/"<<gMeatName[type]<<"/"<<gMeatName[type]<<index<<"_3.png";
            }
        }else{
            if (type==0){
                ostr<<"content/make/bbq/"<<gMeatName[type]<<"/"<<gMeatName[type]<<index<<"_1.png";                
            }else{
                ostr<<"content/make/bbq/"<<gMeatName[type]<<"/"<<gMeatName[type]<<index<<"_2.png";
            }
            
        }
        Sprite* pFood = Sprite::create(ostr.str());
        meatVector.push_back(pFood);
        
    });

    return meatVector;
}