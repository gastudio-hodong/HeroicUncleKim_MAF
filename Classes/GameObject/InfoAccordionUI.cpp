//
//  InfoAccordionUI.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "InfoAccordionUI.h"

USING_NS_CC;

#pragma mark -
InfoAccordionUI::InfoAccordionUI():
_isActivate(true)
,_nIdx(0)
,_strBoxPath("Assets/ui_common/base_box_1_4.png")
,_strTitle("")
{
    _listText.clear();
    _listValueDouble.clear();
}

InfoAccordionUI::~InfoAccordionUI()
{
    _listText.clear();
    _listValueDouble.clear();
}

bool InfoAccordionUI::init()
{
    return true;
}

const bool InfoAccordionUI::isActive() const
{
    return _isActivate;
}
void InfoAccordionUI::setIsActive(const bool isActive)
{
    _isActivate = isActive;
}

const int InfoAccordionUI::getIdx() const
{
    return _nIdx;
}
void InfoAccordionUI::setIdx(const int value)
{
    _nIdx = value;
}

const std::string InfoAccordionUI::getBoxPath() const
{
    return _strBoxPath;
}
void InfoAccordionUI::setBoxPath(const std::string value)
{
    _strBoxPath = value;
}

const std::string InfoAccordionUI::getTitleText() const
{
    return _strTitle;
}
void InfoAccordionUI::setTitleText(const std::string value)
{
    _strTitle = GAME_TEXT(value);
}

const std::string InfoAccordionUI::getText(const std::string key, const bool isTextKey) const
{
    std::string result = "";
    
    auto iter = _listText.find(key);
    
    if(iter != _listText.end())
        result = iter->second;
    
    if(isTextKey)
        result = GAME_TEXT(result);
    
    return result;
}

void InfoAccordionUI::addText(const std::string key, const std::string value)
{
    auto iter = _listText.find(key);
    
    if(iter == _listText.end())
        _listText.insert(std::make_pair(key, value));
}

const double InfoAccordionUI::getValueDouble(const std::string key) const
{
    double result = 0.f;
    
    auto iter = _listValueDouble.find(key);
    
    if(iter != _listValueDouble.end())
        result = iter->second;
    
    return result;
}

void InfoAccordionUI::addValueDouble(const std::string key, const double value)
{
    auto iter = _listValueDouble.find(key);
    
    if(iter == _listValueDouble.end())
        _listValueDouble.insert(std::make_pair(key, value));
}
