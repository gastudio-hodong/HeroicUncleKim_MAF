//
//  InfoRaidBoxrate.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#include "InfoRaidBoxrate.h"

USING_NS_CC;

InfoRaidBoxrate::InfoRaidBoxrate():
_nIdx(0),
_nCount(0)
{
    listRate.clear();
}

InfoRaidBoxrate::~InfoRaidBoxrate()
{
    listRate.clear();
}

bool InfoRaidBoxrate::init()
{
    return true;
}
 
int InfoRaidBoxrate::getIdx()
{
    return _nIdx;
}
void InfoRaidBoxrate::setIdx(int value)
{
    _nIdx = value;
}
int InfoRaidBoxrate::getCount()
{
    return _nCount;
}
void InfoRaidBoxrate::setCount(int value)
{
    _nCount = value;
}

std::vector<InfoRaidBoxrate::InfoRate> InfoRaidBoxrate::getListRate()
{
    return listRate;
}
void InfoRaidBoxrate::setListRate(std::string value)
{
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)value.c_str()).HasParseError();
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    listRate.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        int nIdx = jsonValue["idx"].GetInt();
        float fRate = jsonValue["rate"].GetFloat();
        
        listRate.push_back(InfoRaidBoxrate::InfoRate(nIdx, fRate));
    }
    
}
