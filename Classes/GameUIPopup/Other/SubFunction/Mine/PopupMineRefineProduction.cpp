//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupMineRefineProduction.h"

#include "GameObject/Mineral/InfoMineralRefineUpgrade.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupMineRefineProduction* PopupMineRefineProduction::create()
{
    PopupMineRefineProduction* pRet = new(std::nothrow) PopupMineRefineProduction();
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

PopupMineRefineProduction::PopupMineRefineProduction(void) :
_bInitUI(false),

//
_uiContainerInfo(nullptr),
_uiContainerMine(nullptr),
_uiContainerButtons(nullptr)
{
    
}

PopupMineRefineProduction::~PopupMineRefineProduction(void)
{
    
}

bool PopupMineRefineProduction::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupMineRefineProduction");
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupMineRefineProduction::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiMine();
        uiButtons();
    }
    
    //
    drawInfo();
    drawMine();
    drawButtons();
}

#pragma mark - init
void PopupMineRefineProduction::initVar()
{
    
}

void PopupMineRefineProduction::initUi()
{
    Size size = Size(746, 1040);
    
    // size
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_mine_mk2_production_bt_1"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupMineRefineProduction::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    uiCurrency->setName("UI_CURRENCY");
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui
void PopupMineRefineProduction::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 28 - 28;
    size.height = size.height - 75 - 50 - 28;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 50));
    _spriteContainer->addChild(layout);
    
    _uiContainerButtons = ui::Layout::create();
    _uiContainerButtons->setContentSize(Size(size.width, 116));
    _uiContainerButtons->setIgnoreAnchorPointForPosition(false);
    _uiContainerButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContainerButtons->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContainerButtons);
    
    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, 300));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiContainerInfo);
    
    _uiContainerMine = ui::Layout::create();
    _uiContainerMine->setContentSize(Size(size.width, size.height - _uiContainerInfo->getContentSize().height - _uiContainerButtons->getContentSize().height));
    _uiContainerMine->setIgnoreAnchorPointForPosition(false);
    _uiContainerMine->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerMine->setPosition( Vec2(size.width / 2, size.height - _uiContainerInfo->getContentSize().height) );
    layout->addChild(_uiContainerMine);
}

void PopupMineRefineProduction::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(564, 136));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height - 25));
    layout->addChild(uiLayout);
    {
        auto uiIconBG = ui::ImageView::create("Assets/ui_common/box_item.png");
        uiIconBG->setScale9Enabled(true);
        uiIconBG->setContentSize(Size(136, 136));
        uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiIconBG->setPosition(Vec2(0, uiLayout->getContentSize().height / 2));
        uiLayout->addChild(uiIconBG);
        {
            auto uiIcon = ui::ImageView::create("Assets/ui/mine/mole_smith_01.png");
            uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
            uiIconBG->addChild(uiIcon);
        }
        
        double posY = uiLayout->getContentSize().height;
        
        auto uiType1BG = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
        uiType1BG->setScale9Enabled(true);
        uiType1BG->setCapInsets(Rect(10,10,2,2));
        uiType1BG->setContentSize(Size(423, 38));
        uiType1BG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiType1BG->setPosition(Vec2(uiLayout->getContentSize().width, posY));
        uiType1BG->setName("UI_TYPE_BG_1");
        uiLayout->addChild(uiType1BG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_production_msg_1"), GAME_FONT, 24);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiText->setPosition(Vec2(10, uiType1BG->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(250, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiType1BG->addChild(uiText);
            
            auto uiValue = ui::CText::create(" ", GAME_FONT, 24);
            uiValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiValue->setPosition(Vec2(uiType1BG->getContentSize().width - 50, uiType1BG->getContentSize().height / 2));
            uiValue->setTextAreaSize(Size(100, uiText->getContentSize().height));
            uiValue->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiValue->setTextHorizontalAlignment(TextHAlignment::RIGHT);
            uiValue->setTextOverflow(Label::Overflow::SHRINK);
            uiValue->setName("UI_TYPE_VALUE");
            uiType1BG->addChild(uiValue);
        }
        
        posY = posY - uiType1BG->getContentSize().height - 11;
        auto uiType2BG = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
        uiType2BG->setScale9Enabled(true);
        uiType2BG->setCapInsets(Rect(10,10,2,2));
        uiType2BG->setContentSize(Size(423, 38));
        uiType2BG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiType2BG->setPosition(Vec2(uiLayout->getContentSize().width, posY));
        uiType2BG->setName("UI_TYPE_BG_2");
        uiLayout->addChild(uiType2BG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_production_msg_2"), GAME_FONT, 24);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiText->setPosition(Vec2(10, uiType2BG->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(250, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiType2BG->addChild(uiText);
            
            auto uiValue = ui::CText::create(" ", GAME_FONT, 24);
            uiValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiValue->setPosition(Vec2(uiType2BG->getContentSize().width - 50, uiType2BG->getContentSize().height / 2));
            uiValue->setTextAreaSize(Size(100, uiText->getContentSize().height));
            uiValue->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiValue->setTextHorizontalAlignment(TextHAlignment::RIGHT);
            uiValue->setTextOverflow(Label::Overflow::SHRINK);
            uiValue->setName("UI_TYPE_VALUE");
            uiType2BG->addChild(uiValue);
        }
        
        posY = posY - uiType2BG->getContentSize().height - 11;
        auto uiType3BG = ui::ImageView::create("Assets/ui_common/base_box_1_4.png");
        uiType3BG->setScale9Enabled(true);
        uiType3BG->setCapInsets(Rect(10,10,2,2));
        uiType3BG->setContentSize(Size(423, 38));
        uiType3BG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiType3BG->setPosition(Vec2(uiLayout->getContentSize().width, posY));
        uiType3BG->setName("UI_TYPE_BG_3");
        uiLayout->addChild(uiType3BG);
        {
            auto uiEffect = ui::ImageView::create("Assets/ui_common/base_box_1_19.png");
            uiEffect->setScale9Enabled(true);
            uiEffect->setCapInsets(Rect(10,10,2,2));
            uiEffect->setContentSize(Size(423, 38));
            uiEffect->setPosition(Vec2(uiType3BG->getContentSize().width / 2, uiType3BG->getContentSize().height / 2));
            uiEffect->setName("UI_TYPE_EFFECT");
            uiEffect->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0), FadeIn::create(1.0), NULL)));
            uiType3BG->addChild(uiEffect);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_production_msg_3"), GAME_FONT, 24);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiText->setPosition(Vec2(10, uiType3BG->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(250, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiType3BG->addChild(uiText);
            
            auto uiValue = ui::CText::create(" ", GAME_FONT, 24);
            uiValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiValue->setPosition(Vec2(uiType3BG->getContentSize().width - 50, uiType3BG->getContentSize().height / 2));
            uiValue->setTextAreaSize(Size(100, uiText->getContentSize().height));
            uiValue->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiValue->setTextHorizontalAlignment(TextHAlignment::RIGHT);
            uiValue->setTextOverflow(Label::Overflow::SHRINK);
            uiValue->setName("UI_TYPE_VALUE");
            uiType3BG->addChild(uiValue);
        }
    }
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_msg_1"), GAME_FONT, 32);
    uiText->setPosition(Vec2(size.width / 2, size.height - 240));
    uiText->setTextAreaSize(Size(570, uiText->getContentSize().height));
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    layout->addChild(uiText);
}

void PopupMineRefineProduction::uiMine()
{
    auto layout = _uiContainerMine;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(570, 350));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiBG);
    {
        auto uiNameBG = ui::ImageView::create(PIXEL);
        uiNameBG->setScale9Enabled(true);
        uiNameBG->setContentSize(Size(566, 36));
        uiNameBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiNameBG->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height - 2));
        uiNameBG->setColor(Color3B(40, 49, 58));
        uiBG->addChild(uiNameBG);
        {
            auto uiName = Label::createWithTTF(GAME_TEXT("t_ui_equip_legend_meterial"), GAME_FONT, 24);
            uiName->setPosition(Vec2(uiNameBG->getContentSize().width / 2, uiNameBG->getContentSize().height / 2));
            uiName->setColor(COLOR_TITLE);
            uiNameBG->addChild(uiName);
        }
        
        auto uiMine = ui::Layout::create();
        uiMine->setContentSize(Size(550, 310));
        uiMine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiMine->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2 - uiNameBG->getContentSize().height / 2));
        uiMine->setName("UI_MINE");
        uiBG->addChild(uiMine);
    }
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_autosave"), GAME_FONT, 28);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiText->setPosition(Vec2(uiBG->getContentSize().width / 2, -10));
    uiBG->addChild(uiText);
}

void PopupMineRefineProduction::uiButtons()
{
    auto layout = _uiContainerButtons;
    Size size = layout->getContentSize();
    
    auto uiBtn = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png");
    uiBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtn->setPosition(Vec2(size.width / 2, 0));
    uiBtn->addClickEventListener(CC_CALLBACK_1(PopupMineRefineProduction::onClickProduction, this));
    uiBtn->setName("UI_BTN");
    layout->addChild(uiBtn);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_production_bt_1"), GAME_FONT, 28);
        uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, 32.5));
        uiText->setTextAreaSize(Size(uiBtn->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiBtn->addChild(uiText);
    }
}

#pragma mark - ui
void PopupMineRefineProduction::drawInfo()
{
    auto layout = _uiContainerInfo;
    
    int itemAdd = MineralManager::getInstance()->getRefineProductionAdd();
    int timeNeed = MineralManager::getInstance()->getRefineProductionTimeNeed();
    
    int count = MineralManager::getInstance()->getRefineProductionCount();
    int countMax = MineralManager::getInstance()->getRefineProductionCountMax();
    
    //
    if ( auto uiTypeBG = (ui::ImageView*)utils::findChild(layout, "UI_TYPE_BG_1"); uiTypeBG != nullptr )
    {
        if ( auto uiValue = uiTypeBG->getChildByName<ui::CText*>("UI_TYPE_VALUE"); uiValue != nullptr )
        {
            uiValue->setString(MafUtils::format("+%d", itemAdd));
        }
    }
    
    if ( auto uiTypeBG = (ui::ImageView*)utils::findChild(layout, "UI_TYPE_BG_2"); uiTypeBG != nullptr )
    {
        if ( auto uiValue = uiTypeBG->getChildByName<ui::CText*>("UI_TYPE_VALUE"); uiValue != nullptr )
        {
            uiValue->setString(GAME_TEXTFORMAT("t_ui_second", timeNeed));
        }
    }
    
    if ( auto uiTypeBG = (ui::ImageView*)utils::findChild(layout, "UI_TYPE_BG_3"); uiTypeBG != nullptr )
    {
        auto uiEffect = uiTypeBG->getChildByName<ui::ImageView*>("UI_TYPE_EFFECT");
        uiEffect->setVisible(false);
        if ( count > 0 )
        {
            uiEffect->setVisible(true);
        }
        
        if ( auto uiValue = uiTypeBG->getChildByName<ui::CText*>("UI_TYPE_VALUE"); uiValue != nullptr )
        {
            uiValue->setString(MafUtils::format("%d/%d", count, countMax));
        }
    }
}

void PopupMineRefineProduction::drawMine()
{
    auto layout = _uiContainerMine;
    
    auto listCostMine = MineralManager::getInstance()->getRefineProductionCostMine();
    
    auto uiMine = (ui::Layout*)utils::findChild(layout, "UI_MINE");
    if ( uiMine != nullptr )
    {
        uiMine->removeAllChildren();
        
        for ( int i = 0; i < listCostMine.size(); i++ )
        {
            auto objItem = listCostMine.at(i);
            
            int itemNeed = atoi(objItem->getCount().c_str());
            int itemNow = atoi(ItemsManager::getInstance()->getItems((E_ITEMS)objItem->getIdx()).c_str());
        
            double percent = 0;
            if ( itemNow > 0 && itemNeed > 0 )
            {
                percent = (double)itemNow / itemNeed * 100;
                if ( percent > 100 )
                    percent = 100;
            }
            
            // item bg
            auto uiMineCell = ui::Layout::create();
            uiMineCell->setContentSize(Size(112, 89));
            uiMine->addChild(uiMineCell);
            if ( objItem->getType().compare("MINE") == 0 && itemNeed != 0 && itemNow < itemNeed )
            {
                uiMineCell->setTouchEnabled(true);
                uiMineCell->addClickEventListener(CC_CALLBACK_1(PopupMineRefineProduction::onClickMine, this));
                uiMineCell->setTag(objItem->getTypeParams());
            }
            {
                auto uiIcon = ui::ImageView::create(objItem->getPath());
                uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiIcon->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 24));
                uiIcon->setScale(1.2);
                uiMineCell->addChild(uiIcon);

                auto uiBarBG = ui::ImageView::create("Assets/ui/mine/week_weapon_bar1.png");
                uiBarBG->setScale9Enabled(true);
                uiBarBG->setContentSize(Size(uiMineCell->getContentSize().width, uiBarBG->getContentSize().height));
                uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiBarBG->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 0));
                uiMineCell->addChild(uiBarBG);

                auto uiBar = ui::LoadingBar::create("Assets/ui/mine/week_weapon_bar2.png");
                uiBar->setScale9Enabled(true);
                uiBar->setContentSize(Size(uiMineCell->getContentSize().width, uiBarBG->getContentSize().height));
                uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiBar->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 0));
                uiBar->setPercent(percent);
                uiMineCell->addChild(uiBar);
                
                auto uiCount = Label::createWithTTF("-", GAME_FONT, 16);
                uiCount->setPosition(uiBar->getContentSize().width / 2, uiBar->getContentSize().height / 2);
                uiBar->addChild(uiCount);
                if ( itemNeed > 0 )
                {
                    std::string itemNowText = MafUtils::toString(itemNow);
                    itemNowText = MafUtils::convertNumberToShort(itemNowText);
                    
                    std::string itemNeedText = MafUtils::toString(itemNeed);
                    itemNeedText = MafUtils::convertNumberToShort(itemNeedText);

                    uiCount->setString(MafUtils::format("%s/%s",  itemNowText.c_str(), itemNeedText.c_str()));
                }
                
                //
                if ( objItem->getType().compare("MINE") == 0 && itemNeed != 0 && itemNow < itemNeed )
                {
                    auto uiEffect = ui::ImageView::create("Assets/ui_common/box_item_1_3.png");
                    uiEffect->setPosition(Vec2(uiMineCell->getContentSize().width / 2, uiMineCell->getContentSize().height / 2));
                    uiMineCell->addChild(uiEffect);

                    auto uiReddot = ui::ImageView::create("Assets/ui/exclamation_mark_icon_2.png");
                    uiReddot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                    uiReddot->setPosition(Vec2(0, uiMineCell->getContentSize().height));
                    uiMineCell->addChild(uiReddot);
                }
                else if ( (E_ITEMS)objItem->getIdx() == E_ITEMS::MYTHRIL )
                {
                    auto uiEffect = ui::ImageView::create("Assets/ui_common/box_transcendence1_2.png");
                    uiEffect->setPosition(Vec2(uiMineCell->getContentSize().width / 2, uiMineCell->getContentSize().height / 2 + 10));
                    uiMineCell->addChild(uiEffect, -1);
                }
            }
        }
        
        //
        UtilsWidget::getInstance()->sortWidgetGrid(uiMine, 4, uiMine->getContentSize().width, uiMine->getContentSize().height, 10, false, false);
    }
}

void PopupMineRefineProduction::drawButtons()
{
    auto layout = _uiContainerButtons;
    
    auto objCost = MineralManager::getInstance()->getRefineProductionCost();
    
    int count = MineralManager::getInstance()->getRefineProductionCount();
    int countMax = MineralManager::getInstance()->getRefineProductionCountMax();
    
    //
    if ( auto uiBtn = layout->getChildByName<ui::Button*>("UI_BTN"); uiBtn != nullptr )
    {
        uiBtn->removeChildByName("UI_COST");
        
        if ( objCost != nullptr )
        {
            auto uiCost = UtilsWidget::getInstance()->createIconText(objCost->getPath(), objCost->getCount(), uiBtn->getContentSize().width - 10, 24, 15);
            uiCost->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height - 30));
            uiCost->setName("UI_COST");
            uiBtn->addChild(uiCost);
        }
        
        bool bLock = false;
        
        // cost
        auto itemNeed = objCost->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)objCost->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            bLock = true;
        }
        
        if ( count >= countMax )
        {
            bLock = true;
        }

        if ( bLock == true )
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_upgrade_max.png");
        }
        else
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_upgrade_bg.png");
        }
    }
}


#pragma mark - set, get, other


#pragma mark - callback
void PopupMineRefineProduction::callbackPurchaseOk(int nIdx)
{
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(PopupMineRefineProduction::callbackPurchase, this));
}

void PopupMineRefineProduction::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        //
        drawMine();
        drawButtons();
        
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

#pragma mark - click
void PopupMineRefineProduction::onClickClose(Ref* sender)
{
    hide();
}

void PopupMineRefineProduction::onClickProduction(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    int result = MineralManager::getInstance()->onRefineProduction();
    if ( result != 0 )
    {
        return;
    }
    
    int count = MineralManager::getInstance()->getRefineProductionCount();
    int countMax = MineralManager::getInstance()->getRefineProductionCountMax();
    if ( count >= countMax )
    {
        hide();
        return;
    }
    
    //
    drawInfo();
    drawMine();
    drawButtons();
}

void PopupMineRefineProduction::onClickMine(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    E_MINERAL eType = enumFromInt<E_MINERAL>(uiLayout->getTag());
    
    E_CASH eCash = E_CASH::CASH_MINE_1;
    switch (eType) {
        case E_MINERAL::MINERAL_1:     eCash = E_CASH::CASH_MINE_1;    break;
        case E_MINERAL::MINERAL_2:     eCash = E_CASH::CASH_MINE_2;    break;
        case E_MINERAL::MINERAL_3:     eCash = E_CASH::CASH_MINE_3;    break;
        case E_MINERAL::MINERAL_4:     eCash = E_CASH::CASH_MINE_4;    break;
        case E_MINERAL::MINERAL_5:     eCash = E_CASH::CASH_MINE_5;    break;
        case E_MINERAL::MINERAL_6:     eCash = E_CASH::CASH_MINE_6;    break;
        case E_MINERAL::MINERAL_7_SAPPHIRE:        eCash = E_CASH::CASH_MINE_7;        break;
        case E_MINERAL::MINERAL_8_OPAL:            eCash = E_CASH::CASH_MINE_8;        break;
        case E_MINERAL::MINERAL_9_AQUA:            eCash = E_CASH::CASH_MINE_9;        break;
        case E_MINERAL::MINERAL_10_EMERALD:        eCash = E_CASH::CASH_MINE_10;       break;
        case E_MINERAL::MINERAL_11_AMETHYST:       eCash = E_CASH::CASH_MINE_11;       break;
            
        default:
            break;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(eCash);
    if ( objProduct == nullptr )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }

    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(eCash, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(eCash, CC_CALLBACK_4(PopupMineRefineProduction::callbackPurchase, this));
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
            auto popup = PopupPurchaseBuy::create(eCash);
            popup->setCallbackResult(CC_CALLBACK_1(PopupMineRefineProduction::callbackPurchaseOk, this));
            popup->show();
        }
    });
}
