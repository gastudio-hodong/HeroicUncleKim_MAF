//
//  InfoFriend.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoFriend_h
#define InfoFriend_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoFriend : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoFriend);
    
    InfoFriend();
    virtual ~InfoFriend();
    virtual bool init();
    
public:
    
    void setUseridx(int idx);
    int getUseridx();
    
    void setCostume(std::vector<int> list);
    std::vector<int> getCostume();

    void setPlatform(std::string platform);
    std::string getPlatform();
    
    void setNickname(std::string nickname);
    std::string getNickname();
    
    void setPointReceiveAvailable(bool point);
    bool getPointReceiveAvailable();
    
    void setPointSendAvailable(bool point);
    bool getPointSendAvailable();
    
    void setDibson(bool dib);
    bool getDibson();
    
    void setUserPet(E_PET pet);
    E_PET getUserPet();
    
    void setUserPetEvolution(int nEvolution);
    int getUserPetEvolution();
    
    void setLoginTime(int64_t nTime);
    int getLoginDay();
    
private:
    
    int _nUseridx;
    std::vector<int> _vecCostume;
    
    std::string _strPlatform;
    std::string _strNick;
    
    bool _bReceive;
    bool _bSend;
    
    E_PET _nPet;
    int _nPetEvolution;
    int64_t _nLoginTime;
};
#endif /* InfoFriend_h */
