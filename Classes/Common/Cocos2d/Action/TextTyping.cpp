//
//  TextTyping.cpp
//
//

#include "TextTyping.h"
#include "ui/UIText.h"

NS_CC_BEGIN

TextTyping* TextTyping::create(float duration)
{
    auto fromTo = new (std::nothrow) TextTyping();
    fromTo->initWithDuration(duration);
    fromTo->autorelease();
    return fromTo;
}

TextTyping::TextTyping() :
_length(0),
_idxStart(0),
_idxEnd(0)
{
    
}

TextTyping::~TextTyping()
{
    if (_target)
    {
        Label* text = nullptr;
        if ( ui::Text* temp = dynamic_cast<ui::Text*>(_target) )
        {
            text = (Label*)temp->getVirtualRenderer();
        }
        else if ( Label* temp = dynamic_cast<Label*>(_target) )
        {
            text = temp;
        }

        for (int32_t i = 0; i < text->getStringLength(); i++)
        {
            auto sprite = text->getLetter(i);
            if ( sprite != nullptr )
            {
                sprite->setOpacity(255);
            }
        }
    }
}

bool TextTyping::initWithDuration(float duration)
{
    if(ActionInterval::initWithDuration(duration)) {
        return true;
    }
    return false;
}

TextTyping* TextTyping::clone() const
{
    auto a = new(std::nothrow) TextTyping();
    a->initWithDuration(_duration);
    a->autorelease();
    return a;
}


TextTyping* TextTyping::reverse() const
{
    return TextTyping::create(_duration);
}

void TextTyping::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    
    Label* text = nullptr;
    if ( ui::Text* temp = dynamic_cast<ui::Text*>(_target) )
    {
        text = (Label*)temp->getVirtualRenderer();
    }
    else if ( Label* temp = dynamic_cast<Label*>(_target) )
    {
        text = temp;
    }

    _length = text->getStringLength();
    for (int32_t i = 0; i < _length; i++)
    {
        auto sprite = text->getLetter(i);
        if ( sprite != nullptr )
        {
            sprite->setOpacity(0);
        }
    }
}

void TextTyping::update(float time)
{
    int idxEnd = _length * time;
    if ( _idxEnd == idxEnd )
    {
        return;
    }
    
    //
    _idxEnd = idxEnd;
    
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
    
    for (int32_t i = _idxStart; i < _idxEnd; i++)
    {
        auto sprite = text->getLetter(i);
        if ( sprite != nullptr )
        {
            sprite->setOpacity(255);
        }
    }
    
    //
    _idxStart = _idxEnd;
}

NS_CC_END
