//
//  UICashBox.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UICashBox.h"

#include "GameObject/InfoSetting.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

UICashBox* UICashBox::create(int idx, std::string path)
{
    UICashBox* pRet = new(std::nothrow) UICashBox();
    if ( pRet && pRet->init(idx, path) )
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

UICashBox::UICashBox():
_bInitUI(false),

//

//
_uiContainerInfo(nullptr)
{
    
}

bool UICashBox::init(int idx, std::string path)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    //
    _idx = idx;
    _path = path;
    
    auto uiBG = ui::ImageView::create(_path);
    if ( uiBG != nullptr )
    {
        _size = uiBG->getContentSize();
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(_size);
        
    //
    initVar();
    initUI();
    
    return true;
}


void UICashBox::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        
        _bInitUI = true;
    }
    
    // draw
    
}

#pragma mark - init
void UICashBox::initVar()
{
    
}

void UICashBox::initUI()
{
    
}

#pragma mark - ui
void UICashBox::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, size.height));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    addChild(_uiContainerInfo);
}

void UICashBox::uiInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create(_path);
    uiBG->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBG->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    layout->addChild(uiBG);

    //
    auto objProduct = CashManager::getInstance()->getProduct(_idx);
    
    std::string pathIcon = "Assets/ui/purchase/" + objProduct->getIconName();
    std::string nameText = objProduct->getName();
    std::string descText = getInfoTime(objProduct).append(getInfoCount(objProduct));
    std::string priceText = getInfoPrice(objProduct);
    if ( priceText.length() == 0 )
    {
        priceText = GAME_TEXT("t_ui_shop_price_loading");
    }
    
    //
    auto uiIcon = ui::ImageView::create(pathIcon);
    uiIcon->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiIcon);
    
    
    if ( objProduct->getSaleRatio().length() != 0 )
    {
        // name
        auto uiName = ui::CText::create(nameText, GAME_FONT, 22);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiName->setPosition(Vec2(size.width - 10, size.height - 2 - 21.5));
        uiName->setTextAreaSize(Size(size.width - 90, uiName->getContentSize().height));
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        layout->addChild(uiName);
        
        // ratio
        auto uiRatioBG = ui::ImageView::create("Assets/ui/purchase/shop_value.png");
        uiRatioBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiRatioBG->setPosition(Vec2(-20, size.height + 5));
        layout->addChild(uiRatioBG, 101);
        
        auto uiRatio = ui::CText::create(objProduct->getSaleRatio(), GAME_FONT, 28);
        uiRatio->setPosition(Vec2(uiRatioBG->getContentSize().width / 2, uiRatioBG->getContentSize().height - 40));
        uiRatio->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiRatio->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiRatio->setLineSpacing(-10);
        uiRatio->enableBold();
        uiRatioBG->addChild(uiRatio);
    }
    else
    {
        // name
        auto uiName = ui::CText::create(nameText, GAME_FONT, 22);
        uiName->setPosition(Vec2(size.width / 2, size.height - 2 - 21.5));
        uiName->setTextAreaSize(Size(size.width - 10, uiName->getContentSize().height));
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        layout->addChild(uiName);
    }
    
    // desc
    auto uiDesc = ui::CText::create(descText, GAME_FONT_AL, 22);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiDesc->setPosition(Vec2(size.width - 10, size.height - 45));
    uiDesc->setTextAreaSize(Size(size.width - 90, uiDesc->getContentSize().height));
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setLineSpacing(-5);
    layout->addChild(uiDesc);
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            std::string descText = getInfoTime(objProduct).append(getInfoCount(objProduct));
            
            ((ui::CText*)sender)->setString(descText);
        });
        auto action2 = DelayTime::create(1.0);
        uiDesc->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
    }
    
    // price
    auto uiPrice = ui::CText::create(priceText, SYSTEM_FONT, 28);
    uiPrice->setPosition(Vec2(size.width / 2, 2 + 27));
    uiPrice->setTextAreaSize(Size(size.width - 10, uiPrice->getContentSize().height));
    uiPrice->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiPrice->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiPrice->setTextOverflow(Label::Overflow::SHRINK);
    uiPrice->enableOutline(Color4B(63, 24, 6, 102), 4);
    layout->addChild(uiPrice);
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            auto objProduct = CashManager::getInstance()->getProduct(_idx);
            
            std::string priceText = getInfoPrice(objProduct);
            if ( priceText.length() == 0 )
            {
                priceText = GAME_TEXT("t_ui_shop_price_loading");
                ((ui::CText*)sender)->setString(priceText);
            }
            else
            {
                ((ui::CText*)sender)->setString(priceText);
                ((ui::CText*)sender)->stopAllActions();
            }
        });
        auto action2 = DelayTime::create(1.0);
        uiPrice->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
    }
    
    // items
    Vector<InfoItems*> listItemsAll;
    auto listItems = objProduct->getItems();
    auto listItemsDaily = objProduct->getItemsOther();
    
    listItemsAll.pushBack(listItems);
    listItemsAll.pushBack(listItemsDaily);
    
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(uiLayoutReward);
    {
        for ( int i = 0; i < listItemsAll.size(); i++ )
        {
            auto objItem = listItemsAll.at(i);
            
            //
            auto uiRewardBG = ui::ImageView::create("Assets/ui/purchase/shop_item.png");
            uiLayoutReward->addChild(uiRewardBG);
            {
                auto uiImgItemIcon = ui::ImageView::create(objItem->getPath());
                uiImgItemIcon->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2 + 5));
                uiRewardBG->addChild(uiImgItemIcon);
                 
                // item daily
                if ( i >= listItems.size() )
                {
                    auto spriteDaily = ui::ImageView::create("Assets/ui/purchase/shop_daily.png");
                    spriteDaily->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    spriteDaily->setPosition(Vec2(uiRewardBG->getContentSize().width, uiRewardBG->getContentSize().height - 8));
                    uiRewardBG->addChild(spriteDaily);
                }
                
                // item count
                std::string strCount = objItem->getCount();
                if ( objItem->getType().compare("BUFF") == 0 )
                {
                    if ( objItem->getTypeDetail().compare("REVIVAL") == 0 )
                    {
                        strCount = GAME_TEXTFORMAT("t_ui_hour", atoi(objItem->getCount().c_str()));
                    }
                    else
                    {
                        strCount = GAME_TEXTFORMAT("t_ui_second", atoi(objItem->getCount().c_str()));
                    }
                }
                if ( objItem->getType().compare("ADS") == 0 )
                {
                    if ( objItem->getIdx() == 48 )
                    {
                        strCount.append("H");
                    }
                    else if ( objItem->getIdx() == 53 )
                    {
                        strCount.append("D");
                    }
                }
                
                auto uiTextReward = ui::CText::create(strCount, GAME_FONT, 14);
                uiTextReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiTextReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 4));
                uiRewardBG->addChild(uiTextReward);
                
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(10, 0, 0, 0));
                    uiRewardBG->setLayoutParameter(params);
                }
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
        uiLayoutReward->setContentSize(sizeReward);
        uiLayoutReward->setPosition(Vec2(layout->getContentSize().width / 2, 80));
        uiLayoutReward->requestDoLayout();
    }
    
    // lock
    auto uiLock = ui::ImageView::create("Assets/ui/purchase/shop_sold_01.png");
    uiLock->setPosition(Vec2(size.width / 2, size.height / 2));
    uiLock->setVisible(false);
    layout->addChild(uiLock, 100);
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            bool bLock = false;
             
            // 판매종료
            if ( objProduct->getSaleType() == 5 )
            {
                int64_t timeNow = UtilsDate::getInstance()->getTime();
                if ( objProduct->getSaleTimeStart() > timeNow || objProduct->getSaleTimeEnd() < timeNow )
                {
                    bLock = true;
                }
            }
            
            if ( bLock == true )
            {
                ((ui::ImageView*)sender)->setVisible(true);
                this->setEnabled(false);
            }
        });
        auto action2 = DelayTime::create(1.0);
        uiLock->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
    }
    
    // 잠금
    {
        //
        if ( objProduct->getSaleLimit() > 0 && CashManager::getInstance()->getPurchaseRemaining(objProduct->getIdx()) <= 0 )
        {
            uiLock->setVisible(true);
            this->setEnabled(false);
        }
        
        if ( objProduct->getShopType() == E_PRODUCT::DAILY && CashManager::getInstance()->getDailyDate(objProduct->getIdx()) > 0 && CashManager::getInstance()->isDailyReceive(objProduct->getIdx()) == false )
        {
            this->setEnabled(false);
        }
    }
}

#pragma mark - draw

#pragma mark - set, get
std::string UICashBox::getInfoTime(InfoProduct* obj)
{
    std::string timeText = "";
    if ( (obj->getSaleType() == 2 || obj->getSaleType() == 3 || obj->getSaleType() == 4 || obj->getSaleType() == 5) == false )
    {
        return timeText;
    }
    
    time_t timeNow = UtilsDate::getInstance()->getTime();
    int timeDiff = 0;
    
    if ( obj->getSaleTimeStart() > timeNow )
    {
        timeDiff = 0;
    }
    else
    {
        if ( obj->getSaleType() == 2 )
        {// 2:월별 판매
            struct tm* tm = std::localtime(&timeNow);
            tm->tm_mon += 1;
            tm->tm_mday = 1;
            tm->tm_hour = 0;
            tm->tm_min = 0;
            tm->tm_sec = 0;
            time_t nTimeEnd = mktime(tm);
            
            timeDiff = (int)(nTimeEnd - UtilsDate::getInstance()->getTime());
        }
        else if ( obj->getSaleType() == 3 )
        {// 3:주간판매(월요일 0시 초기화)
            struct tm* tm = std::localtime(&timeNow);
            tm->tm_mday += (7 - tm->tm_wday + 1);
            tm->tm_hour = 0;
            tm->tm_min = 0;
            tm->tm_sec = 0;
            time_t nTimeEnd = mktime(tm);

            timeDiff = (int)(nTimeEnd - UtilsDate::getInstance()->getTime());
        }
        else if ( obj->getSaleType() == 4 )
        {// 4:일간판매
            struct tm* tm = std::localtime(&timeNow);
            tm->tm_mday += 1;
            tm->tm_hour = 0;
            tm->tm_min = 0;
            tm->tm_sec = 0;
            time_t nTimeEnd = mktime(tm);
            
            timeDiff = (int)(nTimeEnd - UtilsDate::getInstance()->getTime());
        }
        else if ( obj->getSaleType() == 5 )
        {
            timeDiff = (int)(obj->getSaleTimeEnd() - UtilsDate::getInstance()->getTime());
        }
        
        if ( timeDiff <= 0 )
        {
            timeDiff = 0;
        }
    }
     
    timeText = MafUtils::format("%02d:%02d:%02d", timeDiff / 3600, timeDiff % 3600 / 60, timeDiff % 60);
    timeText = MafUtils::format("{#FFFF00:%s}", timeText.c_str());
    timeText.append("\n");
    
    return timeText;
}

std::string UICashBox::getInfoCount(InfoProduct* obj)
{
    std::string countText = "";
    
    if ( obj->getSaleLimit() > 0 )
    {
        countText = MafUtils::format("%d/%d", obj->getSaleLimit() - CashManager::getInstance()->getPurchaseRemaining(obj->getIdx()), obj->getSaleLimit());
    }
    return countText;
}

std::string UICashBox::getInfoPrice(InfoProduct* obj)
{
    std::string priceText = obj->getPrice();
    
    if ( obj->getShopType() == E_PRODUCT::DAILY )
    {
        // 30일 정액제 상품
        int nDailyDate = CashManager::getInstance()->getDailyDate(obj->getIdx());
        bool isDailyReceive = CashManager::getInstance()->isDailyReceive(obj->getIdx());
        if ( nDailyDate >  0 )
        {
            if ( isDailyReceive == true )
            {
                priceText = GAME_TEXT("t_ui_shop_daily_reward");
            }
            else
            {
                priceText = GAME_TEXTFORMAT("t_ui_shop_daily_remain", nDailyDate);
            }
        }
    }
    
    return priceText;
}

#pragma mark - callback

#pragma mark - click

#pragma mark - game
