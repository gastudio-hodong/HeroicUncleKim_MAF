//
//  AdsInterstitialErrorManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/03/31.
//

#ifndef AdsErrorManager_h
#define AdsErrorManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigEnum.h"

#include "GameUI/UI/UIAdsCooldown.h"

class AdsUtilsManager : public MafSingleton<AdsUtilsManager>
{
public:
    AdsUtilsManager(void);
    virtual ~AdsUtilsManager(void);
    virtual bool init();
    
    //광고 쿨타임
    int addAdsCooldown();
    int getAdsCooldown();
    int getAdsCooldownMax();
    void setAdsCooldown(int nValue = -1, bool isAuto = true);
    void adsCooldownHide();
    
    //보상형 전면배너 광고 인터넷 오류 체크
    bool isErrorAdsInterstitial(E_ADS_INTERSTITIAL_TYPE eType);
    int getAdsInterstitialErrorType(E_ADS_INTERSTITIAL_TYPE eType);
    void addAdsInterstitialErrorType(int nType, int64_t nTime);
    void removeAdsInterstitialErrorType(int nType);
    std::map<int, int64_t> getListAdsInterstitialError();
    void saveListAdsInterstitialError();
    void loadListAdsInterstitialError();
    std::string getAdsInterstitialErrorData();
    void checkAdsInterstitialErrorTime();
    
private:
    //보상형 전면배너 광고 인터넷 오류 리스트
    std::map<int, int64_t> _listAdsInterstitialError;
    
    
    int _nTimer;
    cocos2d::Node* _uiAdsCooldown;
};
    

#endif /* AdsInterstitialErrorManager_hpp */
