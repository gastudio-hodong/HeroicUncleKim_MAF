//
//  PopupRaidBuyAttackCount.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/10.
//

#include "PopupRaidBuyAttackCount.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupRaidBuyAttackCount* PopupRaidBuyAttackCount::create()
{
    PopupRaidBuyAttackCount* pRet = new(std::nothrow) PopupRaidBuyAttackCount();
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

PopupRaidBuyAttackCount::PopupRaidBuyAttackCount(void) :
_layerContainerContents(nullptr)
,_layerContainerButton(nullptr)
,_callbackResult(nullptr)
{
    
}

PopupRaidBuyAttackCount::~PopupRaidBuyAttackCount(void)
{
    
}

bool PopupRaidBuyAttackCount::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupRaidBuyAttackCount::initVar()
{

}

void PopupRaidBuyAttackCount::initUi()
{
    Size size = Size(500, 444);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 130) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 15) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_raid_pop_title_att"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupRaidBuyAttackCount::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupRaidBuyAttackCount::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_raid_pop_msg_att"), GAME_FONT, 25);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbContent->setPosition(_layerContainerContents->getContentSize()/2);
    lbContent->setDimensions(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height);
    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    _layerContainerContents->addChild(lbContent);
    
}

void PopupRaidBuyAttackCount::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    auto objAdsCntSetting = NewRaidManager::getInstance()->getRaidSetting("max_ads_attack_count");
    auto objCoinPriceSetting = NewRaidManager::getInstance()->getRaidSetting("devil_attack_price");
    
    int nAdsCnt = 0;
    int nCoinPrice = 0;
    
    if(objAdsCntSetting != nullptr)
    {
        nAdsCnt = objAdsCntSetting->getValueToInt() - NewRaidManager::getInstance()->getAdsBuyCount();
    }
    
    if(objCoinPriceSetting != nullptr)
    {
        int nOriginCoinPrice = objCoinPriceSetting->getValueToInt();
        int nCoinCycle = NewRaidManager::getInstance()->getRaidSetting("devil_attack_cycle")->getValueToInt();
        int nCoinIncrease = NewRaidManager::getInstance()->getRaidSetting("devil_attack_increase")->getValueToInt();
        
        nCoinPrice = nOriginCoinPrice + NewRaidManager::getInstance()->getInstance()->getBuyCount() / nCoinCycle * nCoinIncrease;
    }
    
    auto btnAds = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnAds->setPosition(Vec2(15, _layerContainerButton->getContentSize().height/2));
    btnAds->setZoomScale(0.05f);
    btnAds->addClickEventListener(CC_CALLBACK_1(PopupRaidBuyAttackCount::onClickAds, this));
    _layerContainerButton->addChild(btnAds);
    {
        auto sprIcon = Sprite::create("Assets/icon/icon_ad_1_1.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprIcon->setPosition(Vec2(15, btnAds->getContentSize().height + 5));
        btnAds->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_randombox_msg_2"), GAME_FONT, 22);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width + Vec2::DOWN * (sprIcon->getContentSize().height/2));
        lbText->setDimensions(btnAds->getContentSize().width - sprIcon->getContentSize().width - 15, lbText->getContentSize().height);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        btnAds->addChild(lbText);
        
        auto lbCount = Label::createWithTTF(MafUtils::format("(%d/%d)", nAdsCnt,objAdsCntSetting->getValueToInt()), GAME_FONT, 22);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbCount->setPosition(btnAds->getContentSize().width/2, 5);
        lbCount->setDimensions(btnAds->getContentSize().width - 15, lbCount->getContentSize().height);
        lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbCount->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnAds->addChild(lbCount);
        
        if(nAdsCnt <= 0)
        {
            btnAds->setEnabled(false);
        }
    }
    
    auto btnBuyCoin = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnBuyCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnBuyCoin->setPosition(Vec2(_layerContainerButton->getContentSize().width - 15, _layerContainerButton->getContentSize().height/2));
    btnBuyCoin->setZoomScale(0.05f);
    btnBuyCoin->addClickEventListener(CC_CALLBACK_1(PopupRaidBuyAttackCount::onClickBuy, this));
    _layerContainerButton->addChild(btnBuyCoin);
    {
        auto sprIcon = Sprite::create("Assets/icon/icon_coin_2.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(Vec2(15, btnBuyCoin->getContentSize().height/2));
        btnBuyCoin->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(std::to_string(nCoinPrice), GAME_FONT, 22);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width);
        lbText->setDimensions(btnBuyCoin->getContentSize().width - sprIcon->getContentSize().width - 15, lbText->getContentSize().height);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        btnBuyCoin->addChild(lbText);
    }
}

#pragma mark - utils
void PopupRaidBuyAttackCount::onClickClose(cocos2d::Ref* sender)
{
    if(_callbackResult != nullptr)
        _callbackResult();
    
    hide();
}

void PopupRaidBuyAttackCount::onClickBuy(cocos2d::Ref* sender)
{
    auto objCntSetting = NewRaidManager::getInstance()->getRaidSetting("max_devil_attack_count");
    auto objPriceSetting = NewRaidManager::getInstance()->getRaidSetting("devil_attack_price");
    
    int nCnt = 0;
    int nPrice = 0;
    int nNowCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
    
    if(objCntSetting != nullptr)
    {
        nCnt = objCntSetting->getValueToInt() - NewRaidManager::getInstance()->getBuyCount();
    }
    
    if(objPriceSetting != nullptr)
    {
        nPrice = objPriceSetting->getValueToInt();
    }
    
    if(nCnt <= 0)
    {
        std::string strMessage = GAME_TEXT("t_ui_error_15");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(nNowCoin < nPrice)
    {
        std::string strMessage = GAME_TEXT("t_ui_error_17");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    NewRaidManager::getInstance()->requestBuyAttackCount(CC_CALLBACK_2(PopupRaidBuyAttackCount::callbackBuy, this), 2);
}

void PopupRaidBuyAttackCount::onClickAds(cocos2d::Ref* sender)
{
    int nAdsCnt = 0;
    
    auto objAdsCntSetting = NewRaidManager::getInstance()->getRaidSetting("max_ads_attack_count");
    if ( objAdsCntSetting != nullptr )
    {
        nAdsCnt = objAdsCntSetting->getValueToInt() - NewRaidManager::getInstance()->getAdsBuyCount();
    }
    
    if(nAdsCnt <= 0)
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
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupRaidBuyAttackCount::callBackResultAds, this));
    }
}

void PopupRaidBuyAttackCount::callBackResultAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        MafAnalyticsManager::LogEvent(kAdsSlotNewRaidCharging, "useridx", AccountManager::getInstance()->getUserIdx(), kRepeatTrue);
        
        //
        PopupLoading::show();
        NewRaidManager::getInstance()->requestBuyAttackCount(CC_CALLBACK_2(PopupRaidBuyAttackCount::callbackBuy, this), 1);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupRaidBuyAttackCount::callBackResultAds, this));
            popup->setLogParams(kAdsSlotNewRaidCharging);
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


#pragma mark -callback
void PopupRaidBuyAttackCount::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

void PopupRaidBuyAttackCount::callbackBuy(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    std::string strMessage = GAME_TEXT("t_ui_costume_skin_msg_4");
    
    if(bResult == false || nResult != 1)
    {
        switch (nResult)
        {
            case 0: strMessage = GAME_TEXT("t_ui_error_2"); break;
            case 3: strMessage = GAME_TEXT("t_ui_error_31"); break;
        }
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    uiContents();
    uiButton();
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
