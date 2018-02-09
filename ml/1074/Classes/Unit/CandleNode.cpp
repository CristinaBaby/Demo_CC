
#include "CandleNode.h"
#include "GameDataManager.h"

CandleNode::CandleNode()
{
    m_pCandle = nullptr;
}

CandleNode::~CandleNode()
{
    
}

bool CandleNode::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}

void CandleNode::initCandle(int type,int index)
{
    m_nType = type;
    m_nIndex = index;
    std::stringstream ostr;
    ostr<<"content/category/candle/"<<gCandleName[type]<<"/"<<index<<"_1.png";
    m_pCandle = Sprite::create(ostr.str());
    this->addChild(m_pCandle);
    m_pCandle->setAnchorPoint(Vec2(0.5, 0));
    m_pCandle->setPosition(Vec2::ZERO);
    this->setContentSize(m_pCandle->getContentSize());
}
void CandleNode::initCandle(int index)
{
    int type = calculateType(index);
    int num = calculateNum(index);
    initCandle(type,num);
}

int CandleNode::calculateType(int index)
{
    int type = 1;
    if (index<26) {
        type = 1;
    }else if (index<36) {
        type = 2;
    }else {
        type = 3;
    }
    return type;
}

int CandleNode::calculateNum(int index)
{
    int num = index;
    if (index<26) {
        num = index;
    }else if (index<36) {
        num = index-26;
    }else {
        num = index-36;
    }
    return num;
}