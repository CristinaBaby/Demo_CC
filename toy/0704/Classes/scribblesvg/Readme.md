本类库用于针对SVG矢量线框图的填色和涂色。
使用方法参考：
ssh://git@stash.stm.com:7999/cmr/scribbledemo.git

1. iOS配置

(1) Header Search Paths:
$(SRCROOT)/../Classes/#ScribbleSVG_PATH#/ScribbleSVG/svg/MonkSVG
$(SRCROOT)/../Classes/#ScribbleSVG_PATH#/ScribbleSVG/svg/MonkVG/include
$(SRCROOT)/../Classes/#ScribbleSVG_PATH#/ScribbleSVG/svg/MonkVG
$(SRCROOT)/../Classes/#ScribbleSVG_PATH#/ScribbleSVG/svg/
其中，#ScribbleSVG_PATH#为SVG库文件放置的位置，比如，Demo中为:
$(SRCROOT)/../Classes/v2.0/ScribbleSVG/svg/MonkSVG
$(SRCROOT)/../Classes/v2.0/ScribbleSVG/svg/MonkVG/include
$(SRCROOT)/../Classes/v2.0/ScribbleSVG/svg/MonkVG
$(SRCROOT)/../Classes/v2.0/ScribbleSVG/svg/

(2) 开启多重纹理抗锯齿
将AppController.mm文件的方法
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
里面的EAGLView配置修改为如下：
// Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: YES
                                 numberOfSamples: 4];

2. Android 配置

(1) Application.mk配置如下
APP_STL := c++_static
#APP_STL := gnustl_static

APP_ABI := armeabi armeabi-v7a

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_CPPFLAGS += -DTIXML_USE_STL=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_CPPFLAGS += -DTIXML_USE_STL=1
  APP_OPTIM := release
endif

(2) 多重纹理抗锯齿
将文件cocos2d-x-3.2-m/cocos/platform/android/java/src/org/cocos2dx/lib/Cocos2dxGLSurfaceView.java
的public EGLConfig chooseConfig方法内的配置修改如下：
int attribs[] = {
	            EGL10.EGL_LEVEL, 0,
	            EGL10.EGL_RENDERABLE_TYPE, 4,  // EGL_OPENGL_ES2_BIT
	            EGL10.EGL_COLOR_BUFFER_TYPE, EGL10.EGL_RGB_BUFFER,
	            EGL10.EGL_RED_SIZE, 8,
	            EGL10.EGL_GREEN_SIZE, 8,
	            EGL10.EGL_BLUE_SIZE, 8,
	            EGL10.EGL_DEPTH_SIZE, 16,
	            EGL10.EGL_SAMPLE_BUFFERS, 1,
	            EGL10.EGL_SAMPLES, 4,  // This is for 4x MSAA.
	            EGL10.EGL_NONE
	        };

renhong