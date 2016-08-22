#include "AppDelegate.h"
#include "MainScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    #if true == KT_USE_PAK
        CCLOG("\n\n============= NOTE: is using PAK ===============\n");
        FileUtils::getInstance()->setPakFile("res_pak.pak");
    #endif
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Big Two");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    this->setResolution();

    // create a scene. it's an autorelease object
    auto scene = MainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::setResolution()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    Size frameSize = glview->getFrameSize();
    glview->setDesignResolutionSize(1280, 850, ResolutionPolicy::FIXED_HEIGHT);
    
    auto vsize = director->getVisibleSize();
    FULL_WIDTH  = vsize.width;
    FULL_HEIGHT = vsize.height;
    HALF_WIDTH  = floor(FULL_WIDTH/2);
    HALF_HEIGHT = floor(FULL_HEIGHT/2);
    SCALE_X = FULL_WIDTH / 1280;
    SCALE_Y = FULL_HEIGHT / 800;
    CCLOG("width[%d]height[%d]half_width[%d]half_height[%d]",FULL_WIDTH,FULL_HEIGHT,HALF_WIDTH,HALF_HEIGHT);
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
