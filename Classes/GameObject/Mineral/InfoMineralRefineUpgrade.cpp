//
//  InfoMineralRefineUpgrade.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#include "InfoMineralRefineUpgrade.h"

#include "ManagerGame/ItemsManager.h"

InfoMineralRefineUpgrade::InfoMineralRefineUpgrade() :
_idx(0),

_effect(0),

_levelMax(0),

_costIncrease(0)
{
    
}

InfoMineralRefineUpgrade::~InfoMineralRefineUpgrade()
{
}

bool InfoMineralRefineUpgrade::init()
{
    return true;
}
 
int InfoMineralRefineUpgrade::getIdx()
{
    return _idx;
}

void InfoMineralRefineUpgrade::setIdx(int value)
{
    _idx = value;
}

std::string InfoMineralRefineUpgrade::getName()
{
    return GAME_TEXT(MafUtils::format("t_ui_mine_mk2_stat_%d", _idx));
}

std::string InfoMineralRefineUpgrade::getDesc()
{
    return GAME_TEXT(MafUtils::format("t_ui_mine_mk2_stat_info_%d", _idx));
}

std::string InfoMineralRefineUpgrade::getIconPath()
{
    return MafUtils::format("Assets/ui/mine/smelt_item_%02d.png", _idx);
}

double InfoMineralRefineUpgrade::getEffect()
{
    return _effect;
}
void InfoMineralRefineUpgrade::setEffect(double value)
{
    _effect = value;
}

int InfoMineralRefineUpgrade::getLevelMax()
{
    return _levelMax;
}

void InfoMineralRefineUpgrade::setLevelMax(int value)
{
    _levelMax = value;
}

InfoItems* InfoMineralRefineUpgrade::getCost()
{
    InfoItems* result = nullptr;
    
    if ( _listCost.size() > 0 )
    {
        result = _listCost.at(0);
    }
    
    return result;
}

cocos2d::Vector<InfoItems*> InfoMineralRefineUpgrade::getCostMine()
{
    return _listCostMine;
}

void InfoMineralRefineUpgrade::setCost(std::string value)
{
    _listCost.clear();
    _listCostMine.clear();
    
    //
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)(const char*)value.c_str()).HasParseError();
    if( value.length() != 0 && jsonParser.HasParseError() == false )
    {
        auto objItem = ItemsManager::getInstance()->getInfoItems(jsonParser["cost"]["idx"].GetInt());
        if ( objItem != nullptr )
        {
            objItem->setCount(jsonParser["cost"]["count"].GetInt());
            _listCost.pushBack(objItem);
        }
        
        for ( int i = 0; i < jsonParser["mine"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["mine"][i];
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(jsonValue["idx"].GetInt());
            if ( objItem != nullptr )
            {
                objItem->setCount(jsonValue["count"].GetInt());
                _listCostMine.pushBack(objItem);
            }
        }
    }
}

double InfoMineralRefineUpgrade::getCostIncrease()
{
    return _costIncrease;
}
void InfoMineralRefineUpgrade::setCostIncrease(double value)
{
    _costIncrease = value;
}

