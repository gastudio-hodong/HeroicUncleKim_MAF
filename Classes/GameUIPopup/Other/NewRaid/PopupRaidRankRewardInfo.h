//
//  PopupRaidRankRewardInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/12.
//

#ifndef PopupRaidRankRewardInfo_hpp
#define PopupRaidRankRewardInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoBadge.h"
#include "GameObject/InfoItems.h"

class RankInfo;
class InfoRaidTierInfo;
class CellRaidTierInfo;
class PopupRaidRankRewardInfo : public PopupBase
{
public:
    enum class E_RAID_TIER_RANK_TYPE
    {
        RANK = 0,
        RANK_RANGE,
        TIER,
        END
    };
public:
    
    static PopupRaidRankRewardInfo* create();
    
    PopupRaidRankRewardInfo(void);
    virtual ~PopupRaidRankRewardInfo(void);
    virtual bool init() override;
    
    void creatTierInfo();
    
    void initVar();
    void initUI();
    
    void initTopUI();
    void initBottomUI();
    
    void initTopRankUI();
    void initTierUI();
    
    void redrawTimer();
    
    
    void onClick(Ref* sender);
    std::vector<InfoRaidTierInfo*> getInfoListFromType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE type, PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE type2 = PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::END);
    
private:
    
    Layer* _lyTopRankUI;
    Layer* _lyTierUI;
    
    Label* _lbTimer;
    
    RankInfo* _rankInfo;
    
    float _timer;
    std::vector<InfoRaidTierInfo*> _vecInfo;
    std::vector<InfoRaidTierInfo*> _vecSeason0Info;
};

class InfoRaidTierInfo
{
public:
    InfoRaidTierInfo();
    ~InfoRaidTierInfo();
    
    PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE getType();
    int getIdx();
    int getSeasonIdx();
    int getCondition1();
    int getCondition2();
    InfoBadge* getInfoBadge();
    Vector<InfoItems*> getListReward();
    
    void setType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE value);
    void setIdx(int value);
    void setSeasonIdx(int value);
    void setCondition1(int value);
    void setCondition2(int value);
    void setInfoBadge(InfoBadge* value);
    void setListReward(Vector<InfoItems*> value);
    
private:
    PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE _eType;
    int _nSeasonIdx;
    int _nIdx;
    int _nCondition1;
    int _nCondition2;
    InfoBadge* _infoBadge;
    Vector<InfoItems*> _listReward;
};

#endif /* PopupRaidRankRewardInfo_hpp */
