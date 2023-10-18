//
//  Shake.cpp
//
//

#include "Shake.h"

NS_CC_BEGIN

Shake* Shake::create(float duration, float strength)
{
    return create(duration, strength, strength);
}

Shake* Shake::create(float duration, float strength_x, float strength_y)
{
    auto shake = new(std::nothrow) Shake();
    shake->initWithDuration(duration, strength_x, strength_y);
    shake->autorelease();
    return shake;
}

Shake::Shake(void) :
_initial_x(0),
_initial_y(0)
{
    
}

Shake::~Shake(void)
{
    
}

bool Shake::initWithDuration(float duration, float strength_x, float strength_y)
{
    if ( ActionInterval::initWithDuration(duration) )
    {
        _strength_x = strength_x;
        _strength_y = strength_y;
        return true;
    }
    
    return false;
}

Shake* Shake::clone(void) const
{
    // no copy constructor
    auto a = new (std::nothrow) Shake();
    a->initWithDuration(_duration, _strength_x, _strength_y);
    a->autorelease();
    return a;
}

Shake* Shake::reverse() const
{
    // no copy constructor
    auto a = new (std::nothrow) Shake();
    a->initWithDuration(_duration, _strength_x, _strength_y);
    a->autorelease();
    return a;
}

void Shake::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if ( _target )
    {
        _initial_x = _target->getPosition().x;
        _initial_y = _target->getPosition().y;
    }
}

float fgRangeRand(float min, float max)
{
    float rnd = ((float)rand() / (float)RAND_MAX);
    return rnd * (max - min) + min;
}

void Shake::stop()
{
    if ( _target )
    {
        _target->setPosition(Point(_initial_x, _initial_y));
    }
    ActionInterval::stop();
}

void Shake::update(float time)
{
    auto funcRand = [=](float min, float max)->float{
        
        float randGenerator = ((float)rand() / (float)RAND_MAX);
        return randGenerator * (max - min) + min;
    };
    
    float randx = funcRand( -_strength_x, _strength_x );
    float randy = funcRand( -_strength_y, _strength_y );
    
    if ( _target )
    {
        _target->setPosition(Point(_initial_x + randx, _initial_y + randy));
    }
}

NS_CC_END
