#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

HelloWorld::~HelloWorld(){
	if (_planes)
	{
		_planes->release();
		_planes = NULL;
	}

}

HelloWorld::HelloWorld()
:_planes(NULL)
,_birdy(NULL)
{
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() ){
        return false;
    }
    
    //_gameNotStarted = true;
    _planes = new CCArray;
    
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    //set bg
    CCSprite* pSprite = CCSprite::create("bg@2x.png");
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(pSprite, 0);
    
    

    this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );
    //this->schedule( schedule_selector(HelloWorld::update) );
    this->scheduleUpdate();
    this->setAccelerometerEnabled(true);
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    char message[11] = "START GAME";
    CCLabelBMFont * startLabel ;
    startLabel = CCLabelBMFont::create(message, "Arial.fnt");
    
    CCMenuItemLabel *startItem = CCMenuItemLabel::create(startLabel, this, menu_selector(HelloWorld::startTapped) );
    startItem->setScale(0.1);
    startItem->setPosition( ccp(winSize.width/2, winSize.height*0.4));
    
    //CCMenu *startmenu = CCMenu::create(startItem, NULL);
    startmenu = CCMenu::create(startItem, NULL);
    startmenu->setPosition(CCPointZero);
    this->addChild(startmenu);
    
    startItem->runAction(CCScaleTo::create(0.5, 1.0));
    
    return true;
}

float HelloWorld::getTimeTick() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}

void HelloWorld::gameLogic(float dt){
    if (_gameOver || _gameNotStarted)
        return;
    
    this->addPlane();
}

void HelloWorld::addPlane()
{
    CCSprite *target = CCSprite::create("plane@2x.png",
                                        CCRectMake(0,0,109,85) );
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height
    -  target->getContentSize().height/2;
    int rangeY = maxY - minY;
    int actualY = ( rand() % rangeY ) + minY;
    
    target->setPosition(
                        ccp(winSize.width + (target->getContentSize().width/2),
                            actualY) );
    this->addChild(target);
    
    target->setTag(1);
    _planes->addObject(target);
    
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
  
    int actualDuration = ( rand() % rangeDuration )
    + minDuration;
    CCFiniteTimeAction* actionMove =
    CCMoveTo::create( (float)actualDuration,
                     ccp(0 - target->getContentSize().width/2, actualY) );
    CCFiniteTimeAction* actionMoveDone =
    CCCallFuncN::create( this,
                        callfuncN_selector(HelloWorld::spriteMoveFinished));
    target->runAction( CCSequence::create(actionMove,
                                          actionMoveDone, NULL) );
}

void HelloWorld::endScene( EndReason endReason ) {
    if (_gameOver)
        return;
    _gameOver = true;
    _birdy->setVisible(false);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    char message[10] = "GAME OVER";
    if ( endReason == IWIN)
        strcpy(message,"YOU WON!");
 
    CCLabelBMFont * label ;
    label = CCLabelBMFont::create(message, "Arial.fnt");
    label->setScale(0.1);
    label->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    this->addChild(label);
    
    CCLabelBMFont * restartLabel;
    strcpy(message,"Restart");
    restartLabel = CCLabelBMFont::create(message, "Arial.fnt");
    
    CCMenuItemLabel *restartItem = CCMenuItemLabel::create(restartLabel, this, menu_selector(HelloWorld::restartTapped) );
    restartItem->setScale(0.1);
    restartItem->setPosition( ccp(winSize.width/2, winSize.height*0.4));
    
    CCMenu *menu = CCMenu::create(restartItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
   
    restartItem->runAction(CCScaleTo::create(0.5, 1.0));
    label ->runAction(CCScaleTo::create(0.5, 1.0));
    // Terminate update callback
    this->unscheduleUpdate();
}

void HelloWorld::update(float dt){
    if (_gameOver == false && _gameNotStarted == false){
        float curTimeMillis = getTimeTick();
        if (curTimeMillis >= _gameOverTime) {
            this->endScene(IWIN);
        }
    }
    
    CCObject* plane = NULL;
    CCARRAY_FOREACH(_planes, plane){
        if (_birdy->boundingBox().intersectsRect(((CCSprite *)plane)->boundingBox()) ) {
            ((CCSprite *)plane)->setVisible(false);
            this->endScene(ILOSE);
        }
    }
}

void HelloWorld::restartTapped() {
    CCDirector::sharedDirector()->replaceScene
    (CCTransitionZoomFlipX::create(0.5, this->scene()));
    
    this->scheduleUpdate();
}

void HelloWorld::startTapped() {
    _gameNotStarted = false;
    this->removeChild(startmenu, true);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    _birdy = CCSprite::create("birdy@2x.png");
    _birdy->setPosition( ccp(50, size.height/2) );
    this->addChild(_birdy, 0);
    
    double curTime = getTimeTick();
    _gameOverTime = curTime + 30000;
}


void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    CCSprite *sprite = (CCSprite *)sender;
    this->removeChild(sprite, true);
    
    if (sprite->getTag() == 1){
        _planes->removeObject(sprite);
    }
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue) {
#define FILTERINGFACTOR 0.1
#define RESTACCELX -0.6
#define BIRDMAXPOINTSPERSEC (winSize.height*0.5)
#define MAXDIFFX 0.2
    
    double rollingX;
    
    // Cocos2DX inverts X and Y accelerometer depending on device orientation
    // in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
    pAccelerationValue->x = pAccelerationValue->y;
    rollingX = (pAccelerationValue->x * FILTERINGFACTOR) + (rollingX * (1.0 - FILTERINGFACTOR));
    float accelX = pAccelerationValue->x - rollingX;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float accelDiff = accelX - RESTACCELX;
    float accelFraction = accelDiff / MAXDIFFX;
    _birdyPointsPerSecY = BIRDMAXPOINTSPERSEC * accelFraction;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
