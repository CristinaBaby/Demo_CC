
#include "CocostudioReader.h"
#include "cocostudio/CocoStudio.h"

void CocostudioReader::loadScene(std::string path)
{
    pScene = CSLoader::createNode(path);
}

Vec2 CocostudioReader::getPosition(std::string name)
{
    Node* pNode = pScene->getChildByName(name);
    return pNode?pNode->getPosition():Vec2::ZERO;
}