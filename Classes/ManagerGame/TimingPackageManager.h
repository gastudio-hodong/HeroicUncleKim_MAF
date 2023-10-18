//
//  TimingPackageManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/29.
//

#ifndef TimingPackageManager_h
#define TimingPackageManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoTimingPackage.h"

class TimingPackageManager : public MafSingleton<TimingPackageManager>
{
public:
    TimingPackageManager();
    virtual ~TimingPackageManager(void);
    friend class MafSingleton;
    
    bool init();
    void setLoad();
    
    void saveTimingPackageFreeData();
    void loadTimingPackageFreeData();
    
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackageAll();
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackage(E_TIMING_PACKAGE_CONDITION_TYPE eType);
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackage(E_TIMING_PACKAGE_TYPE eType);
    
    InfoTimingPackage* getInfoTimingPackage(E_TIMING_PACKAGE_CONDITION_TYPE eType, int conditionValue, bool isEndValue = false);
    InfoTimingPackage* getInfoTimingPackage(int nIdx);
    
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackageStartAll();
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackageStart(E_TIMING_PACKAGE_CONDITION_TYPE eType);
    
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackageEnd(E_TIMING_PACKAGE_CONDITION_TYPE eType);
    
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackageRewardAll();
    cocos2d::Vector<InfoTimingPackage*> getListInfoTimingPackageReward(E_TIMING_PACKAGE_CONDITION_TYPE eType);
    
    //offline
    cocos2d::Vector<InfoTimingPackage*> getOfflineRewardPackageInfo();
    void checkOfflineRewardInfo(int floor);
    void clearOfflineRewardList();
    
    cocos2d::Vector<InfoTimingPackage*> getOfflineStartPackageInfo();
    void checkOfflineStartInfo(int floor);
    void clearOfflineStartList();
    
    //utilss
    bool isStartTiming(InfoTimingPackage* info);
    
    bool isStartTiming(InfoTimingPackage* info, int nValue);
    
    bool isEndTiming(InfoTimingPackage* info);
    
    bool isEndTiming(InfoTimingPackage* info, int nValue);
    
    bool isRewardTiming(InfoTimingPackage* info);
    
    bool isRewardTiming(InfoTimingPackage* info, int nValue);
    
    void requestReward(InfoTimingPackage* info, const std::function<void(bool, int)>& pCallback = nullptr, bool isPopupScaedule = true);
    
    void setArrangementInfo();
    
    void showTimingPackagePopup(InfoTimingPackage* info) const;
    
    //network
    void requestTimingPackageInfo(const std::function<void(bool)>& pCallback);
    void responseTimingPackageInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestTimingPackageActivate(const std::function<void(bool, int)>& pCallback, InfoTimingPackage* info);
    void responseTimingPackageActivate(cocos2d::network::HttpResponse* response,std::string &data, const std::function<void(bool, int)>& pCallback, InfoTimingPackage* info);
    
    void requestTimingPackageReward(const std::function<void(bool, int)>& pCallback, InfoTimingPackage* info, bool isPopupScaedule = true);
    void responseTimingPackageReward(cocos2d::network::HttpResponse* response,std::string &data, InfoTimingPackage* info, bool isPopupScaedule);
private:
    cocos2d::Vector<InfoTimingPackage*> _listTimingPackgeInfo;
    cocos2d::Vector<InfoTimingPackage*> _listOfflineRewardPackageInfo;
    cocos2d::Vector<InfoTimingPackage*> _listOfflineStartPackageInfo;
    
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool, int)> _callbackReward;
    
    
public://delegate
    void subEventTimingPackage(MafFunction<void(cocos2d::ValueMap )>* func);
    void invokeEventTimingPackage(const cocos2d::ValueMap listData = cocos2d::ValueMap());
    void unSubEventTimingPackage(void* obj);
    
private:
     MafDelegate<void(cocos2d::ValueMap)> _eventTimingPackage;
    
};
#endif /* TimingPackageManager_hpp */
