//
//  UILayerSpiritSummon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/30.
//

#include "UILayerSpiritSummon.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/NewRaid/PopupSpiritSummonDrawInfo.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/NewRaid/PopupSpiritSummon.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

UILayerSpiritSummon* UILayerSpiritSummon::create(cocos2d::Size size)
{
    UILayerSpiritSummon* pRet = new(std::nothrow) UILayerSpiritSummon();
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

UILayerSpiritSummon::UILayerSpiritSummon(void):
_isCloseCondition(false)
,_lyTopUI(nullptr)
,_lyMiddleUI(nullptr)
,_lyBottomUI(nullptr)
,_callbackSetTab(nullptr)
{
}

UILayerSpiritSummon::~UILayerSpiritSummon(void)
{
}

bool UILayerSpiritSummon::init(cocos2d::Size size)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }
    setContentSize(size);
    
    //
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - override
void UILayerSpiritSummon::refreshUI()
{
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark - get set
void UILayerSpiritSummon::setCloseCondition(bool value)
{
    _isCloseCondition = value;
}
void UILayerSpiritSummon::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
void UILayerSpiritSummon::setCallbackTransferShop(const std::function<void(PopupRaidShopDefault::E_TYPE)>& callback)
{
    _callbackSetTab = callback;
}

#pragma mark - init
void UILayerSpiritSummon::initVar()
{
    
}

void UILayerSpiritSummon::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 90));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI, E_ZORER::UI + 1);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(getContentSize().width - 10, 530));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyMiddleUI, E_ZORER::UI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height - 10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    addChild(_lyBottomUI, E_ZORER::UI +1);
    
    //
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark - init
void UILayerSpiritSummon::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_6.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setContentSize(Size(_lyTopUI->getContentSize().width, 70));
    sprBox->setPosition(_lyTopUI->getContentSize()/2);
    _lyTopUI->addChild(sprBox);
    
    auto sprIcon = Sprite::create("Assets/icon/icon_raid_ticket.png");
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIcon->setPosition(30, sprBox->getContentSize().height/2);
    sprBox->addChild(sprIcon);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_purchase_double_ticket_mag4"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(sprBox->getContentSize().width - 30 - sprIcon->getContentSize().width - 210, sprBox->getContentSize().height - 5);
    lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width);
    sprBox->addChild(lbText);
    
    auto btnTicketBuy = ui::Button::create("Assets/ui_common/btn_2_4_on.png");
    btnTicketBuy->setScale9Enabled(true);
    btnTicketBuy->setContentSize(Size(195, 56));
    btnTicketBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnTicketBuy->setPosition(Vec2(sprBox->getContentSize().width - 10, sprBox->getContentSize().height/2));
    btnTicketBuy->setZoomScale(0.05f);
    btnTicketBuy->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickTicketBuy, this));
    btnTicketBuy->setTitleText(GAME_TEXT("t_ui_costume_skin_msg_2"));
    btnTicketBuy->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnTicketBuy->getTitleLabel()->setDimensions(btnTicketBuy->getContentSize().width, btnTicketBuy->getContentSize().height);
    btnTicketBuy->setTitleFontSize(24);
    btnTicketBuy->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(btnTicketBuy);
    
    if(NewRaidManager::getInstance()->isTicket())
    {
        btnTicketBuy->setTitleText(GAME_TEXT("t_ui_raid_tickt_noti"));
    }
}

void UILayerSpiritSummon::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    int nSummonPrice = 10;
    int nSummonPrice11 = 100;
    int nSummonPrice33 = 300;
    
    int nSummonAdsMaxCount = 0;
    int nSummonAdsCount = 0;
    
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_1"))
        nSummonPrice = setting->getValueToInt();
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_11"))
        nSummonPrice11 = setting->getValueToInt();
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_33"))
        nSummonPrice33 = setting->getValueToInt();
    
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_ads_max_count"))
        nSummonAdsMaxCount = setting->getValueToInt();
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_ads_count"))
        nSummonAdsCount = setting->getValueToInt();
    
    auto sprBG = Sprite::create("Assets/ui/raid/raid_shop/raid_shop_bg_1_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(_lyMiddleUI->getContentSize()/2);
    sprBG->setScale(_lyMiddleUI->getContentSize().width / sprBG->getContentSize().width, _lyMiddleUI->getContentSize().height / sprBG->getContentSize().height);
    sprBG->getTexture()->setTexParameters(texParams);
    _lyMiddleUI->addChild(sprBG);
    {
        auto particle = ParticleSystemQuad::create("Assets/Particle/particle_spirit_summon_bg_1.plist");
        particle->setScale(1);
        particle->setDuration(-1);
        particle->setPosition(Vec2(sprBG->getContentSize()/2) + Vec2::UP * 40);
        sprBG->addChild(particle);
        
        auto sprFX = ui::Scale9Sprite::create("Assets/ui/raid/raid_shop/raid_shop_bg_1_2.png");
        sprFX->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprFX->setContentSize(sprBG->getContentSize());
        sprFX->setPosition(sprBG->getContentSize()/2);
        sprBG->addChild(sprFX);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto seq = Sequence::create(fadeOut,fadeIn, NULL);
        sprFX->runAction(RepeatForever::create(seq));
    }
    
    auto btnSpiritList = ui::Button::create("Assets/ui/raid/raid_shop/raid_spirit_shortcuts.png");
    btnSpiritList->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnSpiritList->setPosition(Vec2(0, _lyMiddleUI->getContentSize().height - 10));
    btnSpiritList->setZoomScale(0.05f);
    btnSpiritList->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickSpiritList, this));
    _lyMiddleUI->addChild(btnSpiritList);
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(btnSpiritList->getContentSize().width - 15, btnSpiritList->getContentSize().height - 15);
        btnSpiritList->addChild(uiReddot);
        uiReddot->setVisible(false);
        
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
        uiReddot->runAction(RepeatForever::create(seq));
    }
    
    auto btnInfo = ui::Button::create("Assets/ui_common/base_text_2_4.png");
    btnInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btnInfo->setPosition(Vec2(5, 140));
    btnInfo->setZoomScale(0.05f);
    btnInfo->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickInfo, this));
    _lyMiddleUI->addChild(btnInfo);
    {
        auto sprIcon = Sprite::create("Assets/ui/pet/btn_pet_info.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(Vec2(5, btnInfo->getContentSize().height/2));
        btnInfo->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_title"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(btnInfo->getContentSize().width - 5 - sprIcon->getContentSize().width, btnInfo->getContentSize().height - 5);
        lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width);
        btnInfo->addChild(lbText);
    }
    
    std::vector<int> btnList = {1, 11, 33};
    Vec2 btnLocation = Vec2(10, 20);
    for(int i = 0; i < btnList.size(); ++i)
    {
        int drawCount = btnList[i];
        
        auto btnSummon = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png");
        btnSummon->setScale9Enabled(true);
        btnSummon->setContentSize(Size(168, 104));
        btnSummon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        btnSummon->setPosition(btnLocation);
        btnSummon->setZoomScale(0.05f);
        btnSummon->setTag(drawCount);
        btnSummon->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickSummon, this));
        _lyMiddleUI->addChild(btnSummon);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_raidshop_spiritbox_bt_draw%d", drawCount)), GAME_FONT, 26);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnSummon->getContentSize().width/2, btnSummon->getContentSize().height - btnSummon->getContentSize().height/4);
            lbText->setDimensions(btnSummon->getContentSize().width - 10, btnSummon->getContentSize().height/2);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnSummon->addChild(lbText);
            
            auto sprCurrencyIcon = Sprite::create();
            sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCurrencyIcon->setPosition(btnSummon->getContentSize().width/2.6f, btnSummon->getContentSize().height/4);
            btnSummon->addChild(sprCurrencyIcon);
            
            int price = 0;
            auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL);
            if(drawCount == 1)
            {
                price = nSummonPrice;
                if(WeaponSpiritManager::getInstance()->getSummonTicket_1() > 0)
                {
                    item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_DRAW_TICKET_1);
                    price = WeaponSpiritManager::getInstance()->getSummonTicket_1();
                }
            }
            else if(drawCount == 11)
            {
                price = nSummonPrice11;
                if(WeaponSpiritManager::getInstance()->getSummonTicket_11() > 0)
                {
                    item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_DRAW_TICKET_11);
                    price = WeaponSpiritManager::getInstance()->getSummonTicket_11();
                    sprCurrencyIcon->setPosition(sprCurrencyIcon->getPosition() + Vec2::LEFT * 10);
                }
            }
            else if(drawCount == 33)
            {
                price = nSummonPrice33;
                if(WeaponSpiritManager::getInstance()->getSummonTicket_33() > 0)
                {
                    item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_DRAW_TICKET_33);
                    price = WeaponSpiritManager::getInstance()->getSummonTicket_33();
                    sprCurrencyIcon->setPosition(sprCurrencyIcon->getPosition() + Vec2::LEFT * 10);
                }
            }
            sprCurrencyIcon->setTexture(item->getPath());
            
            
            auto lbCount = Label::createWithTTF(std::to_string(price), GAME_FONT, 26);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
            lbCount->setDimensions(100, btnSummon->getContentSize().height/2);
            lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbCount->setOverflow(Label::Overflow::SHRINK);
            btnSummon->addChild(lbCount);
        }
        btnLocation += Vec2::RIGHT * (5 + btnSummon->getContentSize().width);
    }
    
    auto btnSummonAds = ui::Button::create("Assets/ui_common/btn_1_7_on.png");
    btnSummonAds->setScale9Enabled(true);
    btnSummonAds->setContentSize(Size(168, 104));
    btnSummonAds->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btnSummonAds->setPosition(btnLocation);
    btnSummonAds->setZoomScale(0.05f);
    btnSummonAds->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickSummonAds, this));
    _lyMiddleUI->addChild(btnSummonAds);
    {
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_raidshop_spiritbox_bt_draw_ad", nSummonAdsCount), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnSummonAds->getContentSize().width/2, btnSummonAds->getContentSize().height - btnSummonAds->getContentSize().height/4);
        lbText->setDimensions(btnSummonAds->getContentSize().width - 10, btnSummonAds->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnSummonAds->addChild(lbText);
        
        
        auto sprCurrencyIcon = Sprite::create("Assets/icon/icon_ad_1_1.png");
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCurrencyIcon->setPosition(btnSummonAds->getContentSize().width/2.6f, btnSummonAds->getContentSize().height/4);
        btnSummonAds->addChild(sprCurrencyIcon);
        
        auto lbCount = Label::createWithTTF(MafUtils::format("%d/%d", nSummonAdsMaxCount - WeaponSpiritManager::getInstance()->getAdsSummonCount(), nSummonAdsMaxCount), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnSummonAds->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnSummonAds->addChild(lbCount);
        
        if(nSummonAdsMaxCount - WeaponSpiritManager::getInstance()->getAdsSummonCount() > 0 )
        {
            auto callFunc = CallFunc::create([&, btnSummonAds ,lbText](){
                
                auto objAdsSummonCountSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_ads_count");
                
                int nSummonAdsCount = 0;
                
                if(objAdsSummonCountSetting != nullptr)
                    nSummonAdsCount = objAdsSummonCountSetting->getValueToInt();
                
                long time = WeaponSpiritManager::getInstance()->getAdsSummonCooldown() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
                
                if(time > 0)
                {
                    int min = (time % 3600) / 60;
                    int second = time % 3600 % 60;
                        
                    if(min < 0)
                        min = 0;
                    if(second < 0)
                        second = 0;
                    
                    lbText->setString(MafUtils::format("%02d:%02d", min, second));
                    btnSummonAds->setEnabled(false);
                }
                else
                {
                    lbText->setString(GAME_TEXTFORMAT("t_ui_raidshop_spiritbox_bt_draw_ad", nSummonAdsCount));
                    btnSummonAds->setEnabled(true);
                }
                });
            auto delayTime = DelayTime::create(1.f);
            auto seq = Sequence::create(callFunc, delayTime, nullptr);
            btnSummonAds->runAction(RepeatForever::create(seq));
        }
        else
        {
            btnSummonAds->setEnabled(false);
        }
    }
    
    std::string strAdsDragonBallPath = "Assets/ui_common/ad_time_remaining1_7.png";
        
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_orb_ads_max_count");
    int nAdsMax = 0;
    int nAdsCurrent = NewRaidManager::getInstance()->getAdsDragonBallCount();
    
    if(objSetting != nullptr)
    {
        nAdsMax = objSetting->getValueToInt();
    }
    
    if(nAdsCurrent >= nAdsMax)
    {
        strAdsDragonBallPath = "Assets/ui_common/ad_time_remaining1_8.png";
    }
        
    auto btnAdsDragonBall = ui::Button::create(strAdsDragonBallPath);
    btnAdsDragonBall->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnAdsDragonBall->setPosition(Vec2(_lyMiddleUI->getContentSize().width, btnSummonAds->getPosition().y + btnSummonAds->getContentSize().height + 20));
    btnAdsDragonBall->setZoomScale(0.05f);
    btnAdsDragonBall->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickAdsDragonBall, this));
    _lyMiddleUI->addChild(btnAdsDragonBall);
    
}

void UILayerSpiritSummon::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    int nLv = WeaponSpiritManager::getInstance()->getCurrentMileageLv();
    auto infoCurrentLv = WeaponSpiritManager::getInstance()->getInfoMileageLv(nLv);
    auto infoNextLv = WeaponSpiritManager::getInstance()->getInfoMileageLv(nLv + 1);
    int nTotalEXP = 0;
    int nCurrentPoint = WeaponSpiritManager::getInstance()->getCurrentMileageLvPoint();
    int nPercent = 0;
    bool isMaxLv = false;
    
    auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/list_bg_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setContentSize(Size(_lyBottomUI->getContentSize().width - 10, 170));
    sprBG->setPosition(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height - 20);
    _lyBottomUI->addChild(sprBG);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_spirit_draw_lv_1") + " " + std::to_string(nLv), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setDimensions(sprBG->getContentSize().width - 15, 50);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbTitle->setPosition(15, sprBG->getContentSize().height - 25);
    lbTitle->setColor(COLOR_TITLE);
    sprBG->addChild(lbTitle);
    
    auto lbMileageLvMsg = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_msg"), GAME_FONT, 24);
    lbMileageLvMsg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMileageLvMsg->setDimensions(sprBG->getContentSize().width - 20, lbMileageLvMsg->getContentSize().height);
    lbMileageLvMsg->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
    lbMileageLvMsg->setPosition(15, sprBG->getContentSize().height - 55);
    sprBG->addChild(lbMileageLvMsg);
    
    
    if(infoNextLv != nullptr)
    {
        nTotalEXP = infoNextLv->getValue() - infoCurrentLv->getValue();
    }
    nPercent = (double)nCurrentPoint / nTotalEXP * 100;
    if ( nPercent > 100)
        nPercent = 100;
    
    if(nLv >= WeaponSpiritManager::getInstance()->getListMileageLv().size())
    {
        nPercent = 100;
        isMaxLv = true;
    }
    
    auto sprMileageLvBg = Sprite::create("Assets/ui/raid/raid_shop/raid_shop_progressbar_02_1.png");
    sprMileageLvBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprMileageLvBg->setContentSize(Size(sprBG->getContentSize().width - 30,24));
    sprMileageLvBg->setPosition(lbMileageLvMsg->getPosition() + Vec2::DOWN * (lbMileageLvMsg->getContentSize().height + 10));
    sprBG->addChild(sprMileageLvBg);
        
    
    auto progressLv = ProgressTimer::create(Sprite::create("Assets/ui/raid/raid_shop/raid_shop_progressbar_01_2.png"));
    progressLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    progressLv->setPosition( sprMileageLvBg->getPosition() );
    progressLv->setMidpoint(Vec2(0, 0.5));
    progressLv->setBarChangeRate(Vec2(1,0));
    progressLv->setType(ProgressTimer::Type::BAR);
    progressLv->setPercentage(nPercent);
    sprBG->addChild(progressLv);
    {
        std::string strTimer = MafUtils::format("%d/%d", nCurrentPoint, nTotalEXP);
        
        if(isMaxLv == true)
            strTimer = "MAX";
        
        auto labelTimer = Label::createWithTTF(strTimer, GAME_FONT, 20);
        labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTimer->setPosition( Vec2(15, progressLv->getContentSize().height / 2) );
        progressLv->addChild(labelTimer);
    }
    
    
    nLv = WeaponSpiritManager::getInstance()->getCurrentMileageRewardIdx();
    auto infoReward = WeaponSpiritManager::getInstance()->getInfoMileageReward(nLv);
    nTotalEXP = 0;
    nPercent = 0;
    nCurrentPoint = WeaponSpiritManager::getInstance()->getCurrentMileageRewardPoint();
    isMaxLv = false;
    
    if(infoReward != nullptr)
    {
        nTotalEXP = infoReward->getValue();
    }
    nPercent = (double)nCurrentPoint / nTotalEXP * 100;
    if ( nPercent > 100)
        nPercent = 100;
    
    if(nLv > WeaponSpiritManager::getInstance()->getListMileageReward().size())
    {
        nPercent = 100;
        isMaxLv = true;
    }
    
    auto sprRewardBg = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprRewardBg->setContentSize(Size(_lyBottomUI->getContentSize().width - 10, 136));
    sprRewardBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprRewardBg->setPosition(sprBG->getPosition() + Vec2::DOWN * (sprBG->getContentSize().height + 10));
    _lyBottomUI->addChild(sprRewardBg);
    
    auto sprRewardBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprRewardBox->setContentSize(Size(82,82));
    sprRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRewardBox->setPosition(15, sprRewardBg->getContentSize().height/2);
    sprRewardBg->addChild(sprRewardBox);
    {
        if(infoReward != nullptr && infoReward->getListReward().size() > 0)
        {
            auto objItems = infoReward->getListReward().at(0);
            if(objItems != nullptr)
            {
                auto sprIcon = Sprite::create(objItems->getPath());
                sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprIcon->setPosition(sprRewardBox->getContentSize()/2);
                sprRewardBox->addChild(sprIcon);
                
                auto labelCount = Label::createWithTTF(objItems->getCount(), GAME_FONT, 16);
                labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                labelCount->setPosition(sprRewardBox->getContentSize().width / 2, 0);
                sprRewardBox->addChild(labelCount);
            }
        }
        else
        {
            auto lbText = Label::createWithTTF("DONE", GAME_FONT, 16);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(sprRewardBox->getContentSize()/2);
            lbText->setDimensions(sprRewardBox->getContentSize().width, sprRewardBox->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            sprRewardBox->addChild(lbText);
        }
    }
    
    auto lbMileageRewardMsg = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_draw_mileage_msg"), GAME_FONT, 24);
    lbMileageRewardMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbMileageRewardMsg->setDimensions(sprRewardBg->getContentSize().width - 230, lbMileageRewardMsg->getContentSize().height);
    lbMileageRewardMsg->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbMileageRewardMsg->setPosition(sprRewardBox->getPosition() + Vec2::RIGHT * (sprRewardBox->getContentSize().width + 15) + Vec2::UP * 20);
    lbMileageRewardMsg->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprRewardBg->addChild(lbMileageRewardMsg);
    
    
    auto sprMileageRewardBg = ui::Scale9Sprite::create("Assets/ui/raid/raid_shop/raid_shop_progressbar_02_1.png");
    sprMileageRewardBg->setContentSize(Size(420, 24));
    sprMileageRewardBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprMileageRewardBg->setPosition(lbMileageRewardMsg->getPosition() + Vec2::DOWN * (lbMileageRewardMsg->getContentSize().height - 5));
    sprRewardBg->addChild(sprMileageRewardBg);
    
    
    auto progressReward = ProgressTimer::create(Sprite::create("Assets/ui/raid/raid_shop/raid_shop_progressbar_02_2.png"));
    progressReward->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    progressReward->setPosition( sprMileageRewardBg->getPosition() );
    progressReward->setMidpoint(Vec2(0, 0.5));
    progressReward->setBarChangeRate(Vec2(1,0));
    progressReward->setType(ProgressTimer::Type::BAR);
    progressReward->setPercentage(nPercent);
    sprRewardBg->addChild(progressReward);
    {
        std::string strTimer = MafUtils::format("%d/%d", nCurrentPoint, nTotalEXP);
        
        if(isMaxLv == true)
            strTimer = "MAX";
        
        auto labelTimer = Label::createWithTTF(strTimer, GAME_FONT, 20);
        labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTimer->setPosition( Vec2(15, progressReward->getContentSize().height / 2) );
        progressReward->addChild(labelTimer);
    }
    
    std::string strRewardBtnPath = "Assets/ui/mission/btn_acheive_2.png";
    if(!isMaxLv && nPercent >= 100)
        strRewardBtnPath = "Assets/ui/mission/btn_acheive_1.png";
    
    auto btnMileageReward = ui::Button::create(strRewardBtnPath);
    btnMileageReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMileageReward->setPosition(Vec2(sprRewardBg->getContentSize().width - 15, sprRewardBg->getContentSize().height/2));
    btnMileageReward->setZoomScale(0.05f);
    btnMileageReward->addClickEventListener(CC_CALLBACK_1(UILayerSpiritSummon::onClickMileageReward, this));
    sprRewardBg->addChild(btnMileageReward);
}

#pragma mark -utils
void UILayerSpiritSummon::onClickTicketBuy(cocos2d::Ref* sender)
{
    if(NewRaidManager::getInstance()->isTicket())
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_new_raid_ticket_pop_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(70000000);
    if ( objProduct == nullptr )
    {
        return;
    }
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(70000000, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerSpiritSummon::callbackPurchase, this));
            });
            popup->show();
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            long time = NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            
            std::string strMsg = GAME_TEXT("t_ui_new_raid_ticket_pop_1") + "\n" + GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min);
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
            popup->addButton(GAME_TEXT("t_ui_costume_skin_msg_2"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerSpiritSummon::callbackPurchase, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
                
        }
    });
}


void UILayerSpiritSummon::onClickInfo(Ref* sender)
{
    auto popup = PopupSpiritSummonDrawInfo::create();
    
    popup->show();
}

void UILayerSpiritSummon::onClickSummon(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto objSummonSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_1");
    auto obj11SummonSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_11");
    auto obj33SummonSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_33");
    
    int nPrice = 0;
    bool isTicket = false;
    
    if(node->getTag() == 1)
    {
        if(objSummonSetting != nullptr )
            nPrice = objSummonSetting->getValueToInt();
        
        if(WeaponSpiritManager::getInstance()->getSummonTicket_1() >= 1)
            isTicket = true;
            
    }
    else if(node->getTag() == 11)
    {
        if(obj11SummonSetting != nullptr)
            nPrice = obj11SummonSetting->getValueToInt();
        
        if(WeaponSpiritManager::getInstance()->getSummonTicket_11() >= 1)
            isTicket = true;
    }
    else if(node->getTag() == 33)
    {
        if(obj11SummonSetting != nullptr)
            nPrice = obj33SummonSetting->getValueToInt();
        
        if(WeaponSpiritManager::getInstance()->getSummonTicket_33() >= 1)
            isTicket = true;
    }
    
    if(!isTicket && NewRaidManager::getInstance()->getDragonBall() < nPrice)
    {
//        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
//        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
//        popup->show();
        
        auto popup = PopupCurrencyDetail::create(E_ITEMS::NEW_RAID_DRAGON_BALL);
        popup->setCallbackTransferShop(CC_CALLBACK_0(UILayerSpiritSummon::callbackTransferShop, this));
        popup->show();
        return;
    }
    
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        WeaponSpiritManager::getInstance()->requestSpiritSummon(CC_CALLBACK_3(UILayerSpiritSummon::callbackSummon, this), node->getTag());
    }
}

void UILayerSpiritSummon::onClickSummonAds(Ref* sender)
{
    auto node = (Node*)sender;
    
    if ( WeaponSpiritManager::getInstance()->isServerDispatch() )
        return;
    
    int count = 0;
    
    auto objAdsSummonMaxCountSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_ads_max_count");
    if ( objAdsSummonMaxCountSetting != nullptr )
        count = objAdsSummonMaxCountSetting->getValueToInt() - WeaponSpiritManager::getInstance()->getAdsSummonCount();
    
    if ( count <= 0 )
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
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UILayerSpiritSummon::callBackResultAds, this));
    }
}

void UILayerSpiritSummon::onClickMileageReward(Ref *sender)
{
    int nLv = WeaponSpiritManager::getInstance()->getCurrentMileageRewardIdx();
    auto infoReward = WeaponSpiritManager::getInstance()->getInfoMileageReward(nLv);
    int nTotalEXP = 0;
    int nPercent = 0;
    int nCurrentPoint = WeaponSpiritManager::getInstance()->getCurrentMileageRewardPoint();
    
    if(infoReward != nullptr)
    {
        nTotalEXP = infoReward->getValue();
    }
    nPercent = (double)nCurrentPoint / nTotalEXP * 100;
    if ( nPercent > 100)
        nPercent = 100;
    
    if(nLv > WeaponSpiritManager::getInstance()->getListMileageReward().size() || nPercent < 100)
    {
        return;
    }
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        WeaponSpiritManager::getInstance()->requestMileageReward(CC_CALLBACK_2(UILayerSpiritSummon::callbackMileageReward, this));
    }
}
void UILayerSpiritSummon::onClickSpiritList(Ref *sender)
{
    if(_isCloseCondition)
    {
        if(_callbackHide != nullptr)
        {
            _callbackHide();
            return;
        }
    }
    
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        auto popup = PopupListWeaponSpirit::create();
        popup->setCloseCondition(true);
        popup->show();
    }
}

void UILayerSpiritSummon::onClickAdsDragonBall(Ref *sender)
{
    if ( WeaponSpiritManager::getInstance()->isServerDispatch() )
        return;
    
    int count = 0;
    
    auto objAdsMaxCountSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_orb_ads_max_count");
    if ( objAdsMaxCountSetting != nullptr )
        count = objAdsMaxCountSetting->getValueToInt() - NewRaidManager::getInstance()->getAdsDragonBallCount();
    
    if ( count <= 0 )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_15");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAdsDragonBall(ADS_RESULT::SUCCESS);
    }
    else
    {
        int count = 10;
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_orb_ads_count");
        if(objSetting != nullptr)
            count = objSetting->getValueToInt();
        
        auto popup = PopupAdsWatch::create(E_ADS_WATCH::ITEM_NEW_RAID_DRAGON_BALL, count);
        popup->setCallbackResult([=](){
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UILayerSpiritSummon::callbackAdsDragonBall, this));
        });
        popup->show();
    }
    
    
}

#pragma mark -Callback
void UILayerSpiritSummon::callBackResultAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafAnalyticsManager::LogEvent(kAdsSlotNewRaidSummon, "useridx", AccountManager::getInstance()->getUserIdx(), kRepeatTrue);
        
        //
        int count = 0;
        
        auto objAdsSummonCountSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_ads_count");
        if ( objAdsSummonCountSetting != nullptr )
            count = objAdsSummonCountSetting->getValueToInt();
        
        PopupLoading::show();
        WeaponSpiritManager::getInstance()->requestSpiritSummon(CC_CALLBACK_3(UILayerSpiritSummon::callbackSummon, this), count, true);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(UILayerSpiritSummon::callBackResultAds, this));
            popup->setLogParams(kAnalEventAdsLoad);
            popup->setLogValue(kAdsSlotNewRaidSummon);
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

void UILayerSpiritSummon::callbackAdsDragonBall(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafAnalyticsManager::LogEvent(kAdsSlotNewRaidDragonBall, "useridx", AccountManager::getInstance()->getUserIdx(), kRepeatTrue);
        
        //
        PopupLoading::show();
        NewRaidManager::getInstance()->requestBuyDragonBall(CC_CALLBACK_2(UILayerSpiritSummon::callbackAdsDragonBallBuy, this), true);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(UILayerSpiritSummon::callbackAdsDragonBall, this));
            popup->setLogParams(kAnalEventAdsLoad);
            popup->setLogValue(kAdsSlotNewRaidDragonBall);
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


void UILayerSpiritSummon::callbackSummon(bool bResult, int nResult, Vector<InfoItems*> listReward)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        else if(nResult == 9)
        {
            strMessageKey = "t_ui_error_52";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
    auto popup = PopupSpiritSummon::create();
    popup->setReward(listReward);
    popup->setCallback(CC_CALLBACK_0(UILayerSpiritSummon::refreshUI, this));
    popup->setCallbackTransferShop(CC_CALLBACK_0(UILayerSpiritSummon::callbackTransferShop, this));
    popup->show();
    
    refreshUI();
}

void UILayerSpiritSummon::callbackAdsDragonBallBuy(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_15";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    refreshUI();
}

void UILayerSpiritSummon::callbackTransferShop()
{
    if(_callbackSetTab != nullptr)
        _callbackSetTab(PopupRaidShopDefault::E_TYPE::PURCHASE);
}

#pragma mark - Purchase
void UILayerSpiritSummon::callbackProductOk(int nIdx)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerSpiritSummon::callbackPurchase, this));
}

void UILayerSpiritSummon::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo([=](bool){
            PopupLoading::hide();
            RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
        });
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        PopupLoading::show();
        NewRaidManager::getInstance()->requestInfo([=](bool, int){
            PopupLoading::hide();
            
            RefreshManager::getInstance()->refreshUI(E_REFRESH::RAID_ALL);
        });
        
        // alert
        std::string strText = GAME_TEXT("t_ui_cash_msg_1");
        if(nIdx == 70000000)
            strText = GAME_TEXT("t_ui_new_raid_ticket_pop_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 #이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
}

void UILayerSpiritSummon::callbackMileageReward(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        else if(nResult == 9)
        {
            strMessageKey = "t_ui_error_52";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
    
    refreshUI();
}
