
#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;
MainScene::MainScene()
{
    
}

MainScene::~MainScene()
{
    
}
bool MainScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    FileUtils::getInstance()->addSearchPath("res/");
    Node* logo = CSLoader::createNode("logo.csb");

    
    ui::Helper::doLayout(logo);
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create([=]()
                                                      {
                                                          auto action = CSLoader::createTimeline("logo.csb", logo);
                                                          action->play("animation0", false);
                                                          action->setTimeSpeed(0.5);
                                                          logo->runAction(action);
                                     }), NULL));
    addChildToUILayer(logo);
    
    
    return true;
}

#pragma mark - initData
void MainScene::_initData()
{
    
}