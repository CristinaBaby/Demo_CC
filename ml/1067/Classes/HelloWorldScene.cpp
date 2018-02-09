#include "HelloWorldScene.h"

USING_NS_CC;

enum{Cube, Cone, NumVAOs};
GLuint VAO[NumVAOs];

class Date {
    int d,m,y;
    
public:
    Date(int day,int month,int year){
        d = day;
        m = month;
        y = year;
        cache_valid = false;
    }
//    int day() const{return d;}
//    int month() const{return m;}
//    int year() const{return y;}
    
    inline Date& add_day(int day){
        d+=day;
        return *this;
    }
    inline Date& add_month(int month){
        m+=month;
        return *this;
    }
    inline Date& add_year(int year){
        y+=year;
        return *this;
    }
    int day() const;
    int month() const;
    int year() const;
    
    bool cache_valid;
    std::string cache;
    std::string string_rep() const;
    void compute_cache_value();
};
std::string Date::string_rep()const
{
    if (!cache_valid) {
        Date* th = const_cast<Date*>(this);
//        Date* th = (Date*)this;
        th->compute_cache_value();
        th->cache_valid = true;
    }
    log("=====%d",cache_valid);
    return cache;
}
void Date::compute_cache_value()
{
    
}

inline int Date::day() const
{
    return d;
}
inline int Date::month() const
{
    return m;
}
inline int Date::year() const
{
    return y;
}
struct X{
    int& a;
    const int b = 2;
};

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Date d(20,3,2016);
    const Date cd(20,3,2016);
    d.add_year(3);
    d.year();
    d.add_year(1).add_month(1).add_day(3);
    
    cd.year();
    
    d.string_rep();
    cd.string_rep();
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("loading/loading.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
//    this->addChild(sprite, 0);
//    
//    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    
    auto program = GLProgram::createWithFilenames("myVertextShader.vsh", "myFragmentShader.fsh");
    program->link();
    //set uniform locations
    program->updateUniforms();
    
    
    GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "a_color");
    float uColor[] = {1.0, 0.0, 0.0, 1.0};
    glUniform4fv(uColorLocation,1, uColor);
//    auto size = Director::getInstance()->getWinSize();
//    //指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
//    float vertercies[] = { 0,0,   //第一个点的坐标
//        size.width, 0,   //第二个点的坐标,
//        
//        size.width, size.height,
//        
//        0 , size.height};  //第三个点的坐标
//    //指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
//    float color[] = { 0, 1,0, 0.3,    //第一个点的颜色，绿色
//        1,0,0, 1,  //第二个点的颜色, 红色
//        0, 0, 1, 0.5,
//        
//        0, 1, 1, 0.8};  //第三个点的颜色， 蓝色
//    //激活名字为position和color的vertex attribute
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
//    //分别给position和color指定数据源
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);

    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


//void HelloWorld::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
//{
//    
//    Layer::visit(renderer, parentTransform, parentFlags);
//    _command.init(_globalZOrder);
//    _command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
//    Director::getInstance()->getRenderer()->addCommand(&_command);
//}
//
//void HelloWorld::onDraw()
//{
//    //获得当前HelloWorld的shader
//    auto glProgram = getGLProgram();
//    //使用此shader
//    glProgram->use();
//    //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
//    glProgram->setUniformsForBuiltins();
//    
//    auto size = Director::getInstance()->getWinSize();
//    //指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
//    float vertercies[] = { 0,0,   //第一个点的坐标
//        size.width, 0,   //第二个点的坐标,
//        
//        size.width, size.height,
//        
//        0 , size.height};  //第三个点的坐标
//    //指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
//    float color[] = { 0, 1,0, 0.3,    //第一个点的颜色，绿色
//        1,0,0, 1,  //第二个点的颜色, 红色
//        0, 0, 1, 0.5,
//        
//        0, 1, 1, 0.8};  //第三个点的颜色， 蓝色
//    //激活名字为position和color的vertex attribute
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
//    //分别给position和color指定数据源
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
//    //绘制三角形，所谓的draw call就是指这个函数调用
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
//    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
//    //如果出错了，可以使用这个函数来获取出错信息
//    CHECK_GL_ERROR_DEBUG();
//    
//}