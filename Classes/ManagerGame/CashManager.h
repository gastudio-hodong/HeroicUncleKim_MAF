//
//  UIComboBox.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef CashManager_h
#define CashManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoProduct.h"
#include "GameObject/InfoProductBanner.h"
#include "GameObject/InfoProductGrowth.h"
#include "GameObject/InfoItems.h"

class CashManager  : public MafSingleton<CashManager>
{
public:
    CashManager(void);
    virtual ~CashManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    void setLoadGrowth();
    void setProductPriceLoad();
    void setProductPrice(std::string strProductId, std::string strPrice, std::string strPriceCurrencyCode);
    
    // product info
    cocos2d::Vector<InfoProduct*> getProductGroup(int group);
    cocos2d::Vector<InfoProduct*> getProductType(E_PRODUCT eType);
    InfoProduct* getProduct(int nIdx);
    
    cocos2d::Vector<InfoItems*> getItemsConvert(std::string strItems);
    
    // history
    int getHistoryTotal();
    void setHistoryTotal(int nCount);
    void addHistoryTotal();
    
    std::string getHistoryProductCloud();
    void setHistoryProductCloud(std::string strData);
    int getHistoryProduct(int nIdx);
    void addHistoryProduct(int nIdx);
    
    std::string getHistoryAdsCloud();
    void setHistoryAdsCloud(std::string strData);
    int getHistoryAds(int nIdx);
    void addHistoryAds(int nIdx);
    
    
    // info : daily(연금)
    int getDailyDate(int nIdx);
    void setDailyDate(int nIdx, int nDate);
    bool isDailyReceive(int nIdx);
    bool isDailyReceiveAll();
    void setDailyReceive(int nIdx, bool isReceive);
    
    // info : adsfree(광고스킵)
    void setAdsfreeTime(int nTime);
    int getAdsfreeTime();
    bool isAdsfreeTime();
    
    // info : banner
    cocos2d::Vector<InfoProductBanner*> getBannerAll();
    int getBannerIdx();
    int getBannerTimeRemaining();
    
    int getBannerGrade();
    void setBannerGrade(int nGrade);
    
    
    // info : growth
    cocos2d::Vector<InfoProductGrowth*> getGrowthAll();
    cocos2d::Vector<InfoProductGrowth*> getGrowthGroup(std::string type);
    cocos2d::Vector<InfoProductGrowth*> getGrowthGroup(std::string type, int level);
    InfoProductGrowth* getGrowth(int idx, int idxCash);
    int getGrowthNowLevel(std::string type);
    int getGrowthMaxLevel(std::string type);
    int getGrowthToIdxCash(std::string type, int level);
    bool isGrowthReward(std::string type);
    bool isGrowthFinish(std::string type);
    
    
    // info : first bonus
    int getFirstBonusResult();
    
    // info : news
    std::vector<std::string> getNewsAll();
    std::vector<std::string> getNewsDownloadAll();
    
    // 남은 결제 횟수
    int getPurchaseRemaining(int nItemIdx);
    void setPurchaseRemaining(int nItemIdx, int nCount);
    
    // reddot
    bool isNewIcon(E_PLACE type);
    void setNewIcon(bool bNew, E_PLACE type = E_PLACE::EMPTY);
    
    
    // delegate
    void subDelegateInfo(MafFunction<void(bool)>* func);
    void invokeDelegateInfo(bool bResult);
    void unSubDelegateInfo(void* obj);
    
    void subDelegateDailyInfo(MafFunction<void(bool)>* func);
    void invokeDelegateDailyInfo(bool bResult);
    void unSubDelegateDailyInfo(void* obj);
    
    void subDelegatePurchaseRemainingAll(MafFunction<void(bool)>* func);
    void invokeDelegatePurchaseRemainingAll(bool bResult);
    void unSubDelegatePurchaseRemainingAll(void* obj);
    
    // download
    void downloadNews(const std::function<void(int)>& callbackStart, const std::function<void(double)>& callbackProgress, const std::function<void(int, int)>& callbackCount, const std::function<void(void)>& callbackEnd);
    void downloadStart();
    void downloadEnd();
    void downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected);
    void downloadFileSuccess(const cocos2d::network::DownloadTask& task);
    void downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
    
    // purchase
    void purchaseInitiate(int nIdx, const std::function<void(MafGooglePlay::RESULT, int, int, std::string)>& pCallback);
    void purchaseResult(MafGooglePlay::RESULT eResult, std::string strData, std::string strSignature);
    void purchaseHas(int nIdx, const std::function<void(int)>& callback);
    void purchaseConsumeAll();
    
    // network : default
    void requestInfo(const std::function<void(bool)>& pCallback);
    void requestPurchaseRemainingAll(const std::function<void(bool)>& pCallback);
    void requestPurchaseRemaining(int nItemIdx, const std::function<void(bool, int)>& pCallback);
    
    // network : banner
    void requestBannerInfo(const std::function<void(bool)>& pCallback);
    void requestBannerBonusReceive(const std::function<void(bool)>& pCallback);
    
    // network : daily
    void requestDailyInfo(const std::function<void(bool)>& pCallback);
    void requestDailyReceive(int nIdx, const std::function<void(bool, int, std::string)>& pCallback);
    
    // network : growth
    void requestGrowthInfo(int nItemIdx, const std::function<void(bool, bool)>& pCallback);
    void requestGrowthReceive(int nItemIdx, int nIdx, const std::function<void(bool)>& pCallback);
    
    // network : first
    void requestFirstBonusInfo(const std::function<void(bool)>& pCallback);
    void requestFirstBonusReceive(const std::function<void(bool, std::string)>& pCallback);
    
    // network : news
    void requestNewsInfo(const std::function<void(bool)>& pCallback);
    
    void requestPurchase();
private:
    // network : default
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responsePurchaseRemainingAll(cocos2d::network::HttpResponse* response,std::string &data);
    void responsePurchaseRemaining(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : banner
    void responseBannerInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBannerBonusReceive(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : daily
    void responseDailyInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseDailyReceive(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : growth
    void responseGrowthInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseGrowthReceive(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : first
    void responseFirstBonusInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseFirstBonusReceive(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : news
    void responseNewsInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void responsePurchase(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    //
    std::unordered_map<int, int> _listDailyDate;
    std::unordered_map<int, bool> _listDailyReceive;
    std::unordered_map<int, int> _listPurchaseRemaining;
    
    //
    bool _isNewIconNormal;
    bool _isNewIconDevil;
    
    //
    cocos2d::Vector<InfoProduct*> _listProduct;
    InfoProduct *_objProduct;
    std::string _strPurchaseSignature;
    std::string _strPurchaseData;
    
    //
    cocos2d::Vector<InfoProductBanner*> _listBanner;
    int _nBannerEventIdx;
    int _nBannerEventGrade;
    int64_t _nBannerEventTime;
    
    //
    cocos2d::Vector<InfoProductGrowth*> _listGrowth;
    
    //
    int _nFirstBonusResult;
    int _nFirstBonusVersion;
    
    //
    int _adsFreeEndtime;
    
    //
    std::unique_ptr<cocos2d::network::Downloader> _downloader;
    std::vector<std::string> _listDownload;
    int _downloadTotal;
    int _downloadSuccess;
    int _downloadError;
    std::string _downloadStoragePathFull;
    std::string _downloadStoragePath;
    
    std::vector<std::string> _listNews;
    std::vector<std::string> _listNewsDownload;
    
    
    //
    std::function<void(MafGooglePlay::RESULT, int, int, std::string)> _callbackPurchase;
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool)> _callbackPurchaseRemainingAll;
    std::function<void(bool, int)> _callbackPurchaseRemaining;
    std::function<void(bool)> _callbackDailyInfo;
    std::function<void(bool)> _callbackBannerInfo;
    std::function<void(bool, bool)> _callbackGrowthInfo;
    std::function<void(bool)> _callbackFirstBonusInfo;
    std::function<void(bool)> _callbackBannerBonusReceive;
    std::function<void(bool)> _callbackGrowthReceive;
    std::function<void(bool, int, std::string)> _callbackDailyReceive;
    std::function<void(bool, std::string)> _callbackFirstBonusReceive;
    std::function<void(bool)> _callbackNewsInfo;
    
    //
    MafDelegate<void(bool)> _delegateInfo;
    MafDelegate<void(bool)> _delegateDailyInfo;
    MafDelegate<void(bool)> _delegatePurchaseRemainingAll;
    
    //
    std::function<void(int)> _callbackDownloadStart;
    std::function<void(double)> _callbackDownloadProgress;
    std::function<void(int, int)> _callbackDownloadCount;
    std::function<void(void)> _callbackDownloadEnd;
};

#endif /* ManagerAccount_hpp */
