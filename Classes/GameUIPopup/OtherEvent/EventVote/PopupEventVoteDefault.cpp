//
//  PopupEventVoteDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupEventVoteDefault.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/OtherEvent/EventVote/Layer/LayerEventVoteParticipation.h"
#include "GameUIPopup/OtherEvent/EventVote/Layer/LayerEventVoteMain.h"
#include "GameUIPopup/OtherEvent/EventVote/PopupEventVotePrevResult.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventVoteManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupEventVoteDefault* PopupEventVoteDefault::create()
{
    PopupEventVoteDefault* pRet = new(std::nothrow) PopupEventVoteDefault();
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

PopupEventVoteDefault::PopupEventVoteDefault():
_bInitUI(false)
,_bInitNetworkInfo(false)
,_uiTop(nullptr)
,_uiParticipation(nullptr)
,_uiVote(nullptr)
{
}

PopupEventVoteDefault::~PopupEventVoteDefault(void)
{
    EventVoteManager::getInstance()->unSubDelegateInfo(this);
    EventVoteManager::getInstance()->unSubDelegateVote(this);
    EventVoteManager::getInstance()->unSubDelegateAds(this);
}
bool PopupEventVoteDefault::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    _bHideBackKey = false;
    initVar();
    initUI();
    
    EventVoteManager::getInstance()->subDelegateInfo(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupEventVoteDefault::callbackInfo, this), this));
    EventVoteManager::getInstance()->subDelegateVote(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupEventVoteDefault::callbackVote, this), this));
    EventVoteManager::getInstance()->subDelegateAds(new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupEventVoteDefault::callbackAds, this), this));
    return true;
}

void PopupEventVoteDefault::onEnter()
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
    drawParticipation();
    drawVote();
    
    DialogManager::getInstance()->showDialog("EVENT_VOTE_OPENNING");
    EventVoteManager::getInstance()->requestInfo(true);
}

#pragma mark - ui
void PopupEventVoteDefault::initVar()
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

void PopupEventVoteDefault::initUI()
{
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_vote_title_1"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventVoteDefault::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

void PopupEventVoteDefault::uiContainer()
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

void PopupEventVoteDefault::uiContainerNetwork()
{
    auto layout = (ui::Layout*)utils::findChild(_spriteContainer, "PARENT_LAYOUT");
    
    _uiParticipation = LayerEventVoteParticipation::create(Size(layout->getContentSize().width, 344));
    _uiParticipation->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiParticipation->setPosition(_uiTop->getPosition() + Vec2::DOWN * _uiTop->getContentSize().height);
    _uiParticipation->setClippingEnabled(false);
    layout->addChild(_uiParticipation);
    
    _uiVote = LayerEventVoteMain::create(Size(layout->getContentSize().width, layout->getContentSize().height - _uiTop->getContentSize().height - _uiParticipation->getContentSize().height));
    _uiVote->setIgnoreAnchorPointForPosition(false);
    _uiVote->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiVote->setClippingEnabled(false);
    _uiVote->setPosition( _uiParticipation->getPosition() + Vec2::DOWN * _uiParticipation->getContentSize().height);
    layout->addChild(_uiVote);
    
}

void PopupEventVoteDefault::uiTop()
{
    auto layout = _uiTop;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/event/minority_event_bg.png");
    uiBG->setAnchorPoint(Vec2::ZERO);
    uiBG->setPosition(Vec2::ZERO);
    uiBG->setScale(size.height / uiBG->getContentSize().height);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    
    auto uiTitle = ui::CText::create("", GAME_FONT, 25);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTitle->setPosition(Vec2(size.width/2, size.height - 5));
    uiTitle->setTextAreaSize(Size(size.width - 50, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setName("UI_TITLE");
    layout->addChild(uiTitle);
    {
        auto callFunc = CallFunc::create([&, uiTitle](){
            time_t nTimeEnd = EventVoteManager::getInstance()->getEndEventTime();
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
            if(EventManager::getInstance()->isActivatedEvent(38))
            {
                strTime = GAME_TEXT("t_ui_event_exchange_center_time2") + " : " + strTime;
            }
            else
            {
                strTime = GAME_TEXT("t_ui_token_text_upper_1") + " : " + strTime;
            }
            
            uiTitle->setString(strTime);
        });
        auto seq = Sequence::create(callFunc, DelayTime::create(1), nullptr);
            uiTitle->runAction(RepeatForever::create(seq));
    }
    
    
    // 설명 버튼
    auto uiBtnHelp = ui::Button::create("Assets/ui/race_event/race_btn_gameinfo.png");
    uiBtnHelp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiBtnHelp->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnHelp->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, 10, size.height - 50));
    uiBtnHelp->addClickEventListener(CC_CALLBACK_1(PopupEventVoteDefault::onClickHelp, this));
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
    // 투표권 표기
    auto uiVoteCoinBox = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
    uiVoteCoinBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiVoteCoinBox->setScale9Enabled(true);
    uiVoteCoinBox->setContentSize(Size(150, 65));
    uiVoteCoinBox->setPositionType(ui::Widget::PositionType::PERCENT);
    uiVoteCoinBox->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width - 10, size.height - 50));
    layout->addChild(uiVoteCoinBox);
    {
        auto uiVoteCoin = ui::ImageView::create("Assets/icon/icon_minority_coin.png");
        uiVoteCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiVoteCoin->setScale9Enabled(true);
        uiVoteCoin->setPosition(Vec2(0, uiVoteCoinBox->getContentSize().height/2));
        uiVoteCoinBox->addChild(uiVoteCoin);
        
        auto uiCount = ui::CText::create("", GAME_FONT_AL, 28);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiCount->setPosition(Size(uiVoteCoinBox->getContentSize().width - 10, uiVoteCoinBox->getContentSize().height/2));
        uiCount->setTextAreaSize(Size(uiVoteCoinBox->getContentSize().width - 10 - uiVoteCoin->getContentSize().width, uiCount->getContentSize().height));
        uiCount->setTextOverflow(Label::Overflow::SHRINK);
        uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCount->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiCount->setName("UI_COIN_COUNT");
        uiVoteCoinBox->addChild(uiCount);
    }
    
    // 이전 투표 결과 버튼
    auto uiBtnPrevVote = ui::Button::create("Assets/ui_common/btn_5.png");
    uiBtnPrevVote->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnPrevVote->setScale9Enabled(true);
    uiBtnPrevVote->setContentSize(Size(164, 56));
    uiBtnPrevVote->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnPrevVote->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width - 10, 20));
    uiBtnPrevVote->addClickEventListener(CC_CALLBACK_1(PopupEventVoteDefault::onClickPrevVote, this));
    layout->addChild(uiBtnPrevVote);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_vote_title_4"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiText->setPosition(uiBtnPrevVote->getContentSize()/2);
        uiText->setTextAreaSize(Size(uiBtnPrevVote->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnPrevVote->addChild(uiText);
    }
    
    // 광고 버튼
    auto uiBtnAds = ui::Button::create("Assets/ui_common/btn_2_3_on.png");
    uiBtnAds->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnAds->setScale9Enabled(true);
    uiBtnAds->setContentSize(Size(164, 64));
    uiBtnAds->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnAds->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width - 10, uiBtnPrevVote->getPosition().y + uiBtnPrevVote->getContentSize().height + 10));
    uiBtnAds->addClickEventListener(CC_CALLBACK_1(PopupEventVoteDefault::onClickAds, this));
    uiBtnAds->setName("UI_BTN_ADS");
    layout->addChild(uiBtnAds);
    {
        
        auto uiAdsIcon = ui::ImageView::create("Assets/icon/icon_ad_1_2.png");
        uiAdsIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiAdsIcon->setPosition(Vec2(5, uiBtnAds->getContentSize().height));
        uiBtnAds->addChild(uiAdsIcon);
        
        auto uiVoteCoin = ui::ImageView::create("Assets/icon/icon_minority_coin.png");
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
}

#pragma mark - draw
void PopupEventVoteDefault::drawTop()
{
    auto layout = _uiTop;
    Size size = layout->getContentSize();
    
    if(auto uiCoinCount = (ui::CText*)utils::findChild(layout, "UI_COIN_COUNT"); uiCoinCount != nullptr)
    {
        uiCoinCount->setString(std::to_string(EventVoteManager::getInstance()->getVoteCoinCount()));
    }
    if(auto uiAdsCount = (ui::CText*)utils::findChild(layout, "UI_ADS_COUNT"); uiAdsCount != nullptr)
    {
        uiAdsCount->setString(MafUtils::format("%d/%d", 5 - EventVoteManager::getInstance()->getAdsCount(), 5));
    }
    if(auto uiBtnAds = (ui::Button*)utils::findChild(layout, "UI_BTN_ADS"); uiBtnAds != nullptr)
    {
        if(EventVoteManager::getInstance()->getAdsCount() >= 5)
            uiBtnAds->setEnabled(false);
        else
            uiBtnAds->setEnabled(true);
        
        if(EventVoteManager::getInstance()->isBreakTime())
            uiBtnAds->setEnabled(false);
    }
}
void PopupEventVoteDefault::drawParticipation()
{
    if(_bInitNetworkInfo)
        ((LayerEventVoteParticipation*)_uiParticipation)->onDrawInfo();
}
void PopupEventVoteDefault::drawVote()
{
    if(_bInitNetworkInfo)
        ((LayerEventVoteMain*)_uiVote)->onDrawInfo();
}

#pragma mark - click
void PopupEventVoteDefault::onClickClose(Ref* sender)
{
    hide();
}
void PopupEventVoteDefault::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(1, "t_ui_event_vote_info_title_%d", "t_ui_event_vote_info_msg_%d");
    popup->show();
}
void PopupEventVoteDefault::onClickPrevVote(Ref* sender)
{
    if(EventVoteManager::getInstance()->getCurrentCandidateGroupNumber() <= 1)
    {
        std::string strMessage = GAME_TEXT("t_ui_event_vote_popup_msg_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    auto popup = PopupEventVotePrevResult::create();
    popup->show();
}
void PopupEventVoteDefault::onClickAds(Ref* sender)
{
    if ( EventVoteManager::getInstance()->getAdsCount() >= 5 )
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
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupEventVoteDefault::callBackResultAds, this));
    }
}

#pragma mark - callback
void PopupEventVoteDefault::callbackInfo(const bool bResult, const int nResult)
{
    /*
     _RESULT_AD_MAX           = 10004            # 광고 최대치

     _RESULT_ALREADY_REWARD         = 20000     # 이미 보상받음
     _RESULT_EVENT_CANT_VOTE        = 20001     # 투표시간이 아님 (정산시간)
     _RESULT_EVENT_TICKET_EMPTY     = 20002     # 티켓이 없음
     _RESULT_EVENT_EMPTY_JOIN       = 20003     # 이벤트 참여한 이력이없음
     _RESULT_EVENT_REWARD_FAIL      = 20004     # 이벤트 조건 충족하지못함
     */
    if(nResult == 1 || nResult == 20001)
    {
        // init
        if(!_bInitNetworkInfo)
        {
            _bInitNetworkInfo = true;
            uiContainerNetwork();
        }
        
        drawTop();
        drawParticipation();
        drawVote();
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

void PopupEventVoteDefault::callbackVote(const bool bResult, const int nResult)
{
    if(nResult == 1 || nResult == 20001)
    {
        drawTop();
        drawParticipation();
        drawVote();
    }
    else
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 20002)
            strMessageKey = "t_ui_error_31";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(nResult == 0 || nResult == 20004)
            hide();
        return;
    }
    
}

void PopupEventVoteDefault::callbackAds(const bool bResult, const int nResult)
{
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    EventVoteManager::getInstance()->requestInfo(true);
}


#pragma mark -Callback
void PopupEventVoteDefault::callBackResultAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafAnalyticsManager::LogEvent(kAdsSlotEventVote, "useridx", AccountManager::getInstance()->getUserIdx(), kRepeatTrue);
        
        //
        EventVoteManager::getInstance()->requestAds(true);
    }
    else
    {
        if(result == ADS_RESULT::LOAD)
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupEventVoteDefault::callBackResultAds, this));
            popup->setLogParams(kAnalEventAdsLoad);
            popup->setLogValue(kAdsSlotEventVote);
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
