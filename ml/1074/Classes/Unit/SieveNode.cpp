
#include "SieveNode.h"
#include "SceneManager.h"
#include "ActionHelper.h"

SieveNode::SieveNode()
{
    m_pPowder = nullptr;
}

SieveNode::~SieveNode()
{
}
bool SieveNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_pBody = Sprite::create("content/make/common/sleve/sieve0.png");
    this->addChild(m_pBody);
    
    m_pBodyFront = Sprite::create("content/make/common/sleve/sieve1.png");
    this->addChild(m_pBodyFront,10);
    m_bIsWorking = false;
    m_powderRect = Rect(-160,0,320,10);
    return true;
}

void SieveNode::onEnter()
{
    Node::onEnter();
}

void SieveNode::setSieve(std::string bodyPath,std::string frontPath)
{
    m_pBody->setTexture(bodyPath);
    m_pBodyFront->setTexture(frontPath);
}

void SieveNode::powderFadeOut(float dt)
{
    if (m_pPowder) {
        m_pPowder->runAction(FadeOut::create(dt));
    }
    
}

void SieveNode::setPowder(std::string path,float dt)
{
    if (!m_pPowder) {
        m_pPowder = Sprite::create(path);
        this->addChild(m_pPowder,1);
    }else{
        m_pPowder->setTexture(path);
    }
    if(dt==0){
        m_pPowder->setOpacity(0);
        m_pPowder->runAction(FadeIn::create(dt));
    }else{
        m_pPowder->setOpacity(255);
    }
}

void SieveNode::separatePowder()
{
    if(m_bIsWorking){
        return;
    }
    AudioHelp::getInstance()->playLoopEffect("shake_bowl.mp3");
    m_origngPos = this->getPosition();
    m_bIsWorking = true;
    this->runAction(RepeatForever::create(ActionHelper::createShakeAction(10,0.1)));
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
        for (int i = 0; i<25; i++) {
            Sprite* pFall = Sprite::create("content/make/common/sleve/flour_fall.png");
            this->addChild(pFall);
            pFall->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            Vec2 pos = Vec2(m_powderRect.origin.x+m_powderRect.size.width/40*(arc4random()%40), 0);
            pFall->setPosition(pos);
            pFall->setScale(1,0);
            pFall->setOpacity(0);
            pFall->runAction(Sequence::create(DelayTime::create(0.01),
                                              Spawn::create(ScaleTo::create(0.5, 1),
                                                            MoveBy::create(0.6, Vec2(0, -120-8.0*(arc4random()%10))),
                                                            FadeIn::create(0.5), NULL),
                                              CallFunc::create([=](){
                pFall->removeFromParent();
            }), NULL));
        }
    }),
                                                           DelayTime::create(0.2), NULL)));
}

void SieveNode::stopSeparate()
{
    AudioHelp::getInstance()->stopLoopEffect();
    this->stopAllActions();
    this->setPosition(m_origngPos);
    m_bIsWorking = false;
    
}