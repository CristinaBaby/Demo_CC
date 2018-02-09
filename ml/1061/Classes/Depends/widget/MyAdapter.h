//
//  MyAdapter.h
//  MyCppGame
//
//  Created by youjinchao1 on 14-9-25.
//
//

#ifndef __MyCppGame__MyAdapter__
#define __MyCppGame__MyAdapter__

#include "KDAdapter.h"

class MyAdapter : public KDAdapter{
protected:
    std::string iconName = "";
    
public:
    Vector<__String*> itemName;
    Vector<__String*> itemHName;
    
public:
    MyAdapter(Vector<__String*> itemName, Vector<__String*> itemHName, std::string name);
    MyAdapter(Vector<__String*> itemName, Vector<__String*> itemHName);
    int getCount();
    bool isEmpty();
    float getItemWidth();
    float getItemHeight();
    void setIconName(std::string name);
    std::string getIconName();
    virtual MenuItem* getView(int position,MenuItem* cacheView,Node* parent);
};

#endif /* defined(__MyCppGame__MyAdapter__) */
