#include "ParameterManager.h"

#include "ManagerSystem/TableManager.h"

USING_NS_CC;

ParameterManager::ParameterManager(void)
{
    
}

ParameterManager::~ParameterManager(void)
{
    
}

bool ParameterManager::init()
{
    // load
    setLoad();

    return true;
}

#pragma mark - load
void ParameterManager::setLoad()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PARAMETER);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listParameter.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        std::string strKey = jsonValue["key"].GetString();
        
        elementValue element;
        element.valueInteger = jsonValue["value_integer"].GetInt();
        element.valueDouble = jsonValue["value_double"].GetDouble();
        element.valueString = jsonValue["value_string"].GetString();
        
        _listParameter[strKey] = element;
    }
}

#pragma mark - set, get, add
int ParameterManager::getParameterInteger(std::string strKey)
{
    int value = 0;
    
    if ( _listParameter.find(strKey) != _listParameter.end() )
    {
        auto element = _listParameter.find(strKey)->second;
        value = element.valueInteger;
    }
    
    return value;
}

double ParameterManager::getParameterDouble(std::string strKey)
{
    double value = 0;
    
    if ( _listParameter.find(strKey) != _listParameter.end() )
    {
        auto element = _listParameter.find(strKey)->second;
        value = element.valueDouble;
    }
    
    return value;
}

std::string ParameterManager::getParameterString(std::string strKey)
{
    std::string value = "";
    
    if ( _listParameter.find(strKey) != _listParameter.end() )
    {
        auto element = _listParameter.find(strKey)->second;
        value = element.valueString;
    }
    
    return value;
}
