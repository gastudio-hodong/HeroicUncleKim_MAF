//
//  TintBlink.cpp
//
//

#include "TintBlink.h"

NS_CC_BEGIN

TintBlink* TintBlink::create(float duration, Color3B color)
{
    return create(duration, Color3B::WHITE, color);
}

TintBlink* TintBlink::create(float duration, Color3B from, Color3B to)
{
    auto tint = new (std::nothrow) TintBlink();
    tint->initWithDuration(duration, from, to);
    tint->autorelease();
    return tint;
}

TintBlink::TintBlink() :
_tick(0),
_isChange(false)
{
    
}

TintBlink::~TintBlink()
{
    
}

bool TintBlink::initWithDuration(float duration, Color3B from, Color3B to)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _fromColor = from;
        _toColor = to;
        return true;
    }

    return false;
}

TintBlink* TintBlink::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) TintBlink();
    a->initWithDuration(_duration, _fromColor, _toColor);
    a->autorelease();
    return a;
}

TintBlink* TintBlink::reverse() const
{
    // no copy constructor
    auto a = new (std::nothrow) TintBlink();
    a->initWithDuration(_duration, _toColor, _fromColor);
    a->autorelease();
    return a;
}

void TintBlink::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (_target)
    {
        _fromColor = _target->getColor();
    }
}

void TintBlink::stop()
{
    if (_target)
    {
        _target->setColor(_fromColor);
    }
    ActionInterval::stop();
}
 
void TintBlink::update(float time)
{
    if (_target)
    {
        _tick += time;
        if ( _tick > 2.5f )
        {
            _tick = 0.0f;
            _target->setColor((!_isChange) ? _toColor : _fromColor);

            //
            _isChange = !_isChange;
        }
    }
}

NS_CC_END
