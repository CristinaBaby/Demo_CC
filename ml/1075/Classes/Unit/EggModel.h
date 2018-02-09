
#ifndef __EggModel__
#define __EggModel__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DragNode.h"

USING_NS_CC;
using namespace cocos2d::ui;

class EggModel : public Node
{
public:
    EggModel();
    ~EggModel();
    
    CREATE_FUNC(EggModel);
    
    virtual bool init();
    
    void setModelType(int index,std::string name,std::string path);
    void addEgg();
    void addIngredient(std::string path,float dt,Vec2 pos = Vec2::ZERO);
    void fryEgg(int status);
    void fryEgg(float dt,int count);
    void showModel(bool showModel = true);
    Rect getEggRectWorld();
protected:
    int _fryCount;
    int _type;
    int _eggCount;
    int _status;
    std::string _path;
    std::string _name;
    std::vector<Sprite*> _eggVector;
    std::vector<Sprite*> _modelVector;
};


#endif