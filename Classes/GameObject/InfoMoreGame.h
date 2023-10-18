//
//  InfoMoreGames.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/04/12.
//

#ifndef InfoMoreGames_h
#define InfoMoreGames_h

#include "Common/ConfigDefault.h"

class InfoMoreGame : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoMoreGame);
    
    InfoMoreGame();
    virtual ~InfoMoreGame();
    virtual bool init();
    
    int getIdx();
    std::string getName();
    std::string getCountry();
    std::string getCountryExcept();
    std::string getPlatform();
    std::string getIconFileName();
    std::string getAdsVideoPath();
    std::string getLink();
    
    void setIdx(int value);
    void setName(std::string value);
    void setCountry(std::string value);
    void setCountryExcept(std::string value);
    void setPlatform(std::string value);
    void setIconFileName(std::string value);
    void setAdsVideoPath(std::string value);
    void setLink(std::string value);
    
private:
    
    int _nIdx;
    
    std::string _strName;
    std::string _strCountry;
    std::string _strCountryExcept;
    std::string _strPlatform;
    std::string _strIconFileName;
    std::string _strAdsVideoPath;
    std::string _strLink;
};

#endif /* InfoMoreGames_h */
