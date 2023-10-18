#include "DevilDomScene.h"

#include "Common/ConfigGameDevil.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameScene/BattleNormal/Layer/LayerNormalDungeon.h"

#include "GameUIPopup/Common/MenuPopup.h"
#include "GameUIPopup/Other/Stat/PopupStat.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"
#include "GameUIPopup/Other/Equip/PopupEquipDetail.h"
#include "GameUIPopup/Other/Settings/PopupQuit.h"
#include "GameUIPopup/Other/Revive/PopupRevive.h"
#include "GameUIPopup/Other/Buff/PopupBuff.h"
#include "GameUIPopup/Other/Costume/PopupCostumeDefault.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/Expedition/PopupExpeditionDraw.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Settings/SaveLoad/PopupToastAutoSave.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureGuild.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/FriendManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/FlashmanManager.h"

USING_NS_CC;

static DevilDomScene *s_DevilDom = nullptr;
DevilDomScene* DevilDomScene::getInstance(){
    return s_DevilDom;
}

DevilDomScene::DevilDomScene() :
_bInitUI(false),

_nMapIdx(0),
_nMapType(0),
_fTimer1Min(0),
_uiContainerDungeon(nullptr)
{
    s_DevilDom = this;
}

DevilDomScene::~DevilDomScene()
{
    _vecPet.clear();
    _vecCellQuest.clear();
    _vecCellEquip.clear();
    _vecCellPet.clear();
    _vecCellDungeon.clear();
    _vecNodeTemp.clear();
    _vecMenuTemp.clear();
    
    for(int i=0;i<C_COUNT_GAME_STATE_DEVIL;i++){
        _scroll[i]->removeAllChildrenWithCleanup(true);
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool DevilDomScene::onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event)
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

void DevilDomScene::onButtonTouched(cocos2d::Ref* pSender){
    
    SoundManager::SoundEffectStart(SOUND_SELECT);
    MenuItem *item = (MenuItem*)pSender;
    std::string name = item->getName();
    
    if ( name == "QUEST" || name == "EQUIP" || name == "ARTIFACT" || name == "PET" || name == "DUNGEON_INPUT" )
    {
        
        Vector<MafNode::MafMenuItemSprite*> parentMenu = (Vector<MafNode::MafMenuItemSprite*>&)_menu_btn->getChildren();
        for(auto child : parentMenu){
            if(child->getName() == name)    child->selected();
            else                            child->unselected();
            
            auto label = (Label*)child->getChildByTag(10000);
            if ( label != nullptr )
            {
                if(child->getName() == name)
                    label->setTextColor(Color4B::WHITE);
                else
                    label->setTextColor(Color4B(115, 116, 118, 255));
            }
        }
        
        E_GAME_STATE state = E_GAME_STATE::QUEST;
        if(name == "QUEST")         state = E_GAME_STATE::QUEST;
        else if(name == "EQUIP")    state = E_GAME_STATE::EQUIP;
        else if(name == "ARTIFACT") state = E_GAME_STATE::ARTIFACT;
        else if(name == "SHOP")     state = E_GAME_STATE::SHOP;
        else if(name == "PET")      state = E_GAME_STATE::PET;
        else if(name == "MINE")     state = E_GAME_STATE::MINE;
        else if(name == "DUNGEON_INPUT") state = E_GAME_STATE::DUNGEON;
        
        setGameState(state);
    }
    else if ( name == "SHOP" )
    {
        auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::SHOPITEM);
        popup->show();
    }
    else if ( name == "MINE" )
    {
        auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MINE);
        popup->show();
    }
    else if ( name == "MENU" )
    {
        auto popup = MenuPopup::create();
        popup->show();
    }
    else if ( name == "PRESTIGE" )
    {
        auto popup = PopupRevive::create();
        popup->setCallbackResult(CC_CALLBACK_4(DevilDomScene::callBackRevive, this));
        popup->show();
    }
    else if ( name == "BUFF_DETAIL" )
    {
        ValueVector list;
        list.push_back(Value(E_SHOP::SHOP_D_ATK_DMG));
        list.push_back(Value(E_SHOP::SHOP_D_QUEST_GOLD));
        list.push_back(Value(E_SHOP::SHOP_D_MOVE_SPD));
        list.push_back(Value(E_SHOP::SHOP_D_ATK_SPD));
        
        auto popup = PopupBuff::create(list);
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
            CashManager::getInstance()->setNewIcon(false, E_PLACE::DG_DEVIL_DOM);
            
            //
            auto popup = PopupCash::create();
            popup->show();
        }
        else if ( name == "COSTUME" )
        {
            item->removeChildByName("UIReddot");
            UserDefault::getInstance()->setBoolForKey(KEY_REDDOT_COSTUME_SKIN1, true);
            
            //
            auto popup = PopupCostumeDefault::create();
            popup->show();
        }
    }
}

bool DevilDomScene::init()
{
    if ( !BaseGameScene::init() )
    {
        return false;
    }
    
    s_DevilDom = this;
    
    _ePlace = E_PLACE::DG_DEVIL_DOM;
    UserInfoManager::getInstance()->setCurrentPlace(_ePlace);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    _bRevivalAds = false;
    _itemReinforceEvolution = nullptr;
    _itemPetGrade = nullptr;
    _hero = nullptr;
    _enemy = nullptr;
    _selectEquip = nullptr;
    _sprReceivePopup = nullptr;
    _layerSaveWarning = nullptr;
    
    UserInfoManager::getInstance()->_onModifyFloor = nullptr;
    UserInfoManager::getInstance()->_onModifyEquip = nullptr;
    
    //
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    if ( floor % 100 == 0 )
    {
        SoundManager::SoundBgmStart(SOUND_BGM_D_BOSS_1, true);
    }
    else{
        SoundManager::SoundBgmStart(SOUND_BGM_D_1, true);
    }
    
    
    
    return true;
}

void DevilDomScene::onEnter()
{
    BaseGameScene::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        UserInfoManager::getInstance()->_onModifyFloor = CC_CALLBACK_0(DevilDomScene::drawFloor,this);
        UserInfoManager::getInstance()->_onModifyEquip = CC_CALLBACK_0(DevilDomScene::drawEquip,this);
        UserInfoManager::getInstance()->setOneKill(_ePlace, false);
        UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, E_PRINCESS_BUFF::BUFF_NONE);
        UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER);
        UserInfoManager::getInstance()->setBattleUI(this);
        
        //
        startScene();
        
        //
        _fTimer = _fBuffRedrawTimer = _fTimerQuestSave = _fAdsRedrawTimer = 0;
        schedule(schedule_selector(DevilDomScene::scheduleTimer), 1, kRepeatForever, 1);
        
        // update
        scheduleUpdate();
        
        //
        TutorialManager::getInstance()->drawTutorial();
        
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_START);
        TimingPackageManager::getInstance()->setArrangementInfo();
        
        //
        RefreshManager::getInstance()->addUI(E_REFRESH::BUFF_BUY_DEVIL, this);
    }
    
    //
    MafGoogleRewardAds::HideGoogleBanner();
    
    //
    auto adsErrorMgr = AdsUtilsManager::getInstance();
    if(adsErrorMgr->getAdsCooldown() > 0)
    {
        adsErrorMgr->setAdsCooldown(adsErrorMgr->getAdsCooldown());
    }
    
    //
    requestCheckCostumeCoin();
}

void DevilDomScene::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::BUFF_BUY_DEVIL )
    {
        reDrawQuest();
        drawEquip();
    }
}

void DevilDomScene::move(E_AREA_MOVE eType)
{
    
    if ( eType == E_AREA_MOVE::AREA_NORMAL )
    {
        UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
    }
    else if ( eType == E_AREA_MOVE::AREA_DEVIL )
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("QUEST");
        onButtonTouched(item);
    }
    else if ( eType == E_AREA_MOVE::AREA_PRISON || eType == E_AREA_MOVE::AREA_RAID )
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
        bool bEnter = false;
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
        
        auto popup = PopupAdventureGuild::create();
        popup->show();
    }
    else if ( eType == E_AREA_MOVE::AREA_SHOP )
    {
        auto item = MafNode::MafMenuItem::create();
        item->setName("CASH");
        onButtonTouched(item);
    }
    else if ( eType == E_AREA_MOVE::AREA_NORMAL_QUEST || eType == E_AREA_MOVE::AREA_NORMAL_ARTIFACT || eType == E_AREA_MOVE::AREA_NORMAL_WEAPON ||
              eType == E_AREA_MOVE::AREA_NORMAL_PET || eType == E_AREA_MOVE::AREA_NORMAL_MYHOME || eType == E_AREA_MOVE::AREA_NORMAL_TOWN ||
              eType == E_AREA_MOVE::AREA_NORMAL_MINE || eType == E_AREA_MOVE::AREA_NORMAL_REVIVE )
    {
        UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
    }
    else if ( eType == E_AREA_MOVE::AREA_DEVIL_QUEST || eType == E_AREA_MOVE::AREA_DEVIL_WEAPON || eType == E_AREA_MOVE::AREA_DEVIL_SLATE || eType == E_AREA_MOVE::AREA_DEVIL_REVIVE )
    {
        std::string strName = "QUEST";
        switch (eType) {
            case E_AREA_MOVE::AREA_DEVIL_REVIVE:
                strName = "PRESTIGE";
                break;
            case E_AREA_MOVE::AREA_DEVIL_WEAPON:
                strName = "EQUIP";
                break;
            case E_AREA_MOVE::AREA_DEVIL_SLATE:
                strName = "ARTIFACT";
                break;
                
            default:
                break;
        }
        
        auto item = MafNode::MafMenuItem::create();
        item->setName(strName);
        onButtonTouched(item);
    }
}

#pragma mark-

void DevilDomScene::startScene(){
    
    createMapIdx();
    createMap();//map이 가장위에 있어야 한다.
    
    initUI();
    initBottomAddUI();

    drawFloor();
    
    createHero();
    createTable();
//    createPet(E_PET::D_1_PENGUIN);
//    createPet(E_PET::D_2_HEDGEHOG);
    setGameState(E_GAME_STATE::QUEST);
    
    drawQuestTime();
    
    startDungeon();
    
    if(FriendManager::getInstance()->_bBootLinkAdd == true)
    {
        FriendManager::getInstance()->requestFriendIdx();//친구 초대 있으면 실행
    }

    // snow
    if ( false )
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        auto snow = ParticleSnow::createWithTotalParticles(random(30, 100));
        snow->setGravity(Vec2(0,-10));
        snow->setSpeed(130);
        snow->setSpeedVar(30);
        
        snow->setPosition( Vec2( visibleSize.width*0.5,visibleSize.height) );
        snow->setLife(3);
        snow->setLifeVar(1);
        snow->setDuration(-1);
        snow->setEmissionRate(snow->getTotalParticles()/snow->getLife());
        snow->setTexture( Director::getInstance()->getTextureCache()->addImage("Assets/ui/snow.png") );
        snow->setName("SNOW");
        addChild(snow,POPUP);
    }
    
}

void DevilDomScene::setGameState(E_GAME_STATE state){
    _eGameState = state;
    showTable();
}

#pragma mark-


void DevilDomScene::initUI()
{
    float topHeight = visibleSize.height - UserInfoManager::getInstance()->_fHeightIphoneX;
    
    //
    _menu_btn = MafNode::MafMenu::create();
    _menu_btn->setAnchorPoint(Vec2::ZERO);
    _menu_btn->setPosition(Vec2::ZERO);
    _menu_btn->setName("BOT_MENU");
    addChild(_menu_btn,E_ZORER::UI+1);
    
    //
    _lbFloor = Label::createWithTTF("", GAME_FONT, 24);
    _lbFloor->setAnchorPoint(Vec2(0.5,1));
    _lbFloor->setPosition(Vec2(visibleSize.width/2,topHeight-10));
    _lbFloor->setAlignment(TextHAlignment::CENTER);
    _lbFloor->enableOutline(Color4B::BLACK, 1);
    addChild(_lbFloor,E_ZORER::UI);
    
    _lbWave = Label::createWithTTF("", GAME_FONT, 22);
    _lbWave->setAnchorPoint(Vec2(0.5,1));
    _lbWave->setPosition(Vec2(visibleSize.width/2,topHeight-40));
    _lbWave->setAlignment(TextHAlignment::CENTER);
    _lbWave->enableOutline(Color4B::BLACK, 1);
    addChild(_lbWave,E_ZORER::UI);

    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::D_GOLD);
    list.push_back(E_ITEMS::D_KEY);
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::POINT);
    list.push_back(E_ITEMS::GEM);
    
    auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 58));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiCurrency->setPosition(Vec2(getContentSize().width / 2, visibleSize.height - _sizeMapPattern.height));
    uiCurrency->setEffect(true);
    uiCurrency->setUIPosition(Vec2::ANCHOR_MIDDLE);
    addChild(uiCurrency, E_ZORER::UI+1);
    
    
    
    //
    {
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        addChild(menu, E_ZORER::UI);
        
        auto _sprTimeSpeedAttack = Sprite::create("Assets/ui_common/icon_buff_atk.png");
        auto itemBuffAttack = MafNode::MafMenuItemSprite::create(_sprTimeSpeedAttack, nullptr, CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
        itemBuffAttack->setAnchorPoint(Vec2(0,1));
        itemBuffAttack->setPosition(Vec2(2,topHeight-5));
        itemBuffAttack->setName("BUFF_DETAIL");
        itemBuffAttack->setTouchRect(itemBuffAttack->getContentSize() * 1.5);
        menu->addChild(itemBuffAttack);
        _sprBuff[E_BUFF_TYPE::BUFF_D_ATK_DMG]=_sprTimeSpeedAttack;
        {
            auto sprAdsFx = Sprite::create("Assets/ui_common/icon_buff_fx_1_01.png");
            sprAdsFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAdsFx->setPosition(_sprTimeSpeedAttack->getContentSize()/2);
            sprAdsFx->setName("ADS_FX");
            _sprTimeSpeedAttack->addChild(sprAdsFx);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 16; i++ )
            {
                auto sprFrame = Sprite::create(MafUtils::format("Assets/ui_common/icon_buff_fx_1_%02d.png", i));
                sprFrame->getTexture()->setTexParameters(texParams);
                
                animation->addSpriteFrame(sprFrame->getSpriteFrame());
            }
            sprAdsFx->runAction(RepeatForever::create(Animate::create(animation)));
            sprAdsFx->setVisible(false);
        }
        
        auto _lbTimeSpeedAttack = MafNode::createBmFont("", GAME_BM_FONT_AL, 18);
        _lbTimeSpeedAttack->setAnchorPoint(Vec2(0.5,1));
        _lbTimeSpeedAttack->setPosition(Vec2(_sprTimeSpeedAttack->getContentSize().width/2,0));
        _sprTimeSpeedAttack->addChild(_lbTimeSpeedAttack);
        _lbBuff[E_BUFF_TYPE::BUFF_D_ATK_DMG] =_lbTimeSpeedAttack;
        
        auto _sprTimeCoinBonus = Sprite::create("Assets/ui_common/icon_buff_quest.png");
        auto itemBuffCoin = MafNode::MafMenuItemSprite::create(_sprTimeCoinBonus, nullptr, CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
        itemBuffCoin->setAnchorPoint(Vec2(0,1));
        itemBuffCoin->setPosition(Vec2(2 +_sprTimeCoinBonus->getContentSize().width+4+1,topHeight-5));
        itemBuffCoin->setName("BUFF_DETAIL");
        itemBuffCoin->setTouchRect(itemBuffCoin->getContentSize() * 1.5);
        menu->addChild(itemBuffCoin);
        _sprBuff[E_BUFF_TYPE::BUFF_D_QUEST_GOLD]=_sprTimeCoinBonus;
        {
            auto sprAdsFx = Sprite::create("Assets/ui_common/icon_buff_fx_1_01.png");
            sprAdsFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAdsFx->setPosition(_sprTimeCoinBonus->getContentSize()/2);
            sprAdsFx->setName("ADS_FX");
            _sprTimeCoinBonus->addChild(sprAdsFx);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 16; i++ )
            {
                auto sprFrame = Sprite::create(MafUtils::format("Assets/ui_common/icon_buff_fx_1_%02d.png", i));
                sprFrame->getTexture()->setTexParameters(texParams);
                
                animation->addSpriteFrame(sprFrame->getSpriteFrame());
            }
            sprAdsFx->runAction(RepeatForever::create(Animate::create(animation)));
            sprAdsFx->setVisible(false);
        }
        
        auto _lbTimeCoinBonus = MafNode::createBmFont("", GAME_BM_FONT_AL, 18);
        _lbTimeCoinBonus->setAnchorPoint(Vec2(0.5,1));
        _lbTimeCoinBonus->setPosition(Vec2(_sprTimeSpeedAttack->getContentSize().width/2,0));
        _sprTimeCoinBonus->addChild(_lbTimeCoinBonus);
        _lbBuff[E_BUFF_TYPE::BUFF_D_QUEST_GOLD] =_lbTimeCoinBonus;
        
        auto _sprTimeMoveBonus = Sprite::create("Assets/ui_common/icon_buff_spd.png");
        auto itemBuffMove = MafNode::MafMenuItemSprite::create(_sprTimeMoveBonus, nullptr, CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
        itemBuffMove->setAnchorPoint(Vec2(0,1));
        itemBuffMove->setPosition(Vec2(2 +(_sprTimeCoinBonus->getContentSize().width+4)*2+1,topHeight-5));
        itemBuffMove->setName("BUFF_DETAIL");
        itemBuffMove->setTouchRect(itemBuffMove->getContentSize() * 1.5);
        menu->addChild(itemBuffMove);
        _sprBuff[E_BUFF_TYPE::BUFF_D_MOVE_SPD]=_sprTimeMoveBonus;
        {
            auto sprAdsFx = Sprite::create("Assets/ui_common/icon_buff_fx_1_01.png");
            sprAdsFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAdsFx->setPosition(_sprTimeMoveBonus->getContentSize()/2);
            sprAdsFx->setName("ADS_FX");
            _sprTimeMoveBonus->addChild(sprAdsFx);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 16; i++ )
            {
                auto sprFrame = Sprite::create(MafUtils::format("Assets/ui_common/icon_buff_fx_1_%02d.png", i));
                sprFrame->getTexture()->setTexParameters(texParams);
                
                animation->addSpriteFrame(sprFrame->getSpriteFrame());
            }
            sprAdsFx->runAction(RepeatForever::create(Animate::create(animation)));
            sprAdsFx->setVisible(false);
        }
        
        auto _lbTimeMoveBonus = MafNode::createBmFont("", GAME_BM_FONT_AL, 18);
        _lbTimeMoveBonus->setAnchorPoint(Vec2(0.5,1));
        _lbTimeMoveBonus->setPosition(Vec2(_sprTimeMoveBonus->getContentSize().width/2,0));
        _sprTimeMoveBonus->addChild(_lbTimeMoveBonus);
        _lbBuff[E_BUFF_TYPE::BUFF_D_MOVE_SPD] =_lbTimeMoveBonus;
        
        auto _sprCountKeyBonus = Sprite::create("Assets/ui_common/icon_buff_atkspd.png");
        auto itemBuffKey = MafNode::MafMenuItemSprite::create(_sprCountKeyBonus, nullptr, CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
        itemBuffKey->setAnchorPoint(Vec2(0,1));
        itemBuffKey->setPosition(Vec2(2 +(_sprTimeCoinBonus->getContentSize().width+4)*3+1,topHeight-5));
        itemBuffKey->setName("BUFF_DETAIL");
        itemBuffKey->setTouchRect(itemBuffKey->getContentSize() * 1.5);
        menu->addChild(itemBuffKey);
        _sprBuff[E_BUFF_TYPE::BUFF_D_ATK_SPD]=_sprCountKeyBonus;
        {
            auto sprAdsFx = Sprite::create("Assets/ui_common/icon_buff_fx_1_01.png");
            sprAdsFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprAdsFx->setPosition(_sprCountKeyBonus->getContentSize()/2);
            sprAdsFx->setName("ADS_FX");
            _sprCountKeyBonus->addChild(sprAdsFx);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 16; i++ )
            {
                auto sprFrame = Sprite::create(MafUtils::format("Assets/ui_common/icon_buff_fx_1_%02d.png", i));
                sprFrame->getTexture()->setTexParameters(texParams);
                
                animation->addSpriteFrame(sprFrame->getSpriteFrame());
            }
            sprAdsFx->runAction(RepeatForever::create(Animate::create(animation)));
            sprAdsFx->setVisible(false);
        }
        
        auto _lbCountKeyBonus = MafNode::createBmFont("", GAME_BM_FONT_AL, 18);
        _lbCountKeyBonus->setAnchorPoint(Vec2(0.5,1));
        _lbCountKeyBonus->setPosition(Vec2(_sprCountKeyBonus->getContentSize().width/2,0));
        _sprCountKeyBonus->addChild(_lbCountKeyBonus);
        _lbBuff[E_BUFF_TYPE::BUFF_D_ATK_SPD] =_lbCountKeyBonus;
    }
    
    
    
    
    std::string str = "Assets/ui/menu_bottom/menu_btn_bg_off.png";
    auto sprSize = Sprite::create(str);
    Size sizeMenu = sprSize->getContentSize();
    std::string strSelect = "Assets/ui/menu_bottom/menu_btn_bg_on.png";
    
    // item 퀘스트
    auto itemQuest = MafNode::MafMenuItemSprite::create(Sprite::create(str),Sprite::create(strSelect), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemQuest->setAnchorPoint(Vec2(0.5,0));
    itemQuest->setName("QUEST");
    itemQuest->getSelectedImage()->setScale(1);
    itemQuest->selected();
    _menu_btn->addChild(itemQuest);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_quest_2.png");
        spriteIcon->setPosition(itemQuest->getContentSize().width / 2, itemQuest->getContentSize().height / 2 + 5);
        itemQuest->addChild(spriteIcon);
        
        auto labelName = Label::createWithTTF(GAME_TEXT("t_ui_quest"), GAME_FONT, 22);
        labelName->setDimensions(itemQuest->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(itemQuest->getContentSize().width / 2, 5) );
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTag(10000);
        itemQuest->addChild(labelName);
    }

    // item 장비
    auto itemEquip = MafNode::MafMenuItemSprite::create(Sprite::create(str), Sprite::create(strSelect), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemEquip->setAnchorPoint(Vec2(0.5,0));
    itemEquip->setName("EQUIP");
    itemEquip->getSelectedImage()->setScale(1);
    _menu_btn->addChild(itemEquip);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_equip.png");
        spriteIcon->setPosition(itemEquip->getContentSize().width / 2, itemEquip->getContentSize().height / 2);
        itemEquip->addChild(spriteIcon);
        
        auto labelName = Label::createWithTTF(GAME_TEXT("t_ui_equip"), GAME_FONT, 22);
        labelName->setDimensions(itemEquip->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(itemEquip->getContentSize().width / 2, 5) );
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        itemEquip->addChild(labelName);
    }
 
    // item 동료
    auto itemPet = MafNode::MafMenuItemSprite::create(Sprite::create(str), Sprite::create(strSelect), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemPet->setAnchorPoint(Vec2(0.5,0));
    itemPet->setName("PET");
    itemPet->getSelectedImage()->setScale(1);
    _menu_btn->addChild(itemPet);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_colleague.png");
        spriteIcon->setPosition(itemPet->getContentSize().width / 2, itemPet->getContentSize().height / 2 + 5);
        itemPet->addChild(spriteIcon);
        
        auto labelName = Label::createWithTTF(GAME_TEXT("t_ui_colleague"), GAME_FONT, 22);
        labelName->setDimensions(itemPet->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(itemPet->getContentSize().width / 2, 5) );
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        itemPet->addChild(labelName);
    }
    
    // item 석판
    auto itemArtifact = MafNode::MafMenuItemSprite::create(Sprite::create(str), Sprite::create(strSelect), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemArtifact->setAnchorPoint(Vec2(0.5,0));
    itemArtifact->setName("ARTIFACT");
    itemArtifact->getSelectedImage()->setScale(1);
    _menu_btn->addChild(itemArtifact);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_slab.png");
        spriteIcon->setPosition(itemArtifact->getContentSize().width / 2, itemArtifact->getContentSize().height / 2 + 5);
        itemArtifact->addChild(spriteIcon);
        
        auto labelName = Label::createWithTTF(GAME_TEXT("t_ui_slate"), GAME_FONT, 22);
        labelName->setDimensions(itemArtifact->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(itemArtifact->getContentSize().width / 2, 5) );
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        itemArtifact->addChild(labelName);
    }
    
    // item 아이템
    auto itemShop = MafNode::MafMenuItemSprite::create(Sprite::create(str),Sprite::create(strSelect), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemShop->setAnchorPoint(Vec2(0.5,0));
    itemShop->setName("SHOP");
    itemShop->getSelectedImage()->setScale(1);
    _menu_btn->addChild(itemShop);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_shop.png");
        spriteIcon->setPosition(itemShop->getContentSize().width / 2, itemShop->getContentSize().height / 2 + 5);
        itemShop->addChild(spriteIcon);
        
        auto labelName = Label::createWithTTF(GAME_TEXT("t_ui_item"), GAME_FONT, 22);
        labelName->setDimensions(itemShop->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(itemShop->getContentSize().width / 2, 5) );
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        itemShop->addChild(labelName);
    }
 
    // 던전 입장
    auto spriteMenuOn = ui::Scale9Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_off_2.png", Rect::ZERO, Rect(10,80,10,10));
    spriteMenuOn->setContentSize( Size(121, 243) );
    
    auto spriteMenuOff = ui::Scale9Sprite::create(strSelect, Rect::ZERO, Rect(10,80,10,10));;
    spriteMenuOff->setContentSize( Size(spriteMenuOff->getContentSize().width, 243) );
    
    auto itemDungeon = MafNode::MafMenuItemSprite::create(spriteMenuOn, spriteMenuOff, CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemDungeon->setAnchorPoint(Vec2(0.5,0));
    itemDungeon->setName("DUNGEON_INPUT");
    itemDungeon->getSelectedImage()->setScale(1);
    _menu_btn->addChild(itemDungeon);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_dungeon.png");
        spriteIcon->setPosition(itemDungeon->getContentSize().width / 2, itemDungeon->getContentSize().height / 2);
        itemDungeon->addChild(spriteIcon);
        
        auto labelName = Label::createWithTTF(GAME_TEXT("t_ui_dungeon"), GAME_FONT, 22);
        labelName->setDimensions(itemDungeon->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(itemDungeon->getContentSize().width / 2, 5) );
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        itemDungeon->addChild(labelName);
    }
    
    auto itemMenu = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_menu.png"), Sprite::create("Assets/ui_common/btn_menu.png"), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
    itemMenu->setPosition(visibleSize.width -itemMenu->getContentSize().width/2 - 15, topHeight  - itemMenu->getContentSize().height/2 - 15);
    itemMenu->setAnchorPoint(Vec2(0.5,0.5));
    itemMenu->setName("MENU");
    _menu_btn->addChild(itemMenu);
    
    itemQuest->setPosition(     Vec2(visibleSize.width/2-sizeMenu.width*2.5, 0));
    itemEquip->setPosition(     Vec2(visibleSize.width/2-sizeMenu.width*1.5, 0));
    itemPet->setPosition(       Vec2(visibleSize.width/2-sizeMenu.width*0.5, 0));
    itemArtifact->setPosition(  Vec2(visibleSize.width/2+sizeMenu.width*0.5, 0));
    itemShop->setPosition(      Vec2(visibleSize.width/2+sizeMenu.width*1.5, 0));
    itemDungeon->setPosition(   Vec2(visibleSize.width/2+sizeMenu.width*2.5, 0));
    
    // 저장 경고 출력
    showSaveWarning();
    
}
void DevilDomScene::initBottomAddUI()
{
    std::string strPathItemBg = "Assets/ui/menu_bottom/menu_btn_bg_off.png";
    std::string strPathItemBgSelect = "Assets/ui/menu_bottom/menu_btn_bg_on.png";
 
    auto spriteTemp = Sprite::create(strPathItemBg);
    Size sizeItemBg = spriteTemp->getContentSize();
    
    struct element
    {
        std::string tag;
        std::string strImage;
        std::string strNameKey;
        Vec2 pos;
    };
    
    element listElement[] = {
        { "PRESTIGE",   "Assets/ui/menu_bottom/menu_btn_revive_old.png",    "t_ui_revival_1",       Vec2(visibleSize.width/2-sizeItemBg.width*2.5,    sizeItemBg.height+1) },
        { "COSTUME",    "Assets/ui/menu_bottom/menu_btn_costume.png",   "t_ui_costume",         Vec2(visibleSize.width/2-sizeItemBg.width*1.5,    sizeItemBg.height+1) },
        { "MINE",       "Assets/ui/menu_bottom/menu_btn_mine.png",      "t_ui_mine",            Vec2(visibleSize.width/2-sizeItemBg.width*0.5,    sizeItemBg.height+1) },
        { "CASH",       "Assets/ui/menu_bottom/menu_btn_package.png",      "t_ui_shop_gem",        Vec2(visibleSize.width/2+sizeItemBg.width*1.0,    sizeItemBg.height+1) },
    };
    
    for (int i = 0; i < sizeof(listElement)/sizeof(element); i++ )
    {
        element ele = listElement[i];
        
        MafNode::MafMenuItemSprite *item = nullptr;
        if (ele.tag.compare("CASH") == 0 )
        {
            auto spriteMenuOn = ui::Scale9Sprite::create(strPathItemBg, Rect::ZERO, Rect(55.5,10,10,10));
            spriteMenuOn->setContentSize( Size(242, spriteMenuOn->getContentSize().height) );
            
            auto spriteMenuOff = ui::Scale9Sprite::create(strPathItemBgSelect, Rect::ZERO, Rect(55.5,10,10,10));;
            spriteMenuOff->setContentSize( Size(242, spriteMenuOff->getContentSize().height) );
            
            item = MafNode::MafMenuItemSprite::create(spriteMenuOn,  spriteMenuOff, CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
        }
        else
        {
            item = MafNode::MafMenuItemSprite::create(Sprite::create(strPathItemBg),  Sprite::create(strPathItemBgSelect), CC_CALLBACK_1(DevilDomScene::onButtonTouched, this));
        }
        
        item->setAnchorPoint(Vec2(0.5,0));
        item->setName(ele.tag);
        item->getSelectedImage()->setScale(1);
        item->setPosition(ele.pos);
        _menu_btn->addChild(item);
        
        auto labelName = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 22);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(item->getContentSize().width / 2, 5) );
        labelName->setDimensions(item->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        item->addChild(labelName);
        
        auto sprIcon = Sprite::create(ele.strImage);
        sprIcon->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2 + 5);
        item->addChild(sprIcon);
        if (ele.tag.compare("PRESTIGE") == 0)
        {
            sprIcon->setColor(Color3B(255,124,229));
        }
        // new icon
        if ( ele.tag.compare("CASH") == 0 )
        {
            if ( CashManager::getInstance()->isNewIcon(E_PLACE::DG_DEVIL_DOM) == true )
            {
                auto spriteIcon = Sprite::create("Assets/ui_common/menu_new.png");
                spriteIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                spriteIcon->setPosition(0, item->getContentSize().height);
                spriteIcon->setTag(10001);
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
        else if ( ele.tag.compare("COSTUME") == 0 )
        {
            bool bShow = UserDefault::getInstance()->getBoolForKey(KEY_REDDOT_COSTUME_SKIN1, false);
            if ( bShow == false )
            {
                
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(10, item->getContentSize().height - 10);
                item->addChild(uiReddot);
                
            }
        }
    }
}

void DevilDomScene::drawFloor(){
    
    int revival = UserInfoManager::getInstance()->getRevival(_ePlace);
    std::string str = "";
    if( revival % 10 == 1 ) str = GAME_TEXTFORMAT("t_ui_dungeon_msg_8", revival);
    else if( revival % 10 == 2 ) str = GAME_TEXTFORMAT("t_ui_dungeon_msg_9", revival);
    else if( revival % 10 == 3 ) str = GAME_TEXTFORMAT("t_ui_dungeon_msg_10", revival);
    else
    {
        str = GAME_TEXTFORMAT("t_ui_dungeon_msg_11", revival);
    }
    _lbFloor->setString(str);
    
    std::string strFloor = MafUtils::toString(UserInfoManager::getInstance()->getFloor(_ePlace));
    MafUtils::addCommaToString(strFloor, 3, ',');
    _lbWave->setString(MafUtils::format("%sm %d/5",strFloor.c_str(),UserInfoManager::getInstance()->getFloorCount(_ePlace)));
}

void DevilDomScene::ResultCheckCostumeCoin(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
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
    
    if ( bError == false && jsonParser.HasMember("_coin") == false )
    {
        bError = true;
    }
 
    if ( bError == true )
    {
        return;
    }
    
    int nCoin = jsonParser["_coin"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
}

void DevilDomScene::requestCheckCostumeCoin()
{
    PopupLoading::show();
    
    std::string url = "/costume/currentcoin";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_email", "");
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        ResultCheckCostumeCoin(response,dd);
    };
    req->send(cb);
}

#pragma mark-
void DevilDomScene::update(float delta)
{
    // mineral
    {
        MineralManager::getInstance()->update(delta);
    }
}

void DevilDomScene::scheduleMove(float dt)
{
    moveMap();
}

void DevilDomScene::scheduleTimer(float dt){
    
    _fTimer += dt;
    _fTimer1Min += dt;
    _fBuffRedrawTimer+=dt;
    _fTimerQuestSave+=dt;
    _fAdsRedrawTimer +=dt;
    
    if(_fBuffRedrawTimer >= 1){
        _fBuffRedrawTimer = 0;
        drawBuffTimer();
    }
    
    if(_fTimerQuestSave>=2){
        _fTimerQuestSave-=2;
        SaveManager::saveQuestTime(E_PLACE::DG_DEVIL_DOM);
        ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::PLAYTIME_S, 2);
    }
    
    if(_fAdsRedrawTimer>=1){
        _fAdsRedrawTimer-=1;
        UserInfoManager::getInstance()->setTimeAdsAllMinus();
        UserInfoManager::getInstance()->setTimeMyhomeAdsAllMinus();
    }
    if(_fTimer1Min >= 60)
    {
        ServerMissionManager::getInstance()->checkMidnightReset();
        _fTimer1Min = 0.0f;
    }
}

void DevilDomScene::createMapIdx()
{
    int nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
    
    int nType = 1;
    int nIdx = 1;
    if ( nFloor % 100 == 0 || nFloor % 1000 == 0)
    {
        nType = 2;
    }
    
    if( nFloor>=DEVIL_FLOOR_LIMIT )
    {
        nIdx = 9;
        nType = 2; //한계층수이면 보스몹
    }
    else
    {
        nIdx = (nFloor - 1) / 1000;
    }
    
    _nMapIdx = nIdx;
    _nMapType = nType;
}

void DevilDomScene::createMap()
{
    _sizeMapPattern = Size(getContentSize().width, 427);
    
    std::string strBGDefaultPath = "";
    std::string listBG[] = {
        "Assets/battle_bg/devil/map_01_%d.png",
        "Assets/battle_bg/devil/map_02_%d.png",
        "Assets/battle_bg/devil/map_03_%d.png",
        "Assets/battle_bg/devil/map_04_%d.png",
        "Assets/battle_bg/devil/map_05_%d.png",
        "Assets/battle_bg/devil/map_06_%d.png",
        "Assets/battle_bg/devil/map_07_%d.png",
        "Assets/battle_bg/devil/map_08_%d.png",
        "Assets/battle_bg/devil/map_09_%d.png",
        "Assets/battle_bg/devil/map_10_%d.png"
    };
    
    strBGDefaultPath = listBG[_nMapIdx];
 
    //
    int nMapCount = 2;
    double nBGPosX = 0.0;
    double nBGPosY = getContentSize().height;
    for ( int i = 0; i < nMapCount; i++)
    {
        std::string strBGPath = MafUtils::format(strBGDefaultPath.c_str(), _nMapType);
        
        //
        auto spriteBG = Sprite::create(strBGPath);
        spriteBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteBG->setPosition(nBGPosX, nBGPosY);
        spriteBG->getTexture()->setTexParameters(texParams);
        spriteBG->setScale(3.5);
        addChild(spriteBG, E_ZORER::MAP);
        _vecMapNode.pushBack(spriteBG);
        
        //
        nBGPosX += spriteBG->getBoundingBox().size.width - 5;
    }
}

void DevilDomScene::removeMap()
{
    while (!_vecMapNode.empty())
    {
        _vecMapNode.back()->removeFromParentAndCleanup(false);
        _vecMapNode.popBack();
    }
}

void DevilDomScene::recycleMap()
{
    auto spriteMapFront = _vecMapNode.front();
    auto spriteMapBack = _vecMapNode.back();
 
    //
    spriteMapFront->setPositionX(spriteMapBack->getPosition().x + spriteMapBack->getBoundingBox().size.width - 5);
    
    //
    _vecMapNode.erase(0);
    _vecMapNode.pushBack(spriteMapFront);
}

void DevilDomScene::moveMap()
{
    double nSpeedDefault = C_MOVE_MAP_SPEED;
    double nSpeed = UserInfoManager::getInstance()->getUserSpeed(_hero->getPlace(), false);
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



#pragma mark-

void DevilDomScene::createHero(){
    
    _hero = UnitBehaviorHero::create(_ePlace);
    _hero->setAttackCallBackListener(CallFunc::create(CC_CALLBACK_0(DevilDomScene::heroAttack,this)));
    _hero->setMoveCallBackListener(CallFunc::create(CC_CALLBACK_0(DevilDomScene::heroMoveDone,this)));
    _hero->setUpstairsCallBackListener(CallFunc::create(CC_CALLBACK_0(DevilDomScene::upstairsDungeon,this)));
    _hero->setMotionDevil();
    _hero->setPositionHero(this, _sizeMapPattern.height);
    addChild(_hero,E_ZORER::HERO);
}

void DevilDomScene::createEnemy(){
    if(_enemy==nullptr){
        _enemy = UnitBehaviorMonster::create(_ePlace);
        addChild(_enemy,E_ZORER::MONSTER);
    }
    _enemy->setMotion();
    _enemy->setMonsterHp("x","x");
    _enemy->setPositionMonster(this, _sizeMapPattern.height-5);
}

void DevilDomScene::createPet(E_PET pet){
    
    for(auto child:_vecPet){
        if(child->_ePet == pet){
            return;
        }
    }
    
    if(pet == E_PET::D_1_PENGUIN){
        Penguin *_pet = Penguin::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-120,36));
        _pet->setScale(DevilScale);
        _hero->getLayerCharacter()->addChild(_pet, 10 + 1);
        _vecPet.pushBack(_pet);
    }
    else if(pet == E_PET::D_2_HEDGEHOG){
        Hedgehog *_pet = Hedgehog::create();
        auto cool =  PetNewDevilManager::getInstance()->getPetCoolTime(_pet->_ePet);
        _pet->setCoolTime(cool,false);
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-90,36));
        _pet->setScale(DevilScale);
        _hero->getLayerCharacter()->addChild(_pet, 10 + 1);
        _vecPet.pushBack(_pet);
    }
    else if(pet == E_PET::D_3_GATEKEEPER){
        Gatekeeper *_pet = Gatekeeper::create();
        auto cool = PetNewDevilManager::getInstance()->getPetCoolTime(_pet->_ePet);
        _pet->setCoolTime(cool,true);
        _pet->setPlace(_ePlace);
        _pet->_onBuffEffect = CC_CALLBACK_2(UnitBehaviorHero::onGatekeeperBuff, _hero);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-165,24));
        _pet->setScale(DevilScale);
        _hero->getLayerCharacter()->addChild(_pet, 10 + 1);
        _vecPet.pushBack(_pet);
    }
    else if(pet == E_PET::D_4_FLASH){
        Flash *_pet = Flash::create();
        _pet->setPlace(_ePlace);
        _pet->setAnchorPoint(Vec2(0,0));
        _pet->setPosition(Vec2(-240,45));
        _pet->setScale(DevilScale);
        _hero->getLayerCharacter()->addChild(_pet, 10 + 1);
        _vecPet.pushBack(_pet);
    }
    
}

void DevilDomScene::startDungeon(){
    
    _hero->actionMove();
    for(auto pet : _vecPet)
        pet->move();
    
    if(UserInfoManager::getInstance()->getFloor(_ePlace) >=DEVIL_FLOOR_LIMIT){
        return;
    }
    
    createEnemy();
    _enemy->actionMove();
    
    schedule(schedule_selector(DevilDomScene::scheduleMove));
}

void DevilDomScene::heroMoveDone(){
    _hero->heroStopAllAction();
    heroAttack();
}

void DevilDomScene::heroAttack(){

    if ( UserInfoManager::getInstance()->getCurrentPlace() != E_PLACE::DG_DEVIL_DOM )
    {
        return;
    }
    
    if ( _enemy == nullptr ) {
        _hero->actionRun();
        return;
    }
    
    if(_enemy->isDie() == true){
        if ( UserInfoManager::getInstance()->getFloor(_ePlace) >= DEVIL_FLOOR_LIMIT )
        {
            _enemy->stopAllActions();
            _enemy->removeFromParent();
            _enemy = nullptr;
        }
        
        int fFloorCount = UserInfoManager::getInstance()->getFloorCount(_ePlace);
        if(fFloorCount==1)
        {
            schedule(schedule_selector(DevilDomScene::scheduleMove));
            
            //
            moveScreenEffect(true);
            _hero->actionMoveUpstairs();
            for(auto pet : _vecPet)
                pet->move();
        }
        else
        {
            startDungeon();
        }
    }
    else
    {
        unschedule(schedule_selector(DevilDomScene::scheduleMove));
        
        //
        for(auto pet : _vecPet){
            pet->attack(*_enemy);
        }
        _hero->actionAttack(*_enemy);
    }
}

void DevilDomScene::upstairsDungeon()
{
    unschedule(schedule_selector(DevilDomScene::scheduleMove));

    //
    _hero->setPositionHero(this, _sizeMapPattern.height);
    moveScreenEffect(false);
    startDungeon();
    
    drawFloor();
    
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    
    int tempFloor = floor-1;
    if ( tempFloor%1000 == 0 )
    {
        std::string strPointAdd = "2";
        
        ItemsMoneyManager::getInstance()->setPoint(std::atoi(strPointAdd.c_str()));
        RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::POINT, strPointAdd);
    }

    
    if(floor==1001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_1000);
        
        if(!PetNewDevilManager::getInstance()->getPet(E_PET::D_1_PENGUIN))
        {
            PetNewDevilManager::getInstance()->setPetLevel(E_PET::D_1_PENGUIN, 1);
            PetNewDevilManager::getInstance()->saveData();
            DevilDomScene::getInstance()->createPet(E_PET::D_1_PENGUIN);
        }
    }
    else if(floor==2001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_2000);
        
        if(!PetNewDevilManager::getInstance()->getPet(E_PET::D_2_HEDGEHOG))
        {
            PetNewDevilManager::getInstance()->setPetLevel(E_PET::D_2_HEDGEHOG, 1);
            PetNewDevilManager::getInstance()->saveData();
            DevilDomScene::getInstance()->createPet(E_PET::D_2_HEDGEHOG);
        }
    }
    else if(floor==3001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_3000);
        
        if(!PetNewDevilManager::getInstance()->getPet(E_PET::D_3_GATEKEEPER))
        {
            PetNewDevilManager::getInstance()->setPetLevel(E_PET::D_3_GATEKEEPER, 1);
            PetNewDevilManager::getInstance()->saveData();
            DevilDomScene::getInstance()->createPet(E_PET::D_3_GATEKEEPER);
        }
    }
    else if(floor==4001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_4000);
        
        if(!PetNewDevilManager::getInstance()->getPet(E_PET::D_4_FLASH))
        {
            PetNewDevilManager::getInstance()->setPetLevel(E_PET::D_4_FLASH, 1);
            PetNewDevilManager::getInstance()->saveData();
            DevilDomScene::getInstance()->createPet(E_PET::D_4_FLASH);
        }
    }
    else if(floor==5001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_5000);
    }
    else if(floor==6001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_6000);
    }
    else if(floor==7001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_7000);
    }
    else if(floor==8001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_8000);
    }
    else if(floor==9001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_9000);
    }
    else if(floor==10001){
        // story
        StoryManager::getInstance()->showStory((int)E_COMIC::DEVIL_10000);
    }
    
    int highFloor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    if ( highFloor < floor )
    {
        UserInfoManager::getInstance()->setHighFloor(_ePlace, floor);
    }
}

void DevilDomScene::moveScreenEffect(bool turnOn)
{
    
    if ( turnOn )
    {
        int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
        if ( floor%100 == 0 )
        {
            SoundManager::SoundBgmStart(SOUND_BGM_D_BOSS_1, true);
        }
        else
        {
            SoundManager::SoundBgmStart(SOUND_BGM_D_1, true);
        }
        
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
        }
    }
}

#pragma mark-

void DevilDomScene::createTable(){
    auto sprSize = Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_off.png");
    Size sizeMenu = sprSize->getContentSize();
    float fStartHeight = sizeMenu.height*2;
    float scrollHeight = visibleSize.height - fStartHeight - _sizeMapPattern.height - C_MIDDLE_POSITION;
    
    for ( int qq = 0; qq < C_COUNT_GAME_STATE_DEVIL; qq++ )
    {
        float scSizeBarHeight = C_BAR_SIZE;
        Size scSize = Size(visibleSize.width,scrollHeight);
        if ( qq == E_GAME_STATE::ARTIFACT )
        {
            scSize = Size(visibleSize.width,scrollHeight-80);
        }
        else if ( qq == E_GAME_STATE::SHOP )
        {
            scSize = Size(visibleSize.width,scrollHeight - scSizeBarHeight);
        }
        
        extension::ScrollView *scroll = extension::ScrollView::create(scSize);
        scroll->setDelegate(this);
        scroll->setDirection(extension::ScrollView::Direction::VERTICAL);
        scroll->setPosition(Vec2(0,fStartHeight));
        scroll->setClippingToBounds(true);
        scroll->setBounceable(false);
        addChild(scroll);
        
        _scroll[qq] = scroll;
        
        Layer *container = Layer::create();
        container->setPosition(Vec2(0,0));
        scroll->setContainer(container);
        
        int nCount = 0;
        if ( qq == E_GAME_STATE::QUEST )
        {
            _cellQuestTop = new ModelCellDevil::CellDevilQuestTop(UtilsScene::eType::scene_dungeon_devil);
            _cellQuestTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _cellQuestTop->setPosition(Vec2(getContentSize().width/2, getContentSize().height - _sizeMapPattern.height - _cellQuestTop->getContentSize().height - C_MIDDLE_POSITION));
            _cellQuestTop->autorelease();
            _cellQuestTop->setVisible(false);
            _cellQuestTop->setCallbackTab(CC_CALLBACK_1(DevilDomScene::callbackQuestTab, this));
            addChild(_cellQuestTop,E_ZORER::MAP);
            
            scSize = Size(visibleSize.width, scrollHeight - _cellQuestTop->getContentSize().height);
            scroll->setViewSize(scSize);
            
            nCount = C_COUNT_QUEST_DEVIL;
            for ( int i = 1; i <= nCount; i++ )
            {
                ModelCellDevil::CellDevilQuest *cell = new ModelCellDevil::CellDevilQuest(_ePlace, (E_QUEST)(i));
                cell->autorelease();
                cell->setPosition( Vec2(0, scSizeBarHeight * nCount - i * scSizeBarHeight) );
                container->addChild(cell);
                _vecCellQuest.pushBack(cell);
            }
        }
        else if ( qq == E_GAME_STATE::EQUIP )
        {
            _cellEquipTop = new ModelCellDevil::CellEquipTop();
            _cellEquipTop->setPosition(Vec2(0, visibleSize.height - _sizeMapPattern.height - _cellEquipTop->getContentSize().height - C_MIDDLE_POSITION));
            _cellEquipTop->autorelease();
            _cellEquipTop->setVisible(false);
            _cellEquipTop->setCallbackTab(CC_CALLBACK_1(DevilDomScene::callbackEquipTab, this));
            addChild(_cellEquipTop,E_ZORER::MAP);
            
            scSize = Size(visibleSize.width, scrollHeight - _cellEquipTop->getContentSize().height);
            scroll->setViewSize(scSize);
            nCount = C_COUNT_EQUIP_TYPE_DEVIL;
            for ( int i = 1; i <= nCount; i++ )
            {
                ModelCellDevil::CellDevilEquip *cell = new ModelCellDevil::CellDevilEquip(_ePlace, (E_EQUIP_TYPE)(i-1));
                cell->_onShowDetailListener = [&](cocos2d::Ref *sender){
                    auto popup = PopupEquipDetail::create((ModelCellDevil::CellDevilEquip*)sender);
                    popup->show();
                };
                if(cell->_eEquipType==E_EQUIP_TYPE::E_D_HEAD)
                    cell->_onCompleteListener = CC_CALLBACK_0(DevilDomScene::reDrawQuest, this);
                cell->autorelease();
                cell->setPosition( Vec2(0, scSizeBarHeight * nCount - i * scSizeBarHeight) );
                container->addChild(cell);
                _vecCellEquip.pushBack(cell);
            }
        }
        else if ( qq == E_GAME_STATE::ARTIFACT )
        {
            _topArtifact = new ModelCellDevil::CellDevilArtifactTop();
            _topArtifact->_onSelectListener = CC_CALLBACK_1(DevilDomScene::showArtifactTable,this);
            _topArtifact->setPosition(Vec2(0, visibleSize.height - _sizeMapPattern.height - _topArtifact->getContentSize().height - C_MIDDLE_POSITION));
            addChild(_topArtifact,E_ZORER::MAP);
            _topArtifact->autorelease();
            
            nCount = (int)DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_1_ATK).size();
            for(int i=0;i<C_COUNT_ARTIFACT_TYPE_SORT; i++){
                auto vecArtifact = DataManager::GetDevilArtifactDataType((E_D_ARTIFACT_TYPE)i);
                if(i==(int)E_D_ARTIFACT_TYPE::E_D_TYPE_4_ATK_SPD){
                    auto vecEtc = DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE::E_D_TYPE_5_MOVE_SPD);
                    vecArtifact.pushBack(vecEtc);
                }
                int newCount = (int)vecArtifact.size();
                for ( int j = 1; j <= newCount; j++ )
                {
                    auto info =  vecArtifact.at(j-1);
                    ModelCellDevil::CellDevilArtifact *cell = new ModelCellDevil::CellDevilArtifact(_ePlace, *info);
                    cell->_onCompleteListener = CC_CALLBACK_1(DevilDomScene::reDrawArtifact,this);
                    cell->autorelease();
                    cell->setPosition( Vec2(0, scSizeBarHeight * newCount - j * scSizeBarHeight) );
                    container->addChild(cell);
                    _vecCellDevilArtifact[i].pushBack(cell);
                }
                
                // sort
                std::sort(_vecCellDevilArtifact[i].begin(), _vecCellDevilArtifact[i].end(), CC_CALLBACK_2(DevilDomScene::sortArtifactList, this));
            }
            
            createArtifactTable(false);
        }
        else if ( qq == E_GAME_STATE::PET )
        {
            nCount = C_COUNT_PET_DEVIL;
            for( int i = 1; i <= nCount; i++ )
            {
                ModelCellDevil::CellDevilPet *cell = new ModelCellDevil::CellDevilPet(_ePlace, (E_PET)(i+C_COUNT_PET));
                cell->autorelease();
                cell->setPosition( Vec2(0, scSizeBarHeight * nCount-i * scSizeBarHeight) );
                container->addChild(cell);
                _vecCellPet.pushBack(cell);
            }
        }
        else if ( qq == E_GAME_STATE::DUNGEON )
        {
            scSizeBarHeight = 150;
            bool event = false;
            if(EventManager::getInstance()->isActivatedEvent(5) || EventManager::getInstance()->isActivatedEvent(8))
            {
               event = true;
            }
            
            _uiContainerDungeon = LayerNormalDungeon::create(E_PLACE::DG_DEVIL_DOM);
            _uiContainerDungeon->setContentSize(Size(scSize.width, scSize.height));
            _uiContainerDungeon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            _uiContainerDungeon->setPosition(Vec2(scSize.width / 2, sizeMenu.height*2));
            addChild(_uiContainerDungeon);
        }

        
        container->setContentSize( Size(visibleSize.width, scSizeBarHeight * nCount) );
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
        
        _scroll[qq]->setVisible(false);
    }
    
    //스크롤 상단 메뉴 안눌리게 막는장치
    extension::ControlButton *item = extension::ControlButton::create();
    item->addTargetWithActionForControlEvents(this, nullptr, extension::Control::EventType::TOUCH_UP_INSIDE);
    item->setPosition(visibleSize.width/2,visibleSize.height-_sizeMapPattern.height/2 - C_MIDDLE_POSITION*0.5);
    item->setPreferredSize(Size(visibleSize.width,_sizeMapPattern.height + C_MIDDLE_POSITION));
    item->setScaleRatio(1);
    item->setOpacity(0);
    addChild(item,E_ZORER::MAP);
    
    //스크롤 하단 메뉴 안눌리게 막는장치
    item = extension::ControlButton::create();
    item->addTargetWithActionForControlEvents(this, nullptr, extension::Control::EventType::TOUCH_UP_INSIDE);
    item->setAnchorPoint(Vec2(0.5, 0));
    item->setPosition(visibleSize.width/2, 0);
    item->setPreferredSize(Size(visibleSize.width, 150));
    item->setScaleRatio(1);
    item->setOpacity(255);
    addChild(item,E_ZORER::MAP);
}

int DevilDomScene::createArtifactTable(bool redraw){
    
    extension::ScrollView* scroll = _scroll[E_GAME_STATE::ARTIFACT];
    Layer* container = (Layer*)scroll->getContainer();
    //Vector<Cell*> _vecCell;
    _vecCellDevilArtifactForOriginal.clear();
    

    for(int i=E_ARTIFACT::ARTI_79_EVOL14; i<=E_ARTIFACT::ARTI_82_EVOL14; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_79_EVOL14){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_74_EVOL13)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_74_EVOL13))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_80_EVOL14){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_75_EVOL13)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_75_EVOL13))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_81_EVOL14){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_76_EVOL13)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_76_EVOL13))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_82_EVOL14){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_77_EVOL13)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_77_EVOL13))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_74_EVOL13; i<=E_ARTIFACT::ARTI_77_EVOL13; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_74_EVOL13){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_69_EVOL12)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_69_EVOL12))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_75_EVOL13){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_70_EVOL12)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_70_EVOL12))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_76_EVOL13){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_71_EVOL12)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_71_EVOL12))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_77_EVOL13){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_72_EVOL12)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_72_EVOL12))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_69_EVOL12; i<=E_ARTIFACT::ARTI_72_EVOL12; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_69_EVOL12){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_64_EVOL11)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_64_EVOL11))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_70_EVOL12){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_65_EVOL11)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_65_EVOL11))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_71_EVOL12){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_66_EVOL11)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_66_EVOL11))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_72_EVOL12){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_67_EVOL11)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_67_EVOL11))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_64_EVOL11; i<=E_ARTIFACT::ARTI_67_EVOL11; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_64_EVOL11){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_59_EVOL10)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_59_EVOL10))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_65_EVOL11){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_60_EVOL10)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_60_EVOL10))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_66_EVOL11){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_61_EVOL10)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_61_EVOL10))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_67_EVOL11){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_62_EVOL10)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_62_EVOL10))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_59_EVOL10; i<=E_ARTIFACT::ARTI_62_EVOL10; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_59_EVOL10){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_54_EVOL9)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_54_EVOL9))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_60_EVOL10){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_55_EVOL9)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_55_EVOL9))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_61_EVOL10){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_56_EVOL9)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_56_EVOL9))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_62_EVOL10){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_57_EVOL9)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_57_EVOL9))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    for(int i=E_ARTIFACT::ARTI_54_EVOL9; i<=E_ARTIFACT::ARTI_57_EVOL9; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_54_EVOL9){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_46_EVOL8)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_46_EVOL8))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_55_EVOL9){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_47_EVOL8)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_47_EVOL8))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_56_EVOL9){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_48_EVOL8)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_48_EVOL8))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_57_EVOL9){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_49_EVOL8)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_49_EVOL8))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_46_EVOL8; i<=E_ARTIFACT::ARTI_49_EVOL8; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_46_EVOL8){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_40_EVOL7)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_40_EVOL7))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_47_EVOL8){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_41_EVOL7)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_41_EVOL7))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_48_EVOL8){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_42_EVOL7)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_42_EVOL7))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_49_EVOL8){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_43_EVOL7)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_43_EVOL7))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_40_EVOL7; i<=E_ARTIFACT::ARTI_43_EVOL7; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_40_EVOL7){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_35_EVOL6)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_35_EVOL6))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_41_EVOL7){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_36_EVOL6)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_36_EVOL6))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_42_EVOL7){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_37_EVOL6)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_37_EVOL6))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_43_EVOL7){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_38_EVOL6)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_38_EVOL6))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
    
    for(int i=E_ARTIFACT::ARTI_35_EVOL6; i<=E_ARTIFACT::ARTI_38_EVOL6; i++){
        E_ARTIFACT ea = (E_ARTIFACT)(i);
        bool pass = false;
        if(ea == E_ARTIFACT::ARTI_35_EVOL6){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_28_EVOL5)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_28_EVOL5))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_36_EVOL6){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_29_EVOL5)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_29_EVOL5))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_37_EVOL6){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_30_EVOL5)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_30_EVOL5))
                pass= true;
        }
        else if(ea == E_ARTIFACT::ARTI_38_EVOL6){
            if(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_31_EVOL5)>=DataManager::GetArtifactMaxLevel(E_ARTIFACT::ARTI_31_EVOL5))
                pass= true;
        }
        
        if(!pass)
            continue;
        
        ModelCellDevil::CellDevilArtifactForOriginal *cell2 = new ModelCellDevil::CellDevilArtifactForOriginal(_ePlace, ea);
        cell2->autorelease();
        _vecCellDevilArtifactForOriginal.pushBack(cell2);
    }
        
    int nCount = (int)_vecCellDevilArtifactForOriginal.size();
    int i=1;
    for(auto child : _vecCellDevilArtifactForOriginal){
        child->setPosition(Vec2(0,C_BAR_SIZE*nCount-((i)*C_BAR_SIZE)));
        container->addChild(child);
        i++;
    }
    
    if(redraw)
        container->setContentSize(Size(visibleSize.width,C_BAR_SIZE*nCount));
    
    return nCount;
}

void DevilDomScene::showTable(){
    
    extension::ScrollView *showScroll;
    for(int i=0; i<C_COUNT_GAME_STATE_DEVIL; i++){
        if(i == _eGameState){
            _scroll[i]->setVisible(true);
            showScroll = _scroll[i];
        }
        else{
            _scroll[i]->setVisible(false);
        }
    }
    
    if(_eGameState== E_GAME_STATE::QUEST){
        
        _cellQuestTop->setVisible(true);
        
        int curQuest = E_QUEST::QUEST_1;
        for(int i=1;i<=C_COUNT_QUEST_DEVIL;i++){
            bool open = UserInfoManager::getInstance()->getQuest(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i);
            if(open)
                curQuest = i;
        }
        float offsetY = -(showScroll->getContentSize().height - showScroll->getViewSize().height - (C_BAR_SIZE * (curQuest-2)));
        if (offsetY > showScroll->maxContainerOffset().y)
            offsetY = showScroll->maxContainerOffset().y;
        if (offsetY < showScroll->minContainerOffset().y)
            offsetY = showScroll->minContainerOffset().y;
        showScroll->setContentOffset(Vec2(0,offsetY));
    }
    else{
        _cellQuestTop->setVisible(false);
    }
    
    if(_eGameState== E_GAME_STATE::EQUIP){
        _cellEquipTop->setVisible(true);
        for(auto cell : _vecCellEquip){
            cell->setVisible(true);
            cell->startSchedule();
        }        
    }
    else{
        _cellEquipTop->setVisible(false);
        for(auto cell : _vecCellEquip){
            cell->setVisible(false);
            cell->stopSchedule();
        }
    }
    
    if(_eGameState== E_GAME_STATE::ARTIFACT){
        _topArtifact->setVisible(true);
        showArtifactTable(_topArtifact->_currentType);
    }
    else{
        _topArtifact->setVisible(false);
    }
    if(_eGameState== E_GAME_STATE::DUNGEON){
        _uiContainerDungeon->setVisible(true);
    }
    else{
        _uiContainerDungeon->setVisible(false);
    }
}



#pragma mark-

void DevilDomScene::drawQuestTime(){
    
    CallFunc *call =  CallFunc::create([&]() {
        for(auto cell : _vecCellQuest)
            cell->startResume();
    });
    this->runAction(call);
}

void DevilDomScene::drawEquip(){
    _hero->setChangeParts();
}

void DevilDomScene::reDrawEquip(){
    for(auto cell : _vecCellEquip){
        if(cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON){
            cell->setState(ModelCell::E_CHANGE_STATE::EARN);
            break;
        }
    }
}

void DevilDomScene::reDrawQuest(){
    for(auto cell : _vecCellQuest){
//        cell->setSpend(false);
        cell->setState(ModelCell::E_CHANGE_STATE::EARN);
    }
}

void DevilDomScene::reDrawArtifact(E_D_ARTIFACT_TYPE type){
    //보물 획득시 갱신해야할것.
    if ( type != E_D_ARTIFACT_TYPE::E_D_TYPE_6_ARTIFACT )
    {
        if ( type == E_D_ARTIFACT_TYPE::E_D_TYPE_1_ATK )
        {
            reDrawEquip();
        }
        else if ( type == E_D_ARTIFACT_TYPE::E_D_TYPE_3_GOLD )
        {
            reDrawQuest();
        }
        else if ( type == E_D_ARTIFACT_TYPE::E_D_TYPE_4_ATK_SPD )
        {
            drawEquip();
        }
        
        if ( type == E_D_ARTIFACT_TYPE::E_D_TYPE_5_MOVE_SPD )
        {
            type = E_D_ARTIFACT_TYPE::E_D_TYPE_4_ATK_SPD;
        }
        
        // sort
        std::sort(_vecCellDevilArtifact[type].begin(), _vecCellDevilArtifact[type].end(), CC_CALLBACK_2(DevilDomScene::sortArtifactList, this));
        
        createArtifactTable(true);
        showArtifactTable(type);
    }
}

void DevilDomScene::showArtifactTable(E_D_ARTIFACT_TYPE type){
    
    auto scroll = _scroll[(int)E_GAME_STATE::ARTIFACT];
    
    for(int i=0;i<C_COUNT_ARTIFACT_TYPE_SORT; i++){
        for(auto child : _vecCellDevilArtifact[i]){
            child->setVisible(false);
        }
    }
    for(auto child : _vecCellDevilArtifactForOriginal){
        child->setVisible(false);
    }
    
    int nCount = 0;
    if(type != E_D_ARTIFACT_TYPE::E_D_TYPE_6_ARTIFACT){
        nCount = (int)_vecCellDevilArtifact[type].size();//(int)vecArtifact.size();
        for(int i=1; i<=nCount; i++){
            auto cell = _vecCellDevilArtifact[type].at(i-1);
            cell->setPosition(Vec2(0,C_BAR_SIZE*nCount-((i)*C_BAR_SIZE)));
            cell->setVisible(true);
            cell->drawArtifact();
        }
    }
    else{
        nCount = (int)_vecCellDevilArtifactForOriginal.size();//(int)vecArtifact.size();
        for(int i=1; i<=nCount; i++){
            auto cell = _vecCellDevilArtifactForOriginal.at(i-1);
            cell->setPosition(Vec2(0,C_BAR_SIZE*nCount-((i)*C_BAR_SIZE)));
            cell->setVisible(true);
        }
    }
    
    auto container = scroll->getContainer();
    container->setContentSize(Size(visibleSize.width,C_BAR_SIZE*nCount));
    auto scSize = scroll->getContentSize();
    
    float offsetY = scroll->minContainerOffset().y;
    scroll->setContentOffset(Vec2(0,offsetY));
}

bool DevilDomScene::sortArtifactList(cocos2d::Ref *a, cocos2d::Ref *b)
{
    ModelCellDevil::CellDevilArtifact *pObj1 = static_cast<ModelCellDevil::CellDevilArtifact *>(a);
    ModelCellDevil::CellDevilArtifact *pObj2 = static_cast<ModelCellDevil::CellDevilArtifact *>(b);
    
    bool bHave1 = UserInfoManager::getInstance()->isArtifactHaveDevil(pObj1->_info->_idx);
    bool bHave2 = UserInfoManager::getInstance()->isArtifactHaveDevil(pObj2->_info->_idx);
    
    if ( bHave1 != bHave2 )
        return bHave1 < bHave2;
    else
        return pObj1->_info->_idx < pObj2->_info->_idx;
}

void DevilDomScene::drawBuffTimer(){
    
    bool bContinue = false;
    E_BUFF_TYPE eType[4] = {
        E_BUFF_TYPE::BUFF_D_ATK_DMG,
        E_BUFF_TYPE::BUFF_D_QUEST_GOLD,
        E_BUFF_TYPE::BUFF_D_MOVE_SPD,
        E_BUFF_TYPE::BUFF_D_ATK_SPD,
    };
    
    for(int i=0; i<sizeof(eType)/sizeof(E_BUFF_TYPE); i++)
    {
        int time = UserInfoManager::getInstance()->getBuffTime(eType[i]);
        
        int adsTime = 0;
        
        switch (eType[i])
        {
            case E_BUFF_TYPE::BUFF_D_ATK_DMG: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ATK); break;
            case E_BUFF_TYPE::BUFF_D_QUEST_GOLD: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_GOLD); break;
            case E_BUFF_TYPE::BUFF_D_MOVE_SPD: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_MOVE); break;
            case E_BUFF_TYPE::BUFF_D_ATK_SPD: adsTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ASPD); break;
                
            default:
                break;
        }
        
        if ( time > 0 )
        {
            UserInfoManager::getInstance()->setBuffTime(eType[i],-1);
                        
            //
            time-=1;
            
            //
            int nDay = time / 86400;
            int nHour = time % 86400 / 3600;
            int nMin = time % 3600 / 60;
            int nSec = time % 60;
            
            bool bSec = false;
            std::string strText = "";
            if ( nDay != 0 )        strText = MafUtils::format("%dD", nDay);
            else if ( nHour != 0 )  strText = MafUtils::format("%dH", nHour);
            else if ( nMin != 0 )   strText = MafUtils::format("%dM", nMin);
            else
            {
                bSec = true;
                strText = MafUtils::format("%dS", nSec);
            }
            
            _lbBuff[eType[i]]->setString(strText);
            _sprBuff[eType[i]]->setOpacity(255);
            
            if ( bSec == true )
                _lbBuff[eType[i]]->setColor(COLOR_GEM);
            else
                _lbBuff[eType[i]]->setColor(Color3B::WHITE);
            
            if ( time == 0 )
            {
                reDrawQuest();
                drawEquip();
            }
        }
        else
        {
            _sprBuff[eType[i]]->setOpacity(100);
            _lbBuff[eType[i]]->setString("");
        }
        
        auto adsFx = _sprBuff[eType[i]]->getChildByName("ADS_FX");
        
        
        if(adsTime <= 0)
        {
            if(adsFx != nullptr)
            {
                adsFx->setVisible(true);
            }
        }
        else
        {
            if(adsFx != nullptr)
            {
                adsFx->setVisible(false);
            }
        }
    }
}


#pragma mark-
void DevilDomScene::removeNodeTemp(){
    for(auto child : _vecNodeTemp){
        child->removeFromParentAndCleanup(true);
    }
    _vecNodeTemp.clear();
}

void DevilDomScene::onRemovePopupPet(cocos2d::Ref* pSender)
{
    _itemPetGrade = nullptr;
    _popupPet->hide();
}

#pragma mark-
void DevilDomScene::requestPetGradeCostumeCoin(){
    
    
    int grade = PetNewDevilManager::getInstance()->getPetGrade(_selectPet->_ePet);
    int spendCoin = PetNewDevilManager::getInstance()->getPetGradeUpgradePrice(_selectPet->_ePet, grade);
    auto coin = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
    int myCoin = atoi(coin.c_str());
    
    if(grade > C_MAX_PET_GRADE)
        return;
    
    if(myCoin < spendCoin)
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
        popup->show();
        return;
    }
        
    
    PopupLoading::show();
    
    std::string url = "/costume/consumecoin";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_category", 2);
    req->setData("_coin", MafUtils::toString(spendCoin).c_str());
    req->setData("_key", "0");
    req->setData("_type", (int)_selectPet->_ePet);
    req->setData("_num", grade);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        ResultPetGradeCostumeCoin(response,dd);
    };
    req->send(cb);
}
void DevilDomScene::ResultPetGradeCostumeCoin(network::HttpResponse* response,std::string &data){
    
    PopupLoading::hide();
    
    bool bSuc = true;
    if (!response)
        bSuc = false;
    if(!response->isSucceed())
        bSuc = false;
    if(response->getResponseCode() != 200)
        bSuc = false;
    
    if(!bSuc)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(data.length()>0){
        rapidjson::Document jsondoc;
        jsondoc.ParseInsitu<0>((char*)(const char*)data.c_str()).HasParseError();
        auto suc = jsondoc[("_suc")].GetBool();
        if(!suc)
            return;
        auto costumeCoin = jsondoc[("_coin")].GetInt();
        auto current = getInstance();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, costumeCoin);
        
        
        current->onSuccessPetGrade();
    }
}
void DevilDomScene::onSuccessPetGrade(){
    
    int grade = PetNewDevilManager::getInstance()->getPetGrade(_selectPet->_ePet);
    grade++;
    PetNewDevilManager::getInstance()->setPetGrade(_selectPet->_ePet, grade);
    PetNewDevilManager::getInstance()->saveData();
    
    _selectPet->_grade = grade;
    
    //팝업, 테이블 다시그리기
    _selectPet->drawUpgrade();
    
    
    auto petdevil = _popupPet->_selectPetDevil;
    onRemovePopupPet(nullptr);
    
    _popupPet = PopupPetDevil::create(*petdevil);
    _popupPet->show();
}
#pragma mark-

void DevilDomScene::onPrestige(){
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    if(_iPrestigeSpecial==1)
    {
        if(ItemsMoneyManager::getInstance()->getGem() < C_PRESTIGE_GEM)
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
            return;
        }
    }
    else if(_iPrestigeSpecial==2)
    {
        if(ItemsMoneyManager::getInstance()->getGem() < C_PRESTIGE_GEM2)
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
            return;
        }
    }
    
    PopupLoading::show();
    
    _hero->stopAllActions();
    auto children = _hero->getChildren();
    for(auto child : children)
        child->setVisible(false);
    
    if(_enemy !=nullptr)
        _enemy->setVisible(false);
    
    for(auto quest: _vecCellQuest) //전생중 퀘스트로 인한 보상금지
        quest->_bar->stopAllActions();
    
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
    
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(DevilDomScene::reStart,this));
    
    auto spr = Sprite::create();
    spr->setAnchorPoint(Vec2(1,0));
    spr->setPosition(Vec2(0,0));
    _hero->addChild(spr);
    Sequence *seq = Sequence::create(animate,call,RemoveSelf::create(), nullptr);
    spr->runAction(seq);
    
//    CallFunc *call = CallFunc::create(CC_CALLBACK_0(DevilDomScene::reStart,this));
//    Sequence *seq = Sequence::create(DelayTime::create(0.5f),animate,Hide::create(),call, nullptr);
//    _hero->runAction(seq);
    
    SoundManager::SoundEffectStart(SOUND_REVIVAL);
}


void DevilDomScene::reStart()
{
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    auto addKey = DataManager::getInstance()->GetPrestigeKey(_ePlace, floor);
    auto per = UserInfoManager::getInstance()->getEquipTotalEffectPercentDevil(E_EQUIP_TYPE::E_D_BODY);
    if(per>0){
        per*=100;
        if(per>100){
            auto tt = MafUtils::bigMulNum(addKey, MafUtils::doubleToString(per));
            tt.pop_back();
            tt.pop_back();
            tt.pop_back();
            tt.pop_back();
            addKey = MafUtils::bigAddNum(addKey, tt);
        }
    }
    
    if (_bRevivalAds)
    { /*10%추가*/
        auto strTen = addKey;
        strTen.pop_back();
        addKey = MafUtils::bigAddNum(addKey, strTen);
    }
    
    if(_iPrestigeSpecial == 1 || _iPrestigeSpecial == 2)
    {
        if (_iPrestigeSpecial == 1)
        {
            addKey = MafUtils::bigMulNum(addKey, "5");
            ItemsMoneyManager::getInstance()->setGem(-C_PRESTIGE_GEM);
        }
        else
        {
            addKey = MafUtils::bigMulNum(addKey, "10");
            ItemsMoneyManager::getInstance()->setGem(-C_PRESTIGE_GEM2);
        }
    }
    
    //
    UserInfoManager::getInstance()->setFloor(_ePlace,1,true);
    UserInfoManager::getInstance()->setFloorCount(_ePlace,1);
    UserDefault::getInstance()->setStringForKey(KEY_D_MONSTER_HP,"0");
    
    //
    int revival = UserInfoManager::getInstance()->getRevival(_ePlace);
    revival++;
    UserInfoManager::getInstance()->setRevival(_ePlace,revival);
    
    //
    ItemsMoneyManager::getInstance()->setDevilKey(addKey);
    
    //
    SaveManager::saveAllData();
    
    
    PopupLoading::hide();
    UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
    
    bool bAutoSave = UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_REVIVE_NORMAL", false);
    
    if(bAutoSave == true)
    {
        showAutoSaveToast();
    }
}


void DevilDomScene::ResultRevivalAdsCallBack(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        onPrestige();
    }
    else
    {
        if(result == ADS_RESULT::LOAD)
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(DevilDomScene::ResultRevivalAdsCallBack, this));
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
 


#pragma mark-

void DevilDomScene::showReceiveAdsPopup(E_SHOP_ADS adsType)
{
    //
    cocos2d::Vector<InfoItems*> listReward;
    
    if ( adsType == E_SHOP_ADS::SHOP_D_GEM_ADS )
    {
        int earnCount = C_BONUS_ADS_GEM;
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_GEM_ADS, TIME_ADS_GEM_BONUS);
        
        //
        ItemsMoneyManager::getInstance()->setGem(earnCount);
        RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::GEM, MafUtils::toString(earnCount));
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
        obj->setCount(earnCount);
        listReward.pushBack(obj);
    }
    else if ( adsType == E_SHOP_ADS::SHOP_D_GOLD_ADS )
    {
        std::string earnCount = "0";
        std::string levelEncryptTemp = MafAes256::XorEncrypt(0);
        for(auto quest : DevilDomScene::getInstance()->_vecCellQuest)
        {
            if ( quest->_levelEncrypt.compare(levelEncryptTemp) != 0 )
            {
                earnCount = MafUtils::bigAddNum(earnCount, quest->_uQuestEarnMoney);
            }
        }
        earnCount = MafUtils::bigAddNum(earnCount, "3");
        earnCount.append("0");//퀘스트완료 x30회 보상을 준다.
        earnCount = MafUtils::bigMulNum(earnCount, MafUtils::toString(4)); // 광고시청 대기시간 증가로 보상 확장
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_GOLD_ADS, TIME_ADS_GOLD_BONUS);
        
        //
        ItemsMoneyManager::getInstance()->setDevilGold(earnCount);
        RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::D_GOLD, earnCount);
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::D_GOLD);
        obj->setCount(earnCount);
        listReward.pushBack(obj);
    }
    else if ( adsType == E_SHOP_ADS::SHOP_D_BUFF_ADS )
    {
        int buff = random(E_SHOP::SHOP_D_MOVE_SPD, E_SHOP::SHOP_D_QUEST_GOLD);
        int buffTime = 1800;
        E_ITEMS eReward = E_ITEMS::BUFF_DEVIL_ATK;
        
        //
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_D_BUFF_ADS, TIME_ADS_BUFF_BONUS);
     
        if ( buff == E_SHOP::SHOP_D_ATK_DMG )
        {
            eReward = E_ITEMS::BUFF_DEVIL_ATK;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_ATK_DMG,buffTime);
        }
        else if ( buff == E_SHOP::SHOP_D_QUEST_GOLD )
        {
            eReward = E_ITEMS::BUFF_DEVIL_COIN;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_QUEST_GOLD,buffTime);
            reDrawQuest();
        }
        else if ( buff == E_SHOP::SHOP_D_MOVE_SPD )
        {
            eReward = E_ITEMS::BUFF_DEVIL_SPD;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_MOVE_SPD,buffTime);
            drawEquip();
        }
        else if ( buff == E_SHOP::SHOP_D_ATK_SPD )
        {
            eReward = E_ITEMS::BUFF_DEVIL_ASPD;
            UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_D_ATK_SPD,buffTime);
            drawEquip();
        }
        
        //
        auto obj = ItemsManager::getInstance()->getInfoItems(eReward);
        obj->setCount(buffTime);
        listReward.pushBack(obj);
    }
    
    //
    PopupToastItems::show(listReward);
    
}


#pragma mark-
void DevilDomScene::showSaveWarning()
{
    if ( _layerSaveWarning != nullptr )
    {
        return;
    }
    
    std::string strMsg = "";
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

void DevilDomScene::hideSaveWarning()
{
    if ( _layerSaveWarning == nullptr )
        return;
    
    _layerSaveWarning->removeFromParent();
    _layerSaveWarning = nullptr;
}

void DevilDomScene::showAutoSaveToast()
{
    auto toast = PopupToastAutoSave::create();
    toast->setPosition(getContentSize().width/2, getContentSize().height*0.84);
    toast->show();
}

#pragma mark-
UnitBehaviorHero* DevilDomScene::getUnitHero()
{
    return _hero;
}

UnitBehaviorMonster* DevilDomScene::getUnitMonster()
{
    return _enemy;
}

cocos2d::Vector<Pet*> DevilDomScene::getUnitPets()
{
    return _vecPet;
}

#pragma mark-
void DevilDomScene::callbackQuestTab(int tab)
{
    int levelAdd = 1;
    switch (tab) {
        case 1: levelAdd = 1;               break;
        case 2: levelAdd = 10;              break;
        case 3: levelAdd = 100;             break;
        case 4: levelAdd = C_MAX_QUEST;     break;
            
    }
    
    for(auto cell : _vecCellQuest)
    {
        cell->setLevelAdd(levelAdd);
    }
}

void DevilDomScene::callbackEquipTab(int tab)
{
    int levelAdd = 1;
    switch (tab) {
        case 1: levelAdd = 1;               break;
        case 2: levelAdd = 10;              break;
        case 3: levelAdd = 100;             break;
        case 4: levelAdd = C_MAX_EQUIP_LEVEL_DEVIL;     break;
            
    }
    
    for(auto cell : _vecCellEquip)
    {
        cell->setLevelAdd(levelAdd);
    }
}

void DevilDomScene::callBackRevive(int nType, bool bAds, bool bFree, int nFloorSub)
{
    _iPrestigeSpecial = nType;
    _bRevivalAds = bAds;
    
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
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(DevilDomScene::ResultRevivalAdsCallBack, this));
        }
    }
}
