//
//  PopupPurchaseGrowth.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupPurchaseGrowth.h"

#include "GameObject/InfoProductGrowth.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupPurchaseGrowth* PopupPurchaseGrowth::create(std::string type)
{
    PopupPurchaseGrowth* pRet = new(std::nothrow) PopupPurchaseGrowth();
    if ( pRet && pRet->init(type) )
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

PopupPurchaseGrowth::PopupPurchaseGrowth(void) :
_type(""),
_level(0),
_idxCash(0),
_bBuy(false),

_objProduct(nullptr),

// widget
_uiContentsUIInfo(nullptr),
_uiContentsUIContentCash(nullptr),
_uiContentsUIContentList(nullptr),
_uiInfoBG(nullptr),
_uiList(nullptr)
{
    
}

PopupPurchaseGrowth::~PopupPurchaseGrowth(void)
{
    if ( _objProduct != nullptr )
    {
        _objProduct->release();
        _objProduct = nullptr;
    }
}

bool PopupPurchaseGrowth::init(std::string type)
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    //
    _type = type;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupPurchaseGrowth::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(PopupPurchaseGrowth::callbackInfo, this));
}

#pragma mark - init
void PopupPurchaseGrowth::initVar()
{
    //
    _level = CashManager::getInstance()->getGrowthNowLevel(_type);
    _levelNow = _level;
    _levelMax = CashManager::getInstance()->getGrowthMaxLevel(_type);
    _idxCash = CashManager::getInstance()->getGrowthToIdxCash(_type, _levelNow);
}

void PopupPurchaseGrowth::initUi()
{
    // size
    Size size = Size(696, 930);
    
    //
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_1.png", Rect::ZERO, Rect(77,23,2,2));
    _spriteContainer->setContentSize(size);
    _spriteContainer->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 13.5);
    addChild(_spriteContainer);
    
    {
        double widthUI = 710;
        double widthUICash = 628;
        
        double heightUIInfo = 400;
        double heightUICash = 200;
        double heightUIList = size.height - heightUIInfo - 20 - 25;

        _uiContentsUIInfo = ui::Layout::create();
        _uiContentsUIInfo->setContentSize(Size(widthUI, heightUIInfo));
        _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiContentsUIInfo->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height));
        _spriteContainer->addChild(_uiContentsUIInfo);
        
        _uiContentsUIContentCash = ui::Layout::create();
        _uiContentsUIContentCash->setContentSize(Size(widthUICash, heightUICash));
        _uiContentsUIContentCash->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIContentCash->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIContentCash->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, heightUIList + 20));
        _spriteContainer->addChild(_uiContentsUIContentCash);
        
        _uiContentsUIContentList = ui::Layout::create();
        _uiContentsUIContentList->setContentSize(Size(widthUI, heightUIList));
        _uiContentsUIContentList->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIContentList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIContentList->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
        _spriteContainer->addChild(_uiContentsUIContentList);
    }
    
    //
    uiInfo();
    uiContentCash();
    uiContentList();
}

#pragma mark - override DelegateListView
void PopupPurchaseGrowth::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size PopupPurchaseGrowth::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 90;

    return Size(width, height);
}

void PopupPurchaseGrowth::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto obj = _listGrowth.at(idx);
    
    bool bRewardNormal = obj->isRewardFree();
    bool bRewardPass = obj->isRewardCash();
    
    bool bCondition = isCondition(obj);
    bool bLock = false;
    if ( bRewardNormal == true && bRewardPass == true )
    {
        bLock = true;
    }
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);

     
    // bg
    std::string pathBG = "Assets/ui/purchase/shop_box8_6.png";
    if ( bLock == true )
    {
        pathBG = "Assets/ui/purchase/shop_box8_8.png";
    }
    else if ( bCondition == false )
    {
        pathBG = "Assets/ui/purchase/shop_box8_7.png";
    }
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
    
    // icon
    std::string pathIcon = MafUtils::format("Assets/ui/purchase/shop_box_no1_%d.png", obj->getIdx());
    if ( bCondition == false )
    {
        pathIcon = MafUtils::format("Assets/ui/purchase/shop_box_no2_%d.png", obj->getIdx());
    }
    auto uiIcon = ui::ImageView::create(pathIcon);
    uiIcon->setPosition(Vec2(30, uiBG->getContentSize().height / 2));
    uiBG->addChild(uiIcon);
    
    // title
    auto uiTitle = ui::CText::create(getGrowthName(obj), GAME_FONT, 24);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiTitle->setPosition(Vec2(80, uiBG->getContentSize().height / 2));
    uiTitle->setTextAreaSize(Size(130, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextColor(Color4B(146, 146, 146, 255));
    uiBG->addChild(uiTitle);
    if ( bLock == false && bCondition == true )
    {
        uiTitle->setTextColor(Color4B(COLOR_TITLE));;
    }
            
    
    // item buy
    auto uiBtnGet = ui::Button::create("Assets/ui_common/btn_1_3_on.png");
    uiBtnGet->setScale9Enabled(true);
    uiBtnGet->setContentSize(Size(125, 70));
    uiBtnGet->setPosition(Vec2(uiBG->getContentSize().width - uiBtnGet->getContentSize().width / 2 - 12, uiBG->getContentSize().height / 2));
    uiBtnGet->addClickEventListener(CC_CALLBACK_1(PopupPurchaseGrowth::onClickGet, this));
    uiBtnGet->setTag(obj->getIdx());
    uiBG->addChild(uiBtnGet);
    {
        //
        std::string strMessage = GAME_TEXT("t_ui_shop_bonus_msg_4");
        if ( _bBuy == true && bLock == false && bRewardNormal == true )
        {
            strMessage = GAME_TEXT("t_ui_shop_bonus_msg_8");
            uiBtnGet->loadTextureNormal("Assets/ui_common/btn_1_6_on.png");
            
            //
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.13f);
            int listAnimationIdx[] = {1,2,3,4,3,2,1};
            for ( int i = 0; i < sizeof(listAnimationIdx)/sizeof(int); i++ )
            {
                animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui_common/ef_btn_%d.png", listAnimationIdx[i]));
            }
            
            auto uiImgEffect = ui::ImageView::create("Assets/ui_common/ef_btn_1.png");
            uiImgEffect->setPosition(Vec2(uiBtnGet->getContentSize().width / 2, uiBtnGet->getContentSize().height / 2));
            uiImgEffect->setScaleX(1.5);
            uiBtnGet->addChild(uiImgEffect);

            auto action1 = Show::create();
            auto action2 = Animate::create(animation);
            auto action3 = Hide::create();
            auto action4 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, action3, action4, NULL);
            ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(seq));
        }

        auto uiGet = ui::CText::create(strMessage, GAME_FONT, 24);
        uiGet->setPosition(Vec2(uiBtnGet->getContentSize().width / 2, uiBtnGet->getContentSize().height / 2));
        uiGet->setTextAreaSize(Size(uiBtnGet->getContentSize().width * 0.9, uiGet->getContentSize().height));
        uiGet->setTextOverflow(Label::Overflow::SHRINK);
        uiGet->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnGet->addChild(uiGet);
        
        if ( bLock == true || bCondition == false )
        {
            uiBtnGet->loadTextureNormal("Assets/ui_common/btn_1_off.png");
        }
        
        if ( bLock == true )
        {
            auto uiReceive = Sprite::create("Assets/ui/purchase/icon_complete_1.png");
            uiReceive->setPosition(Vec2(uiBtnGet->getContentSize().width / 2, uiBtnGet->getContentSize().height / 2));
            uiBtnGet->addChild(uiReceive);
        }
    }

    
     
    
    //
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setContentSize(Size(170, 18));
    uiLayoutReward->setIgnoreAnchorPointForPosition(false);
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiLayoutReward->setPosition(Vec2(230, uiBG->getContentSize().height / 2));
    uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiBG->addChild(uiLayoutReward);
    {
        // reward free
        auto listItemsFree = obj->getItemsFree();
        for ( int i = 0; i < listItemsFree.size(); i++ )
        {
            auto objItem = listItemsFree.at(i);
            
            // item bg
            auto uiItemBG = ui::ImageView::create("Assets/ui/purchase/shop_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(45.6, 45.6));
            uiLayoutReward->addChild(uiItemBG);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                params->setMargin(ui::Margin(10, 0, 10, 0));
                uiItemBG->setLayoutParameter(params);
            }

            // item image
            auto uiItemIcon = ui::ImageView::create(objItem->getPath());
            uiItemIcon->setPositionType(ui::Widget::PositionType::PERCENT);
            uiItemIcon->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiItemIcon->setScale(uiItemBG->getContentSize().width / uiItemIcon->getContentSize().width);
            uiItemBG->addChild(uiItemIcon);
         
            // item free bg
            auto uiItemTopBG = ui::ImageView::create("Assets/ui/purchase/shop_item_free.png");
            uiItemTopBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiItemTopBG->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height));
            uiItemTopBG->setScale(0.8);
            uiItemBG->addChild(uiItemTopBG);
            {
                auto uiFree = ui::CText::create("FREE", GAME_FONT_AL, 18);
                uiFree->setPosition(Vec2(uiItemTopBG->getContentSize().width / 2, uiItemTopBG->getContentSize().height / 2));
                uiItemTopBG->addChild(uiFree);
            }

            // item count
            auto uiCount = ui::CText::create(objItem->getCount(), GAME_FONT, 14);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 4));
            uiItemBG->addChild(uiCount);

            if ( bRewardNormal == true )
            {
                auto uiBlack = ui::Layout::create();
                uiBlack->setContentSize(uiItemBG->getContentSize());
                uiBlack->setBackGroundColor(Color3B::BLACK);
                uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
                uiBlack->setOpacity(200);
                uiItemBG->addChild(uiBlack);

                auto uiBlackCheck = ui::ImageView::create("Assets/ui/purchase/shop_box_check.png");
                uiBlackCheck->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiBlackCheck);
            }
        }
        
        // icon
        auto uiPlus = ui::ImageView::create("Assets/ui/purchase/shop_item_plus.png");
        uiLayoutReward->addChild(uiPlus);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
            params->setMargin(ui::Margin(10, 0, 10, 0));
            uiPlus->setLayoutParameter(params);
        }
        
        // reward cash
        auto listItemsCash = obj->getItemsCash();
        for ( int i = 0; i < listItemsCash.size(); i++ )
        {
            auto objItem = listItemsCash.at(i);
            
            // item bg
            auto uiItemBG = ui::ImageView::create("Assets/ui/purchase/shop_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(45.6, 45.6));
            uiLayoutReward->addChild(uiItemBG);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                params->setMargin(ui::Margin(10, 0, 10, 0));
                uiItemBG->setLayoutParameter(params);
            }
            
            // item image
            auto uiItemIcon = ui::ImageView::create(objItem->getPath());
            uiItemIcon->setPositionType(ui::Widget::PositionType::PERCENT);
            uiItemIcon->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiItemIcon->setScale(uiItemBG->getContentSize().width / uiItemIcon->getContentSize().width);
            uiItemBG->addChild(uiItemIcon);
            
            // item effect
            if ( obj->isRewardCash() == false )
            {
                //
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.13f);
                for ( int i = 1; i <= 4; i++ )
                {
                    animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/purchase/shop_item_ef_%d.png", i));
                }
                
                auto uiImgEffect = ui::ImageView::create("Assets/ui/purchase/shop_item_ef_1.png");
                uiImgEffect->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiImgEffect->setScale(0.8);
                ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(Animate::create(animation)));
                uiItemBG->addChild(uiImgEffect);
            }
            
            // item count
            auto uiCount = ui::CText::create(objItem->getCount(), GAME_FONT, 14);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 4));
            uiItemBG->addChild(uiCount);

            // item lock
            if ( _bBuy == false )
            {
                auto uiLock = ui::ImageView::create("Assets/ui/purchase/shop_item_lock.png");
                uiLock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                uiLock->setPosition(Vec2(uiItemBG->getContentSize().width - 5, 5));
                uiItemBG->addChild(uiLock);
            }
            
            if ( bRewardPass == true )
            {
                auto uiBlack = ui::Layout::create();
                uiBlack->setContentSize(uiItemBG->getContentSize());
                uiBlack->setBackGroundColor(Color3B::BLACK);
                uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
                uiBlack->setOpacity(200);
                uiItemBG->addChild(uiBlack);

                auto uiBlackCheck = ui::ImageView::create("Assets/ui/purchase/shop_box_check.png");
                uiBlackCheck->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiBlackCheck);
            }
        }
    }
    
    
}

int64_t PopupPurchaseGrowth::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listGrowth.size();
   
    return count;
}

#pragma mark - ui
void PopupPurchaseGrowth::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    auto size = _uiContentsUIInfo->getContentSize();
    
    //
    _uiInfoBG = ui::ImageView::create("Assets/ui/purchase/shop_bg_1.png");
    _uiInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiInfoBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiInfoBG);
    
    // item close
    auto uiBtnClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiBtnClose->setPosition(Vec2(size.width - uiBtnClose->getContentSize().width, size.height - 37.5));
    uiBtnClose->addClickEventListener(CC_CALLBACK_1(PopupPurchaseGrowth::onClickClose, this));
    layout->addChild(uiBtnClose);
    
    //
    auto uiInfoTitleBG = ui::ImageView::create("Assets/ui_common/popup_mini_title.png");
    uiInfoTitleBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiInfoTitleBG);
    {
        std::string strTitle = "t_cash_title_402";
        if ( _type.compare("PRISON_FLOOR") == 0 )
        {
            strTitle = "t_cash_title_403";
        }
        
        auto uiInfoTitle = ui::CText::create(GAME_TEXT(strTitle), GAME_FONT, 31);
        uiInfoTitle->setPositionType(ui::Widget::PositionType::PERCENT);
        uiInfoTitle->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiInfoTitle->setTextAreaSize(Size(350, uiInfoTitle->getContentSize().height));
        uiInfoTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiInfoTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiInfoTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiInfoTitle->setTextColor(Color4B(COLOR_TITLE));
        uiInfoTitleBG->addChild(uiInfoTitle);
    }
    
    auto uiInfoGuideBG = ui::ImageView::create("Assets/ui_common/base_text_1_2.png");
    uiInfoGuideBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoGuideBG->setPosition(Vec2(size.width / 2, size.height - 30));
    layout->addChild(uiInfoGuideBG);
    {
        auto uiInfoGuide = ui::CText::create(GAME_TEXT("t_ui_shop_growth_402"), GAME_FONT, 18);
        uiInfoGuide->setPositionType(ui::Widget::PositionType::PERCENT);
        uiInfoGuide->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiInfoGuide->setTextAreaSize(Size(uiInfoGuideBG->getContentSize().width, uiInfoGuide->getContentSize().height));
        uiInfoGuide->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiInfoGuide->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiInfoGuideBG->addChild(uiInfoGuide);
    }
    
    // move
    {
        std::string strMovePath = "Assets/ui/bg_icon/btn_prison.png";
        std::string strMoveTitle = "t_cash_title_simple_403";
        if ( _type.compare("PRISON_FLOOR") == 0 )
        {
            strMovePath = "Assets/ui/bg_icon/btn_normal.png";
            strMoveTitle = "t_cash_title_simple_402";
        }
        auto uiBtnMove = ui::Button::create(strMovePath);
        uiBtnMove->setPosition(Vec2(80, size.height - 60));
        uiBtnMove->addClickEventListener(CC_CALLBACK_1(PopupPurchaseGrowth::onClickMove, this));
        layout->addChild(uiBtnMove);
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.13f);
            for ( int i = 1; i <= 3; i++ )
            {
                animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/bg_icon/ef_btn_package_%d.png", i));
            }
            
            auto uiImgEffect = ui::ImageView::create("Assets/ui/bg_icon/ef_btn_package_1.png");
            uiImgEffect->setPosition(Vec2(uiBtnMove->getContentSize().width / 2, uiBtnMove->getContentSize().height / 2));
            uiBtnMove->addChild(uiImgEffect);

            auto action1 = Show::create();
            auto action2 = Animate::create(animation);
            auto action3 = Hide::create();
            auto action4 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, action3, action4, NULL);
            ((ui::Scale9Sprite*)uiImgEffect->getVirtualRenderer())->runAction(RepeatForever::create(seq));
            
            //
            auto uiTextName = ui::CText::create(GAME_TEXT(strMoveTitle), GAME_FONT, 16);
            uiTextName->setPosition(Vec2(uiBtnMove->getContentSize().width / 2, 0));
            uiTextName->enableOutline(Color4B::BLACK, 1);
            uiTextName->setScale(MIN(1, uiBtnMove->getContentSize().width / uiTextName->getContentSize().width));
            uiBtnMove->addChild(uiTextName);
        }
    }
    
    // draw
    drawInfo();
}

void PopupPurchaseGrowth::uiContentCash()
{
    auto layout = _uiContentsUIContentCash;
    auto size = _uiContentsUIContentCash->getContentSize();
    
    //
    auto uiLayoutCash = ui::Layout::create();
    uiLayoutCash->setContentSize(Size(size.width, 145));
    uiLayoutCash->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiLayoutCash->setPosition(Vec2(size.width / 2, 0));
    uiLayoutCash->setName("UI_LAYOUT_CASH");
    layout->addChild(uiLayoutCash, 1);
    
    auto uiCashBG = ui::ImageView::create("Assets/ui_common/popup_mini_scale2_3.png");
    uiCashBG->setScale9Enabled(true);
    uiCashBG->setContentSize(uiLayoutCash->getContentSize());
    uiCashBG->setAnchorPoint(uiLayoutCash->getAnchorPoint());
    uiCashBG->setPosition(uiLayoutCash->getPosition());
    layout->addChild(uiCashBG);
    
    //
    auto uiLevelBG = ui::ImageView::create("Assets/ui_common/base_text_2_2.png");
    uiLevelBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiLevelBG->setPosition(Vec2(size.width, 150));
    layout->addChild(uiLevelBG);
    {
        _uiInfoLevel = ui::CText::create(MafUtils::format("Lv.%d", _level), GAME_FONT, 28);
        _uiInfoLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _uiInfoLevel->setPosition(Vec2(uiLevelBG->getContentSize().width - 20, uiLevelBG->getContentSize().height / 2));
        _uiInfoLevel->setTextColor(Color4B(201, 173, 109, 255));
        uiLevelBG->addChild(_uiInfoLevel);
    }
    
    // level buttons
    auto uiLayoutButtons = ui::Layout::create();
    uiLayoutButtons->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiLayoutButtons->setPosition(Vec2(0, 150));
    uiLayoutButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayoutButtons->setName("UI_LAYOUT_BUTTONS");
    layout->addChild(uiLayoutButtons);
    {
        for ( int i = 1; i <= _levelMax; i++ )
        {
            auto uiBtn = ui::Button::create("Assets/ui_common/btn_2_1_off.png");
            uiBtn->setScale9Enabled(true);
            uiBtn->setContentSize(Size(78, 62));
            uiBtn->addClickEventListener(CC_CALLBACK_1(PopupPurchaseGrowth::onClickLevel, this));
            uiBtn->setTag(i);
            uiLayoutButtons->addChild(uiBtn);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
                params->setMargin(ui::Margin(0, 0, 10, 0));
                uiBtn->setLayoutParameter(params);
                
                auto uiNumber = ui::ImageView::create(MafUtils::format("Assets/ui/adventure/adventure_no%d_2.png", i));
                uiNumber->setPositionType(ui::Widget::PositionType::PERCENT);
                uiNumber->setPositionPercent(Vec2::ANCHOR_MIDDLE);
                uiNumber->setName("UI_IMG_NUMBER");
                uiBtn->addChild(uiNumber);
                
                auto uiBoard = ui::ImageView::create("Assets/ui_common/btn_2_7_on.png");
                uiBoard->setScale9Enabled(true);
                uiBoard->setContentSize(Size(78, 62));
                uiBoard->setPositionType(ui::Widget::PositionType::PERCENT);
                uiBoard->setPositionPercent(Vec2::ANCHOR_MIDDLE);
                uiBoard->setVisible(false);
                uiBoard->setName("UI_IMG_BOARD");
                uiBtn->addChild(uiBoard);
                
                
            }
        }
        
        //
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutButtons);
        uiLayoutButtons->setContentSize(size);
        uiLayoutButtons->requestDoLayout();
    }
    
    //
    drawContentCash();
    
}

void PopupPurchaseGrowth::uiContentList()
{
    auto layout = _uiContentsUIContentList;
    auto size = _uiContentsUIContentList->getContentSize();
    
    //
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ZERO);
    _uiList->setPosition(Vec2(0, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - ui draw
void PopupPurchaseGrowth::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    
        std::string pathBG = "";
    if ( _type.compare("FLOOR") == 0 )
    {
        switch (_levelNow) {
            case 1: pathBG = "Assets/ui/purchase/shop_bg_1.png"; break;
            case 2: pathBG = "Assets/ui/purchase/shop_bg_5.png"; break;
            case 3: pathBG = "Assets/ui/purchase/shop_bg_7.png"; break;
            case 4: pathBG = "Assets/ui/purchase/shop_bg_9.png"; break;
                
            default:
                break;
        }
    }
    else
    {
        switch (_levelNow) {
            case 1: pathBG = "Assets/ui/purchase/shop_bg_2.png"; break;
            case 2: pathBG = "Assets/ui/purchase/shop_bg_4.png"; break;
            case 3: pathBG = "Assets/ui/purchase/shop_bg_6.png"; break;
            case 4: pathBG = "Assets/ui/purchase/shop_bg_8.png"; break;
                
            default:
                break;
        }
    }
    _uiInfoBG->loadTexture(pathBG);
}

void PopupPurchaseGrowth::drawContentCash()
{
    auto layout = _uiContentsUIContentCash;
    
    //
    auto uiLayoutCash = layout->getChildByName<ui::Layout*>("UI_LAYOUT_CASH");
    uiLayoutCash->removeAllChildren();
    
    if ( _objProduct != nullptr )
    {
        // 즉시
        auto uiGetBG = ui::ImageView::create("Assets/ui_common/base_box_1_10.png");
        uiGetBG->setScale9Enabled(true);
        uiGetBG->setContentSize(Size(170, 112));
        uiGetBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiGetBG->setPosition(Vec2(210 - 10, uiLayoutCash->getContentSize().height / 2));
        uiLayoutCash->addChild(uiGetBG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_cash_passfree"), GAME_FONT, 18);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiText->setPosition(Vec2(uiGetBG->getContentSize().width / 2, uiGetBG->getContentSize().height - 4));
            uiText->setTextAreaSize(Size(uiGetBG->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiGetBG->addChild(uiText);
            
            auto uiLayoutItems = ui::Layout::create();
            uiLayoutItems->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiLayoutItems->setLayoutType(ui::Layout::Type::HORIZONTAL);
            uiGetBG->addChild(uiLayoutItems);
            
            bool bFirst = true;
            auto listItems = _objProduct->getItems();
            for ( auto& obj : listItems )
            {
                auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
                uiLayoutItems->addChild(uiItemBG);
                {
                    if ( bFirst == false )
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setMargin(ui::Margin(10, 0, 0, 0));
                        uiItemBG->setLayoutParameter(params);
                    }
                    bFirst = false;
                    
                    // item image
                    auto uiItemIcon = ui::ImageView::create(obj->getPath());
                    uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                    uiItemIcon->setScale(uiItemBG->getContentSize().width / uiItemIcon->getContentSize().width);
                    uiItemBG->addChild(uiItemIcon);
                    
                    // item count
                    auto uiCount = ui::CText::create(obj->getCount(), GAME_FONT, 14);
                    uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    uiCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 4));
                    uiItemBG->addChild(uiCount);
                }
            }
 
            //
            auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutItems);
            uiLayoutItems->setContentSize(size);
            uiLayoutItems->setPosition(Vec2(uiGetBG->getContentSize().width / 2, uiGetBG->getContentSize().height / 2 - 10));
            uiLayoutItems->requestDoLayout();
        }
         
        //
        auto uiPlus = ui::ImageView::create("Assets/ui_common/plus_2.png");
        uiPlus->setPosition(Vec2(210, uiLayoutCash->getContentSize().height / 2));
        uiLayoutCash->addChild(uiPlus);
        
        // 누적
        auto uiGetTotalBG = ui::ImageView::create("Assets/ui_common/base_box_1_8.png");
        uiGetTotalBG->setScale9Enabled(true);
        uiGetTotalBG->setContentSize(Size(190, 112));
        uiGetTotalBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiGetTotalBG->setPosition(Vec2(210 + 10, uiLayoutCash->getContentSize().height / 2));
        uiLayoutCash->addChild(uiGetTotalBG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_cash_passpurchase"), GAME_FONT, 18);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiText->setPosition(Vec2(uiGetTotalBG->getContentSize().width / 2, uiGetTotalBG->getContentSize().height - 4));
            uiGetTotalBG->addChild(uiText);
            
            auto uiLayoutItems = ui::Layout::create();
            uiLayoutItems->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiLayoutItems->setLayoutType(ui::Layout::Type::HORIZONTAL);
            uiGetTotalBG->addChild(uiLayoutItems);
            
            bool bFirst = true;
            for ( auto& obj : _listProductItmes )
            {
                auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item_2_3.png");
                uiLayoutItems->addChild(uiItemBG);
                {
                    if ( bFirst == false )
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setMargin(ui::Margin(10, 0, 0, 0));
                        uiItemBG->setLayoutParameter(params);
                    }
                    bFirst = false;
                    
                    // item image
                    auto uiItemIcon = ui::ImageView::create(obj->getPath());
                    uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                    uiItemIcon->setScale(uiItemBG->getContentSize().width / uiItemIcon->getContentSize().width);
                    uiItemBG->addChild(uiItemIcon);
                    
                    // item count
                    auto uiCount = ui::CText::create(obj->getCount(), GAME_FONT, 14);
                    uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    uiCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 4));
                    uiItemBG->addChild(uiCount);
                }
            }
            
            //
            auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutItems);
            uiLayoutItems->setContentSize(size);
            uiLayoutItems->setPosition(Vec2(uiGetTotalBG->getContentSize().width / 2, uiGetTotalBG->getContentSize().height / 2 - 10));
            uiLayoutItems->requestDoLayout();
        }
        
        // 버튼
        auto uiBuy = ui::Button::create("Assets/ui_common/quest_btn_on.png");
        uiBuy->setPosition(Vec2(uiLayoutCash->getContentSize().width - 110, uiLayoutCash->getContentSize().height / 2));
        uiBuy->addClickEventListener(CC_CALLBACK_1(PopupPurchaseGrowth::onClickBuy, this));
        uiLayoutCash->addChild(uiBuy);
        {
            auto uiPrice = ui::CText::create(_objProduct->getPrice(), SYSTEM_FONT, 24);
            uiPrice->setPosition(Vec2(uiBuy->getContentSize().width / 2, uiBuy->getContentSize().height / 2));
            uiBuy->addChild(uiPrice);
            
            std::string strPrice = _objProduct->getPrice();
            if ( strPrice.length() == 0 )
            {
                uiPrice->setString(GAME_TEXT("t_ui_shop_price_loading"));

                auto action1 = CallFuncN::create( [=]( Ref* sender ) {

                    auto obj = CashManager::getInstance()->getProduct(_objProduct->getIdx());
                    std::string strProductID = obj->getProductIdReal();
                    std::string strPrice = obj->getPrice();

                    if ( strPrice.length() != 0 )
                    {
                        ((ui::CText*)sender)->setString(strPrice);
                        ((ui::CText*)sender)->stopAllActions();
                        return;
                    }
                });
                auto action2 = DelayTime::create(5.0);
                auto seq = Sequence::create(action1, action2, nullptr);
                uiPrice->runAction( RepeatForever::create(seq) );
            }
        }
        
        if ( _bBuy == true )
        {
            //
            uiBuy->setEnabled(false);
        }
    }
    
    
    //
    _uiInfoLevel->setString(MafUtils::format("Lv.%d", _level));
    
    //
    auto uiLayoutButtons = layout->getChildByName<ui::Layout*>("UI_LAYOUT_BUTTONS");
    for ( auto& child : uiLayoutButtons->getChildren() )
    {
        auto uiBtn = dynamic_cast<ui::Button*>(child);
        auto uiNumber = uiBtn->getChildByName<ui::ImageView*>("UI_IMG_NUMBER");
        auto uiBoard = uiBtn->getChildByName<ui::ImageView*>("UI_IMG_BOARD");
        
        int level = uiBtn->getTag();
        if ( level == _levelNow )
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_2_4_on.png");
            uiNumber->loadTexture(MafUtils::format("Assets/ui/adventure/adventure_no%d_1.png", level));
            uiBoard->setVisible(true);
        }
        else if ( level <= _level )
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_2_4_on.png");
            uiNumber->loadTexture(MafUtils::format("Assets/ui/adventure/adventure_no%d_1.png", level));
            uiBoard->setVisible(false);
        }
        else
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_2_1_off.png");
            uiNumber->loadTexture(MafUtils::format("Assets/ui/adventure/adventure_no%d_2.png", level));
            uiBoard->setVisible(false);
        }
    }

}

void PopupPurchaseGrowth::drawContentList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - set, get
std::string PopupPurchaseGrowth::getGrowthName(InfoProductGrowth* objGrowth)
{
    std::string strResult = "";
    if ( objGrowth->getType().compare("FLOOR") == 0 )
    {
        strResult = GAME_TEXTFORMAT("t_ui_shop_bonus_msg_1", objGrowth->getCondition());
    }
    else if ( objGrowth->getType().compare("PRISON_FLOOR") == 0 )
    {
        strResult = GAME_TEXTFORMAT("t_ui_shop_bonus_msg_6", objGrowth->getCondition());
    }
    
    return strResult;
}

bool PopupPurchaseGrowth::isCondition(InfoProductGrowth* objGrowth)
{
    bool bResult = false;
    if ( objGrowth->getType().compare("FLOOR") == 0 )
    {
        if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= objGrowth->getCondition() )
        {
            bResult = true;
        }
    }
    else if ( objGrowth->getType().compare("PRISON_FLOOR") == 0 )
    {
        if ( SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER) >= objGrowth->getCondition() )
        {
            bResult = true;
        }
    }
    
    return bResult;
}

#pragma mark - click
void PopupPurchaseGrowth::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupPurchaseGrowth::onClickMove(cocos2d::Ref* sender)
{
    std::string type = "FLOOR";
    if ( _type.compare("FLOOR") == 0 )
    {
        type = "PRISON_FLOOR";
    }

    auto popup = PopupPurchaseGrowth::create(type);
    popup->show();

    hide();
}

void PopupPurchaseGrowth::onClickLevel(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;
    
    int level = uiButton->getTag();
    if ( level > _levelMax || level > _level )
    {
        return;
    }
    
    if ( level == _levelNow )
    {
        return;
    }
    
    //
    _levelNow = level;
    
    _level = CashManager::getInstance()->getGrowthNowLevel(_type);
    _levelMax = CashManager::getInstance()->getGrowthMaxLevel(_type);
    _idxCash = CashManager::getInstance()->getGrowthToIdxCash(_type, _levelNow);
    
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(PopupPurchaseGrowth::callbackInfo, this));
}

void PopupPurchaseGrowth::onClickBuy(cocos2d::Ref* sender)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    std::string type = _type;
    int levelPrev = _levelNow - 1;
    if ( levelPrev > 0 )
    {
        int idxCash = CashManager::getInstance()->getGrowthToIdxCash(type, levelPrev);
        
        int buyCount = CashManager::getInstance()->getHistoryProduct(idxCash);
        if ( buyCount <= 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_growth_pack_purchase_msg"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    
    
    
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(_idxCash, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(_idxCash, CC_CALLBACK_4(PopupPurchaseGrowth::callbackPurchase, this));
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
            PopupLoading::show();
            CashManager::getInstance()->purchaseInitiate(_idxCash, CC_CALLBACK_4(PopupPurchaseGrowth::callbackPurchase, this));
        }
    });
}

void PopupPurchaseGrowth::onClickGet(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;

    int idx = uiButton->getTag();
    auto objGrowth = CashManager::getInstance()->getGrowth(idx, _idxCash);
    
    bool bCondition = isCondition(objGrowth);
    if ( bCondition == false )
    {
        return;
    }
    
    if ( objGrowth->isRewardFree() == true && objGrowth->isRewardCash() == true )
    {
        return;
    }
    
    if ( objGrowth->isRewardFree() == true && objGrowth->isRewardCash() == false )
    {
        if ( _bBuy == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_shop_bonus_msg_7"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }

    //
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthReceive(_idxCash, idx, CC_CALLBACK_1(PopupPurchaseGrowth::callbackReceive, this));
}

#pragma mark - callback
void PopupPurchaseGrowth::callbackInfo(bool isResult, bool bBuy)
{
    //
    PopupLoading::hide();
    
    //
    if ( isResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    //
    _bBuy = bBuy;
    
    //
    _listGrowth = CashManager::getInstance()->getGrowthGroup(_type, _levelNow);
    
    //
    if ( _objProduct != nullptr )
    {
        _objProduct->release();
        _objProduct = nullptr;
    }
    _objProduct = CashManager::getInstance()->getProduct(_idxCash);
    _objProduct->retain();
    
    _listProductItmes.clear();
    for ( int i = 0; i < _listGrowth.size(); i++ )
    {
        auto objGrowth = _listGrowth.at(i);

        //
        auto listItemsFree = objGrowth->getItemsFree();
        for ( int j = 0; j < listItemsFree.size(); j++ )
        {
            auto objItem = listItemsFree.at(j);

            bool bHave = false;
            for ( auto& obj : _listProductItmes )
            {
                if ( obj->getIdx() == objItem->getIdx() )
                {
                    std::string count = obj->getCount();
                    std::string countPlus = objItem->getCount();
                    
                    count = MafUtils::bigAddNum(count, countPlus);
                    obj->setCount(count);
                    bHave = true;
                    break;
                }
            }
            if ( bHave == false )
            {
                auto objItemClone = objItem->clone();
                objItemClone->setCount(objItem->getCount());
                _listProductItmes.pushBack(objItemClone);
            }
        }
        
        //
        auto listItemsCash = objGrowth->getItemsCash();
        for ( int j = 0; j < listItemsCash.size(); j++ )
        {
            auto objItem = listItemsCash.at(j);

            bool bHave = false;
            for ( auto& obj : _listProductItmes )
            {
                if ( obj->getIdx() == objItem->getIdx() )
                {
                    std::string count = obj->getCount();
                    std::string countPlus = objItem->getCount();
                    
                    count = MafUtils::bigAddNum(count, countPlus);
                    obj->setCount(count);
                    bHave = true;
                    break;
                }
            }
            if ( bHave == false )
            {
                auto objItemClone = objItem->clone();
                objItemClone->setCount(objItem->getCount());
                _listProductItmes.pushBack(objItemClone);
            }
        }
    }
            
             
    //
    CashManager::getInstance()->addHistoryAds(_idxCash);
    
    // draw
    drawInfo();
    drawContentCash();
    drawContentList();
}

void PopupPurchaseGrowth::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(PopupPurchaseGrowth::callbackInfo, this));
        
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
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
}

void PopupPurchaseGrowth::callbackReceive(bool isResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( isResult == false )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            // 정보 다시 가져오기
            PopupLoading::show();
            CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(PopupPurchaseGrowth::callbackInfo, this));
        });
        popup->show();
        return;
    }
    
    // 정보 다시 가져오기
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(PopupPurchaseGrowth::callbackInfo, this));
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_shop_bonus_msg_3"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
}
