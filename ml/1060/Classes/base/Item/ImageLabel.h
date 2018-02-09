//
//  ImageLabel.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/13.
//
//

#ifndef ImageLabel_hpp
#define ImageLabel_hpp
#include"cocos2d.h"
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio;
using namespace std;
class ImageLabel:public Node{
public:
	enum class Direction{
		center,
		right,
		left
	};
	CC_SYNTHESIZE(Direction,_direction,Direction);
    CC_SYNTHESIZE(float,_space,Space);
public:
	ImageLabel();
    virtual ~ImageLabel();
    CREATE_FUNC(ImageLabel);
	static ImageLabel* create(const map<char,string>&);
    static ImageLabel* createWithPlist(const string& _fileNames);
public:
	void setNodes(const Map<char,Sprite*>&);
	void setNodesPath(const map<char,string>&);
    void setPlistFile(const string& _fileNames);
	void addNode(char,Sprite*);
	void setText(const string&);
protected:
	Map<char,Sprite*> *nodes;
	Node* _container;
};

#endif /* ImageLabel_hpp */
