//
//  PopupEventRace.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/21.
//

#include "PopupEventRace.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "GameUIPopup/Other/Ranking/PopupPageRanking.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceMission.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceReward.h"
#include "GameUIPopup/OtherEvent/EventRace/UI/UIRaceCommentary.h"
#include "GameUIPopup/OtherEvent/EventRace/Layer/LayerEventRaceTopUI.h"
#include "GameUIPopup/OtherEvent/EventRace/Layer/LayerEventRaceMainGame.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/CashManager.h"


USING_NS_CC;

PopupEventRace* PopupEventRace::create()
{
    PopupEventRace* pRet = new(std::nothrow) PopupEventRace();
    if ( pRet && pRet->init() )
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

PopupEventRace::PopupEventRace():
_lyContainerParent(nullptr)
,_lyBanner(nullptr)
,_lyTop(nullptr)
,_lyMainGame(nullptr)
,_lyChat(nullptr)
,_lyButton(nullptr)
,_lyAds(nullptr)
{
}

PopupEventRace::~PopupEventRace(void)
{
    MafGoogleRewardAds::HideGoogleBanner();
    EventRaceManager::getInstance()->unSubInvestmentEvent(this);
    EventRaceManager::getInstance()->unSubRaceGameOverEvent(this);
    SoundManager::ChangeSoundState(E_SOUND_TYPE::SCENE);
}
bool PopupEventRace::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    _bHideBackKey = false;
    initVar();
    
    EventRaceManager::getInstance()->subInvestmentEvent(new MafFunction<void(int)>(CC_CALLBACK_1(PopupEventRace::onEventInvest, this), this));
    EventRaceManager::getInstance()->subRaceGameOverEvent(new MafFunction<void(bool)>(CC_CALLBACK_1(PopupEventRace::onEventGameOver, this), this));
    EventRaceManager::getInstance()->subEventOverEvent(new MafFunction<void()>(CC_CALLBACK_0(PopupEventRace::onEventEventOver, this), this));
    return true;
}

void PopupEventRace::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    EventRaceManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventRace::callbackInfo, this));
    EventRaceManager::getInstance()->requestMission(nullptr);
}

#pragma mark - ui
void PopupEventRace::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupEventRace::initUI()
{
    Size size = getContentSize();
    bool isSizeOver = false;
    if(size.width >= 750)
    {
        size.width = 750;
        isSizeOver = true;
    }
    
    _lyContainerParent = Layer::create();
    _lyContainerParent->setContentSize(size);
    _lyContainerParent->setIgnoreAnchorPointForPosition(false);
    _lyContainerParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyContainerParent->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    addChild(_lyContainerParent);
    if(isSizeOver)
    {
        auto sprEdge = ui::Scale9Sprite::create("Assets/ui/race_event/race_popup_edge.png", Rect::ZERO, Rect(3,3,4,4));
        sprEdge->setContentSize(_lyContainerParent->getContentSize() + Size(3,3));
        sprEdge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprEdge->setPosition(_lyContainerParent->getContentSize()/2);
        _lyContainerParent->addChild(sprEdge);
    }
    
    _lyBanner = LayerColor::create();
    _lyBanner->setContentSize(Size(_lyContainerParent->getContentSize().width, 124));
    _lyBanner->setIgnoreAnchorPointForPosition(false);
    _lyBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBanner->setPosition(_lyContainerParent->getContentSize().width / 2, _lyContainerParent->getContentSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    _lyContainerParent->addChild(_lyBanner);
    
    _lyTop = LayerColor::create(Color4B::BLACK);
    _lyTop->setContentSize(Size(_lyContainerParent->getContentSize().width, 56));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_lyBanner->getPosition() + Vec2::DOWN * _lyBanner->getContentSize().height);
    _lyContainerParent->addChild(_lyTop);
    
    _lyMainGame = LayerColor::create(Color4B::BLACK);
    _lyMainGame->setContentSize( Size(_lyContainerParent->getContentSize().width, 852) );
    _lyMainGame->setIgnoreAnchorPointForPosition(false);
    _lyMainGame->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMainGame->setPosition( _lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _lyContainerParent->addChild(_lyMainGame);
    
    _lyAds = LayerColor::create();
    _lyAds->setContentSize(Size(_lyContainerParent->getContentSize().width, 108));
    _lyAds->setIgnoreAnchorPointForPosition(false);
    _lyAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyAds->setPosition(_lyContainerParent->getContentSize().width / 2, 0);
    _lyContainerParent->addChild(_lyAds);
    
    _lyButton = LayerColor::create(Color4B::BLACK);
    _lyButton->setContentSize( Size(_lyContainerParent->getContentSize().width, 100) );
    _lyButton->setIgnoreAnchorPointForPosition(false);
    _lyButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _lyButton->setPosition( _lyAds->getPosition() + Vec2::UP * _lyAds->getContentSize().height);
    _lyContainerParent->addChild(_lyButton);
    
    _lyChat = LayerColor::create(Color4B::BLACK);
    _lyChat->setContentSize( Size(_lyContainerParent->getContentSize().width,
                                  (_lyMainGame->getPosition().y - _lyMainGame->getContentSize().height) - (_lyButton->getPosition().y + _lyButton->getContentSize().height)));
    _lyChat->setIgnoreAnchorPointForPosition(false);
    _lyChat->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _lyChat->setPosition( _lyButton->getPosition() + Vec2::UP * _lyButton->getContentSize().height);
    _lyContainerParent->addChild(_lyChat);
    
    uiBanner();
    uiTop();
    uiMainGame();
    uiChat();
    uiButton();
    uiAds();
    MafGoogleRewardAds::ShowGoogleBanner();
}

void PopupEventRace::uiBanner()
{
    _lyBanner->removeAllChildren();
    
    auto sprBanner = Sprite::create("Assets/ui/race_event/race_top_img.png");
    sprBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBanner->setPosition(_lyBanner->getContentSize().width/2, _lyBanner->getContentSize().height/2);
    sprBanner->setScale(2);
    sprBanner->getTexture()->setTexParameters(texParams);
    _lyBanner->addChild(sprBanner);
    
    std::string strTitlePath = "Assets/ui/race_event/race_title_en.png";
    if(TextManager::getInstance()->getLang().compare("ko") == 0)
        strTitlePath = "Assets/ui/race_event/race_title.png";
    else if(TextManager::getInstance()->getLang().compare("jp") == 0)
        strTitlePath = "Assets/ui/race_event/race_title_jn.png";
    else if(TextManager::getInstance()->getLang().compare("zh_tw") == 0)
        strTitlePath = "Assets/ui/race_event/race_title_ta.png";
    
    auto sprTitle = Sprite::create(strTitlePath);
    sprTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprTitle->setPosition(_lyBanner->getContentSize().width- 20, _lyBanner->getContentSize().height/2);
    sprTitle->setScale(2.f);
    sprTitle->getTexture()->setTexParameters(texParams);
    _lyBanner->addChild(sprTitle);
    
    auto btnClose = ui::Button::create("Assets/ui/race_event/race_btn_back.png");
    btnClose->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnClose->setPosition(Vec2(5, _lyBanner->getContentSize().height - 5));
    btnClose->setZoomScale(0.02f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupEventRace::onClickClose, this));
    _lyBanner->addChild(btnClose);

}
void PopupEventRace::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto layerTopUI = LayerEventRaceTopUI::create(_lyTop->getContentSize());
    layerTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layerTopUI->setPosition(_lyTop->getContentSize()/2);
    _lyTop->addChild(layerTopUI);
}
void PopupEventRace::uiMainGame()
{
    _lyMainGame->removeAllChildren();
    
    auto uiMainGame = LayerEventRaceMainGame::create(_lyMainGame->getContentSize());
    uiMainGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiMainGame->setPosition(_lyMainGame->getContentSize()/2);
    _lyMainGame->addChild(uiMainGame);
    
}

void PopupEventRace::uiChat()
{
    _lyChat->removeAllChildren();
    
    auto uiCommentary = UIRaceCommentary::create(_lyChat->getContentSize() - Size(20, 20));
    uiCommentary->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCommentary->setPosition(_lyChat->getContentSize()/2);
    _lyChat->addChild(uiCommentary);
}

void PopupEventRace::uiButton()
{
    _lyButton->removeAllChildren();
    
    std::string strBtnRankPath = "Assets/ui/race_event/race_btn_big_purple.png";
    std::string strBtnMissionPath = "Assets/ui/race_event/race_btn_big_navy.png";
    std::string strBtnDoubleAds = "Assets/ui/race_event/race_btn_big_blue.png";
    std::string strBtnHiddenAds = "Assets/ui/race_event/race_btn_big_blue.png";
    std::string strBtnStart = "Assets/ui/race_event/race_btn_big_yellow.png";
    
    if(EventRaceManager::getInstance()->isGameRunning())
    {
        strBtnRankPath = "Assets/ui/race_event/race_btn_big_gray.png";
        strBtnMissionPath = "Assets/ui/race_event/race_btn_big_gray.png";
        strBtnDoubleAds = "Assets/ui/race_event/race_btn_big_gray.png";
        strBtnHiddenAds = "Assets/ui/race_event/race_btn_big_gray.png";
        strBtnStart = "Assets/ui/race_event/race_btn_big_gray.png";
    }
    
    auto btnRank = ui::Button::create(strBtnRankPath);
    btnRank->setScale9Enabled(true);
    btnRank->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnRank->setContentSize(Size(84, 84));
    btnRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnRank->setPosition(Vec2(15, _lyButton->getContentSize().height/2));
    btnRank->setZoomScale(0.02f);
    btnRank->addClickEventListener(CC_CALLBACK_1(PopupEventRace::onClickRank, this));
    _lyButton->addChild(btnRank);
    {
        auto sprIcon = Sprite::create("Assets/ui/ranking/icon_ranking.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprIcon->setPosition(btnRank->getContentSize().width/2, btnRank->getContentSize().height - 5);
        btnRank->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_rank_title"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition(btnRank->getContentSize().width/2, 5);
        lbText->setDimensions(btnRank->getContentSize().width - 10, 40);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnRank->addChild(lbText);
    }
    
    auto btnMission = ui::Button::create(strBtnMissionPath);
    btnMission->setScale9Enabled(true);
    btnMission->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnMission->setContentSize(Size(84, 84));
    btnMission->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnMission->setPosition(btnRank->getPosition() + Vec2::RIGHT * (btnRank->getContentSize().width + 10 ));
    btnMission->setZoomScale(0.02f);
    btnMission->addClickEventListener(CC_CALLBACK_1(PopupEventRace::onClickMission, this));
    _lyButton->addChild(btnMission);
    {
        auto sprIcon = Sprite::create("Assets/ui/race_event/icon_mission_s.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprIcon->setPosition(btnMission->getContentSize().width/2, btnMission->getContentSize().height - 5);
        btnMission->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_event_race_mission_bt"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition(btnMission->getContentSize().width/2, 5);
        lbText->setDimensions(btnMission->getContentSize().width - 10, 40);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnMission->addChild(lbText);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(3, btnMission->getContentSize().height - 10);
        uiReddot->setVisible(false);
        btnMission->addChild(uiReddot);
        
        RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::EVENT_RACE_MISSION, uiReddot);
    }
    
    if(EventRaceManager::getInstance()->getGameRewardMag() > 1 ||
       EventRaceManager::getInstance()->isEventEndSeason())
    {
        strBtnDoubleAds = "Assets/ui/race_event/race_btn_big_gray.png";
    }
    auto btnDoubleAds = ui::Button::create(strBtnDoubleAds);
    btnDoubleAds->setScale9Enabled(true);
    btnDoubleAds->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnDoubleAds->setContentSize(Size(182, 84));
    btnDoubleAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnDoubleAds->setPosition(btnMission->getPosition() + Vec2::RIGHT * (btnMission->getContentSize().width + 10 ));
    btnDoubleAds->setZoomScale(0.02f);
    btnDoubleAds->addClickEventListener(CC_CALLBACK_1(PopupEventRace::onClickDoubleAds, this));
    _lyButton->addChild(btnDoubleAds);
    {
        auto sprAdsIcon = Sprite::create("Assets/icon/icon_race_ad.png");
        sprAdsIcon->setPosition(btnDoubleAds->getContentSize().width / 2, btnDoubleAds->getContentSize().height - 21);
        btnDoubleAds->addChild(sprAdsIcon);
        
        auto sprDoubleIcon = Sprite::create("Assets/ui/race_event/ad_x2_mini.png");
        sprDoubleIcon->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        sprDoubleIcon->setPosition(btnDoubleAds->getContentSize().width + 15, btnDoubleAds->getContentSize().height + 15);
        btnDoubleAds->addChild(sprDoubleIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_event_race_double_chance_bt"), GAME_FONT, 24);
        lbText->setPosition(btnDoubleAds->getContentSize().width / 2, 21);
        lbText->setDimensions(btnDoubleAds->getContentSize().width - 10, 40);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnDoubleAds->addChild(lbText);
    }
    
    if(EventRaceManager::getInstance()->isHiddenAds() ||
       EventRaceManager::getInstance()->isEventEndSeason())
    {
        strBtnHiddenAds = "Assets/ui/race_event/race_btn_big_gray.png";
    }
    auto btnHiddenAds = ui::Button::create(strBtnHiddenAds);
    btnHiddenAds->setScale9Enabled(true);
    btnHiddenAds->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnHiddenAds->setContentSize(Size(144, 84));
    btnHiddenAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnHiddenAds->setPosition(btnDoubleAds->getPosition() + Vec2::RIGHT * (btnDoubleAds->getContentSize().width + 10 ));
    btnHiddenAds->setZoomScale(0.02f);
    btnHiddenAds->addClickEventListener(CC_CALLBACK_1(PopupEventRace::onClickHiddenAds, this));
    _lyButton->addChild(btnHiddenAds);
    {
        auto sprIcon = Sprite::create("Assets/icon/icon_race_ad.png");
        sprIcon->setPosition(btnHiddenAds->getContentSize().width / 2, btnHiddenAds->getContentSize().height - 21);
        btnHiddenAds->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_event_race_hidden_ads_1"), GAME_FONT, 24);
        lbText->setPosition(btnHiddenAds->getContentSize().width / 2, 21);
        lbText->setDimensions(btnHiddenAds->getContentSize().width - 10, 40);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnHiddenAds->addChild(lbText);
    }
    
    if(EventRaceManager::getInstance()->isEventEndSeason())
    {
        strBtnStart = "Assets/ui/race_event/race_btn_big_gray.png";
    }
    auto btnStart = ui::Button::create(strBtnStart);
    btnStart->setScale9Enabled(true);
    btnStart->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnStart->setContentSize(Size(192, 84));
    btnStart->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnStart->setPosition(btnHiddenAds->getPosition() + Vec2::RIGHT * (btnHiddenAds->getContentSize().width + 10 ));
    btnStart->setZoomScale(0.02f);
    btnStart->addClickEventListener(CC_CALLBACK_1(PopupEventRace::onClickStart, this));
    _lyButton->addChild(btnStart);
    {
        auto sprIcon = Sprite::create("Assets/icon/icon_race_ticket.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(10, btnStart->getContentSize().height/2);
        btnStart->addChild(sprIcon);
        
        auto sprStart = Sprite::create("Assets/ui/race_event/race_start_text.png");
        sprStart->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprStart->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width - 5));
        btnStart->addChild(sprStart);
    }
    
    if(EventRaceManager::getInstance()->isEventEndSeason())
    {
        btnDoubleAds->setEnabled(false);
        btnHiddenAds->setEnabled(false);
        btnStart->setEnabled(false);
    }
}

void PopupEventRace::uiAds()
{
    _lyAds->removeAllChildren();
    
    auto sprAdsBox = ui::Scale9Sprite::create("Assets/ui/race_event/race_bottom_ad_box.png", Rect::ZERO, Rect(12,13,16,18));
    sprAdsBox->setContentSize(_lyAds->getContentSize());
    sprAdsBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprAdsBox->setPosition(_lyAds->getContentSize()/2);
    _lyAds->addChild(sprAdsBox);
}

#pragma mark - ui
void PopupEventRace::onClickClose(Ref* sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    hide();
}

void PopupEventRace::onClickRank(Ref* sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    
    auto popup = PopupPageRanking::create(E_RANK_LOCAL_TYPE::EVENT_RACE);
    popup->show();
}

void PopupEventRace::onClickMission(Ref* sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    
    
    auto popup = PopupEventRaceMission::create();
    popup->show();
}

void PopupEventRace::onClickDoubleAds(Ref* sender)
{
    if ( EventRaceManager::getInstance()->isGameRunning() )
        return;
    
    if ( EventRaceManager::getInstance()->getGameRewardMag() > 1 )
        return;
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callBackResultDoubleAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupEventRace::callBackResultDoubleAds, this));
    }
}

void PopupEventRace::onClickHiddenAds(Ref* sender)
{
    if ( EventRaceManager::getInstance()->isGameRunning())
        return;

    if ( EventRaceManager::getInstance()->isHiddenAds() )
        return;
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callBackResultHiddenAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupEventRace::callBackResultHiddenAds, this));
    }
}

void PopupEventRace::onClickStart(Ref* sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    
    if(EventRaceManager::getInstance()->getInvestCount() <= 0||
       EventRaceManager::getInstance()->getTicket() <= 0)
    {
        std::string strMessageKey = "t_ui_event_race_error_1";
        if(EventRaceManager::getInstance()->getTicket() <= 0)
            strMessageKey = "t_ui_event_race_error_2";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(EventRaceManager::getInstance()->isEventEndSeason())
    {
        PopupLoading::show();
        EventRaceManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventRace::callbackGameOver, this));
        EventRaceManager::getInstance()->requestMission(nullptr);
        return;
    }
    
    PopupLoading::show();
    EventRaceManager::getInstance()->requestGameStart(CC_CALLBACK_2(PopupEventRace::callbackGameStart, this));
}

#pragma mark - event
void PopupEventRace::onEventInvest(const int nIdx)
{
    uiTop();
}
void PopupEventRace::onEventGameOver(const bool isBankrupt)
{
    if(isBankrupt)
    {
        if(DialogManager::getInstance()->isViewDialog("EVENT_RACE_EP01") == 0)
        {
            PopupLoading::show();
            EventRaceManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventRace::callbackGameOver, this));
            EventRaceManager::getInstance()->requestMission(nullptr);
            
            DialogManager::getInstance()->showDialog("EVENT_RACE_EP01");
            return;
        }
        
        std::string strMessageKey = "t_ui_event_race_no_coin";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref*){
            PopupLoading::show();
            EventRaceManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventRace::callbackGameOver, this));
            EventRaceManager::getInstance()->requestMission(nullptr);
        });
        popup->show();
        return;
    }
    
    PopupLoading::show();
    EventRaceManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventRace::callbackGameOver, this));
    EventRaceManager::getInstance()->requestMission(nullptr);
}

void PopupEventRace::onEventEventOver()
{
    PopupLoading::show();
    EventRaceManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventRace::callbackGameOver, this));
    EventRaceManager::getInstance()->requestMission(nullptr);
}
#pragma mark - Ads callback
void PopupEventRace::callBackResultHiddenAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        if ( CashManager::getInstance()->isAdsfreeTime() )
        {
            MafAnalyticsManager::LogEvent(kAdsSlotEventRaceHiddenAds, kAnalEventAdsParameterSkip, "Y", kRepeatTrue);
        }
        else
        {
            MafAnalyticsManager::LogEvent(kAdsSlotEventRaceHiddenAds, kAnalEventAdsParameterSkip, "N", kRepeatTrue);
        }
        
        //
        PopupLoading::show();
        EventRaceManager::getInstance()->requestAds(CC_CALLBACK_3(PopupEventRace::callbackAds, this), 1);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupEventRace::callBackResultHiddenAds, this));
            popup->setLogParams(kAdsSlotEventRaceHiddenAds);
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
void PopupEventRace::callBackResultDoubleAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        if ( CashManager::getInstance()->isAdsfreeTime() )
        {
            MafAnalyticsManager::LogEvent(kAdsSlotEventRaceDoubleAds, kAnalEventAdsParameterSkip, "Y", kRepeatTrue);
        }
        else
        {
            MafAnalyticsManager::LogEvent(kAdsSlotEventRaceDoubleAds, kAnalEventAdsParameterSkip, "N", kRepeatTrue);
        }
        
        //
        PopupLoading::show();
        EventRaceManager::getInstance()->requestAds(CC_CALLBACK_3(PopupEventRace::callbackAds, this), 2);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupEventRace::callBackResultDoubleAds, this));
            popup->setLogParams(kAdsSlotEventRaceDoubleAds);
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

#pragma mark - callback
void PopupEventRace::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    // init
    initUI();
    
    SoundManager::ChangeSoundState(E_SOUND_TYPE::CONTENTS);
    SoundManager::SoundBgmStart(SOUND_EVENT_RACE_BGM_1, E_SOUND_TYPE::CONTENTS);
    
    if(UserDefault::getInstance()->getBoolForKey("event_race_first_open", true))
    {
        auto popup = PopupHelp::create(4, "t_ui_event_race_info_title_%d", "t_ui_event_race_info_msg_%d");
        popup->show();
        UserDefault::getInstance()->setBoolForKey("event_race_first_open", false);
    }
}

void PopupEventRace::callbackAds(bool bResult, int nResult, int nType)
{
    PopupLoading::hide();
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    uiButton();
    EventRaceManager::getInstance()->invokeAdsEvent(nType);
}

void PopupEventRace::callbackGameStart(bool bResult, int nResult)
{
    PopupLoading::hide();
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    uiButton();
    EventRaceManager::getInstance()->invokeRaceReadyEvent();
}

void PopupEventRace::callbackGameOver(bool bResult, int nResult)
{
    PopupLoading::hide();
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    uiTop();
    uiMainGame();
    uiChat();
    uiButton();
}
void PopupEventRace::callbackRankingHide()
{
    MafGoogleRewardAds::ShowGoogleBanner();
}
