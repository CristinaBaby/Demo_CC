//
//  MakeBaseNode.cpp
//  BHUG1064
//
//  Created by maxiang on 5/26/16.
//
//

#include "MakeBaseNode.h"

MakeBaseNode::~MakeBaseNode()
{
    
}

bool MakeBaseNode::init()
{
    if (!Node::init())
        return false;

    return true;
}

const Size& MakeBaseNode::getContentSize() const
{
    if (_mainContent)
        return _mainContent->getContentSize();
    
    return Size::ZERO;
}

