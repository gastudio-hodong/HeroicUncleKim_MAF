//
//  LayerPrisonBattle.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerPrisonBattle.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/BattlePrison/PopupPrison.h"
#include "GameUIPopup/Other/Ads/PopupAdsInterstitial.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

LayerPrisonBattle* LayerPrisonBattle::create(cocos2d::Size size)
{
    LayerPrisonBattle* pRet = new(std::nothrow) LayerPrisonBattle();
    if ( pRet && pRet->init(size) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

LayerPrisonBattle::LayerPrisonBattle(void) :
_ePlace(E_PLACE::EMPTY),

_bInitEnter(false),

_scaleBG(0),

_bLayoutMinimize(false),
_bLayoutMove(false),
_posLayoutMovePrev(Vec2::ZERO),

_battleTime(0),

_hero(nullptr),
_enemy(nullptr),

_uiContentsLobby(nullptr),
_uiContentsBattleMap(nullptr),
_uiContentsBattle(nullptr),
_uiContentsInfo(nullptr),
_uiContentsTimer(nullptr),

_spriteDark(nullptr),
_spriteWhite(nullptr),

_uiNpcBeelzebub(nullptr),
_uiNpcBaphomet(nullptr),
_uiNpcGummy(nullptr),
_uiNpcJelly(nullptr),

_uiTextTitle(nullptr),
_uiTextFloor(nullptr),

_uiImgAuto(nullptr),

_uiImgDungeonTimerBG(nullptr),
_uiImgDungeonRewardBG(nullptr)
{
    
}

LayerPrisonBattle::~LayerPrisonBattle(void)
{
    //
    PrisonManager::getInstance()->setEnterAutoCount(0);
    PrisonManager::getInstance()->setEnterAutoCountMax(0);
    PrisonManager::getInstance()->setDungeonEarnKey("0");
    PrisonManager::getInstance()->setDungeonKillAdd(0);
    PrisonManager::getInstance()->setDungeonHp("0");
    PrisonManager::getInstance()->setDungeonTimeNow(PrisonManager::getInstance()->getDungeonTimeDefault());
    PrisonManager::getInstance()->setDungeonPlay(false);
    
    UserInfoManager::getInstance()->setOneKill(_ePlace, false);
    UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, E_PRINCESS_BUFF::BUFF_NONE);
    UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER);
}

bool LayerPrisonBattle::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setBackGroundColor(Color3B::GRAY);
    setBackGroundColorType(BackGroundColorType::SOLID);
    setClippingEnabled(true);
    setTouchEnabled(true);
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME, this);
    
    return true;
}

void LayerPrisonBattle::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitEnter == false )
    {
        _bInitEnter = true;
    }
    else
    {
        //
        bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
        if ( bPlay == true )
        {
            auto scene = UtilsScene::getInstance()->getRunningScene();
            auto popupPrison =  scene->getChildByName<PopupPrison*>("PopupPrison");
            if ( popupPrison != nullptr )
            {
                if( MafRemoteConfig::GetRemoteValueBool("enable_banner") )
                {
                    if(!CashManager::getInstance()->isAdsfreeTime())
                        MafGoogleRewardAds::ShowGoogleBanner();
                }
            }
        }
    }
}

void LayerPrisonBattle::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::COSTUME )
    {
        heroRefresh();
    }
}

#pragma mark - init
void LayerPrisonBattle::initVar()
{
    _ePlace = E_PLACE::DG_PRISON;
    
    _scaleBG = MAX(getContentSize().width / 200, getContentSize().height / 150);
    
    PrisonManager::getInstance()->setEnterAutoCount(0);
    PrisonManager::getInstance()->setEnterAutoCountMax(0);
    PrisonManager::getInstance()->setDungeonEarnKey("0");
    PrisonManager::getInstance()->setDungeonKillAdd(0);
    PrisonManager::getInstance()->setDungeonHp("0");
    PrisonManager::getInstance()->setDungeonTimeNow(PrisonManager::getInstance()->getDungeonTimeDefault());
    PrisonManager::getInstance()->setDungeonPlay(false);
    
    UserInfoManager::getInstance()->setOneKill(_ePlace, false);
    UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, E_PRINCESS_BUFF::BUFF_NONE);
    UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER);
}

void LayerPrisonBattle::initUi()
{
    //
    _uiContentsLobby = ui::Layout::create();
    _uiContentsLobby->setContentSize( getContentSize() );
    _uiContentsLobby->setIgnoreAnchorPointForPosition(false);
    _uiContentsLobby->setAnchorPoint(Vec2::ZERO);
    _uiContentsLobby->setPosition(Vec2::ZERO);
    addChild(_uiContentsLobby);
    
    _uiContentsBattleMap = ui::Layout::create();
    _uiContentsBattleMap->setContentSize( getContentSize() );
    _uiContentsBattleMap->setIgnoreAnchorPointForPosition(false);
    _uiContentsBattleMap->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattleMap->setPosition(Vec2::ZERO);
    addChild(_uiContentsBattleMap);
    
    _uiContentsBattle = ui::Layout::create();
    _uiContentsBattle->setContentSize( getContentSize() );
    _uiContentsBattle->setIgnoreAnchorPointForPosition(false);
    _uiContentsBattle->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattle->setPosition(Vec2::ZERO);
    addChild(_uiContentsBattle);
    
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize( getContentSize() );
    _uiContentsInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsInfo->setAnchorPoint(Vec2::ZERO);
    _uiContentsInfo->setPosition(Vec2::ZERO);
    addChild(_uiContentsInfo);
    
    _uiContentsTimer = ui::Layout::create();
    _uiContentsTimer->setContentSize(Size(getContentSize().width, 120));
    _uiContentsTimer->setIgnoreAnchorPointForPosition(false);
    _uiContentsTimer->setAnchorPoint(Vec2::ZERO);
    _uiContentsTimer->setPosition(Vec2::ZERO);
    _uiContentsTimer->setVisible(false);
    addChild(_uiContentsTimer);
    
    
    _spriteDark = ui::Scale9Sprite::create(PIXEL,Rect(0,0, getContentSize().width, getContentSize().height));
    _spriteDark->setColor(Color3B::BLACK);
    _spriteDark->setOpacity(0);
    _spriteDark->setAnchorPoint(Vec2::ZERO);
    _spriteDark->setPosition(Vec2::ZERO);
    addChild(_spriteDark, 100);
    
    _spriteWhite = ui::Scale9Sprite::create(PIXEL,Rect(0,0, getContentSize().width, getContentSize().height));
    _spriteWhite->setColor(Color3B::WHITE);
    _spriteWhite->setOpacity(0);
    _spriteWhite->setAnchorPoint(Vec2::ZERO);
    _spriteWhite->setPosition(Vec2::ZERO);
    addChild(_spriteWhite, 100);
    
    //
    uiLobby();
    uiLobbyNPC();
    uiInfo();
    uiTimer();
    
    // hero
    heroCreate();
    
    // pet
    E_PET listPet[] = {E_PET::CHICK_1, E_PET::DRAGON_3, E_PET::PRINCESS_4, E_PET::HONG_6, E_PET::HANDSOME_8};
    for ( int i = 0; i < sizeof(listPet)/sizeof(E_PET); i++ )
    {
        if ( PetNewManager::getInstance()->getPet(listPet[i]) )
        {
            petCreate(listPet[i]);
        }
    }
}

#pragma mark - ui : defalut
void LayerPrisonBattle::uiLobby()
{
    auto layout = _uiContentsLobby;
    
    //
    std::string pathGateBG = "Assets/battle_bg/prison/bg_prison_gate1.png";
    std::string pathGate = "Assets/battle_bg/prison/bg_prison_gate2.png";
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
    {
       
    }
    else
    {
        pathGateBG = "Assets/battle_bg/prison/bg_prison_2_gate1.png";
        pathGate = "Assets/battle_bg/prison/bg_prison_2_gate2.png";
    }
    
    auto uiImgLobbyBG = ui::ImageView::create(pathGateBG);
    uiImgLobbyBG->setAnchorPoint(Vec2::ZERO);
    uiImgLobbyBG->setScale(_scaleBG);
    ((ui::Scale9Sprite*)uiImgLobbyBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiImgLobbyBG);
    
    auto uiImgLobby = ui::ImageView::create(pathGate);
    uiImgLobby->setAnchorPoint(Vec2::ZERO);
    uiImgLobby->setScale(_scaleBG);
    ((ui::Scale9Sprite*)uiImgLobby->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiImgLobby);
    
    
    // effect
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
    {
        
    }
}

void LayerPrisonBattle::uiLobbyNPC()
{
    auto layout = _uiContentsLobby;
    
    // 바알세불
    {
        auto uiImgNPC = ui::ImageView::create("Assets/ui/prison/beelzebub/pet_beelzebub_idle_1.png");
        uiImgNPC->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgNPC->setPosition(Vec2(350, 290));
        uiImgNPC->setScale(_scaleBG);
        uiImgNPC->setVisible(true);
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        layout->addChild(uiImgNPC);
        {
            auto spriteShadow = ui::ImageView::create("Assets/ui/prison/beelzebub/pet_beelzebub_idle_0.png");
            spriteShadow->setPosition( Vec2(77, - 10) );
            uiImgNPC->addChild(spriteShadow, -2);
        }
        
        //
        _uiNpcBeelzebub = uiImgNPC;
    }
    
    // 바포메트
    {
        auto uiImgNPC = ui::ImageView::create("Assets/ui/prison/bafometz/pet_baphomet_idle_1.png");
        uiImgNPC->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgNPC->setPosition(Vec2(450, 190));
        uiImgNPC->setScale(_scaleBG);
        uiImgNPC->setVisible(false);
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        layout->addChild(uiImgNPC);
        {
            auto spriteShadow = ui::ImageView::create("Assets/ui/prison/bafometz/pet_baphomet_shadow.png");
            spriteShadow->setPosition( Vec2(uiImgNPC->getContentSize().width / 2, uiImgNPC->getContentSize().height / 2) );
            uiImgNPC->addChild(spriteShadow, -2);
        }
        
        //
        _uiNpcBaphomet = uiImgNPC;
    }
    
    // 구미
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/prison/jelly/pet_gummy_%d.png", i));
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiImgNPC = ui::ImageView::create("Assets/ui/prison/jelly/pet_gummy_1.png");
        uiImgNPC->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgNPC->setPosition(Vec2(480, 180));
        uiImgNPC->setScale(_scaleBG);
        uiImgNPC->setVisible(false);
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
        layout->addChild(uiImgNPC);
        
        //
        _uiNpcGummy = uiImgNPC;
    }
    
    // 젤리
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/prison/jelly/pet_jelly_%d.png", i));
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiImgNPC = ui::ImageView::create("Assets/ui/prison/jelly/pet_jelly_1.png");
        uiImgNPC->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgNPC->setPosition(Vec2(530, 180));
        uiImgNPC->setScale(_scaleBG);
        uiImgNPC->setVisible(false);
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
        layout->addChild(uiImgNPC);
        
        //
        _uiNpcJelly = uiImgNPC;
    }
    
    //
    drawLobbyNPC(true);
    animNPCBeelzebub(true);
    animNPCBaphomet(true);
}

void LayerPrisonBattle::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = _uiContentsInfo->getContentSize();
    
    _uiTextTitle = ui::CText::create(" ", GAME_FONT, 24);
    _uiTextTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiTextTitle->setPosition(Vec2(size.width / 2, size.height - 40));
    _uiTextTitle->enableOutline(Color4B::BLACK, 1);
    layout->addChild(_uiTextTitle);
    
    _uiTextFloor = ui::CText::create(" ", GAME_FONT, 24);
    _uiTextFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiTextFloor->setPosition(Vec2(size.width / 2, size.height - 40));
    _uiTextFloor->enableOutline(Color4B::BLACK, 1);
    layout->addChild(_uiTextFloor);
    
    _uiImgAuto = ui::ImageView::create("Assets/ui/prison/icon_auto1_2.png");
    _uiImgAuto->setPosition(Vec2(size.width - 50, size.height - 50));
    _uiImgAuto->setVisible(false);
    layout->addChild(_uiImgAuto);
    {
        auto uiBtnAuto = ui::Button::create("Assets/ui/prison/icon_auto1_1.png");
        uiBtnAuto->setPosition(Vec2(_uiImgAuto->getContentSize().width / 2, _uiImgAuto->getContentSize().height / 2));
        uiBtnAuto->addClickEventListener(CC_CALLBACK_1(LayerPrisonBattle::onClickAuto, this));
        uiBtnAuto->runAction(RepeatForever::create(RotateBy::create(1.0, -180)));
        uiBtnAuto->setName("UI_BTN_AUTO");
        _uiImgAuto->addChild(uiBtnAuto, -1);
        
        auto uiTextCount = ui::CText::create("0/0", GAME_FONT, 20);
        uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiTextCount->setPosition(Vec2(_uiImgAuto->getContentSize().width / 2, -10));
        uiTextCount->enableOutline(Color4B::BLACK, 2);
        uiTextCount->setName("UI_TEXT_COUNT");
        _uiImgAuto->addChild(uiTextCount);
    }
    
    //
    drawInfo();
}

void LayerPrisonBattle::uiTimer()
{
    auto layout = _uiContentsTimer;
    auto size = _uiContentsTimer->getContentSize();
    
    
    auto uiImgBG = ui::ImageView::create("Assets/ui_common/popup_mini_scale2_1.png");
    uiImgBG->setContentSize(size);
    uiImgBG->setScale9Enabled(true);
    uiImgBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiImgBG);
    
    _uiImgDungeonTimerBG = ui::ImageView::create("Assets/ui/prison/gauge_bg.png");
    _uiImgDungeonTimerBG->setPosition(Vec2(size.width / 2, size.height * 0.7));
    layout->addChild(_uiImgDungeonTimerBG);
    {
        auto uiImgIcon = ui::ImageView::create("Assets/ui/prison/clock.png");
        uiImgIcon->setPosition(Vec2(0, _uiImgDungeonTimerBG->getContentSize().height / 2));
        uiImgIcon->setScale(0.9);
        _uiImgDungeonTimerBG->addChild(uiImgIcon, 1);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/prison/gauge_yellow.png");
        uiBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBar->setPosition(Vec2(0, 0));
        uiBar->setPercent(90);
        uiBar->setName("UI_BAR_TIME");
        _uiImgDungeonTimerBG->addChild(uiBar);
    }
    
    _uiImgDungeonRewardBG = ui::ImageView::create("Assets/ui/prison/gauge_red.png");
    _uiImgDungeonRewardBG->setPosition(Vec2(size.width / 2, size.height * 0.3));
    layout->addChild(_uiImgDungeonRewardBG);
    {
        auto uiImgIcon = ui::ImageView::create("Assets/ui/prison/gem.png");
        uiImgIcon->setPosition(Vec2(0, _uiImgDungeonRewardBG->getContentSize().height / 2));
        uiImgIcon->setScale(0.9);
        _uiImgDungeonRewardBG->addChild(uiImgIcon, 1);
        
        auto uiImgIconReward = ui::ImageView::create("Assets/ui/prison/icon_chest.png");
        uiImgIconReward->setPosition(Vec2(_uiImgDungeonRewardBG->getContentSize().width, _uiImgDungeonRewardBG->getContentSize().height / 2));
        _uiImgDungeonRewardBG->addChild(uiImgIconReward, 1);
        
        auto uiImgIconBongsik = ui::ImageView::create("Assets/ui/prison/head.png");
        uiImgIconBongsik->setPosition(Vec2(0, _uiImgDungeonRewardBG->getContentSize().height / 2));
        uiImgIconBongsik->setName("UI_IMG_BONGSIK");
        _uiImgDungeonRewardBG->addChild(uiImgIconBongsik, 2);
        
        auto uiImgKey = ui::ImageView::create("Assets/icon/menu_key.png");
        uiImgKey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiImgKey->setPosition(Vec2(_uiImgDungeonRewardBG->getContentSize().width - 100, - 10));
        uiImgKey->setScale(0.6);
        _uiImgDungeonRewardBG->addChild(uiImgKey);
        
        auto uiTextKey = ui::CText::create("x 100", GAME_FONT, 18);
        uiTextKey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextKey->setPosition(Vec2(_uiImgDungeonRewardBG->getContentSize().width - 100, - 10));
        uiTextKey->setColor(COLOR_KEY);
        uiTextKey->setName("UI_TEXT_KEY");
        _uiImgDungeonRewardBG->addChild(uiTextKey);
    }
    
}

#pragma mark - ui : draw
void LayerPrisonBattle::drawLobbyNPC(bool bShow)
{
    if ( bShow == false )
    {
        _uiNpcGummy->setVisible(false);
        _uiNpcJelly->setVisible(false);
        _uiNpcBaphomet->setVisible(false);
        _uiNpcBeelzebub->setVisible(false);
    }
    else
    {
        _uiNpcGummy->setVisible(true);
        _uiNpcJelly->setVisible(true);
        if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
        {
            _uiNpcBeelzebub->setVisible(false);
            _uiNpcBaphomet->setVisible(true);
        }
        else
        {
            _uiNpcBeelzebub->setVisible(true);
            _uiNpcBaphomet->setVisible(false);
        }
    }
}

void LayerPrisonBattle::drawInfo()
{
    std::string titleText = "";
    std::string floorText = "";
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
    {
        int count = PrisonManager::getInstance()->getDungeonCount() + 1;
        
        if     (count==1) titleText=GAME_TEXT("t_ui_prison_msg_2");
        else if(count==2) titleText=GAME_TEXT("t_ui_prison_msg_3");
        else if(count==3) titleText=GAME_TEXT("t_ui_prison_msg_4");
        else
        {
            titleText = GAME_TEXTFORMAT("t_ui_prison_msg_5", count);
        }
        
        floorText = MafUtils::toString(PrisonManager::getInstance()->getDungeonKill());
        MafUtils::addCommaToString(floorText, 3, ',');
        floorText.append("M");
    }
    else
    {
        int count = PrisonManager::getInstance()->getDungeonCount() + 1;
        
        if     (count==1) titleText=GAME_TEXT("t_ui_prison_msg_6");
        else if(count==2) titleText=GAME_TEXT("t_ui_prison_msg_7");
        else if(count==3) titleText=GAME_TEXT("t_ui_prison_msg_8");
        else
        {
            titleText = GAME_TEXTFORMAT("t_ui_prison_msg_9", count);
        }
        
        floorText = MafUtils::toString(PrisonManager::getInstance()->getDungeonKill());
        MafUtils::addCommaToString(floorText, 3, ',');
        floorText.append("M");
    }
    
    _uiTextTitle->setString(titleText);
    _uiTextFloor->setString(floorText);
    
    
    //
    bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
    bool bEnterAuto = PrisonManager::getInstance()->isEnterAuto();
    int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
    int nEnterAutoCountMax = PrisonManager::getInstance()->getEnterAutoCountMax();
    if ( bPlay == true && nEnterAutoCountMax > 1 )
    {
        auto uiBtnAuto = _uiImgAuto->getChildByName<ui::Button*>("UI_BTN_AUTO");
        auto uiTextKey = _uiImgAuto->getChildByName<ui::CText*>("UI_TEXT_COUNT");
        uiTextKey->setString(MafUtils::format("%d/%d", nEnterAutoCount, nEnterAutoCountMax));
        
        _uiImgAuto->setVisible(true);
        if ( bEnterAuto == true )
        {
            _uiImgAuto->loadTexture("Assets/ui/prison/icon_auto1_2.png");
            uiBtnAuto->loadTextureNormal("Assets/ui/prison/icon_auto1_1.png");
            uiBtnAuto->runAction(RepeatForever::create(RotateBy::create(1.0, -180)));
        }
        else
        {
            _uiImgAuto->loadTexture("Assets/ui/prison/icon_auto2_2.png");
            uiBtnAuto->loadTextureNormal("Assets/ui/prison/icon_auto2_1.png");
            uiBtnAuto->stopAllActions();
        }
    }
    else
    {
        _uiImgAuto->setVisible(false);
    }
}

void LayerPrisonBattle::drawTimer()
{
    // time
    {
        double timeMax = PrisonManager::getInstance()->getDungeonTimeDefault();
        double time = PrisonManager::getInstance()->getDungeonTimeNow();
        double nPercent = (double)time / timeMax * 100.0;
        if ( nPercent > 100 )
            nPercent = 100;
        
        auto uiBar = _uiImgDungeonTimerBG->getChildByName<ui::LoadingBar*>("UI_BAR_TIME");
        uiBar->setPercent(nPercent);
    }
    
    // 봉식 위치
    {
        int nKill = PrisonManager::getInstance()->getDungeonKill();
        double nPos = 0;
        if ( nKill % 20 == 0 )
        {
            if ( nKill / 20 > 0)    nPos = 1;
            else                    nPos = 0;
        }
        else
        {
            nPos = (double)(nKill % 20) / 20;
        }
        
        auto uiImgIconBongsik = _uiImgDungeonRewardBG->getChildByName<ui::ImageView*>("UI_IMG_BONGSIK");
        uiImgIconBongsik->setPositionX(_uiImgDungeonRewardBG->getContentSize().width * nPos);
    }
    
    // 획득 열쇠
    {
        int nKill = PrisonManager::getInstance()->getDungeonKill();
        if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::FIRE )
        {
            nKill += 15000;
        }
        
        std::string strEarnKey = DataManager::GetDungeonEarnKey(nKill);
        MafUtils::convertNumberToShort(strEarnKey);
        
        auto uiTextKey = _uiImgDungeonRewardBG->getChildByName<ui::CText*>("UI_TEXT_KEY");
        uiTextKey->setString(MafUtils::format("x %s", strEarnKey.c_str()));
    }
}

void LayerPrisonBattle::animNPCBeelzebub(bool bIdle)
{
    auto uiImgNPC = _uiNpcBeelzebub;
    
    // stop action
    ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->stopAllActions();
    
    //
    if ( bIdle == true )
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.15f);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/prison/beelzebub/pet_beelzebub_idle_%d.png", i));
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->stopAllActions();
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
        
        // effect
        uiImgNPC->removeChildByName("UI_IMG_EFFECT");
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18) == true )
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.15f);
            for ( int i = 1; i <= 4; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/prison/beelzebub/pet_beelzebub_ready_%d.png", i));
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }

            auto uiImgEffect = ui::ImageView::create("Assets/ui/prison/beelzebub/pet_beelzebub_ready_1.png");
            uiImgEffect->setPosition( Vec2(60, 50));
            uiImgEffect->setName("UI_IMG_EFFECT");
            ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
            uiImgNPC->addChild(uiImgEffect);
        }
    }
    else
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.15f);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/prison/beelzebub/pet_beelzebub_attack_%d.png", i));
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->stopAllActions();
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->runAction(Animate::create(animation));
        
        // effect
        uiImgNPC->removeChildByName("UI_IMG_EFFECT");
    }
    
}

void LayerPrisonBattle::animNPCBaphomet(bool bIdle)
{
    auto uiImgNPC = _uiNpcBaphomet;
    
    // stop action
    ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->stopAllActions();
    
    //
    if ( bIdle == true )
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 4; i++ )
        {
            std::string strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_idle_%d.png",i);
            if ( DataManager::isEventChristmas() == true )
            {
                strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_idle_%d_event.png",i);
            }
            else if(DataManager::isNewYear() == true)
            {
                strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_event_02_idle_%d.png",i);
            }
            
            auto spriteAni = Sprite::create(strPath);
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->stopAllActions();
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
        
        // effect
        uiImgNPC->removeChildByName("UI_IMG_EFFECT");
        if ( UserInfoManager::getInstance()->getPetBafometzHave() == true )
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.25f);
            for ( int i = 1; i <= 4; i++ )
            {
                std::string strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_idle_outline_%d.png",i);
                if ( DataManager::isEventChristmas() == true )
                {
                    strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_idle_outline_%d_event.png",i);
                }
                else if(DataManager::isNewYear() == true)
                {
                    strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_event_02_idle_outline_%d.png",i);
                }
                auto spriteAni = Sprite::create(strPath);
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }

            auto uiImgEffect = ui::ImageView::create("Assets/ui/prison/bafometz/pet_baphomet_idle_outline_1.png");
            uiImgEffect->setPosition( Vec2(uiImgNPC->getContentSize().width / 2, uiImgNPC->getContentSize().height / 2) );
            uiImgEffect->setName("UI_IMG_EFFECT");
            ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
            uiImgNPC->addChild(uiImgEffect, -1);
        }
    }
    else
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.25f);
        for ( int i = 1; i <= 4; i++ )
        {
            std::string strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_attack_%d.png",i);
            if ( DataManager::isEventChristmas() == true )
            {
                strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_attack_%d_event.png",i);
            }
            else if(DataManager::isNewYear() == true)
            {
                strPath = MafUtils::format("Assets/ui/prison/bafometz/pet_baphomet_event_02_attack_%d.png",i);
            }
            
            auto spriteAni = Sprite::create(strPath);
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->stopAllActions();
        ((ui::Scale9Sprite*)uiImgNPC->getVirtualRenderer())->runAction(Animate::create(animation));
        
        // effect
        uiImgNPC->removeChildByName("UI_IMG_EFFECT");
    }
}

#pragma mark - ui : set, get
UnitBehaviorHero* LayerPrisonBattle::getUnitHero()
{
    return _hero;
}

UnitBehaviorMonster* LayerPrisonBattle::getUnitMonster()
{
    return _enemy;
}

cocos2d::Vector<Pet*> LayerPrisonBattle::getUnitPets()
{
    return _listPet;
}

#pragma mark - ui : click
void LayerPrisonBattle::onTouchBattleLayout(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if ( _bLayoutMinimize == false )
    {
        return;
    }
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    auto uiLayoutParent = dynamic_cast<ui::Layout*>(this);
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
            _bLayoutMove = false;
            _posLayoutMovePrev = uiLayout->getTouchBeganPosition();
            break;

        case ui::Widget::TouchEventType::MOVED:
        {
            if ( uiLayout->getTouchBeganPosition().distance(uiLayout->getTouchMovePosition()) > 10 )
            {
                _bLayoutMove = true;
            }

            if ( _bLayoutMove == true )
            {
                double width = uiLayout->getContentSize().width * getScaleX();
                double height = uiLayout->getContentSize().height * getScaleY();
                double posMinX = width * uiLayout->getAnchorPoint().x;
                double posMaxX = _parent->getContentSize().width - (width * (1.0f - uiLayout->getAnchorPoint().x));
                double posMinY = height * uiLayout->getAnchorPoint().y;
                double posMaxY = _parent->getContentSize().height - (height * (1.0f - uiLayout->getAnchorPoint().y));
                
                auto posDiff = _posLayoutMovePrev - uiLayout->getTouchMovePosition();
                posDiff = uiLayoutParent->getPosition() - posDiff;
                posDiff.x = MIN(MAX(posMinX, posDiff.x), posMaxX);
                posDiff.y = MIN(MAX(posMinY, posDiff.y), posMaxY);

                uiLayoutParent->setPosition(posDiff);
            }
            
            _posLayoutMovePrev = uiLayout->getTouchMovePosition();
            break;
        }
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED:
        {

            break;
        }
        default:
            break;
    }
}

void LayerPrisonBattle::onClickBattleLayout(cocos2d::Ref* sender)
{
    if ( _bLayoutMinimize == false || _bLayoutMove == true )
    {
        return;
    }
    
    //
    auto popup = PopupPrison::create();
    popup->show();
}

void LayerPrisonBattle::onClickAuto(cocos2d::Ref* sender)
{
    bool bEnterAuto = PrisonManager::getInstance()->isEnterAuto();
    PrisonManager::getInstance()->setEnterAuto(!bEnterAuto);
    
    // draw
    drawInfo();
}

#pragma mark - hero
void LayerPrisonBattle::heroCreate()
{
    _hero = UnitBehaviorHero::create(_ePlace);
    _hero->setAttackCallBackListener(CallFunc::create(CC_CALLBACK_0(LayerPrisonBattle::heroAttack,this)));
    _hero->setMoveCallBackListener(CallFunc::create(CC_CALLBACK_0(LayerPrisonBattle::heroMoveDone,this)));
    _hero->setUpstairsCallBackListener(CallFunc::create(CC_CALLBACK_0(LayerPrisonBattle::heroUpstairs,this)));

    _hero->setMotion();
    _hero->actionRun();
    _hero->setTalkShow(false);
    _hero->setPositionHero(_uiContentsBattle, _uiContentsBattle->getContentSize().height);
    _uiContentsBattle->addChild(_hero, 1);
}

void LayerPrisonBattle::heroRefresh()
{
    _hero->setChangeParts();
    _hero->setMotion();
}

void LayerPrisonBattle::heroAttack()
{
    if ( _enemy->isDie() == true )
    {
        PrisonManager::getInstance()->setDungeonHp("0");
        PrisonManager::getInstance()->setDungeonKillAdd(PrisonManager::getInstance()->getDungeonKillAdd() + 1);

        //보상지급
        UnitBehaviorMonster::E_BOSS type = _enemy->getMonsterType();
        if(type == UnitBehaviorMonster::E_BOSS::PRISON_TREASURE || type == UnitBehaviorMonster::E_BOSS::PRISON_TREASURE_FIRE)
        {
            int nKill = PrisonManager::getInstance()->getDungeonKill() - 1;
            if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::FIRE )
            {
                nKill += 15000;
            }

            std::string dungeonEarnKey = PrisonManager::getInstance()->getDungeonEarnKey();
            dungeonEarnKey = MafUtils::bigAddNum(dungeonEarnKey, DataManager::GetDungeonEarnKey(nKill));
            PrisonManager::getInstance()->setDungeonEarnKey(dungeonEarnKey);
        }

        if ( PrisonManager::getInstance()->getDungeonKill()  == 2000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_2000_DUNGEON, 1, 1);
        }
        else if ( PrisonManager::getInstance()->getDungeonKill() == 5000 )
        {
            MafGooglePlay::UnlockAchievement(ACHIEVE_5000_DUNGEON, 1, 1);
        }

        // move
        dungeonMove();
        
        // draw
        drawInfo();
        drawTimer();
    }
    else
    {
        //
        PrisonManager::getInstance()->setDungeonHp(_enemy->getMonsterHp());

        // attack pet
        for ( auto unitPet : _listPet )
        {
            unitPet->attack(*_enemy);
        }

        // attack
        _hero->actionAttack(*_enemy);

        // move stop
        this->unschedule(schedule_selector(LayerPrisonBattle::scheduleMap));
    }
}

void LayerPrisonBattle::heroMoveDone()
{
    _hero->heroStopAllAction();
    heroAttack();
}

void LayerPrisonBattle::heroUpstairs()
{
    _hero->setPosition(Vec2(-_hero->getContentSize().width, _hero->getPosition().y));
    
    //
    dungeonMove();
}

#pragma mark - pet
void LayerPrisonBattle::petCreate(E_PET ePet)
{
    Pet *unitPet = nullptr;
    if ( ePet == E_PET::CHICK_1 )
    {
        double nTimeCool = PetNewManager::getInstance()->getPetCoolTime(ePet);
        
        unitPet = Chick::create();
        unitPet->setAnchorPoint(Vec2(0,0));
        unitPet->setPosition(Vec2(-65,-10));
        unitPet->setCoolTime(nTimeCool, true);
        _hero->getLayerCharacter()->addChild(unitPet, 10 + 1);
    }
    else if ( ePet == E_PET::DRAGON_3 )
    {
        unitPet = Dragon::create();
        unitPet->setAnchorPoint(Vec2(0,0));
        unitPet->setPosition(Vec2(-120,0));
        _hero->getLayerCharacter()->addChild(unitPet, 10 + 0);
    }
    else if ( ePet == E_PET::PRINCESS_4 )
    {
        double nTimeCool = PetNewManager::getInstance()->getPetCoolTime(ePet);
        
        unitPet = Princess::create();
        ((Princess*)unitPet)->_onBuffEffect = CC_CALLBACK_2(UnitBehaviorHero::onPrincessBuff, _hero);
        unitPet->setAnchorPoint(Vec2(0,0));
        unitPet->setPosition(Vec2(-245,0));
        unitPet->setCoolTime(nTimeCool, true);
        _hero->getLayerCharacter()->addChild(unitPet, 10 + 2);

        ((Princess*)unitPet)->_onBuffEffect = CC_CALLBACK_2(UnitBehaviorHero::onPrincessBuff, _hero);
    }
    else if ( ePet == E_PET::HONG_6 )
    {
        double nTimeCool = PetNewManager::getInstance()->getPetCoolTime(ePet);
        
        unitPet = Hong::create();
        unitPet->setAnchorPoint(Vec2(0,0));
        unitPet->setPosition(Vec2(-65,10));
        unitPet->setCoolTime(nTimeCool, true);
        unitPet->setHero(_hero);
        _hero->getLayerCharacter()->addChild(unitPet, 10 + 0);
    }
    else if ( ePet == E_PET::HANDSOME_8 )
    {
        unitPet = Handsome::create();
        unitPet->setAnchorPoint(Vec2(0.5,0));
        unitPet->setPosition(Vec2(-110,0));
        _hero->getLayerCharacter()->addChild(unitPet, 10 + 0);
    }

    if ( unitPet == nullptr )
    {
        return;
    }

    //
    unitPet->setPlace(_ePlace);
    unitPet->setEvolution();
 
    //
    _listPet.pushBack(unitPet);
}

#pragma mark - enemy
void LayerPrisonBattle::enemyCreate()
{
    if ( _enemy == nullptr )
    {
        _enemy = UnitBehaviorMonster::create(_ePlace);
        _enemy->setMotion();
        _enemy->setPositionMonster(_uiContentsBattle, _uiContentsBattle->getContentSize().height);
        _uiContentsBattle->addChild(_enemy);
    }
    
    int nCount = PrisonManager::getInstance()->getDungeonKill();
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::FIRE )
    {
        nCount += 15000;
    }
    
    std::string hpMax = DataManager::GetMonsterHpPrison(nCount);
    if ( PrisonManager::getInstance()->getDungeonHp().compare("0") == 0 )
    {
        PrisonManager::getInstance()->setDungeonHp(hpMax);
    }
    
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
    {
        if ( nCount != 0 && nCount % 20 == 0 )
            _enemy->setMotion(UnitBehaviorMonster::E_BOSS::PRISON_TREASURE);
        else
            _enemy->setMotion(UnitBehaviorMonster::E_BOSS::PRISON_NORMAL);
    }
    else
    {
        if ( nCount != 0 && nCount % 20 == 0 )
            _enemy->setMotion(UnitBehaviorMonster::E_BOSS::PRISON_TREASURE_FIRE);
        else
            _enemy->setMotion(UnitBehaviorMonster::E_BOSS::PRISON_NORMAL_FIRE);
    }
    
    // hp
    _enemy->setPositionMonster(_uiContentsBattle, _uiContentsBattle->getContentSize().height);
    _enemy->setMonsterHp(PrisonManager::getInstance()->getDungeonHp(), hpMax);
}

#pragma mark - dungeon
void LayerPrisonBattle::dungeonStart()
{
    PrisonManager::getInstance()->setDungeonPlay(true);
    
    //
    _spriteDark->stopAllActions();
    _spriteDark->setOpacity(0);
    _spriteDark->runAction(FadeIn::create(0.5));
    
    //
    _uiContentsTimer->setVisible(true);
    
    // hero
    _hero->stopAllActions();
    _hero->actionMoveUpstairs();
    
    // map create
    dungeonMapCreate();
    
    // draw
    drawLobbyNPC(false);
    drawInfo();
    drawTimer();
    
    //
    this->schedule(schedule_selector(LayerPrisonBattle::scheduleBattle));
    
    // mission
    SpecialMissionManager::getInstance()->clearSpecialMission(38);
    
    //
    auto scene = UtilsScene::getInstance()->getRunningScene();
    auto popupPrison =  scene->getChildByName<PopupPrison*>("PopupPrison");
    if ( popupPrison != nullptr )
    {
        popupPrison->onDraw();
        
        //
        if( MafRemoteConfig::GetRemoteValueBool("enable_banner") )
        {
            if(!CashManager::getInstance()->isAdsfreeTime())
                MafGoogleRewardAds::ShowGoogleBanner();
        }
    }
}

void LayerPrisonBattle::dungeonMove()
{
    // hero move
    if ( _hero->getPositionX() <= 0 )
    {
        //
        _spriteDark->stopAllActions();
        _spriteDark->setOpacity(255);
        _spriteDark->runAction(FadeOut::create(0.5));
        
        //
        _hero->actionMoveDungeon(true);
    }
    else
    {
        _hero->actionMoveDungeon(false);
    }
    
    // enemy create & move
    enemyCreate();
    _enemy->actionMove();
    
    // move pet
    for ( auto pet : _listPet )
    {
        pet->move();
    }
    
    // move map
    scheduleMap(0);
    this->schedule(schedule_selector(LayerPrisonBattle::scheduleMap));
}

void LayerPrisonBattle::dungeonEnd()
{
    _battleTime = PrisonManager::getInstance()->getDungeonTimeDefault();
    PrisonManager::getInstance()->setDungeonTimeNow(_battleTime);
    PrisonManager::getInstance()->setDungeonPlay(false);
    
    // stop batttle
    this->unschedule(schedule_selector(LayerPrisonBattle::scheduleMap));
    this->unschedule(schedule_selector(LayerPrisonBattle::scheduleBattle));
    
    // stop unit
    _hero->stopAllActions();
    _enemy->stopAllActions();
    for ( auto pet : _listPet )
    {
        pet->stopAllActions();
        pet->unscheduleUpdate();
    }
    
    // mission
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::PRISON_TICKET_USE, 1);
    
    //
    MafGoogleRewardAds::HideGoogleBanner();
    
    //
    auto action1 = FadeIn::create(0.5);
    auto action2 = CallFuncN::create([=](Node* sender){
        
        // 마지막 몬스터 체력 설정 
        PrisonManager::getInstance()->setDungeonHp(_enemy->getMonsterHp());
        
        //
        _uiContentsTimer->setVisible(false);
        
        // draw
        drawLobbyNPC(true);
        drawInfo();
        drawTimer();
        
        // remove unit
        _uiContentsBattleMap->removeAllChildren();
        _uiContentsBattle->removeAllChildren();
        _listPet.clear();
        _hero = nullptr;
        _enemy = nullptr;
        
        // create hero
        heroCreate();
        
        // create pet
        E_PET listPet[] = {E_PET::CHICK_1, E_PET::DRAGON_3, E_PET::PRINCESS_4, E_PET::HONG_6, E_PET::HANDSOME_8};
        for ( int i = 0; i < sizeof(listPet)/sizeof(E_PET); i++ )
        {
            if ( PetNewManager::getInstance()->getPet(listPet[i]) )
            {
                petCreate(listPet[i]);
            }
        }
        
        auto spriteDark = (Sprite*)sender;
        spriteDark->setOpacity(0);
        
        //
        bool bPopupLoading = false;
        auto scene = UtilsScene::getInstance()->getRunningScene();
        auto popupPrison =  scene->getChildByName<PopupPrison*>("PopupPrison");
        if ( popupPrison != nullptr )
        {
            bPopupLoading = true;
        }
        
        if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
        {
            PrisonManager::getInstance()->requestDefaultResult(bPopupLoading, CC_CALLBACK_1(LayerPrisonBattle::callbackDungeonResult, this));
        }
        else
        {
            PrisonManager::getInstance()->requestFireResult(bPopupLoading, CC_CALLBACK_1(LayerPrisonBattle::callbackDungeonResult, this));
        }
        
        //
        if ( popupPrison != nullptr )
        {
            popupPrison->onDraw();
        }
    });
    auto seq = Sequence::create(action1, action2, NULL);
    _spriteDark->stopAllActions();
    _spriteDark->setOpacity(0);
    _spriteDark->runAction(seq);
}

#pragma mark - dungeon map
void LayerPrisonBattle::dungeonMapCreate()
{
    _uiContentsBattleMap->removeAllChildren();
    _listMap.clear();


    //
    std::string pathBGBack = "";
    std::string pathBG = "";

    // path bg back


    // path bg
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
    {
        pathBG = "Assets/battle_bg/prison/bg_prison_1.png";
    }
    else
    {
        pathBG = "Assets/battle_bg/prison/bg_prison_2.png";
    }


    //
    if ( pathBGBack.empty() == false )
    {
        auto spriteBG = Sprite::create(pathBGBack);
        spriteBG->setAnchorPoint(Vec2::ZERO);
        spriteBG->setScale(_scaleBG);
        spriteBG->getTexture()->setTexParameters(texParams);
        _uiContentsBattleMap->addChild(spriteBG);
    }

    //
    for ( int i = 0; i < 3; i++ )
    {
        auto spriteBG = Sprite::create(pathBG);
        spriteBG->setAnchorPoint(Vec2::ZERO);
        spriteBG->setScale(_scaleBG);
        spriteBG->setPosition(spriteBG->getBoundingBox().size.width * i, 0);
        spriteBG->getTexture()->setTexParameters(texParams);
        _uiContentsBattleMap->addChild(spriteBG);

        _listMap.pushBack(spriteBG);
    }
}

void LayerPrisonBattle::dungeonMapMove()
{
    double nSpeedDefault = C_MOVE_MAP_SPEED;
    double nSpeed = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    nSpeedDefault *= 0.5 / (nSpeed * 4);

    for (auto spriteMap : _listMap)
    {
        spriteMap->setPositionX(spriteMap->getPosition().x - nSpeedDefault);
    }

    for (auto spriteMap : _listMap)
    {
        if ( spriteMap->getPosition().x <= (-spriteMap->getBoundingBox().size.width) )
        {
            auto spriteMapFront = _listMap.front();
            auto spriteMapBack = _listMap.back();

            //
            spriteMapFront->setPositionX(spriteMapBack->getPosition().x + spriteMapBack->getBoundingBox().size.width);

            //
            _listMap.erase(0);
            _listMap.pushBack(spriteMapFront);
            break;
        }
    }
}

#pragma mark - callback
void LayerPrisonBattle::callbackDungeonInfo(bool bResult, bool bAvailable, int nType)
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    auto popupPrison =  scene->getChildByName<PopupPrison*>("PopupPrison");
    
    if ( bResult == false )
    {
        if ( popupPrison == nullptr )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            removeFromParent();
        }
        return;
    }
    
    if ( bAvailable == false )
    {
        if ( popupPrison == nullptr )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_42"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            removeFromParent();
        }
        return;
    }
    
    bool bEnterAuto = PrisonManager::getInstance()->isEnterAuto();
    if ( bEnterAuto == false )
    {
        PrisonManager::getInstance()->setEnterAutoCount(0);
        PrisonManager::getInstance()->setEnterAutoCountMax(0);
    }
    
    bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
    int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
    if ( bPlay == false && nEnterAutoCount > 0 )
    {
        onBattleStart();
    }
    
    // draw
    drawInfo();
    drawLobbyNPC(true);
    animNPCBeelzebub(true);
    animNPCBaphomet(true);
}

void LayerPrisonBattle::callbackDungeonResult(bool bResult)
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    auto popupPrison =  scene->getChildByName<PopupPrison*>("PopupPrison");
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if ( popupPrison != nullptr )
        {
            popupPrison->hide();
        }
        else
        {
            removeFromParent();
        }
        return;
    }
    
    //
    if ( popupPrison != nullptr )
    {
        popupPrison->onPopupAdsInterstitial();
        
        PrisonManager::getInstance()->requestInfo(true, (int)PrisonManager::getInstance()->getPrisonType(), CC_CALLBACK_3(LayerPrisonBattle::callbackDungeonInfo, this));
    }
    else
    {
        PrisonManager::getInstance()->requestInfo(false, (int)PrisonManager::getInstance()->getPrisonType(), CC_CALLBACK_3(LayerPrisonBattle::callbackDungeonInfo, this));
    }
}

#pragma mark - schedule
void LayerPrisonBattle::scheduleBattle(float dt)
{
    // time
    _battleTime -= dt;
    PrisonManager::getInstance()->setDungeonTimeNow(_battleTime);
    
    // draw
    drawTimer();
    
    // end
    if ( _battleTime <= 0 )
    {
        // battle stop
        dungeonEnd();
    }
}

void LayerPrisonBattle::scheduleMap(float dt)
{
    dungeonMapMove();
}

#pragma mark - game
void LayerPrisonBattle::setMinimize(bool bMinimize)
{
    _bLayoutMinimize = bMinimize;
    if ( _bLayoutMinimize == true )
        onPopupLockShow();
    else
        onPopupLockHide();
}

void LayerPrisonBattle::onDraw()
{
    // draw
    drawInfo();
    drawLobbyNPC(true);
    animNPCBeelzebub(true);
    animNPCBaphomet(true);
}

void LayerPrisonBattle::onBattleStart()
{
    _battleTime = PrisonManager::getInstance()->getDungeonTimeDefault();
    PrisonManager::getInstance()->setDungeonTimeNow(_battleTime);
    PrisonManager::getInstance()->setDungeonKillAdd(0);
    PrisonManager::getInstance()->setDungeonEarnKey("0");
    
    // npc action
    bool isBafometzAction = false;
    bool isBeelzebubAction = false;
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT && PrisonManager::getInstance()->getDungeonKill() == 0 && UserInfoManager::getInstance()->getPetBafometzHave() == true )
    {
        int count = PrisonManager::getInstance()->getAuthorityCount();
        PrisonManager::getInstance()->setDungeonKillAdd(count);
        
        isBafometzAction = true;
    }
    else if(PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::FIRE && PrisonManager::getInstance()->getDungeonKill() == 0 && CostumeManager::getInstance()->getExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18))
    {
        int count = PrisonManager::getInstance()->getAuthorityCount();
        PrisonManager::getInstance()->setDungeonKillAdd(count);
        
        isBeelzebubAction = true;
    }
    
    if ( isBafometzAction == true )
    {
        //
        PopupLoadingLock::show();
        
        //
        animNPCBaphomet(false);
        
        // blanck white
        auto action1 = DelayTime::create(0.5f);
        auto action2 = FadeIn::create(0.2f);
        auto action3 = DelayTime::create(0.2f);
        auto action4 = CallFuncN::create([=](Node* sender){

            PopupLoadingLock::hide();
            
            //
            animNPCBaphomet(true);
            dungeonStart();
            
            //
            auto spriteWhite = (Sprite*)sender;
            spriteWhite->setOpacity(0);
        });
        auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
        _spriteWhite->setOpacity(0);
        _spriteWhite->runAction(seq);
    }
    else if ( isBeelzebubAction == true )
    {
        //
        PopupLoadingLock::show();
        
        //
        animNPCBeelzebub(false);
        
        // blanck white
        auto action1 = DelayTime::create(0.5f);
        auto action2 = FadeIn::create(0.2f);
        auto action3 = DelayTime::create(0.2f);
        auto action4 = CallFuncN::create([=](Node* sender){

            PopupLoadingLock::hide();
            
            //
            animNPCBeelzebub(true);
            dungeonStart();
            
            //
            auto spriteWhite = (Sprite*)sender;
            spriteWhite->setOpacity(0);
        });
        auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
        _spriteWhite->setOpacity(0);
        _spriteWhite->runAction(seq);
    }
    else
    {
        //
        dungeonStart();
    }
}

void LayerPrisonBattle::onPopupLockShow()
{
    auto uiLayoutLock = ui::Layout::create();
    uiLayoutLock->setContentSize(getContentSize());
    uiLayoutLock->setTouchEnabled(true);
    uiLayoutLock->setName("UI_LOCK");
    uiLayoutLock->addTouchEventListener(CC_CALLBACK_2(LayerPrisonBattle::onTouchBattleLayout, this));
    uiLayoutLock->addClickEventListener(CC_CALLBACK_1(LayerPrisonBattle::onClickBattleLayout, this));
    addChild(uiLayoutLock, INT_MAX);
    
    auto uiImgBoard = ui::ImageView::create("Assets/ui_common/btn_2_7_l_on.png");
    uiImgBoard->setScale9Enabled(true);
    uiImgBoard->setContentSize(Size(uiLayoutLock->getContentSize().width, uiLayoutLock->getContentSize().height));
    uiImgBoard->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiImgBoard->setPositionType(PositionType::PERCENT);
    uiLayoutLock->addChild(uiImgBoard);
        
}

void LayerPrisonBattle::onPopupLockHide()
{
    removeChildByName("UI_LOCK");
}
