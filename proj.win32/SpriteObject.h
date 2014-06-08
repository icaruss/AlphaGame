#ifndef _SPRITEOBJECT_H_
#define _SPRITEOBJECT_H_

#include "cocos2d.h"

class Paddle;

USING_NS_CC;

class SpriteObject : public Sprite
{
    Vec2 _velocity;
public:
    SpriteObject(void);
    virtual ~SpriteObject(void);

    float radius();

    void move(float delta);
    void collideWithFrame(FramePart* framePart);


public:
    void setVelocity(Vec2 velocity){_velocity = velocity;}
    Vec2 getVelocity(){return _velocity;}

public:
    static SpriteObject* ballWithTexture(Texture2D* aTexture);
};

#endif
