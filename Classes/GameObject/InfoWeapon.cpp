//
//  InfoWeapon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoWeapon.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

#pragma mark - InfoWeapon
InfoWeapon::InfoWeapon() :
_idx(0),
_nameKey(""),
_descKey(""),

_legend(0),

_enhanceMax(0)
{
    
}

InfoWeapon::~InfoWeapon()
{
    
}

bool InfoWeapon::init()
{
    return true;
}

int InfoWeapon::getIdx()
{
    return _idx;
}

void InfoWeapon::setIdx(int idx)
{
    _idx = idx;
}

std::string InfoWeapon::getName()
{
    return GAME_TEXT(_nameKey);
}
void InfoWeapon::setNameKey(std::string key)
{
    _nameKey = key;
}

std::string InfoWeapon::getComment()
{
    return GAME_TEXT(_descKey);
}
void InfoWeapon::setCommentKey(std::string key)
{
    _descKey = key;
}

int InfoWeapon::getLegend()
{
    return _legend;
}

void InfoWeapon::setLegend(int legend)
{
    _legend = legend;
}
  
cocos2d::Vector<InfoItems*> InfoWeapon::getCost()
{
    return _listCost;
}

void InfoWeapon::setCost(std::string value)
{
    _listCost.clear();
    
    //
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)(const char*)value.c_str()).HasParseError();
    if( value.length() != 0 && jsonParser.HasParseError() == false )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(jsonValue["idx"].GetInt());
            if ( objItem != nullptr )
            {
                objItem->setCount(jsonValue["count"].GetInt());
                _listCost.pushBack(objItem);
            }
        }
    }
}



cocos2d::Vector<InfoItems*> InfoWeapon::getEnhanceCost()
{
    return _listEnhanceCost;
}
void InfoWeapon::setEnhanceCost(std::string value)
{
    _listEnhanceCost.clear();
    
    //
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)(const char*)value.c_str()).HasParseError();
    if( value.length() != 0 && jsonParser.HasParseError() == false )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(jsonValue["idx"].GetInt());
            if ( objItem != nullptr )
            {
                objItem->setCount(jsonValue["count"].GetInt());
                _listEnhanceCost.pushBack(objItem);
            }
        }
    }
}

int InfoWeapon::getEnhanceMax()
{
    return _enhanceMax;
}

void InfoWeapon::setEnhanceMax(int enhanceMax)
{
    _enhanceMax = enhanceMax;
}
