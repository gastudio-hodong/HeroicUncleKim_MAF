//
//  UICurrency.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UICurrency.h"

#include "ManagerGame/ItemsMoneyManager.h"

#include "GameUIPopup/Common/PopupDefault.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerEvent/EventFishingManager.h"
#include "ManagerEvent/EventBoardGameManager.h"
#include "ManagerEvent/EventTradingMarketManager.h"

USING_NS_CC;

UICurrency* UICurrency::create(std::vector<E_ITEMS> list, cocos2d::Size size)
{
    UICurrency* pRet = new(std::nothrow) UICurrency();
    if ( pRet && pRet->init() )
    {
        pRet->setList(list);
        pRet->setContentSize(size);
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

UICurrency* UICurrency::create(cocos2d::Vector<InfoItems*> list, cocos2d::Size size)
{
    UICurrency* pRet = new(std::nothrow) UICurrency();
    if ( pRet && pRet->init() )
    {
        pRet->setList(list);
        pRet->setContentSize(size);
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

UICurrency::UICurrency(void) :
_bInitUI(false),

//
_bEffect(false),
_bKeyLongShow(false),

_uiPosition(Vec2::ANCHOR_MIDDLE_LEFT)
{
    
}

UICurrency::~UICurrency(void)
{
    
}

bool UICurrency::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void UICurrency::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        // init
        initVar();
        initUi();
        
        //
        RefreshManager::getInstance()->addUI(E_REFRESH::CURRENCY, this);
        
        //
        _bInitUI = true;
    }
    
    //
    drawCurrency();
    drawCurrencyCount();
}
 

#pragma mark - refresh
void UICurrency::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::CURRENCY )
    {
        if ( listData.size() == 0 )
        {
            drawCurrencyCount();
        }
        else if ( _bEffect == true )
        {
            E_ITEMS eCurrencyType = (E_ITEMS)(listData.at("type").asInt());
            std::string strCurrencyCount = listData.at("count").asString();

            bool bCurrencyPet = false;
            if ( listData.find("pet") != listData.end() )
            {
                bCurrencyPet = listData.at("pet").asBool();
            }
            
            for ( int i = 0; i < _listData.size(); i++ )
            {
                auto objItems = _listData.at(i);
                if ( objItems == nullptr || objItems->isVersion() == false )
                {
                    continue;
                }
                
                if( (E_ITEMS)objItems->getIdx() != eCurrencyType )
                {
                    continue;
                }
                
                auto layer = (ui::Layout*)_layerContainerCurrency->getChildByTag(i);
                if ( layer == nullptr )
                    continue;
                
                auto labelParent = (ui::CText*)layer->getChildByName("LABLE_COUNT");
                if ( labelParent == nullptr )
                    continue;

                //
                MafUtils::convertNumberToShort(strCurrencyCount);
                strCurrencyCount.insert(0, "+ ");

                auto label = MafNode::createBmFont(strCurrencyCount, GAME_BM_FONT_AL, 30);
                label->setAnchorPoint(Vec2(0,0));
                label->setPosition(0, labelParent->getContentSize().height);
                if ( bCurrencyPet == true )
                {
                    label->setColor(Color3B::WHITE);
                }
                else
                {
                    label->setColor(labelParent->getColor());
                }
                labelParent->addChild(label);

                auto action1 = MoveBy::create(0.2f, Vec2(0,20));
                auto action2 = EaseIn::create(action1, 3);
                auto action3 = FadeOut::create(0.2f);
                auto seq = Sequence::create(action2, action3, RemoveSelf::create(), nullptr);
                label->runAction(seq);
                
                break;
            }
        }
    }
}

#pragma mark - init
void UICurrency::initVar()
{
    
}

void UICurrency::initUi()
{
    auto size = getContentSize();
    
    _layerContainerCurrency = ui::Layout::create();
    _layerContainerCurrency->setContentSize(size);
    _layerContainerCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _layerContainerCurrency->setPositionType(ui::Widget::PositionType::PERCENT);
    _layerContainerCurrency->setPositionPercent(Vec2::ANCHOR_MIDDLE_LEFT);
    _layerContainerCurrency->setLayoutType(ui::Layout::Type::HORIZONTAL);
    addChild(_layerContainerCurrency);
}

#pragma mark - ui
void UICurrency::drawCurrency()
{
    auto layout = _layerContainerCurrency;
    auto size = getContentSize();
    
    layout->setAnchorPoint(_uiPosition);
    layout->setPositionPercent(_uiPosition);
    layout->setContentSize(size);
    layout->removeAllChildren();
    
    
    //
    double padding = 5;
    
    double width = (layout->getContentSize().width - (padding * (_listData.size()-1))) / _listData.size();
    width = MIN(170, width);
    
    double height = layout->getContentSize().height;
    
    for ( int i = 0; i < _listData.size(); i++ )
    {
        auto objItems = _listData.at(i);
        if ( objItems == nullptr || objItems->isVersion() == false )
        {
            continue;
        }
        
        E_ITEMS eType = (E_ITEMS)objItems->getIdx();
        
        auto uiChild = ui::Layout::create();
        uiChild->setContentSize(Size(width, height));
        uiChild->setTouchEnabled(true);
        uiChild->addClickEventListener(CC_CALLBACK_1(UICurrency::onClickCurrencyIcon, this));
        uiChild->setTag(i);
        layout->addChild(uiChild);
        if ( i != 0 )
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(5, 0, 0, 0));
            uiChild->setLayoutParameter(params);
        }
      
        // icon
        auto uiIcon = ui::ImageView::create(objItems->getPath());
        uiIcon->setPosition(Vec2(29, uiChild->getContentSize().height / 2));
        uiChild->addChild(uiIcon);
        switch((E_ITEMS)objItems->getIdx())
        {
            case E_ITEMS::SPIRIT_DRAW_TICKET_1:
            case E_ITEMS::SPIRIT_DRAW_TICKET_11:
            case E_ITEMS::SPIRIT_DRAW_TICKET_33:
            case E_ITEMS::DEFENSE_DRAW_TICKET_1:
            case E_ITEMS::DEFENSE_DRAW_TICKET_11:
            case E_ITEMS::DEFENSE_DRAW_TICKET_33: uiIcon->setScale(0.8f); break;
            default:break;
        }
        
        //
        auto uiCount = ui::CText::create("0", GAME_FONT_AL, 22);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCount->setPosition(Vec2(53, uiChild->getContentSize().height / 2));
        uiCount->setTextColor(Color4B(objItems->getColor()));
        uiCount->enableOutline(Color4B::BLACK, 1);
        uiCount->setName("LABLE_COUNT");
        uiChild->addChild(uiCount);
        switch((E_ITEMS)objItems->getIdx())
        {
            case E_ITEMS::SPIRIT_DRAW_TICKET_1:
            case E_ITEMS::SPIRIT_DRAW_TICKET_11:
            case E_ITEMS::SPIRIT_DRAW_TICKET_33:
            case E_ITEMS::DEFENSE_DRAW_TICKET_1:
            case E_ITEMS::DEFENSE_DRAW_TICKET_11:
            case E_ITEMS::DEFENSE_DRAW_TICKET_33: uiCount->setPosition(uiCount->getPosition() + Vec2::RIGHT * 5); break;
            default:break;
        }
        
        if ( eType == E_ITEMS::KEY || eType == E_ITEMS::D_KEY )
        {
            uiCount->setTouchEnabled(true);
            uiCount->addClickEventListener(CC_CALLBACK_1(UICurrency::onClickCurrency, this));
        }
    }
    
    //
    auto sizeCurrency = UtilsWidget::getInstance()->getLayoutAccumulatedSize(_layerContainerCurrency);
    _layerContainerCurrency->setContentSize(sizeCurrency);
    _layerContainerCurrency->requestDoLayout();
}

void UICurrency::drawCurrencyCount()
{
    auto layout = _layerContainerCurrency;
    
    for ( int i = 0; i < _listData.size(); i++ )
    {
        std::string strCount = "0";
        
        auto objItems = _listData.at(i);
        if ( objItems == nullptr || objItems->isVersion() == false )
        {
            continue;
        }
        
        //
        E_ITEMS eType = (E_ITEMS)objItems->getIdx();
        switch (eType)
        {
            case E_ITEMS::GOLD:
            {
                strCount = ItemsMoneyManager::getInstance()->getGold();
                MafUtils::convertNumberToShort(strCount);
                break;
            }
            case E_ITEMS::KEY:
            {
                strCount = ItemsMoneyManager::getInstance()->getKey();
                
                if ( _bKeyLongShow == false )
                {
                    MafUtils::convertNumberToShort(strCount);
                }
                break;
            }
            case E_ITEMS::POINT:
            {
                strCount = MafUtils::toString(ItemsMoneyManager::getInstance()->getPoint());
                break;
            }
            case E_ITEMS::GEM:
            {
                strCount = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());
                break;
            }
            case E_ITEMS::D_GOLD:
            {
                strCount = ItemsMoneyManager::getInstance()->getDevilGold();
                MafUtils::convertNumberToShort(strCount);
                break;
            }
            case E_ITEMS::D_KEY:
            {
                strCount = ItemsMoneyManager::getInstance()->getDevilKey();
                
                if ( _bKeyLongShow == false )
                {
                    MafUtils::convertNumberToShort(strCount);
                }
                break;
            }
            case E_ITEMS::COSTUME_COUPON:
            {
                strCount = MafUtils::toString(ItemsMoneyManager::getInstance()->getCostumeCoupon());
                break;
            }
            case E_ITEMS::CUBE:
            {
                strCount = MafUtils::toString(ItemsMoneyManager::getInstance()->getCube());
                break;
            }
            case E_ITEMS::MONSTER_SOUL_PIECE:
            {
                strCount = MafUtils::toString(ItemsMoneyManager::getInstance()->getMonsterSoulPiece());
                break;
            }
            case E_ITEMS::EVENT_BAIT:
            {
                strCount = MafUtils::toString(EventFishingManager::getInstance()->getBaitCount());
                break;
            }
            case E_ITEMS::EVENT_BLACK_PEARL:
            {
                strCount = MafUtils::toString(EventFishingManager::getInstance()->getPearlCount());
                break;
            }
            case E_ITEMS::EVENT_HALLOWEEN_CANDY:
            {
                strCount = MafUtils::toString(EventBoardGameManager::getInstance()->getTotalCandy());
                break;
            }
            case E_ITEMS::EVENT_DICE_POINT:
            {
                strCount = MafUtils::toString(EventBoardGameManager::getInstance()->getTotalPoint());
                break;
            }
            case E_ITEMS::NEW_RAID_DRAGON_BALL:
            {
                strCount = MafUtils::toString(NewRaidManager::getInstance()->getDragonBall());
                break;
            }
            case E_ITEMS::SPIRIT_STON:
            {
                strCount = MafUtils::toString(WeaponSpiritManager::getInstance()->getSpiritStonCount());
                break;
            }
            case E_ITEMS::SPIRIT_DRAW_TICKET_1:
            {
                strCount = MafUtils::toString(WeaponSpiritManager::getInstance()->getSummonTicket_1());
                break;
            }
            case E_ITEMS::SPIRIT_DRAW_TICKET_11:
            {
                strCount = MafUtils::toString(WeaponSpiritManager::getInstance()->getSummonTicket_11());
                break;
            }
            case E_ITEMS::SPIRIT_DRAW_TICKET_33:
            {
                strCount = MafUtils::toString(WeaponSpiritManager::getInstance()->getSummonTicket_33());
                break;
            }
            case E_ITEMS::EVENT_TRADING_MARKET_COIN:
            {
                strCount = MafUtils::toString(EventTradingMarketManager::getInstance()->getTotalCoin());
                break;
            }
            case E_ITEMS::ADVENTURE_TICKET:
            {
                strCount = MafUtils::toString(AdventureManager::getInstance()->getTicket());
                break;
            }
            case E_ITEMS::ADVENTURE_CORE:
            {
                int nCount = 0;
                auto objCore = AdventureRelicManager::getInstance()->getInfoCore(objItems->getTypeParams());
                
                if(objCore != nullptr)
                    nCount = objCore->getCount();
                strCount = MafUtils::toString(nCount);
                break;
            }
            case E_ITEMS::TICKET:
            {
                strCount = MafUtils::toString(PrisonManager::getInstance()->getHeart());
                break;
            }
            case E_ITEMS::EVENT_RACE_COIN:
            {
                strCount = ItemsManager::getInstance()->getItems(E_ITEMS::EVENT_RACE_COIN);
                MafUtils::convertNumberToShort(strCount);
                break;
            }
            case E_ITEMS::DEFENSE_COIN:
            {
                strCount = std::to_string(DefenseManager::getInstance()->getDefenseCoin());
            }break;
            case E_ITEMS::DEFENSE_DRAW_TICKET_1:
            {
                strCount = MafUtils::toString(DefenseManager::getInstance()->getDrawTicket_1());
            }break;
            case E_ITEMS::DEFENSE_DRAW_TICKET_11:
            {
                strCount = MafUtils::toString(DefenseManager::getInstance()->getDrawTicket_11());
            }break;
            case E_ITEMS::DEFENSE_DRAW_TICKET_33:
            {
                strCount = MafUtils::toString(DefenseManager::getInstance()->getDrawTicket_33());
            }break;
                
            default:
                strCount = ItemsManager::getInstance()->getItems(eType);
                break;
        }
        
        auto uiChild = (ui::Layout*)layout->getChildByTag(i);
        if ( uiChild == nullptr )
        {
            continue;
        }
        
        auto uiCount = (ui::CText*)uiChild->getChildByName("LABLE_COUNT");
        if ( uiCount == nullptr )
        {
            continue;
        }
        
        double widthCount = uiChild->getContentSize().width - 53;
        uiCount->setString(strCount);
        uiCount->setScale(MIN(1, widthCount / uiCount->getContentSize().width));
        if ( _bKeyLongShow == true )
        {
            uiCount->setScale(1);
        }
    }
}

#pragma mark - click
void UICurrency::onClickCurrencyIcon(cocos2d::Ref* sender)
{
    auto item = (ui::Layout*)sender;
    
    auto obj = _listData.at(item->getTag());
    if(obj == nullptr)
        return;
    
    if ( PrisonManager::getInstance()->isDungeonPlay() == true )
    {
        return;
    }
    
    E_ITEMS eItems = (E_ITEMS)obj->getIdx();
    if ( eItems == E_ITEMS::COSTUME_COUPON )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_costume_coupon"), GAME_TEXT("t_ui_costume_coupon_explain"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else
    {
        if(eItems == E_ITEMS::ADVENTURE_CORE ||
           eItems == E_ITEMS::ADVENTURE_RELIC ||
           eItems == E_ITEMS::ADVENTURE_TICKET)
        {
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
            {
                return;
            }
        }
        auto popup = PopupCurrencyDetail::create(eItems, PopupCurrencyDetail::E_TYPE::CURRENCY, obj->getTypeParams());
        popup->show();
    }
    
}

void UICurrency::onClickCurrency(cocos2d::Ref* sender)
{
    if ( _bKeyLongShow == true )
    {
        return;
    }
        
    _bKeyLongShow = true;
    drawCurrencyCount();
    
    auto action1 = DelayTime::create(4.0);
    auto action2 = CallFunc::create([=](){
        _bKeyLongShow = false;
        drawCurrencyCount();
    });
    auto seq = Sequence::create(action1, action2, nullptr);
    runAction(seq);
}

#pragma mark - set, get, add
void UICurrency::setList(std::vector<E_ITEMS> list)
{
    _listData.clear();
    for(auto eType : list)
    {
        auto obj = ItemsManager::getInstance()->getInfoItems(eType);
        _listData.pushBack(obj);
    }
    
    if ( _bInitUI == true )
    {
        drawCurrency();
        drawCurrencyCount();
    }
}

void UICurrency::setList(cocos2d::Vector<InfoItems*> list)
{
    _listData.clear();
    _listData = list;
    
    if ( _bInitUI == true )
    {
        drawCurrency();
        drawCurrencyCount();
    }
}

void UICurrency::setEffect(bool bShow)
{
    _bEffect = bShow;
}

void UICurrency::setUIPosition(Vec2 position)
{
    _uiPosition = position;
    
    if ( _bInitUI == true )
    {
        drawCurrency();
        drawCurrencyCount();
    }
}
