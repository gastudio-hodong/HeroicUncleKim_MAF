//
//  UIDialogueText.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/14.
//

#include "UIDialogueText.h"

USING_NS_CC;

UIDialogueText* UIDialogueText::create(Size size, Vec2 anchor)
{
    UIDialogueText* pRet = new(std::nothrow) UIDialogueText();
    if ( pRet && pRet->init(size, anchor) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

UIDialogueText::UIDialogueText(void) :
_currentIdx(0)
,_speed(0.001)
,_text(nullptr)
,_callbackEndEvent(nullptr)
{
    _listLine.clear();
}

UIDialogueText::~UIDialogueText(void)
{
    _listLine.clear();
}

bool UIDialogueText::init(Size size, Vec2 anchor)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    //
    setAnchorPoint(anchor);
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    
    
    _text = ui::Text::create("", GAME_FONT, 30);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    _text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    _text->setPosition(getContentSize()/2);
    _text->setTextAreaSize(Size(getContentSize().width - 20, getContentSize().height - 10));
     ((Label*)_text->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
     addChild(_text);
    
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        onEventClick();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
#pragma mark - utils
const bool UIDialogueText::isEnd() const
{
    bool result = false;
    
    if(_currentIdx == _listLine.size() - 1 && _text->getString().compare(_listLine[_currentIdx]) == 0)
        result = true;
    
    return result;
}
const bool UIDialogueText::isLineEnd() const
{
    bool result = false;
    
    if(_text->getString().compare(_listLine[_currentIdx]) == 0)
        result = true;
    
    return result;
}
void UIDialogueText::setTextSpeed(const float value)
{
    _speed = value;
}
void UIDialogueText::addText(const std::string line)
{
    _listLine.push_back(line);
}

void UIDialogueText::setListText(const std::vector<std::string> list)
{
    _listLine = list;
}

void UIDialogueText::setTextAnchorPoint(Vec2 anchor)
{
    _text->setAnchorPoint(anchor);
}
void UIDialogueText::setTextAlignment(TextVAlignment v, TextHAlignment h)
{
    _text->setTextVerticalAlignment(v);
    _text->setTextHorizontalAlignment(h);
}
void UIDialogueText::setTextPosition(Vec2 pos)
{
    _text->setPosition(pos);
}
void UIDialogueText::setTextDefaultColor(const Color3B color)
{
    _text->setColor(color);
}
#pragma mark - dialogue
void UIDialogueText::startDialogue()
{
    _currentIdx = 0;
    _text->setString("");
    runAction(Sequence::create(DelayTime::create(_speed), CallFunc::create(CC_CALLBACK_0(UIDialogueText::dialogueLine, this)), nullptr));
}
void UIDialogueText::nextLine()
{
    if(_currentIdx < _listLine.size() - 1)
    {
        _currentIdx++;
        _text->setString("");
        runAction(Sequence::create(DelayTime::create(_speed), CallFunc::create(CC_CALLBACK_0(UIDialogueText::dialogueLine, this)), nullptr));
    }
}

void UIDialogueText::dialogueLine()
{
    std::string strText = _listLine[_currentIdx];
    if(_text->getString().compare(strText) != 0)
    {
        std::string tmp = _text->getString();
        if(tmp.length() < strText.length())
        {
            if ((strText.at(tmp.length()) & 0x80) != 0x80)
                tmp.push_back(strText.at(tmp.length()));
            else
                tmp.append(strText.substr(tmp.length(), 3));
        }
        _text->setString(tmp);
        runAction(Sequence::create(DelayTime::create(_speed), CallFunc::create(CC_CALLBACK_0(UIDialogueText::dialogueLine, this)), nullptr));
    }
    _text->setPosition(_text->getPosition());
}

#pragma mark - utils
void UIDialogueText::onEventNextLine()
{
    if(_listLine.size() <= 0)
        return;
    
    if(_text->getString().compare(_listLine[_currentIdx]) == 0)
    {
        nextLine();
    }
    else
    {
        stopAllActions();
        _text->setString(_listLine[_currentIdx]);
    }
}

void UIDialogueText::onEventClick()
{
    if(_callbackEndEvent != nullptr)
        _callbackEndEvent();
    else
        onEventNextLine();
}

void UIDialogueText::setCallbackEndEvent(const std::function<void(void)>& callback)
{
    _callbackEndEvent = callback;
}
