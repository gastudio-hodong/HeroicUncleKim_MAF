//
//  InfoArtifact.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoArtifact.h"

USING_NS_CC;

InfoArtifact::InfoArtifact() :
_idx(E_ARTIFACT::ARTI_NONE),
_type(E_ARTIFACT_TYPE::ARTI_TYPE_NONE),

_title(""),
_desc(""),
_iconPath(""),

_generation(0),
_condition(0),
_levelMax(0),
_levelReinforceMax(0),

_price_type(E_ITEMS::NONE),
_price_point(0),

_prev_artifact(0),

_is_calculator(false),
_is_reinforce(false)
{
    
}

InfoArtifact::~InfoArtifact()
{
    
}

bool InfoArtifact::init()
{
    return true;
}

E_ARTIFACT InfoArtifact::getIdx()
{
    return _idx;
}
void InfoArtifact::setIdx(E_ARTIFACT value)
{
    _idx = value;
}

E_ARTIFACT_TYPE InfoArtifact::getType()
{
    return _type;
}
void InfoArtifact::setType(E_ARTIFACT_TYPE value)
{
    _type = value;
}

std::string InfoArtifact::getTitle()
{
    return GAME_TEXT(_title);
}
void InfoArtifact::setTitleKey(std::string value)
{
    _title = value;
}

std::string InfoArtifact::getDesc()
{
    return GAME_TEXT(_desc);
}
void InfoArtifact::setDescKey(std::string value)
{
    _desc = value;
}

std::string InfoArtifact::getIconPath()
{
    return _iconPath;
}
void InfoArtifact::setIconPath(std::string value)
{
    _iconPath = value;
}

int InfoArtifact::getGeneration()
{
    return _generation;
}
void InfoArtifact::setGeneration(int value)
{
    _generation = value;
}

int InfoArtifact::getCondition()
{
    return _condition;
}
void InfoArtifact::setCondition(int value)
{
    _condition = value;
}

int InfoArtifact::getLevelMax()
{
    return _levelMax;
}
void InfoArtifact::setLevelMax(int value)
{
    _levelMax = value;
}

int InfoArtifact::getLevelReinforceMax()
{
    return _levelReinforceMax;
}
void InfoArtifact::setLevelReinforceMax(int value)
{
    _levelReinforceMax = value;
}

E_ITEMS InfoArtifact::getPriceType()
{
    return _price_type;
}
void InfoArtifact::setPriceType(E_ITEMS value)
{
    _price_type = value;
}

int InfoArtifact::getPricePoint()
{
    return _price_point;
}
void InfoArtifact::setPricePoint(int value)
{
    _price_point = value;
}

int InfoArtifact::getPrevArtifact()
{
    return _prev_artifact;
}
void InfoArtifact::setPrevArtifact(int value)
{
    _prev_artifact = value;
}

bool InfoArtifact::isCalculator()
{
    return _is_calculator;
}
void InfoArtifact::setCalculator(bool value)
{
    _is_calculator = value;
}

bool InfoArtifact::isReinforce()
{
    return _is_reinforce;
}
void InfoArtifact::setReinforce(bool value)
{
    _is_reinforce = value;
}
