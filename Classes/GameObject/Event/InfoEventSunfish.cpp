//
//  InfoEventSunfish.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#include "InfoEventSunfish.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventSunfish::InfoEventSunfish() :
_type(0),
_level(0),

_weight(0),
_survival_rate(0),
_bonus_rate(0),
_bonus_weight(0),

_cost(0),
_point(0),

_nameKey(""),
_die_dialogue(""),
_die_text(""),
_illustrated_title_text(""),
_illustrated_text(""),
_survival_text(""),

_reward(""),
_reward_die("")
{
    
}

InfoEventSunfish::~InfoEventSunfish()
{
    
}

bool InfoEventSunfish::init()
{
    return true;
}
 
int InfoEventSunfish::getType()
{
    return _type.valueInt();
}
void InfoEventSunfish::setType(int value)
{
    _type = value;
}

int InfoEventSunfish::getLevel()
{
    return _level.valueInt();
}
void InfoEventSunfish::setLevel(int value)
{
    _level = value;
}

double InfoEventSunfish::getWeight()
{
    return _weight;
}
void InfoEventSunfish::setWeight(double value)
{
    _weight = value;
}

int InfoEventSunfish::getSurvivalRate()
{
    return _survival_rate.valueInt();
}
void InfoEventSunfish::setSurvivalRate(int value)
{
    _survival_rate = value;
}

int InfoEventSunfish::getBonusRate()
{
    return _bonus_rate.valueInt();
}
void InfoEventSunfish::setBonusRate(int value)
{
    _bonus_rate = value;
}

int InfoEventSunfish::getBonusWeight()
{
    return _bonus_weight.valueInt();
}
void InfoEventSunfish::setBonusWeight(int value)
{
    _bonus_weight = value;
}

int InfoEventSunfish::getCost()
{
    return _cost.valueInt();
}
void InfoEventSunfish::setCost(int value)
{
    _cost = value;
}

int InfoEventSunfish::getPoint()
{
    return _point.valueInt();
}
void InfoEventSunfish::setPoint(int value)
{
    _point = value;
}

std::string InfoEventSunfish::getName()
{
    return GAME_TEXT(_nameKey);
}
void InfoEventSunfish::setNameKey(std::string key)
{
    _nameKey = key;
}

std::string InfoEventSunfish::getSkinPath()
{
    std::string path = "";
    
    if ( _type == 1 )
    {
        path = MafUtils::format("Assets/ui/event/lobster_mon_%d_1.png", getLevel());
    }
    else if ( _type == 2 )
    {
        
    }
    else if ( _type == 3 )
    {
        path = MafUtils::format("Assets/ui/event/lobster_event_%d_1.png", getLevel());
    }
    
    return path;
}

std::string InfoEventSunfish::getIconPath()
{
    std::string path = "";
    
    if ( _type == 1 )
    {
        path = MafUtils::format("Assets/ui/event/lobster_icon_mon_1_%d.png", getLevel());
    }
    else if ( _type == 2 )
    {
        path = MafUtils::format("Assets/ui/event/lobster_icon__bg_1_%d.png", getLevel());
    }
    else if ( _type == 3 )
    {
        
    }
    
    return path;
}

std::string InfoEventSunfish::getDieDialogue()
{
    return GAME_TEXT(_die_dialogue);
}
void InfoEventSunfish::setDieDialogueKey(std::string key)
{
    _die_dialogue = key;
}

std::string InfoEventSunfish::getDieText()
{
    return GAME_TEXT(_die_text);
}
void InfoEventSunfish::setDieTextKey(std::string key)
{
    _die_text = key;
}

std::string InfoEventSunfish::getIllustratedTitleText()
{
    return GAME_TEXT(_illustrated_title_text);
}
void InfoEventSunfish::setIllustratedTitleTextKey(std::string key)
{
    _illustrated_title_text = key;
}

std::string InfoEventSunfish::getIllustratedText()
{
    return GAME_TEXT(_illustrated_text);
}
void InfoEventSunfish::setIllustratedTextKey(std::string key)
{
    _illustrated_text = key;
}

std::string InfoEventSunfish::getSurvivalText()
{
    return GAME_TEXT(_survival_text);
}
void InfoEventSunfish::setSurvivalTextKey(std::string key)
{
    _survival_text = key;
}

cocos2d::Vector<InfoItems*> InfoEventSunfish::getReward()
{
    return ItemsManager::getInstance()->getConvertArray(_reward);
}
void InfoEventSunfish::setReward(const std::string value)
{
    _reward = value;
}

cocos2d::Vector<InfoItems*> InfoEventSunfish::getRewardDie()
{
    return ItemsManager::getInstance()->getConvertArray(_reward_die);
}
void InfoEventSunfish::setRewardDie(const std::string value)
{
    _reward_die = value;
}
