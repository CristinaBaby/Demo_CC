//
//  CakepopOvenPlateNode.cpp
//  shake5008
//
//  Created by liji on 17/1/4.
//
//

#include "CakepopOvenPlateNode.h"
#include "BowlNode.h"
#include "ESDataManager.h"
#include "ComponetFade.h"
bool CakepopOvenPlateNode::init()
{
    if (!MakeBaseNode::init())
    {
        return false;
    }
    
    _plate = BowlNode::create("res/cake_pop/step3_oven/pan.png","");
    this->addChild(_plate);
    _plate->initMaskNode("res/cake_pop/step3_oven/shadow1.png",Vec2(2, 32));
    
    return true;
}

void CakepopOvenPlateNode::showMixtureInPlate(int mouldIndex, int colorIndex)
{

    auto string = StringUtils::format("res/cake_pop/step3_oven/oven_mixture/mixture%d.png",colorIndex);
    
    auto mix = Sprite::create(string);
    _plate->addContentToMask(mix, StringUtils::format("mix%d",mouldIndex), 1,_posVec.at(mouldIndex));
    mix->setScale(0);
    mix->runAction(ScaleTo::create(3.f, 1,0.75));
    _mixtureVec.push_back(mix);
//    _plate->showContentInMask(StringUtils::format("mix%d",mouldIndex),3.f);
}

void CakepopOvenPlateNode::showAllMixture()
{
    for (auto p : _mixtureVec)
    {
        p->removeFromParent();
    }
    _mixtureVec.clear();
    for (int mouldIndex = 0; mouldIndex<4; mouldIndex++)
    {
        auto string = StringUtils::format("res/cake_pop/step3_oven/oven_mixture/mixture%d.png",kESDataManager->getRainbowcakepopFood().colorIndexVec.at(mouldIndex));
        auto mix = Sprite::create(string);
        _plate->addContentToMask(mix, StringUtils::format("mix%d",mouldIndex), 1,_posVec.at(mouldIndex));
        _mixtureVec.push_back(mix);
        mix->setTag(mouldIndex);
        mix->setScale(1,0.75);
//        _plate->showContentInMask(StringUtils::format("mix%d",mouldIndex),3.f);
    }
}

void CakepopOvenPlateNode::showAllMixtureBaked()
{
    for (auto p : _mixtureVec)
    {
        p->removeFromParent();
    }

    for (int mouldIndex = 0; mouldIndex<4; mouldIndex++)
    {
        auto string = StringUtils::format("res/cake_pop/step3_oven/oven_mixture/oven_mixture%d.png",kESDataManager->getRainbowcakepopFood().colorIndexVec.at(mouldIndex));
        auto mix = Sprite::create(string);
        _plate->addContentToMask(mix, StringUtils::format("mix%d",mouldIndex), 1,_posVec.at(mouldIndex));
        _mixtureVec.push_back(mix);
        mix->setTag(mouldIndex);
        mix->setScale(1,0.75);
    }
}

void CakepopOvenPlateNode::bakeMixture(float duration)
{
    for (auto mix : _mixtureVec)
    {
        mix->setScale(1,0.75);

        auto pathBefore = StringUtils::format("res/cake_pop/step3_oven/oven_mixture/mixture%d.png",kESDataManager->getRainbowcakepopFood().colorIndexVec.at(mix->getTag()));
        auto pathAfter = StringUtils::format("res/cake_pop/step3_oven/oven_mixture/oven_mixture%d.png",kESDataManager->getRainbowcakepopFood().colorIndexVec.at(mix->getTag()));

        auto cFade = ComponetFade::create(pathBefore, pathAfter);
        mix->addComponent(cFade);
        cFade->setFadeDuration(duration);
        cFade->start();
        cFade->setName("cFade");
    }


}
