//
//  InfoBadge.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/01.
//

#ifndef InfoBadge_h
#define InfoBadge_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoBadge : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoBadge);
    
    InfoBadge();
    virtual ~InfoBadge();
    virtual bool init();

public:
    int getIdx();
    void setIdx(int nIdx);
    
    int getIdxSub();
    void setIdxSub(int nIdxSub);
    
    std::string getName();
    void setNameKey(std::string strNameKey);
    
    std::string getDesc();
    void setDescKey(std::string strDescKey);
    
    std::string getPath();
    void setPath(std::string strPath);
    
    std::string getGroup();
    void setGroup(std::string strGroup);
    
    E_BADGE_BUFF getBuffType();
    void setBuffType(E_BADGE_BUFF value);
    
    float getBuffCount(const bool isOriginal = false);
    void setBuffCount(float value);
    std::string getBuffText();
    
private:
    int _nIdx;
    int _nIdxSub;
    
    std::string _strNameKey;
    std::string _strDescKey;
    std::string _strPath;
    std::string _strGroup;
    
    E_BADGE_BUFF _eBuff;
    float _nBuffCnt;
    
};

class InfoBadgeHave : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoBadgeHave);
    
    InfoBadgeHave();
    virtual ~InfoBadgeHave();
    virtual bool init();

public:
    int getIdx();
    void setIdx(int nIdx);
    
    int getIdxBadge();
    void setIdxBadge(int nIdx);
    
    int getIdxSeason();
    void setIdxSeason(int nIdx);
    
    int64_t getCreated();
    void setCreated(int64_t nTime);
    
    E_BADGE_BUFF getBuffType();
    float getBuffCount();
    
    
private:
    int _nIdx;
    int _nIdxBadge;
    int _nIdxSeason;
    
    int64_t _nCreated;
};

#endif /* InfoBadge_h */
