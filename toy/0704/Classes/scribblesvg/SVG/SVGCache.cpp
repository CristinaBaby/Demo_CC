//
//  SVGCache.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/21/15.
//
//

#include "SVGCache.h"
#include <ostream>
SVGCache *SVGCache::sharedSVGCache = nullptr;

#pragma mark - Singleton support
SVGCache *SVGCache::getInstance(){
    if (nullptr == sharedSVGCache) {
        sharedSVGCache = new SVGCache();
    }
    return sharedSVGCache;
}

void SVGCache::purge(){
    sharedSVGCache->release();
    sharedSVGCache = nullptr;
}

#pragma mark - method

bool SVGCache::init(){
    
    return true;
}

SVGSource *SVGCache::addData(Data *pData, string pKey, SVGFill::FillState pState){
    SVGSource *lSource = this->sourceForKey(pKey);
    if (nullptr == lSource) {
        lSource = new SVGSource();
        lSource->initWithData(pData, pState);
        this->sources_.insert(make_pair(pKey, lSource));
    }
    return lSource;
}

SVGSource *SVGCache::addFile(string pFileName, SVGFill::FillState pState){
    std::ostringstream lOS;
    lOS << pFileName;
    lOS << "_";
    lOS << static_cast<int>(pState);
    string lSVGKey = lOS.str();
    SVGSource *lSource = this->sourceForKey(lSVGKey);
    if (nullptr == lSource) {
        lSource = new SVGSource();
        lSource->initWithFile(pFileName, pState);
        this->sources_.insert(make_pair(lSVGKey, lSource));
    }
    return lSource;
}

SVGSource *SVGCache::addSource(SVGSource *pSource, string pKey){
    CCAssert(pSource != nullptr, "source cannot be nil");
    CCAssert(!pKey.empty(), "key cannot be nil");
    CCAssert(this->sourceForKey(pKey) == pSource, "duplicate key");
    
    this->sources_.insert(make_pair(pKey, pSource));
    return pSource;
}

SVGSource *SVGCache::sourceForKey(string pKey){
    map<string, SVGSource *>::iterator lItor = this->sources_.find(pKey);
    if (this->sources_.end() == lItor) {
        return nullptr;
    }
    SVGSource *lSource = this->sources_.find(pKey)->second;
    return lSource;
}

void SVGCache::removeAllSources(){
    for (const auto &kv : this->sources_) {
        SVGSource *lSource = kv.second;
        lSource->release();
    }
    this->sources_.clear();
}

void SVGCache::removeAllUnusedSources(){
    for (const auto &kv : this->sources_) {
        SVGSource *lSource = kv.second;
        if (lSource->getReferenceCount() == 1) {
            lSource->release();
        }
        this->sources_.erase(kv.first);
    }
}

void SVGCache::removeSource(SVGSource *pData){
    for (auto &kv : this->sources_) {
        if (kv.second == pData) {
            kv.second->release();
            this->sources_.erase(kv.first);
        }
    }
}

void SVGCache::removeSource(string pkey){
    SVGSource *lSource = this->sourceForKey(pkey);
    lSource->release();
    this->sources_.erase(pkey);
}