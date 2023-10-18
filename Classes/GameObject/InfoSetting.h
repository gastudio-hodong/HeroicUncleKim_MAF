//
//  InfoRaidSetting.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/05.
//

#ifndef InfoRaidSetting_hpp
#define InfoRaidSetting_hpp

#include "Common/ConfigDefault.h"

class InfoSetting : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoSetting);
    
    InfoSetting();
    virtual ~InfoSetting();
    virtual bool init();
    
    
public:
    
    std::string getKey();
    void setKey(std::string value);
    
    std::string getType();
    void setType(std::string value);
    
    std::string getValueToString();
    int getValueToInt();
    double getValueToFloat();
    bool getValueToBool();
    void setValue(std::string value);
    
private:
    std::string _strKey;
    std::string _strType;
    std::string _strValue;
    
};

#endif /* InfoRaidSetting_hpp */
