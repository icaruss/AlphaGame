#ifndef _FRAMEPART_H_
#define _FRAMEPART_H_

#include "cocos2d.h"

USING_NS_CC;

typedef enum tagFramePartState 
{
    kFramePartStateGrabbed,
    kFramePartStateUngrabbed
} FramePartState; 

class FramePart : public Sprite, public Clonable
{
    FramePartState        _state;

public:
    FramePart(void);
    virtual ~FramePart(void);

    Rect getRect();
    bool initWithTexture(Texture2D* aTexture);
    virtual void onEnter() override;
    virtual void onExit() override;
    bool containsTouchLocation(Touch* touch);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    virtual FramePart* clone() const;

    static FramePart* createWithTexture(Texture2D* aTexture);
};

#endif
