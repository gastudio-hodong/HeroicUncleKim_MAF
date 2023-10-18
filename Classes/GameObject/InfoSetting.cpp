//
//  InfoRaidSetting.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/05.
//

#include "InfoSetting.h"

USING_NS_CC;

InfoSetting::InfoSetting():
_strKey(""),
_strType(""),
_strValue("")
{
    
}

InfoSetting::~InfoSetting()
{
    
}

bool InfoSetting::init()
{
    return true;
}
 
std::string InfoSetting::getKey()
{
    return _strKey;
}
void InfoSetting::setKey(std::string value)
{
    _strKey = value;
}

std::string InfoSetting::getType()
{
    return _strType;
}
void InfoSetting::setType(std::string value)
{
    _strType = value;
}

std::string InfoSetting::getValueToString()
{
    return _strValue;
}
int InfoSetting::getValueToInt()
{
    return std::atoi(_strValue.c_str());
}
double InfoSetting::getValueToFloat()
{
    return std::atof(_strValue.c_str());
}
bool InfoSetting::getValueToBool()
{
    std::string value = _strValue;
    
    return strcasecmp("true", value.c_str()) == 0;
}
void InfoSetting::setValue(std::string value)
{
    _strValue = value;
}
