#include "SpriteObject.h"
#include "FramePart.h"
#include "VisibleRect.h"

SpriteObject::SpriteObject(void)
{
}

SpriteObject::~SpriteObject(void)
{
}

float SpriteObject::radius()
{
    return getTexture()->getContentSize().width / 2;
}

SpriteObject* SpriteObject::ballWithTexture(Texture2D* aTexture)
{
    SpriteObject* spriteObject = new SpriteObject();
    spriteObject->initWithTexture(aTexture);
    spriteObject->autorelease();

    return spriteObject;
}

void SpriteObject::move(float delta)
{
    this->setPosition(getPosition() + _velocity * delta);
    
    if (getPosition().x > VisibleRect::right().x - radius()) 
    {
        setPosition( Vec2( VisibleRect::right().x - radius(), getPosition().y) );
        _velocity.x *= -1;
    } 
    else if (getPosition().x < VisibleRect::left().x + radius()) 
    {
        setPosition( Vec2(VisibleRect::left().x + radius(), getPosition().y) );
        _velocity.x *= -1;
    }
}

void SpriteObject::collideWithFrame(FramePart* framePart)
{
    auto framePartRect = framePart->getRect();
    framePartRect.origin.x += framePart->getPosition().x;
    framePartRect.origin.y += framePart->getPosition().y;
    
    float lowY  = framePartRect.getMinY();
    float midY  = framePartRect.getMidY();
    float highY = framePartRect.getMaxY();
    
    float leftX  = framePartRect.getMinX();
    float rightX = framePartRect.getMaxX();
    
    if (getPosition().x > leftX && getPosition().x < rightX) {
    
        bool hit = false;
        float angleOffset = 0.0f; 
        
        if (getPosition().y > midY && getPosition().y <= highY + radius()) 
        {
            setPosition( Vec2(getPosition().x, highY + radius()) );
            hit = true;
            angleOffset = (float)M_PI / 2;
        }
        else if (getPosition().y < midY && getPosition().y >= lowY - radius()) 
        {
            setPosition( Vec2(getPosition().x, lowY - radius()) );
            hit = true;
            angleOffset = -(float)M_PI / 2;
        }
        
        if (hit) 
        {
            float hitAngle = (framePart->getPosition() - getPosition()).getAngle() + angleOffset;
            
            float scalarVelocity = _velocity.getLength() * 1.05f;
            float velocityAngle = -_velocity.getAngle() + 0.5f * hitAngle;
            
            _velocity = Vec2::forAngle(velocityAngle) * scalarVelocity;
        }
    }    
} 
