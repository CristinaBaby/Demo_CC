//
//  CupCakeOvenPlateNode.cpp
//  shake5008
//
//  Created by liji on 17/1/4.
//
//

#include "CupCakeOvenPlateNode.h"
#include "BowlNode.h"
#include "ESDataManager.h"
#include "ComponetFade.h"
bool CupCakeOvenPlateNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }
    
    _plate = BowlNode::create("res/rainbow_cupcake/7/ovenware_s/ovenware_s.png","");
    this->addChild(_plate);
    
    auto btm = Sprite::create(StringUtils::format("res/rainbow_cupcake/7/ovenware_s/%d_0.png",kESDataManager->getRainbowcupcakeFood().cupIndex));
    _plate->addChild(btm,1);

//    _plate->initMaskNode("res/cake_pop/step3_oven/shadow1.png",Vec2(2, 32));
    
    return true;
}

void CupCakeOvenPlateNode::showMixtureInPlate(int mouldIndex, int colorIndex)
{

    auto string = StringUtils::format("res/cake_pop/step3_oven/oven_mixture/mixture%d.png",colorIndex);
    
    _mix = Sprite::create(string);
    _plate->addContentToMask(_mix, StringUtils::format("mix%d",mouldIndex), 1);
    _mix->setScale(0);
    _mix->runAction(ScaleTo::create(3.f, 1));
//    _plate->showContentInMask(StringUtils::format("mix%d",mouldIndex),3.f);
}

void CupCakeOvenPlateNode::showAllMixture()
{
    auto string = StringUtils::format("res/rainbow_cupcake/7/ovenware_mix/%d.png",kESDataManager->getRainbowcupcakeFood().colorIndexVec.back());
    _mix = Sprite::create(string);
    _plate->addContent(_mix, StringUtils::format("mix"), 3);
    _mix->setOpacity(255);
}

void CupCakeOvenPlateNode::showAllMixtureBaked()
{
    auto string = StringUtils::format("res/rainbow_cupcake/7/ovenware_cake/%d.png",kESDataManager->getRainbowcupcakeFood().colorIndexVec.back());
    _mix = Sprite::create(string);
    _plate->addContent(_mix, StringUtils::format("mix"), 4);
}

void CupCakeOvenPlateNode::bakeMixture(float duration)
{
    auto pathBefore = StringUtils::format("res/rainbow_cupcake/7/ovenware_mix/%d.png",kESDataManager->getRainbowcupcakeFood().colorIndexVec.back());
    auto pathAfter = StringUtils::format("res/rainbow_cupcake/7/ovenware_cake/%d.png",kESDataManager->getRainbowcupcakeFood().colorIndexVec.back());
    
    auto cFade = ComponetFade::create(pathBefore, pathAfter);
    _mix->addComponent(cFade);
    cFade->setFadeDuration(duration);
    cFade->start();
    cFade->setName("cFade");
}
