//
//  ComponetFade.cpp
//  ccats1061
//
//  Created by liji on 16/8/8.
//
//

#include "ComponetFade.h"


ComponetFade* ComponetFade::create(string pathBefore,string pathAfter)
{
    auto ret = new (std::nothrow) ComponetFade();
    if (ret)
    {
        ret->init(pathBefore,pathAfter);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;


}

bool ComponetFade::init(string pathBefore,string pathAfter)
{
    _pathBefore = pathBefore;
    _pathAfter = pathAfter;
    
    return true;
}

void ComponetFade::onAdd()
{
    _owner = dynamic_cast<Sprite*>(getOwner());
    
    _owner->setTexture(_pathAfter);
    _owner->setOpacity(0);
    _imageBefore = Sprite::create(_pathBefore);
    _owner->addChild(_imageBefore,999);
    _imageBefore->setAnchorPoint(Vec2::ZERO);

};


void ComponetFade::start()
{
    _owner->runAction(FadeIn::create(_fadeDuaration));
    
    _imageBefore->runAction(Sequence::create(FadeOut::create(_fadeDuaration),
                                             CallFunc::create([=]
    {
        _isFadeOut = true;
        if (_fadeDoneCallback)
            _fadeDoneCallback();
        
    }),
                                             NULL));
}

void ComponetFade::pause()
{
    Director::getInstance()->getActionManager()->pauseTarget(_imageBefore);

}
void ComponetFade::resume()
{
    Director::getInstance()->getActionManager()->resumeTarget(_imageBefore);

}

void ComponetFade::stop()
{
    _isFadeOut = false;
    _imageBefore->setOpacity(0);

}


