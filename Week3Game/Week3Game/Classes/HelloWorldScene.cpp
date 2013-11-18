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
    
    _planes = new CCArray;
    _birdy = new CCArray;

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

    //label
    //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    //window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    // pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    //this->addChild(pLabel, 1);

    //set bg
    CCSprite* pSprite = CCSprite::create("bg@2x.png");
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(pSprite, 0);
    
    //set birdy
    CCSprite* bSprite = CCSprite::create("birdy@2x.png");
    bSprite->setPosition( ccp(50, size.height/2) );
    this->addChild(bSprite, 0);
    
    bSprite->setTag(2);
    _birdy->addObject(bSprite);
    
    this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );
    
    return true;
}

void HelloWorld::gameLogic(float dt){
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
    
    // Determine speed of the target
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
    // srand( TimGetTicks() );
    int actualDuration = ( rand() % rangeDuration )
    + minDuration;
    // Create the actions
    CCFiniteTimeAction* actionMove =
    CCMoveTo::create( (float)actualDuration,
                     ccp(0 - target->getContentSize().width/2, actualY) );
    CCFiniteTimeAction* actionMoveDone =
    CCCallFuncN::create( this,
                        callfuncN_selector(HelloWorld::spriteMoveFinished));
    target->runAction( CCSequence::create(actionMove,
                                          actionMoveDone, NULL) );
}


void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    CCSprite *sprite = (CCSprite *)sender;
    this->removeChild(sprite, true);
    
    
    if (sprite->getTag() == 1){
        _planes->removeObject(sprite);
    }else if (sprite->getTag() == 2){
        _birdy->removeObject(sprite);
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
