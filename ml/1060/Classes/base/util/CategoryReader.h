//
//  CategoryReader.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/24.
//
//

#ifndef CategoryReader_hpp
#define CategoryReader_hpp

#include "FilePathUtil.h"
#include <vector>
class CategoryReader{
public:
    static vector<string> getIconPath(string categoryName);
    static string getCategoryPath(string categoryName ,int index);
};

#endif /* CategoryReader_hpp */
