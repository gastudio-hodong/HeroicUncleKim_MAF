//
//  PopupAdsInterstitial.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupAdsInterstitial.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupAdsInterstitial* PopupAdsInterstitial::create(int nType)
{
    PopupAdsInterstitial* pRet = new(std::nothrow) PopupAdsInterstitial();
    if ( pRet && pRet->init(nType) )
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

PopupAdsInterstitial::PopupAdsInterstitial(void) :
_nType(0),

_nTimeHide(0),
_nTimeHideMax(0),

_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupAdsInterstitial::~PopupAdsInterstitial(void)
{
    
}

bool PopupAdsInterstitial::init(int nType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _nType = nType;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupAdsInterstitial::onEnter()
{
    PopupBase::onEnter();
    
    //
    this->schedule(schedule_selector(PopupAdsInterstitial::scheduleHide), 1.0);
}

#pragma mark - init
void PopupAdsInterstitial::initVar()
{
    //
    _nTimeHide = 6;
    _nTimeHideMax = 6;
}

void PopupAdsInterstitial::initUi()
{
    Size size = Size(500, 544);

    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_ads_title_%d", _nType)), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    
    //
    uiContents();
    uiButton();
}


#pragma mark - ui
void PopupAdsInterstitial::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    auto spriteAd = Sprite::create("Assets/ui/ads/bg_ad_1_1.png");
    spriteAd->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteAd->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    _layerContainerContents->addChild(spriteAd);
    
    auto label = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_ads_desc_%d", _nType)), GAME_FONT, 26);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    label->setPosition(_layerContainerContents->getContentSize().width / 2, 10);
    label->setDimensions(_layerContainerContents->getContentSize().width, 120);
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(label);
}

void PopupAdsInterstitial::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupAdsInterstitial::onClickClose, this));
    menu->addChild(itemClose);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 24);
        label->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(label);
    }
    
    auto itemAds = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupAdsInterstitial::onClickAds, this));
    menu->addChild(itemAds);
    {
        auto spriteIcon = Sprite::create("Assets/icon/icon_ad_1_1.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteIcon->setPosition(itemAds->getContentSize().width / 2, itemAds->getContentSize().height / 2);
        itemAds->addChild(spriteIcon);

        _labelButtonClose = Label::createWithTTF(MafUtils::format("%d", _nTimeHide), GAME_FONT, 24);
        _labelButtonClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _labelButtonClose->setPosition(itemClose->getContentSize().width / 2 + 10, itemClose->getContentSize().height / 2);
        itemAds->addChild(_labelButtonClose);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupAdsInterstitial::drawButton()
{
    std::string strTemp = MafUtils::format("%d", _nTimeHide);
    _labelButtonClose->setString(strTemp);
}

#pragma mark - set, get, sort, add
void PopupAdsInterstitial::setCallbackResult(const std::function<void(int)>& callback)
{
    _callbackResult = callback;
}


#pragma mark - schedule
void PopupAdsInterstitial::scheduleHide(float dt)
{
    _nTimeHide -= 1;
    if ( _nTimeHide < 0 )
    {
        _nTimeHide = _nTimeHideMax;
        this->unschedule(schedule_selector(PopupAdsInterstitial::scheduleHide));

        //
        if ( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            callbackAdsInterstitial(ADS_RESULT::SUCCESS);
        }
        else
        {
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupAdsInterstitial::callbackAdsInterstitial, this));
        }
        return;
    }
    
    //
    drawButton();
}

#pragma mark - click
void PopupAdsInterstitial::onClickClose(cocos2d::Ref* sender)
{
    _nTimeHide = _nTimeHideMax;
    this->unschedule(schedule_selector(PopupAdsInterstitial::scheduleHide));
    
    //
    hide();
}

void PopupAdsInterstitial::onClickAds(cocos2d::Ref* sender)
{
    _nTimeHide = _nTimeHideMax;
    this->unschedule(schedule_selector(PopupAdsInterstitial::scheduleHide));
    
    //
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAdsInterstitial(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupAdsInterstitial::callbackAdsInterstitial, this));
    }
}

#pragma mark - callback
void PopupAdsInterstitial::callbackAdsInterstitial(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        if ( _nType == 1 )
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotPrisonPlus, CashManager::getInstance()->isAdsfreeTime());
        else if ( _nType == 2 )
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotExpedPlus, CashManager::getInstance()->isAdsfreeTime());
        else if ( _nType == 3 )
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotExpedJump, CashManager::getInstance()->isAdsfreeTime());
        
        //
        if ( _callbackResult != nullptr )
        {
            _callbackResult(_nType);
        }
        
        //
        hide();
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupAdsInterstitial::callbackAdsInterstitial, this));
            if ( _nType == 1 )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotPrisonPlus);
            }
            else if ( _nType == 2 )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotExpedPlus);
            }
            else if ( _nType == 3 )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotExpedJump);
            }
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
