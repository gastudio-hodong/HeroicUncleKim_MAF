//
//  RankingManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/13.
//

#ifndef RankingManager_h
#define RankingManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#pragma mark -Info
class RankInfo
{
public:
    //Top Rank
    RankInfo();
    
    RankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, std::string nick, std::string platform, std::string userIdx, int skin, std::string costume, std::string weapon, int tier = 0);
    
    //User Rank
    RankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, std::string nick, std::string platform, std::string userIdx, int tier = 0);
    
    //My Rank
    RankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, int tier = 0);
    
    ~RankInfo();
    
    //get set
    int getRank();
    int getPercentage();
    double getScore();
    int getTier();
    std::string getNick();
    std::string getPlatform();
    std::string getUserIdx();
    E_RANK_TYPE getRankType();
    E_RANK_LOCAL_TYPE getLocalRankType();
    E_RANK_FLUCTUATION getFluctuation();
    std::map<E_COSTUME, E_COSTUME_IDX> getCostume();
    int getWeaponIdx();
    int getSkinIdx();
    
    void setRank(int rank);
    void setPercentage(int value);
    void setScore(double score);
    void setTier(int tier);
    void setName(std::string value);
    void setPlatform(std::string value);
    void setUserIdx(std::string value);
    void setRankType(E_RANK_TYPE value);
    void setLocalRankType(E_RANK_LOCAL_TYPE value);
    void setRankType(E_RANK_FLUCTUATION value);
    void setCostume(std::string value);
    void setWeaponIdx(std::string value);
    void setSkinIdx(int value);
    
private:
    int _rank;
    double _score;
    int _percentage;
    
    int _tier;
    std::string _nick;
    std::string _platform;
    std::string _userIdx;
    std::map<E_COSTUME, E_COSTUME_IDX> _mapCostume;
    int _weaponIdx;
    int _skinIdx;
    E_RANK_TYPE _eType;
    E_RANK_LOCAL_TYPE _eLocalType;
    E_RANK_FLUCTUATION _eFluctuation;
    
};

#pragma mark -manager

class RankingManager : public MafSingleton<RankingManager>
{
public:
    RankingManager();
    virtual ~RankingManager(void);
    bool init();
    void update(float dt, E_RANK_LOCAL_TYPE type = E_RANK_LOCAL_TYPE::HIGH_FLOOR);
    
    RankInfo* getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE type);
    
    
    //network
    void requestHighFloorRankUpdate(const std::function<void(bool)>& pCallback, bool bShowLoading = false);
    void responsHighFloorRankUpdate(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestHighFloorUserRankInfo(const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback);
    void responstHighFloorUserRankInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestRaidUserRankInfo(int nTier, int nPage, const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback);
    void responstRaidUserRankInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestEventRaceRankInfo(int nPage, const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback);
    void responstEventRaceRankInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestFlashmanRankInfo(int nPage, const std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)>& pCallback);
    void responstFlashmanRankInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    //utils
    void renewalMyRankInfo(E_RANK_LOCAL_TYPE localType, E_RANK_TYPE type, int rank, double score, int tier = 0, int percentage = 0);
    
    void setCallbackReDraw(const std::function<void(bool)>& pCallback);
    void setRankUpdateTimer(float value);
    int getNowTier();
    int getNowPage();
    int getMaxPage();
    bool isNextPage();
    bool isPrevPage();
    void sceneChange();
private:

    std::vector<RankInfo*> _vecMyRankInfo;
    
    float _rankUpdateTimer;
    float _rankUpdateTimerMax;
    int _nNowViewTier;
    bool _bPrev;
    bool _bNext;
    
    int _nCurrentPage;
    int _nMaxPage;
    
    std::function<void(bool)> _callbackHighFloorRank;
    
    std::function<void(bool)> _callbackReDraw;
    
    std::function<void(bool, std::vector<RankInfo*>, std::vector<RankInfo*>)> _callbackUserRankInfo;
    
};

#endif /* RankingManager_h */
