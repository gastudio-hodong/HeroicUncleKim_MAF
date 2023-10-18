//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupMineRefineUpgrade.h"

#include "GameObject/Mineral/InfoMineralRefineUpgrade.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupMineRefineUpgrade* PopupMineRefineUpgrade::create()
{
    PopupMineRefineUpgrade* pRet = new(std::nothrow) PopupMineRefineUpgrade();
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

PopupMineRefineUpgrade::PopupMineRefineUpgrade(void) :
_bInitUI(false),

_idx(0),

//
_uiContainerInfo(nullptr),
_uiContainerMine(nullptr),
_uiContainerButtons(nullptr)
{
    
}

PopupMineRefineUpgrade::~PopupMineRefineUpgrade(void)
{
    
}

bool PopupMineRefineUpgrade::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupMineRefineUpgrade");
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupMineRefineUpgrade::onEnter()
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
void PopupMineRefineUpgrade::initVar()
{
    
}

void PopupMineRefineUpgrade::initUi()
{
    Size size = Size(746, 1040);
    
    // size
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_mine_mk2_upgrade_bt"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupMineRefineUpgrade::onClickClose, this));
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
void PopupMineRefineUpgrade::uiContainer()
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

void PopupMineRefineUpgrade::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiIconBG = ui::ImageView::create("Assets/ui/mine/smelt_item_box.png");
    uiIconBG->setPosition(Vec2(size.width / 2, size.height - 110));
    layout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create("");
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setName("UI_ICON");
        uiIconBG->addChild(uiIcon);
        
        auto uiName = ui::CText::create("", GAME_FONT, 28);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiName->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height + 5));
        uiName->setName("UI_NAME");
        uiIconBG->addChild(uiName);
    }
    
    auto uiLayoutDefault = ui::Layout::create();
    uiLayoutDefault->setContentSize(Size(size.width, 130));
    uiLayoutDefault->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiLayoutDefault->setPosition(Vec2(size.width / 2, 0));
    uiLayoutDefault->setName("UI_LAYOUT_DEFAULT");
    layout->addChild(uiLayoutDefault);
    {
        auto uiLevelPrev = ui::CText::create("Lv.0", GAME_FONT, 28);
        uiLevelPrev->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiLevelPrev->setPosition(Vec2(uiLayoutDefault->getContentSize().width / 2 - 50, uiLayoutDefault->getContentSize().height - 25));
        uiLevelPrev->setName("UI_LEVEL_PREV");
        uiLayoutDefault->addChild(uiLevelPrev);
        
        auto uiLevelNext = ui::CText::create("Lv.0", GAME_FONT, 28);
        uiLevelNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiLevelNext->setPosition(Vec2(uiLayoutDefault->getContentSize().width / 2 + 50, uiLayoutDefault->getContentSize().height - 25));
        uiLevelNext->setTextColor(Color4B(246, 203, 28, 255));
        uiLevelNext->setName("UI_LEVEL_NEXT");
        uiLayoutDefault->addChild(uiLevelNext);
        
        auto uiLevelArrow = ui::ImageView::create("Assets/ui_common/btn_shortcuts_4.png");
        uiLevelArrow->setPosition(Vec2(uiLayoutDefault->getContentSize().width / 2, uiLayoutDefault->getContentSize().height - 25));
        uiLayoutDefault->addChild(uiLevelArrow);
        
        auto uiEffect = ui::CText::create(" ", GAME_FONT, 30);
        uiEffect->setPosition(Vec2(uiLayoutDefault->getContentSize().width / 2, uiLayoutDefault->getContentSize().height - 80));
        uiEffect->setTextAreaSize(Size(570, uiEffect->getContentSize().height));
        uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiEffect->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiEffect->setName("UI_EFFECT");
        uiLayoutDefault->addChild(uiEffect);
    }
    
    auto uiLayoutMax = ui::Layout::create();
    uiLayoutMax->setContentSize(Size(size.width, 130));
    uiLayoutMax->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiLayoutMax->setPosition(Vec2(size.width / 2, 0));
    uiLayoutMax->setName("UI_LAYOUT_MAX");
    uiLayoutMax->setVisible(false);
    layout->addChild(uiLayoutMax);
    {
        auto uiLevelPrev = ui::CText::create("Lv.0", GAME_FONT, 28);
        uiLevelPrev->setPosition(Vec2(uiLayoutMax->getContentSize().width / 2, uiLayoutMax->getContentSize().height - 25));
        uiLevelPrev->setName("UI_LEVEL");
        uiLayoutMax->addChild(uiLevelPrev);
        
        auto uiEffect = ui::CText::create(" ", GAME_FONT, 30);
        uiEffect->setPosition(Vec2(uiLayoutMax->getContentSize().width / 2, uiLayoutMax->getContentSize().height - 80));
        uiEffect->setTextAreaSize(Size(570, uiEffect->getContentSize().height));
        uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiEffect->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiEffect->setName("UI_EFFECT");
        uiLayoutMax->addChild(uiEffect);
    }
    
    
}

void PopupMineRefineUpgrade::uiMine()
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
        uiMine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiMine->setPosition(Vec2(uiBG->getContentSize().width / 2, 2));
        uiMine->setName("UI_MINE");
        uiBG->addChild(uiMine);
        
        auto uiTextMax = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_upgrade_max"), GAME_FONT, 28);
        uiTextMax->setPosition(Vec2(uiBG->getContentSize().width / 2, 157));
        uiTextMax->setVisible(false);
        uiTextMax->setName("UI_TEXT_MAX");
        uiBG->addChild(uiTextMax);
    }
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_mine_mk2_autosave"), GAME_FONT, 28);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiText->setPosition(Vec2(uiBG->getContentSize().width / 2, -10));
    uiBG->addChild(uiText);
}

void PopupMineRefineUpgrade::uiButtons()
{
    auto layout = _uiContainerButtons;
    Size size = layout->getContentSize();
    
    auto uiBtn = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png");
    uiBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtn->setPosition(Vec2(size.width / 2, 0));
    uiBtn->addClickEventListener(CC_CALLBACK_1(PopupMineRefineUpgrade::onClickLevelUp, this));
    uiBtn->setName("UI_BTN");
    layout->addChild(uiBtn);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_raid_p_lvup_bt"), GAME_FONT, 28);
        uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, 32.5));
        uiText->setTextAreaSize(Size(uiBtn->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiBtn->addChild(uiText);
    }
}

#pragma mark - ui
void PopupMineRefineUpgrade::drawInfo()
{
    auto layout = _uiContainerInfo;
    
    auto objRefineUpgrade = MineralManager::getInstance()->getInfoRefineUpgrade(_idx);
    
    int levelPrev = MineralManager::getInstance()->getRefineUpgradeLevel(objRefineUpgrade->getIdx());
    int levelNext = levelPrev + 1;
    int levelMax = objRefineUpgrade->getLevelMax();
    bool bLevelMax = levelPrev >= levelMax ? true : false;
    
    double effectPrev = MineralManager::getInstance()->getRefineUpgradeEffect(objRefineUpgrade->getIdx());
    double effectNext = MineralManager::getInstance()->getRefineUpgradeEffect(objRefineUpgrade->getIdx(), levelNext);
    
    std::string pathIcon = objRefineUpgrade->getIconPath();
    
    std::string nameText = objRefineUpgrade->getName();
    std::string levelPrevText = MafUtils::format("Lv.%d", levelPrev);
    std::string levelNextText = MafUtils::format("Lv.%d", levelNext);
    std::string infoText = objRefineUpgrade->getDesc();
    infoText = MafUtils::format(infoText.c_str(), effectPrev);
    if ( bLevelMax == false )
    {
        if ( objRefineUpgrade->getIdx() == 3 )
        {
            infoText.append(MafUtils::format(" {#36F030:(+%.0f)}", effectNext));
        }
        else
        {
            infoText.append(MafUtils::format(" {#36F030:(+%.1f%%)}", effectNext));
        }
    }
    
    
    //
    auto uiIcon = (ui::ImageView*)utils::findChild(layout, "UI_ICON");
    if ( uiIcon != nullptr )
    {
        uiIcon->loadTexture(pathIcon);
    }
    
    auto uiName = (ui::CText*)utils::findChild(layout, "UI_NAME");
    if ( uiName != nullptr )
    {
        uiName->setString(nameText);
    }
    
    //
    auto uiLayoutDefault = layout->getChildByName<ui::Layout*>("UI_LAYOUT_DEFAULT");
    auto uiLayoutMax = layout->getChildByName<ui::Layout*>("UI_LAYOUT_MAX");
    if ( bLevelMax == false )
    {
        uiLayoutDefault->setVisible(true);
        uiLayoutMax->setVisible(false);
        
        auto uiLevelPrev = uiLayoutDefault->getChildByName<ui::CText*>("UI_LEVEL_PREV");
        if ( uiLevelPrev != nullptr )
        {
            uiLevelPrev->setString(levelPrevText);
        }
        
        auto uiLevelNext = uiLayoutDefault->getChildByName<ui::CText*>("UI_LEVEL_NEXT");
        if ( uiLevelNext != nullptr )
        {
            uiLevelNext->setString(levelNextText);
        }
        
        auto uiEffect = uiLayoutDefault->getChildByName<ui::CText*>("UI_EFFECT");
        if ( uiEffect != nullptr )
        {
            uiEffect->setString(infoText);
        }
    }
    else
    {
        uiLayoutDefault->setVisible(false);
        uiLayoutMax->setVisible(true);
        
        auto uiLevelPrev = uiLayoutMax->getChildByName<ui::CText*>("UI_LEVEL");
        if ( uiLevelPrev != nullptr )
        {
            uiLevelPrev->setString(levelPrevText);
        }
        
        auto uiEffect = uiLayoutMax->getChildByName<ui::CText*>("UI_EFFECT");
        if ( uiEffect != nullptr )
        {
            uiEffect->setString(infoText);
        }
    }
}

void PopupMineRefineUpgrade::drawMine()
{
    auto layout = _uiContainerMine;
    
    auto objRefineUpgrade = MineralManager::getInstance()->getInfoRefineUpgrade(_idx);
    auto listCostMine = MineralManager::getInstance()->getRefineUpgradeCostMine(_idx);
    
    int levelPrev = MineralManager::getInstance()->getRefineUpgradeLevel(objRefineUpgrade->getIdx());
    int levelNext = levelPrev + 1;
    int levelMax = objRefineUpgrade->getLevelMax();
    bool bLevelMax = levelPrev >= levelMax ? true : false;
    
    //
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
                uiMineCell->addClickEventListener(CC_CALLBACK_1(PopupMineRefineUpgrade::onClickMine, this));
                uiMineCell->setTag(objItem->getTypeParams());
            }
            {
                auto uiIcon = ui::ImageView::create(objItem->getPath());
                uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiIcon->setPosition(Vec2(uiMineCell->getContentSize().width / 2, 24));
                uiIcon->setScale(1.2);
                ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
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
        
        if ( bLevelMax == true )
        {
            uiMine->setVisible(false);
        }
        else
        {
            uiMine->setVisible(true);
        }
    }
    
    //
    auto uiTextMax = (ui::CText*)utils::findChild(layout, "UI_TEXT_MAX");
    if ( bLevelMax == true )
    {
        uiTextMax->setVisible(true);
    }
    else
    {
        uiTextMax->setVisible(false);
    }
}

void PopupMineRefineUpgrade::drawButtons()
{
    auto layout = _uiContainerButtons;
    
    auto objRefineUpgrade = MineralManager::getInstance()->getInfoRefineUpgrade(_idx);
    auto objCost = MineralManager::getInstance()->getRefineUpgradeCost(_idx);
    
    int levelPrev = MineralManager::getInstance()->getRefineUpgradeLevel(objRefineUpgrade->getIdx());
    int levelNext = levelPrev + 1;
    int levelMax = objRefineUpgrade->getLevelMax();
    bool bLevelMax = levelPrev >= levelMax ? true : false;
    
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
        
        if ( bLock == true )
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_upgrade_max.png");
        }
        else
        {
            uiBtn->loadTextureNormal("Assets/ui_common/btn_upgrade_bg.png");
        }
        
        if ( bLevelMax == false )
        {
            uiBtn->setVisible(true);
        }
        else
        {
            uiBtn->setVisible(false);
        }
    }
}

#pragma mark - set, get, other


#pragma mark - callback
void PopupMineRefineUpgrade::callbackPurchaseOk(int nIdx)
{
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(PopupMineRefineUpgrade::callbackPurchase, this));
}

void PopupMineRefineUpgrade::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
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
void PopupMineRefineUpgrade::onClickClose(Ref* sender)
{
    hide();
}

void PopupMineRefineUpgrade::onClickLevelUp(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    int result = MineralManager::getInstance()->onRefineUpgrade(_idx);
    if ( result != 0 )
    {
        return;
    }
    
    auto objRefineUpgrade = MineralManager::getInstance()->getInfoRefineUpgrade(_idx);
    
    int level = MineralManager::getInstance()->getRefineUpgradeLevel(_idx);
    int levelMax = objRefineUpgrade->getLevelMax();
    if ( level >= levelMax )
    {
        hide();
        return;;
    }
    
    //
    drawInfo();
    drawMine();
    drawButtons();
}

void PopupMineRefineUpgrade::onClickMine(cocos2d::Ref* sender)
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
                CashManager::getInstance()->purchaseInitiate(eCash, CC_CALLBACK_4(PopupMineRefineUpgrade::callbackPurchase, this));
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
            popup->setCallbackResult(CC_CALLBACK_1(PopupMineRefineUpgrade::callbackPurchaseOk, this));
            popup->show();
        }
    });
}

#pragma mark - game
void PopupMineRefineUpgrade::setUpgradeIdx(int idx)
{
    _idx = idx;
    
    if ( _bInitUI == true )
    {
        drawInfo();
        drawMine();
        drawButtons();
    }
}
