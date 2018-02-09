
#include "ChooseBgScene.h"
#include "ChooseBackgroudLayer.h"
#include "SceneManager.h"
#include "SSCFileUtility.h"

ChooseBgScene::ChooseBgScene()
{
    m_pTable = nullptr;
}

ChooseBgScene::~ChooseBgScene()
{
    
}
bool ChooseBgScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    Sprite* pBg = Sprite::create("content/make/dec/dec_bk.jpg");
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    m_pBg = pBg;
    
    
    std::stringstream ostrKid;
    int kidIndex = arc4random()%3;
    ostrKid<<"content/common/make/child"<<kidIndex+1<<"/";
    Sprite* pKid = Sprite::create(ostrKid.str()+"1.png");
    pKid->setTag(0);
    CMVisibleRect::setPosition(pKid, 60,300,kBorderRight);
    this->addChildToContentLayer(pKid);
    pKid->setLocalZOrder(-1);
    
    Sprite* pEye = Sprite::create(ostrKid.str()+"wink.png");
    pKid->addChild(pEye);
    pEye->setName("eye");
    pEye->setPosition(pKid->getContentSize()*0.5);
    pEye->setOpacity(0);
    pEye->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.02),
                                                           DelayTime::create(0.3),
                                                           FadeOut::create(0.02),
                                                           DelayTime::create(3),
                                                           NULL)));
    m_pCandleLayer = Layer::create();
    this->addChildToContentLayer(m_pCandleLayer);
    m_pCandleLayer->setLocalZOrder(1);
    m_pCandleLayer->setScale(0.6);
    m_pCandleLayer->setPosition(0, -visibleSize.height*0.1);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pDecoratedCake = Sprite::create(SSCFileUtility::getStoragePath()+"decorate.png");
#else
    m_pDecoratedCake = Sprite::create(SSCFileUtility::getStoragePath()+"/decorate.png");
#endif
    m_pCandleLayer->addChild(m_pDecoratedCake);
    m_pDecoratedCake->setPosition(visibleSize.width/2,visibleSize.height*0.5);
    
    float delt = 100;
    std::vector<Vec3> candleVector = GameDataManager::getInstance()->m_VectorCandle;
    CandlePosition candlePos;
    Vec2 pos = CMVisibleRect::getPositionAdapted(Vec2(320+delt, 260+20));
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
    
    _showChooseLayer();
    AudioHelp::getInstance()->playBackGroundMusic("bg_theme.mp3");
    return true;
}

#pragma mark - initData
void ChooseBgScene::_initData()
{
    setExPath("content/category/bg/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecGift;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagDecGift;
    
}

void ChooseBgScene::_showChooseLayer()
{
    ChooseBackgroudLayer* pLayer = ChooseBackgroudLayer::create();
    pLayer->onItemCallback = CC_CALLBACK_1(ChooseBgScene::onBgCallback, this);
    this->addChildToUILayer(pLayer);
}

void ChooseBgScene::onBgCallback(int index)
{
    std::stringstream ostr;
    ostr<<"scene"<<index<<".jpg";
    m_pBg->setTexture(localPath(ostr.str()));
    
    std::stringstream ostrTable;
    ostrTable<<"table"<<index<<".png";
    if (!m_pTable) {
        m_pTable = Sprite::create(localPath(ostrTable.str()));
        this->addChildToContentLayer(m_pTable);
        CMVisibleRect::setPosition(m_pTable, 480, 20);
    }else{
        m_pTable->setTexture(localPath(ostrTable.str()));
    }
    GameDataManager::getInstance()->m_nBgIndex = index;
    m_pGameUI->showNextLayout();
    
    AudioHelp::getInstance()->playEffect("finish_particle.mp3");
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
    this->addChildToUILayer(pParticle);
    pParticle->setPosition(visibleSize*0.5);
    this->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=]()
                                                      {
                                                          SceneManager::pushTheScene<DecorateConeScene>();
                                                      }), NULL));
}