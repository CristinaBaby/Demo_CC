

#include "GetGiftScene.h"

GetGiftScene::GetGiftScene()
{
    
}

GetGiftScene::~GetGiftScene()
{
    
}

bool GetGiftScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    Sprite* pBg = Sprite::create("content/make/dec/dec_bk.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    Sprite* pDec;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    pDec = Sprite::create(FileUtility::getStoragePath()+"dec2.png");
#else
    pDec = Sprite::create(FileUtility::getStoragePath()+"/dec2.png");
#endif
    if (pDec) {
        this->addChildToContentLayer(pDec);
        pDec->setPosition(visibleSize.width/2,visibleSize.height/2);
        pDec->setLocalZOrder(1);
    }
    
    m_pCandleLayer = Layer::create();
    this->addChildToContentLayer(m_pCandleLayer);
    m_pCandleLayer->setLocalZOrder(1);
    m_pCandleLayer->setScale(0.6);
    m_pCandleLayer->setPosition(0, -visibleSize.height*0.1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"decorate.png");
#else
    m_pDecoratedCake = Sprite::create(FileUtility::getStoragePath()+"/decorate.png");
#endif
    m_pCandleLayer->addChild(m_pDecoratedCake);
    m_pDecoratedCake->setPosition(visibleSize.width/2,visibleSize.height*0.5);
    
    std::vector<Vec3> candleVector = GameDataManager::getInstance()->m_VectorCandle;
    CandlePosition candlePos;
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320, 260));
    candlePos.init(5, 5, 65, 48, pos);
    for_each(candleVector.begin(), candleVector.end(), [=,&candlePos](Vec3 vec){
        int index = vec.x;
        int type = vec.y;
        int tag = vec.z;
        Vec2 worldPos = candlePos.getPosition(index);
        Vec2 posCandle = m_pContentLayer->convertToNodeSpace(worldPos);
        CandleNode* pCandle = CandleNode::create();
        pCandle->initCandle(type, tag);
        pCandle->setFirePosition(CandleData::getCandleData(type).firePos);
        if (type==3) {
            if(tag==6){
                pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+18,CandleData::getCandleData(type).firePos.y-11));
            }else if(tag==7){
                pCandle->setFirePosition(Vec2(CandleData::getCandleData(type).firePos.x+21,CandleData::getCandleData(type).firePos.y));
            }
        }
        pCandle->setPosition(Vec2(posCandle.x, posCandle.y-CandleData::getCandleData(type).holderHeight));
        m_pCandleLayer->addChild(pCandle);
        pCandle->setLocalZOrder(50-index);
        pCandle->setTag(0);
        m_VectorCandle.pushBack(pCandle);
    });
    m_pTool = _createDrageNode("content/make/dec/knife1.png");
    this->addChildToContentLayer(m_pTool);
    m_pTool->setLocalZOrder(50);
    CMVisibleRect::setPosition(m_pTool, 480, 320);
    m_pTool->setActionPoint(Vec2(18, 313));
    m_pTool->setLimitRect(Rect(0, 0, visibleSize.width, visibleSize.height));
//    m_pGameUI->showBackLayout();
    
    m_pCandleLayer->runAction(Sequence::create(DelayTime::create(0.5),
                                               Spawn::create(MoveBy::create(0.5, Vec2(0, visibleSize.height*0.1)),
                                                             ScaleTo::create(0.5, 1), NULL), NULL));
    return true;
}

#pragma mark - initData
void GetGiftScene::_initData()
{
    setExPath("content/category/candle/");
    m_nNextSceneTag = GameUIEvent::eSceneTagShare;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagShare;
    m_bFired = false;
    m_bDark = false;
}


void GetGiftScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    
}
void GetGiftScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    if (m_pTool==pDragNode) {
        for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=](CandleNode* pCandle){
            if (pCandle->getTag()==0) {
                Vec2 pos = pCandle->getFireWorldPos();
                Rect rect = Rect(pos.x-10, pos.y-10, 20, 20);
                if (rect.containsPoint(worldPoint)) {
                    stringstream ostr;
                    int candleType = pCandle->getCandleType();
                    int candleIndex = pCandle->getCandleIndex();
                    if (candleType==0) {
                        ostr<<"particle/firework"<<candleIndex<<".plist";
                    }else{
                        ostr<<"particle/fire.plist";
                    }
                    ParticleSystemQuad* pParticle = ParticleSystemQuad::create(ostr.str());
                    pCandle->addFire(pParticle);
                    pCandle->setTag(1);
                    m_bFired = true;
                }
            }
        });
        
    }
}
void GetGiftScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (m_pTool==pDragNode) {
        if (m_bFired) {
            _lightCandle();
            m_pTool->setTouchEnabled(false);
            m_pTool->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 100)));
            Sprite* pDark = nullptr;
            if (!m_bDark) {
                m_bDark = true;
                pDark = Sprite::create("content/make/dec/black.png");
                this->addChildToContentLayer(pDark);
                pDark->setPosition(visibleSize*0.5);
                pDark->setOpacity(0);
                pDark->runAction(RepeatForever::create(Sequence::create(EaseBounceIn::create(FadeTo::create(1, 255)),
                                                                        EaseSineIn::create(FadeTo::create(1, 220)), NULL)));
            }
            this->runAction(Sequence::create(DelayTime::create(10),
                                             CallFunc::create(CC_CALLBACK_0(GetGiftScene::_dieoutCandle, this)),
                                             CallFunc::create([=]()
                                                              {
                                                                  if (pDark) {
                                                                      pDark->stopAllActions();
                                                                      pDark->runAction(Sequence::create(FadeOut::create(1),
                                                                                                        CallFunc::create([=](){
                                                                          m_pGameUI->showNextLayout();
                                                                          ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                                                                          this->addChildToUILayer(pParticle);
                                                                          pParticle->setPosition(visibleSize*0.5);
                                                                      }), NULL));
                                                                  }
                                                              }), NULL));
        }
    }
}
void GetGiftScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

void GetGiftScene::_lightCandle()
{
    for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=](CandleNode* pCandle){
        if (pCandle->getTag()==0) {
            stringstream ostr;
            int candleType = pCandle->getCandleType();
            int candleIndex = pCandle->getCandleIndex();
            if (candleType==0) {
                ostr<<"particle/firework"<<candleIndex<<".plist";
            }else{
                ostr<<"particle/fire.plist";
            }
            ParticleSystemQuad* pParticle = ParticleSystemQuad::create(ostr.str());
            pCandle->addFire(pParticle);
            pCandle->setTag(1);
        }
    });
}

void GetGiftScene::_dieoutCandle()
{
    for_each(m_VectorCandle.begin(), m_VectorCandle.end(), [=](CandleNode* pCandle){
        pCandle->removeFire();
        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/smoke.plist");
        pCandle->addFire(pParticle);
    });
}