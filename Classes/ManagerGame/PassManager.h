//
//  PassManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/16.
//

#ifndef PassManager_h
#define PassManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/ModelInfoDevil.h"

enum class E_PASS_TYPE
{
    PASS_NONE = 0,
    PASS_DUNGEON = 1,
    PASS_END
};

enum class E_PASS_REWARD_TYPE
{
    FREE = 0,
    PURCHASE = 1
};

class PassStep : public cocos2d::Ref // 단계 정보
{
public:
    static PassStep* create();
    
    PassStep( );
    virtual ~PassStep();
    virtual  bool init();
public:
    int getIdx();
    void setIdx(int nIdx);
    
    int getPoint();
    void setPoint(int nPoint);
    
    int getStepUpCost();
    void setStepUpCost(int nCost);
    
    //
    int getRewardFreeIdx();
    void setRewardFreeIdx(int nIdx);
    
    int getRewardFreeTypeParams();
    void setRewardFreeTypeParams(int nTypeParams);
    
    int getRewardFreeCount();
    void setRewardFreeCount(int nCount);
    
    bool isRewardFree();
    void setRewardFree(bool bReward);
    
    //
    int getRewardPurIdx();
    void setRewardPurIdx(int nIdx);
    
    int getRewardPurTypeParams();
    void setRewardPurTypeParams(int nTypeParams);
    
    int getRewardPurCount();
    void setRewardPurCount(int nCount);
    
    bool isRewardPur();
    void setRewardPur(bool bReward);
    
     
    
private:
    int _nIdx;
    int _nPoint;
    int _nStepUpCost;
    
    int _nRewardFreeIdx;
    int _nRewardFreeTypeParams;
    int _nRewardFreeCnt;
    bool _bRewardFree;
    
    int _nRewardPurIdx;
    int _nRewardPurTypeParams;
    int _nRewardPurCnt;
    bool _bRewardPur;
};

class PassInfo : public cocos2d::Ref // 패스 정보
{
public:
    static PassInfo* create();
    
    PassInfo( );
    virtual ~PassInfo();
    virtual  bool init();
    
public:
    E_PASS_TYPE getType();
    void setType(E_PASS_TYPE eType);
    
    int getSeason();
    void setSeason(int nSeason);
    
    int64_t getPurchaseTime();
    void setPurchaseTime(int64_t nTime);
    
    int64_t getResetTime();
    void setResetTime(int64_t nTime);
    
    int getPoint();
    void setPoint(int nPoint);
    
    bool isBuyPass();
    void setBuyPass(bool bBuyPass);
    
    int getBadgeFree();
    void setBadgeFree(int nBadge);
    
    int getBadgePur();
    void setBadgePur(int nBadge);
    
    cocos2d::Vector<PassStep*> getListStep();
    void setListStep(cocos2d::Vector<PassStep*> list);
    PassStep* getStep(int nIdx);
    
    int getLevel();
    bool isLevelFinish();
    
    int getExpNext();
    int getExp();
    
    bool isReward();
    
    
private:
    E_PASS_TYPE _eType;
    
    int _nSeason;
    int64_t _nPurchaseTime;
    int64_t _nResetTime;
    
    int _nPoint;
    bool _bBuyPass;
    
    int _nBadgeFree;
    int _nBadgePur;
    
    //
    cocos2d::Vector<PassStep*> _listStep;
};

class PassManager : public MafSingleton<PassManager>
{
public:
    PassManager();
    virtual ~PassManager(void);
    bool init();
    
public:
    PassInfo* getPassInfo(E_PASS_TYPE type);
    bool isBuyPass(E_PASS_TYPE type);
    
    //network
    void requestInfo(const std::function<void(bool)>& pCallback);
    void requestReward(int nSeason, int nIdx, E_PASS_REWARD_TYPE eRewardType, std::function<void(bool, bool, bool)> pCallback);
    void requestStepUp(int nSeason, int nIdx, const std::function<void(bool, int)>& pCallback);
    
protected:
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseReward(cocos2d::network::HttpResponse* response, std::string &data);
    void responseStepUp(cocos2d::network::HttpResponse* response, std::string &data);
    
private:
    //
    cocos2d::Vector<PassInfo*> _listPass;
    
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool, bool, bool)> _callbacReward;
    std::function<void(bool, int)> _callbackStepUp;
    
};

#endif /* PassManager_h */
