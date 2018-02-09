
#include "AreaLayer.h"


AreaLayer::AreaLayer()
{
	
}

AreaLayer::~AreaLayer()
{
}

AreaLayer* AreaLayer::create()
{
	AreaLayer* area = new AreaLayer();
	if (area && area->init())
	{
		area->autorelease();
		
		return area;
	}
	else
	{
		delete area;
		area = NULL;
		return NULL;
	}

}


void AreaLayer::onEnter()
{
	Layer::onEnter();
}
void AreaLayer::onExit()
{
	Layer::onExit();
}

bool AreaLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	

	return true;
}

void AreaLayer::AddRectLayerToParent(Node* node, int zOrder)
{
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 180));
	layer->setContentSize(m_rRect.size);
	this->addChild(layer);

	this->ignoreAnchorPointForPosition(false);
	this->setContentSize(m_rRect.size);
	this->setPosition(m_rRect.origin);
	log("m_rRect---------->%f  %f", m_rRect.origin.x, m_rRect.origin.y);
	this->setAnchorPoint(Vec2(0,0));
	node->addChild(this, zOrder);
}

void AreaLayer::AddNodeLayerToParent(Node* node, int zOrder)
{
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 180));
	layer->setContentSize(m_pNode->getContentSize());
	this->addChild(layer);

	this->ignoreAnchorPointForPosition(false);
	this->setContentSize(m_pNode->getContentSize());
	this->setAnchorPoint(m_pNode->getAnchorPoint());
	this->setPosition(m_pNode->getPosition());
	node->addChild(this,zOrder);
}

void AreaLayer::setRect(Rect rect)
{
	m_rRect = rect;
}

void AreaLayer::setNode(Node* node)
{
	m_pNode = node;
}
