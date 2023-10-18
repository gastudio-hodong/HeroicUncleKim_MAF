//
//  UILayerDefenseSummon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/11.
//

#include "UILayerDefenseSummon.h"

#include "Common/ConfigGameNormal.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/Raid/InfoRaidMileageLv.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Defense/Shop/Layer/UILayerDefensePass.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Defense/Shop/PopupDefenseSummon.h"
#include "GameUIPopup/Other/Defense/Shop/PopupDefenseSummonCartoon.h"
#include "GameUIPopup/Other/Defense/Shop/PopupDefenseDrawInfo.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

UILayerDefenseSummon* UILayerDefenseSummon::create(cocos2d::Size size)
{
    UILayerDefenseSummon* pRet = new(std::nothrow) UILayerDefenseSummon();
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

UILayerDefenseSummon::UILayerDefenseSummon(void):
_isCloseCondition(false)
,_lyTopUI(nullptr)
,_lyBottomUI(nullptr)
{
    _onTrensferShop.Clear();
    _onHide.Clear();
}

UILayerDefenseSummon::~UILayerDefenseSummon(void)
{
    _onTrensferShop.Clear();
    _onHide.Clear();
    DefenseManager::getInstance()->_onServerSummon.Clear(this);
}

bool UILayerDefenseSummon::init(cocos2d::Size size)
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
    
    DefenseManager::getInstance()->_onServerSummon += new MafFunction<void(bool, int)>(CC_CALLBACK_2(UILayerDefenseSummon::callbackSummonUIRefresh, this), this);
    
    return true;
}

#pragma mark - override
void UILayerDefenseSummon::refreshUI()
{
    uiTop();
    //uiBottom();
}

#pragma mark - get set
void UILayerDefenseSummon::setCloseCondition(bool value)
{
    _isCloseCondition = value;
}
#pragma mark - init
void UILayerDefenseSummon::initVar()
{
    
}

void UILayerDefenseSummon::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 620));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI, E_ZORER::UI + 1);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - 10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyBottomUI, E_ZORER::UI +1);
    
    //
    uiTop();
    uiBottom();
}

#pragma mark - init
void UILayerDefenseSummon::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    int nSummonPrice = 10;
    int nSummonPrice11 = 100;
    int nSummonPrice33 = 300;
    
    int nSummonAdsDailyMaxCount = 0;
    int nSummonAdsDrawCount = 0;
    
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_price_1"))
        nSummonPrice = setting->getValueToInt();
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_price_11"))
        nSummonPrice11 = setting->getValueToInt();
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_price_33"))
        nSummonPrice33 = setting->getValueToInt();
    
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_ads_max_count"))
        nSummonAdsDailyMaxCount = setting->getValueToInt();
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_ads_draw_count"))
        nSummonAdsDrawCount = setting->getValueToInt();
    
    
    auto sprBG = Sprite::create("Assets/ui/defense/defense_shop_bg_1_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 5);
    sprBG->setScale(3);
    sprBG->getTexture()->setTexParameters(texParams);
    _lyTopUI->addChild(sprBG);
    {
        auto anim = Animation::create();
        for(int i = 1; i <= 8; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/defense/defense_shop_bg_1_%d.png", i);
            
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        anim->setDelayPerUnit(0.1f);
        auto animate = Animate::create(anim);
        sprBG->runAction(RepeatForever::create(animate));
    }
    
    auto drawLvInfoBox = ui::ImageView::create("Assets/ui_common/base_box_1_21.png");
    drawLvInfoBox->setScale9Enabled(true);
    drawLvInfoBox->setCapInsets(Rect(13,15,35,100));
    drawLvInfoBox->setContentSize(Size(706, 134));
    drawLvInfoBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    drawLvInfoBox->setPosition(Vec2(_lyTopUI->getContentSize().width/2, 135));
    _lyTopUI->addChild(drawLvInfoBox);
    {
        int nLv = DefenseManager::getInstance()->getCurrentMileageLv();
        auto infoCurrentLv = DefenseManager::getInstance()->getInfoMileageLv(nLv);
        auto infoNextLv = DefenseManager::getInstance()->getInfoMileageLv(nLv + 1);
        int nTotalEXP = 0;
        int nCurrentPoint = DefenseManager::getInstance()->getMileage() - infoCurrentLv->getValue();
        int nPercent = 0;
        bool isMaxLv = false;
        
        auto lbTitle = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_defense_box_draw_lv", nLv), GAME_FONT, 24);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setDimensions(drawLvInfoBox->getContentSize().width - 15, 50);
        lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbTitle->setPosition(15, drawLvInfoBox->getContentSize().height - 25);
        lbTitle->setColor(COLOR_TITLE);
        drawLvInfoBox->addChild(lbTitle);
        
        auto lbMileageLvMsg = Label::createWithTTF(GAME_TEXT("t_ui_defense_box_draw_lv_content"), GAME_FONT, 24);
        lbMileageLvMsg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbMileageLvMsg->setDimensions(drawLvInfoBox->getContentSize().width - 20, lbMileageLvMsg->getContentSize().height);
        lbMileageLvMsg->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        lbMileageLvMsg->setPosition(15, drawLvInfoBox->getContentSize().height - 45);
        drawLvInfoBox->addChild(lbMileageLvMsg);
        
        
        if(infoNextLv != nullptr)
        {
            nTotalEXP = infoNextLv->getValue() - infoCurrentLv->getValue();
        }
        nPercent = (double)nCurrentPoint / nTotalEXP * 100;
        if ( nPercent > 100)
            nPercent = 100;
        
        if(nLv >= DefenseManager::getInstance()->getInfoMileageLvList().size())
        {
            nPercent = 100;
            isMaxLv = true;
        }
        
        auto sprMileageLvBg = Sprite::create("Assets/ui/raid/raid_shop/raid_shop_progressbar_02_1.png");
        sprMileageLvBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprMileageLvBg->setContentSize(Size(drawLvInfoBox->getContentSize().width - 30,24));
        sprMileageLvBg->setPosition(lbMileageLvMsg->getPosition() + Vec2::DOWN * (lbMileageLvMsg->getContentSize().height + 10));
        drawLvInfoBox->addChild(sprMileageLvBg);
            
        
        auto progressLv = ProgressTimer::create(Sprite::create("Assets/ui/raid/raid_shop/raid_shop_progressbar_01_2.png"));
        progressLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        progressLv->setPosition( sprMileageLvBg->getPosition() );
        progressLv->setMidpoint(Vec2(0, 0.5));
        progressLv->setBarChangeRate(Vec2(1,0));
        progressLv->setType(ProgressTimer::Type::BAR);
        progressLv->setPercentage(nPercent);
        drawLvInfoBox->addChild(progressLv);
        {
            std::string strTimer = MafUtils::format("%d/%d", nCurrentPoint, nTotalEXP);
            
            if(isMaxLv == true)
                strTimer = "MAX";
            
            auto labelTimer = Label::createWithTTF(strTimer, GAME_FONT, 20);
            labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelTimer->setPosition( Vec2(15, progressLv->getContentSize().height / 2) );
            progressLv->addChild(labelTimer);
        }
    }
    
    auto btnInfo = ui::Button::create("Assets/ui_common/base_text_2_9.png");
    btnInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btnInfo->setPosition(Vec2(5, drawLvInfoBox->getPositionY() + drawLvInfoBox->getContentSize().height + 5));
    btnInfo->setZoomScale(0.05f);
    btnInfo->addClickEventListener(CC_CALLBACK_1(UILayerDefenseSummon::onClickInfo, this));
    _lyTopUI->addChild(btnInfo);
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
        btnSummon->addClickEventListener(CC_CALLBACK_1(UILayerDefenseSummon::onClickSummon, this));
        _lyTopUI->addChild(btnSummon);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_defense_box_draw%d", drawCount)), GAME_FONT, 26);
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
                if(DefenseManager::getInstance()->getDrawTicket_1() > 0)
                {
                    item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_DRAW_TICKET_1);
                    price = DefenseManager::getInstance()->getDrawTicket_1();
                }
            }
            else if(drawCount == 11)
            {
                price = nSummonPrice11;
                if(DefenseManager::getInstance()->getDrawTicket_11() > 0)
                {
                    item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_DRAW_TICKET_11);
                    price = DefenseManager::getInstance()->getDrawTicket_11();
                    sprCurrencyIcon->setPosition(sprCurrencyIcon->getPosition() + Vec2::LEFT * 10);
                }
            }
            else if(drawCount == 33)
            {
                price = nSummonPrice33;
                if(DefenseManager::getInstance()->getDrawTicket_33() > 0)
                {
                    item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::DEFENSE_DRAW_TICKET_33);
                    price = DefenseManager::getInstance()->getDrawTicket_33();
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
    btnSummonAds->addClickEventListener(CC_CALLBACK_1(UILayerDefenseSummon::onClickSummonAds, this));
    _lyTopUI->addChild(btnSummonAds);
    {
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_raidshop_spiritbox_bt_draw_ad", nSummonAdsDrawCount), GAME_FONT, 26);
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
        
        auto lbCount = Label::createWithTTF(MafUtils::format("%d/%d", nSummonAdsDailyMaxCount - DefenseManager::getInstance()->getAdsCount(), nSummonAdsDailyMaxCount), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnSummonAds->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnSummonAds->addChild(lbCount);
        
        if(nSummonAdsDailyMaxCount - DefenseManager::getInstance()->getAdsCount() > 0 )
        {
            auto callFunc = CallFunc::create([&, btnSummonAds ,lbText, nSummonAdsDrawCount](){
                
                long time = DefenseManager::getInstance()->getAdsCooltime() - UtilsDate::getInstance()->getTime();
                
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
                    lbText->setString(GAME_TEXTFORMAT("t_ui_raidshop_spiritbox_bt_draw_ad", nSummonAdsDrawCount));
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
    
}
void UILayerDefenseSummon::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto lyPass = UILayerDefensePass::create(_lyBottomUI->getContentSize() - Size(0, 30));
    lyPass->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lyPass->setPosition(_lyBottomUI->getContentSize().width/2, 0);
    lyPass->_onRefresh += new MafFunction<void(void)>(CC_CALLBACK_0(UILayerDefenseSummon::refreshUI, this), this);
    _lyBottomUI->addChild(lyPass);
}

#pragma mark -utils
void UILayerDefenseSummon::onClickInfo(Ref* sender)
{
    auto popup = PopupDefenseDrawInfo::create();
    popup->show();
}

void UILayerDefenseSummon::onClickSummon(Ref* sender)
{
    auto node = (Node*)sender;
    
    int nPrice = 0;
    int count = 0;
    bool isTicket = false;
    
    if(node->getTag() == 1)
    {
        if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_price_1"))
            nPrice = setting->getValueToInt();
        
        if(DefenseManager::getInstance()->getDrawTicket_1() >= 1)
            isTicket = true;
        count = 1;
            
    }
    else if(node->getTag() == 11)
    {
        if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_price_11"))
            nPrice = setting->getValueToInt();
        
        if(DefenseManager::getInstance()->getDrawTicket_11() >= 1)
            isTicket = true;
        count = 11;
    }
    else if(node->getTag() == 33)
    {
        if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_price_33"))
            nPrice = setting->getValueToInt();
        
        if(DefenseManager::getInstance()->getDrawTicket_33() >= 1)
            isTicket = true;
        count = 33;
    }
    
    if(!isTicket && NewRaidManager::getInstance()->getDragonBall() < nPrice)
    {
//        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
//        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
//        popup->show();
        
        auto popup = PopupCurrencyDetail::create(E_ITEMS::NEW_RAID_DRAGON_BALL);
        popup->setCallbackTransferShop(CC_CALLBACK_0(UILayerDefenseSummon::callbackTransferShop, this));
        popup->show();
        return;
    }
    
    DefenseManager::getInstance()->requestSummon(count, false, true,  CC_CALLBACK_3(UILayerDefenseSummon::callbackSummon, this));
}

void UILayerDefenseSummon::onClickSummonAds(Ref* sender)
{
    auto node = (Node*)sender;
    
    int count = 0;
    
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_switch"))
    {
        if(!setting->getValueToBool())
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            return;
        }
    }
    
    if (auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_ads_max_count"))
        count = setting->getValueToInt();
    
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
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(UILayerDefenseSummon::callBackResultAds, this));
    }
    
    DefenseManager::getInstance()->requestSummon(count, true, true,  CC_CALLBACK_3(UILayerDefenseSummon::callbackSummon, this));
}

#pragma mark -Callback
void UILayerDefenseSummon::callBackResultAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafAnalyticsManager::LogEvent(kAdsSlotNewRaidSummon, "useridx", AccountManager::getInstance()->getUserIdx(), kRepeatTrue);
        
        //
        int count = 0;
        
        
        if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_gacha_ads_draw_count"))
            count = setting->getValueToInt();
        
//        PopupLoading::show();
//        WeaponSpiritManager::getInstance()->requestSpiritSummon(CC_CALLBACK_3(UILayerDefenseSummon::callbackSummon, this), count, true);
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(UILayerDefenseSummon::callBackResultAds, this));
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


void UILayerDefenseSummon::callbackSummon(bool bResult, int nResult, Vector<InfoItems*> listReward)
{
    if(nResult != 1)
    {
        if(nResult != 9)
        {
            std::string strMessageKey = "t_ui_error_30";
            
            if(nResult == 3)
                strMessageKey = "t_ui_error_31";
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    auto summonPopup = PopupDefenseSummon::create();
    summonPopup->setReward(listReward);
    summonPopup->_onTransferShop += new MafFunction<void(void)>(CC_CALLBACK_0(UILayerDefenseSummon::callbackTransferShop, this), this);
    
    bool isSpecial = false;
    
    for(auto items : listReward)
    {
        if((E_ITEMS)items->getIdx() == E_ITEMS::DEFENSE_WEAPON)
        {
            if(auto waepon = DefenseManager::getInstance()->getWeaponData(items->getTypeParams()))
            {
                if(waepon->getGrade() == DefenseManager::getInstance()->getLastGrade(E_DF_Trench_SettingType::WEAPON))
                {
                    isSpecial = true;
                    break;
                }
            }
        }
        if((E_ITEMS)items->getIdx() == E_ITEMS::DEFENSE_CHARACTER)
        {
            if(auto character = DefenseManager::getInstance()->getCharacterData(items->getTypeParams()))
            {
                if(character->getGrade() == DefenseManager::getInstance()->getLastGrade(E_DF_Trench_SettingType::CHARACTER))
                {
                    isSpecial = true;
                    break;
                }
            }
        }
    }
    auto cartoonPopup = PopupDefenseSummonCartoon::create(isSpecial);
    cartoonPopup->setNextPopup(summonPopup);
    cartoonPopup->show();
    
    refreshUI();
}

void UILayerDefenseSummon::callbackSummonUIRefresh(bool bResult, int nResult)
{
    refreshUI();
}
void UILayerDefenseSummon::callbackTransferShop()
{
    _onTrensferShop.Invoke();
}
