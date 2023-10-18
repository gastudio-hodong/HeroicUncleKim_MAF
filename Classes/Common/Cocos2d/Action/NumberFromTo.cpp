//
//  NumberFromTo.cpp
//
//

#include "NumberFromTo.h"
#include "ui/UIText.h"

NS_CC_BEGIN

NumberFromTo* NumberFromTo::create(float duration, int from, int to, E_TYPE type/* = E_TYPE::NUMBER*/)
{
    auto fromTo = new (std::nothrow) NumberFromTo();
    fromTo->initWithDuration(duration, from, to, type);
    fromTo->autorelease();
    return fromTo;
}

NumberFromTo::NumberFromTo() :
_to(0),
_from(0),

_type(E_TYPE::NUMBER)
{
    
}

NumberFromTo::~NumberFromTo()
{
    
}

bool NumberFromTo::initWithDuration(float duration, int from, int to, E_TYPE type)
{
    if(ActionInterval::initWithDuration(duration)) {
        _to = to;
        _from = from;
        _type = type;
        return true;
    }
    return false;
}

NumberFromTo* NumberFromTo::clone() const
{
    auto a = new(std::nothrow) NumberFromTo();
    a->initWithDuration(_duration, _from, _to, _type);
    a->autorelease();
    return a;
}


NumberFromTo* NumberFromTo::reverse() const
{
    return NumberFromTo::create(_duration, _to, _from, _type);
}

void NumberFromTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
}

void NumberFromTo::update(float time)
{
    int result = _from + (_to - _from) * time;
    
    //
    Label* text = nullptr;
    if ( ui::Text* temp = dynamic_cast<ui::Text*>(_target) )
    {
        text = (Label*)temp->getVirtualRenderer();
    }
    else if ( Label* temp = dynamic_cast<Label*>(_target) )
    {
        text = temp;
    }
    
    if ( _type == E_TYPE::NUMBER )
    {
        text->setString(StringUtils::format("%d", result));
    }
    else if ( _type == E_TYPE::TIME_M_S )
    {
        int32_t m = result / 60;
        int32_t s = result % 60;
        
        text->setString(StringUtils::format("%02d:%02d", m, s));
    }
 
    
    //"%.1f"
}

NS_CC_END
