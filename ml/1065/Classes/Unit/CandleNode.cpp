
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

void CandleNode::initCandle(int type, int index)
{
    m_nType = type;
    m_nIndex = index;
    std::stringstream ostr;
    ostr<<"content/category/candle/"<<gCandleName[type]<<"/"<<index<<"_1.png";
    m_pCandle = Sprite::create(ostr.str());
    this->addChild(m_pCandle);
    m_pCandle->setAnchorPoint(Vec2(0.5, 0));
    m_pCandle->setPosition(Vec2::ZERO);
}