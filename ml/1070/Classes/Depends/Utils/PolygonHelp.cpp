
#include "PolygonHelp.h"
using namespace cocos2d;

void PolygonHelp::loadFile(const std::string &plist)
{
    FileUtils *fu= FileUtils::getInstance();
    ValueMap vm;
    
    vm = fu->getValueMapFromFile(plist);//dictionary,如果是vector,使用fu->getValueVectorFromFile(const std::string &filename)
    
    shapMap.operator=(vm);
    
    for (auto it = vm.begin(); it!=vm.end(); ++it){
        ValueVector polygonVector = it->second.asValueVector();
        std::string key = it->first;
//        ValueVector posVec;
//        int i = 0;
//        ValueVector tepVector;
//        for (auto item = polygonVector.begin(); item!=polygonVector.end(); ++item){
//            ValueVector pointVector = item->asValueVector();
//            for (auto point = pointVector.begin(); point!=pointVector.end(); ++point){
//                log("=======%s",point->asString().c_str());
//                Vec2 pos = PointFromString(point->asString());
//                posVec.push_back(Value(point->asString()));
//                i++;
//            }
//            
//        }
        shapMap.insert(ValueMap::value_type(key,Value(polygonVector)));
        
    }
}
void PolygonHelp::addShapesWithFile(const std::string &shape,PhysicsBody* pBody, const float posX, const float posY) {
    
    auto polygon  = shapMap.find(shape);
    if (polygon !=shapMap.end()) {
        ValueVector polygonVector = polygon->second.asValueVector();
        for (auto item = polygonVector.begin(); item!=polygonVector.end(); ++item){
            ValueVector pointVector = item->asValueVector();
            Vec2 pointVec[100];
            int i = 0;
            for (auto point = pointVector.begin(); point!=pointVector.end(); ++point){
                log("=======%s",point->asString().c_str());
                Vec2 pos = PointFromString(point->asString());
                pointVec[i] = pos;
                i++;
            }
            pBody->addShape(PhysicsShapePolygon::create(pointVec, i,PHYSICSSHAPE_MATERIAL_DEFAULT,Vec2(posX,posY)));
        }
//        for (auto it = polygonVector.begin(); it!=polygonVector.end(); ++it){
//            Vec2 pointVec[100];
//            ValueVector PointVector = it->asValueVector();
//            pBody->addShape(PhysicsShapePolygon::create(pointVec, i,PHYSICSSHAPE_MATERIAL_DEFAULT,Vec2(posX,posY)));
//            
//        }
//        for (auto it = polygonVector.begin(); it!=polygonVector.end(); ++it){
//            Vec2 pointVec[100];
//            ValueVector PointVector = it->asValueVector();
//            int i = 0;
//            for (auto point = PointVector.begin(); point!=PointVector.end(); ++point){
//                log("=======%s",point->asString().c_str());
//                Vec2 pos = PointFromString(point->asString());
//                log("=======%f,%f",pos.x,pos.y);
//                pointVec[i] = pos;
//                i++;
//            }
//            pBody->addShape(PhysicsShapePolygon::create(pointVec, i,PHYSICSSHAPE_MATERIAL_DEFAULT,Vec2(posX,posY)));
//            
//        }
        
    }
    
}