//
//  MafRewardAdsManager.hpp
//  FantasyClicker-mobile
//
//  Created by maf on 16/09/2019.
//

#ifndef __FantasyClicker__AdsManager__
#define __FantasyClicker__AdsManager__

#include "Libs/maf/MafSingleton.h"

class MafRewardAdsManager : public MafSingleton<MafRewardAdsManager>
{
public:
    MafRewardAdsManager();
    virtual ~MafRewardAdsManager(void);
    virtual bool init();
    
public:
    enum E_ADS_TYPE : int
    {
        E_ADMOB = 1,
        E_APPLOVIN,
        E_FACEBOOK,
    };
    
//    void initAds();
//    void startAds(std::function<void(ADS_RESULT)> cb);
//    void setAdsOrder(int adsType, ...);
//    void clearAdsOrder();
    void setHistoryData(const char* strAdsType, bool bAdsFree = false);
    
private:
//    int m_curAdsIndex;
//    std::list<MafAds *> m_adsOrder;
//    std::function<void(ADS_RESULT)> _ResultCBRewardAds;
    
//    MafAds* getAdsClass(E_ADS_TYPE type);
};

#endif /* __FantasyClicker__AdsManager__ */
