//
//  UILayoutCashGroup.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutCashGroup.h"

#include "GameObject/InfoSetting.h"

#include "GameUIPopup/Other/PurchaseCash/UI/UICashBox.h"
#include "GameUIPopup/Other/PurchaseCash/UI/UICashBoxPhase.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

UILayoutCashGroup* UILayoutCashGroup::create(std::vector<int> list)
{
    UILayoutCashGroup* pRet = new(std::nothrow) UILayoutCashGroup();
    if ( pRet && pRet->init(list) )
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

UILayoutCashGroup::UILayoutCashGroup():
_bInitUI(false),

//

//
_uiContainerInfo(nullptr)
{
    
}

bool UILayoutCashGroup::init(std::vector<int> list)
{
    if ( !UILayoutCashBase::init() )
    {
        return false;
    }
    
    //
    _listGroup = list;
    
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutCashGroup::onEnter()
{
    UILayoutCashBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        
        _bInitUI = true;
    }
    
    // draw
    drawInfo();
}

#pragma mark - init
void UILayoutCashGroup::initVar()
{
    
}

void UILayoutCashGroup::initUI()
{
    
}

#pragma mark - ui
void UILayoutCashGroup::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, size.height));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    _uiContainerInfo->setClippingEnabled(true);
    addChild(_uiContainerInfo);
}

void UILayoutCashGroup::uiInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();

    auto uiScrollView = ui::ScrollView::create();
    uiScrollView->setContentSize(size);
    uiScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiScrollView->setPosition(Vec2(size.width / 2, 0));
    uiScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    uiScrollView->setScrollBarPositionFromCorner(Vec2(4, 4));
    uiScrollView->setScrollBarColor(Color3B::YELLOW);
    uiScrollView->setScrollBarAutoHideEnabled(false);
    uiScrollView->setBounceEnabled(false);
    uiScrollView->setLayoutType(ui::Layout::Type::VERTICAL);
    uiScrollView->setName("UI_SCROLL");
    layout->addChild(uiScrollView);
}

cocos2d::ui::Layout* UILayoutCashGroup::getUiGroup(int group, double width)
{
    std::string pathBG = "";
    std::string pathBox = "";
    
    std::string nameKey = "";
    switch (group) {
           
        case 1://
            pathBG = "Assets/ui/purchase/shop_box1_1.png";
            pathBox = "Assets/ui/purchase/shop_box1_2.png";
            
            nameKey = "t_ui_shop_group_name_4";
            break;
            
        case 4://
            pathBG = "Assets/ui/purchase/shop_box3_1.png";
            pathBox = "Assets/ui/purchase/shop_box3_3.png";
            
            nameKey = "t_ui_shop_group_name_4";
            break;
            
        case 5://
            pathBG = "Assets/ui/purchase/shop_box4_1.png";
            pathBox = "Assets/ui/purchase/shop_box4_2.png";
            
            nameKey = "t_ui_shop_group_name_5";
            break;
            
        case 6://
            pathBG = "Assets/ui/purchase/shop_box5_1.png";
            pathBox = "Assets/ui/purchase/shop_box5_2.png";
            
            nameKey = "t_ui_shop_group_name_6";
            break;
            
        case 7:
            pathBG = "Assets/ui/purchase/shop_box6_1.png";
            pathBox = "Assets/ui/purchase/shop_box6_2.png";
            
            nameKey = "t_ui_shop_group_name_7";
            break;
            
        default:
            break;
    }
    
    //
    auto layoutInner = ui::Layout::create();
    
    auto listProduct = CashManager::getInstance()->getProductGroup(group);
    for ( auto& product : listProduct )
    {
        auto uiBox = UICashBox::create(product->getIdx(), pathBox);
        uiBox->setTouchEnabled(true);
        uiBox->addClickEventListener(CC_CALLBACK_1(UILayoutCashGroup::onClickProduct, this));
        uiBox->setTag(product->getIdx());
        layoutInner->addChild(uiBox);
    }
    UtilsWidget::getInstance()->sortWidgetGrid(layoutInner, 2, width, 0, 40, false, true);
    
    //
    auto size = Size(width, 180 + layoutInner->getContentSize().height);
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBG->setPositionPercent(Vec2::ANCHOR_MIDDLE_TOP);
    layout->addChild(uiBG);
    
    // name
    auto uiName = ui::CText::create(GAME_TEXT(nameKey), GAME_FONT, 56);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiName->setPosition(Vec2(size.width - 50, size.height - 79.5));
    uiName->setTextAreaSize(Size(400, uiName->getContentSize().height));
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    layout->addChild(uiName);
    
    //
    layout->addChild(layoutInner);
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

cocos2d::ui::Layout* UILayoutCashGroup::getUiGroup5(int group, double width)
{
    auto layout = getUiGroup(group, width);
    auto size = layout->getContentSize();
   
    // desc
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_shop_daily_info"), GAME_FONT, 20);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiDesc->setPosition(Vec2(size.width - 50, size.height - 180));
    uiDesc->setTextAreaSize(Size(400, uiDesc->getContentSize().height));
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc->setColor(Color3B::YELLOW);
    uiDesc->setLineSpacing(-10);
    layout->addChild(uiDesc);
     
    return layout;
}

cocos2d::ui::Layout* UILayoutCashGroup::getUiGroup100(int group, double width)
{
    std::string pathBG = "Assets/ui/purchase/shop_box2_1.png";
    std::string pathBoxEnable = "Assets/ui/purchase/shop_box2_3.png";
    std::string pathBoxDisable = "Assets/ui/purchase/shop_box2_2.png";
    
    std::string nameKey = "t_ui_shop_group_name_100";
    
    //
    auto layoutInner = ui::Layout::create();
    
    int idxPrev = 0;
    
    auto listProduct = CashManager::getInstance()->getProductGroup(group);
    for ( auto& product : listProduct )
    {
        auto uiBox = UICashBoxPhase::create(product->getIdx(), idxPrev, pathBoxEnable, pathBoxDisable);
        uiBox->setTouchEnabled(true);
        uiBox->addClickEventListener(CC_CALLBACK_1(UILayoutCashGroup::onClickProduct, this));
        uiBox->setTag(product->getIdx());
        layoutInner->addChild(uiBox);
        
        idxPrev = product->getIdx();
    }
    UtilsWidget::getInstance()->sortWidgetGrid(layoutInner, 1, width, 0, 10, false, true);
    
    //
    auto size = Size(width, 302 + layoutInner->getContentSize().height);
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBG->setPositionPercent(Vec2::ANCHOR_MIDDLE_TOP);
    layout->addChild(uiBG);
    
    // name
    auto uiName = ui::CText::create(GAME_TEXT(nameKey), GAME_FONT, 56);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiName->setPosition(Vec2(size.width - 50, size.height - 79.5));
    uiName->setTextAreaSize(Size(400, uiName->getContentSize().height));
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    layout->addChild(uiName);
    
    auto uiDesc1 = ui::CText::create(GAME_TEXT("t_ui_shop_msg_5"), GAME_FONT, 28);
    uiDesc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc1->setPosition(Vec2(20, size.height - 159 - 52));
    uiDesc1->setTextAreaSize(Size(size.width - 40, uiDesc1->getContentSize().height));
    uiDesc1->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc1->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiDesc1->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc1->setColor(Color3B::YELLOW);
    uiDesc1->setLineSpacing(-10);
    layout->addChild(uiDesc1);
    
    auto uiDesc2 = ui::CText::create(GAME_TEXT("t_ui_shop_msg_4"), GAME_FONT, 20);
    uiDesc2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc2->setPosition(Vec2(20, size.height - 159 - 104 - 19.5));
    uiDesc2->setTextAreaSize(Size(size.width - 40, uiDesc2->getContentSize().height));
    uiDesc2->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc2->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiDesc2->setTextOverflow(Label::Overflow::SHRINK);
    uiDesc2->setColor(Color3B::BLACK);
    layout->addChild(uiDesc2);
    
    //
    layout->addChild(layoutInner);
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

#pragma mark - draw
void UILayoutCashGroup::drawInfo()
{
    auto layout = _uiContainerInfo;
    
    auto uiScrollView = layout->getChildByName<ui::ScrollView*>("UI_SCROLL");
    if ( uiScrollView != nullptr )
    {
        auto offset = uiScrollView->getInnerContainerPosition();
        
        bool first = false;
        if ( uiScrollView->getChildrenCount() <= 0 )
        {
            first = true;
        }
        
        //
        uiScrollView->removeAllChildren();
        
        //
        double width = uiScrollView->getInnerContainerSize().width;
        for ( int i = 0; i < _listGroup.size(); i++ )
        {
            int group = _listGroup.at(i);
            if ( group == 100 )
            {
                uiScrollView->addChild(getUiGroup100(group, width));
            }
            else if ( group == 5 )
            {
                uiScrollView->addChild(getUiGroup5(group, width));
            }
            else
            {
                uiScrollView->addChild(getUiGroup(group, width));
            }
        }
        
        //
        auto sizeInner = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiScrollView->getInnerContainer());
        uiScrollView->setInnerContainerSize(sizeInner);
        uiScrollView->getInnerContainer()->requestDoLayout();
        
        if ( first == true )
        {
            offset.y = (sizeInner.height - uiScrollView->getContentSize().height) * -1;
        }
        uiScrollView->setInnerContainerPosition(offset);
    }
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
void UILayoutCashGroup::onClickProduct(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    int idx = uiLayout->getTag();
    
    auto objProduct = CashManager::getInstance()->getProduct(idx);
    if ( objProduct == nullptr )
    {
        return;
    }
    
    //
    if ( objProduct->getSaleLimit() > 0 && CashManager::getInstance()->getPurchaseRemaining(idx) <= 0 )
    {
        return;
    }
    
    if ( objProduct->getShopType() == E_PRODUCT::DAILY && CashManager::getInstance()->getDailyDate(idx) > 0 && CashManager::getInstance()->isDailyReceive(idx) == true )
    {
        onPurchaseDailyReceive(idx);
    }
    else
    {
        onPurchase(idx);
    }
}

#pragma mark - game
void UILayoutCashGroup::onDraw()
{
    if ( _bInitUI == true )
    {
        drawInfo();
    }
}
