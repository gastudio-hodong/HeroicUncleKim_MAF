//
//  SaveManager.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 6..
//
//

#ifndef SaveManager_hpp
#define SaveManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class SaveManager  : public MafSingleton<SaveManager>
{
public:
    SaveManager(void);
    virtual ~SaveManager(void);
    
public:
    void timerUpdate(float dt);
    
public:
    // cloud
    std::string saveCloudData();
    void loadCloudData();
    
    // load data
    static void loadAllData();
    static void loadAllDataDevil();
    
    // save data
    static void saveAllData();
    static void saveQuest(E_PLACE eplace);
    static void saveQuestTime(E_PLACE eplace);
    static void saveEquip(E_PLACE eplace);
    static void saveBuffTime(); //
    static void saveAdsTime();
    
    static void saveCostume();
    static void saveCostumeRentList();
    static void saveCostumeRentTime();
    
    static void saveMyhomeLevel();
    static void saveMyhomeWatchcount();
    static void saveMyhomeAdsTime();
    
    // history data
    static void setHistoryData(E_HISTORY_DATA eHistory, const std::string & value);
    static void setHistoryData(E_HISTORY_DATA eHistory, const int value);
    static std::string getHistoryData(E_HISTORY_DATA eHistory);
    static int getHistoryDataInt(E_HISTORY_DATA eHistory);
    static std::string getHistoryKey(E_HISTORY_DATA eHistory);
    static std::string getHistorySaveToServerData();
    static void setHistorySaveFromServerData(std::string data);

    /**
     *회원탈퇴를 위한 모든 데이터 리셋 함수
     *그 외의 용도로 사용금지.
     */
    void destroyAllUserData();
public:
    // set, get
    time_t getSaveTime();
    int getSaveFloorHigh();
    int getSaveFloorTotal();
    
    // network
    void requestSave(const std::function<void(bool)>& pCallback);
    void requestLoad(const std::function<void(bool, bool)>& pCallback);
    void requestLoadFinish();
    /**
     *회원탈퇴 API
     */
    void requestDestroyUserData(const std::function<void(int)>& pCallback);
private:
    // network
    void responseSave(cocos2d::network::HttpResponse* response,std::string &data);
    void responseLoad(cocos2d::network::HttpResponse* response,std::string &data);
    void responseDestroyUserData(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    std::string _strSaveData;
    time_t _nSaveTime;
    int _nSaveFloorHigh;
    int _nSaveFloorTotal;
    std::function<void(bool)> _callbackSave;
    std::function<void(bool, bool)> _callbackLoad;
    std::function<void(int)> _callbackDestroyUserData;
};


#endif /* SaveManager_hpp */
