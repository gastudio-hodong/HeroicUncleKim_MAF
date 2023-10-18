#include "HelloWorldScene.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoArtifact.h"

#include "GameUnit/Skill/UnitHeroSkillFrost.h"

#include "GameScene/BattleNormal/Layer/LayerNormalPet.h"
#include "GameScene/BattleNormal/Layer/LayerNormalQuest.h"
#include "GameScene/BattleNormal/Layer/LayerNormalWeapon.h"
#include "GameScene/BattleNormal/Layer/LayerNormalDungeon.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/OtherEvent/Event/PopupEventDaily.h"
#include "GameUIPopup/Other/Buff/PopupBuff.h"
#include "GameUIPopup/Other/Buff/PopupBuffNewbie.h"
#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"
#include "GameUIPopup/Other/Settings/PopupQuit.h"
#include "GameUIPopup/Other/Attend/PopupAttendMini.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBanner.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseGrowth.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Revive/PopupRevive.h"
#include "GameUIPopup/Other/Stat/PopupStat.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseFirst.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseReturn.h"
#include "GameUIPopup/Other/Attend/PopupAttend2020.h"
#include "GameUIPopup/Other/Costume/PopupCostumeRecommend.h"
#include "GameUIPopup/Other/Costume/PopupCostumeDefault.h"
#include "GameUIPopup/Other/Pass/DungeonPass/PopupDungeonPass.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/Expedition/PopupExpeditionDraw.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/OtherEvent/EventToken/PopupEventDrawLimit.h"
#include "GameUIPopup/Other/User/PopupUserNickname.h"
#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"
#include "GameUIPopup/OtherEvent/EventBingoGame/PopupBingoGame.h"
#include "GameUIPopup/Other/Settings/SaveLoad/PopupToastAutoSave.h"
#include "GameUIPopup/OtherEvent/EventTradingMarket/PopupEventTradingMarket.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/TimeTravel/PopupTimeTravel.h"
#include "GameUIPopup/Other/TimeTravel/PopupTimeTravelReward.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureGuild.h"
#include "GameUIPopup/Other/Chat/PopupChat.h"
#include "GameUIPopup/OtherEvent/Event7th/Popup7thInformation.h"
#include "GameUIPopup/OtherEvent/Event7th/Popup7th.h"
#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicDefault.h"
#include "GameUIPopup/OtherEvent/EventNewUserAttend/PopupEventNewUserAttend.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceTitle.h"
#include "GameUIPopup/Other/Mission/PopupMissionDefault.h"
#include "GameUIPopup/OtherEvent/EventBoardGame/PopupBoardGameMain.h"
#include "GameUIPopup/OtherEvent/EventReturnAttend/PopupEventReturnAttend.h"
#include "GameUIPopup/Other/Tutorial/TargetFloorUI.h"
#include "GameUIPopup/Other/Tutorial/DragFingerUI.h"
#include "GameUIPopup/OtherEvent/EventVote/PopupEventVoteDefault.h"
#include "GameUIPopup/OtherEvent/EventLuckyBag/PopupEventLuckyBagDefault.h"
#include "GameUIPopup/OtherEvent/EventTreasureHunt/PopupEventTreasureHunt.h"
#include "GameUIPopup/Other/Defense/PopupDefenseDefault.h"
#include "GameUIPopup/OtherEvent/EventSunfish/PopupEventSunfish.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UIBannerPurchase.h"
#include "GameUI/UI/UIFirstPurchase.h"
#include "GameUI/UI/UIReturnPurchase.h"
#include "GameUI/UI/UISubMenu.h"
#include "GameUI/UI/UISettingMenu.h"
#include "GameUI/UI/UIHotDealMenu.h"
#include "GameUI/UI/UIUserInfo.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIRandomBox.h"
#include "GameUI/UI/UICurrency.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventDrawLimitManager.h"
#include "ManagerEvent/EventBingoManager.h"
#include "ManagerEvent/EventTradingMarketManager.h"
#include "ManagerEvent/Event7thManager.h"
#include "ManagerEvent/EventReturnAttendManager.h"
#include "ManagerEvent/EventNewUserAttendManager.h"
#include "ManagerEvent/EventTreasureHuntManager.h"
#include "ManagerEvent/EventLuckyBagManager.h"
#include "ManagerEvent/EventSunfishManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/FriendManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/TimeTravelManager.h"
#include "ManagerGame/ChatManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/RankingManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/DefenseManager.h"

USING_NS_CC;

static HelloWorldScene *s_HelloWorld = nullptr;
HelloWorldScene* HelloWorldScene::getInstance()
{
    return s_HelloWorld;
}

HelloWorldScene::HelloWorldScene() :
_bInitUI(false),

_eGameState(E_GAME_STATE::QUEST),
_itemPurchaseBanner(nullptr),
_itemMission(nullptr),
_itemNewYear2020(nullptr),
_itemBuffRecommend(nullptr),
_layerTopWidget(nullptr),
_layerTableWidget(nullptr),
_layerItemFever(nullptr),
_uiSettingMenu(nullptr),
_selectadPath(1),
_nMapIdx(0),
_nMapType(0)
{
    s_HelloWorld = this;
}

HelloWorldScene::~HelloWorldScene()
{
    
}

bool HelloWorldScene::init()
{
    if ( !BaseGameScene::init() )
    {
        return false;
    }
    //
    s_HelloWorld = this;
    
    
    _ePlace = E_PLACE::DG_NORMAL;
    UserInfoManager::getInstance()->setCurrentPlace(_ePlace);
    
    _iPrestigeSpecial = 0;
    _bRevivalAds = false;
    _bRevivalFree = false;
    _nRevivalFloorSub = 0;
    _sprReceivePopup = nullptr;
    _sprDarkScreen = nullptr;
    _enemy = nullptr;
    _hero = nullptr;
    _layerFever = nullptr;
    _layerItemFever = nullptr;
    _layerSaveWarning = nullptr;
    _layerTimeTravel = nullptr;

    UserInfoManager::getInstance()->_onModifyFloor = nullptr;
    UserInfoManager::getInstance()->_onModifyEquip = nullptr;
    
    // sound
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    if( floor % 100 == 0 )
    {
        SoundManager::SoundBgmStart(SOUND_BGM_BOSS, true);
    }
    else if ( floor >= 1001 )
    {
        SoundManager::SoundBgmStart(SOUND_BGM_HELL, true);
    }
    else
    {
        SoundManager::SoundBgmStart(SOUND_BGM, true);
    }
    
    EventManager::getInstance()->_onEventIconShow.Clear();
    EventManager::getInstance()->_onEventIconShow += new MafFunction<void()>(CC_CALLBACK_0(HelloWorldScene::initTopUI, this), this);
    EventManager::getInstance()->_onEventIconShow += new MafFunction<void()>([=](){
        stepwiseContentsOpen();
        reDrawSpecialQuest();
        }, this);

    return true;
}

void HelloWorldScene::onEnter()
{
    BaseGameScene::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        UserInfoManager::getInstance()->_onModifyFloor = CC_CALLBACK_0(HelloWorldScene::drawFloor,this);
        UserInfoManager::getInstance()->setOneKill(_ePlace, false);
        UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, E_PRINCESS_BUFF::BUFF_NONE);
        UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER);
        UserInfoManager::getInstance()->setBattleUI(this);
        
        E_NEWBIE_STEP eNewbie = DataManager::GetNewbieStep(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL), UserInfoManager::getInstance()->getTotalFloor() , SpecialMissionManager::getInstance()->getSpecialMissionClear());
        UserInfoConfigManager::getInstance()->setNewbieStep(eNewbie);

        //
        startScene();
        
        //
        _fTimer = _fTimer1sec = _fTimerQuestSave = 0;
        schedule(schedule_selector(HelloWorldScene::scheduleTimer), 1, kRepeatForever, 1);

        //버프 광고 스케줄
        schedule(schedule_selector(HelloWorldScene::timeSchedule), 1, kRepeatForever, 1);
        
        // update
        scheduleUpdate();
        
        //
        TutorialManager::getInstance()->drawTutorial();
        
        // 날짜가 다를때만 출력
        std::string strToday = UtilsDate::getInstance()->getTimeString(UtilsDate::getInstance()->getTime(), UtilsDate::eType::date_real_Ymd);
        if ( UserDefault::getInstance()->getStringForKey(KEY_TODAY, "").compare(strToday) != 0 )
        {
            //출석체크를 한 적이 없다면
            if (UserInfoManager::getInstance()->getAttendHave() == true && SpecialMissionManager::getInstance()->getSpecialMissionComplete() >= 85)
            {
                auto popup = PopupAttendMini::create();
                popup->show();
            }
            
            UserDefault::getInstance()->setStringForKey(KEY_TODAY, strToday);
        }
        
        if ( EventManager::getInstance()->isActivatedEvent(14) == true && UserDefault::getInstance()->getBoolForKey(KEY_RETURN_POPUP, false) == false )
        {
            UserDefault::getInstance()->setBoolForKey(KEY_RETURN_POPUP, true);
            
            auto popup = PopupPurchaseReturn::create();
            popup->show();
        }
        
        if(TutorialManager::getInstance()->getFirstReviver())
        {
            StoryManager::getInstance()->showStory((int)E_COMIC::TUTORIAL);
            TutorialManager::getInstance()->setFirstReviver(false);
        }
        
        //
        MineralManager::getInstance()->onRefineProductionUpdate();
        
        //
        RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME_BUY, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::ARTIFACT, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::PET, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::PET_FREE, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::ITEM_FEVER, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_INFO, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::WEAPON_BUY, this);
        
        _bInitUI = true;
    }
    
    //
    MafGoogleRewardAds::HideGoogleBanner();
    
    //
    auto adsErrorMgr = AdsUtilsManager::getInstance();
    if(adsErrorMgr->getAdsCooldown() > 0)
    {
        adsErrorMgr->setAdsCooldown(adsErrorMgr->getAdsCooldown());
    }
}

void HelloWorldScene::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::COSTUME || eType == E_REFRESH::COSTUME_BUY )
    {
        reSetHeroMotion();
    }
    else if ( eType == E_REFRESH::ARTIFACT )
    {
        E_ARTIFACT eType = (E_ARTIFACT)listData.at("type").asInt();
        reDrawArtifact(eType);
    }
    else if ( eType == E_REFRESH::PET_FREE )
    {
        E_PET ePet = (E_PET)listData.at("type").asInt();
        createPet(ePet);
    }
    else if ( eType == E_REFRESH::PET )
    {
        E_PET ePet = (E_PET)listData.at("type").asInt();
        evolutionPet(ePet);
    }
    else if ( eType == E_REFRESH::ITEM_FEVER )
    {
        showItemFeverTime();
    }
    else if ( eType == E_REFRESH::WEAPON_INFO || eType == E_REFRESH::WEAPON_BUY )
    {
        reSetHeroMotion();
    }
}

#pragma mark -
void HelloWorldScene::move(E_AREA_MOVE eType)
{
    
    if ( eType == E_AREA_MOVE::AREA_NORMAL )
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("QUEST");
        onButtonTouched(item);
    }
    else if ( eType == E_AREA_MOVE::AREA_DEVIL || eType == E_AREA_MOVE::AREA_PRISON)
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("DUNGEON_INPUT");
        onButtonTouched(item);
    }
    else if(eType == E_AREA_MOVE::AREA_NEW_RAID)
    {
        bool bEnter = false;
        
        int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
        if ( nRevival > 1 )
        {
            bEnter = true;
        }
        
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupRaidDefault::create();
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_COSTUME )
    {
        auto popup = PopupCostumeDefault::create();
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_FRIEND )
    {
        auto popup = PopupFriend::create();
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_DRAW )
    {
        auto popup = PopupExpeditionDraw::create();
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_ADVENTURE )
    {
        bool bEnter = true;
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE)
        {
            bEnter = false;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_info_title_1"), GAME_TEXT("t_ui_error_12"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
        {
            Vec2 posToast = getContentSize()/2;
            PopupToast::showImmediately(GAME_TEXT("t_ui_advt_error_7"), posToast.x, posToast.y);
            return;
        }
        
        auto popup = PopupAdventureGuild::create();
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_SHOP )
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("CASH");
        onButtonTouched(item);
    }
    else if ( eType == E_AREA_MOVE::AREA_NEW_RAID_DRAGON_BALL_SHOP )
    {
        auto popup = PopupRaidShopDefault::create(PopupRaidShopDefault::E_TYPE::PURCHASE);
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_ADVENTURE_CORE_SHOP )
    {
        if(AdventureManager::getInstance()->getTutorialStep() > 6)
        {
            auto popup = PopupAdventureRelicDefault::create(PopupAdventureRelicDefault::E_TYPE::SHOP);
            popup->show();
        }
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_QUEST || eType == E_AREA_MOVE::AREA_NORMAL_ARTIFACT || eType == E_AREA_MOVE::AREA_NORMAL_WEAPON ||
              eType == E_AREA_MOVE::AREA_NORMAL_PET || eType == E_AREA_MOVE::AREA_NORMAL_MYHOME )
    {
        std::string strName = "QUEST";
        switch (eType) {
            case E_AREA_MOVE::AREA_NORMAL_ARTIFACT:
                strName = "ARTIFACT";
                break;
            case E_AREA_MOVE::AREA_NORMAL_WEAPON:
                strName = "EQUIP";
                break;
            case E_AREA_MOVE::AREA_NORMAL_PET:
                strName = "PET";
                break;
                
            default:
                break;
        }
        
        auto item = MafNode::MafMenuItem::create();
        item->setName(strName);
        onButtonTouched(item);
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_MYHOME )
    {
        auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MYHOME);
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_TOWN )
    {
        auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::TOWN);
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_MINE )
    {
        if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MINE);
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_12"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_REVIVE )
    {
        onClickoRevive();
    }
    else if ( eType == E_AREA_MOVE::AREA_DEVIL_QUEST || eType == E_AREA_MOVE::AREA_DEVIL_WEAPON || eType == E_AREA_MOVE::AREA_DEVIL_SLATE )
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("DUNGEON_INPUT");
        onButtonTouched(item);
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_MISSION )
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("DAILY_MISSION");
        onButtonTouched(item);
    }
    else if(eType == E_AREA_MOVE::AREA_DEFENSE)
    {
        bool bEnter = false;
        
        if (UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 10000)
        {
            bEnter = true;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if(!DefenseManager::getInstance()->isOpenContentsNow())
        {
            auto popup = PopupDefenseDefault::create();
            popup->show();
        }
    }
}



#pragma mark-
bool HelloWorldScene::onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event)
{
    //
    bool bResult = BaseGameScene::onKeyRelease(keyCode, event);
    if ( bResult == false )
    {
        return false;
    }
    
    //
    auto popup = PopupQuit::create();
    popup->show();
    
    return true;
}

void HelloWorldScene::onButtonTouched(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    //
    MenuItem *item = (MenuItem*)pSender;
    
    //
    std::string name = item->getName();
    if ( name == "QUEST" || name == "EQUIP" || name == "ARTIFACT" || name == "PET" || name == "DUNGEON_INPUT" )
    {
        
        E_GAME_STATE state = E_GAME_STATE::QUEST;
        if(name == "QUEST")         state = E_GAME_STATE::QUEST;
        else if(name == "EQUIP")    state = E_GAME_STATE::EQUIP;
        else if(name == "ARTIFACT")
        {
            if(_cellArtifacTop->getTabType() == 0 || _cellArtifacTop->getTabType() == 1 || _cellArtifacTop->getTabType() == 100) state = E_GAME_STATE::ARTIFACT;
            else if(_cellArtifacTop->getTabType() == 9000) state = E_GAME_STATE::ARTIFACT_INFINITE;
            else state = E_GAME_STATE::RELIC;
        }
        else if(name == "PET")      state = E_GAME_STATE::PET;
        else if(name == "DUNGEON_INPUT")
        {
            state = E_GAME_STATE::DUNGEON;
            
        }
        
        Vector<MafNode::MafMenuItemSprite*> parentMenu = (Vector<MafNode::MafMenuItemSprite*>&)_menu_btn->getChildren();
        for(auto child : parentMenu)
        {
            if(child->getName() == name)
                child->selected();
            else
                child->unselected();
            
            auto label = (Label*)child->getChildByTag(10000);
            if ( label != nullptr )
            {
                if(child->getName() == name)
                    label->setTextColor(Color4B::WHITE);
                else
                    label->setTextColor(Color4B(115, 116, 118, 255));
            }
        }
        
        setGameState(state);
    }
    else if ( name == "MENU" || name == "ATTEND" || name == "GIFT" )
    {
        if ( _uiSettingMenu != nullptr )
        {
            _uiSettingMenu->onExpand();
        }
    }
    else if ( name == "DAILY_MISSION" )
    {
        E_MISSION eType = E_MISSION::SPECIAL;
        if ( ServerMissionManager::isServerMissionOpenCondition() )
        {
            if(!ServerMissionManager::getInstance()->isServerMissionCompleteDaily() && ServerMissionManager::getInstance()->isServerMissionCompleteWeekly())
                eType = E_MISSION::WEEK;
            else
                eType = E_MISSION::DAILY;
        }
        
        //
        auto popup = PopupMissionDefault::create(eType);
        popup->show();
    }
    else if ( name == "SPECIAL_MISSION" )
    {
        if(_itemSpecialMission->getScaleX() != 1.0f)
            return;
        
        auto popup = PopupMissionDefault::create(E_MISSION::SPECIAL);
        popup->show();
    }
    else if ( name == "BUFF_DETAIL" )
    {
        ValueVector list;
        list.push_back(Value(E_SHOP::SHOP_ATTACK_BONUS));
        list.push_back(Value(E_SHOP::SHOP_COIN_BONUS));
        list.push_back(Value(E_SHOP::SHOP_MOVE_BONUS));
        
        auto popup = PopupBuff::create(list);
        popup->show();
    }
    else if ( name == "BUFF_NEWBIE")
    {
        auto popup = PopupBuffNewbie::create();
        popup->show();
    }
    else if ( name == "TIME_TRAVEL" )
    {
        if ( UserInfoManager::getInstance()->getHighFloor(_ePlace) <=  500 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_time_jump_unlock_mag", 500));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        auto popup = PopupTimeTravel::create();
        popup->setCallbackTimeTravel(CC_CALLBACK_0(HelloWorldScene::callbackTimeTravel, this));
        popup->show();
    }
    else if (name == "ADVENTURE")
    {
        bool bEnter = true;
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE)
        {
            bEnter = false;
        }
        
        if ( bEnter == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_info_title_1"), GAME_TEXT("t_ui_error_12"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
        {
            Vec2 posToast = getContentSize()/2;
            PopupToast::showImmediately(GAME_TEXT("t_ui_advt_error_7"), posToast.x, posToast.y);
            return;
        }
        
        auto popup = PopupAdventureGuild::create();
        popup->show();
    }
    else
    {
        if ( AccountManager::getInstance()->getUserIdx() == 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if ( name == "CASH" )
        {
            //
            CashManager::getInstance()->setNewIcon(false, E_PLACE::DG_NORMAL);
            if(auto objIcon = item->getChildByName("NEW_ICON"))
            {
                objIcon->setVisible(false);
            }
            //
            auto popup = PopupCash::create();
            popup->show();
        }
        else if ( name == "CASH2" )
        {
            //
            RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::PURCHASE_BANNER, false);
            RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PURCHASE_BANNER);
            
            //
            auto popup = PopupPurchaseBanner::create();
            popup->show();
        }
        else if ( name == "CASH3" )
        {
            // 던전 선장 패키지
            auto popup = PopupDungeonPass::create();
            popup->show();
        }
        else if ( name == "CASH_RETURN" )
        {
            if ( EventManager::getInstance()->isActivatedEvent(14) == false )
            {
                item->removeFromParent();
                return;
            }
            auto popup = PopupPurchaseReturn::create();
            popup->show();
        }
        else if(name == "CHAT")
        {
            if ( ChatManager::getInstance()->isBan() == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_7"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            auto popup = PopupChat::create(E_CHAT_TYPE::NORMAL);
            popup->show();
        }
        else if ( name == "COSTUME" )
        {
            auto popup = PopupCostumeDefault::create();
            popup->show();
        }
        else if ( name == "EVENT_7TH" )
        {
            int countPrison = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_COUNT);
            int floorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
            if ( floorHigh >= 300 && countPrison >= 1 )
            {
                auto poup = Popup7th::create();
                poup->show();
            }
            else
            {
                auto poup = Popup7thInformation::create();
                poup->show();
            }
        }
        else if ( name == "EVENT_DRAW_LIMIT" )
        {
            auto uiReddot = item->getChildByName("UIReddot");
            if ( uiReddot != nullptr )
            {
                uiReddot->setVisible(false);
            }
            
            auto popup = PopupEventDrawLimit::create();
            popup->show();
        }
        else if ( name == "EVENT_DRAW_TRADING" )
        {
            bool bEnd = false;
            if ( EventManager::getInstance()->isActivatedEvent(28) == false &&  EventManager::getInstance()->isActivatedEvent(29) == false )
            {
                bEnd = true;
            }
            
            if ( bEnd == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                
                item->setVisible(false);
                return;
            }
            
            auto popup = PopupEventTradingMarket::create();
            popup->show();
        }
        else if ( name == "EVENT_BOARD" )
        {
            bool bEnd = false;
            if ( EventManager::getInstance()->isActivatedEvent(26) == false && EventManager::getInstance()->isActivatedEvent(27) == false  )
            {
                bEnd = true;
            }
            
            if ( bEnd == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                
                item->setVisible(false);
                return;
            }
            
            auto popup = PopupBoardGameMain::create();
            popup->show();
        }
        else if ( name == "EVENT_BINGO" )
        {
            bool bEnd = false;
            if ( EventManager::getInstance()->isActivatedEvent(24) == false && EventManager::getInstance()->isActivatedEvent(25) == false  )
            {
                bEnd = true;
            }
            
            if ( bEnd == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                
                item->setVisible(false);
                return;
            }
            
            auto popup = PopupBingoGame::create();
            popup->show();
        }
        else if ( name == "EVENT_VOTE" )
        {
            bool bEnd = false;
            if ( EventManager::getInstance()->isActivatedEvent(37) == false)
            {
                bEnd = true;
            }
            
            if ( bEnd == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                
                item->setVisible(false);
                return;
            }
            
            auto popup = PopupEventVoteDefault::create();
            popup->show();
        }
        else if ( name == "EVENT_LUCKY_BAG" )
        {
            if ( EventLuckyBagManager::getInstance()->isExtraTime() == true )
            {
                bool bEnd = false;
                if ( EventLuckyBagManager::getInstance()->getEndEventTime() - UtilsDate::getInstance()->getTime() <= 0 )
                {
                    bEnd = true;
                }
                
                if ( bEnd == true )
                {
                    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                    popup->show();
                    
                    item->setVisible(false);
                    return;
                }
            }
            
            auto popup = PopupEventLuckyBagDefault::create();
            popup->show();
        }
        else if ( name == "EVENT_RETURN_ATTEND" )
        {
            auto popup = PopupEventReturnAttend::create();
            popup->show();
        }
        else if ( name == "EVENT_NEW_USER_ATTEND" )
        {
            auto popup = PopupEventNewUserAttend::create();
            popup->show();
        }
        else if ( name == "EVENT_RACE")
        {
            if ( EventManager::getInstance()->isActivatedEvent(33) || EventManager::getInstance()->isActivatedEvent(34))
            {
                auto popup = PopupEventRaceTitle::create();
                popup->show();
            }
            else
            {
                PopupLoading::show();
                EventManager::getInstance()->requestInfo([=](bool){
                    
                    PopupLoading::hide();
                    if (EventManager::getInstance()->isActivatedEvent(33) || EventManager::getInstance()->isActivatedEvent(34))
                        {
                            auto popup = PopupEventRaceTitle::create();
                            popup->show();
                        }
                    else
                        item->setVisible(false);
                        
                });
            }
        }
        else if ( name == "EVENT_TREASURE_HUNT" )
        {
            if ( EventTreasureHuntManager::getInstance()->isExtraTime() == true )
            {
                bool bEnd = false;
                if ( EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime() <= 0 )
                {
                    bEnd = true;
                }
                
                if ( bEnd == true )
                {
                    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                    popup->show();
                    
                    item->setVisible(false);
                    return;
                }
            }
            
            auto popup = PopupEventTreasureHunt::create();
            popup->show();
        }
        else if ( name == "EVENT_SUNFISH" )
        {
            if ( EventSunfishManager::getInstance()->isExtraTime() == true )
            {
                bool bEnd = false;
                if ( EventSunfishManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime() <= 0 )
                {
                    bEnd = true;
                }
                
                if ( bEnd == true )
                {
                    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
                    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                    popup->show();
                    
                    item->setVisible(false);
                    return;
                }
            }
            
            auto popup = PopupEventSunfish::create();
            popup->show();
        }
    }
}
void HelloWorldScene::onLockButtonTouched(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    MenuItem *item = (MenuItem*)pSender;
    
    int nMissionNeed = 0;
    int nFloorNeed = 0;
    
    std::string name = item->getName();
    if( name == "COSTUME_Lock" )
    {
        nMissionNeed = 38;
    }
    else if(name == "BUILDING_Lock")
    {
        nMissionNeed = 34;
    }
    else if(name == "PET_Lock")
    {
        nMissionNeed = 35;
    }
    else if(name == "ARTIFACT_Lock")
    {
        nMissionNeed = 34;
    }
    else if(name == "MINE_Lock")
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_12"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( name == "DUNGEONPASS_Lock");
    {
        nFloorNeed = 100;
    }
    
    if ( nMissionNeed != 0 )
    {
        std::string strMessage = GAME_TEXTFORMAT("t_ui_locked_menu", nMissionNeed);
        
        Vec2 posToast = Vec2::ZERO;
        if( name == "COSTUME_Lock" )
        {
            Size size = UtilsRect::getVisibleSize();
            
            posToast = Vec2(size.width / 2, size.height - 255 - UserInfoManager::getInstance()->_fHeightIphoneX);
        }
        else
        {
            posToast = item->getParent()->convertToWorldSpace(item->getPosition());
            posToast.y += item->getContentSize().height * 1.2f ;
        }
            
        
        PopupToast::showImmediately(strMessage, posToast.x, posToast.y);
    }
    else if ( nFloorNeed != 0 )
    {
        std::string strMessage = GAME_TEXTFORMAT("t_ui_dungeonpass_lock", nFloorNeed);
        
        Size size = UtilsRect::getVisibleSize();
        
        Vec2 posToast = Vec2(size.width / 2, size.height - 255 - UserInfoManager::getInstance()->_fHeightIphoneX);
        PopupToast::showImmediately(strMessage, posToast.x, posToast.y);
    }
}

void HelloWorldScene::update(float delta)
{
    // mineral
    {
        MineralManager::getInstance()->update(delta);
    }
}

void HelloWorldScene::scheduleMove(float dt)
{
    moveMap();
}

void HelloWorldScene::scheduleTimer(float dt){
    
    _fTimer += dt;
    _fTimer1sec+=dt;
    _fTimerQuestSave+=dt;
    
    if (_fTimer >= 60 )
    {
        ServerMissionManager::getInstance()->checkMidnightReset();
        _fTimer = 0.0f;
    }
    
    if(_fTimer1sec >= 1)
    {
        _fTimer1sec = 0.f;
        drawBuffTimer();
        TutorialManager::getInstance()->drawTutorial();
        CheckServerMission();
        ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::PLAYTIME_S, 1);
        
        ItemsMoneyManager::getInstance()->setGold(UserInfoManager::getInstance()->getSecondEarnGoldNormal());
        
        WeaponSpiritManager::getInstance()->scheduleSpiritBuff();
        if(WeaponSpiritManager::getInstance()->getBuffState() == WeaponSpiritManager::E_SPIRIT_BUFF_STATE::DURATION_TIME)
        {
            _hero->setSpiritBuff(true);
        }
        else
        {
            _hero->setSpiritBuff(false);
        }
    }
    
    if(_fTimerQuestSave>=2)
    {
        _fTimerQuestSave-=2;
        MineralManager::getInstance()->saveData();
        
        SpecialMissionManager::getInstance()->checkSpeicalMission(_ePlace);
        
        auto itemRevive = getMenuTopWidget("ITEM_REVIVE");
        if ( itemRevive != nullptr )
        {
            auto spriteBox = itemRevive->getChildByName<Sprite*>("REVIVE_BOX");
            auto labelKey = spriteBox->getChildByName<Label*>("REVIVE_TEXT");
            
            int nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
            std::string strPrestigeKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, nFloor);
            
            strPrestigeKey = MafUtils::bigMulNum(strPrestigeKey, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(502, 1) * 100));
            strPrestigeKey.pop_back();
            strPrestigeKey.pop_back();
            
            MafUtils::convertNumberToShort(strPrestigeKey);
            labelKey->setString(strPrestigeKey);
        }
         
        stepwiseContentsOpen(false, E_STEPWISE_TYPE::ETC);
    }
    
    for(int i = 0; i < _vecRecommendCostumes.size(); ++i)
    {
        _vecRecommendCostumes[i]._timer += dt;
        
        if(_vecRecommendCostumes[i]._timer >= 5)
        {
            callbackRecommendCostume(_vecRecommendCostumes[i]._eType, _vecRecommendCostumes[i]._eIdx);
            break;
        }
    }
    
    bool isRefreshRank = UserDefault::getInstance()->getBoolForKey(KEY_REFRESH_RANK, true);
    if(isRefreshRank == true)
    {
        RankingManager::getInstance()->update(dt);
    }
}

void HelloWorldScene::scheduleFever(float dt)
{
    // 피버타임
    float time = UserInfoManager::getInstance()->getFeverTime();
    if ( time > 0 )
    {
        time = time - dt;
        UserInfoManager::getInstance()->setFeverTime(time);
    }
}

void HelloWorldScene::scheduleItemFever(float dt)
{
    // 피버타임
    float time = UserInfoManager::getInstance()->getItemFeverTime();
    if ( time > 0 )
    {
        time = time - dt;
        UserInfoManager::getInstance()->setItemFeverTime(time);
    }
}

void HelloWorldScene::timeSchedule(float dt)
{
    UserInfoManager::getInstance()->setTimeAdsAllMinus();
    UserInfoManager::getInstance()->setTimeMyhomeAdsAllMinus();
    
   //
    if(UserInfoStackManager::getInstance()->getTotalAttackChickBuffTime() > 0)
    {
        UserInfoStackManager::getInstance()->setTotalAttackChickBuffTime(UserInfoStackManager::getInstance()->getTotalAttackChickBuffTime()-1);
    }
    

    //
    for (int i=1; i<=C_COUNT_COSTUME_TYPE; i++)
    {
        int renttime = CostumeManager::getInstance()->getRentTime((E_COSTUME)i);
        if (renttime > 0) {
            renttime -= 1;
            if (renttime % 10 == 0) {
                CostumeManager::getInstance()->setRentTime((E_COSTUME)i, renttime, true);
            }else{
                CostumeManager::getInstance()->setRentTime((E_COSTUME)i, renttime, false);
            }
            
            if (renttime == 0)
            {
                if (CostumeManager::getInstance()->getEquip((E_COSTUME)i) == CostumeManager::getInstance()->getRent((E_COSTUME)i))
                {
                    CostumeManager::getInstance()->setEquip((E_COSTUME)i, (E_COSTUME_IDX)1);
                    
                    _hero->setChangeParts();
                    _hero->setTalk(GAME_TEXT("t_ui_talk_msg_1"));
                    _hero->setTalkAuto();
                    
                    //
                    RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
                }
                CostumeManager::getInstance()->setRentTime((E_COSTUME)i, 0);
                CostumeManager::getInstance()->setRent((E_COSTUME)i, (E_COSTUME_IDX)0);
                SaveManager::saveCostume();
                SaveManager::saveCostumeRentTime();
                SaveManager::saveCostumeRentList();

                if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_15) == false)
                {
                    
                    DataManager::getInstance()->initArtifactSaveData(DG_NORMAL);
                    reDrawArtifact();
                    
                }
            }
        }
    }
    
}

#pragma mark-
void HelloWorldScene::startScene()
{
    //
    createMapIdx();
    createMap();
    
    initUI();
    initTopUI();
    initBottomUI();

    stepwiseContentsOpen();
    
    createHero();
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        E_PET ePet = (E_PET)i;
        if ( PetNewManager::getInstance()->getPet(ePet) == true )
        {
            createPet(ePet);
        }
    }
    
    
    createTable();
    
    setGameState(E_GAME_STATE::QUEST);
    
    drawFloor();
    reDrawSpecialQuest();
    
    //
    startDungeon();
    
    // 피버타임
    float time = UserInfoManager::getInstance()->getFeverTime();
    if ( time > 0 )
    {
        showFeverTime();
    }
    else
    {
        if ( UserInfoManager::getInstance()->getItemFeverTime() > 0 )
        {
            showItemFeverTime();
        }
    }
    
    //
    checkTutorial();
    
    //
    if(FriendManager::getInstance()->_bBootLinkAdd == true)
    {
        FriendManager::getInstance()->requestFriendIdx();//친구 초대 있으면 실행
    }
    
    
}

void HelloWorldScene::initUI()
{
    // 랜덤 박스 출력
    auto boxRandom = UIRandomBox::create(Size(getContentSize().width, _sizeMapPattern.height), _ePlace);
    boxRandom->setPosition(getContentSize().width / 2, getContentSize().height - _sizeMapPattern.height / 2);
    addChild(boxRandom, E_ZORER::UI+3);
}

void HelloWorldScene::initTopUI()
{
    if ( _layerTopWidget == nullptr )
    {
        double nPosY = getContentSize().height - UserInfoManager::getInstance()->_fHeightIphoneX;
        double nHeight = _sizeMapPattern.height -UserInfoManager::getInstance()->_fHeightIphoneX;
       
        _layerTopWidget = Layer::create();
        _layerTopWidget->setContentSize(Size(_sizeMapPattern.width, nHeight));
        _layerTopWidget->setIgnoreAnchorPointForPosition(false);
        _layerTopWidget->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _layerTopWidget->setPosition(0, nPosY);
        addChild(_layerTopWidget, E_ZORER::UI+1);
    }
    _layerTopWidget->removeAllChildren();
    
    //
    Size size = _layerTopWidget->getContentSize();
    

    // menu
    _menu_TopWidget = MafNode::MafMenu::create();
    _menu_TopWidget->setAnchorPoint(Vec2::ZERO);
    _menu_TopWidget->setPosition(Vec2::ZERO);
    _layerTopWidget->addChild(_menu_TopWidget, 1);
    
    double posX = 0;
    double posY = 0;
    auto menu = _menu_TopWidget;
    
    // userinfo
    {
        auto uiLayout = UIUserInfo::create(_ePlace);
        uiLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiLayout->setPosition(6, size.height - 6);
        _layerTopWidget->addChild(uiLayout);
    }
    
    // floor info
    {
        _layerDungeonInfo = LayerColor::create();
        _layerDungeonInfo->setContentSize(Size(200, 80));
        _layerDungeonInfo->setIgnoreAnchorPointForPosition(false);
        _layerDungeonInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _layerDungeonInfo->setPosition(size.width / 2, size.height);
        _layerTopWidget->addChild(_layerDungeonInfo);
        
        auto spriteWaveBG = Sprite::create("Assets/ui/main/main_center_gauge_box.png");
        spriteWaveBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteWaveBG->setPosition(_layerDungeonInfo->getContentSize().width / 2, 0);
        spriteWaveBG->setName("UI_IMG_WAVE");
        _layerDungeonInfo->addChild(spriteWaveBG);
        
        auto labelRevive = Label::createWithTTF("12", GAME_FONT, 24);
        labelRevive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelRevive->setPosition(_layerDungeonInfo->getContentSize().width / 2, _layerDungeonInfo->getContentSize().height);
        labelRevive->enableOutline(Color4B::BLACK, 1);
        labelRevive->setName("UI_TEXT_REVIVE");
        _layerDungeonInfo->addChild(labelRevive);
        
        auto labelFloor = Label::createWithTTF("1", GAME_FONT, 20);
        labelFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelFloor->setPosition(_layerDungeonInfo->getContentSize().width / 2, 42.5);
        labelFloor->enableOutline(Color4B::BLACK, 1);
        labelFloor->setDimensions(90, labelFloor->getContentSize().height);
        labelFloor->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelFloor->setOverflow(Label::Overflow::SHRINK);
        labelFloor->setColor(Color3B(213, 192, 125));
        labelFloor->setName("UI_TEXT_FLOOR");
        _layerDungeonInfo->addChild(labelFloor);
        
        auto spriteIconBoss = Sprite::create("Assets/ui/main/main_center_gauge_boss.png");
        spriteIconBoss->setPosition(_layerDungeonInfo->getContentSize().width, 10);
        spriteIconBoss->setName("UI_IMG_BOSS");
        _layerDungeonInfo->addChild(spriteIconBoss);
        
        //
        drawFloor();
    }
    
    // buff icon
    {
        posX = 6;
        posY = size.height - 80;
        
        auto uiLayoutBuff = ui::Layout::create();
        uiLayoutBuff->setContentSize(Size(164, 41));
        uiLayoutBuff->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiLayoutBuff->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiLayoutBuff->setPosition(Vec2(posX, posY));
        uiLayoutBuff->setName("UI_LAYOUT_BUFF");
        _layerTopWidget->addChild(uiLayoutBuff);
        
        // newbie
        _itemBuffNewbie = ui::Button::create("Assets/ui_common/icon_newbie_1.png");
        _itemBuffNewbie->addClickEventListener(CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        _itemBuffNewbie->setName("BUFF_NEWBIE");
        uiLayoutBuff->addChild(_itemBuffNewbie);
        
        // buff
        {
            std::vector<E_BUFF_TYPE> listBuff = {E_BUFF_TYPE::BUFF_ATTACK_BONUS, E_BUFF_TYPE::BUFF_COIN_BONUS, E_BUFF_TYPE::BUFF_MOVE_BONUS, E_BUFF_TYPE::BUFF_KEY_BONUS, E_BUFF_TYPE::BUFF_ATTACK_9_BONUS,E_BUFF_TYPE::BUFF_ATTACK_10_BONUS};

            auto uiLayoutBuffDetail = UIBuff::create(listBuff);
            uiLayoutBuffDetail->setAnchorPoint(Vec2::ZERO);
            uiLayoutBuffDetail->setPosition(Vec2(0, 0));
            uiLayoutBuff->addChild(uiLayoutBuffDetail);
        }
        
        //
        reDrawBuffTimer();
    }
    
    // revive
    {
        //환생 아이콘
        posX = 25;
        posY = 65;
        auto itemRevive = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/main/menu_btn_revival_00.png"), nullptr, CC_CALLBACK_0(HelloWorldScene::onClickoRevive, this));
        itemRevive->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        itemRevive->setPosition(posX, posY);
        itemRevive->setName("ITEM_REVIVE");
        itemRevive->setTag(-1);
        menu->addChild(itemRevive);
        
        auto spriteBox = Sprite::create("Assets/ui/main/menu_btn_revival_infobox.png");
        spriteBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteBox->setPosition(Vec2(itemRevive->getContentSize().width / 2, 15));
        spriteBox->setName("REVIVE_BOX");
        itemRevive->addChild(spriteBox);
        {
            auto spriteKey = Sprite::create("Assets/icon/menu_key.png");
            spriteKey->setScale(0.6f);
            spriteKey->setPosition(Vec2(15, spriteBox->getContentSize().height / 2 - 2));
            spriteBox->addChild(spriteKey);
            
            std::string strPrestigeKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, UserInfoManager::getInstance()->getFloor(_ePlace));
            strPrestigeKey = MafUtils::bigMulNum(strPrestigeKey, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(502, 1) * 100));
            strPrestigeKey.pop_back();
            strPrestigeKey.pop_back();
            MafUtils::convertNumberToShort(strPrestigeKey);
            
            auto labelKey = MafNode::createBmFont(strPrestigeKey, GAME_BM_FONT_AL, 20);
            labelKey->setColor(COLOR_KEY);
            labelKey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelKey->setPosition( spriteKey->getPositionX() + 15, spriteBox->getContentSize().height / 2 - 2);
            labelKey->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            labelKey->setName("REVIVE_TEXT");
            spriteBox->addChild(labelKey);
            
            double scaleKey = 70 / labelKey->getContentSize().width;
            if ( scaleKey < 1 )
            {
                labelKey->setScale(scaleKey);
            }
            
            std::string strPrestigeTitle = GAME_TEXT("t_ui_revival_1");
            auto labelTitle = Label::createWithTTF(strPrestigeTitle, GAME_FONT, 20);
            labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelTitle->setPosition( Vec2(spriteBox->getContentSize().width / 2, spriteBox->getContentSize().height - 10) );
            labelTitle->enableOutline(Color4B::BLACK, 2);
            spriteBox->addChild(labelTitle);
        }
        
        drawReviveButton();
    }
    //defense
    {
        auto layerBG = Layer::create();
        layerBG->setContentSize(Size(90,90));
        
        auto itemDefenseBackground = MafNode::MafMenuItemSprite::create(layerBG, nullptr, CC_CALLBACK_1(HelloWorldScene::onClickDefenseBackground, this));
        itemDefenseBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        itemDefenseBackground->setPosition(Vec2(75,165));
        menu->addChild(itemDefenseBackground);
        
        auto sprAutoBG = Sprite::create("Assets/ui/prison/icon_auto1_1.png");
        sprAutoBG->setPosition(itemDefenseBackground->getContentSize()/2);
        itemDefenseBackground->addChild(sprAutoBG);
        sprAutoBG->runAction(RepeatForever::create(RotateBy::create(5.f, -180)));
        
        auto sprIcon = Sprite::create("Assets/ui/defense/icon_defense_main_auto.png");
        sprIcon->setPosition(itemDefenseBackground->getContentSize()/2);
        itemDefenseBackground->addChild(sprIcon);
        
        itemDefenseBackground->setVisible(false);
        if(DefenseManager::getInstance()->isFirstGift())
        {
            auto action1 = CallFunc::create([&, itemDefenseBackground](){
                
                if ( DefenseManager::getInstance()->isBackground())
                {
                    itemDefenseBackground->setVisible(true);
                }
                else
                {
                    itemDefenseBackground->setVisible(false);
                }
            });
            auto action2 = DelayTime::create(0.5f);
            auto seq = Sequence::create(action1, action2, NULL);
            itemDefenseBackground->runAction(RepeatForever::create(seq));
        }
    }
    
    // sub menu
    {
        auto uiLayout = UISubMenu::create(_ePlace);
        uiLayout->setAnchorPoint( Vec2(1.0f, 0.0f) );
        uiLayout->setPosition(Vec2(size.width - 8, 54));
        _layerTopWidget->addChild(uiLayout);
    }
    
    // mission
    {
        // item : mission
        posX = size.width - 11;
        posY = 142;
        _itemMission = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/bg_icon/icon_mission.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        _itemMission->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _itemMission->setPosition(posX, posY);
        _itemMission->setName("DAILY_MISSION");
        menu->addChild(_itemMission);
        
        posX = size.width - 70;
        auto spriteNormal = ui::Scale9Sprite::create("Assets/ui/main/mission_bg_box.png");
        spriteNormal->setContentSize(Size(390, 52));
        
        _itemSpecialMission = MafNode::MafMenuItemSprite::create(spriteNormal, nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        _itemSpecialMission->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _itemSpecialMission->setPosition(posX , posY);
        _itemSpecialMission->setName("SPECIAL_MISSION");
        menu->addChild(_itemSpecialMission);
        {
            auto foldNumber = Label::createWithTTF("QUEST_NUMBER", GAME_FONT, 18);
            foldNumber->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            foldNumber->setPosition(_itemSpecialMission->getContentSize().width - 10, _itemSpecialMission->getContentSize().height);
            foldNumber->setOverflow(Label::Overflow::SHRINK);
            foldNumber->setLocalZOrder(_itemSpecialMission->getLocalZOrder() + 1);
            foldNumber->setColor(Color3B(255, 233, 0));
            foldNumber->enableOutline(Color4B::BLACK, 1);
            foldNumber->setName("QUEST_NUMBER");
            _itemSpecialMission->addChild(foldNumber);
            
            
            auto foldTitle = Label::createWithTTF("QUEST_TITLE", GAME_FONT, 20);
            foldTitle->setContentSize(Size(_itemSpecialMission->getContentSize().width, _itemSpecialMission->getContentSize().height/2));
            foldTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            foldTitle->setPosition(_itemSpecialMission->getContentSize().width - 10, 0);
            foldTitle->setOverflow(Label::Overflow::SHRINK);
            foldTitle->setLocalZOrder(_itemSpecialMission->getLocalZOrder() + 1);
            foldTitle->setAlignment(TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
            foldTitle->enableOutline(Color4B::BLACK, 1);
            foldTitle->setName("QUEST_TITLE");
            _itemSpecialMission->addChild(foldTitle);
        }
    }
    
    // hotdeal menu
    {
        auto uiLayout = UIHotDealMenu::create(_ePlace);
        uiLayout->setAnchorPoint( Vec2(1.0f, 0.0f) );
        uiLayout->setPosition(Vec2(size.width - 8, 230));
        _layerTopWidget->addChild(uiLayout);
    }
    
    // pass, costume, 7주년, 이벤트
    {
        // pass
        posX = size.width - 10;
        posY = size.height - 88;
        auto itemPass = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/main/icon_pass_00.png"), Sprite::create("Assets/ui/main/icon_pass_00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        itemPass->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        itemPass->setPosition(posX, posY);
        itemPass->setName("CASH3");
        menu->addChild(itemPass);
        {
            auto animationIcon = Animation::create();
            animationIcon->setDelayPerUnit(0.15);
            for ( int i = 1; i <= 4; i++ )
            {
                std::string strTemp = MafUtils::format("Assets/ui/main/icon_pass_%02d.png",i);
                animationIcon->addSpriteFrameWithFile(strTemp);
            }
            animationIcon->addSpriteFrameWithFile("Assets/ui/main/icon_pass_00.png");
            
            auto action1 = Animate::create(animationIcon);
            auto action2 = DelayTime::create(0.5);
            auto seq = Sequence::create(action1, action2, NULL);
            auto spriteNormal = (Sprite*)itemPass->getNormalImage();
            spriteNormal->runAction(RepeatForever::create(seq));
            
            //
            _itemDungeonPass = ui::Button::create("Assets/ui/main/icon_pass_lock.png");
            _itemDungeonPass->setPosition(Vec2(itemPass->getContentSize().width / 2, itemPass->getContentSize().height / 2));
            _itemDungeonPass->addClickEventListener(CC_CALLBACK_1(HelloWorldScene::onLockButtonTouched, this));
            _itemDungeonPass->setName("DUNGEONPASS_Lock");
            itemPass->addChild(_itemDungeonPass);
            
            //
            auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_dungeonpass"), GAME_FONT, 16);
            uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextName->setPosition(Vec2(itemPass->getContentSize().width / 2, 20));
            uiTextName->enableOutline(Color4B::BLACK, 2);
            uiTextName->setTextAreaSize(Size(itemPass->getContentSize().width, uiTextName->getContentSize().height));
            uiTextName->setTextOverflow(Label::Overflow::SHRINK);
            uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiTextName->setLineSpacing(-10);
            itemPass->addChild(uiTextName);
        }
        
        // costume
        posX -= 74;
        auto itemCostume = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/main/icon_btn_costume.png"), Sprite::create("Assets/ui/main/icon_btn_costume.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        itemCostume->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        itemCostume->setPosition(posX, posY);
        itemCostume->setName("COSTUME");
        menu->addChild(itemCostume);
        {
            _itemCostumeLock = ui::Button::create("Assets/ui/main/icon_btn_costume_lock.png");
            _itemCostumeLock->setPosition(Vec2(itemCostume->getContentSize().width / 2, itemCostume->getContentSize().height / 2));
            _itemCostumeLock->addClickEventListener(CC_CALLBACK_1(HelloWorldScene::onLockButtonTouched, this));
            _itemCostumeLock->setName("COSTUME_Lock");
            itemCostume->addChild(_itemCostumeLock);
            
            //
            auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_costume"), GAME_FONT, 16);
            uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextName->setPosition(Vec2(itemCostume->getContentSize().width / 2, 20));
            uiTextName->enableOutline(Color4B::BLACK, 2);
            uiTextName->setTextAreaSize(Size(itemCostume->getContentSize().width, uiTextName->getContentSize().height));
            uiTextName->setTextOverflow(Label::Overflow::SHRINK);
            uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiTextName->setLineSpacing(-10);
            itemCostume->addChild(uiTextName);
        }
        
        // 7주년 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(30) == true || EventManager::getInstance()->isActivatedEvent(31) == true )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/white_day_event_00.png"), Sprite::create("Assets/ui/event/white_day_event_00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_7TH");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/white_day_event_%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/white_day_event_00.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        
        // 토큰 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(20) == true && UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 140 )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_token_event_00.png"), Sprite::create("Assets/ui/event/icon_token_event_00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_DRAW_LIMIT");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_token_event_%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_token_event_00.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
            
            if ( EventDrawLimitManager::getInstance()->getTokenNow() >= 1 || EventDrawLimitManager::getInstance()->getResetTime() == 0 )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
                uiReddot->setPosition(itemEvent->getContentSize().width, itemEvent->getContentSize().height);
                itemEvent->addChild(uiReddot);
            }
            
            auto action1 = DelayTime::create(1.0);
            auto action2 = CallFuncN::create([=](Node *sender){
                
                auto uiReddot = sender->getChildByName("UIReddot");
                if ( uiReddot == nullptr )
                {
                    sender->stopAllActions();
                    return;
                }
                
                if ( EventDrawLimitManager::getInstance()->getTokenNow() >= 1 || EventDrawLimitManager::getInstance()->getResetTime() == 0 )
                {
                    uiReddot->setVisible(true);
                }
                else
                {
                    uiReddot->setVisible(false);
                }
            });
            auto seq = Sequence::create(action1, action2, NULL);
            itemEvent->runAction(RepeatForever::create(seq));
        }
        
        // 레이스 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(33) == true || EventManager::getInstance()->isActivatedEvent(34) == true )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/race_event/icon_race_event00.png"), Sprite::create("Assets/ui/race_event/icon_race_event00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_RACE");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/race_event/icon_race_event%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/race_event/icon_race_event00.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        
        // 교환 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(28) == true || EventManager::getInstance()->isActivatedEvent(29) == true )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_22tajir_3.png"), Sprite::create("Assets/ui/event/icon_22tajir_3.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_DRAW_TRADING");
            menu->addChild(itemEvent);
            {
                auto uiTextCount = ui::CText::create("", GAME_FONT, 16);
                uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                uiTextCount->setPosition(Vec2(itemEvent->getContentSize().width / 2, 20));
                uiTextCount->enableOutline(Color4B::BLACK, 2);
                itemEvent->addChild(uiTextCount);
                
                auto action1 = CallFuncN::create([=](Ref* sender) {
                    
                    int nMaxCount = 0;
                    auto objMaxCountSetting = EventTradingMarketManager::getInstance()->getRaidSetting("day_max_coin");
                    if(objMaxCountSetting != nullptr)
                    {
                        nMaxCount = objMaxCountSetting->getValueToInt();
                    }
                        
                    std::string strText = MafUtils::format("%d/%d", EventTradingMarketManager::getInstance()->getTodayCoin(), nMaxCount);

                    auto uiText = (ui::CText*)sender;
                    uiText->setString(strText);
                    
                    if(EventManager::getInstance()->isActivatedEvent(29))
                        uiText->setString(GAME_TEXT("t_ui_event_exchange_center_icon_end"));
                });
                auto action2 = DelayTime::create(1.f);
                auto seq = Sequence::create(action1, action2, nullptr);
                uiTextCount->runAction(RepeatForever::create(seq));
            }
        }
        
        // 보드게임 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(26) == true || EventManager::getInstance()->isActivatedEvent(27) == true )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_map_event_01.png"), Sprite::create("Assets/ui/event/icon_map_event_01.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_BOARD");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 1; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_map_event_%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_map_event_01.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        
        // 빙고게임 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(24) == true || EventManager::getInstance()->isActivatedEvent(25) == true )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_bingo01.png"), nullptr, nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_BINGO");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 1; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_bingo%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_bingo01.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        //소수결 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(37) == true)
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_minority00.png"), Sprite::create("Assets/ui/event/icon_minority00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_VOTE");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i < 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_minority%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_minority00.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        //럭키백 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(38) == true)
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_fishing_1.png"), Sprite::create("Assets/ui/event/icon_fishing_1.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_LUCKY_BAG");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 1; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_fishing_%d.png",i+1);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_fishing_1.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        //보물찾기 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(39) == true)
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_8th_1.png"), Sprite::create("Assets/ui/event/icon_8th_1.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_TREASURE_HUNT");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i < 7; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_8th_%d.png",i+1);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_8th_1.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        // 23년 여름 이벤트
        if ( EventManager::getInstance()->isActivatedEvent(40) == true)
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_lobster_1.png"), Sprite::create("Assets/ui/event/icon_lobster_1.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_SUNFISH");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i < 7; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/event/icon_lobster_%d.png",i+1);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/event/icon_lobster_1.png");
                
                auto action1 = Animate::create(animationIcon);
                auto action2 = DelayTime::create(0.5);
                auto seq = Sequence::create(action1, action2, NULL);
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(seq));
            }
        }
        
    }
    
    // timeTravel, 복귀자 출석
    {
        // 돌파
        posX = size.width - 5;
        posY = size.height - 176;
        auto itemTimeTravel = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/bg_icon/icon_time_jump_off.png"), Sprite::create("Assets/ui/bg_icon/icon_time_jump_off.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        itemTimeTravel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        itemTimeTravel->setPosition(posX, posY);
        itemTimeTravel->setName("TIME_TRAVEL");
        itemTimeTravel->setScale(0.9);
        menu->addChild(itemTimeTravel);
        {
            auto spriteLight = Sprite::create("Assets/ui/bg_icon/icon_time_fx.png");
            spriteLight->setPosition(itemTimeTravel->getContentSize().width / 2, itemTimeTravel->getContentSize().height / 2);
            spriteLight->setName("SPRITE_LIGHT");
            spriteLight->setVisible(false);
            spriteLight->runAction(RepeatForever::create(RotateBy::create(1.0, -180)));
            itemTimeTravel->addChild(spriteLight, -1);
            
            auto action1 = CallFuncN::create([=](Node* sender){
            
                auto itemTimeTravel = (MafNode::MafMenuItemSprite*)sender;
                auto spriteLight = (Sprite *)itemTimeTravel->getChildByName("SPRITE_LIGHT");
                spriteLight->setVisible(false);
                
                std::string strIconPath = "Assets/ui/bg_icon/icon_time_jump_off.png";
                if ( UserInfoManager::getInstance()->getHighFloor(_ePlace) <=  500 )
                {
                    strIconPath = "Assets/ui/bg_icon/icon_time_jump_lock.png";
                }
                else if ( TimeTravelManager::getInstance()->getTimeTravelCount() >= TimeTravelManager::getInstance()->getTimeTravelCountMax())
                {
                    strIconPath = "Assets/ui/bg_icon/icon_time_jump_max.png";
                    
                    spriteLight->setVisible(true);
                }
                else if ( TimeTravelManager::getInstance()->getTimeTravelCount() > 0 )
                {
                    strIconPath = "Assets/ui/bg_icon/icon_time_jump_on.png";
                }
                
                //
                ((Sprite*)itemTimeTravel->getNormalImage())->setTexture(strIconPath);
                ((Sprite*)itemTimeTravel->getSelectedImage())->setTexture(strIconPath);
            });
            auto action2 = DelayTime::create(1.0f);
            auto seq = Sequence::create(action1, action2, NULL);
            itemTimeTravel->runAction(RepeatForever::create(seq));
        }
        
        // 복귀자 출석
        if ( EventReturnAttendManager::getInstance()->isReturnEventCondition() )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/return_event/icon_return_00.png"), Sprite::create("Assets/ui/return_event/icon_return_00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_RETURN_ATTEND");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i <= 7; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/return_event/icon_return_%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/return_event/icon_return_00.png");
                
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(Animate::create(animationIcon)));
            }
            if(EventReturnAttendManager::getInstance()->isRewardCondition())
            {
                auto popup = PopupEventReturnAttend::create();
                popup->show();
            }
        }
        
        // 복귀자 출석
        if ( EventNewUserAttendManager::getInstance()->isNewUserEventCondition() )
        {
            posX -= 74;
            auto itemEvent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/return_event/icon_newbie_00.png"), Sprite::create("Assets/ui/return_event/icon_newbie_00.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
            itemEvent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            itemEvent->setPosition(posX, posY);
            itemEvent->setName("EVENT_NEW_USER_ATTEND");
            menu->addChild(itemEvent);
            {
                auto animationIcon = Animation::create();
                animationIcon->setDelayPerUnit(0.15);
                for ( int i = 0; i <= 6; i++ )
                {
                    std::string strTemp = MafUtils::format("Assets/ui/return_event/icon_newbie_%02d.png",i);
                    animationIcon->addSpriteFrameWithFile(strTemp);
                }
                animationIcon->addSpriteFrameWithFile("Assets/ui/return_event/icon_newbie_00.png");
                
                auto spriteNormal = (Sprite*)itemEvent->getNormalImage();
                spriteNormal->runAction(RepeatForever::create(Animate::create(animationIcon)));
            }
            if(EventNewUserAttendManager::getInstance()->isRewardCondition())
            {
                auto popup = PopupEventNewUserAttend::create();
                popup->show();
            }
        }
    }

    // menu, chat
    {
        // menu
        posX = size.width - 10;
        posY = size.height - 8;
        
        _uiSettingMenu = UISettingMenu::create(_ePlace);
        _uiSettingMenu->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        _uiSettingMenu->setPosition(posX, posY);
        _layerTopWidget->addChild(_uiSettingMenu);

        // chat
        posX -= 74;
        auto itemChat = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/chatting/btn_chatting.png"), Sprite::create("Assets/ui/chatting/btn_chatting.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        itemChat->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        itemChat->setPosition(posX, posY);
        itemChat->setScale(0.6f);
        itemChat->setName("CHAT");
        menu->addChild(itemChat);
        
        // adventure
        posX -= 74;
        auto itemAdventure = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/adventure/adventure_notice.png"), Sprite::create("Assets/ui/adventure/adventure_notice.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        itemAdventure->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        itemAdventure->setPosition(posX, posY);
        itemAdventure->setName("ADVENTURE");
        itemAdventure->setVisible(false);
        menu->addChild(itemAdventure);
        auto action1 = CallFuncN::create([=](Node* sender){
            auto itemAdventure = (MafNode::MafMenuItemSprite*)sender;
            
            if(AdventureManager::getInstance()->isAdventureComplete())
                itemAdventure->setVisible(true);
            else
                itemAdventure->setVisible(false);
        });
        auto action2 = DelayTime::create(1.0f);
        auto seq = Sequence::create(action1, action2, NULL);
        itemAdventure->runAction(RepeatForever::create(seq));
    }
    
    // currency
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GOLD);
    list.push_back(E_ITEMS::KEY);
    list.push_back(E_ITEMS::CUBE);
    list.push_back(E_ITEMS::POINT);
    list.push_back(E_ITEMS::GEM);
    
    auto uiCurrency = UICurrency::create(list, Size(getContentSize().width - 10, 58));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiCurrency->setPosition(Vec2(_layerTopWidget->getContentSize().width / 2, 0));
    uiCurrency->setEffect(true);
    uiCurrency->setUIPosition(Vec2::ANCHOR_MIDDLE);
    _layerTopWidget->addChild(uiCurrency, 1);
    
    //
    _itemBuffRecommend = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/icon_attack_buff2_ad.png"), Sprite::create("Assets/ui/icon_attack_buff2_ad.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onBuffRecommend, this));
    _itemBuffRecommend->setVisible(false);
    menu->addChild(_itemBuffRecommend);
    
    // 저장 경고 출력
    showSaveWarning();
}

void HelloWorldScene::initBottomUI()
{
    //
    _menu_btn = MafNode::MafMenu::create();
    _menu_btn->setAnchorPoint(Vec2::ZERO);
    _menu_btn->setPosition(Vec2::ZERO);
    _menu_btn->setName("BOT_MENU");
    addChild(_menu_btn,E_ZORER::UI);
      
    //
    std::string strPathItemBg = "Assets/ui/menu_bottom/menu_btn_bg_off_new.png";
    std::string strPathItemBgSelect = "Assets/ui/menu_bottom/menu_btn_bg_on_new.png";

    auto spriteTemp = Sprite::create(strPathItemBg);
    Size sizeItemBg = spriteTemp->getContentSize();
    
    
    struct element
    {
        std::string tag;
        std::string strImage;
        Vec2 pos;
        std::string strNameKey;
    };
    
    element listElement1[] = {//아래줄
        {"QUEST",           "Assets/ui/menu_bottom/menu_btn_quest.png",     Vec2(getContentSize().width/2-sizeItemBg.width*3, 0), "t_ui_quest"},
        {"EQUIP",           "Assets/ui/menu_bottom/menu_btn_weapon.png",    Vec2(getContentSize().width/2-sizeItemBg.width*2, 0), "t_ui_weapon"},
        {"PET",             "Assets/ui/menu_bottom/menu_btn_pet.png",       Vec2(getContentSize().width/2-sizeItemBg.width*1, 0), "t_ui_pet"},
        {"ARTIFACT",        "Assets/ui/menu_bottom/menu_btn_treasure.png",  Vec2(getContentSize().width/2+sizeItemBg.width*0, 0), "t_ui_artifact"},
        {"CASH",            "Assets/ui/menu_bottom/menu_btn_package.png",   Vec2(getContentSize().width/2+sizeItemBg.width*1, 0), "t_ui_shop_gem"},
        {"DUNGEON_INPUT",   "Assets/ui/main/menu_btn_dungeon0.png",         Vec2(getContentSize().width/2+sizeItemBg.width*2, 0), "t_ui_dungeon"},
    };
 
    for ( int i = 0; i < sizeof(listElement1)/sizeof(element); i++ )
    {
        element ele = listElement1[i];
        
        MafNode::MafMenuItemSprite *item = MafNode::MafMenuItemSprite::create(Sprite::create(strPathItemBg),  Sprite::create(strPathItemBgSelect), CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
        item->setAnchorPoint(Vec2(0,0));
        item->setName(ele.tag);
        item->getSelectedImage()->setScale(1);
        item->setPosition(ele.pos);
        _menu_btn->addChild(item);
        
        auto labelName = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 22);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2 - 50) );
        labelName->setDimensions(item->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        item->addChild(labelName);
        
        auto sprIcon = Sprite::create(ele.strImage);
        sprIcon->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2 + 10);
        item->addChild(sprIcon);
        
        if ( ele.tag.compare("QUEST") == 0 )
        {
            
            if ( UserDefault::getInstance()->getBoolForKey(KEY_NOTIFY_EFFECT_QUEST, false) == true )
            {
                auto action1 = CallFuncN::create([=](Node* sender){
                    
                    if ( UserDefault::getInstance()->getBoolForKey(KEY_NOTIFY_EFFECT_QUEST, false) == false )
                    {
                        sender->stopAllActions();
                        sender->removeFromParent();
                        return;
                    }
                    
                    int nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
                    if ( nFloor < 11 )
                    {
                        sender->setVisible(false);
                        return;
                    }
                     
                    
                    auto aniButton = Animation::create();
                    aniButton->setDelayPerUnit(0.1);
                    for ( int i = 1; i <= 7; i++ )
                    {
                        aniButton->addSpriteFrameWithFile(MafUtils::format("Assets/ui/tutorial/menu_btn_bg_tutorial_new_%02d.png",i));
                    }
                    
                    sender->setVisible(true);
                    sender->runAction(Animate::create(aniButton));
                });
                auto action2 = DelayTime::create(1.0);
                auto seq = Sequence::create(action1, action2, NULL);
                
                auto spriteEffect = Sprite::create("Assets/ui/tutorial/menu_btn_bg_tutorial_new_01.png");
                spriteEffect->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
                spriteEffect->setVisible(false);
                spriteEffect->runAction(RepeatForever::create(seq));
                item->addChild(spriteEffect);
            }
        }
        else if ( ele.tag.compare("CASH") == 0 )
        {
            if ( CashManager::getInstance()->isNewIcon(E_PLACE::DG_NORMAL) == true)
            {
                auto spriteIcon = Sprite::create("Assets/ui_common/menu_new.png");
                spriteIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                spriteIcon->setPosition(0, item->getContentSize().height);
                spriteIcon->setName("NEW_ICON");
                item->addChild(spriteIcon, 1);
            }
            
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot->setPosition(3, item->getContentSize().height - 3);
            uiReddot->setVisible(false);
            item->addChild(uiReddot);
            {
                auto action1 = CallFuncN::create([=](Node* sender){
            
                    bool bVisible = false;
                    if ( CashManager::getInstance()->isDailyReceiveAll() == true )
                    {
                        bVisible = true;
                    }
                    
                    auto item = (UIReddot*)sender;
                    item->setVisible(bVisible);
                    
                });
                auto action2 = DelayTime::create(2.0);
                auto seq = Sequence::create(action1, action2, NULL);
                uiReddot->runAction(RepeatForever::create(seq));
            }
        }
        else if ( ele.tag.compare("PET") == 0 )
        {
            if ( EventManager::getInstance()->isActivatedEvent(16) == true )
            {
                auto spriteIconEvent = Sprite::create("Assets/ui/pet/event1_1.png");
                spriteIconEvent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                spriteIconEvent->setPosition(item->getContentSize().width / 2, item->getContentSize().height);
                item->addChild(spriteIconEvent);
                {
                    auto aniframeRun = Animation::create();
                    aniframeRun->setDelayPerUnit(0.1f);
                    for ( int i = 1; i <= 5; i++ )
                    {
                        aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/pet/event1_%d.png", i));
                    }
                    aniframeRun->addSpriteFrameWithFile("Assets/ui/pet/event1_1.png");
                    
                    auto action1 = Animate::create(aniframeRun);
                    auto action2 = DelayTime::create(0.5);
                    auto seq = Sequence::create(action1, action2, NULL);
                    spriteIconEvent->runAction(RepeatForever::create(seq));
                }
            }
            
            auto uiReddot1 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot1->setPosition(3, item->getContentSize().height - 3);
            uiReddot1->setVisible(false);
            item->addChild(uiReddot1);
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_ALL, uiReddot1);
            
            auto uiReddot2 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot2->setPosition(3, item->getContentSize().height - 3);
            uiReddot2->setVisible(false);
            item->addChild(uiReddot2);
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EVOLUTION_ALL, uiReddot2);
            
            auto uiReddot3 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot3->setPosition(3, item->getContentSize().height - 3);
            uiReddot3->setVisible(false);
            item->addChild(uiReddot3);
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_BOX, uiReddot3);
            
            auto uiReddot4 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot4->setPosition(3, item->getContentSize().height - 3);
            uiReddot4->setVisible(false);
            item->addChild(uiReddot4);
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_EQUIP_ALL, uiReddot4);
        }
        else if ( ele.tag.compare("EQUIP") == 0 )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot->setPosition(3, item->getContentSize().height - 3);
            uiReddot->setVisible(false);
            item->addChild(uiReddot);
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::EQUIP_CRAFT, uiReddot);
            
            auto uiReddot2 = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            uiReddot2->setPosition(3, item->getContentSize().height - 3);
            item->addChild(uiReddot2);
            uiReddot2->setVisible(false);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                    
                auto uiReddot = (UIReddot*)sender;
                    
                if(WeaponSpiritManager::getInstance()->isReddotCondition())
                {
                    uiReddot->setVisible(true);
                }
                else
                {
                    uiReddot->setVisible(false);
                }
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot2->runAction(RepeatForever::create(seq));
            
            //
            int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(_ePlace);
            if ( nFloorHigh <= 150 )
            {
                auto action1 = CallFuncN::create([=](Node* sender){
                    
                    if ( UserDefault::getInstance()->getBoolForKey(KEY_NOTIFY_EFFECT_EQUIP, false) == false )
                    {
                        sender->setVisible(false);
                        
                        // 몬스터 공격횟수
                        int nHitCount = UserInfoManager::getInstance()->getHitCount(_ePlace);
                        if ( nHitCount < 15 )
                        {
                            return;
                        }
                        
                        int weaponEquip = WeaponManager::getInstance()->getEquip();
                        int weaponLevel = WeaponManager::getInstance()->getLevel(weaponEquip);
                        
                        
                        std::string currencyNeed = WeaponManager::getInstance()->getBuyPrice(weaponEquip, weaponLevel);
                        std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
                        if ( MafUtils::compareStr(currencyGold, currencyNeed) == false )
                        {
                            return;
                        }
                        
                        UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_EQUIP, true);
                    }
                    else
                    {
                        //
                        auto aniButton = Animation::create();
                        aniButton->setDelayPerUnit(0.1);
                        for ( int i = 1; i <= 7; i++ )
                        {
                            aniButton->addSpriteFrameWithFile(MafUtils::format("Assets/ui/tutorial/menu_btn_bg_tutorial_new_%02d.png",i));
                        }
                        sender->setVisible(true);
                        sender->runAction(Animate::create(aniButton));
                    }
                });
                auto action2 = DelayTime::create(1.0);
                auto seq = Sequence::create(action1, action2, NULL);
                
                auto spriteEffect = Sprite::create("Assets/ui/tutorial/menu_btn_bg_tutorial_new_01.png");
                spriteEffect->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
                spriteEffect->setVisible(false);
                spriteEffect->runAction(RepeatForever::create(seq));
                item->addChild(spriteEffect);
            }
        }
        else if ( ele.tag.compare("DUNGEON_INPUT") == 0 )
        {
            if ( EventManager::getInstance()->isActivatedEvent(19) || EventManager::getInstance()->isActivatedEvent(10) )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::EVENT, Vec2::ANCHOR_MIDDLE_TOP);
                uiReddot->setPosition(item->getContentSize().width / 2, item->getContentSize().height - 30);
                item->addChild(uiReddot);
            }
            
            bool isNewRaidActivate = false;
            if ( (NewRaidManager::getInstance()->getAttackCount() >= 1 || NewRaidManager::getInstance()->isLevelUpPossible() || NewRaidManager::getInstance()->isMissionComplete()) && NewRaidManager::getInstance()->isRaidEnterCondition() )
            {
                isNewRaidActivate = true;
            }
            
            if ( PrisonManager::getInstance()->getHeart() >= PrisonManager::getInstance()->getHeartMax() || PrisonManager::getInstance()->isJellyRewardMax() == true || isNewRaidActivate )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(3, item->getContentSize().height - 5);
                item->addChild(uiReddot);
                
                auto action1 = CallFuncN::create([=](Ref* sender){
                    
                    auto uiReddot = (UIReddot*)sender;
                    
                    bool isNewRaidActivate = false;
                    
                    if ( (NewRaidManager::getInstance()->getAttackCount() >= 1 || NewRaidManager::getInstance()->isLevelUpPossible() || NewRaidManager::getInstance()->isMissionComplete()) && NewRaidManager::getInstance()->isRaidEnterCondition() )
                    {
                        isNewRaidActivate = true;
                    }
                    
                    if ( PrisonManager::getInstance()->getHeart() >= PrisonManager::getInstance()->getHeartMax() || PrisonManager::getInstance()->isJellyRewardMax() == true || isNewRaidActivate )
                    {
                        uiReddot->setVisible(true);
                    }
                    else
                    {
                        uiReddot->setVisible(false);
                    }
                });
                auto action2 = DelayTime::create(2);
                auto seq = Sequence::create(action1, action2, NULL);
                uiReddot->runAction(RepeatForever::create(seq));
            }
        }
        
        if ( ele.tag.compare("PET") == 0 || ele.tag.compare("ARTIFACT") == 0 )
        {
            MafNode::MafMenuItemSprite *lockItem = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_lock_new1_1.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onLockButtonTouched, this));
            lockItem->setAnchorPoint(Vec2(0.0,0));
            lockItem->setName(ele.tag + "_Lock");
            lockItem->setPosition(ele.pos);
            _menu_btn->addChild(lockItem);
            
            auto lockLabelName = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 22);
            lockLabelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
            lockLabelName->setPosition( Vec2(lockItem->getContentSize().width / 2, lockItem->getContentSize().height / 2 - 50) );
            lockLabelName->setDimensions(lockItem->getContentSize().width * 0.9, 40);
            lockLabelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lockLabelName->setOverflow(Label::Overflow::SHRINK);
            lockLabelName->setTextColor(Color4B(115, 116, 118, 255));
            lockLabelName->setTag(10000);
            lockItem->addChild(lockLabelName);
        }
    }
}


 
#pragma mark -
void HelloWorldScene::drawFloor()
{

    int nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
    int nWave = UserInfoManager::getInstance()->getFloorCount(_ePlace);
    int revival = UserInfoManager::getInstance()->getRevival(_ePlace);
    
    if ( _enemy != nullptr && _enemy->isDie() == true )
    {
        if ( --nWave <= 0 )
        {
            nWave = C_MAX_FLOOR_COUNT;
        }
    }

    std::string str;
    if     ( revival == 1 ) str = GAME_TEXT("t_ui_dungeon_msg_1");
    else if( revival == 2 ) str = GAME_TEXT("t_ui_dungeon_msg_2");
    else if( revival == 3 ) str = GAME_TEXT("t_ui_dungeon_msg_3");
    else if( revival % 10 == 1 ) str = GAME_TEXTFORMAT("t_ui_dungeon_msg_4", revival);
    else if( revival % 10 == 2 ) str = GAME_TEXTFORMAT("t_ui_dungeon_msg_5", revival);
    else if( revival % 10 == 3 ) str = GAME_TEXTFORMAT("t_ui_dungeon_msg_6", revival);
    else
    {
        str = GAME_TEXTFORMAT("t_ui_dungeon_msg_7", revival);
    }
    
    auto labelRevive = _layerDungeonInfo->getChildByName<Label*>("UI_TEXT_REVIVE");
    labelRevive->setString(str.c_str());
    
    //
    std::string strFloor = MafUtils::toString(nFloor);
    MafUtils::addCommaToString(strFloor, 3, ',');

    auto labelFloor = _layerDungeonInfo->getChildByName<Label*>("UI_TEXT_FLOOR");
    labelFloor->setString(GAME_TEXTFORMAT("t_relic_option_9", strFloor.c_str()));
    
    //
    auto spriteWaveBG = _layerDungeonInfo->getChildByName<Sprite*>("UI_IMG_WAVE");
    spriteWaveBG->removeAllChildren();
    
    //
    auto spriteIconBoss = _layerDungeonInfo->getChildByName<Sprite*>("UI_IMG_BOSS");
    if ( nWave == C_MAX_FLOOR_COUNT )
        spriteIconBoss->setVisible(true);
    else
        spriteIconBoss->setVisible(false);
        
    
    double posX = 5;
    double posY = 4;
    for ( int i = 0; i < nWave; i++ )
    {
        int nWaveTemp = i + 1;
        
        std::string pathBar = "Assets/ui/main/main_center_gauge_red.png";
        if ( nWaveTemp == nWave && (_enemy != nullptr && _enemy->isDie() == false) )
        {
            pathBar = "Assets/ui/main/main_center_gauge_green.png";
        }
        auto spriteBar = Sprite::create(pathBar);
        spriteBar->setAnchorPoint(Vec2::ZERO);
        spriteBar->setPosition(posX, posY);
        spriteWaveBG->addChild(spriteBar);
        
        posX += spriteBar->getContentSize().width;
        posX += 2;
    }
}

void HelloWorldScene::drawReviveButton()
{
    auto itemRevive = getMenuTopWidget("ITEM_REVIVE");
    
    //
    int nFloor = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    int nHitCount = UserInfoManager::getInstance()->getHitCount(E_PLACE::DG_NORMAL);
    
    int nTypePrev = itemRevive->getTag();
    int nTypeNow = 0;

    if( nFloor >= 100 && nHitCount > MafRemoteConfig::GetRemoteValueInt("buff_recommend_count") && nFloorHigh <= MafRemoteConfig::GetRemoteValueInt("buff_recommend_floor") )
    {
        // buff_recommend_count : 100
        // buff_recommend_floor : 2000
        nTypeNow = 2;
    }
    else if ( nFloor >= 100 )
    {
        nTypeNow = 1;
    }
    
    if ( nTypePrev != nTypeNow )
    {
        auto spriteNormal = (Sprite*)itemRevive->getNormalImage();
        
        spriteNormal->stopAllActions();
        spriteNormal->setTexture("Assets/ui/main/menu_btn_revival_00.png");
        
        if ( nTypeNow == 1 )
        {
            auto aniframeRun = Animation::create();
            aniframeRun->setDelayPerUnit(0.1f);
            for ( int i = 0; i <= 5; i++ )
            {
                aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/main/menu_btn_revival_%02d.png", i));
            }
            spriteNormal->runAction(Animate::create(aniframeRun));
        }
        else if ( nTypeNow == 2 )
        {
            auto aniframeRun = Animation::create();
            aniframeRun->setDelayPerUnit(0.1f);
            for ( int i = 5; i <= 10; i++ )
            {
                aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/main/menu_btn_revival_%02d.png", i));
            }
            spriteNormal->runAction(RepeatForever::create(Animate::create(aniframeRun)));
        }
        
        //
        itemRevive->setTag(nTypeNow);
    }
    
}

void HelloWorldScene::reDrawArtifact(E_ARTIFACT arti)
{
    if ( arti == E_ARTIFACT::E_ARTIFACT_999_NONE )
    {
        for(auto cell : _vecCellArtifact)
        {
            int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, cell->_eArtifact);
            if ( level != 0 )
            {
                cell->_levelEncrypt = MafAes256::XorEncrypt(level);
                cell->drawUpgrade();
            }
        }
    }
    else{
        for(auto cell : _vecCellArtifact)
        {
            if (cell->_eArtifact == arti)
            {
                int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, cell->_eArtifact);
                if ( level != 0 )
                {
                    cell->_levelEncrypt = MafAes256::XorEncrypt(level);
                    cell->drawUpgrade();
                }
                break;
            }
        }
    }
    
    if ( _cellArtifacTop != nullptr )
    {
        _cellArtifacTop->drawInfo();
    }
}

void HelloWorldScene::drawBuffTimer()
{
    E_BUFF_TYPE listBuffDefault[] = {
        E_BUFF_TYPE::BUFF_ATTACK_BONUS,
        E_BUFF_TYPE::BUFF_COIN_BONUS,
        E_BUFF_TYPE::BUFF_MOVE_BONUS,
        E_BUFF_TYPE::BUFF_ATTACK_9_BONUS,
        E_BUFF_TYPE::BUFF_ATTACK_10_BONUS,
    };
    
    for ( int i = 0; i < sizeof(listBuffDefault)/sizeof(int); i++)
    {
        E_BUFF_TYPE eType = listBuffDefault[i];
         
        int time = UserInfoManager::getInstance()->getBuffTime(eType);
        if ( time > 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(eType, -1);
            
            time -= 1;
            if ( time == 0 )
            {
                RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
            }
        }
    }
    
    // draw
    reDrawBuffTimer();
}

void HelloWorldScene::reDrawBuffTimer()
{
    {
        E_NEWBIE_STEP eType = UserInfoConfigManager::getInstance()->getNewbieStep();
        if ( eType == E_NEWBIE_STEP::STEP_NONE || eType == E_NEWBIE_STEP::STEP_FINISH )
        {
            _itemBuffNewbie->setVisible(false);
        }
        else
        {
            _itemBuffNewbie->loadTextureNormal(MafUtils::format("Assets/ui_common/icon_newbie_%d.png", (int)eType));
            _itemBuffNewbie->setVisible(true);
        }
    }
    
    auto uiLayoutBuff = _layerTopWidget->getChildByName<ui::Layout*>("UI_LAYOUT_BUFF");
    uiLayoutBuff->requestDoLayout();
    
}

void HelloWorldScene::reSetHeroMotion()
{
    _hero->setChangeParts();
}

void HelloWorldScene::redrawTutorialObject()
{
    _progress = TutorialManager::getInstance()->getTutorialProgress();

    MafNode::MafMenuItemSprite* button = nullptr;

    if(_progress.second == SUB_SHORTCUT_BUTTON || _progress.second == SUB_REWARD_BUTTON)
    {
        if(_itemSpecialMission->getScaleX() == 1.0f)
            button = _itemSpecialMission;
        else
            button = _itemMission;
    }
    else
    {
        if(_progress.first == PROGRESS_QUEST )
        {
            
        }
        else if(_progress.first == PROGRESS_WEAPON)
        {
            
        }
        else if(_progress.first == PROGRESS_ARTIFACT)
        {
            if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 34)
            {
                if(_progress.second == SUB_OBJECT_1)
                {
                    if(_vecCellArtifact.at(0)->_eArtifact == E_ARTIFACT::ARTI_1_EVOL1)
                    {
                        button = _vecCellArtifact.at(0)->_itemUpgradeBtn;
                    }
                }
            }
            else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 36)
            {
                if(_progress.second == SUB_OBJECT_1)
                {
                    if(_vecCellArtifact.at(1)->_eArtifact == E_ARTIFACT::ARTI_3_EVOL1)
                    {
                        button = _vecCellArtifact.at(1)->_itemUpgradeBtn;
                    }
                }
            }
        }
        else if(_progress.first == PROGRESS_PET)
        {
            if(_progress.second == SUB_OBJECT_1)
            {
                
            }
        }
        else if(_progress.first == PROGRESS_SHOP)
        {
            if(_progress.second == SUB_OBJECT_1)
            {
                
            }
        }
        else
        {
            return;
        }
    }

    if(button != nullptr)
    {
        TutorialManager::getInstance()->attachEffect(EFFECT_FINGER, _progress, button);
    }
}

void HelloWorldScene::reDrawSpecialQuest()
{
    //special mission
    auto special_mission_idx = SpecialMissionManager::getInstance()->getSpecialMissionClear() + 1;
    auto list = SpecialMissionManager::getInstance()->getListSpecialMission();
    if(special_mission_idx > list.size())
    {
        _itemSpecialMission->setScaleX(0.f);
        return;
    }
    
    auto cur_special_mission = SpecialMissionManager::getInstance()->getInfoSpecialMission(special_mission_idx);
    
    bool bViewMission = UserDefault::getInstance()->getBoolForKey(KEY_VIEW_SP_QUEST, true);//setUserData
    
    if(!bViewMission)
    {
        _itemSpecialMission->setScaleX(0.f);

        return;
    }else{
        _itemSpecialMission->setScaleX(1.f);
        
        if(SpecialMissionManager::getInstance()->getSpecialMissionComplete() == 1)
        {
            auto spriteNormal = ui::Scale9Sprite::create("Assets/ui/main/mission_bg_box_success.png");
            spriteNormal->setContentSize(Size(390, 52));
            
            _itemSpecialMission->setNormalImage(spriteNormal);
        }
        else
        {
            auto spriteNormal = ui::Scale9Sprite::create("Assets/ui/main/mission_bg_box.png");
            spriteNormal->setContentSize(Size(390, 52));
            
            _itemSpecialMission->setNormalImage(spriteNormal);
        }

        auto quest_label = (Label*)_itemSpecialMission->getChildByName("QUEST_TITLE");
        quest_label->setString(cur_special_mission->getContent());
        
        auto quest_num_label = (Label*)_itemSpecialMission->getChildByName("QUEST_NUMBER");
        quest_num_label->setString(GAME_TEXTFORMAT("t_new_mission_title", cur_special_mission->getIdx()));
        
    }
}

#pragma mark-
MafNode::MafMenuItemSprite* HelloWorldScene::getMenu(std::string name)
{
    return _menu_btn->getChildByName<MafNode::MafMenuItemSprite*>(name);
}

MafNode::MafMenuItemSprite* HelloWorldScene::getMenuTopWidget(std::string name)
{
    return _menu_TopWidget->getChildByName<MafNode::MafMenuItemSprite*>(name);
}

void HelloWorldScene::setGameState(E_GAME_STATE state)
{
    _eGameState = state;    
    showTable();
}

void HelloWorldScene::setGameStateToInt(int state)
{
    _eGameState = (E_GAME_STATE)state;
    showTable();
}

void HelloWorldScene::createTable()
{
    if ( _layerTableWidget == nullptr )
    {
        
        double topWidgetHeight = _sizeMapPattern.height - UserInfoManager::getInstance()->_fHeightIphoneX;
        
        _layerTableWidget = LayerColor::create(Color4B::BLACK);
        _layerTableWidget->setContentSize(Size(getContentSize().width, getContentSize().height - topWidgetHeight));
        _layerTableWidget->setIgnoreAnchorPointForPosition(false);
        _layerTableWidget->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _layerTableWidget->setPosition(getContentSize().width/2, 0);
        addChild(_layerTableWidget);
    }
    _layerTableWidget->removeAllChildren();
    
    auto sprSize = Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_off_new.png");
    
    Size sizeMenu = sprSize->getContentSize();
    float fStartHeight = sizeMenu.height;
    float scrollHeight = getContentSize().height - fStartHeight - _sizeMapPattern.height;
    
    for ( int qq = 0; qq < C_COUNT_GAME_STATE; qq++ )
    {
        float scSizeBarHeight = C_BAR_SIZE;
        
        //
        Size scSize = Size(getContentSize().width, scrollHeight);
        
        //
        extension::ScrollView *scroll = extension::ScrollView::create(scSize);
        scroll->setDelegate(this);
        scroll->setDirection(extension::ScrollView::Direction::VERTICAL);
        scroll->setPosition(Vec2(0, fStartHeight));
        scroll->setClippingToBounds(true);
        scroll->setBounceable(false);
        _layerTableWidget->addChild(scroll);
        
        _scroll[qq] = scroll;
        
        //
        Layer *container = Layer::create();
        container->setPosition(Vec2(0,0));
        scroll->setContainer(container);
        
        int nCount = 0;
        if ( qq == E_GAME_STATE::QUEST )
        {
            _uiContainerQuest = LayerNormalQuest::create();
            _uiContainerQuest->setContentSize(Size(scSize.width, scSize.height));
            _uiContainerQuest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _uiContainerQuest->setPosition(Vec2(scSize.width / 2, sizeMenu.height));
            _layerTableWidget->addChild(_uiContainerQuest);
        }
        else if ( qq == E_GAME_STATE::EQUIP )
        {
            _uiContainerWeapon = LayerNormalWeapon::create();
            _uiContainerWeapon->setContentSize(Size(scSize.width, scSize.height));
            _uiContainerWeapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _uiContainerWeapon->setPosition(Vec2(scSize.width / 2, sizeMenu.height));
            _layerTableWidget->addChild(_uiContainerWeapon);
        }
        else if ( qq == E_GAME_STATE::ARTIFACT )
        {
            _cellArtifacTop = new ModelCell::CellArtifactTop(_ePlace);
            _cellArtifacTop->setCallbackShowRelic(CC_CALLBACK_1(HelloWorldScene::setGameStateToInt, this));
            _cellArtifacTop->setPosition(Vec2(0, getContentSize().height - _sizeMapPattern.height - _cellArtifacTop->getContentSize().height - C_MIDDLE_POSITION));
            _cellArtifacTop->autorelease();
            _cellArtifacTop->setVisible(false);
            _cellArtifacTop->setCallbackTab(CC_CALLBACK_1(HelloWorldScene::callbackArtifactTab, this));
            _layerTableWidget->addChild(_cellArtifacTop,E_ZORER::UI+2);
            
            scSize = Size(getContentSize().width, scrollHeight - _cellArtifacTop->getContentSize().height);
            scroll->setViewSize(scSize);
            
            nCount = createArtifactTable(0);
        }
        else if ( qq == E_GAME_STATE::RELIC )
        {
            nCount = C_COUNT_RELIC;
            for(int i=1; i<=nCount; i++){
                ModelCell::CellRelic *cell = new ModelCell::CellRelic(_ePlace, (E_RELIC)i);
                cell->autorelease();
                //cell->setPosition(Vec2(0,C_BAR_SIZE*nCount-((i)*C_BAR_SIZE)));
                container->addChild(cell);
                
                if(i == E_RELIC::RELIC_1)
                    cell->_onCompleteListener = CC_CALLBACK_0(HelloWorldScene::reSetHeroMotion, this);
                else if(i == E_RELIC::RELIC_2)
                    cell->_onCompleteListener = CC_CALLBACK_0(HelloWorldScene::reSetHeroMotion, this);
                else if(i == E_RELIC::RELIC_6)
                    cell->_onCompleteListener = [=](){
                        
                        // 퀘스트 새로고침
                        RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
                    };
                
                if (i == E_RELIC::RELIC_5) {
                    _vecCellRelic.insert(0,cell);
                }else if (i == E_RELIC::RELIC_6) {
                    _vecCellRelic.insert(1,cell);
                }else{
                    _vecCellRelic.pushBack(cell);
                }
            }
            scSize = Size(getContentSize().width, scrollHeight -68);
            scroll->setViewSize(scSize);
            
            for(int i=1; i<=nCount; i++)
            {
                _vecCellRelic.at(i-1)->setPosition( Vec2(0, scSizeBarHeight * nCount - i * scSizeBarHeight) );
            }
        }
        else if(qq == E_GAME_STATE::PET)
        {
            _uiContainerPet = LayerNormalPet::create();
            _uiContainerPet->setContentSize(Size(scSize.width, scSize.height));
            _uiContainerPet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _uiContainerPet->setPosition(Vec2(scSize.width / 2, sizeMenu.height));
            _layerTableWidget->addChild(_uiContainerPet);
        }
        else if ( qq == E_GAME_STATE::DUNGEON )
        {
            _uiContainerDungeon = LayerNormalDungeon::create(E_PLACE::DG_NORMAL);
            _uiContainerDungeon->setContentSize(Size(scSize.width, scSize.height));
            _uiContainerDungeon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _uiContainerDungeon->setPosition(Vec2(scSize.width / 2, sizeMenu.height));
            _layerTableWidget->addChild(_uiContainerDungeon);
            
            // 2020 신년 보석 500개 이벤트
            if ( EventManager::getInstance()->isActivatedEvent(7) == true )
            {
                auto itemPrestige = _menu_btn->getChildByName("PRESTIGE");
                
                double nPosX = itemPrestige->getPositionX() + itemPrestige->getContentSize().width;
                double nPosY = itemPrestige->getPositionY() + itemPrestige->getContentSize().height + 10;
                
                _itemNewYear2020 = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/event/icon_newmail.png"), nullptr, CC_CALLBACK_1(HelloWorldScene::onButtonTouched, this));
                _itemNewYear2020->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                _itemNewYear2020->setPosition(nPosX, nPosY);
                _itemNewYear2020->setName("EVENT_NEWYEAR");
                _itemNewYear2020->setVisible(false);
                _menu_btn->addChild(_itemNewYear2020);
            }
        }
        else if ( qq == E_GAME_STATE::ARTIFACT_INFINITE )
        {
            
            auto infiniteMgr = InfiniteArtifactManager::getInstance();
            
            if(infiniteMgr->isActive())
            {
                auto vecInfiniteInfo = infiniteMgr->getInfiniteArtiInfoFromGrade(infiniteMgr->getCurGrade());
                
                nCount = INFINITE_ARTIFACT_KIND;
                
                
                for(int i=1; i <= vecInfiniteInfo.size(); i++)
                {
                    ModelCell::CellInfiniteArtifact *cell = new ModelCell::CellInfiniteArtifact(_ePlace, (INFINITE_ARTIFACT_TYPE)i);
                    cell->autorelease();
                    cell->setCompleteListener(CC_CALLBACK_0(HelloWorldScene::callbackCheckInfiniteUpgrade, this));
                    
                    _vecCellInfiniteArti.pushBack(cell);
                }
                
                scSize = Size(getContentSize().width, scrollHeight - _cellArtifacTop->getContentSize().height);
                scroll->setViewSize(scSize);
            }

            
        }
        
        //
        container->setContentSize( Size(getContentSize().width, scSizeBarHeight * nCount) );
        if ( scSize.height > scSizeBarHeight * nCount )
        {
            float h = scSize.height - scSizeBarHeight * nCount;
            scroll->setContentOffset(Vec2(0,h));
        }
        else
        {
            float h = (scSizeBarHeight * nCount - scSize.height);
            scroll->setContentOffset(Vec2(0,-h));
        }
        
        //
        if(qq == E_GAME_STATE::ARTIFACT_INFINITE)
        {
            int i = 1;
            nCount = (int)_vecCellInfiniteArti.size();
            for ( auto child : _vecCellInfiniteArti )
            {
                child->setPosition(0, (i) * C_BAR_SIZE - C_BAR_SIZE);
                container->addChild(child);
                i++;
            }
            
            if(InfiniteArtifactManager::getInstance()->isUpgradeCondition())
            {
                auto cellInfiniteMover = new ModelCell::CellUpgradeInfinite();
                cellInfiniteMover->setName("INFINITE_ARTI_UPGRADE");
                cellInfiniteMover->autorelease();
                cellInfiniteMover->setPosition(0, (_vecCellInfiniteArti.size()+1) * C_BAR_SIZE - C_BAR_SIZE);
                cellInfiniteMover->setCallbackMove(CC_CALLBACK_1(HelloWorldScene::setGameStateToInt, this));
                cellInfiniteMover->setCallbackUpgrade(CC_CALLBACK_1(HelloWorldScene::callbackUpgradeInfiniteArtifact, this));
                container->addChild(cellInfiniteMover);
                nCount++;
            }
            
            container->setContentSize(Size(getContentSize().width,C_BAR_SIZE*nCount));
            scroll->setContentOffset(Vec2(0,-C_BAR_SIZE*nCount));
        }

        //
        _scroll[qq]->setVisible(false);
    }
    
    //스크롤 상단 메뉴 안눌리게 막는장치
    extension::ControlButton *item = extension::ControlButton::create();
    item->addTargetWithActionForControlEvents(this, nullptr, extension::Control::EventType::TOUCH_UP_INSIDE);
    item->setPosition(getContentSize().width/2, getContentSize().height-_sizeMapPattern.height/2 - C_MIDDLE_POSITION*0.5);
    item->setPreferredSize(Size(getContentSize().width,_sizeMapPattern.height + C_MIDDLE_POSITION));
    item->setScaleRatio(1);
    item->setOpacity(0);
    _layerTableWidget->addChild(item,E_ZORER::MAP);
    
    //스크롤 하단 메뉴 안눌리게 막는장치
    item = extension::ControlButton::create();
    item->addTargetWithActionForControlEvents(this, nullptr, extension::Control::EventType::TOUCH_UP_INSIDE);
    item->setAnchorPoint(Vec2(0.5, 0));
    item->setPosition(getContentSize().width/2, 0);
    item->setPreferredSize(Size(getContentSize().width, 150));
    item->setScaleRatio(1);
    item->setOpacity(255);
    _layerTableWidget->addChild(item,E_ZORER::MAP);
}

int HelloWorldScene::createArtifactTable(int type_tab)
{
    
    auto scroll = _scroll[E_GAME_STATE::ARTIFACT];
    auto container = (Layer*)scroll->getContainer();
    container->removeAllChildren();
    
    _vecCellArtifact.clear();

    //
    if ( type_tab == 0 )
    {
        std::vector<int> list;
        list.clear();
        //증폭기 주석처리 (추가 예정)
        if(InfiniteArtifactManager::getInstance()->isActive())
            list = {45,32,52,51,50,11,10,9,2,5,6,106,7,103,104};
        else
            list = {45,32,52,51,50,11,10,9,2,5,6,106,7};
        
        for (auto idx : list)
        {
            E_ARTIFACT type_artifact = (E_ARTIFACT)idx;
            
            auto cell = new ModelCell::CellArtifact(_ePlace, type_artifact);
            cell->setCellArtifactTop(_cellArtifacTop);
            cell->setTab(type_tab);
            cell->autorelease();
            _vecCellArtifact.pushBack(cell);
            
            if ( type_artifact == E_ARTIFACT::ARTI_5)
            {// 찬락한 신속의 망토, 독수리의 상
                cell->_onCompleteListener = [=](){
                    
                    // 퀘스트 새로고침
                    RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
                };
            }
            else if(type_artifact == E_ARTIFACT::ARTI_6)
            {//상인의 가면
                cell->_onCompleteListener = [=](){
                    
                    // 무기 새로고침
                    RefreshManager::getInstance()->refreshUI(E_REFRESH::WEAPON_INFO);
                    
                    // 퀘스트 새로고침
                    RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
                };
            }
            else if(type_artifact == E_ARTIFACT::ARTI_7)
            {// 쾌속의 반지
                cell->_onCompleteListener = CC_CALLBACK_0(HelloWorldScene::reSetHeroMotion,this);
            }
        }
    }
    else
    {
        int list[] = {
            1,  3,  4,  8,  12, 89,
            13, 14, 15, 16, 17, 90,
            18, 19, 20, 21, 22, 91,
            23, 24, 25, 26, 27, 92,
            28, 29, 30, 31, 33, 93,
            35, 36, 37, 38, 39, 94,
            40, 41, 42, 43, 44, 95,
            46, 47, 48, 49, 53, 96,
            54, 55, 56, 57, 58, 97,
            59, 60, 61, 62, 63, 98,
            64, 65, 66, 67, 68, 99,
            69, 70, 71, 72, 73, 100,
            74, 75, 76, 77, 78, 101,
            79, 80, 81, 82, 83, 102,
            84, 85, 86, 87, 88
        };
        
        for (int i = 0; i < sizeof(list)/sizeof(int); i++ )
        {
            E_ARTIFACT type_artifact = (E_ARTIFACT)list[i];
            
            auto obj = DataManager::GetArtifactInfo(type_artifact);
            
            if ( type_tab == 100 )
            {//강화 보물
                if ( obj->isReinforce() == 0 )
                {
                    continue;
                }
                
                if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, type_artifact) < obj->getLevelMax() )
                {
                    continue;
                }
            }
            else if ( type_tab == 1 )
            {//일반 보물
                
                if ( obj->getType() != E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT )
                {
                    auto objPrev = DataManager::GetArtifactInfo((E_ARTIFACT)obj->getPrevArtifact());
                    if ( obj->getPrevArtifact() != 0 && UserInfoManager::getInstance()->getArtifactLevel(_ePlace, (E_ARTIFACT)obj->getPrevArtifact()) < objPrev->getLevelMax() )
                    {
                        continue;
                    }
                }
                else if ( obj->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT && obj->getGeneration() != 1 )
                {
                    bool bContinue = true;
                    for ( int i = 1; i < E_ARTIFACT::E_ARTIFACT_999_NONE; i++ )
                    {
                        auto objTemp = DataManager::GetArtifactInfo((E_ARTIFACT)i);
                        
                        if(objTemp == nullptr)
                            continue;
                        
                        if ( objTemp->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT )
                        {
                            continue;
                        }
                        
                        if ( obj->getGeneration() - 1 == objTemp->getGeneration() )
                        {
                            if ( UserInfoManager::getInstance()->getArtifactLevel(_ePlace, (E_ARTIFACT)i) >= objTemp->getLevelMax() )
                            {
                                bContinue = false;
                                break;
                            }
                        }
                    }
                    
                    if ( bContinue == true )
                    {
                        continue;
                    }
                }
                    
            }
            
            auto cell = new ModelCell::CellArtifact(_ePlace, type_artifact);
            cell->setTab(type_tab);
            cell->setCellArtifactTop(_cellArtifacTop);
            cell->autorelease();
            _vecCellArtifact.pushBack(cell);
            
            
            if ( type_artifact == E_ARTIFACT::ARTI_5 || DataManager::GetArtifactType(type_artifact) == E_ARTIFACT_TYPE::ARTI_TYPE_QUEST)
            {// 찬락한 신속의 망토, 독수리의 상
                cell->_onCompleteListener = [=](){
                    
                    // 퀘스트 새로고침
                    RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
                };
            }
            
            auto tutorial = TutorialManager::getInstance();
            if(type_artifact == E_ARTIFACT::ARTI_1_EVOL1)
            {
                tutorial->attachEffect(EFFECT_FINGER, std::pair<PROGRESS_MAIN, PROGRESS_SUB>(PROGRESS_ARTIFACT,SUB_OBJECT_1), cell->_itemUpgradeBtn);
            }

            if ( obj->isReinforce() == 1 )
            {// 강화 리턴 : 보물 상단 UI redraw
                cell->_onCompleteReinforceListener = CC_CALLBACK_0(HelloWorldScene::callbackArtifactReinforce,this);
            }
        }
        
    }
    
    int nCount = 0;
    int i = 1;
    
    nCount = (int)_vecCellArtifact.size();
    for ( auto child : _vecCellArtifact )
    {
        child->setPosition(0, (i) * C_BAR_SIZE - C_BAR_SIZE);
        container->addChild(child);
        i++;
    }
    
    auto infiniteMgr = InfiniteArtifactManager::getInstance();

    //초월보물 이동 및 업그레이드 셀
    if(type_tab == 1)
    {
        if(infiniteMgr->isActive() == false && infiniteMgr->isOpenCondition())
        {
            auto cellInfiniteMover = new ModelCell::CellUpgradeInfinite();
            cellInfiniteMover->setName("INFINITE_ARTI_UPGRADE");
            cellInfiniteMover->autorelease();
            cellInfiniteMover->setPosition(0, (_vecCellArtifact.size()+1) * C_BAR_SIZE - C_BAR_SIZE);
            cellInfiniteMover->setCallbackMove(CC_CALLBACK_1(HelloWorldScene::setGameStateToInt, this));
            cellInfiniteMover->setCallbackUpgrade(CC_CALLBACK_1(HelloWorldScene::callbackUpgradeInfiniteArtifact, this));
            container->addChild(cellInfiniteMover);
            nCount++;
        }
    }

    container->setContentSize(Size(getContentSize().width,C_BAR_SIZE*nCount));
    scroll->setContentOffset(Vec2(0,-C_BAR_SIZE*nCount));
    
    return nCount;
}

void HelloWorldScene::createInfiniteArtifactTable()
{
    float fStartHeight = 210;
    float scrollHeight = getContentSize().height - fStartHeight - _sizeMapPattern.height - C_MIDDLE_POSITION;
    
    auto infiniteMgr = InfiniteArtifactManager::getInstance();
    auto vecInfiniteInfo = infiniteMgr->getInfiniteArtiInfoFromGrade(infiniteMgr->getCurGrade());
    
    int nCount = INFINITE_ARTIFACT_KIND;
    
    auto scroll = _scroll[E_GAME_STATE::ARTIFACT_INFINITE];
    auto container = (Layer*)scroll->getContainer();
    
    for(int i=1; i <= vecInfiniteInfo.size(); i++)
    {
        ModelCell::CellInfiniteArtifact *cell = new ModelCell::CellInfiniteArtifact(_ePlace, (INFINITE_ARTIFACT_TYPE)i);
        cell->autorelease();
        cell->setCompleteListener(CC_CALLBACK_0(HelloWorldScene::callbackCheckInfiniteUpgrade, this));
        
        _vecCellInfiniteArti.pushBack(cell);
    }
    
    Size scSize = Size(getContentSize().width, scrollHeight - _cellArtifacTop->getContentSize().height);
    scroll->setViewSize(scSize);
    
    int i = 1;
    nCount = (int)_vecCellInfiniteArti.size();
    for ( auto child : _vecCellInfiniteArti )
    {
        child->setPosition(0, (i) * C_BAR_SIZE - C_BAR_SIZE);
        container->addChild(child);
        i++;
    }
    
    if(InfiniteArtifactManager::getInstance()->isUpgradeCondition())
    {
        auto cellInfiniteMover = new ModelCell::CellUpgradeInfinite();
        cellInfiniteMover->setName("INFINITE_ARTI_UPGRADE");
        cellInfiniteMover->autorelease();
        cellInfiniteMover->setPosition(0, (_vecCellInfiniteArti.size()+1) * C_BAR_SIZE - C_BAR_SIZE);
        cellInfiniteMover->setCallbackMove(CC_CALLBACK_1(HelloWorldScene::setGameStateToInt, this));
        cellInfiniteMover->setCallbackUpgrade(CC_CALLBACK_1(HelloWorldScene::callbackUpgradeInfiniteArtifact, this));
        container->addChild(cellInfiniteMover);
        nCount++;
    }

        container->setContentSize(Size(getContentSize().width,C_BAR_SIZE*nCount));
        scroll->setContentOffset(Vec2(0,-C_BAR_SIZE*nCount));
}

void HelloWorldScene::showTable()
{
    extension::ScrollView *showScroll;
    for ( int i = 0; i < C_COUNT_GAME_STATE; i++ )
    {
        if (i == _eGameState )
        {
            _scroll[i]->setVisible(true);
            showScroll = _scroll[i];
        }
        else
        {
            _scroll[i]->setVisible(false);
        }
    }
    
    //
    if ( _eGameState == E_GAME_STATE::QUEST )
    {
        if ( auto uiLayout = dynamic_cast<LayerNormalQuest*>(_uiContainerQuest); uiLayout != nullptr )
        {
            uiLayout->setVisible(true);
//            uiLayout->onListMove();
        }
    }
    else
    {
        _uiContainerQuest->setVisible(false);
    }
        
    //
    if(_eGameState== E_GAME_STATE::EQUIP)
    {
        if ( auto uiLayout = dynamic_cast<LayerNormalWeapon*>(_uiContainerWeapon); uiLayout != nullptr )
        {
            uiLayout->setVisible(true);
            uiLayout->onListMove();
        }
    }
    else
    {
        _uiContainerWeapon->setVisible(false);
    }
    
    //
    if ( _eGameState == E_GAME_STATE::ARTIFACT || _eGameState == E_GAME_STATE::RELIC || _eGameState == E_GAME_STATE::ARTIFACT_INFINITE)
    {
        _cellArtifacTop->setVisible(true);
        _cellArtifacTop->drawInfo();
    }
    else
    {
        _cellArtifacTop->setVisible(false);
    }
    
    //
    if(_eGameState == E_GAME_STATE::PET)
    {
        _uiContainerPet->setVisible(true);
    }
    else
    {
        _uiContainerPet->setVisible(false);
    }
    
    if(_eGameState == E_GAME_STATE::DUNGEON )
    {
        _uiContainerDungeon->setVisible(true);
        if ( _itemNewYear2020 != nullptr )
        {
            _itemNewYear2020->setVisible(true);
        }
    }
    else
    {
        _uiContainerDungeon->setVisible(false);
        if ( _itemNewYear2020 != nullptr )
        {
            _itemNewYear2020->setVisible(false);
        }
    }
}

#pragma mark-
void HelloWorldScene::createMapIdx()
{
    int nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
    
    int nType = 1;
    int nIdx = 1;
    
    if ( nFloor > 30000 )
    {
        nIdx = 15;
    }
    else if ( nFloor > 20000 )
    {
        nIdx = 14;
    }
    else if ( nFloor > 18000 )
    {
        nIdx = 13;
    }
    else if ( nFloor > 16000 && nFloor < 18001  )
    {
        nIdx = 12;
    }
    else if ( nFloor > 14000 && nFloor < 16001 )
    {
        nIdx = 11;
    }
    else if ( nFloor > 12000 && nFloor < 14001 )
    {
        nIdx = 10;
    }
    else
    {
        int nPos = (nFloor-1) / 1000;
        if ( nPos > 10 )
        {//이미지 1만층까지만 존재함.
            nPos = 0;
        }
        
        nIdx = nPos;
    }
    
    // type
    if ( nFloor % 100 == 0 )        nType = 3;
    else if ( nFloor % 10 == 0 )    nType = 2;
    else                            nType = 1;
    
    if ( nFloor == 1000 )           nType = 4;
    
    _nMapIdx = nIdx;
    _nMapType = nType;
}

void HelloWorldScene::createMap()
{
    //
    _sizeMapPattern = Size(getContentSize().width, 700);
    _sizeMapPattern.height += UserInfoManager::getInstance()->_fHeightIphoneX;
    
    //
    std::string strBGDefaultPath = "";
    std::string listBG[] = {
        "Assets/battle_bg/normal/bg_1_%d_%d.png",
        "Assets/battle_bg/normal/bg_1000_%d_%d.png",
        "Assets/battle_bg/normal/bg_2000_%d_%d.png",
        "Assets/battle_bg/normal/bg_3000_%d_%d.png",
        "Assets/battle_bg/normal/bg_4000_%d_%d.png",
        "Assets/battle_bg/normal/bg_5000_%d_%d.png",
        "Assets/battle_bg/normal/bg_6000_%d_%d.png",
        "Assets/battle_bg/normal/bg_7000_%d_%d.png",
        "Assets/battle_bg/normal/bg_8000_%d_%d.png",
        "Assets/battle_bg/normal/bg_9000_%d_%d.png",
        "Assets/battle_bg/normal/bg_12000_%d_%d.png",
        "Assets/battle_bg/normal/bg_14000_%d_%d.png",
        "Assets/battle_bg/normal/bg_16000_%d_%d.png",
        "Assets/battle_bg/normal/bg_18000_%d_%d.png",
        "Assets/battle_bg/normal/bg_20000_%d_%d.png",
        "Assets/battle_bg/normal/bg_30000_%d_%d.png",
    };
    
    //
    strBGDefaultPath = listBG[_nMapIdx];
    
    //
    int nMapCount = 3;
    if ( strBGDefaultPath.compare("Assets/battle_bg/normal/bg_1_%d_%d.png") == 0 )
    {//맵이 4종류
        nMapCount = 4;
    }
    
    
    double nBGPosX = 0.0;
    double nBGPosY = getContentSize().height;
    for ( int i = 0; i < nMapCount; i++)
    {
        std::string strBGPath = MafUtils::format(strBGDefaultPath.c_str(), _nMapType, i+1);
        
        //
        auto spriteBG = Sprite::create(strBGPath);
        spriteBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteBG->setPosition(nBGPosX, nBGPosY);
        spriteBG->getTexture()->setTexParameters(texParams);
        addChild(spriteBG, E_ZORER::MAP);
        _vecMapNode.pushBack(spriteBG);
        
        //
        double nScale = _sizeMapPattern.height / spriteBG->getContentSize().height;
        spriteBG->setScale(nScale);
        
        //
        nBGPosX += spriteBG->getBoundingBox().size.width - 5;
    }
 
    if ( _sprDarkScreen == nullptr )
    {
        _sprDarkScreen = ui::Scale9Sprite::create(PIXEL,Rect(0,0, _sizeMapPattern.width, _sizeMapPattern.height));
        _sprDarkScreen->setColor(Color3B::BLACK);
        _sprDarkScreen->setOpacity(0);
        _sprDarkScreen->setAnchorPoint(Vec2(0,1));
        _sprDarkScreen->setPosition(0, getContentSize().height);
        addChild(_sprDarkScreen, E_ZORER::UI);
    }
}

void HelloWorldScene::removeMap()
{
    while (!_vecMapNode.empty())
    {
        _vecMapNode.back()->removeFromParentAndCleanup(false);
        _vecMapNode.popBack();
    }
}

void HelloWorldScene::recycleMap()
{
    auto spriteMapFront = _vecMapNode.front();
    auto spriteMapBack = _vecMapNode.back();
 
    //
    spriteMapFront->setPositionX(spriteMapBack->getPosition().x + spriteMapBack->getBoundingBox().size.width - 5);
    
    //
    _vecMapNode.erase(0);
    _vecMapNode.pushBack(spriteMapFront);
}

void HelloWorldScene::moveMap()
{
    double nSpeedDefault = C_MOVE_MAP_SPEED;
    double nSpeed = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    nSpeedDefault *= 0.5 / (nSpeed * 4);

    for (auto spriteMap : _vecMapNode)
    {
        spriteMap->setPositionX(spriteMap->getPosition().x - nSpeedDefault);
    }

    for (auto spriteMap : _vecMapNode)
    {
        if ( spriteMap->getPosition().x <= (-spriteMap->getBoundingBox().size.width) )
        {
            recycleMap();
            break;
        }
    }
}

void HelloWorldScene::redrawPetCoolTime(bool bReset)
{
    for ( auto child : _vecPet)
    {
        auto cool = PetNewManager::getInstance()->getPetCoolTime(child->_ePet);
        child->setCoolTime(cool,bReset);
    }
}

void HelloWorldScene::redrawPetCoolTime(E_PET pet,bool bReset)
{
    for(auto child : _vecPet)
    {
        if ( child->_ePet == pet )
        {
            auto cool = PetNewManager::getInstance()->getPetCoolTime(child->_ePet);
            child->setCoolTime(cool,bReset);
            break;
        }
    }
}

void HelloWorldScene::createPet(E_PET pet){

    for(auto child:_vecPet){
        if(child->_ePet == pet){
            return;
        }
    }
    
    if(pet == E_PET::CHICK_1){
        Chick *_pet = Chick::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-65,-5));
        _pet->setZorderNum(10 + 1);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        redrawPetCoolTime(pet,true);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
    else if(pet == E_PET::HAMSTER_2){
        Hamster *_pet = Hamster::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0.5,0));
        _pet->setPosition(Vec2(-60,-10));
        _pet->setZorderNum(10 + 1);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        redrawPetCoolTime(pet,true);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
    else if(pet == E_PET::DRAGON_3){
        Dragon *_pet = Dragon::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-120,10));
        _pet->setZorderNum(10 + 0);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
    else if(pet == E_PET::PRINCESS_4){
        Princess *_pet = Princess::create();
        _pet->setPlace(E_PLACE::DG_NORMAL);
        _pet->_onBuffEffect = CC_CALLBACK_2(UnitBehaviorHero::onPrincessBuff, _hero);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-245,0));
        _pet->setZorderNum(10 + 2);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        redrawPetCoolTime(pet,true);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
    else if(pet == E_PET::PURIRING_5){
        Puriring *_pet = Puriring::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0.5,0));
        _pet->setPosition(Vec2(-105.5,-15));
        _pet->setZorderNum(10 + 2);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
    else if(pet == E_PET::HONG_6){
        Hong *_pet = Hong::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-65,10));
        _pet->setZorderNum(10 + 0);
        _pet->setHero(_hero);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        redrawPetCoolTime(pet,true);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
    else if(pet == E_PET::HANDSOME_8){
        Handsome *_pet = Handsome::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0.5,0));
        _pet->setPosition(Vec2(-110,20));
        _pet->setZorderNum(10 + 0);
        _hero->getLayerCharacter()->addChild(_pet, _pet->getZorderNum());
        _vecPet.pushBack(_pet);
        if(_enemy != nullptr && _enemy->isDie() == false)
        {
            _pet->attack(*_enemy);
        }
        if (3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000) {
            _pet->showRing();
        }
    }
}

void HelloWorldScene::evolutionPet(E_PET pet)
{
    for(auto child:_vecPet)
    {
        if(child->_ePet == pet)
        {
            child->setEvolution();
        }
    }
}

void HelloWorldScene::createHero(){
    
    _hero = UnitBehaviorHero::create(_ePlace);
    _hero->setAttackCallBackListener(CallFunc::create(CC_CALLBACK_0(HelloWorldScene::heroAttack,this)));
    _hero->setMoveCallBackListener(CallFunc::create(CC_CALLBACK_0(HelloWorldScene::heroMoveDone,this)));
    _hero->setUpstairsCallBackListener(CallFunc::create(CC_CALLBACK_0(HelloWorldScene::upstairsDungeon,this)));
    _hero->setAttackCountCallBackListener(CallFunc::create(CC_CALLBACK_0(HelloWorldScene::showBuffRecommend,this)));
    _hero->setMotion();
    _hero->setPositionHero(this, _sizeMapPattern.height);
    addChild(_hero, E_ZORER::HERO);
    
    if ( 3001 <= UserInfoManager::getInstance()->getFloor(_ePlace) && UserInfoManager::getInstance()->getFloor(_ePlace) <= 6000 )
    {
        _hero->setShowRing();
    }
    
    // skill frost
    auto heroSkillFrost = UnitHeroSkillFrost::create();
    heroSkillFrost->setBattleInfo(this);
    heroSkillFrost->setPosition(Vec2(0, _hero->getPosition().y));
    addChild(heroSkillFrost, E_ZORER::HERO);
    
}

void HelloWorldScene::createEnemy(){

    if(_enemy==nullptr){
        _enemy = UnitBehaviorMonster::create(_ePlace);
        addChild(_enemy,E_ZORER::MONSTER);
    }
    _enemy->setMotion();
    _enemy->setMonsterHp("x","x");
    _enemy->setPositionMonster(this, _sizeMapPattern.height);
}


void HelloWorldScene::heroMoveDone(){
    
    _itemBuffRecommend->setVisible(false);
    _hero->heroStopAllAction();
    heroAttack();
}

void HelloWorldScene::onBuffRecommend(cocos2d::Ref* pSender){
    //누르면 동영상 재생
    //몬스터가 죽으면 버프버튼은 삭제 필요.
    _itemBuffRecommend->setVisible(false);
    
    if( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackBuffRecommendAds(ADS_RESULT::SUCCESS);
    }
    else{
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(HelloWorldScene::callbackBuffRecommendAds, this));
    }
}

void HelloWorldScene::showBuffRecommend(){
    Vec2 pos = _itemBuffRecommend->getParent()->convertToNodeSpace(_hero->getPosition());
    pos = Vec2(pos.x-_hero->getLayerCharacter()->getContentSize().width, pos.y+_hero->getLayerCharacter()->getContentSize().height - _itemBuffRecommend->getContentSize().height*1.5);
    _itemBuffRecommend->setPosition(pos);
    _itemBuffRecommend->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _itemBuffRecommend->setVisible(true);
    MafAnalyticsManager::LogEvent(kAnalEventBuffRecommend, kRepeatTrue);
    
    //
    drawReviveButton();
}

void HelloWorldScene::heroAttack()
{
    if ( UserInfoManager::getInstance()->getCurrentPlace() != E_PLACE::DG_NORMAL )
    {
        return;
    }
    
    if(_enemy->isDie() == true)
    {
        int fFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
        int fFloorCount = UserInfoManager::getInstance()->getFloorCount(_ePlace);
        if(fFloorCount==1)
        {
            moveScreenEffect(true);
            
            //
            _hero->actionMoveUpstairs();
            
            //
            for(auto pet : _vecPet)
                pet->move();

            scheduleMove(0);
            schedule(schedule_selector(HelloWorldScene::scheduleMove));
            
            //
            if ( fFloor % 100 == 1 )
            {
                RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_ALL);
                RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_ALL);
                RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_BOX);
                RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EQUIP_CRAFT);
            }
        }
        else
        {
            startDungeon();
        }
        
        for(auto pet : _vecPet)
        {
            if (pet->_ePet == E_PET::PURIRING_5)
            {
                Puriring* puriring = (Puriring*)pet;
                puriring->getKey();
                break;
            }
        }
        
        //
        drawFloor();
    }
    else
    {
        for(auto pet : _vecPet)
        {
            pet->attack(*_enemy);
        }
        
        _hero->actionAttack(*_enemy);
        
        unschedule(schedule_selector(HelloWorldScene::scheduleMove));
    }

}
void HelloWorldScene::showHeroWeaponUpgrade()
{
    auto sprLvUpEffect = Sprite::create("Assets/ui_common/ef_change_1.png");
    sprLvUpEffect->setPosition(-sprLvUpEffect->getContentSize().width*2/3,sprLvUpEffect->getContentSize().height/2);
    sprLvUpEffect->setTag(999);
    _hero->addChild(sprLvUpEffect);



    auto animationEffect = Animation::create();
    animationEffect->setDelayPerUnit(0.05f);
    for (int i = 1; i <= 8; i++)
    {
        animationEffect->addSpriteFrameWithFile(MafUtils::format("Assets/ui_common/ef_change_%d.png",i));
    }
    auto actionEffect1 = Animate::create(animationEffect);

    auto actionEffect2 = CallFunc::create([=](){

        _hero->removeChildByTag(999);
    });

    auto sequenceEffect = Sequence::create(actionEffect1,actionEffect2, NULL);
    sprLvUpEffect->runAction(sequenceEffect);

    
}

void HelloWorldScene::startDungeon(){
    
    createEnemy();
    
    _hero->actionMove();
    _enemy->actionMove();
    for(auto pet : _vecPet)
        pet->move();
    
    //bg move
    scheduleMove(0);
    schedule(schedule_selector(HelloWorldScene::scheduleMove));
}

void HelloWorldScene::upstairsDungeon(){
    
    unschedule(schedule_selector(HelloWorldScene::scheduleMove));
    
    _hero->setPositionHero(this, _sizeMapPattern.height);
    moveScreenEffect(false);
    startDungeon();
    
    drawFloor();
    drawReviveButton();
    
    //
    auto scene = UtilsScene::getInstance()->getRunningScene();
    
    //
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    
    // item point 지급
    int tempFloor = floor-1;
    if ( tempFloor % 1000 == 0 )
    {
        std::string strPointAdd = "2";
        if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_27)){
            strPointAdd = "3";
        }
        
        ItemsMoneyManager::getInstance()->setPoint(std::atoi(strPointAdd.c_str()));
        RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::POINT, strPointAdd);
    }
    
    // story
    if ( utils::findChild(scene, "POPUP_STORY") == nullptr )
    {
        // story floor
        if ( floor >= 50001 && DialogManager::getInstance()->isViewDialog("STORY_MAIN_FLOOR_50000") == false ) {
            DialogManager::getInstance()->showDialog("STORY_MAIN_FLOOR_50000");
        } else if ( floor >= 40001 && DialogManager::getInstance()->isViewDialog("STORY_MAIN_FLOOR_40000") == false ) {
            DialogManager::getInstance()->showDialog("STORY_MAIN_FLOOR_40000");
        } else if ( floor >= 30001 && DialogManager::getInstance()->isViewDialog("STORY_MAIN_FLOOR_30000") == false ) {
            DialogManager::getInstance()->showDialog("STORY_MAIN_FLOOR_30000");
        } else if ( floor >= 20001 && DialogManager::getInstance()->isViewDialog("STORY_MAIN_FLOOR_20000") == false ) {
            DialogManager::getInstance()->showDialog("STORY_MAIN_FLOOR_20000");
        } else if ( floor >= 14001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_14000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_14000);
        } else if ( floor >= 12001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_12000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_12000);
        } else if ( floor >= 10001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_10000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_10000);
        } else if ( floor == 9001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_9000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_9000);
        } else if ( floor == 8001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_8000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_8000);
        } else if ( floor >= 7601 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_7600) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_7600);
        } else if ( floor >= 7001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_7000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_7000);
        } else if ( floor >= 6601 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_6600) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_6600);
        } else if ( floor >= 6501 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_6500) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_6500);
        } else if ( floor >= 6001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_6000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_6000);
        } else if ( floor >= 5601 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_5600) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_5600);
        } else if ( floor >= 5001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_5000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_5000);
        } else if ( floor >= 4501 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_4500) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_4500);
        } else if ( floor >= 4001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_4000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_4000);
        } else if ( floor >= 3001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_3000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_3000);
        } else if ( floor >= 2501 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_2500) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_2500);
        } else if ( floor >= 2201 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_2200) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_2200);
        } else if ( floor >= 2001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_2000) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_2000);
        } else if ( floor >= 1701 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_1700) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_1700);
        } else if ( floor >= 1401 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_1400) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_1400);
        } else if ( floor >= 1201 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_1200) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_1200);
        } else if ( floor >= 1101 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_1100) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_1100);
        } else if ( floor >= 1001 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_1000) == false ){
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_1000);
        } else if ( floor >= 901 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_900) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_900);
        } else if ( floor >= 778 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_777) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_777);
        } else if ( floor >= 601 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_600) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_600);
        } else if ( floor >= 301 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_300) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_300);
        } else if ( floor >= 151 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_150) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_150);
        } else if ( floor == 101 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_100) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_100);
        } else if ( floor >= 11 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::CLEAR_10) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_10);
        }
        
           
        
        // story quest
        if( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,E_QUEST::QUEST_7) > 0 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::QUEST_OPEN_7) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::QUEST_OPEN_7);
        } else if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,E_QUEST::QUEST_11) > 0 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::QUEST_OPEN_11) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::QUEST_OPEN_11);
        } else if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,E_QUEST::QUEST_24) > 0 && StoryManager::getInstance()->isStoryOpen((int)E_COMIC::QUEST_OPEN_24) == false ) {
            StoryManager::getInstance()->showStory((int)E_COMIC::QUEST_OPEN_24);
        }
    }
    
    // pet create
    {
        if ( floor == 6501 && PetNewManager::getInstance()->getPet(E_PET::HANDSOME_8) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::HANDSOME_8);
        else if ( floor == 4501 && PetNewManager::getInstance()->getPet(E_PET::HONG_6) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::HONG_6);
        else if ( floor == 1101 && PetNewManager::getInstance()->getPet(E_PET::PURIRING_5) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::PURIRING_5);
        else if ( floor == 901 && PetNewManager::getInstance()->getPet(E_PET::PRINCESS_4) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::PRINCESS_4);
        else if ( floor == 601 && PetNewManager::getInstance()->getPet(E_PET::DRAGON_3) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::DRAGON_3);
        else if ( floor == 301 && PetNewManager::getInstance()->getPet(E_PET::HAMSTER_2) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::HAMSTER_2);
        else if ( floor == 101 && PetNewManager::getInstance()->getPet(E_PET::CHICK_1) == false )
            PetNewManager::getInstance()->onPetFree(E_PET::CHICK_1);
    }
    
    // relic
    if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_1) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_1) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_1, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_2) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_2) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_2, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_3) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_3) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_3, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_4) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_4) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_4, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_5) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_5) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_5, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_6) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_6) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_6, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_7) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_7) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_7, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_8) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_8) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_8, 1);
    } else if ( floor == DataManager::GetRelicOpenFloor(E_RELIC::RELIC_9) && UserInfoManager::getInstance()->getRelic(_ePlace, E_RELIC::RELIC_9) == false ) {
        UserInfoManager::getInstance()->setRelicLevel(_ePlace, E_RELIC::RELIC_9, 1);
    }
    
    // draw
    {
        if ( floor == 1001 )
        {
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_13) )
            {
                RefreshManager::getInstance()->refreshUI(E_REFRESH::QUEST_INFO);
            }
        }
    }
      
    
    
    //
    if (floor == 3001) {
        _hero->setShowRing();
        for(auto pet : _vecPet){
            pet->showRing();
        }
    }else if (floor == 6001) {
        _hero->setHideRing();
        for(auto pet : _vecPet){
            pet->hideRing();
        }
    }

    /*맨밑은 업적관련된 것들.*/
    int highFloor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    if ( highFloor < floor )
    {
        UserInfoManager::getInstance()->setHighFloor(_ePlace,floor);
        MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_HIGH, floor);
        
        auto info = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::HIGH_FLOOR);
        bool isRefreshRank = UserDefault::getInstance()->getBoolForKey(KEY_REFRESH_RANK, true);
        
        if(info != nullptr && isRefreshRank == true)
        {
            if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) - info->getScore() >= ParameterManager::getInstance()->getParameterInteger("high_floor_rank_interval"))
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
        TimingPackageManager::getInstance()->invokeEventTimingPackage();
        
        //펫 진화 체크
        for (int i=1; i<=C_COUNT_PET; i++)
        {
            E_PET ePet = (E_PET)i;
            
            PetNewManager::getInstance()->onPetEvolution(ePet);
        }
        
        if(UserInfoManager::getInstance()->getHighFloor(_ePlace) == 10000)
        {
            auto layerDungeon = dynamic_cast<LayerNormalDungeon*>(_uiContainerDungeon);
            if(layerDungeon != nullptr)
                layerDungeon->updateDungeonList();
        }
    }
    
    int total = UserInfoManager::getInstance()->getTotalFloor();
    total++;
    UserInfoManager::getInstance()->setTotalFloor(total);
    
    auto before = UserInfoConfigManager::getInstance()->getNewbieStep();
    E_NEWBIE_STEP eNewbie = DataManager::GetNewbieStep(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL), UserInfoManager::getInstance()->getTotalFloor(),SpecialMissionManager::getInstance()->getSpecialMissionClear() );
    UserInfoConfigManager::getInstance()->setNewbieStep(eNewbie);
    
    // 뉴비 보물 다시 계산
    if(UserInfoConfigManager::getInstance()->getNewbieStep() >= E_NEWBIE_STEP::STEP_1 && UserInfoConfigManager::getInstance()->getNewbieStep() <= E_NEWBIE_STEP::STEP_5)
    {
        if(before != UserInfoConfigManager::getInstance()->getNewbieStep()){
            DataManager::initArtifactSaveData(E_PLACE::DG_NORMAL);
                    
            if(eNewbie > E_NEWBIE_STEP::STEP_NONE && eNewbie < E_NEWBIE_STEP::STEP_FINISH)
            {
                reDrawBuffTimer();

                auto spriteIcon = Sprite::create(MafUtils::format("Assets/ui_common/icon_newbie_%d.png",(int)eNewbie));
                spriteIcon->setPosition(_layerTopWidget->getContentSize().width*0.5 - _itemBuffNewbie->getContentSize().width*1.5 ,
                                       _layerTopWidget->getContentSize().height*0.3 + _itemBuffNewbie->getContentSize().height*1.5);
                spriteIcon->setScale(3);
                spriteIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                _layerTopWidget->addChild(spriteIcon, 100);
                

                auto action1 = DelayTime::create(5.0);
                auto action2 = CallFuncN::create([=](Node* sender){
                
                    auto vec1 = _itemBuffNewbie->getParent()->convertToWorldSpace(_itemBuffNewbie->getPosition());
                    auto vec2 = _layerTopWidget->convertToNodeSpace(vec1);

                    Spawn *spawn = Spawn::create(MoveTo::create(0.5f, vec2), ScaleTo::create(0.4f, 1), nullptr);
                    auto seq = Sequence::create(spawn, RemoveSelf::create(), nullptr);
                    sender->runAction(seq);
                });
                auto seq = Sequence::create(action1, action2, NULL);
                spriteIcon->runAction(seq);
            }
        }
    }
    
    if(floor == 25 || floor == 50 || floor == 75 || floor == 750)
        stepwiseContentsOpen(false, E_STEPWISE_TYPE::FLOOR);
    else if(floor == 100){
        stepwiseContentsOpen(false, E_STEPWISE_TYPE::FLOOR);
        bool bAllPass = true;
        if(UserInfoManager::getInstance()->getQuest(E_PLACE::DG_NORMAL,E_QUEST::QUEST_1))
            bAllPass = false;
        for(int i=2;i<=C_COUNT_QUEST;i++){
            bool bOpen = UserInfoManager::getInstance()->getQuest(E_PLACE::DG_NORMAL,(E_QUEST)i);
            if(bOpen){
                bAllPass = false;
                break;
            }
        }
        if(bAllPass)
            MafGooglePlay::UnlockAchievement(ACHIEVE_100_NOQUEST, 1, 1);
    }
    else if(floor == 120)
        stepwiseContentsOpen(false, E_STEPWISE_TYPE::FLOOR);
    else if(floor==200)
        MafGooglePlay::UnlockAchievement(ACHIEVE_200_NOREVIVE, 1, 1); //200층에서 그냥열어준다.
    else if(floor==300)
        MafGooglePlay::UnlockAchievement(ACHIEVE_300_FLOOR, 1, 1);
    else if(floor==500)
        MafGooglePlay::UnlockAchievement(ACHIEVE_500_FLOOR, 1, 1);
    else if(floor==700)
        MafGooglePlay::UnlockAchievement(ACHIEVE_700_FLOOR, 1, 1);
    else if(floor==1000)
        MafGooglePlay::UnlockAchievement(ACHIEVE_1000_FLOOR, 1, 1);
    else if(floor==1200)
        MafGooglePlay::UnlockAchievement(ACHIEVE_1200_FLOOR, 1, 1);
    else if(floor==1400)
        MafGooglePlay::UnlockAchievement(ACHIEVE_1400_FLOOR, 1, 1);
    else if(floor==1600)
        MafGooglePlay::UnlockAchievement(ACHIEVE_1600_FLOOR, 1, 1);
    else if(floor==1800)
        MafGooglePlay::UnlockAchievement(ACHIEVE_1800_FLOOR, 1, 1);
    else if(floor==2000)
        MafGooglePlay::UnlockAchievement(ACHIEVE_2000_FLOOR, 1, 1);
    else if(floor==2500)
        MafGooglePlay::UnlockAchievement(ACHIEVE_2500_FLOOR, 1, 1);
    else if(floor==3000)
        MafGooglePlay::UnlockAchievement(ACHIEVE_3000_FLOOR, 1, 1);
    
    highFloor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    if(highFloor == 10 || highFloor == 25 || highFloor == 40 || highFloor == 50||
       highFloor == 60 || highFloor == 75 || highFloor == 100 || highFloor == 120 ||
       highFloor == 150 || highFloor == 200)
    {
        //std::string logContent = kAnalEventDungeon + MafUtils::ChangeLPAD(highFloor, 4);
        std::string logContent = MafUtils::format(kAnalEventDungeon, highFloor);
        MafAnalyticsManager::LogEvent(logContent.c_str(), kRepeatFalse);
    }
    else if (highFloor > 250 && highFloor <= 2000)
    {
        if(highFloor % 50 == 0)
        {
            std::string logContent = MafUtils::format(kAnalEventDungeon, highFloor);
            MafAnalyticsManager::LogEvent(logContent.c_str(), kRepeatFalse);
        }
    }
    
}

void HelloWorldScene::moveScreenEffect(bool turnOn)
{
    if ( turnOn )
    {
        int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
        if(floor%100==0)
            SoundManager::SoundBgmStart(SOUND_BGM_BOSS, true);
        else if(floor>=1001)
            SoundManager::SoundBgmStart(SOUND_BGM_HELL, true);
        else
            SoundManager::SoundBgmStart(SOUND_BGM, true);
        
        //
        int nMapIdxTemp = _nMapIdx;
        int nMapTypeTemp = _nMapType;
        createMapIdx();
        
        if ( nMapIdxTemp != _nMapIdx || nMapTypeTemp != _nMapType )
        {
            auto action = FadeIn::create(0.5);
            _sprDarkScreen->runAction(action);
        }
        
        _nMapIdx = nMapIdxTemp;
        _nMapType = nMapTypeTemp;
    }
    else
    {
        int nMapIdxTemp = _nMapIdx;
        int nMapTypeTemp = _nMapType;
        createMapIdx();
        
        if ( nMapIdxTemp != _nMapIdx || nMapTypeTemp != _nMapType )
        {
            removeMap();
            createMap();
            
            auto action = FadeOut::create(0.5);
            _sprDarkScreen->runAction(action);
        }
    }
}

#pragma mark -
void HelloWorldScene::checkTutorial()
{
    int revivecount = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
    int questlevel =  UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,E_QUEST::QUEST_1);
    
    TutorialManager* tutorial = TutorialManager::getInstance();
    if(revivecount == 1 && questlevel < 5)
    {
        tutorial->startTutorial();
    }
    else
    {
        tutorial->loadTutorialProgress();
    }
    
}
#pragma mark-

void HelloWorldScene::ResultRevivalAdsCallBack(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotRevival, CashManager::getInstance()->isAdsfreeTime());
        
        // 환생 광고 시청 카운트 수집
        int nCountAds = UserInfoManager::getInstance()->getRevivalAds(E_PLACE::DG_NORMAL) + 1;
        UserInfoManager::getInstance()->setRevivalAds(E_PLACE::DG_NORMAL, nCountAds);
        
        //
        onPrestige();
    }
    else
    {
        if(result == ADS_RESULT::LOAD)
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(HelloWorldScene::ResultRevivalAdsCallBack, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotRevival);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

void HelloWorldScene::onPrestige()
{
    SoundManager::SoundEffectStart(SOUND_SELECT);

    //
    if( _iPrestigeSpecial == 1 && _bRevivalFree == false)
    {
        if(ItemsMoneyManager::getInstance()->getGem() < C_PRESTIGE_GEM)
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
            return;
        }
    }
    else if( _iPrestigeSpecial == 2 )
    {
        if(ItemsMoneyManager::getInstance()->getGem() < C_PRESTIGE_GEM2)
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
            return;
        }
    }
    PopupLoading::show();
    
    // mission
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::REVIVE, 1);
    
    // event mission
    if ( EventManager::getInstance()->isActivatedEvent(30) == true )
    {
        Event7thManager::getInstance()->requestUpdate(1, nullptr);
    }
    
    //
    UserInfoStackManager::getInstance()->setReviveSpecial(_iPrestigeSpecial);
    
    _hero->heroStopAllAction();//stopAllActions();
    auto children = _hero->getChildren();
    for(auto child : children)
        child->setVisible(false);
    
    if(_enemy !=nullptr)
        _enemy->setVisible(false);
    
    for(auto pet : _vecPet){
        pet->unscheduleUpdate();
        pet->stopAllActions();
    }
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    //walk body
    _vec.clear();
    for(int i=1; i<=5; i++){
        std::stringstream name;
        name << "Assets/character_hero/normal/effect/ef_formerlife_" << i << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(HelloWorldScene::reStart,this));
//    Sequence *seq = Sequence::create(DelayTime::create(0.5f),animate,Hide::create(),call, nullptr);
    
    auto spr = Sprite::create();
    spr->setAnchorPoint(Vec2(1,0));
    spr->setPosition(Vec2(0,0));
    _hero->addChild(spr);
    Sequence *seq = Sequence::create(animate,call,RemoveSelf::create(), nullptr);
    spr->runAction(seq);

    
    SoundManager::SoundEffectStart(SOUND_REVIVAL);
}

void HelloWorldScene::reStart()
{
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    auto addKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, floor);
    
    if ( _bRevivalFree == true )
    {// 무료 강화환생시 환생광고 수집 카운트 초기화
        UserInfoManager::getInstance()->setRevivalAds(E_PLACE::DG_NORMAL, 0);
    }
    
    // 봉신강림
    if (UserInfoManager::getInstance()->getBuffTime(BUFF_HERO_BONUS) > 0){
        UserInfoManager::getInstance()->setBuffTime(BUFF_HERO_BONUS, UserInfoManager::getInstance()->getBuffTime(BUFF_HERO_BONUS) * -1);
    }
    
    // 피버타임
    int feverDay = UserInfoManager::getInstance()->getFeverDay();
    if (feverDay > 0){
        int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_52);
        int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_52);
        float time = C_MAX_FEVER_TIME + DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_52, level, reinforce);

        UserInfoManager::getInstance()->setFeverTime(time);
        UserInfoManager::getInstance()->setFeverDay(feverDay - 1);
    }
    else{
        UserInfoManager::getInstance()->setFeverTime(0);
    }
    
    // 5배 버프
    time_t nBuffTimeDiff = UserInfoManager::getInstance()->getBuffRevival() - UtilsDate::getInstance()->getTime();
    
    
    // 큐브
    int cubeDay = UserInfoManager::getInstance()->getCubeDay();
    int cubeGain = UserInfoManager::getInstance()->getCubeTotalGain();
    if ( cubeDay > 0 && cubeGain > 0 )
    {
        if ( _bRevivalAds == true )
        {
            cubeGain *= 2;
        }
        
        if ( nBuffTimeDiff > 0 )
        {// 5배 버프
            cubeGain *= 5;
        }
        
        UserInfoManager::getInstance()->setCubeDay(cubeDay - 1);
        ItemsMoneyManager::getInstance()->setCube(cubeGain);
    }
    
    
    if (UserInfoManager::getInstance()->getBuffTime(BUFF_KEY_BONUS) > 0)
    {
        UserInfoManager::getInstance()->setBuffTime(BUFF_KEY_BONUS,-1);
        UserInfoStackManager::getInstance()->setReviveKeyBooster(1);
    }
    else{
        UserInfoStackManager::getInstance()->setReviveKeyBooster(0);
    }
    reDrawBuffTimer();
    
    if (_bRevivalAds)
    { /*10%추가*/
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_14))
        {
            auto strTen = addKey; //30% 추가
            strTen.pop_back();
            strTen = MafUtils::bigMulNum(strTen, "3");
            addKey = MafUtils::bigAddNum(addKey, strTen);
        }
        else{
            auto strTen = addKey;
            strTen.pop_back();
            addKey = MafUtils::bigAddNum(addKey, strTen);
        }
    }
    
    if(_iPrestigeSpecial == 1 || _iPrestigeSpecial == 2)
    {
        UserInfoStackManager::getInstance()->setSetItemEffectIdx25Count(UserInfoStackManager::getInstance()->getSetItemEffectIdx25Count()+1);
        
        if (_iPrestigeSpecial == 1)
        {// 강화 환생
            addKey = MafUtils::bigMulNum(addKey, "3");
            
            if ( _bRevivalFree == false )
            {
                ItemsMoneyManager::getInstance()->setGem(-C_PRESTIGE_GEM);
            }
            
            if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_15))
            {
                ItemsMoneyManager::getInstance()->setGem(100);
            }
        }
        else
        {// 초강화 환생
            addKey = MafUtils::bigMulNum(addKey, "5");
            ItemsMoneyManager::getInstance()->setGem(-C_PRESTIGE_GEM2);
            
            if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_15))
            {
                ItemsMoneyManager::getInstance()->setGem(200);
            }
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_27)) {
            auto tt = MafUtils::bigMulNum(addKey, MafUtils::toString(2));
            tt.pop_back();
            addKey = MafUtils::bigAddNum(addKey, tt);
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_6)) {
            auto tt = MafUtils::bigMulNum(addKey, MafUtils::toString(2));
            tt.pop_back();
            addKey = MafUtils::bigAddNum(addKey, tt);
        }
        
        // VIP 불꽃
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_32) ){
            int gain = 0;
            switch (_iPrestigeSpecial) {
                case 1: gain = 5; break;
                case 2: gain = 10; break;
            }
            ItemsMoneyManager::getInstance()->setPoint(gain);
        }
    }
    
    if ( nBuffTimeDiff > 0 )
    {// 5배 버프
        addKey = MafUtils::bigMulNum(addKey, "5");
    }
    
    addKey = MafUtils::bigMulNum(addKey, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(502, 1) * 100));
    addKey.pop_back();
    addKey.pop_back();
    
    //모험 유물 적용
    double advtRelicBuff = 100.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::REVIVE_KEY_INCREASE);
       
    addKey = MafUtils::bigMulNum(addKey, MafUtils::doubleToString(advtRelicBuff));
    addKey.pop_back();
    addKey.pop_back();
    
    
    // 만능 보관함
    int levelPoint = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_50);
    int levelPointAdd = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_51);
    
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_50);
    int reinforceAdd = UserInfoManager::getInstance()->getArtifactReinforce(_ePlace, E_ARTIFACT::ARTI_51);
    
    if ( levelPoint != 0 )
    {
        int gain = UserInfoManager::getInstance()->getFloor(_ePlace) / DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_50, levelPoint, reinforce);
        int gainToday = UserInfoManager::getInstance()->getArtifactPointGain(_ePlace);
        int gainTodayMax = C_MAX_PRESTIGE_POINT + DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_51, levelPointAdd, reinforceAdd);
        
        //모험 유물 적용
        double advtRelicBuff = AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::REVIVE_POINT_INCREASE);
        int gainBonus = gainTodayMax * advtRelicBuff * 0.01;
        
        gainTodayMax += gainBonus;
        
        gainTodayMax = gainTodayMax - gainToday;
        if ( gain > gainTodayMax )
            gain = gainTodayMax;
        
        gain = MAX(gain, 0);
        
        ItemsMoneyManager::getInstance()->setPoint(gain);
        UserInfoManager::getInstance()->setArtifactPointGain(_ePlace, gain + gainToday);
    }
    
    ItemsMoneyManager::getInstance()->setKey(addKey);
    auto data = SaveManager::getHistoryData(E_HISTORY_DATA::K_REBORN_KEY);
    auto compare = MafUtils::compareStr(addKey, data);
    if(compare){
        SaveManager::setHistoryData(E_HISTORY_DATA::K_REBORN_KEY, addKey);
    }
    
    
    std::string coin = ItemsMoneyManager::getInstance()->getGold();
    if (coin.length() > 2) {
        coin.pop_back();
        coin.pop_back();
        
        int maxcount = 0;
        if (TownManager::getInstance()->getTownLevel(BUILDING_2) >= TownManager::getInstance()->getTownLevelMax(E_TOWN::BUILDING_2))
            maxcount++;
        if (TownManager::getInstance()->getTownLevel(BUILDING_3) >= TownManager::getInstance()->getTownLevelMax(E_TOWN::BUILDING_3))
            maxcount++;
        if (TownManager::getInstance()->getTownLevel(BUILDING_4) >= TownManager::getInstance()->getTownLevelMax(E_TOWN::BUILDING_4))
            maxcount++;
        if (TownManager::getInstance()->getTownLevel(BUILDING_5) >= TownManager::getInstance()->getTownLevelMax(E_TOWN::BUILDING_5))
            maxcount++;
        
        if (maxcount == 0) {
            coin = MafUtils::bigMulNum(coin, MafUtils::toString(25));
        }else if (maxcount == 1) {
            coin = MafUtils::bigMulNum(coin, MafUtils::toString(33));
        }else if (maxcount == 2) {
            coin = MafUtils::bigMulNum(coin, MafUtils::toString(50));
        }else if (maxcount == 3) {
            coin.append("00");
        }
        
        TownManager::getInstance()->onTownUpgrade(BUILDING_2, coin);
        TownManager::getInstance()->onTownUpgrade(BUILDING_3, coin);
        TownManager::getInstance()->onTownUpgrade(BUILDING_4, coin);
        TownManager::getInstance()->onTownUpgrade(BUILDING_5, coin);
    }
    
    if (UserInfoStackManager::getInstance()->getPrincessEvolutionActive(_ePlace) && floor >= 2001)
    {
        int count = (floor-1) / 2000;
        int resetFloor = count * 2000 - (_nRevivalFloorSub * 2000 + 999);
        if ( resetFloor < 1 )
            resetFloor = 1;
        
        //모험 유물 적용
        resetFloor += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::PRINCESSES_SKILL_FLOOR_INCREASE);
        
        UserInfoManager::getInstance()->resetNormal(resetFloor);
        UserInfoManager::getInstance()->setFloor(_ePlace,resetFloor,true);
    }
    else
    {
        UserInfoManager::getInstance()->resetNormal(1);
        UserInfoManager::getInstance()->setFloor(_ePlace,1,true);
    }

    UserDefault::getInstance()->setStringForKey(KEY_MONSTER_HP,"0");
    UserInfoManager::getInstance()->setFloorCount(_ePlace,1);
    UserInfoStackManager::getInstance()->setQuest25Buff(0);
    
    SaveManager::saveAllData();
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_5)){
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS,60);
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_11)) {
        UserInfoStackManager::getInstance()->setRevivalAttackBuff(UserInfoStackManager::getInstance()->getRevivalAttackBuff()+1);
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_15)) {
        UserInfoStackManager::getInstance()->setRevivalQuestBuff(UserInfoStackManager::getInstance()->getRevivalQuestBuff()+1);
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_17)) {
        UserInfoStackManager::getInstance()->setRevivalKeyBuff1(UserInfoStackManager::getInstance()->getRevivalKeyBuff1()+1);
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_35)) {
        UserInfoStackManager::getInstance()->setRevivalKeyBuff2(UserInfoStackManager::getInstance()->getRevivalKeyBuff2()+1);
    }
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_12)) {
        UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_CRI, 2);
        
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_12)) {
        UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_KILL, 2);
    }
    
    if(_iPrestigeSpecial == 1 || _iPrestigeSpecial == 2){
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_6)) {
            UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_ATK, 15);
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_10)) {
            UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_CRI, 15);
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_6)) {
            UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, 10);
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_10)) {
            UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_KILL, 15);
        }
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_15)) {
            UserInfoStackManager::getInstance()->setEnchantRevivalAttackBuff(UserInfoStackManager::getInstance()->getEnchantRevivalAttackBuff()+1);
        }
    }
    
    int revival = UserInfoManager::getInstance()->getRevival(_ePlace);
    revival++;
    UserInfoManager::getInstance()->setRevival(_ePlace,revival);
    
    if(_iPrestigeSpecial == 1){
        auto data = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_REVIVAL_2);
        SaveManager::setHistoryData(E_HISTORY_DATA::K_REVIVAL_2, ++data);
    }
    else if(_iPrestigeSpecial == 2){
        auto data = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_REVIVAL_3);
        SaveManager::setHistoryData(E_HISTORY_DATA::K_REVIVAL_3, ++data);
    }
    
    
    // 환생 후 퀘스트 버튼 이페트 효과 유무
    bool bQuestButtonEffect = true;
    for ( int i = 1; i <= C_COUNT_QUEST; i++ )
    {
        int nLevel = UserInfoManager::getInstance()->getQuestLevel(_ePlace, (E_QUEST)i);
        if ( nLevel > 0 )
        {
            bQuestButtonEffect = false;
            break;
        }
    }
    UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_QUEST, bQuestButtonEffect);
    
    
    
    MafGooglePlay::IncrementAchievement(ACHIEVE_10_REVIVE, 1, 10);
    MafGooglePlay::IncrementAchievement(ACHIEVE_20_REVIVE, 1, 20);
    MafGooglePlay::IncrementAchievement(ACHIEVE_50_REVIVE, 1, 50);
    MafGooglePlay::IncrementImmediateAchievement(ACHIEVE_1000_REVIVE, revival, 1000);
    MafGooglePlay::IncrementImmediateAchievement(ACHIEVE_5000_REVIVE, revival, 5000);
    if(floor==100){
        MafGooglePlay::UnlockAchievement(ACHIEVE_100_REVIVE, 1, 1);
    }
    


    
    PopupLoading::hide();
    UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
    
    // 로그 이벤트
    MafAnalyticsManager::LogEvent(kAnalEventRivival, kRepeatTrue);
    if ( _iPrestigeSpecial == 0 )
    {
        MafAnalyticsManager::LogEvent(kAnalEventRivivalX1, kRepeatTrue);
    }
    else if ( _iPrestigeSpecial == 1 )
    {
        if ( _bRevivalFree == true )
        {
            MafAnalyticsManager::LogEvent(kAnalEventRivivalX3Free, kRepeatTrue);
        }
        else
        {
            MafAnalyticsManager::LogEvent(kAnalEventRivivalX3, kRepeatTrue);
        }
    }
    else if ( _iPrestigeSpecial == 2 )
    {
        MafAnalyticsManager::LogEvent(kAnalEventRivivalX5, kRepeatTrue);
    }
    
    if ( UserInfoStackManager::getInstance()->getReviveKeyBooster() == 1 )
    {
        MafAnalyticsManager::LogEvent(kAnalEventRivivalKeyboost, kRepeatTrue);
    }
    
    bool bAutoSave = UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_REVIVE_NORMAL", false);
    
    if(bAutoSave == true)
    {
        showAutoSaveToast();
    }
}

#pragma mark-
void HelloWorldScene::showFeverTime()
{
    if ( _layerFever != nullptr )
        return;
    
    if ( isScheduled(schedule_selector(HelloWorldScene::scheduleFever)) == false )
    {
        schedule(schedule_selector(HelloWorldScene::scheduleFever), 0.1, kRepeatForever, 0);
    }
    
    _layerFever = LayerColor::create(Color4B::GREEN, UtilsRect::getVisibleSize().width, _sizeMapPattern.height);
    _layerFever->setIgnoreAnchorPointForPosition(false);
    _layerFever->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    _layerFever->setPosition( Vec2(0, UtilsRect::getVisibleSize().height) );
    _layerFever->setCascadeOpacityEnabled(true);
    addChild(_layerFever, E_ZORER::UI);
    
    // 피버 배경 애니
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 8; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/fever/fever_run_%d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    auto spriteBg = Sprite::create("Assets/ui/fever/fever_run_1.png");
    spriteBg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    spriteBg->setPosition( Vec2(_layerFever->getContentSize().width / 2, 0) );
    spriteBg->runAction(RepeatForever::create(Animate::create(animation)));
    _layerFever->addChild(spriteBg);
    
    float scale = MAX(_layerFever->getContentSize().width / spriteBg->getContentSize().width, _layerFever->getContentSize().height / spriteBg->getContentSize().height);
    spriteBg->setScale(scale);
    
    // 피버 타이머
    auto spriteTimerBg = Sprite::create("Assets/ui/fever/fever_bar_1.png");
    spriteTimerBg->setPosition(_layerFever->getContentSize().width / 2, 500);
    _layerFever->addChild(spriteTimerBg);
    
    auto timer = ProgressTimer::create(Sprite::create("Assets/ui/fever/fever_bar_2.png"));
    timer->setPosition( Vec2(spriteTimerBg->getContentSize().width / 2, spriteTimerBg->getContentSize().height / 2) );
    timer->setType(ProgressTimer::Type::BAR);
    timer->setPercentage(100);
    timer->setMidpoint( Vec2::ANCHOR_MIDDLE_LEFT );
    timer->setBarChangeRate( Vec2(1,0) );
    spriteTimerBg->addChild(timer);
    
    auto actionTimer = ProgressFromTo::create(UserInfoManager::getInstance()->getFeverTime(), timer->getPercentage(), 0);
    auto actionCall = CallFunc::create(CC_CALLBACK_0(HelloWorldScene::hideFeverTIme,this));
    auto seq = Sequence::create(actionTimer, actionCall, nullptr);
    timer->runAction(seq);
    
    auto spriteTimerText = Sprite::create("Assets/ui/fever/fever_bar_3.png");
    spriteTimerText->setPosition( Vec2(spriteTimerBg->getContentSize().width / 2, spriteTimerBg->getContentSize().height / 2) );
    spriteTimerBg->addChild(spriteTimerText);
}

void HelloWorldScene::hideFeverTIme()
{
    if ( _layerFever == nullptr )
        return;
    
    // schedule
    unschedule(schedule_selector(HelloWorldScene::scheduleFever));
    
    // data
    UserInfoManager::getInstance()->setFeverTime(0);
    
    // ui
    auto action1 = FadeOut::create(0.5f);
    auto action2 = CallFunc::create([=](){
        _layerFever->removeFromParent();
        _layerFever = nullptr;
    });
    auto seq = Sequence::create(action1, action2, nullptr);
    _layerFever->runAction(seq);
    
    if(UserInfoManager::getInstance()->getItemFeverTime() > 0)
    {
        showItemFeverTime();
    }
}

void HelloWorldScene::showItemFeverTime()
{
    if ( isScheduled(schedule_selector(HelloWorldScene::scheduleItemFever)) == false )
    {
        schedule(schedule_selector(HelloWorldScene::scheduleItemFever), 0.1, kRepeatForever, 0);
    }
    
    if ( _layerItemFever == nullptr )
    {
        _layerItemFever = Layer::create();
        _layerItemFever->setContentSize(Size(UtilsRect::getVisibleSize().width, _sizeMapPattern.height));
        _layerItemFever->setIgnoreAnchorPointForPosition(false);
        _layerItemFever->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        _layerItemFever->setPosition( Vec2(0, UtilsRect::getVisibleSize().height) );
        _layerItemFever->setCascadeOpacityEnabled(true);
        addChild(_layerItemFever, E_ZORER::UI);
    }
    _layerItemFever->setOpacity(255);
    _layerItemFever->removeAllChildren();
    
    _hero->showItemFeverFx();
    
    auto spriteBg = Sprite::create("Assets/ui/fever/bg_fever1_1.png");
    spriteBg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    spriteBg->setPosition( Vec2(_layerItemFever->getContentSize().width / 2, 0) );
    spriteBg->setScale(1.1f);
    spriteBg->getTexture()->setTexParameters(texParams);
    spriteBg->setColor(Color3B(0,255,255));
    _layerItemFever->addChild(spriteBg);
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 6; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/fever/bg_fever1_%d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    
    auto callFunc = CallFunc::create([&, spriteBg]()
    {
        spriteBg->runAction(TintTo::create(0.3, random(0, 255), random(0, 255), random(0, 255)));
    });
    
    spriteBg->runAction(RepeatForever::create(Animate::create(animation)));
    spriteBg->runAction(RepeatForever::create(Sequence::create(callFunc, DelayTime::create(0.3), nullptr)));
    
    // 피버 타이머
    auto spriteTimerBg = Sprite::create("Assets/ui/fever/fever_bar_1.png");
    spriteTimerBg->setPosition(_layerItemFever->getContentSize().width / 2, 500);
    _layerItemFever->addChild(spriteTimerBg);
    
    auto timer = ProgressTimer::create(Sprite::create("Assets/ui/fever/fever_bar_2.png"));
    timer->setPosition( Vec2(spriteTimerBg->getContentSize().width / 2, spriteTimerBg->getContentSize().height / 2) );
    timer->setType(ProgressTimer::Type::BAR);
    timer->setPercentage(100);
    timer->setMidpoint( Vec2::ANCHOR_MIDDLE_LEFT );
    timer->setBarChangeRate( Vec2(1,0) );
    spriteTimerBg->addChild(timer);
    
    auto actionTimer = ProgressFromTo::create(UserInfoManager::getInstance()->getItemFeverTime(), timer->getPercentage(), 0);
    auto actionCall = CallFunc::create(CC_CALLBACK_0(HelloWorldScene::hideItemFeverTime,this));
    auto seq = Sequence::create(actionTimer, actionCall, nullptr);
    timer->runAction(seq);
    
    auto spriteTimerText = Sprite::create("Assets/ui/fever/fever_bar_3.png");
    spriteTimerText->setPosition( Vec2(spriteTimerBg->getContentSize().width / 2, spriteTimerBg->getContentSize().height / 2) );
    spriteTimerBg->addChild(spriteTimerText);
    
}

void HelloWorldScene::hideItemFeverTime()
{
    if ( _layerItemFever == nullptr )
        return;
    
    // schedule
    unschedule(schedule_selector(HelloWorldScene::scheduleItemFever));
    
    // data
    UserInfoManager::getInstance()->setItemFeverTime(0);
    _hero->hideItemFeverFx();
    // ui
    auto action1 = FadeOut::create(0.5f);
    auto action2 = CallFunc::create([=](){
        _layerItemFever->removeAllChildren();
    });
    auto seq = Sequence::create(action1, action2, nullptr);
    _layerItemFever->runAction(seq);
}

void HelloWorldScene::showSaveWarning()
{
    if ( _layerSaveWarning != nullptr )
    {
        return;
    }
    
    
    int diff = UserInfoConfigManager::getInstance()->getSaveTimeDiff();
    if ( diff <= 0 )
    {
        return;
    }
    
    
    _layerSaveWarning = LayerColor::create();
    addChild(_layerSaveWarning, E_ZORER::POPUP);
    {
        auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_1.png", Rect::ZERO, Rect(77,23,2,2));
        spriteBG->setContentSize(Size(630, 148));
        spriteBG->setPosition(getContentSize().width / 2, getContentSize().height*0.75);
        _layerSaveWarning->addChild(spriteBG);
        
        //
        auto spriteIcon = Sprite::create("Assets/ui_common/icon_data_save1.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteIcon->setPosition(120 - 10, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteIcon);
        
        //
        if ( diff >= 30 )       diff = 30;
        else if ( diff >= 15 )  diff = 15;
        else if ( diff >= 7 )   diff = 7;
        else if ( diff >= 3 )   diff = 3;
        else                    diff = 1;
        std::string strMsg = GAME_TEXTFORMAT("t_ui_dungeon_msg_17", diff);
        
        auto labelMsg = Label::createWithTTF(strMsg, GAME_FONT, 28);
        labelMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelMsg->setPosition(120 + 10, spriteBG->getContentSize().height / 2);
        labelMsg->setDimensions(490, labelMsg->getContentSize().height);
        labelMsg->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelMsg->setOverflow(Label::Overflow::SHRINK);
        labelMsg->setColor(Color3B::YELLOW);
        spriteBG->addChild(labelMsg);
    }
    
    // event
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        _eventDispatcher->removeEventListenersForTarget(_layerSaveWarning);
        hideSaveWarning();
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _layerSaveWarning);
    
    // action
    auto action = CallFunc::create([&]() {
        hideSaveWarning();
    });
    auto seq = Sequence::create(DelayTime::create(5.f),action, nullptr);
    _layerSaveWarning->runAction(seq);
}

void HelloWorldScene::hideSaveWarning()
{
    if ( _layerSaveWarning == nullptr )
        return;
    
    _layerSaveWarning->removeFromParent();
    _layerSaveWarning = nullptr;
}

void HelloWorldScene::showAutoSaveToast()
{
    auto toast = PopupToastAutoSave::create();
    toast->setPosition(_layerTopWidget->getContentSize().width/2 ,_layerTopWidget->getPosition().y -  _layerTopWidget->getContentSize().height/2);
    toast->show();
}

void HelloWorldScene::showTimeTravel()
{
    if ( _layerTimeTravel != nullptr )
        return;
    
    auto layerBlack = LayerColor::create(Color4B::BLACK);
    layerBlack->setPosition(Vec2::ZERO);
    layerBlack->setOpacity(200);
    addChild(layerBlack, E_ZORER::UI);
    
    _layerTimeTravel = LayerColor::create(Color4B::GREEN, UtilsRect::getVisibleSize().width, _sizeMapPattern.height);
    _layerTimeTravel->setIgnoreAnchorPointForPosition(false);
    _layerTimeTravel->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    _layerTimeTravel->setPosition( Vec2(0, UtilsRect::getVisibleSize().height) );
    _layerTimeTravel->setCascadeOpacityEnabled(true);
    addChild(_layerTimeTravel, E_ZORER::UI);
    
    
    // 피버 배경 애니
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 30; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/fever/fx_time_jump_1_%02d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    auto spriteAnim = Sprite::create("Assets/ui/fever/fx_time_jump_1_01.png");
    spriteAnim->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    spriteAnim->setPosition( Vec2(_layerTimeTravel->getContentSize().width / 2, 0) );
    spriteAnim->runAction(RepeatForever::create(Animate::create(animation)));
    _layerTimeTravel->addChild(spriteAnim, 10);
    
    float scale = MAX(_layerTimeTravel->getContentSize().width / spriteAnim->getContentSize().width, _layerTimeTravel->getContentSize().height / spriteAnim->getContentSize().height);
    spriteAnim->setScale(scale);
    
    //
    {
        auto spriteBG1 = Sprite::create("Assets/ui/fever/fx_time_jump_2_01.png");
        spriteBG1->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        spriteBG1->setPosition( Vec2(0, 0) );
        spriteBG1->setScale(scale);
        spriteBG1->setTag(0);
        _layerTimeTravel->addChild(spriteBG1);
        
        auto spriteBG2 = Sprite::create("Assets/ui/fever/fx_time_jump_2_01.png");
        spriteBG2->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        spriteBG2->setPosition( Vec2(spriteBG1->getBoundingBox().size.width, 0) );
        spriteBG2->setScale(scale);
        spriteBG2->setTag(1);
        _layerTimeTravel->addChild(spriteBG2);
        
        //
        auto action =CallFunc::create([=](){
            spriteBG1->setPositionX(spriteBG1->getPositionX() - 2000 * Director::getInstance()->getDeltaTime());
            spriteBG2->setPositionX(spriteBG2->getPositionX() - 2000 * Director::getInstance()->getDeltaTime());
            
            if ( spriteBG1->getPositionX() + spriteBG1->getBoundingBox().size.width < 0 )
            {
                spriteBG1->setPositionX(spriteBG2->getPositionX() + spriteBG2->getBoundingBox().size.width);
            }
            if ( spriteBG2->getPositionX() + spriteBG2->getBoundingBox().size.width < 0 )
            {
                spriteBG2->setPositionX(spriteBG1->getPositionX() + spriteBG1->getBoundingBox().size.width);
            }
        });
        
        auto seq = Sequence::create(action, NULL);
        _layerTimeTravel->runAction(RepeatForever::create(seq));
    }
    
    //
    {
        int level = TimeTravelManager::getInstance()->getItemLevel(E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD);
        _nTimeTravelTime = TimeTravelManager::getInstance()->getItemEffect(E_TIME_TRAVEL::TIME_TRAVEL_TIME_ADD, level);
        
        //
        int32_t m = _nTimeTravelTime / 60;
        int32_t s = _nTimeTravelTime % 60;

        std::string strTime = StringUtils::format("%02d:%02d", m, s);

        auto labelTime = Label::createWithTTF(strTime, GAME_FONT_AL, 100);
        labelTime->setPosition(_layerTimeTravel->getContentSize().width / 2, _layerTimeTravel->getContentSize().height / 2 + 80);
        _layerTimeTravel->addChild(labelTime, 10);
        
        {
            auto action = NumberFromTo::create(3.0f, _nTimeTravelTime, 0, NumberFromTo::E_TYPE::TIME_M_S);
            labelTime->runAction(action);
        }
        
    }
}
#pragma mark-

void HelloWorldScene::CheckServerMission()
{
    bool complete = ServerMissionManager::getInstance()->checkServerMission();
    
    //업적 체크
    if(complete == false)
    {
        complete = AchievementManager::getInstance()->isRewardCondition();
    }
    
    auto spriteIcon = (Sprite*)_itemMission->getChildByTag(100);
    auto sprRedDot = _itemMission->getChildByName("UIReddot");
    
    if(complete)
    {
        if(spriteIcon == nullptr)
        {
            auto aniframeRun = Animation::create();
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_1.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_2.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_3.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_4.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_5.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission.png");
            aniframeRun->setDelayPerUnit(0.1f);
            
            auto aniRun = Animate::create(aniframeRun);
            auto seq = Sequence::create(aniRun, DelayTime::create(1.0), nullptr);
            
            spriteIcon = Sprite::create("Assets/ui/exclamation_mark_icon.png");
            spriteIcon->setPosition(_itemMission->getContentSize().width / 2, _itemMission->getContentSize().height / 2);
            spriteIcon->setTag(100);
            spriteIcon->runAction(RepeatForever::create(seq));
            _itemMission->addChild(spriteIcon);
        }
        if(_itemMission->getChildByName("FINGER") != nullptr)
        {
            _itemMission->reorderChild(_itemMission->getChildByName("FINGER"), 50);
        }
        if(sprRedDot == nullptr)
        {
             auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
            
             uiReddot->setPosition(-10, _itemMission->getContentSize().height + 5);
             
            _itemMission->addChild(uiReddot);
        }
    }
    else
    {
        if ( spriteIcon != nullptr )
        {
            spriteIcon->removeFromParent();
        }
        if(sprRedDot != nullptr)
        {
            sprRedDot->removeFromParent();
        }
    }
}

UnitBehaviorHero* HelloWorldScene::getUnitHero()
{
    return _hero;
}

UnitBehaviorMonster* HelloWorldScene::getUnitMonster()
{
    return _enemy;
}

cocos2d::Vector<Pet*> HelloWorldScene::getUnitPets()
{
    return _vecPet;
}


void HelloWorldScene::CompleteSpecialMission(bool complete)
{
    stepwiseContentsOpen(false,E_STEPWISE_TYPE::SPECIAL_MISSION);
    
    auto spriteIcon = (Sprite*)_itemMission->getChildByTag(100);
    auto spriteIconSpecial = (Sprite*)_itemMission->getChildByTag(200);
    if ( spriteIcon != nullptr )
    {
        return;
    }
    
    if ( complete == false )
    {
        if ( spriteIconSpecial != nullptr )
        {
            spriteIconSpecial->removeFromParent();
        }
    }
    else if ( complete == true && spriteIconSpecial == nullptr )
    {
        auto aniframeRun = Animation::create();
        aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_1.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_2.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_3.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_4.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission_fx_5.png");
        aniframeRun->addSpriteFrameWithFile("Assets/ui/bg_icon/icon_mission.png");
        aniframeRun->setDelayPerUnit(0.1f);
        
        auto aniRun = Animate::create(aniframeRun);
        auto seq = Sequence::create(aniRun, DelayTime::create(1.0), nullptr);

        spriteIcon = Sprite::create("Assets/ui/exclamation_mark_icon.png");
        spriteIcon->setPosition(_itemMission->getContentSize().width / 2, _itemMission->getContentSize().height / 2);
        spriteIcon->setTag(200);
        spriteIcon->runAction(RepeatForever::create(seq));
        _itemMission->addChild(spriteIcon);
        
        if(_itemMission->getChildByName("FINGER") != nullptr)
        {
            _itemMission->reorderChild(_itemMission->getChildByName("FINGER"), 50);
        }
    }

}

#pragma mark - callback
void HelloWorldScene::callbackArtifactTab(int tab)
{
    createArtifactTable(tab);
}

void HelloWorldScene::callbackArtifactReinforce()
{
    if ( _cellArtifacTop != nullptr )
    {
        _cellArtifacTop->drawInfo();
    }
    
    for(auto cell : _vecCellArtifact)
    {
        cell->drawUpgrade();
    }
}



void HelloWorldScene::callbackRevive(int nType, bool bAds, bool bFree, int nFloorSub)
{
    _iPrestigeSpecial = nType;
    _bRevivalAds = bAds;
    _bRevivalFree = bFree;
    _nRevivalFloorSub = nFloorSub;
    
    if ( _bRevivalAds == true )
    {
        if( _iPrestigeSpecial == 1 && _bRevivalFree == false)
        {
            if(ItemsMoneyManager::getInstance()->getGem() < C_PRESTIGE_GEM)
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
                popup->show();
                return;
            }
        }
        else if( _iPrestigeSpecial == 2 )
        {
            if(ItemsMoneyManager::getInstance()->getGem() < C_PRESTIGE_GEM2)
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
                popup->show();
                return;
            }
        }
    }
    
    if ( bAds == false )
    {
        onPrestige();
    }
    else if ( bAds == true )
    {
        if ( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            ResultRevivalAdsCallBack(ADS_RESULT::SUCCESS);
        }
        else
        {
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(HelloWorldScene::ResultRevivalAdsCallBack, this));
        }
    }
    
    //튜토리얼 매니저에 첫 환생 관련 bool값 추가 해서 가져오고 설정하는 방법으로 해야할 듯
    if(UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL) == 1)
        TutorialManager::getInstance()->setFirstReviver(true);
}
void HelloWorldScene::onClickoRevive()
{
    // story
    StoryManager::getInstance()->showStory((int)E_COMIC::REVIVE);
    
    if ( UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 100 )
    {
        MafAnalyticsManager::LogEvent(kAnalEventRivivalClick, kRepeatTrue);
    }
    
    //
    auto popup = PopupRevive::create();
    popup->setCallbackResult(CC_CALLBACK_4(HelloWorldScene::callbackRevive, this));
    popup->show();
}
void HelloWorldScene::onClickDefenseBackground(Ref* sender)
{
    if(DefenseManager::getInstance()->isBackground())
        DefenseManager::getInstance()->setIsBackground(false);
}
void HelloWorldScene::callbackBuffRecommendAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotBuffRecommend, CashManager::getInstance()->isAdsfreeTime());
        
        //
        int buffTime = MafRemoteConfig::GetRemoteValueInt("buff_recommend_time");
        UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_9_BONUS, buffTime);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(HelloWorldScene::callbackBuffRecommendAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotBuffRecommend);
            popup->show();
        }
    }
}

void HelloWorldScene::callbackRecommendCostume(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    
    std::vector<tagRecommendCostumeData>::iterator iter = _vecRecommendCostumes.begin();
    
    if(CostumeManager::getInstance()->isEquip(eType, eIdx))
    {
        for(; iter != _vecRecommendCostumes.end(); ++iter)
        {
            if(iter->_eType == eType && iter->_eIdx == eIdx)
            {
                _vecRecommendCostumes.erase(iter);
                break;
            }
        }
        return;
    }
    
    for(iter = _vecRecommendCostumes.begin(); iter != _vecRecommendCostumes.end(); ++iter)
    {
        if(iter->_eType == eType && iter->_eIdx == eIdx)
            break;
    }
    
    if(iter != _vecRecommendCostumes.end())
        iter->_button->removeFromParentAndCleanup(true);
    
    _vecRecommendCostumes.erase(iter);
    
    CostumeManager::getInstance()->setEquip(eType, eIdx);
    CostumeManager::getInstance()->setRent(eType, eIdx);
    CostumeManager::getInstance()->setRentTime(eType, TIME_COSTUME_RENT);
    
    SaveManager::saveCostume();
    SaveManager::saveCostumeRentList();
    SaveManager::saveCostumeRentTime();
    
    //costume 착용 변환
    _hero->setChangeParts();
    //_itemStartBtn->removeFromParentAndCleanup(true);
    MafAnalyticsManager::LogEvent(kAnalEventRecommendCostumeClick, kRepeatTrue);
    
}


void HelloWorldScene::callbackUpgradeInfiniteArtifact(bool bFirstUpgrade)
{
    if(bFirstUpgrade)
    {
        auto scroll = _scroll[E_GAME_STATE::ARTIFACT];
        auto container = (Layer*)scroll->getContainer();
        
        container->setContentSize(Size(getContentSize().width,C_BAR_SIZE*_vecCellArtifact.size()));
        scroll->setContentOffset(Vec2(0,-C_BAR_SIZE*_vecCellArtifact.size()));
        
        createInfiniteArtifactTable();
        _cellArtifacTop->reDrawCell();
    }
    else
    {
        for(auto info : _vecCellInfiniteArti)
        {
            info->upgradeInfiniteArtifact();
        }
        
        auto scroll = _scroll[E_GAME_STATE::ARTIFACT_INFINITE];
        auto container = (Layer*)scroll->getContainer();
        
        container->setContentSize(Size(getContentSize().width,C_BAR_SIZE*_vecCellInfiniteArti.size()));
        scroll->setContentOffset(Vec2(0,-C_BAR_SIZE*_vecCellInfiniteArti.size()));
    }
}

void HelloWorldScene::callbackCheckInfiniteUpgrade()
{
    if(!InfiniteArtifactManager::getInstance()->isUpgradeCondition())
        return;
    
    auto scroll = _scroll[E_GAME_STATE::ARTIFACT_INFINITE];
    auto container = (Layer*)scroll->getContainer();
    
    auto cellInfiniteMover = new ModelCell::CellUpgradeInfinite();
    cellInfiniteMover->setName("INFINITE_ARTI_UPGRADE");
    cellInfiniteMover->autorelease();
    cellInfiniteMover->setPosition(0, (_vecCellInfiniteArti.size()+1) * C_BAR_SIZE - C_BAR_SIZE);
    cellInfiniteMover->setCallbackMove(CC_CALLBACK_1(HelloWorldScene::setGameStateToInt, this));
    cellInfiniteMover->setCallbackUpgrade(CC_CALLBACK_1(HelloWorldScene::callbackUpgradeInfiniteArtifact, this));
    container->addChild(cellInfiniteMover);
    
    int nCount = (int)_vecCellInfiniteArti.size() + 1;
               
    container->setContentSize(Size(getContentSize().width,C_BAR_SIZE*nCount));
    scroll->setContentOffset(Vec2(0,-C_BAR_SIZE*nCount));
}

void HelloWorldScene::callbackTimeTravel()
{
    //
    PopupLoadingLock::show();
    
    // stop all action
    _hero->heroStopAllAction();//stopAllActions();
    auto children = _hero->getChildren();
    for(auto child : children)
        child->setVisible(false);
    
    if(_enemy !=nullptr)
        _enemy->setVisible(false);
        
    for ( auto pet : _vecPet )
    {
        pet->unscheduleUpdate();
        pet->stopAllActions();
    }

    //
    showTimeTravel();
    
    //
    TimeTravelManager::getInstance()->setCallbackCalculation(nullptr);
    auto t = std::thread(CC_CALLBACK_0(TimeTravelManager::OnRewardCalculation, TimeTravelManager::getInstance()));
    t.detach();
    
    auto action1 = DelayTime::create(3.0f);
    auto action2 = CallFunc::create([=](){
        
        //
        callbackTimeTravelCalculation();
    
    });
    auto seq = Sequence::create(action1, action2, NULL);
    runAction(seq);
}

void HelloWorldScene::callbackTimeTravelCalculation()
{
    PopupLoadingLock::hide();
    
    auto popup = PopupTimeTravelReward::create();
    popup->setHideBackKey(false);
    popup->setHideAll(false);
    popup->show();
}

#pragma mark -
void HelloWorldScene::showRecommendCostume(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    if(CostumeManager::getInstance()->isExist(eType, eIdx))
        return;
    
    if(MafRemoteConfig::GetRemoteValueBool("recommend_costume")==false)
    { //a,b test
        return;
    }
    
    //
    MafAnalyticsManager::LogEvent(kAnalEventRecommendCostumePop, kRepeatTrue);
    
    //menuitem 안에 넣기.
    auto _info = CostumeManager::getInstance()->getCostume(eType, eIdx);
    auto _itemStartBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/box_bg_costume_01.png"),
                                                   Sprite::create("Assets/ui_common/box_bg_costume_01.png"),nullptr);
    _itemStartBtn->setPosition(Vec2(_layerTopWidget->getContentSize().width*0.25,_layerTopWidget->getContentSize().height*0.35));
    _itemStartBtn->setName("Recommend_Costume");
    _menu_TopWidget->addChild(_itemStartBtn);
    
    JumpBy *jump = JumpBy::create(1, Vec2(0,15), 15, 1);
    Sequence *seq = Sequence::create(jump,jump->reverse(), nullptr);
    RepeatForever *repeat = RepeatForever::create(seq);
    _itemStartBtn->runAction(repeat);
    
    auto _sprIcon = Sprite::create(DataManager::GetCostumeIconPath(_info->getType(), _info->getIdx()));
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setPosition(_itemStartBtn->getContentSize().width*0.5, _itemStartBtn->getContentSize().height*0.5);
    _sprIcon->setScale(2);
    _itemStartBtn->addChild(_sprIcon);
    
    //
    _vecRecommendCostumes.push_back(tagRecommendCostumeData(_itemStartBtn, eType, eIdx, 0));
    
    //
    _itemStartBtn->setCallback([=](Ref* sender){
        auto popup = PopupCostumeRecommend::create(eType, eIdx);
        popup->setCallbackYes(CC_CALLBACK_2(HelloWorldScene::callbackRecommendCostume ,this));
        popup->show();
    });
    
}

#pragma mark -
void HelloWorldScene::stepwiseContentsOpen(bool init, E_STEPWISE_TYPE type)
{
    auto tutoMgr = TutorialManager::getInstance();
    if(init)
    {//init 할 때
        {//스페셜 미션

            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34))
            {
                auto item = _menu_btn->getChildByName("ARTIFACT_Lock");
                if(item != nullptr)
                    item->setVisible(false);
            }
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35))
            {
                auto item = _menu_btn->getChildByName("PET_Lock");
                
                if(item != nullptr)
                    item->setVisible(false);
            }
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38))
            {
                if ( _itemCostumeLock != nullptr )
                    _itemCostumeLock->setVisible(false);
            }
        }
        {//층
            if(tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 100))
            {
                if ( _itemDungeonPass != nullptr )
                    _itemDungeonPass->setVisible(false);
            }
        }
        {//기타 조건
            
        }
    }
    else
    {//init 이 아닐 떄
        switch (type) {
            case FLOOR:
            {
                switch(UserInfoManager::getInstance()->getHighFloor(_ePlace))
                {
                    case 75:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 75))
                        {
                            if(AccountManager::getInstance()->getUserIdx() != 0)
                            {
                                int nResult = CashManager::getInstance()->getFirstBonusResult();
                                if ( nResult != 5 )
                                {
                                    auto popup = PopupPurchaseFirst::create();
                                    popup->setCallbackResult([=](int nType){
                                        
                                        if ( nType != 5 )
                                        {
                                            auto popup = PopupCash::create();
                                            popup->show();
                                        }
                                    });
                                    popup->show();
                                }
                            }
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 75, true);
                        }
                    }
                        break;
                    case 100:
                    {
                        //월간 출석
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 100))
                        {
                            if ( _itemDungeonPass != nullptr )
                                _itemDungeonPass->setVisible(false);
                            
                            //성장 패키지
                            if ( AccountManager::getInstance()->getUserIdx() != 0 )
                            {
                                auto popup = PopupPurchaseGrowth::create("FLOOR");
                                popup->show();
                            }
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 100, true);
                        }
                    }
                    case 120:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34))
                        {
                            auto item = _menu_btn->getChildByName("ARTIFACT_Lock");
                            if(item != nullptr)
                                item->setVisible(false);
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34, true);
                        }
                        
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35))
                        {
                            auto item = _menu_btn->getChildByName("PET_Lock");
                                                   
                            if(item != nullptr)
                            item->setVisible(false);
                                                   
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35, true);
                        }
                        
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38))
                        {
                            if (_itemCostumeLock != nullptr )
                                _itemCostumeLock->setVisible(false);
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38, true);
                        }
                    }
                        break;
                    case 750:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 750))
                        {
                            
                            if ( EventManager::getInstance()->isActivatedEvent(21) == true || EventManager::getInstance()->isActivatedEvent(22) == true )
                            {
                                auto popup = PopupPurchaseBanner::create();
                                popup->show();
                            }
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 750, true);
                        }
                    }
                        break;
                }
            }
                break;
                
            case SPECIAL_MISSION:
            {
                switch(SpecialMissionManager::getInstance()->getSpecialMissionClear())
                {
                        
                    case 9:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 9))
                        {
                            if(UserInfoManager::getInstance()->getFloor(_ePlace) < 25)
                            {
                                auto floorUI = TargetFloorUI::create(25);
                                floorUI->setPosition(_layerTopWidget->getContentSize().width / 2, 200);
                                _layerTopWidget->addChild(floorUI);
                                tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 9, true);
                            }
                        }
                    }
                        break;

                    case 10:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 10))
                        {
                            showRecommendCostume(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_3);
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 10, true);
                        }
                    }
                    
                        break;
                    case 11:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 11))
                        {
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 11, true);
                        }
                    }
                    case 12:
                    {

                    }
                        break;
                    case 17:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 17))
                        {
                            showRecommendCostume(E_COSTUME::BODY, E_COSTUME_IDX::IDX_2);
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 17, true);
                        }
                    }
                        break;
                    case 18:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 18))
                        {
                            if(UserInfoManager::getInstance()->getFloor(_ePlace) < 50)
                            {
                                auto floorUI = TargetFloorUI::create(50);
                                floorUI->setPosition(_layerTopWidget->getContentSize().width / 2, 200);
                                _layerTopWidget->addChild(floorUI);
                                tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 18, true);
                            }
                        }
                    }
                        break;
                    case 22:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 22))
                        {
                            showRecommendCostume(E_COSTUME::FOOT, E_COSTUME_IDX::IDX_3);
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 22, true);
                        }
                    }
                        break;
                    case 27:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 27))
                        {
                            if(UserInfoManager::getInstance()->getFloor(_ePlace) < 80)
                            {
                                auto floorUI = TargetFloorUI::create(80);
                                floorUI->setPosition(_layerTopWidget->getContentSize().width / 2, 200);
                                _layerTopWidget->addChild(floorUI);
                                tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 27, true);
                            }
                        }
                    }
                        break;
                    case 34:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34))
                        {
                            auto item = _menu_btn->getChildByName("ARTIFACT_Lock");
                            
                            if(item != nullptr)
                                item->setVisible(false);
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34, true);
                        }
                    }
                        break;
                    case 35:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35))
                        {
                            auto item = _menu_btn->getChildByName("PET_Lock");
                            
                            if(item != nullptr)
                                item->setVisible(false);
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35, true);
                        }
                    }
                        break;
                    case 38:
                    {
                        if(!tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38))
                        {
                            if (_itemCostumeLock != nullptr )
                                _itemCostumeLock->setVisible(false);
                            
                            tutoMgr->setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38, true);
                        }
                    }
                        break;
                    case 83:
                    {
                        
                    }
                }
            }
                break;
            case ETC:
            {
                if(!tutoMgr->getOpenData("WEAPON_76"))
                {
                    if(WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE)
                    {
                        tutoMgr->setOpenData("WEAPON_76", true);
                    }
                }
            }
                break;
        }
    }
}
