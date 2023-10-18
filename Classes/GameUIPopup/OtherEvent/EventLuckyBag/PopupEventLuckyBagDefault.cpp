//
//  PopupEventLuckyBagDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#include "PopupEventLuckyBagDefault.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/OtherEvent/EventLuckyBag/Layer/LayerEventLuckyBagCurrentLvInfo.h"
#include "GameUIPopup/OtherEvent/EventLuckyBag/Layer/LayerEventLuckyBagMain.h"

#include "ManagerEvent/EventLuckyBagManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupEventLuckyBagDefault* PopupEventLuckyBagDefault::create()
{
    PopupEventLuckyBagDefault* pRet = new(std::nothrow) PopupEventLuckyBagDefault();
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

PopupEventLuckyBagDefault::PopupEventLuckyBagDefault():
_bInitUI(false)
,_bInitNetworkInfo(false)
,_uiTop(nullptr)
,_uiCurrentLvInfo(nullptr)
,_uiMain(nullptr)
{
}

PopupEventLuckyBagDefault::~PopupEventLuckyBagDefault(void)
{
    EventLuckyBagManager::getInstance()->unSubDelegateInfo(this);
    EventLuckyBagManager::getInstance()->unSubDelegateProcess(this);
    EventLuckyBagManager::getInstance()->unSubDelegateAds(this);
}
bool PopupEventLuckyBagDefault::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    _bHideBackKey = false;
    initVar();
    initUI();
    
    EventLuckyBagManager::getInstance()->subDelegateInfo(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupEventLuckyBagDefault::callbackInfo, this), this));
    EventLuckyBagManager::getInstance()->subDelegateProcess(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupEventLuckyBagDefault::callbackProgress, this), this));
    EventLuckyBagManager::getInstance()->subDelegateAds(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupEventLuckyBagDefault::callbackAds, this), this));
    return true;
}

void PopupEventLuckyBagDefault::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiTop();
    }
    
    drawTop();
    drawMyReward();
    drawMain();
    
    // 현재 시점에 막기.
    // DialogManager::getInstance()->showDialog("EVENT_ENHANCE_DRAW_OPENNING");
    
    //
    EventLuckyBagManager::getInstance()->requestInfo(true);
}

#pragma mark - ui
void PopupEventLuckyBagDefault::initVar()
{
    double nWidth = 750;
    double nHeight = getContentSize().height;
    if ( nHeight > 1332 )
        nHeight = 1332;
    
    // size
    _spriteContainer->setContentSize( Size(nWidth, nHeight) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
}

void PopupEventLuckyBagDefault::initUI()
{
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_enhance_draw_title_1"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventLuckyBagDefault::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

void PopupEventLuckyBagDefault::uiContainer()
{
    auto layout = ui::Layout::create();
    layout->setContentSize(_spriteContainer->getContentSize() - Size(10, 75));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    layout->setName("PARENT_LAYOUT");
    _spriteContainer->addChild(layout);
    
    _uiTop = ui::Layout::create();
    _uiTop->setContentSize(Size(layout->getContentSize().width, 390));
    _uiTop->setIgnoreAnchorPointForPosition(false);
    _uiTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiTop->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    _uiTop->setClippingEnabled(false);
    layout->addChild(_uiTop);
}

void PopupEventLuckyBagDefault::uiContainerNetwork()
{
    auto layout = (ui::Layout*)utils::findChild(_spriteContainer, "PARENT_LAYOUT");
    
    _uiCurrentLvInfo = LayerEventLuckyBagCurrentLvInfo::create(Size(layout->getContentSize().width, 210));
    _uiCurrentLvInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiCurrentLvInfo->setPosition(_uiTop->getPosition() + Vec2::DOWN * _uiTop->getContentSize().height);
    _uiCurrentLvInfo->setClippingEnabled(false);
    layout->addChild(_uiCurrentLvInfo);
//
    _uiMain = LayerEventLuckyBagMain::create(Size(layout->getContentSize().width, layout->getContentSize().height - _uiTop->getContentSize().height - _uiCurrentLvInfo->getContentSize().height));
    _uiMain->setIgnoreAnchorPointForPosition(false);
    _uiMain->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiMain->setClippingEnabled(false);
    _uiMain->setPosition( _uiCurrentLvInfo->getPosition() + Vec2::DOWN * _uiCurrentLvInfo->getContentSize().height);
    layout->addChild(_uiMain);
    
}

void PopupEventLuckyBagDefault::uiTop()
{
    auto layout = _uiTop;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/event/event_8th_bg_1_1.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width/2, size.height -60));
    uiBG->setScale(4);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    {
        auto anim = Animation::create();
        anim->setDelayPerUnit(0.15);
        for ( int i = 1; i <= 6; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/event_8th_bg_1_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            
            anim->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        auto action1 = Animate::create(anim);
        uiBG->getVirtualRenderer()->runAction(RepeatForever::create(action1));
    }
    layout->addChild(uiBG);
    
    // 설명 버튼
    auto uiBtnHelp = ui::Button::create("Assets/ui/race_event/race_btn_gameinfo.png");
    uiBtnHelp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiBtnHelp->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnHelp->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, 10, size.height - 5));
    uiBtnHelp->addClickEventListener(CC_CALLBACK_1(PopupEventLuckyBagDefault::onClickHelp, this));
    layout->addChild(uiBtnHelp);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_frost_prison_info"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiText->setPosition(Vec2(uiBtnHelp->getContentSize().width - 45, uiBtnHelp->getContentSize().height / 2 + 2));
        uiText->setTextAreaSize(Size(90, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setColor(Color3B(150,120,55));
        uiBtnHelp->addChild(uiText);
    }
    
    auto uiTimer = ui::CText::create("", GAME_FONT, 25);
    uiTimer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiTimer->setPositionType(ui::Widget::PositionType::PERCENT);
    uiTimer->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, uiBtnHelp->getPosition().x + uiBtnHelp->getContentSize().width + 10, size.height - 5));
    uiTimer->setTextAreaSize(Size(420, uiTimer->getContentSize().height));
    uiTimer->setTextOverflow(Label::Overflow::SHRINK);
    uiTimer->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTimer->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTimer->setName("UI_TIMER");
    layout->addChild(uiTimer);
    {
        auto callFunc = CallFunc::create([&, uiTimer](){
            time_t nTimeEnd = EventLuckyBagManager::getInstance()->getEndEventTime();
            int64_t nTimeNow = UtilsDate::getInstance()->getTime();
            
            int nTimeDiff = (int)(nTimeEnd - nTimeNow);

            int day = (int)(nTimeDiff / (24 * 3600));
            if(day >0)
                nTimeDiff = nTimeDiff % (24 * 3600);
            int hour = (int)nTimeDiff / 3600;
            int min = (nTimeDiff % 3600) / 60;

            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            std::string strTime = MafUtils::format("%02d(D):%02d(H):%02d(M)", day, hour, min);
            if(EventLuckyBagManager::getInstance()->isExtraTime())
            {
                strTime = GAME_TEXTFORMAT("t_ui_event_reward_timer_2", strTime.c_str());
            }
            else
            {
                strTime = GAME_TEXTFORMAT("t_ui_event_timer_1", strTime.c_str());
            }
            
            uiTimer->setString(strTime);
        });
        auto seq = Sequence::create(callFunc, DelayTime::create(1), nullptr);
            uiTimer->runAction(RepeatForever::create(seq));
    }
    
    //뽑기 표기
    auto uiCoinBox = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
    uiCoinBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiCoinBox->setScale9Enabled(true);
    uiCoinBox->setContentSize(Size(150, 65));
    uiCoinBox->setPositionType(ui::Widget::PositionType::PERCENT);
    uiCoinBox->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, 20, size.height - 70));
    layout->addChild(uiCoinBox);
    {
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::EVENT_LUCKEY_BACK);
        
        auto uiVoteCoin = ui::ImageView::create(objItem->getPath());
        uiVoteCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiVoteCoin->setScale9Enabled(true);
        uiVoteCoin->setPosition(Vec2(0, uiCoinBox->getContentSize().height/2));
        uiCoinBox->addChild(uiVoteCoin);
        
        auto uiCount = ui::CText::create("", GAME_FONT_AL, 28);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiCount->setPosition(Size(uiCoinBox->getContentSize().width - 10, uiCoinBox->getContentSize().height/2));
        uiCount->setTextAreaSize(Size(uiCoinBox->getContentSize().width - 10 - uiVoteCoin->getContentSize().width, uiCount->getContentSize().height));
        uiCount->setTextOverflow(Label::Overflow::SHRINK);
        uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCount->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiCount->setName("UI_COIN_COUNT");
        uiCoinBox->addChild(uiCount);
    }
    
    //성공 확률
    auto uiRateBox = ui::ImageView::create("Assets/ui/event/event_bubble_1_1.png");
    uiRateBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiRateBox->setPosition(Vec2(size.width - 20, size.height - 65));
    layout->addChild(uiRateBox);
    
    auto uiBarGauge = Sprite::create("Assets/ui/event/event_bubble_2_2.png");
    
    auto uiBarBG = ui::ImageView::create("Assets/ui/event/event_bubble_2_1.png");
    uiBarBG->setScale9Enabled(true);
    uiBarBG->setPosition(Vec2(uiRateBox->getContentSize()/2) + Vec2::UP * 15 + Vec2::RIGHT * 10);
    uiRateBox->addChild(uiBarBG);
    
    auto uiBar = ProgressTimer::create(uiBarGauge);
    uiBar->setType(ProgressTimer::Type::BAR);
    uiBar->setMidpoint(Vec2(0,0));
    uiBar->setBarChangeRate(Vec2(0,1));
    uiBar->setPercentage(50);
    uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBar->setPosition(Vec2(uiBarBG->getContentSize().width/2, 0));
    uiBar->setName("UI_RATE");
    uiBarBG->addChild(uiBar);
    
    auto uiRateText = ui::CText::create("", GAME_FONT, 25);
    uiRateText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiRateText->setPosition(Vec2(uiRateBox->getContentSize().width/2 + 10, 15));
    uiRateText->setTextAreaSize(Size(uiRateBox->getContentSize().width - 10, 40));
    uiRateText->setTextOverflow(Label::Overflow::SHRINK);
    uiRateText->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    uiRateText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiRateText->setName("UI_RATE_TEXT");
    uiRateText->enableBold();
    uiRateText->enableOutline(Color4B::BLACK, 2);
    uiRateBox->addChild(uiRateText);
    
    // 광고 버튼
    auto uiBtnAds = ui::Button::create("Assets/ui_common/btn_2_3_on.png");
    uiBtnAds->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnAds->setScale9Enabled(true);
    uiBtnAds->setContentSize(Size(164, 64));
    uiBtnAds->setPosition(Vec2(size.width - 20, 20));
    uiBtnAds->addClickEventListener(CC_CALLBACK_1(PopupEventLuckyBagDefault::onClickAds, this));
    uiBtnAds->setName("UI_BTN_ADS");
    layout->addChild(uiBtnAds);
    {
        
        auto uiAdsIcon = ui::ImageView::create("Assets/icon/icon_ad_1_2.png");
        uiAdsIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiAdsIcon->setPosition(Vec2(5, uiBtnAds->getContentSize().height));
        uiBtnAds->addChild(uiAdsIcon);
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::EVENT_LUCKEY_BACK);
        
        auto uiVoteCoin = ui::ImageView::create(objItem->getPath());
        uiVoteCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiVoteCoin->setPosition(Vec2(20, uiBtnAds->getContentSize().height/2));
        uiBtnAds->addChild(uiVoteCoin);
        
        auto uiCount = ui::CText::create("", GAME_FONT, 25);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCount->setPosition(uiVoteCoin->getPosition() + Vec2::RIGHT * uiVoteCoin->getContentSize().width);
        uiCount->setTextOverflow(Label::Overflow::SHRINK);
        uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiCount->setName("UI_ADS_COUNT");
        uiBtnAds->addChild(uiCount);
    }
    
    //연출 온오프
    auto uiToggleBox = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
    uiToggleBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiToggleBox->setScale9Enabled(true);
    uiToggleBox->setContentSize(Size(105, 70));
    uiToggleBox->setPosition(Vec2(30, 20));
    layout->addChild(uiToggleBox);
    
    auto uiBtnToggle = ui::Button::create("Assets/ui/mission/mission_on.png");
    uiBtnToggle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtnToggle->setPosition(Vec2(uiToggleBox->getContentSize().width/2, 5));
    uiBtnToggle->setName("UI_TOGGLE");
    uiBtnToggle->addClickEventListener(CC_CALLBACK_1(PopupEventLuckyBagDefault::onClickToggleAnimation, this));
    uiBtnToggle->setScale(0.6f);
    uiToggleBox->addChild(uiBtnToggle);
    
    auto uiToggleText = ui::CText::create(GAME_TEXT("t_ui_prison_enter_frost_skip_bt"), GAME_FONT, 20);
    uiToggleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiToggleText->setPosition(Vec2(uiToggleBox->getContentSize().width/2, uiToggleBox->getContentSize().height));
    uiToggleBox->addChild(uiToggleText);
}

#pragma mark - draw
void PopupEventLuckyBagDefault::drawTop()
{
    auto layout = _uiTop;
    Size size = layout->getContentSize();
    
    if(auto uiCoinCount = (ui::CText*)utils::findChild(layout, "UI_COIN_COUNT"); uiCoinCount != nullptr)
    {
        uiCoinCount->setString(ItemsManager::getInstance()->getItems(E_ITEMS::EVENT_LUCKEY_BACK));
    }
    if(auto uiRate = (ProgressTimer*)utils::findChild(layout, "UI_RATE"); uiRate != nullptr)
    {
        float per = 100 - EventLuckyBagManager::getInstance()->getCurrentSuccessRate();
        //5레벨 부턴 출렁거림이 1%씩 더 증가함 | 0레벨 부터 시작 하기 때문에 1 보정값
        float inc = EventLuckyBagManager::getInstance()->getCurrentLevel() < 4 ? 5 : EventLuckyBagManager::getInstance()->getCurrentLevel() + 1 + 1;
        float speed = EventLuckyBagManager::getInstance()->getCurrentLevel() < 4 ? 0.3f : 0.3f - 0.05 * (EventLuckyBagManager::getInstance()->getCurrentLevel() - 4);
        
        uiRate->setPercentage(per);
        auto seq = Sequence::create( ProgressTo::create(speed, per + 5), ProgressTo::create(speed, per), nullptr);
        uiRate->runAction(RepeatForever::create(seq));
    }
    if(auto uiRateText = (ui::CText*)utils::findChild(layout, "UI_RATE_TEXT"); uiRateText != nullptr)
    {
        uiRateText->setString(GAME_TEXTFORMAT("t_ui_event_enhance_draw_msg_3", EventLuckyBagManager::getInstance()->getCurrentSuccessRate()));
        Color3B color;
        if(EventLuckyBagManager::getInstance()->getCurrentLevel() < 4)
            color = Color3B(72, 255, 0);
        else if(EventLuckyBagManager::getInstance()->getCurrentLevel() < 7)
            color = Color3B(255, 234, 0);
        else
            color = Color3B(255, 79, 117);
        uiRateText->setColor(color);
    }
    if(auto uiAdsCount = (ui::CText*)utils::findChild(layout, "UI_ADS_COUNT"); uiAdsCount != nullptr)
    {
        uiAdsCount->setString(MafUtils::format("%d/%d", EventLuckyBagManager::getInstance()->getAdsMaxCount() - EventLuckyBagManager::getInstance()->getAdsCount(), EventLuckyBagManager::getInstance()->getAdsMaxCount()));
    }
    if(auto uiBtnAds = (ui::Button*)utils::findChild(layout, "UI_BTN_ADS"); uiBtnAds != nullptr)
    {
        if(EventLuckyBagManager::getInstance()->getAdsCount() >= EventLuckyBagManager::getInstance()->getAdsMaxCount() ||
           EventLuckyBagManager::getInstance()->isExtraTime())
            uiBtnAds->setEnabled(false);
        else
            uiBtnAds->setEnabled(true);
    }
    if(auto uiToggle = (ui::Button*)utils::findChild(layout, "UI_TOGGLE"); uiToggle != nullptr)
    {
        if ( EventLuckyBagManager::getInstance()->isEffect() )
        {
            uiToggle->loadTextureNormal("Assets/ui/mission/mission_on.png");
        }
        else
        {
            uiToggle->loadTextureNormal("Assets/ui/mission/mission_off.png");
        }
    }
}
void PopupEventLuckyBagDefault::drawMyReward()
{
    if(_bInitNetworkInfo)
        ((LayerEventLuckyBagCurrentLvInfo*)_uiCurrentLvInfo)->onDrawInfo();
}
void PopupEventLuckyBagDefault::drawMain()
{
    if(_bInitNetworkInfo)
        ((LayerEventLuckyBagMain*)_uiMain)->onDrawInfo();
}

#pragma mark - click
void PopupEventLuckyBagDefault::onClickClose(Ref* sender)
{
    hide();
}
void PopupEventLuckyBagDefault::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(1, "t_ui_event_vote_info_title_%d", "t_ui_event_enhance_draw_info_%d");
    popup->show();
}
void PopupEventLuckyBagDefault::onClickAds(Ref* sender)
{
    if ( EventLuckyBagManager::getInstance()->getAdsCount() >= EventLuckyBagManager::getInstance()->getAdsMaxCount() )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_15");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callBackResultAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupEventLuckyBagDefault::callBackResultAds, this));
    }
    
}

void PopupEventLuckyBagDefault::onClickToggleAnimation(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    EventLuckyBagManager::getInstance()->setIsEffect(!EventLuckyBagManager::getInstance()->isEffect());
    if ( EventLuckyBagManager::getInstance()->isEffect() )
    {
        uiBtn->loadTextureNormal("Assets/ui/mission/mission_on.png");
    }
    else
    {
        uiBtn->loadTextureNormal("Assets/ui/mission/mission_off.png");
    }
    
}
#pragma mark - callback
void PopupEventLuckyBagDefault::callbackInfo(const bool bResult, const int nResult)
{
    /*
     */
    if(nResult == 1)
    {
        // init
        if(!_bInitNetworkInfo)
        {
            _bInitNetworkInfo = true;
            uiContainerNetwork();
        }
        
        drawTop();
        drawMyReward();
        drawMain();
    }
    else
    {
        std::string strMessageKey = "t_ui_error_30";
        if(nResult == 2)
            strMessageKey = "t_ui_draw_msg_1";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
}

void PopupEventLuckyBagDefault::callbackProgress(const bool bResult, const int nResult)
{
    if(nResult == 1)
    {
        EventLuckyBagManager::getInstance()->requestInfo(true);
    }
    else
    {
        std::string strMessageKey = "t_ui_error_30";
        if(nResult == 2)
            strMessageKey = "t_ui_draw_msg_1";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(nResult == 0)
            hide();
        return;
    }
}

void PopupEventLuckyBagDefault::callbackAds(const bool bResult, const int nResult)
{
    /*
     */
    if(nResult == 1)
    {
        drawTop();
        drawMyReward();
        drawMain();
    }
    else
    {
        std::string strMessageKey = "t_ui_error_30";
        if(nResult == 2)
            strMessageKey = "t_ui_draw_msg_1";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
}

#pragma mark -Ads
void PopupEventLuckyBagDefault::callBackResultAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafAnalyticsManager::LogEvent(kAdsSlotEventLuckyBag, "useridx", AccountManager::getInstance()->getUserIdx(), kRepeatTrue);
        
        //
        EventLuckyBagManager::getInstance()->requestAds(true);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupEventLuckyBagDefault::callBackResultAds, this));
            popup->setLogParams(kAnalEventAdsLoad);
            popup->setLogValue(kAdsSlotEventLuckyBag);
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
