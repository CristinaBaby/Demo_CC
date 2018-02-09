
#ifndef __IngredientManager__
#define __IngredientManager__


#include "SingleTon.h"
#include "cocos2d.h"
#include "DragNode.h"
#include "KettleNode.h"

using namespace std;
USING_NS_CC;

class IngredientManager : public SingleTon<IngredientManager>
{
public:
    void addIngredient(DragNode* pIngredient);
    void removeIngredient(DragNode* pIngredient);
    void showShadow(int index,bool show);
    void frozeIngredients(int index,bool froze = true);
    void addContainer(Node* pIngredient,std::string path,std::string name[5],bool water = false);
    void showIngredient(int index);
    
    void reset(){
        m_IngredientVector.clear();
    }
protected:
    DragNode* _createDrageNode(std::string path,bool enable);
    
    Vector<DragNode*> m_IngredientVector;
};

#endif