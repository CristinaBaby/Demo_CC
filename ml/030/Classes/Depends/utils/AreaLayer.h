#ifndef __AreaLayer__Utils__
#define __AreaLayer__Utils__

#include "cocos2d.h"

USING_NS_CC;

//A Layer to show Area for node,rect

class AreaLayer : public Layer
{
public:
	AreaLayer();
	~AreaLayer();
public:
	bool init();
	static AreaLayer* create();
	
	
public:
	void onEnter();
	void onExit();
public:
	void AddRectLayerToParent(Node* node,int zOrder = 10);
	void AddNodeLayerToParent(Node* node, int zOrder = 10);

	void setRect(Rect rect);
	void setNode(Node* node);
public:
	Rect  m_rRect;
	Node* m_pNode;

};




#endif


