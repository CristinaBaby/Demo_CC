
#include "EggModel.h"
#include "Global.h"
#include "AudioHelp.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"


using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

EggModel::EggModel()
{
    _fryCount = 4;
    _eggCount = 0;
}

EggModel::~EggModel()
{
    
}
bool EggModel::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    return true;
}

void EggModel::setModelType(int index,std::string name,std::string path)
{
    this->removeAllChildren();
    _modelVector.clear();
    _type = index;
    _name = name;
    _path = path;
    if (_type<4) {
        for (int i =0; i<2; i++) {
            Sprite* pModel = Sprite::create(_path+_name+"_0.png");
            this->addChild(pModel,2*i);
            _modelVector.push_back(pModel);
        }
    }else{
        for (int i =0; i<3; i++) {
            Sprite* pModel = Sprite::create(_path+"special"+std::to_string(_type-3)+"_"+std::to_string(i)+".png");
            this->addChild(pModel,2*i);
            _modelVector.push_back(pModel);
        }
    }
}

void EggModel::showModel(bool showModel){
    for_each(_modelVector.begin(), _modelVector.end(), [=](Sprite* pModel){
        pModel->setVisible(showModel);
    });
}

void EggModel::addEgg()
{
    if (_type<4) {
        if (_eggCount>=1) {
            return;
        }
        Sprite* pEgg = Sprite::create(_path+"egg_1.png");
        this->addChild(pEgg,1);
        _eggVector.push_back(pEgg);
        _eggCount++;
    }else{
        if (_eggCount>=2) {
            return;
        }
        if(_eggCount==0){
            for (int i =0; i<2; i++) {
                Sprite* pEgg = Sprite::create(_path+"egg"+std::to_string(i+1)+"_"+std::to_string(_eggCount)+".png");
                this->addChild(pEgg,i*2+1);
                _eggVector.push_back(pEgg);
            }
            _eggCount++;
        }else{
            for (int i =0; i<2; i++) {
                Sprite* pEgg = _eggVector.at(i);
                pEgg->setTexture(_path+"egg"+std::to_string(i+1)+"_"+std::to_string(_eggCount)+".png");
            }
            
        }
    }
}

Rect EggModel::getEggRectWorld()
{
    if(_eggVector.size()==0){
        return Rect::ZERO;
    }
    Rect rect = _eggVector.at(0)->getBoundingBox();
    rect.origin = this->convertToWorldSpace(rect.origin);
    return rect;
}
void EggModel::addIngredient(std::string path,float dt,Vec2 pos)
{
    Sprite* pAdd = Sprite::create(path);
    this->addChild(pAdd,10);
    pAdd->setOpacity(0);
    pAdd->setPosition(pos);
    pAdd->runAction(FadeIn::create(dt));
}

void EggModel::fryEgg(float dt,int count)
{
    AudioHelp::getInstance()->playLoopEffect("frying_food.mp3");
    _status =  1;
    _fryCount = count;
    this->runAction(Repeat::create(Sequence::create(DelayTime::create(dt/count),
                                                    CallFunc::create([=](){
        fryEgg(_status++);
    }), NULL), count));
    
}
void EggModel::fryEgg(int status)
{
    if(status>_fryCount){
        return;
    }
    if (_type<4){
        Sprite* pEgg = _eggVector.at(0);
        if (pEgg) {
            Sprite* pEggTemp = Sprite::create(_path+"egg_"+std::to_string(status+1)+".png");
            this->addChild(pEggTemp,1);
            pEggTemp->setOpacity(0);
            pEggTemp->runAction(Sequence::create(FadeIn::create(0.8),
                                                 CallFunc::create([=](){
                pEgg->setTexture(pEggTemp->getTexture());
                pEggTemp->removeFromParent();
            }), NULL));
        }
    }else{
        for (int i = 0; i<2; i++) {
            Sprite* pEgg = _eggVector.at(i);
            Sprite* pEggTemp = Sprite::create(_path+"egg"+std::to_string(i+1)+"_"+std::to_string(status+1)+".png");
            this->addChild(pEggTemp,i*2+1);
            pEggTemp->setOpacity(0);
            pEggTemp->runAction(Sequence::create(FadeIn::create(0.8),
                                                 CallFunc::create([=](){
                pEgg->setTexture(pEggTemp->getTexture());
                pEggTemp->removeFromParent();
            }), NULL));
        }
        
    }
}