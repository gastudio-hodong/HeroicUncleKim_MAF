//
//  UILayerRaidTrading.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/02/03.
//

#include "UILayerRaidTrading.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/NewRaid/PopupSpiritSummon.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupListWeaponSpirit.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/NewRaid/PopupSpiritSummonDrawInfo.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

UILayerRaidTrading* UILayerRaidTrading::create(cocos2d::Size size)
{
    UILayerRaidTrading* pRet = new(std::nothrow) UILayerRaidTrading();
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

UILayerRaidTrading::UILayerRaidTrading(void):
_isCloseCondition(false)
,_lyTopUI(nullptr)
,_lyMiddleUI(nullptr)
,_lyBottomUI(nullptr)
,_table(nullptr)
{
    _listInfoItems.clear();
}

UILayerRaidTrading::~UILayerRaidTrading(void)
{
    _listInfoItems.clear();
}

bool UILayerRaidTrading::init(cocos2d::Size size)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }
    setContentSize(size);
    
    //
    _listInfoItems = NewRaidManager::getInstance()->getListActivateTradingMarketReward();
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayerRaidTrading::onEnter()
{
    LayerColor::onEnter();
}

#pragma mark - table
void UILayerRaidTrading::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerRaidTrading::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 250);
}

extension::TableViewCell* UILayerRaidTrading::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    // obj
    ssize_t nIdxStart = idx * 4;
    ssize_t nIdxEnd = nIdxStart + 4;
    
    if ( nIdxEnd > _listInfoItems.size() )
    {
        nIdxEnd = _listInfoItems.size();
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdx = (int)nIdxStart + i;
        
        auto objTradingItem = _listInfoItems.at(nIdx);
        if ( objTradingItem == nullptr )
        {
            continue;
        }
        
        double nPosX = 38 + 159 * i + 10 * i;
        double nPosY = cell->getContentSize().height;
        
        bool isImpossibleBuy = false;
        
        std::string strBoxPath = "Assets/ui/event/tajir_shop_box_1_2.png";
        
        if(objTradingItem->getMaxBuyCount() - objTradingItem->getBuyCount() <= 0)
        {
            isImpossibleBuy = true;
        }
        auto objItem = objTradingItem->getItem();
        auto objPrice = objTradingItem->getPrice();
        if(objItem == nullptr || objPrice == nullptr)
            continue;
        
        if(objItem->getType().compare("COSTUME") == 0)
        {
            if(CostumeSkinManager::getInstance()->isCostumeSkinBuy(objItem->getTypeParams()))
                isImpossibleBuy = true;
            
        }
        else if(objItem->getType().compare("PET_SKIN") == 0)
        {
            E_PET ePet;
            if(objItem->getIdx() == 14000)
                ePet = E_PET::CHICK_1;
            else if(objItem->getIdx() == 14001)
                ePet = E_PET::HAMSTER_2;
            else if(objItem->getIdx() == 14002)
                ePet = E_PET::DRAGON_3;
            else if(objItem->getIdx() == 14003)
                ePet = E_PET::PRINCESS_4;
            else if(objItem->getIdx() == 14004)
                ePet = E_PET::PURIRING_5;
            else if(objItem->getIdx() == 14005)
                ePet = E_PET::HONG_6;
            else if(objItem->getIdx() == 14006)
                ePet = E_PET::MOLE_7;
            else if(objItem->getIdx() == 14007)
                ePet = E_PET::HANDSOME_8;
            
            if(PetNewManager::getInstance()->isPetSkinBuy(ePet, objItem->getTypeParams()))
            {
                isImpossibleBuy = true;
            }
        }
        else if(objItem->getType().compare("STONE") == 0)
        {
            if(objItem->getTypeDetail().compare("ATK") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK) >= 4300)
                    isImpossibleBuy = true;
            }
            else if(objItem->getTypeDetail().compare("GOLD") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD) >= 4300)
                    isImpossibleBuy = true;
            }
            else if(objItem->getTypeDetail().compare("KEY") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY) >= 4300)
                    isImpossibleBuy = true;
            }
            else if(objItem->getTypeDetail().compare("FORCE") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::FORCE_ATTACK) >= 4300)
                    isImpossibleBuy = true;
            }
        }
        
        if(isImpossibleBuy)
            strBoxPath = "Assets/ui/event/tajir_shop_box_1_3.png";
        
        auto itemBG = MafNode::MafMenuItemSprite::create(Sprite::create(strBoxPath), nullptr, CC_CALLBACK_1(UILayerRaidTrading::onClickBuyItem, this));
        itemBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        itemBG->setPosition(nPosX, nPosY);
        itemBG->setTag(objTradingItem->getIdx());
        menu->addChild(itemBG);
        {
            auto sprIcon = Sprite::create(objItem->getPath());
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            sprIcon->setPosition(itemBG->getContentSize().width/2, 45);
            sprIcon->getTexture()->setTexParameters(texParams);
            if(objItem->getType().compare("ITEM") == 0)
            {
                if((E_ITEMS)objItem->getIdx() == E_ITEMS::SPIRIT_DRAW_TICKET_1 ||
                   (E_ITEMS)objItem->getIdx() == E_ITEMS::SPIRIT_DRAW_TICKET_11 ||
                   (E_ITEMS)objItem->getIdx() == E_ITEMS::SPIRIT_DRAW_TICKET_33 ||
                   (E_ITEMS)objItem->getIdx() == E_ITEMS::DEFENSE_DRAW_TICKET_1 ||
                   (E_ITEMS)objItem->getIdx() == E_ITEMS::DEFENSE_DRAW_TICKET_11 ||
                   (E_ITEMS)objItem->getIdx() == E_ITEMS::DEFENSE_DRAW_TICKET_33 
                   )
                {
                    sprIcon->setScale(1.5f);
                    sprIcon->setPosition(sprIcon->getPosition() + Vec2::UP * 15);
                }
                else
                    sprIcon->setScale(2.f);
            }
            else if(objItem->getType().compare("STONE") == 0)
                sprIcon->setScale(2.f);
            else if(objItem->getType().compare("BOX") == 0)
            {
                sprIcon->setScale(1.5f);
                sprIcon->setPosition(sprIcon->getPosition() + Vec2::UP * 15);
            }
            else if(objItem->getType().compare("COSTUME") == 0)
                sprIcon->setScale(2.f);
            else if(objItem->getType().compare("PET_SKIN") == 0)
                sprIcon->setScale(3.f);
            else if(objItem->getType().compare("WEAPON_SPIRIT") == 0)
                sprIcon->setScale(2.f);
            itemBG->addChild(sprIcon);
            
            if(objItem->getType().compare("COSTUME") != 0 &&
               objItem->getType().compare("PET_SKIN") != 0)
            {
                auto lbCount = Label::createWithTTF(objItem->getCount(), GAME_FONT, 20);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbCount->setPosition(itemBG->getContentSize().width/2, 47);
                itemBG->addChild(lbCount);
                
            }
            
            if(isImpossibleBuy)
            {
                sprIcon->setColor(Color3B(90,90,90));
            }
        }
        
        auto sprPriceBox = Sprite::create("Assets/ui/event/tajir_shop_box_price_1.png");
        sprPriceBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprPriceBox->setPosition(itemBG->getContentSize().width/2, 15);
        itemBG->addChild(sprPriceBox);
        {
            
            auto sprIcon = Sprite::create(objPrice->getPath());
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprIcon->setPosition(-5, sprPriceBox->getContentSize().height/2);
            sprPriceBox->addChild(sprIcon);
            
            auto lbPrice = MafNode::createBmFont(objPrice->getCount(), GAME_BM_FONT_AL, 24);
            lbPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            lbPrice->setPosition(sprPriceBox->getContentSize().width - 20, sprPriceBox->getContentSize().height/2);
            lbPrice->setDimensions(sprPriceBox->getContentSize().width - sprIcon->getContentSize().width - 5, sprPriceBox->getContentSize().height);
            lbPrice->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
            lbPrice->setOverflow(Label::Overflow::SHRINK);
            lbPrice->setColor(Color3B(159,86,4));
            sprPriceBox->addChild(lbPrice);
            
            if(isImpossibleBuy)
            {
                lbPrice->enableStrikethrough();
            }
        }
        if(objTradingItem->getType() == InfoRaidTradingMarketReward::E_TYPE::FIXED)
        {
            auto sprDaily = Sprite::create("Assets/ui/purchase/shop_event_1.png");
            sprDaily->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            sprDaily->setPosition(itemBG->getContentSize().width, itemBG->getContentSize().height);
            itemBG->addChild(sprDaily);
        }
        
        auto lbLimitCount = MafNode::createBmFont(MafUtils::format("%d/%d", objTradingItem->getBuyCount(), objTradingItem->getMaxBuyCount()), GAME_BM_FONT_AL, 24);
        lbLimitCount->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        lbLimitCount->setPosition(itemBG->getContentSize().width - 10, sprPriceBox->getPosition().y + 2);
        lbLimitCount->setDimensions(itemBG->getContentSize().width -10, lbLimitCount->getContentSize().height);
        lbLimitCount->setAlignment(TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
        lbLimitCount->setOverflow(Label::Overflow::SHRINK);
        itemBG->addChild(lbLimitCount);
        if(isImpossibleBuy)
        {
            lbLimitCount->setVisible(false);
        }
        if(objTradingItem->getType() == InfoRaidTradingMarketReward::E_TYPE::FIXED)
        {
            auto lbTimer = Label::createWithTTF("", GAME_FONT, 18);
            lbTimer->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            lbTimer->setPosition(itemBG->getContentSize().width - 10, sprPriceBox->getPosition().y - sprPriceBox->getContentSize().height - 5);
            lbTimer->setDimensions(itemBG->getContentSize().width -10, lbTimer->getContentSize().height);
            lbTimer->setAlignment(TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
            lbTimer->setOverflow(Label::Overflow::SHRINK);
            itemBG->addChild(lbTimer);
            {
                auto callFunc = CallFunc::create([&, lbTimer, objTradingItem](){
                    auto time = objTradingItem->getEndTime() - UtilsDate::getInstance()->getTime();
                    
                    if(time < 0)
                    {
                        refreshUI();
                        return;
                    }
                    
                    int hour = (int)time / 3600;
                    int min = (time % 3600) / 60;
                    int sec = time % 3600 % 60;
                    
                    if(hour < 0)
                        hour = 0;
                    if(min < 0)
                        min = 0;
                    if(sec < 0)
                        sec = 0;
                    
                    lbTimer->setString(MafUtils::format("%02d:%02d:%02d", hour, min, sec));
                });
                auto delayTime = DelayTime::create(1);
                auto seq = Sequence::create(callFunc, delayTime, nullptr);
                lbTimer->runAction(RepeatForever::create(seq));
            }
        }
        
    }
    
    return cell;
}

ssize_t UILayerRaidTrading::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _listInfoItems.size()/4;
    if ( _listInfoItems.size() % 4 != 0 )
        size++;
    return size;
}


#pragma mark - override
void UILayerRaidTrading::refreshUI()
{
    uiTop();
    uiMiddle();
    uiBottom();
    _listInfoItems = NewRaidManager::getInstance()->getListActivateTradingMarketReward();
    uiTable();
}

#pragma mark - get set
void UILayerRaidTrading::setCloseCondition(bool value)
{
    _isCloseCondition = value;
}
void UILayerRaidTrading::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark - utils
void UILayerRaidTrading::requestInfo()
{
    // network
    PopupLoading::show();
    NewRaidManager::getInstance()->requestTradingMarketInfo(CC_CALLBACK_2(UILayerRaidTrading::callbackInfo, this));
}

#pragma mark - init
void UILayerRaidTrading::initVar()
{
    
}

void UILayerRaidTrading::initUi()
{
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 90));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI, E_ZORER::UI + 1);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(getContentSize().width - 10, 240));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyMiddleUI, E_ZORER::UI);
    
    _lyTableUI = LayerColor::create();
    _lyTableUI->setContentSize( Size(getContentSize().width - 10, 540));
    _lyTableUI->setIgnoreAnchorPointForPosition(false);
    _lyTableUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTableUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    addChild(_lyTableUI, E_ZORER::UI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - _lyTableUI->getContentSize().height - _lyMiddleUI->getContentSize().height - 10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyTableUI->getPosition() + Vec2::DOWN * _lyTableUI->getContentSize().height);
    addChild(_lyBottomUI, E_ZORER::UI +1);
    
    //
    uiTop();
    uiMiddle();
    uiTable();
    uiBottom();
}

#pragma mark - init
void UILayerRaidTrading::uiTop()
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
    btnTicketBuy->addClickEventListener(CC_CALLBACK_1(UILayerRaidTrading::onClickTicketBuy, this));
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

void UILayerRaidTrading::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();

    auto sprBG = Sprite::create("Assets/ui/raid/raid_shop/exchange_bg1_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyMiddleUI->getContentSize().width/2, _lyMiddleUI->getContentSize().height);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4);
    _lyMiddleUI->addChild(sprBG);
    
    auto particle = ParticleSnow::createWithTotalParticles(random(40, 90));
    particle->setGravity(Vec2(0, 10));
    particle->setAngle(85);
    particle->setSpeed(130);
    particle->setSpeedVar(30);
    particle->setStartSize(random(5, 15));
    particle->setStartSizeVar(5);
    particle->setEndSize(-1);
    particle->setEndSizeVar(0);
    particle->setPosition( sprBG->getPosition() + Vec2::DOWN * (sprBG->getContentSize().height * sprBG->getScale())  );
    particle->setLife(1.5f);
    particle->setLifeVar(1);
    particle->setEmissionRate(particle->getTotalParticles()/particle->getLife());
    particle->setTexture( Director::getInstance()->getTextureCache()->addImage("Assets/ui/team_raid/flame_s_2.png") );

    _lyMiddleUI->addChild(particle,E_ZORER::UI);
    
    auto sprClock = Sprite::create("Assets/ui/prison/clock.png");
    sprClock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprClock->setPosition(Vec2(5, -8));
    _lyMiddleUI->addChild(sprClock);
    
    auto lbSeasonTimer = Label::createWithTTF("", GAME_FONT, 24);
    lbSeasonTimer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbSeasonTimer->setPosition(Vec2(8, 5) + Vec2::RIGHT * sprClock->getContentSize().width);
    lbSeasonTimer->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
    lbSeasonTimer->setDimensions(_lyMiddleUI->getContentSize().width - sprClock->getContentSize().width, lbSeasonTimer->getContentSize().height);
    _lyMiddleUI->addChild(lbSeasonTimer);
    {
        auto callFunc = CallFunc::create([&, lbSeasonTimer](){
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
            
            lbSeasonTimer->setString(GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min));
        });
        auto delayTime = DelayTime::create(60.f);
        auto seq = Sequence::create(callFunc, delayTime, nullptr);
        lbSeasonTimer->runAction(RepeatForever::create(seq));
    }
    
}

void UILayerRaidTrading::uiTable()
{
    _lyTableUI->removeAllChildren();
    auto sprBG = Sprite::create("Assets/ui/event/event_tajir_bg2_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyTableUI->getContentSize().width/2, _lyTableUI->getContentSize().height - 10);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4);
    _lyTableUI->addChild(sprBG);
    
    auto sprRefreshBox = Sprite::create("Assets/ui_common/base_text_2_5.png");
    sprRefreshBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprRefreshBox->setPosition(0, _lyTableUI->getContentSize().height - 30);
    _lyTableUI->addChild(sprRefreshBox);
    
    auto btnRefresh = ui::Button::create("Assets/ui_common/btn_refresh2.png");
    btnRefresh->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnRefresh->setPosition(Vec2(10, sprRefreshBox->getContentSize().height/2));
    btnRefresh->setZoomScale(0.05f);
    btnRefresh->addClickEventListener(CC_CALLBACK_1(UILayerRaidTrading::onClickRefresh, this));
    sprRefreshBox->addChild(btnRefresh);
    {
        int nMax = 0;
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("market_reset_limit");
        
        if(objSetting != nullptr)
            nMax = objSetting->getValueToInt();
        
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_raidshop_exchange_reset_bt", NewRaidManager::getInstance()->getMarketRefreshCount(), nMax), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(btnRefresh->getPosition() + Vec2::RIGHT * btnRefresh->getContentSize().width);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
        lbText->setDimensions(sprRefreshBox->getContentSize().width - btnRefresh->getContentSize().width, sprRefreshBox->getContentSize().height - 5);
        sprRefreshBox->addChild(lbText);
        
        if(nMax <= NewRaidManager::getInstance()->getMarketRefreshCount())
        {
            btnRefresh->setEnabled(false);
        }
    }
    
    
    _table = extension::TableView::create(this, Size(_lyTableUI->getContentSize().width, 425));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyTableUI->getContentSize().width / 2, 20);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTableUI->addChild(_table);
    
    _table->reloadData();
}

void UILayerRaidTrading::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBG->setContentSize(Size(_lyBottomUI->getContentSize().width - 10, _lyBottomUI->getContentSize().height - 10));
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(_lyBottomUI->getContentSize()/2);
    _lyBottomUI->addChild(sprBG);
    
    std::string strText = "t_ui_raidshop_exchange_info";
    
    auto lbText = Label::createWithTTF(GAME_TEXT(strText), GAME_FONT, 24);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbText->setPosition(Vec2(20, sprBG->getContentSize().height/2));
    lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(sprBG->getContentSize().width - 30, sprBG->getContentSize().height - 10);
    lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBG->addChild(lbText);
    
}
#pragma mark -utils
void UILayerRaidTrading::onClickTicketBuy(cocos2d::Ref* sender)
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
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerRaidTrading::callbackPurchase, this));
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
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_new_raid_ticket_pop_1"));
            popup->addButton(GAME_TEXT("t_ui_costume_skin_msg_2"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(UILayerRaidTrading::callbackPurchase, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
                
        }
    });
}
void UILayerRaidTrading::onClickBuyItem(cocos2d::Ref* sender)
{
    auto node = (Node*)sender;
    
    int nIdx = node->getTag();
    bool isImpossibleBuy = false;
    
    auto objTradingItem = NewRaidManager::getInstance()->getRaidTradingMarketReward(nIdx);
    
    if(objTradingItem == nullptr)
        return;
    
    if(objTradingItem->getBuyCount() >= objTradingItem->getMaxBuyCount())
        isImpossibleBuy = true;
    
    auto objItem = objTradingItem->getItem();
    
    if(objItem->getType().compare("COSTUME") == 0)
    {
        if(CostumeSkinManager::getInstance()->isCostumeSkinBuy(objItem->getTypeParams()))
            isImpossibleBuy = true;
        
    }
    else if(objItem->getType().compare("PET_SKIN") == 0)
    {
        E_PET ePet;
        if(objItem->getIdx() == 14000)
            ePet = E_PET::CHICK_1;
        else if(objItem->getIdx() == 14001)
            ePet = E_PET::HAMSTER_2;
        else if(objItem->getIdx() == 14002)
            ePet = E_PET::DRAGON_3;
        else if(objItem->getIdx() == 14003)
            ePet = E_PET::PRINCESS_4;
        else if(objItem->getIdx() == 14004)
            ePet = E_PET::PURIRING_5;
        else if(objItem->getIdx() == 14005)
            ePet = E_PET::HONG_6;
        else if(objItem->getIdx() == 14006)
            ePet = E_PET::MOLE_7;
        else if(objItem->getIdx() == 14007)
            ePet = E_PET::HANDSOME_8;
        
        if(PetNewManager::getInstance()->isPetSkinBuy(ePet, objItem->getTypeParams()))
        {
            isImpossibleBuy = true;
        }
    }
    else if(objItem->getType().compare("STONE") == 0)
    {
        if(objItem->getTypeDetail().compare("ATK") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK) >= 4300)
                isImpossibleBuy = true;
        }
        else if(objItem->getTypeDetail().compare("GOLD") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD) >= 4300)
                isImpossibleBuy = true;
        }
        else if(objItem->getTypeDetail().compare("KEY") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY) >= 4300)
                isImpossibleBuy = true;
        }
        else if(objItem->getTypeDetail().compare("FORCE") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::FORCE_ATTACK) >= 4300)
                isImpossibleBuy = true;
        }
    }
        
    if(isImpossibleBuy)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_53"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    auto objPriceItem = objTradingItem->getPrice();
    auto objRewardItem = objTradingItem->getItem();
    
    if(objPriceItem == nullptr || objRewardItem == nullptr)
        return;
    
    if((E_ITEMS)objPriceItem->getIdx() == E_ITEMS::NEW_RAID_DRAGON_BALL)
    {
        if(std::atoi(objPriceItem->getCount().c_str()) > NewRaidManager::getInstance()->getDragonBall())
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    else if((E_ITEMS)objPriceItem->getIdx() == E_ITEMS::COSTUME_COIN)
    {
        if(std::atoi(objPriceItem->getCount().c_str()) > std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()))
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_team_raid_buy_1", objRewardItem->getText().c_str()));
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_buy"), false, [=](Ref*){
        PopupLoading::show();
        NewRaidManager::getInstance()->requestTradingMarketBuy(CC_CALLBACK_2(UILayerRaidTrading::callbackBuyItem, this), nIdx);
    });
    popup->show();
    
    
}
void UILayerRaidTrading::onClickRefresh(cocos2d::Ref* sender)
{
    
    auto objPriceSetting = NewRaidManager::getInstance()->getRaidSetting("market_reset_cost");
    auto objMaxSetting = NewRaidManager::getInstance()->getRaidSetting("market_reset_limit");
    int price = 100;
    int max = 5;
    
    if(objPriceSetting != nullptr)
        price = objPriceSetting->getValueToInt();
    if(objMaxSetting != nullptr)
        max = objMaxSetting->getValueToInt();
    
    if(max <= NewRaidManager::getInstance()->getMarketRefreshCount())
    {
        return;
    }
    
    if(price > std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()))
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_raidshop_exchange_reset_cost_msg", price));
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_buy"), false, [=](Ref*){
        PopupLoading::show();
        NewRaidManager::getInstance()->requestTradingMarketInfo(CC_CALLBACK_2(UILayerRaidTrading::callbackInfo, this), true);
    });
    popup->show();
}
#pragma mark -Callback
void UILayerRaidTrading::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 8 )
        {
            strMessageKey = "t_ui_new_raid_error_1";
        }
        else if( nResult == 9 )
        {
            strMessageKey = "t_ui_new_raid_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    _listInfoItems = NewRaidManager::getInstance()->getListActivateTradingMarketReward();
    
    refreshUI();
}

#pragma mark - Purchase
void UILayerRaidTrading::callbackProductOk(int nIdx)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(UILayerRaidTrading::callbackPurchase, this));
}

void UILayerRaidTrading::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
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

void UILayerRaidTrading::callbackBuyItem(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 2)
            strMessageKey = "t_ui_cash_msg_9_0";
        else if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    refreshUI();
}
