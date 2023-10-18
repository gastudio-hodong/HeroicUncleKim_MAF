//
//  IntroScene.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 31/05/2019.
//

#ifndef IntroScene_h
#define IntroScene_h

#include "GameScene/Base/BaseScene.h"

#include "Common/ConfigEnum.h"

class IntroScene : public BaseScene
{
public:
    CREATE_FUNC(IntroScene);
    
    IntroScene();
    virtual ~IntroScene();
    
    // override base
    virtual void onEnter() override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // schedule
    void timeSchedule(float dt);
    
    // info
    void infoRequest();
    void infoRequestOther();
    
    // start game
    void startGame();
    
    
    
    // ui
    void uiMessage();
    void uiUrlSelect();
    
    //
    void drawMessage(int nType);

    // check
    void checkMaintainance();
    void checkVersion();
    void checkReverseApk();
    
    // callback
    void callbackUserAgree();
    void callbackStory();
    void callbackOffline();
    void callbackOfflineReward();
    void callbackEvent(bool bResult);
    void callbackBan(bool bResult, std::string strDay, E_BAN_TYPE eType);
    void callbackDataLoad(bool bResult, bool bData);
    void callbackDataLoadFinish();
    void callbackTable(bool bResult);
    void callbackTableProgress(int nTotal, int nSuccess, int nProgress);
    
    
    
    // network
    void requestTime();
    void responseTime(cocos2d::network::HttpResponse* response,std::string &data);
    void requestModulation();
    void responseModulation(cocos2d::network::HttpResponse* response,std::string &data);
    void requestContents();
    void responseContents(cocos2d::network::HttpResponse* response,std::string &data);
    

    //
    int checkFinishJobTotal();
    int checkFinishJob();
    
private:
    bool _bStartGame;
    
    bool _bInitUserData;
    bool _bInitUrl;
    bool _bInitStory;
    bool _bInitAgree;
    bool _bInitTable;
    bool _bInitMaintainance;
    bool _bInitVersion;
    bool _bInitModulation;
    bool _bInitLoginSelect;
    bool _bInitLogin;
    bool _bInitBan;
    
    bool _bInitDataLoad;
    bool _bInitEvent;
    bool _bInitContents;
    bool _bInitOfflineAds;
    
    bool _bInitTime;
    
    bool _bInitAdsFree;
    bool _bInitCashBannerInfo;
    bool _bInitCashGrowthInfo;
    bool _bInitCashFirstBonus;
    bool _bInitCashRemaining;
    
    bool _bInitDailyMission;
    bool _bInitWeekMission;
    bool _bInitDungeonPass;
    bool _bInitBongSkin;
    bool _bInitBadge;
    bool _bInitUserCount;
    bool _bInitPetEquipInfo;
    bool _bInitPetEquipBox;
    bool _bInitExpeditionLoot;
    bool _bInitTimingPackage;
    
    bool _bInitEventBingo;
    bool _bInitNewRaid;
    bool _bInitWeaponSpirit;
    bool _bInitEventTradingMarket;
    bool _bInitChat;
    bool _bInitAdventure;
    bool _bInitAdventureRelic;
    bool _bInitAdventureCore;
    bool _bInitAdventureRelicCollect;
    bool _bInitFriend;
    bool _bInitEventReturnAttend;
    bool _bInitEventNewUserAttend;
    bool _bInitPrisonFrost;
    bool _bInitEventVote;
    bool _bInitEventLuckybag;
    bool _bInitEventTreasureHunt;
    bool _bInitEventBoardGame;
    bool _bInitFlashman;
    bool _bInitDefense;
    bool _bInitDefenseAMP;
    
    
    // table download
    int _nTableProgress;
    int _nTableDownloadTotal;
    int _nTableDownloadSuccess;
    
    // widget
    cocos2d::Layer* _layerContainerMessage;
    cocos2d::Layer* _layerContainerUrl;
    
    cocos2d::ui::EditBox* _editUrl;
};

#endif /* SceneIntro_h */
