
#ifndef __IAPUnit__
#define __IAPUnit__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class IAPUnit :public Ref{
    
    
public:
    static IAPUnit* createWithDic(ValueMap &map);
    static IAPUnit* create(string sID,
                           string sKeyString,
                           string sImageName,
                           std::vector<std::string>);
//    bool init(string sID,
//              string sKeyString,
//              string sImageName);
    virtual bool initWithDic(ValueMap &map);
    
    string getIapID(){return m_pID;}
    string getKeyString(){return m_pKeyString;}
    string getImageName(){return m_pImageName;}
    std::vector<std::string> getTypeNames(){
        return m_pTypeNamesVector;
    }
protected:
    IAPUnit();
    ~IAPUnit();
    
    string m_pID;
    string m_pKeyString;
    string m_pImageName;
    
    std::vector<std::string> m_pTypeNamesVector;
};
#endif /* defined(__IAPUnit__) */
