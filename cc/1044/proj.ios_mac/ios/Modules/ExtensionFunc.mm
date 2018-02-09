//
//  ExtensionFunc.cpp
//  BBQParty
//
//  Created by liuwei on 14-8-11.
//
//

#include "ExtensionFunc.h"
#include "MoreGameURL.h"
ExtensionFunc::ExtensionFunc(){
    
}

ExtensionFunc::~ExtensionFunc(){
    
}

string ExtensionFunc::getMoregameUrl(){
    NSString* urlString = [MoreGameURL MoreGameUrl];
    stringstream stream;
    stream<<[urlString UTF8String];
    return stream.str();
}

