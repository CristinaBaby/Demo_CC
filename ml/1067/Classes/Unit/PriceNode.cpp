
#include "PriceNode.h"
#include "Global.h"

PriceNode::PriceNode()
{
    m_pContentSrite = nullptr;
    m_pLock = nullptr;
    m_bLocked = false;
}

PriceNode::~PriceNode()
{
}
bool PriceNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    return true;
}

void PriceNode::initWithName(const std::string nomal,const std::string highlight,int price,bool locked)
{
    m_sNormalPath = nomal;
    m_sHighlightPath = highlight;
    m_nPrice = price;
    
    if (std::strcmp(nomal.c_str(), "")!=0){
        m_pContentSrite = Sprite::create(nomal);
        this->addChild(m_pContentSrite);
        this->setContentSize(m_pContentSrite->getContentSize());
    }
    
    ostringstream ostr;
    ostr<<m_nPrice;
    m_pPriceLable = Label::createWithTTF( ostr.str(), font_felt, 20);
    this->addChild(m_pPriceLable);
    m_pPriceLable->setLocalZOrder(5);
    
    
    m_bLocked = locked;
    if (m_bLocked) {
        if (std::strcmp(highlight.c_str(), "")!=0) {
            m_pContentSrite->setTexture(highlight);
        }
    }
    
}

void PriceNode::unlock(bool unlock)
{
    if (unlock) {
        m_bLocked =  false;
        if (std::strcmp(m_sNormalPath.c_str(), "")!=0) {
            m_pContentSrite->setTexture(m_sNormalPath);
        }
    }else{
        m_bLocked =  true;
        if (std::strcmp(m_sHighlightPath.c_str(), "")!=0) {
            m_pContentSrite->setTexture(m_sHighlightPath);
        }
    }
    
}
