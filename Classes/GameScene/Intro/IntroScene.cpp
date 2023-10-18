//
//  IntroScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 31/05/2019.
//

#include "IntroScene.h"


#include "GameObject/Event/InfoActivatedEvent.h"

#include "GameUIPopup/Other/User/PopupUserAgree.h"
#include "GameUIPopup/Other/Offline/PopupOffline.h"
#include "GameUIPopup/Other/Settings/SaveLoad/PopupCloudLoad.h"
#include "GameUIPopup/Other/Settings/PopupNotic.h"
#include "GameUIPopup/Other/User/PopupUserLogin.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/Event7thManager.h"
#include "ManagerEvent/EventBingoManager.h"
#include "ManagerEvent/EventBoardGameManager.h"
#include "ManagerEvent/EventDrawLimitManager.h"
#include "ManagerEvent/EventFishingManager.h"
#include "ManagerEvent/EventLuckyBagManager.h"
#include "ManagerEvent/EventNewUserAttendManager.h"
#include "ManagerEvent/EventRaceManager.h"
#include "ManagerEvent/EventReturnAttendManager.h"
#include "ManagerEvent/EventSunfishManager.h"
#include "ManagerEvent/EventTradingMarketManager.h"
#include "ManagerEvent/EventTreasureHuntManager.h"
#include "ManagerEvent/EventVoteManager.h"
#include "ManagerEvent/EventVoteManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AttackEffectManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/ChatManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/ExpeditionManager.h"
#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/FriendManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/LootsManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/MonsterManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/OfflineRewardManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/PetEquipBoxManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/QuestManager.h"
#include "ManagerGame/RankingManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/ShopItemManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/StoneManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/TimeTravelManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/TradingMarketManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/WeaponSpiritManager.h"

//
#include <thread>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    #include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif

USING_NS_CC;

IntroScene::IntroScene() :
_bStartGame(false),

_bInitUserData(false),
_bInitUrl(false),
_bInitStory(false),
_bInitAgree(false),
_bInitTable(false),
_bInitMaintainance(false),
_bInitVersion(false),
_bInitModulation(false),
_bInitLoginSelect(false),
_bInitLogin(false),
_bInitBan(false),

_bInitDataLoad(false),
_bInitEvent(false),
_bInitContents(false),
_bInitOfflineAds(false),

_bInitTime(false),

_bInitAdsFree(false),
_bInitCashBannerInfo(false),
_bInitCashGrowthInfo(false),
_bInitCashFirstBonus(false),
_bInitCashRemaining(false),
_bInitDailyMission(false),
_bInitWeekMission(false),
_bInitDungeonPass(false),
_bInitBongSkin(false),
_bInitBadge(false),
_bInitUserCount(false),
_bInitPetEquipInfo(false),
_bInitPetEquipBox(false),
_bInitExpeditionLoot(false),
_bInitTimingPackage(false),
_bInitEventBingo(false),
_bInitNewRaid(false),
_bInitWeaponSpirit(false),
_bInitEventTradingMarket(false),
_bInitChat(false),
_bInitAdventure(false),
_bInitAdventureRelic(false),
_bInitAdventureRelicCollect(false),
_bInitAdventureCore(false),
_bInitFriend(false),
_bInitEventReturnAttend(false),
_bInitEventNewUserAttend(false),
_bInitPrisonFrost(false),
_bInitEventVote(false),
_bInitEventLuckybag(false),
_bInitEventTreasureHunt(false),
_bInitEventBoardGame(false),
_bInitFlashman(false),
_bInitDefense(false),
_bInitDefenseAMP(false),

_nTableProgress(0),
_nTableDownloadTotal(0),
_nTableDownloadSuccess(0),

_layerContainerMessage(nullptr),
_layerContainerUrl(nullptr),
_editUrl(nullptr)
{
    
}

IntroScene::~IntroScene()
{
    
}

#pragma mark - override base
void IntroScene::onEnter()
{
    BaseScene::onEnter();
    
    //
    initVar();
    initUi();
    
    // lib IDFA
    MafNative::GetAdsID();
    
    //
    infoRequest();
    
    // scheduler
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->schedule(schedule_selector(IntroScene::timeSchedule), this, 1.0f , kRepeatForever, 1.0f, false);
}
 
#pragma mark - schedule
void IntroScene::timeSchedule(float dt)
{
    //
    drawMessage(1);
    
    //
    if ( checkFinishJobTotal() != checkFinishJob() )
    {
        return;
    }
    
    if ( _bStartGame )
    {
        return;
    }
    _bStartGame = true;
    
    // 스케줄 종료
    getScheduler()->unschedule(schedule_selector(IntroScene::timeSchedule), this);
    
    // 오프라인 계산 : 다른거 처리후 마지막 작업
    if( UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::OFFLINE) == true )
    {
        //
        drawMessage(2);
        
        //
        OfflineRewardManager::getInstance()->setCallbackCalculation(CC_CALLBACK_0(IntroScene::callbackOffline, this));
        auto t = std::thread(CC_CALLBACK_0(OfflineRewardManager::rewardCalculation, OfflineRewardManager::getInstance()));
        t.detach();
    }
    else
    {
        startGame();
    }
}

#pragma mark - init
void IntroScene::initVar()
{
    // sound init
    SoundManager::InitSoundConfig();
    SoundManager::SetAutoPreload(true);
    SoundManager::SoundPreload(SOUND_BGM);
    
    // network url
    MafHttpNew::_strRootUrl = DUNGEON_SERVER_URL_LIVE;
    
    // server time 삭제
    UtilsDate::getInstance()->setTimeServer(0);
    
    // manager destroy & create
    AccountManager::destroyInstance();
    AchievementManager::destroyInstance();
    AdventureManager::destroyInstance();
    AdventureRelicManager::destroyInstance();
    AttackEffectManager::destroyInstance();
    BadgeManager::destroyInstance();
    CashManager::destroyInstance();
    ChatManager::destroyInstance();
    CostumeManager::destroyInstance();
    CostumeSkinManager::destroyInstance();
    DataManager::destroyInstance();
    DialogManager::destroyInstance();
    ExpeditionManager::destroyInstance();
    FlashmanManager::destroyInstance();
    FriendManager::destroyInstance();
    InfiniteArtifactManager::destroyInstance();
    ItemsManager::destroyInstance();
    ItemsMoneyManager::destroyInstance();
    LootsManager::destroyInstance();
    MineralManager::destroyInstance();
    MonsterManager::destroyInstance();
    NewRaidManager::destroyInstance();
    OfflineRewardManager::destroyInstance();
    PassManager::destroyInstance();
    PetEquipBoxManager::destroyInstance();
    PetNewDevilManager::destroyInstance();
    PetNewManager::destroyInstance();
    PrisonFrostManager::destroyInstance();
    PrisonManager::destroyInstance();
    QuestManager::destroyInstance();
    RankingManager::destroyInstance();
    SaveManager::destroyInstance();
    ServerMissionManager::destroyInstance();
    ShopItemManager::destroyInstance();
    SpecialMissionManager::destroyInstance();
    StoneManager::destroyInstance();
    StoryManager::destroyInstance();
    TimeTravelManager::destroyInstance();
    TimingPackageManager::destroyInstance();
    TownManager::destroyInstance();
    TradingMarketManager::destroyInstance();
    TutorialManager::destroyInstance();
    UserInfoConfigManager::destroyInstance();
    UserInfoManager::destroyInstance();
    UserInfoStackManager::destroyInstance();
    WeaponManager::destroyInstance();
    WeaponSpiritManager::destroyInstance();
    DefenseManager::destroyInstance();
    
    EventManager::destroyInstance();
    Event7thManager::destroyInstance();
    EventBingoManager::destroyInstance();
    EventBoardGameManager::destroyInstance();
    EventDrawLimitManager::destroyInstance();
    EventFishingManager::destroyInstance();
    EventLuckyBagManager::destroyInstance();
    EventNewUserAttendManager::destroyInstance();
    EventSunfishManager::destroyInstance();
    EventRaceManager::destroyInstance();
    EventReturnAttendManager::destroyInstance();
    EventTradingMarketManager::destroyInstance();
    EventTreasureHuntManager::destroyInstance();
    EventVoteManager::destroyInstance();
    
    
    // text init
    // manager system
    TableManager::getInstance();
    SaveManager::getInstance();
    BadgeManager::getInstance();
    CostumeManager::getInstance();
    CostumeSkinManager::getInstance();
    InfiniteArtifactManager::getInstance();
    ParameterManager::getInstance();
    LootsManager::getInstance();
    WeaponSpiritManager::getInstance();
    NewRaidManager::getInstance();
    DefenseManager::getInstance();
    
    //
    TableManager::getInstance()->setLoad();
    
    TextManager::getInstance();
    TextManager::getInstance()->textInit();
    TextManager::getInstance()->dialogTextInit();
    TextManager::getInstance()->talkInit();
    
    DataManager::getInstance();
    AttackEffectManager::getInstance();
    
    // version 저장
    std::string strVersion = UserDefault::getInstance()->getStringForKey(KEY_VERSION, "");
    if ( strVersion.compare(MafNative::GetGameVersion()) != 0 )
    {
        UserDefault::getInstance()->setStringForKey(KEY_VERSION, MafNative::GetGameVersion());
        TableManager::getInstance()->setVersionNew(true);
    }
    CashManager::getInstance()->setNewIcon(true);
    
    //
    if ( AccountManager::getInstance()->getUserSocialID().empty() == true )
    {
        AccountManager::getInstance()->onSocialLogin();
    }   
}

void IntroScene::initUi()
{    
    // bg
    std::string strPathBG = "Assets/ui_intro/title.png";
    std::string strPathTitle = "Assets/ui_intro/title_logo_normal_%s.png";
    
    if(DataManager::getInstance()->isEventChristmas())
        strPathBG = "Assets/ui_intro/title_2020max.png";
    else if(DataManager::getInstance()->isNewYear())
        strPathBG = "Assets/ui_intro/title_new2022.png";
    
    auto spriteBg = Sprite::create(strPathBG);
    spriteBg->setPosition(getContentSize().width / 2 , getContentSize().height / 2);
    spriteBg->setScale(MAX(getContentSize().width / spriteBg->getContentSize().width, getContentSize().height / spriteBg->getContentSize().height));
    addChild(spriteBg);
    
    auto spriteBGTitle = Sprite::create(TextManager::getInstance()->getLangFileName(strPathTitle));
    spriteBGTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteBGTitle->setPosition(getContentSize().width / 2, 48);
    addChild(spriteBGTitle);
    
    //
    _layerContainerMessage = Layer::create();
    _layerContainerMessage->setContentSize( Size(getContentSize().width * 0.8, 48) );
    _layerContainerMessage->setIgnoreAnchorPointForPosition(false);
    _layerContainerMessage->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerMessage->setPosition(getContentSize().width / 2, 0);
    addChild(_layerContainerMessage);
    
    //
    uiMessage();
}

#pragma mark - ui
void IntroScene::uiMessage()
{
    _layerContainerMessage->removeAllChildren();
    
    //
    auto spriteMessageBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_3.png", Rect::ZERO, Rect(77,23,2,2));
    spriteMessageBG->setContentSize(Size(_layerContainerMessage->getContentSize().width, 148));
    spriteMessageBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteMessageBG->setPosition(_layerContainerMessage->getContentSize().width / 2, _layerContainerMessage->getContentSize().height);
    _layerContainerMessage->addChild(spriteMessageBG);
    
    auto labelMessage = Label::createWithTTF(GAME_TEXT("t_ui_intro_msg1"), GAME_FONT, 24);
    labelMessage->setPosition(_layerContainerMessage->getContentSize().width / 2, _layerContainerMessage->getContentSize().height / 2);
    labelMessage->setDimensions(_layerContainerMessage->getContentSize().width * 0.9, labelMessage->getContentSize().height);
    labelMessage->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMessage->setOverflow(Label::Overflow::SHRINK);
    labelMessage->setName("LABEL_TEXT");
    _layerContainerMessage->addChild(labelMessage);
}

void IntroScene::uiUrlSelect()
{
    if ( _layerContainerUrl == nullptr )
    {
        _layerContainerUrl = Layer::create();
        _layerContainerUrl->setContentSize( Size(getContentSize().width, getContentSize().height) );
        _layerContainerUrl->setIgnoreAnchorPointForPosition(false);
        _layerContainerUrl->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerUrl->setPosition(getContentSize().width / 2, getContentSize().height);
        addChild(_layerContainerUrl, 1000);
    }
    _layerContainerUrl->removeAllChildren();
    
    //
    std::string listUrl[] = {
        DUNGEON_SERVER_URL_LIVE,
        DUNGEON_SERVER_URL_DEV,
        DUNGEON_SERVER_URL_TEST1,
        DUNGEON_SERVER_URL_TEST2
    };
    
    std::string listUrlTitle[] = {
        "리얼",
        "개발",
        "테스트1",
        "테스트2"
    };
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerUrl->addChild(menu);
    for ( int i = 0; i < sizeof(listUrl)/sizeof(std::string); i++ )
    {
        double posX = _layerContainerUrl->getContentSize().width / 2;
        double posY = _layerContainerUrl->getContentSize().height - 200 - i * 100 - i * 10;
        
        auto spriteUrlBG = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteUrlBG->setContentSize(Size(500, 100));
        
        auto itemUrl = MafNode::MafMenuItemSprite::create(spriteUrlBG, nullptr, [=](Ref* sender){
            
            auto item = (MafNode::MafMenuItemSprite*)sender;
            
            // network url
            MafHttpNew::_strRootUrl = item->getName();
            
            //
            _layerContainerUrl->removeAllChildren();
            _layerContainerUrl = nullptr;
            
            // init
            _bInitUrl = true;
            infoRequest();
        });
        itemUrl->setName(listUrl[i]);
        itemUrl->setPosition(posX, posY);
        menu->addChild(itemUrl);
        
        auto labelUrl = Label::createWithTTF(listUrlTitle[i], GAME_FONT, 36);
        labelUrl->setPosition(itemUrl->getContentSize().width / 2, itemUrl->getContentSize().height / 2);
        labelUrl->enableBold();
        itemUrl->addChild(labelUrl);
    }
    
    //
    _editUrl = ui::EditBox::create(Size(500, 100), "Assets/ui/transfer/input.png");
    _editUrl->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _editUrl->setPosition(Vec2(_layerContainerUrl->getContentSize().width / 2, 200));
    _editUrl->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editUrl->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    _editUrl->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
    _editUrl->setFont(GAME_FONT, 28);
    _layerContainerUrl->addChild(_editUrl);
  
    auto spriteUrlInputBG = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteUrlInputBG->setContentSize(Size(500, 100));
    
    auto itemUrlInput = MafNode::MafMenuItemSprite::create(spriteUrlInputBG, nullptr, [=](Ref* sender){
        
        // network url
        MafHttpNew::_strRootUrl = _editUrl->getText();
        
        //
        _layerContainerUrl->removeAllChildren();
        _layerContainerUrl = nullptr;
        
        // init
        _bInitUrl = true;
        infoRequest();
    });
    itemUrlInput->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    itemUrlInput->setPosition(_layerContainerUrl->getContentSize().width / 2, 200);
    menu->addChild(itemUrlInput);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 28);
        label->setPosition(itemUrlInput->getContentSize().width / 2, itemUrlInput->getContentSize().height / 2);
        itemUrlInput->addChild(label);
    }
}

#pragma mark - draw
void IntroScene::drawMessage(int nType)
{
    std::string strMessage = "";

    if ( nType == 1 )
    {
        if ( _nTableDownloadTotal != 0 )
        {
            strMessage = MafUtils::format("Download(%d/%d) - %d%%", _nTableDownloadSuccess, _nTableDownloadTotal, _nTableProgress);
        }
        else
        {
            strMessage = GAME_TEXT("t_ui_intro_msg1");  // 서버 정보를 가져오는 중입니다
            
            const int countNow = checkFinishJob();
            const int countTotal = checkFinishJobTotal();
            
            std::string count = MafUtils::format(" (%d/%d)", countNow, countTotal);
            strMessage.append(count);
        }
    }
    else
    {
        strMessage = GAME_TEXT("t_ui_intro_msg3");  // 오프라인 보상을 처리 중입니다
        
        int nCount = UtilsDate::getInstance()->getTime() % 4;
        for ( int i = 0; i < nCount; i++ )
        {
            strMessage.append(".");
        }
    }
    
    auto labelMessage = (Label*)_layerContainerMessage->getChildByName("LABEL_TEXT");
    labelMessage->setString(strMessage);
}


#pragma mark - info
void IntroScene::infoRequest()
{
    if ( _bInitUserData == false )
    {
        SaveManager::loadAllData();
        
        _bInitUserData = true;
        infoRequest();
    }
    else if ( _bInitUrl == false )
    {
        #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        {
            uiUrlSelect();
        }
        #else
        {
            _bInitUrl = true;
            infoRequest();
        }
        #endif
    }
    else if ( _bInitTable == false )
    {
        _nTableProgress = 0;
        _nTableDownloadTotal = 0;
        _nTableDownloadSuccess = 0;
        TableManager::getInstance()->requestInfo(CC_CALLBACK_1(IntroScene::callbackTable, this), CC_CALLBACK_3(IntroScene::callbackTableProgress, this));
    }
    else if ( _bInitStory == false )
    {// 스토리 시작
        _bInitStory = StoryManager::getInstance()->isStoryOpen((int)E_COMIC::BEGINNING);
        if ( _bInitStory == true )
        {
            infoRequest();
        }
        else
        {
            StoryManager::getInstance()->showStory((int)E_COMIC::BEGINNING, CC_CALLBACK_0(IntroScene::callbackStory,this));
        }
    }
    else if ( _bInitAgree == false )
    {// 개인 정보 동의
        _bInitAgree = UserDefault::getInstance()->getBoolForKey(KEY_USER_AGREE, false);
        if ( _bInitAgree == true )
        {
            infoRequest();
        }
        else
        {
            // 한국은 약관동의 유지, 해외유저들은 전부 삭제 remoteconfig
            if (MafRemoteConfig::GetRemoteValueBool("user_agree_skip") == true && TextManager::getInstance()->getLang().compare("ko") != 0)
            {
                callbackUserAgree();
            }
            else
            {
                auto popup = PopupUserAgree::create();
                popup->setCallbackResult(CC_CALLBACK_0(IntroScene::callbackUserAgree, this));
                popup->setHideBackKey(false);
                popup->setHideAll(false);
                popup->show();
            }            
        }
    }
    else if ( _bInitMaintainance == false )
    {// 서버점검
        checkMaintainance();
    }
    else if ( _bInitVersion == false )
    {// 버전
        checkVersion();
    }
    else if ( _bInitLoginSelect == false )
    {
        if ( AccountManager::getInstance()->getUserID().empty() == true )
        {
            auto floorTotal = UserInfoManager::getInstance()->getTotalFloor();
            if ( floorTotal > 0 )
            {//게스트 로그인 없을때 이미 로그인 된 유저들 게스트 자동 가입
                AccountManager::getInstance()->requestGuestRegister(false, [=](bool bResult){
                    
                    if ( bResult == false )
                    {
                        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
                        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
                            infoRequest();
                        });
                        popup->show();
                        return;
                    }
                    
                    _bInitLoginSelect = true;
                    infoRequest();
                });
            }
            else
            {
                auto popup = PopupUserLogin::create();
                popup->setCallbackResult([=](){
                    _bInitLoginSelect = true;
                    infoRequest();
                });
                popup->show();
            }
        }
        else
        {
            _bInitLoginSelect = true;
            infoRequest();
        }
    }
    else if ( _bInitLogin == false )
    {
        AccountManager::getInstance()->onLogin(false, [=](bool bResult, int nResult){
            
            if ( bResult == false )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
                    infoRequest();
                });
                popup->show();
                return;
            }
            
            _bInitLogin = true;
            infoRequest();
        });
    }
    else if ( _bInitBan == false )
    {// 유저 벤 및 데이터 정보 가져오기
        AccountManager::getInstance()->requestBan(false, CC_CALLBACK_3(IntroScene::callbackBan, this));
    }
    else if ( _bInitEvent == false )
    {// 이벤트 정보 가져오기
        EventManager::getInstance()->requestInfo(CC_CALLBACK_1(IntroScene::callbackEvent, this));
    }
    else if ( _bInitModulation == false )
    {// apk 변조관련
        std::string strInstallStore = MafNative::GetDeviceInstallStore();
        UserDefault::getInstance()->setStringForKey(KEY_HACK_STORE, strInstallStore);
        
        // hask key
        std::string keyHash = MafNative::getKeyHash();
        if ( keyHash.length() != 0 && keyHash.compare("tJ8yuzxvSTu/tz+n/yeob3oj4RU=") != 0 )
        {
            UserDefault::getInstance()->setStringForKey(KEY_HACK_HASH, keyHash);
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _bInitModulation = true;
        infoRequest();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        _bInitModulation = true;
        infoRequest();
#else
        checkReverseApk();
#endif
#endif
    }
    else if ( _bInitDataLoad == false )
    {
        std::string strCloudVer = UserDefault::getInstance()->getStringForKey(KEY_CLOUD_VER, "");
        if ( strCloudVer.length() == 0 || AccountManager::getInstance()->isLoadData() == true )
        {
            SaveManager::getInstance()->requestLoad(CC_CALLBACK_2(IntroScene::callbackDataLoad, this));
        }
        else
        {
            _bInitDataLoad = true;
            infoRequest();
            return;
        }
    }
    else if ( _bInitTime == false )
    {
        requestTime();
    }
    else if ( _bInitContents == false )
    {// 컨텐츠 정보 가져오기
        requestContents();
        
        infoRequestOther();
    }
    else if ( _bInitOfflineAds == false )
    {// 오프라인 광고 시간 감소
        time_t nTime = 0;
        std::string offlineTimeDecrypt = UserDefault::getInstance()->getStringForKey(KEY_OFFLINE_TIME, "");
        if ( offlineTimeDecrypt.length() != 0 )
        {
            offlineTimeDecrypt = MafAes256::Decrypt(offlineTimeDecrypt);
            nTime = atol(offlineTimeDecrypt.c_str());
        }
        
        if ( nTime <= 0 )
        {
            _bInitOfflineAds = true;
            infoRequest();
            return;
        }
        
        time_t nTimeDiff = UtilsDate::getInstance()->getTime() - nTime;
        if ( nTimeDiff > 0 )
        {
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_GEM_ADS, (int)nTimeDiff * -1);
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_GOLD_ADS, (int)nTimeDiff * -1);
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_ADS, (int)nTimeDiff * -1);
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_MINE_ADS, (int)nTimeDiff * -1);
            
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_GEM_ADS, (int)nTimeDiff * -1);
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_GOLD_ADS, (int)nTimeDiff * -1);
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_BUFF_ADS, (int)nTimeDiff * -1);
            
            UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_SELECT_ADS, (int)nTimeDiff * -1);
            
            for ( int i = 1; i <= C_MAX_MYHOME; i++ )
            {
                UserInfoManager::getInstance()->setMyhomeAdsTime(i, (int)nTimeDiff * -1, false);
            }
            
            SaveManager::saveAdsTime();
            SaveManager::saveMyhomeAdsTime();
        }
        
        _bInitOfflineAds = true;
        infoRequest();
    }
}

void IntroScene::infoRequestOther()
{// 순서 및 중요하지 않은 네트워크 패킷은 바로 그리기 시작한다.
    
    // 푸쉬 정보 가져오기
    DataManager::getInstance()->requestPushInfo("");
    
    // mission
    ServerMissionManager::getInstance()->requestInfoMission([=](bool, int){
        _bInitDailyMission = true;
    }, E_MISSION_RESET_TYPE::DAILY, true);
    
    ServerMissionManager::getInstance()->requestInfoMission([=](bool, int){
        _bInitWeekMission = true;
    }, E_MISSION_RESET_TYPE::WEEK, true);
    
    PassManager::getInstance()->requestInfo([=](bool bResult){
        //pass
        _bInitDungeonPass = true;
    });
    
    CostumeSkinManager::getInstance()->requestSkinIsBuy([=](bool bResult){
        //BongSkin
        _bInitBongSkin = true;
    });
    
    
    BadgeManager::getInstance()->requestInfo([=](bool bResult){
        // 배지
        _bInitBadge = true;
    });
    
    AccountManager::getInstance()->requestReportCount(false, [=](bool bResult){
        _bInitUserCount = true;
    });
    
    PetNewManager::getInstance()->requestEquipInfo(false, [=](bool bResult){
        _bInitPetEquipInfo = true;
    });
    
    PetEquipBoxManager::getInstance()->requestEquipBoxInfo([=](bool bResult){
        _bInitPetEquipBox = true;
    });
    
    
    LootsManager::getInstance()->requestLootsInfo(false, [=](bool bResult){
        //원정대 전리품
        _bInitExpeditionLoot = true;
    });
    
    TimingPackageManager::getInstance()->requestTimingPackageInfo([=](bool bResult){
        _bInitTimingPackage = true;
    });
    
    NewRaidManager::getInstance()->requestInfo([=](bool, int){
        _bInitNewRaid = true;
    }, true);
    
    WeaponSpiritManager::getInstance()->requestInfo([=](bool, int){
        _bInitWeaponSpirit = true;
    });
    
    ChatManager::getInstance()->requestInfo(E_CHAT_TYPE::NORMAL, [=](bool bResult, int nResult){
        _bInitChat = true;
    });
    
    AdventureManager::getInstance()->requestInfo([=](bool bResult, int nResult){
        _bInitAdventure = true;
    });
    
    AdventureRelicManager::getInstance()->requestInfoCore([=](bool bResult, int nResult){
        _bInitAdventureCore = true;
    });
    
    AdventureRelicManager::getInstance()->requestInfoRelic([=](bool bResult, int nResult){
        _bInitAdventureRelic = true;
    });
    
    AdventureRelicManager::getInstance()->requestInfoRelicCollect([=](bool bResult, int nResult){
        _bInitAdventureRelicCollect = true;
    });
    
    FriendManager::getInstance()->requestFriendInfo([=](bool){
        _bInitFriend = true;
    });
    
    FlashmanManager::getInstance()->requestInfo(false, [=](bool, int){
        _bInitFlashman = true;
    });
    
    PrisonManager::getInstance()->requestInfo(false, 1, nullptr);
    PrisonManager::getInstance()->requestJellyInfo(nullptr);
    PrisonFrostManager::getInstance()->requestInfo(false, [=](bool, int){
        _bInitPrisonFrost = true;
    });
    
    //
    CashManager::getInstance()->requestNewsInfo([=](bool bResult){
        CashManager::getInstance()->downloadNews(nullptr, nullptr, nullptr, nullptr);
    });
    
    // 정액제 정보 가져오기
    CashManager::getInstance()->requestDailyInfo([=](bool bResult){
        _bInitAdsFree = true;
    });
    
    // 상점 배너 정보 가져오기
    CashManager::getInstance()->requestBannerInfo([=](bool bResult){
        _bInitCashBannerInfo = true;
    });
    
    // 상점 성장 정보 가져오기
    CashManager::getInstance()->requestGrowthInfo(0, [=](bool bResult, bool bBuy){
        _bInitCashGrowthInfo = true;
    });
    
    //
    CashManager::getInstance()->requestPurchaseRemainingAll([=](bool bResult){
        _bInitCashRemaining = true;
    });
    
    // 상점 첫결제 정보 가져오기
    if ( EventManager::getInstance()->isActivatedEvent(13) == false )
    {
        _bInitCashFirstBonus = true;
    }
    else
    {
        CashManager::getInstance()->requestFirstBonusInfo([=](bool bResult){
            _bInitCashFirstBonus = true;
        });
    }
    
    //
    EventBingoManager::getInstance()->requestInfo([=](bool bResult, int nResult){
        _bInitEventBingo = true;
    });
    
    EventTradingMarketManager::getInstance()->requestInfo([=](bool bResult, int nResult){
        _bInitEventTradingMarket = true;
    });
    
    EventReturnAttendManager::getInstance()->requestInfo([=](bool, int){
        _bInitEventReturnAttend = true;
    });
    
    EventNewUserAttendManager::getInstance()->requestInfo([=](bool, int){
        _bInitEventNewUserAttend = true;
    });
    
    EventVoteManager::getInstance()->requestInfo(false, [=](bool, int){
        _bInitEventVote = true;
    });
    
    DefenseManager::getInstance()->requestInfo(false, [=](bool, int)
    {
        _bInitDefense = true;
        DefenseManager::getInstance()->loadLocalData();
    }, true);
    
    DefenseManaAMPManager::getInstance()->requestInfo(false, [=](bool, int){
        _bInitDefenseAMP = true;
    }, true);
    
    //EventBoardGameManager::getInstance()->requestBoardInfo([=](bool, int){
        _bInitEventBoardGame = true;
    //});
    
    //EventLuckyBagManager::getInstance()->requestInfo(false, [=](bool, int){
        _bInitEventLuckybag = true;
    //});
    
    //EventTreasureHuntManager::getInstance()->requestInfo(false, [=](bool, int){
        _bInitEventTreasureHunt = true;
    //});
    
    EventSunfishManager::getInstance()->requestInfo(false);
    
    EventRaceManager::getInstance()->requestInfo(nullptr);
    
    //
    if(UserInfoConfigManager::getInstance()->isAlarm(E_ALARM_ACTIVE::BONGGIOK))
    {
        MafNative::CancelLocalNotification(E_NOTI::NOTI_RAID_BONGGIOK);
        UserInfoConfigManager::getInstance()->setAlarm(E_ALARM_ACTIVE::BONGGIOK, false);
    }
    if(UserInfoConfigManager::getInstance()->isAlarm(E_ALARM_ACTIVE::RAID))
    {
        DataManager::getInstance()->requestPushTopic("raid", false);
        UserInfoConfigManager::getInstance()->setAlarm(E_ALARM_ACTIVE::RAID, false);
    }
}

#pragma mark - start game
void IntroScene::startGame()
{
    // schedule game
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    if ( scheduler->isScheduled(schedule_selector(SaveManager::timerUpdate), SaveManager::getInstance()) == false )
    {
        scheduler->schedule(schedule_selector(SaveManager::timerUpdate), SaveManager::getInstance(), 1 , kRepeatForever, 1.0f, false);
    }
    
    //저장값으로 시즌1, 시즌2 구분
    E_PLACE place = (E_PLACE)UserDefault::getInstance()->getIntegerForKey(KEY_PLACE, (int)E_PLACE::DG_NORMAL);
    if ( place == E_PLACE::DG_DEVIL_DOM )
    {
        UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
    }
    else
    {
        //
        int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
        MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_HIGH, nFloorHigh);
        
        //
        auto scene = UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
        scene->runAction(CallFunc::create([=](){
            auto popup = PopupNotic::create();
            popup->show();
        }));
    }
}

#pragma mark - check
void IntroScene::checkMaintainance()
{
    bool bMaintainance = false;
    
    int nMaintainance = ParameterManager::getInstance()->getParameterInteger("maintainance");
    if ( nMaintainance == 1 )
    {
        bMaintainance = true;
    }
    else
    {
        std::string strMaintainance = ParameterManager::getInstance()->getParameterString("maintainance_version");
        
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)strMaintainance.c_str());
        if( jsonParser.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParser.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser[i];
                
                std::string strVersion = jsonValue.GetString();
                if ( strVersion.compare(MafNative::GetGameVersion()) == 0 )
                {
                    bMaintainance = true;
                    break;
                }
            }
        }
    }
    
    if ( bMaintainance == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_47"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            Director::getInstance()->end();
        });
        popup->show();
    }
    else
    {
        _bInitMaintainance = true;
        infoRequest();
    }
}

void IntroScene::checkVersion()
{
    std::string strVersion = "";
    std::string strVersionForce = "";
    if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    {
        strVersion = ParameterManager::getInstance()->getParameterString("ios_version");
        strVersionForce = ParameterManager::getInstance()->getParameterString("ios_force_version");
    }
    else
    {
        strVersion = ParameterManager::getInstance()->getParameterString("aos_version");
        strVersionForce = ParameterManager::getInstance()->getParameterString("aos_force_version");
    }
    
    if ( MafNative::IsLessThanCurrVersion(strVersionForce) == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_system_update_title"), GAME_TEXT("t_ui_system_update_message"));
        popup->addButton(GAME_TEXT("t_ui_system_update_shortcut"), true, [=](Ref* sender){
            // link
            MafNative::OpenUrl(URL_THISGAME);
        }, false);
        popup->setHideBackKey(false);
        popup->setHideAll(false);
        popup->show();
    }
    else if ( MafNative::IsLessThanCurrVersion(strVersion) == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_system_update_title"), GAME_TEXT("t_ui_system_update_message"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            // init
            _bInitVersion = true;
            infoRequest();
        });
        popup->addButton(GAME_TEXT("t_ui_system_update_shortcut"), false, [=](Ref* sender){
            // link
            MafNative::OpenUrl(URL_THISGAME);
        }, false);
        popup->show();
    }
    else
    {
        // init
        _bInitVersion = true;
        infoRequest();
    }
}

void IntroScene::checkReverseApk()
{
    ssize_t nFileLen1 = 0;
    ssize_t nFileLen2 = 0;
    ssize_t nFileLen3 = 0;
    ssize_t nFileLen4 = 0;
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        std::string strApkPath = getApkPath();
        FileUtils::getInstance()->getFileDataFromZip(strApkPath, "classes.dex", &nFileLen1);
        FileUtils::getInstance()->getFileDataFromZip(strApkPath, "classes2.dex", &nFileLen2);
        FileUtils::getInstance()->getFileDataFromZip(strApkPath, "lib/arm64-v8a/libMyGame.so", &nFileLen3);
        FileUtils::getInstance()->getFileDataFromZip(strApkPath, "lib/armeabi-v7a/libMyGame.so", &nFileLen4);
    #endif
    
    std::string strDocumentsText = TableManager::getInstance()->getTableData(E_TABLE::REVERSE_APK);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strDocumentsText.c_str());
    if( strDocumentsText.length() == 0 || jsonParser.HasParseError() == true )
    {
        //
        _bInitModulation = true;
        infoRequest();
        
        return;
    }
    
    bool bReverse = false;
    std::string strFileSize = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        std::string strVersion = jsonValue["version"].GetString();
        if ( strVersion.compare(MafNative::GetGameVersion()) != 0 )
        {
            continue;
        }
        
        strFileSize = "";
        
        int64_t classes1 = jsonValue["classes_1"].GetInt64();
        int64_t classes2 = jsonValue["classes_2"].GetInt64();
        int64_t so_v8a = jsonValue["so_v8a"].GetInt64();    //64bit : arm64_v8a
        int64_t so_v7a = jsonValue["so_v7a"].GetInt64();    //32bit : armeabi-v7a
        
        if ( nFileLen1 != 0 && classes1 != 0 && nFileLen1 != classes1  )
        {
            bReverse = true;
        }
        
        if ( nFileLen2 != 0 && classes2 != 0 && nFileLen2 != classes2 )
        {
            bReverse = true;
        }
    
        if ( nFileLen3 != 0 && so_v8a != 0 && nFileLen3 != so_v8a )
        {//64bit : arm64_v8a
            bReverse = true;
        }
        
        if ( nFileLen4 != 0 && so_v7a != 0 && nFileLen4 != so_v7a )
        {//32bit : armeabi-v7a
            bReverse = true;
        }
        
        if ( bReverse == true )
        {
            strFileSize = MafUtils::format("%ld, %ld, %ld, %ld", nFileLen3, nFileLen4, nFileLen1, nFileLen2);
        }
    }
    
    if ( bReverse == true )
    {
        UserDefault::getInstance()->setStringForKey(KEY_HACK_SIZE, strFileSize);
    }
    
    if ( strFileSize.compare("0, 0, 0, 0") == 0 )
    {
        UserDefault::getInstance()->setStringForKey(KEY_HACK_SIZE, "");
    }
    
    //
    _bInitModulation = true;
    infoRequest();
}

int IntroScene::checkFinishJobTotal()
{
    int count = 0;
    
    count++;// _bInitUserData;
    count++;// _bInitUrl;
    count++;// _bInitStory;
    count++;// _bInitAgree;
    count++;// _bInitTable;
    count++;// _bInitMaintainance;
    count++;// _bInitVersion;
    count++;// _bInitModulation;
    count++;// _bInitLoginSelect;
    count++;// _bInitLogin;
    count++;// _bInitBan;
    
    count++;// _bInitDataLoad;
    count++;// _bInitEvent;
    count++;// _bInitContents
    count++;// _bInitOfflineAds;
    
    count++;// _bInitTime;
    
    count++;// _bInitAdsFree;
    count++;// _bInitCashBannerInfo;
    count++;// _bInitCashGrowthInfo;
    count++;// _bInitCashFirstBonus;
    count++;// _bInitCashRemaining;
    count++;//  _bInitDailyMission
    count++;//  _bInitWeekMission
    count++;//  _bInitDungeonPass
    count++;//  _bInitBongSkin
    count++;//  _bInitBadge
    count++;//  _bInitUserCount
    count++;//  _bInitPetEquipInfo
    count++;//  _bInitPetEquipBox
    count++;//  _bInitExpeditionLoot
    count++;//  _bInitTimingPackage
    count++;//  _bInitEventBingo
    count++;//  _bInitNewRaid
    count++;//  _bInitWeaponSpirit
    count++;//  _bInitEventTradingMarket
    count++;//  _bInitChat
    count++;//  _bInitAdventure
    count++;//  _bInitAdventureRelic
    count++;//  _bInitAdventureCore
    count++;//  _bInitAdventureRelicCollect
    count++;//  _bInitFriend
    count++;//  _bInitEventReturnAttend
    count++;//  _bInitEventNewUserAttend
    count++;//  _bInitPrisonFrost
    count++;//  _bInitEventVote
    count++;//  _bInitEventLuckybag
    count++;//  _bInitEventTreasureHunt
    count++;//  _bInitEventBoardGame
    count++;//  _bInitFlashman
    count++;//  _bInitDefense
    count++;//  _bInitDefenseAMP
    
    return count;
}

int IntroScene::checkFinishJob()
{
    int count = 0;
    if ( _bInitUserData == true )   count++;
    if ( _bInitUrl == true )   count++;
    if ( _bInitStory == true )   count++;
    if ( _bInitAgree == true )   count++;
    if ( _bInitTable == true )   count++;
    if ( _bInitMaintainance == true )   count++;
    if ( _bInitVersion == true )   count++;
    if ( _bInitModulation == true )   count++;
    if ( _bInitLoginSelect == true ) count++;
    if ( _bInitLogin == true )   count++;
    if ( _bInitBan == true )   count++;
    
    if ( _bInitDataLoad == true )   count++;
    if ( _bInitEvent == true )   count++;
    if ( _bInitContents == true )   count++;
    if ( _bInitOfflineAds == true )   count++;

    if ( _bInitTime == true )   count++;
    
    if ( _bInitAdsFree == true )   count++;
    if ( _bInitCashBannerInfo == true )   count++;
    if ( _bInitCashGrowthInfo == true )   count++;
    if ( _bInitCashFirstBonus == true )   count++;
    if ( _bInitCashRemaining == true )   count++;
    if ( _bInitDailyMission == true )   count++;
    if ( _bInitWeekMission == true )   count++;
    if ( _bInitDungeonPass == true )   count++;
    if ( _bInitBongSkin == true )   count++;
    if ( _bInitBadge == true )   count++;
    if ( _bInitUserCount == true )   count++;
    if ( _bInitPetEquipInfo == true )   count++;
    if ( _bInitPetEquipBox == true )   count++;
    if ( _bInitExpeditionLoot == true )   count++;
    if ( _bInitTimingPackage == true )   count++;
    if ( _bInitEventBingo == true )   count++;
    if ( _bInitWeaponSpirit == true )   count++;
    if ( _bInitNewRaid == true )   count++;
    if ( _bInitEventTradingMarket == true )   count++;
    if ( _bInitChat == true )   count++;
    if ( _bInitAdventure == true )   count++;
    if ( _bInitAdventureCore == true )   count++;
    if ( _bInitAdventureRelic == true )   count++;
    if ( _bInitAdventureRelicCollect == true )   count++;
    if ( _bInitFriend == true )   count++;
    if ( _bInitEventReturnAttend == true )   count++;
    if ( _bInitEventNewUserAttend == true )   count++;
    if ( _bInitPrisonFrost == true )   count++;
    if ( _bInitEventVote == true )   count++;
    if ( _bInitEventLuckybag == true )   count++;
    if ( _bInitEventTreasureHunt == true )   count++;
    if ( _bInitEventBoardGame == true )   count++;
    if ( _bInitFlashman == true )   count++;
    if ( _bInitDefense == true )   count++;
    if ( _bInitDefenseAMP == true )   count++;
    
    return count;
}

#pragma mark - callback
void IntroScene::callbackUserAgree()
{
    MafAnalyticsManager::LogEvent(kAnalEventAgree, kRepeatFalse);
    
    //
    UserDefault::getInstance()->setBoolForKey(KEY_USER_AGREE, true);
    
    //
    _bInitAgree = true;
    infoRequest();
}

void IntroScene::callbackStory()
{
    MafAnalyticsManager::LogEvent(kAnalEventStory, kRepeatFalse);
    
    _bInitStory = true;
    infoRequest();
}

void IntroScene::callbackOffline()
{
    // 오프라인 보상
    bool bOfflineTime = OfflineRewardManager::getInstance()->isOfflineTime();
    if ( bOfflineTime == true )
    {
        auto popup = PopupOffline::create();
        popup->setCallbackResult(CC_CALLBACK_0(IntroScene::callbackOfflineReward, this));
        popup->setHideBackKey(false);
        popup->setHideAll(false);
        popup->show();
    }
    else
    {
        startGame();
    }
}

void IntroScene::callbackOfflineReward()
{
    startGame();
}

void IntroScene::callbackEvent(bool bResult)
{
    _bInitEvent = true;
    infoRequest();
    
    // table load
    if ( EventManager::getInstance()->isActivatedEvent(20) == true )
    {
        EventDrawLimitManager::getInstance()->setLoad();
    }
}

void IntroScene::callbackBan(bool bResult, std::string strDay, E_BAN_TYPE eType)
{
    if ( bResult == true )
    {// ban 처리
        
        std::string strErrorText = GAME_TEXT("t_ui_error_7");
        
        switch (eType) {
            case E_BAN_TYPE::BAN_PERM:
            {
                strErrorText += "\n\n" + GAME_TEXT("t_ban_text_1");
            }
            break;
            case E_BAN_TYPE::BAN_PERIOD:
            {
                strErrorText = GAME_TEXT("t_ui_ban_msg_1");
                
                strDay += " UTC+09:00";
                
                strErrorText = MafUtils::format(strErrorText.c_str(), strDay.c_str());
                
                strErrorText += "\n" + GAME_TEXT("t_ban_text_3");
            }
            break;
            case E_BAN_TYPE::BAN_TRANSFER:
            {
                strErrorText += "\n\n" + GAME_TEXT("t_ban_text_2");
                SaveManager::getInstance()->destroyAllUserData();
            }
            break;
            default:
            {
                strErrorText += "\n\n" + GAME_TEXT("t_ban_text_default");
            }
            break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strErrorText);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            Director::getInstance()->end();
        });
        popup->addButton(GAME_TEXT("t_ui_menu_contactus"), false, [=](Ref* sender){

            std::string strGameId = "";
            std::string strVersion = "";
            std::string strAuth = "";
            
            if ( AccountManager::getInstance()->getUserID().length() != 0 )
                strGameId = AccountManager::getInstance()->getUserID();
            else
                strGameId = GAME_TEXT("t_ui_error_1");
            
            strVersion = MafNative::GetGameVersion();
            strAuth = MafAes256::Encrypt(strGameId);
            
            std::string strSubject = GAME_TEXT("t_ui_menu_msg_13");
            std::string strBody = GAME_TEXTFORMAT("t_ui_menu_msg_14", strGameId.c_str(), strVersion.c_str(), MafNative::getDeviceModel().c_str(), MafNative::getDeviceSystemVersion().c_str(), strAuth.c_str());
            
            MafNative::SendEmail(EMAIL_HELP, strSubject.c_str(), strBody.c_str());
            
        }, false);
        popup->setHideBackKey(false);
        popup->setHideAll(false);
        popup->show();
    }
    else
    {
        _bInitBan = true;
        infoRequest();
    }
}

void IntroScene::callbackDataLoad(bool bResult, bool bData)
{
    if ( bResult == false || bData == false )
    {
        _bInitDataLoad = true;
        infoRequest();
        return;
    }
    
    auto popup = PopupCloudLoad::create();
    popup->setCallbackResult(CC_CALLBACK_0(IntroScene::callbackDataLoadFinish, this));
    popup->show();
}

void IntroScene::callbackDataLoadFinish()
{
    _bInitDataLoad = true;
    infoRequest();
}

void IntroScene::callbackTable(bool bResult)
{
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            infoRequest();
        });
        popup->show();
        return;
    }
    
    // text init again
    TextManager::getInstance()->textInit();
    TextManager::getInstance()->dialogTextInit();
    TextManager::getInstance()->talkInit();
    
    ParameterManager::getInstance()->setLoad();
    
    //
    DataManager::getInstance()->loadArtifactData();
    
    AchievementManager::getInstance()->setLoadPrison();
    AchievementManager::getInstance()->setLoadNormal();
    AdventureManager::getInstance()->setLoadAll();
    AdventureRelicManager::getInstance()->setLoadAll();
    BadgeManager::getInstance()->setLoad();
    CashManager::getInstance()->setLoad();
    CashManager::getInstance()->setLoadGrowth();
    PrisonFrostManager::getInstance()->setLoad();
    FlashmanManager::getInstance()->setLoad();
    CostumeManager::getInstance()->setLoad();
    CostumeSkinManager::getInstance()->setLoadSkinList();
    CostumeSkinManager::getInstance()->setLoadSkinShop();
    DialogManager::getInstance()->setLoadAll();
    ItemsManager::getInstance()->setLoad();
    LootsManager::getInstance()->setLoad();
    NewRaidManager::getInstance()->setLoad();
    WeaponManager::getInstance()->setLoad();
    PetNewManager::getInstance()->setLoad();
    PetNewDevilManager::getInstance()->setLoad();
    PetEquipBoxManager::getInstance()->setLoad();
    MineralManager::getInstance()->setLoad();
    MonsterManager::getInstance()->setLoad();
    TradingMarketManager::getInstance()->setLoad();
    
    EventBoardGameManager::getInstance()->setLoadBoard();
    EventBoardGameManager::getInstance()->setLoadQuest();
    EventTradingMarketManager::getInstance()->setLoadTradingItem();
    EventTradingMarketManager::getInstance()->setLoadSetting();
    EventVoteManager::getInstance()->setLoad();
    EventLuckyBagManager::getInstance()->setLoad();
    EventTreasureHuntManager::getInstance()->setLoad();
    EventSunfishManager::getInstance()->setLoad();
    
    //
    _bInitTable = true;
    
    _nTableDownloadTotal = 0;
    _nTableDownloadSuccess = 0;
    _nTableProgress = 0;
    infoRequest();
}

void IntroScene::callbackTableProgress(int nTotal, int nSuccess, int nProgress)
{
    _nTableDownloadTotal = nTotal;
    _nTableDownloadSuccess = nSuccess;
    _nTableProgress = nProgress;
    
    //
    drawMessage(1);
}

#pragma mark - network
void IntroScene::requestTime()
{
    std::string url = "/time/utc/timestamp";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTime(response,dd);
    };
    req->send(cb);
}

void IntroScene::responseTime(network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("result") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            requestTime();  // 다시 요청
        });
        popup->show();
        return;
    }
    
    // date
    time_t nTime = jsonParser["result"].GetInt64();
    UtilsDate::getInstance()->setTimeServer(nTime);
    
    // init
    _bInitTime = true;
    infoRequest();
}

void IntroScene::requestContents()
{
    std::string url = "/contents/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseContents(response,dd);
    };
    req->send(cb);
}

void IntroScene::responseContents(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            requestContents(); // 다시 요청
        });
        popup->show();
        return;
    }
    
    int nCoin = jsonParser["_coin"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
    
    int nFriendAmount = jsonParser["_friends"].GetInt();
    FriendManager::getInstance()->setFriendListAmount(nFriendAmount);
    
    bool bGift = jsonParser["_is_gift"].GetBool();
    UserInfoManager::getInstance()->setGiftHave(bGift);
    
    bool bAttend = jsonParser["_is_daily"].GetBool();
    UserInfoManager::getInstance()->setAttendHave(bAttend);
    
    if(jsonParser.HasMember("_eventticket"))
    {
        int currentEventTicket = jsonParser["_eventticket"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TICKET, 0, currentEventTicket);
    }
    
    if(jsonParser.HasMember("_my_rank_1"))
    {
        auto objRank = jsonParser["_my_rank_1"].GetObject();
        int score = objRank["score"].GetInt();
        int rank = objRank["rank"].GetInt();
        
        RankingManager::getInstance()->renewalMyRankInfo(E_RANK_LOCAL_TYPE::HIGH_FLOOR, E_RANK_TYPE::RANK_HIGH_FLOOR, rank, score);
        auto rankInfo = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
        if(rankInfo != nullptr)
        {
            //랭킹 정보가 있을 때 랭킹 정보의 최대 층이 현재 최대 층과 다르다면 랭킹 정보 업데이트
            if(rankInfo->getScore() != UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL))
            {
                int diff = UserInfoConfigManager::getInstance()->getSaveTimeDiff();
                if(diff < 30)
                {
                    RankingManager::getInstance()->requestHighFloorRankUpdate(nullptr);
                }
                else
                {
                    RankingManager::getInstance()->setRankUpdateTimer(0.f);
                }
            }
        }
    }
    
    //
    _bInitContents = true;
    infoRequest();
}
 
