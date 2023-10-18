//
//  InfoChat.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#ifndef InfoChat_h
#define InfoChat_h

#include "Common/ConfigDefault.h"

class InfoChat : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoChat);
    
    InfoChat();
    virtual ~InfoChat();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int value);
    
    int getUserIdx();
    void setUserIdx(int value);
    
    std::string getUserId();
    void setUserId(std::string value);
    
    std::string getUserNick();
    void setUserNick(std::string value);
    
    std::string getUserPlatform();
    void setUserPlatform(std::string value);
    
    std::string getMessage();
    void setMessage(std::string value);
    
    int getRank();
    void setRank(int value);
     
    int getSkin();
    void setSkin(int value);
    
    int getCostume();
    void setCostume(std::string value);
    
private:
    int _nIdx;
    
    int _nUserIdx;
    std::string _strUserID;
    std::string _strUserNick;
    std::string _strUserPlatform;
    std::string _strMessage;
    
    int _nRank;
    
    int _nSkin;
    std::string _strCostume;
};

#endif /* InfoChat_hpp */
