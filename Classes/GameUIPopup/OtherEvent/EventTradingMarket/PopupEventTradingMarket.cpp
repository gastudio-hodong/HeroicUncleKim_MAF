//
//  PopupEventTradingMarket.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#include "PopupEventTradingMarket.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventTradingMarketManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

bool sortList(InfoEventTradingItem *a, InfoEventTradingItem *b)
{
    bool result = false;
    
    if(a->getOrder() != b->getOrder())
        result = a->getOrder() < b->getOrder();
    else
        result = a->getIdx() < b->getIdx();
    
    return result;
}

PopupEventTradingMarket* PopupEventTradingMarket::create()
{
    PopupEventTradingMarket* pRet = new(std::nothrow) PopupEventTradingMarket();
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

PopupEventTradingMarket::PopupEventTradingMarket(void):
_lyTopUI(nullptr),
_lyMiddleUI(nullptr),
_lyBottomUI(nullptr),
_table(nullptr)
{
    _listInfoItems.clear();
}

PopupEventTradingMarket::~PopupEventTradingMarket(void)
{
    _listInfoItems.clear();
}


#pragma mark - table
void PopupEventTradingMarket::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupEventTradingMarket::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 210);
}

extension::TableViewCell* PopupEventTradingMarket::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
        
        double nPosX = 50 + 159 * i + 10 * i;
        double nPosY = cell->getContentSize().height/2;
        
        
        E_BUY_TYPE type = E_BUY_TYPE::POSSIBLE;
        
        std::string strBoxPath = "Assets/ui/event/tajir_shop_box_1_2.png";
        
        if(objTradingItem->getType().compare("All") == 0)
        {
            strBoxPath = "Assets/ui/event/tajir_shop_box_1_1.png";
        }
        
        if(objTradingItem->getMaxBuyCount() - objTradingItem->getBuyCount() <= 0)
        {
            type = E_BUY_TYPE::IMPOSSIBLE;
            strBoxPath = "Assets/ui/event/tajir_shop_box_1_3.png";
        }
        //무제한 구매 가능 상품
        if(objTradingItem->getType().compare("All") == 0 && objTradingItem->getMaxBuyCount() == 0)
        {
            type = E_BUY_TYPE::INFINITY_BUY;
            strBoxPath = "Assets/ui/event/tajir_shop_box_1_2.png";
        }
            
        auto objItem = objTradingItem->getItem();
        
        if(objItem == nullptr)
            continue;
        
        if(objItem->getType().compare("COSTUME") == 0)
        {
            if(CostumeSkinManager::getInstance()->isCostumeSkinBuy(objItem->getTypeParams()))
                type = E_BUY_TYPE::IMPOSSIBLE;
            
        }
        else if(objItem->getType().compare("STONE") == 0)
        {
            if(objItem->getTypeDetail().compare("ATK") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK) >= 4300)
                    type = E_BUY_TYPE::IMPOSSIBLE;
            }
            else if(objItem->getTypeDetail().compare("GOLD") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD) >= 4300)
                    type = E_BUY_TYPE::IMPOSSIBLE;
            }
            else if(objItem->getTypeDetail().compare("KEY") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY) >= 4300)
                    type = E_BUY_TYPE::IMPOSSIBLE;
            }
            else if(objItem->getTypeDetail().compare("FORCE") == 0)
            {
                if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::FORCE_ATTACK) >= 4300)
                    type = E_BUY_TYPE::IMPOSSIBLE;
            }
        }
        
        auto itemBG = MafNode::MafMenuItemSprite::create(Sprite::create(strBoxPath), nullptr, CC_CALLBACK_1(PopupEventTradingMarket::onClickBuyItem, this));
        itemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        itemBG->setPosition(nPosX, nPosY);
        itemBG->setTag(objTradingItem->getIdx());
        menu->addChild(itemBG);
        {
            auto sprIcon = ui::ImageView::create(objItem->getPath());
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            sprIcon->setPosition(Vec2(itemBG->getContentSize().width/2, 45));
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
            
            if(type == E_BUY_TYPE::IMPOSSIBLE)
            {
                sprIcon->setColor(Color3B(90,90,90));
            }
        }
        if(objTradingItem->getType().compare("Daily") == 0)
        {
            auto sprDaily = Sprite::create("Assets/ui/purchase/shop_daily2.png");
            sprDaily->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            sprDaily->setPosition(itemBG->getContentSize().width, itemBG->getContentSize().height);
            itemBG->addChild(sprDaily);
        }
        
        auto sprPriceBox = Sprite::create("Assets/ui/event/tajir_shop_box_price_1.png");
        sprPriceBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprPriceBox->setPosition(itemBG->getContentSize().width/2, 15);
        itemBG->addChild(sprPriceBox);
        {
            auto objPrice = objTradingItem->getPrice();
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
            
            if(type == E_BUY_TYPE::IMPOSSIBLE)
            {
                lbPrice->enableStrikethrough();
            }
        }
        
        auto lbLimitCount = MafNode::createBmFont(MafUtils::format("%d/%d", objTradingItem->getBuyCount(), objTradingItem->getMaxBuyCount()), GAME_BM_FONT_AL, 24);
        lbLimitCount->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        lbLimitCount->setPosition(itemBG->getContentSize().width - 10, sprPriceBox->getPosition().y + 2);
        lbLimitCount->setDimensions(itemBG->getContentSize().width -10, lbLimitCount->getContentSize().height);
        lbLimitCount->setAlignment(TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
        lbLimitCount->setOverflow(Label::Overflow::SHRINK);
        itemBG->addChild(lbLimitCount);
        if(type == E_BUY_TYPE::IMPOSSIBLE ||
           type == E_BUY_TYPE::INFINITY_BUY)
        {
            lbLimitCount->setVisible(false);
        }
    }
    
    return cell;
}

ssize_t PopupEventTradingMarket::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _listInfoItems.size()/4;
    if ( _listInfoItems.size() % 4 != 0 )
        size++;
    return size;
}

#pragma mark -init
bool PopupEventTradingMarket::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    std::sort(_listInfoItems.begin(), _listInfoItems.end(), sortList);
    _listInfoItems = EventTradingMarketManager::getInstance()->getListInfoEventTradingItem();
    
    initVar();
    initUI();
    
    return true;
}

void PopupEventTradingMarket::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    EventTradingMarketManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventTradingMarket::callbackInfo, this));
}

void PopupEventTradingMarket::initVar()
{
    _spriteContainer->setContentSize(Size(712+10, 1290));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupEventTradingMarket::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_exchange_center_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventTradingMarket::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 360));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyCurrencyUI = LayerColor::create();
    _lyCurrencyUI->setContentSize( Size(size.width - 10, 50));
    _lyCurrencyUI->setIgnoreAnchorPointForPosition(false);
    _lyCurrencyUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyCurrencyUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyCurrencyUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(size.width - 10, 650));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyCurrencyUI->getPosition() + Vec2::DOWN * _lyCurrencyUI->getContentSize().height);
    _spriteContainer->addChild(_lyMiddleUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height - 10 - _lyCurrencyUI->getContentSize().height));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiCurrency();
    uiMiddle();
    uiBottom();
}

#pragma mark -ui
void PopupEventTradingMarket::uiRefresh()
{
    _listInfoItems = EventTradingMarketManager::getInstance()->getListInfoEventTradingItem();
    uiTop();
    uiCurrency();
    uiBottom();
    if(_table != nullptr)
    {
        Vec2 offset = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(offset);
    }
        
}

void PopupEventTradingMarket::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/event/event_tajir_bg1_2.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 5);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4);
    _lyTopUI->addChild(sprBG);
    
    auto btnShop = ui::Button::create("Assets/ui_common/icon_help.png");
    btnShop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnShop->setPosition(Vec2(25, _lyTopUI->getContentSize().height - 25));
    btnShop->setZoomScale(0.05f);
    btnShop->addClickEventListener(CC_CALLBACK_1(PopupEventTradingMarket::onClickHelp, this));
    _lyTopUI->addChild(btnShop);
    
    
    {
        long time = EventTradingMarketManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();

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

        std::string strTime = MafUtils::format("%02d(D):%02d(H):%02d(M)", day, hour, min);
        if(EventManager::getInstance()->isActivatedEvent(29))
        {
            strTime = GAME_TEXT("t_ui_event_exchange_center_time2") + " : " + strTime;
        }
        else
        {
            strTime = GAME_TEXTFORMAT("t_ui_buff_newbie_time", strTime.c_str());
        }
 
        auto nodeBG = ui::ImageView::create("Assets/ui_common/base_text_2_4.png");
        nodeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        nodeBG->setPosition(Vec2(_lyTopUI->getContentSize().width, _lyTopUI->getContentSize().height - 50));
        nodeBG->setFlippedX(true);
        _lyTopUI->addChild(nodeBG);
        
        auto nodeText = ui::CText::create(strTime, GAME_FONT, 24);
        nodeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        nodeText->setPosition(Vec2(_lyTopUI->getContentSize().width, _lyTopUI->getContentSize().height - 50));
        nodeText->setTextAreaSize(Size(330, nodeText->getContentSize().height));
        nodeText->setTextOverflow(Label::Overflow::SHRINK);
        nodeText->setTextVerticalAlignment(TextVAlignment::CENTER);
        nodeText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        _lyTopUI->addChild(nodeText);
        
        auto nodeIcon = ui::ImageView::create("Assets/ui/prison/clock.png");
        nodeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        nodeIcon->setPosition(Vec2(nodeText->getPositionX() - nodeText->getContentSize().width + 10, nodeText->getPositionY()));
        _lyTopUI->addChild(nodeIcon);
        
        auto callFunc = CallFunc::create([&, nodeText](){

            long time = EventTradingMarketManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();

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

            std::string strTime = MafUtils::format("%02d(D):%02d(H):%02d(M)", day, hour, min);
            if(EventManager::getInstance()->isActivatedEvent(29))
            {
                strTime = GAME_TEXT("t_ui_event_exchange_center_time2") + " : " + strTime;
            }
            else
            {
                strTime = GAME_TEXTFORMAT("t_ui_buff_newbie_time", strTime.c_str());
            }

            nodeText->setString(strTime);
        });
        auto delayTime = DelayTime::create(60.f);
        auto seq = Sequence::create(callFunc, delayTime, nullptr);
        nodeText->runAction(RepeatForever::create(seq));
    }
}

void PopupEventTradingMarket::uiCurrency()
{
    _lyCurrencyUI->removeAllChildren();
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::EVENT_TRADING_MARKET_COIN);

    auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 40));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiCurrency->setPosition(Vec2(15, _lyCurrencyUI->getContentSize().height/2 - 5));
    _lyCurrencyUI->addChild(uiCurrency, E_ZORER::UI+1);
    
    list.clear();
    list.push_back(E_ITEMS::COSTUME_COIN);

    auto uiCurrency2 = UICurrency::create(list, Size(200, _lyCurrencyUI->getContentSize().height - 10));
    uiCurrency2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiCurrency2->setPosition(Vec2(_lyCurrencyUI->getContentSize().width - 15, _lyCurrencyUI->getContentSize().height/2 - 5));
    _lyCurrencyUI->addChild(uiCurrency2, E_ZORER::UI+1);
    
    if(EventManager::getInstance()->isActivatedEvent(28))
    {
        int nMaxCount = 0;
        
        auto objMaxCountSetting = EventTradingMarketManager::getInstance()->getRaidSetting("day_max_coin");
        
        if(objMaxCountSetting != nullptr)
            nMaxCount = objMaxCountSetting->getValueToInt();
        
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_event_exchange_center_limit", EventTradingMarketManager::getInstance()->getTodayCoin(), nMaxCount), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(Vec2(170, _lyCurrencyUI->getContentSize().height/2 - 5));
        _lyCurrencyUI->addChild(lbText);
    }
}

void PopupEventTradingMarket::uiMiddle()
{
    auto sprBG = Sprite::create("Assets/ui/event/event_tajir_bg2_1.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyMiddleUI->getContentSize().width/2, _lyMiddleUI->getContentSize().height - 10);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4);
    _lyMiddleUI->addChild(sprBG);
    
    _table = extension::TableView::create(this, Size(_lyMiddleUI->getContentSize().width, 540));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyMiddleUI->getContentSize().width / 2, 35);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyMiddleUI->addChild(_table);
    
    std::sort(_listInfoItems.begin(), _listInfoItems.end(), sortList);
    _table->reloadData();
}

void PopupEventTradingMarket::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBG->setContentSize(Size(_lyBottomUI->getContentSize().width - 10, _lyBottomUI->getContentSize().height - 10));
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(_lyBottomUI->getContentSize()/2);
    _lyBottomUI->addChild(sprBG);
    
    std::string strText = "t_ui_event_exchange_center_msg1";
    if(EventManager::getInstance()->isActivatedEvent(29) == true)
        strText = "t_ui_event_exchange_center_msg2";
    
    auto lbText = Label::createWithTTF(GAME_TEXT(strText), GAME_FONT, 24);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbText->setPosition(Vec2(20, sprBG->getContentSize().height/2));
    lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(sprBG->getContentSize().width - 30, sprBG->getContentSize().height - 10);
    lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBG->addChild(lbText);
    
    
}

#pragma mark -utils
void PopupEventTradingMarket::onClickBuyItem(Ref* sender)
{
    auto node = (Node*)sender;
    
    int nIdx = node->getTag();
    
    E_BUY_TYPE type = E_BUY_TYPE::POSSIBLE;
    
    auto objTradingItem = EventTradingMarketManager::getInstance()->getInfoEventTradingItem(nIdx);
    
    if(objTradingItem == nullptr)
        return;
    
    if(objTradingItem->getType().compare("All") == 0 && objTradingItem->getMaxBuyCount() == 0)
        type = E_BUY_TYPE::INFINITY_BUY;
    
    if(type != E_BUY_TYPE::INFINITY_BUY &&
       objTradingItem->getBuyCount() >= objTradingItem->getMaxBuyCount())
        return;
    
    auto objItem = objTradingItem->getItem();
    
    if(objItem->getType().compare("COSTUME") == 0)
    {
        if(CostumeSkinManager::getInstance()->isCostumeSkinBuy(objItem->getTypeParams()))
            type = E_BUY_TYPE::IMPOSSIBLE;
    }
    else if(objItem->getType().compare("STONE") == 0)
    {
        if(objItem->getTypeDetail().compare("ATK") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK) >= 4300)
                type = E_BUY_TYPE::IMPOSSIBLE;
        }
        else if(objItem->getTypeDetail().compare("GOLD") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD) >= 4300)
                type = E_BUY_TYPE::IMPOSSIBLE;
        }
        else if(objItem->getTypeDetail().compare("KEY") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY) >= 4300)
                type = E_BUY_TYPE::IMPOSSIBLE;
        }
        else if(objItem->getTypeDetail().compare("FORCE") == 0)
        {
            if(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::FORCE_ATTACK) >= 4300)
                type = E_BUY_TYPE::IMPOSSIBLE;
        }
    }
        
    if(type == E_BUY_TYPE::IMPOSSIBLE)
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
    
    if((E_ITEMS)objPriceItem->getIdx() == E_ITEMS::EVENT_TRADING_MARKET_COIN)
    {
        if(std::atoi(objPriceItem->getCount().c_str()) > EventTradingMarketManager::getInstance()->getTotalCoin())
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
        EventTradingMarketManager::getInstance()->requestBuyItem(CC_CALLBACK_2(PopupEventTradingMarket::callbackBuyItem, this), nIdx);
    });
    popup->show();
    
    
}
void PopupEventTradingMarket::onClickHelp(Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_exchange_center_info"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
}

void PopupEventTradingMarket::onClickClose(Ref* sender)
{
    hide();
}

#pragma mark -callback
void PopupEventTradingMarket::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    uiRefresh();
}


void PopupEventTradingMarket::callbackBuyItem(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        //_result' : 0 # 2 : 이벤트 진행 아님, 3 : 이벤트 정보 없음, 4 : 재화 부족, 5 : 구매 횟수 초과
        
        if(nResult == 2)
            strMessageKey = "t_ui_draw_msg_1";
        else if(nResult == 4)
            strMessageKey = "t_ui_error_31";
        else if(nResult == 5)
            strMessageKey = "t_ui_cash_msg_9_0";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    uiRefresh();
}
