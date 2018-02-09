//
//  CBQuitGameLayer.cpp
//  ColorBook
//
//  Created by maxiang on 6/3/15.
//
//

#include "CBQuitGameLayer.h"
USING_NS_CC;
CB_BEGIN_NAMESPACE

#define TEXT_QUIT_GAME    "Are you sure you\nwant to exit the\ngame?"

bool QuitGameLayer::init()
{
    if (!DownloadFaildLayer::init()) {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _textLabel->setString(TEXT_QUIT_GAME);
        
        rcode = true;
    } while (0);
    
    return rcode;
}

void QuitGameLayer::setText(const std::string text)
{
    _textLabel->setString(text);
}


CB_END_NAMESPACE