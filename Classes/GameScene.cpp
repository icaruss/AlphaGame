#include "GameScene.h"
#include "DEFINITIONS.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	points = new vector<Point*>();
	frameDelta = new Point(visibleSize.width * 0.1 , visibleSize.height * 0.1);

   

	/*CCSprite* ball = CCSprite::create("CloseNormal.png");
	ball->setPosition(Point(origin.x + visibleSize.width - ball->getContentSize().width/2 ,
		origin.y + ball->getContentSize().height/2));

	this->addChild(ball,TAG_BALL);*/

   
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1, TAG_LABEL);

    // add "HelloWorld" splash screen"
	string strFolder = FOLDER_BACKGROUND;
	string strImageBackground = IMAGE_BACKGROUND;
    auto sprite = Sprite::create(strFolder + strImageBackground);

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0, TAG_BACKGROUND);
    

	//Objects locations

	int movingObjects = 5;
	int staticObjects = 0;
	CreateObjects(staticObjects, movingObjects);
	moveObjectsRandomly(movingObjects);

    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
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


/************************************************************************/
/*				TOUCHES REGION                                          */
/************************************************************************/

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	
	LabelTTF *label = (LabelTTF*)this->getChildByTag(TAG_LABEL);

	if (isTouchOnFrame(touch->getLocation()))
	{
		label->setColor(Color3B::RED);

		label->setPosition(touch->getLocation());
	}
	else
	{
		label->setColor(Color3B::WHITE);

		label->setPosition(touch->getPreviousLocation());
	}

	return true;
}


void GameScene::onTouchEnded(Touch *touch, Event *event)
{
	LabelTTF *label = (LabelTTF*)this->getChildByTag(TAG_LABEL);

	if (isTouchOnFrame(touch->getLocation()))
	{
		label->setColor(Color3B::RED);

		label->setPosition(touch->getLocation());
	}
	else
	{
		label->setColor(Color3B::WHITE);

		label->setPosition(touch->getPreviousLocation());
	}

}
void GameScene::onTouchMoved(Touch *touch, Event *event)
{
	LabelTTF *label = (LabelTTF*)this->getChildByTag(TAG_LABEL);
	

	if (isTouchOnFrame(touch->getLocation()))
	{
		label->setColor(Color3B::RED);

		label->setPosition(touch->getLocation());
	}
	else
	{
		label->setColor(Color3B::WHITE);

		label->setPosition(touch->getPreviousLocation());
	}

}
void GameScene::onTouchCancelled(Touch *touch, Event *event)
{
	onTouchEnded(touch,event);
}
	


/************************************************************************/
/*						LOGIC REGION                                    */
/************************************************************************/

bool GameScene::isTouchOnFrame(Point point)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	if ( (point.x < frameDelta->x)							||
		 (point.x > visibleSize.width - frameDelta->x)		||
		 (point.y < frameDelta->y)							||
		 (point.y > visibleSize.height - frameDelta->y) )
	{
		return true;
	}

	return false;
}

void GameScene::CreateObjects(int staticObjects, int movingObjects)
{
	//No stsaic objects for now...

	//Moving objects!

	Size visibleSize = Director::getInstance()->getVisibleSize();
	string strFolder = FOLDER_OBJECTS;
	string strImageBackground = IMAGE_BALL;

	for(int i=0; i<movingObjects; ++i)
	{

		auto spriteBall = Sprite::create(strFolder + strImageBackground);
		
		spriteBall->setScale(0.25);

		spriteBall->setPosition(Point((rand() % (int)(visibleSize.width - frameDelta->x)) + frameDelta->x,
										(rand() % (int)(visibleSize.height - frameDelta->y))+frameDelta->y));
		this->addChild(spriteBall,Z_ORDER_BALL,TAG_BALL + i);
	}
}



void GameScene::moveObjectsRandomly(int movingObjects)
{
	for(int i = 0 ; i < movingObjects ; ++i)
	{
		Sprite* spriteBall = (Sprite*) this->getChildByTag(TAG_BALL+i);

		if (spriteBall != nullptr)
		{
			int angle = rand() % 180 + i * 20;
			
			setObjectToMove(spriteBall, angle);
		}
		
	}
}

void GameScene::setObjectToMove(Sprite *sprite, int angle)
{
	Point pointToMoveTo = getNewPositionToMoveToByAngle(angle);

	//Move the ball 
	MoveTo *moveTo = MoveTo::create(1.0f, pointToMoveTo);

	//raotate the ball
	RotateTo *ratateTo = RotateTo::create(1.0f, 180);

	//Run actions simultaneously
	Spawn *spawn = Spawn::create(moveTo, ratateTo, NULL);

	//  recursive sequence
	Sequence *sequence = Sequence::create(	
											spawn , 
											DelayTime::create(1.01f) ,
											CallFunc::create(CC_CALLBACK_0(GameScene::setObjectToMove, this,sprite,angle)) ,
											NULL
										);

	sprite->runAction(sequence);
}



Point GameScene::getNewPositionToMoveToByAngle(int angle)
{

	if (angle % 90 == 0)
	{
		angle += (30 + rand() % 100);
	}

	angle += 90;

	float angleInRadians = angle * M_PI / 180;

	Point anglePoint = ccpForAngle(angleInRadians);


	anglePoint = ccpMult(anglePoint, 100);
	
	// check If point out of frame
	if (isTouchOnFrame(anglePoint))
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();

		if (anglePoint.x <= frameDelta->x)
		{
			anglePoint.x = frameDelta->x;
		}
		else if (anglePoint.x >= (visibleSize.width - frameDelta->x))
		{
			anglePoint.x = visibleSize.width - frameDelta->x;
		}

		if (anglePoint.y <= frameDelta->y)
		{
			anglePoint.y = frameDelta->y;
		}
		else if (anglePoint.y >= (visibleSize.height - frameDelta->y))
		{
			anglePoint.y = visibleSize.height - frameDelta->y;
		}
	}

	return anglePoint;
}