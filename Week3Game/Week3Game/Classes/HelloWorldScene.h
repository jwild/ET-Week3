#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

typedef enum {
    IWIN,
    ILOSE
} EndReason;

class HelloWorld : public cocos2d::CCLayer
{
public:
    HelloWorld();
	~HelloWorld();
    virtual bool init();
    

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    //track accelerameter
    float _birdyPointsPerSecY;
    
    
    double _gameOverTime;
    bool _gameOver;
    bool _gameNotStarted=true;
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void endScene( EndReason endReason );
    void restartTapped();
    void startTapped();
    float getTimeTick();
    
    void addPlane();
    void spriteMoveFinished(CCNode* sender);
    void gameLogic(float dt);
    void update(float dt);
    
   virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
   // virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    protected:
        cocos2d::CCArray *_planes;
        cocos2d::CCSprite *_birdy;
        cocos2d::CCMenu *startmenu;

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
