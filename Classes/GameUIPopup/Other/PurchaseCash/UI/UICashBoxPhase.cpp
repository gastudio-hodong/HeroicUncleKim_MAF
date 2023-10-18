//
//  UICashBoxPhase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UICashBoxPhase.h"

#include "GameObject/InfoSetting.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

UICashBoxPhase* UICashBoxPhase::create(int idx, int idxPrev, std::string pathEnable, std::string pathDisable)
{
    UICashBoxPhase* pRet = new(std::nothrow) UICashBoxPhase();
    if ( pRet && pRet->init(idx, idxPrev, pathEnable, pathDisable) )
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

UICashBoxPhase::UICashBoxPhase():
_bInitUI(false),

//
_size(Size::ZERO),

_idx(0),
_idxPrev(0),
_pathEnable(""),
_pathDisable(""),

//
_uiContainerInfo(nullptr)
{
    
}

bool UICashBoxPhase::init(int idx, int idxPrev, std::string pathEnable, std::string pathDisable)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    //
    _idx = idx;
    _idxPrev = idxPrev;
    _pathEnable = pathEnable;
    _pathDisable = pathDisable;
    
    auto uiBG = ui::ImageView::create(_pathEnable);
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


void UICashBoxPhase::onEnter()
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
void UICashBoxPhase::initVar()
{
    
}

void UICashBoxPhase::initUI()
{
    
}

#pragma mark - ui
void UICashBoxPhase::uiContainer()
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

void UICashBoxPhase::uiInfo()
{
    //
    auto objProduct = CashManager::getInstance()->getProduct(_idx);
    
    //
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    std::string pathBG = _pathEnable;
    if ( _idxPrev != 0 && CashManager::getInstance()->getPurchaseRemaining(_idxPrev) > 0 )
    {
        pathBG = _pathDisable;
    }
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBG->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    layout->addChild(uiBG);

    std::string pathIcon = "Assets/ui/purchase/" + objProduct->getIconName();
    std::string nameText = objProduct->getName();
    std::string priceText = getInfoPrice(objProduct);
    if ( priceText.length() == 0 )
    {
        priceText = GAME_TEXT("t_ui_shop_price_loading");
    }
    
    //
    auto uiIcon = ui::ImageView::create(pathIcon);
    uiIcon->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiIcon);
    
    // name
    auto uiName = ui::CText::create(nameText, GAME_FONT, 22);
    uiName->setPosition(Vec2(size.width / 2, size.height - 23));
    uiName->setTextAreaSize(Size(size.width - 10, uiName->getContentSize().height));
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    layout->addChild(uiName);
    
    // price
    auto uiPrice = ui::CText::create(priceText, SYSTEM_FONT, 28);
    uiPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiPrice->setPosition(Vec2(size.width - 10, 26 + 25));
    uiPrice->setTextAreaSize(Size(155, uiPrice->getContentSize().height));
    uiPrice->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiPrice->setTextHorizontalAlignment(TextHAlignment::RIGHT);
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
        auto action2 = DelayTime::create(2.0);
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
        uiLayoutReward->setPosition(Vec2(layout->getContentSize().width / 2, 50.5));
        uiLayoutReward->requestDoLayout();
    }
    
    // lock
    auto uiLock = ui::ImageView::create("Assets/ui/purchase/shop_sold_02.png");
    uiLock->setPosition(Vec2(size.width / 2, size.height / 2));
    uiLock->setVisible(false);
    layout->addChild(uiLock, 100);
    
    // 잠금
    {
        //
        if ( objProduct->getSaleLimit() > 0 && CashManager::getInstance()->getPurchaseRemaining(objProduct->getIdx()) <= 0 )
        {
            uiLock->setVisible(true);
            this->setEnabled(false);
        }
        
        //
        if ( _idxPrev != 0 && CashManager::getInstance()->getPurchaseRemaining(_idxPrev) > 0 )
        {
            this->setEnabled(false);
        }
    }
}

#pragma mark - draw

#pragma mark - set, get
std::string UICashBoxPhase::getInfoPrice(InfoProduct* obj)
{
    std::string priceText = obj->getPrice();
    
    return priceText;
}

#pragma mark - callback

#pragma mark - click

#pragma mark - game
