//
//  FrameWorkHelper.cpp
//  FashionDoll
//
//  Created by zhangguangzong1 on 15-4-19.
//
//

#include "FrameWorkHelper.h"

bool FrameWorkHelper::splitWithForm(const std::string& content, strArray& strs)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(content.empty());
        
        size_t nPosLeft  = content.find('{');
        size_t nPosRight = content.find('}');
        
        // don't have '{' and '}'
        CC_BREAK_IF(nPosLeft == std::string::npos || nPosRight == std::string::npos);
        // '}' is before '{'
        CC_BREAK_IF(nPosLeft > nPosRight);
        
        const std::string pointStr = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
        // nothing between '{' and '}'
        CC_BREAK_IF(pointStr.length() == 0);
        
        size_t nPos1 = pointStr.find('{');
        size_t nPos2 = pointStr.find('}');
        // contain '{' or '}'
        CC_BREAK_IF(nPos1 != std::string::npos || nPos2 != std::string::npos);
        
        split(pointStr, ",", strs);
//        if (strs.size() != 2 || strs[0].length() == 0 || strs[1].length() == 0)
//        {
//            strs.clear();
//            break;
//        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}