//
//  SVGCache.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/21/15.
//
//

#ifndef __ScribbleDemo_3_2__SVGCache__
#define __ScribbleDemo_3_2__SVGCache__

#include <stdio.h>
#include <string>
#include <map>
#include "cocos2d.h"
#include "SVGSource.h"
#include "SVGFill.h"
using namespace cocos2d;
using namespace std;
class SVGCache : public Ref{
public:
#pragma mark - Singleton support
    static SVGCache *getInstance();
    static void purge();
    
#pragma mark - method
    virtual bool init();
    SVGSource *addData(Data *pData, string pKey, SVGFill::FillState pState);
    SVGSource *addFile(string pFileName, SVGFill::FillState pState);
    SVGSource *addSource(SVGSource *pSource, string pKey);
    SVGSource *sourceForKey(string pKey);
    void removeAllSources();
    void removeAllUnusedSources();
    void removeSource(SVGSource *pData);
    void removeSource(string pkey);
private:
    SVGCache(){};
private:
    static SVGCache *sharedSVGCache;
    typedef map<string, SVGSource *> SVGSources;
    CC_SYNTHESIZE(SVGSources, sources_, Sources);
};
#endif /* defined(__ScribbleDemo_3_2__SVGCache__) */
