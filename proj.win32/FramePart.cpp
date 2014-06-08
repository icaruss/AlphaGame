#include "FramePart.h"

FramePart::FramePart(void)
{
}

FramePart::~FramePart(void)
{
}

Rect FramePart::getRect()
{
    auto s = getTexture()->getContentSize();
    return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

FramePart* FramePart::createWithTexture(Texture2D* aTexture)
{
    FramePart* framePart = new FramePart();
    framePart->initWithTexture(aTexture);
    framePart->autorelease();

    return framePart;
}

bool FramePart::initWithTexture(Texture2D* aTexture)
{
    if( Sprite::initWithTexture(aTexture) )
    {
		_state = kFramePartStateUngrabbed;
    }
    
    return true;
}

void FramePart::onEnter()
{
    Sprite::onEnter();
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(FramePart::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(FramePart::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(FramePart::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FramePart::onExit()
{
//    auto director = Director::getInstance();
//    director->getTouchDispatcher()->removeDelegate(this);
    Sprite::onExit();
}    

bool FramePart::containsTouchLocation(Touch* touch)
{
    return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool FramePart::onTouchBegan(Touch* touch, Event* event)
{
    CCLOG("FramePart::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
	if (_state != kFramePartStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
	_state = kFramePartStateGrabbed;
    CCLOG("return true");
    return true;
}

void FramePart::onTouchMoved(Touch* touch, Event* event)
{
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get Sets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    
    CCLOG("FramePart::onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
	CCASSERT(_state == kFramePartStateGrabbed, "FramePart - Unexpected state!");    
    
    auto touchPoint = touch->getLocation();
    
    setPosition( Vec2(touchPoint.x, getPosition().y) );
}

FramePart* FramePart::clone() const
{
    FramePart* ret = FramePart::createWithTexture(_texture);
    ret->_state = _state;
    ret->setPosition(getPosition());
    ret->setAnchorPoint(getAnchorPoint());
    return ret;
}

void FramePart::onTouchEnded(Touch* touch, Event* event)
{
    CCASSERT(_state == kFramePartStateGrabbed, "FramePart - Unexpected state!");    
    
	_state = kFramePartStateGrabbed;
} 
