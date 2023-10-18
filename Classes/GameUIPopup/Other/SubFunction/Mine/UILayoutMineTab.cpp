//
//  UILayoutMineTab.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutMineTab.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"
#include "GameUIPopup/Other/SubFunction/Mine/UILayoutMine.h"
#include "GameUIPopup/Other/SubFunction/Mine/UILayoutMineRefine.h"

#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UILayoutMineTab* UILayoutMineTab::create()
{
    UILayoutMineTab* pRet = new(std::nothrow) UILayoutMineTab();
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

UILayoutMineTab::UILayoutMineTab():
_bInitUI(false),

_eTab(E_TAB::MINE),


//
_uiContainerTab(nullptr),
_uiContainerContents(nullptr)
{
    
}

bool UILayoutMineTab::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
        
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutMineTab::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiTab();
        uiContents();
        
        _bInitUI = true;
    }
    
    drawTab();
    drawContents();
}

#pragma mark - init
void UILayoutMineTab::initVar()
{
    
}

void UILayoutMineTab::initUI()
{
    // size
    Size size = Size(750, 1109);
    setContentSize(size);
    setAnchorPoint(Vec2::ZERO);
}
 
#pragma mark - ui
void UILayoutMineTab::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerTab = ui::Layout::create();
    _uiContainerTab->setContentSize( Size(size.width - 12, 64) );
    _uiContainerTab->setIgnoreAnchorPointForPosition(false);
    _uiContainerTab->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerTab->setPosition( Vec2(size.width / 2, size.height) );
    addChild(_uiContainerTab);
    
    _uiContainerContents = ui::Layout::create();
    _uiContainerContents->setContentSize( Size(size.width - 12, size.height - _uiContainerTab->getContentSize().height) );
    _uiContainerContents->setIgnoreAnchorPointForPosition(false);
    _uiContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerContents->setPosition( Vec2(size.width / 2, _uiContainerTab->getPositionY() - _uiContainerTab->getContentSize().height) );
    addChild(_uiContainerContents);
}

void UILayoutMineTab::uiTab()
{
    auto layout = _uiContainerTab;
    Size size = layout->getContentSize();
    
    auto uiTab = ui::Layout::create();
    uiTab->setContentSize(Size(size.width, size.height));
    uiTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTab->setPosition(Vec2(size.width / 2, size.height));
    uiTab->setName("UI_TAB");
    layout->addChild(uiTab);
}

void UILayoutMineTab::uiContents()
{
    auto layout = _uiContainerContents;
    Size size = layout->getContentSize();
    
    // content
    auto uiContent = ui::Layout::create();
    uiContent->setContentSize(Size(size.width, size.height));
    uiContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiContent->setPosition(Vec2(size.width / 2, 0));
    uiContent->setName("UI_CONTENT");
    layout->addChild(uiContent);
}

#pragma mark - draw
void UILayoutMineTab::drawTab()
{
    auto layout = _uiContainerTab;
    Size size = layout->getContentSize();
    
    // button
    auto uiTab = layout->getChildByName<ui::Layout*>("UI_TAB");
    if ( uiTab != nullptr )
    {
        uiTab->removeAllChildren();
        
        //
        std::vector<E_TAB> listTab = {E_TAB::MINE, E_TAB::REFINE};
        std::vector<std::string> listTabTextKey = {"t_ui_mine", "t_ui_smelting_1"};
 
        size_t cellCount = listTab.size();
        double cellPadding = 0;
        double cellHeight = 64;
        double cellWidth = (uiTab->getContentSize().width - cellPadding * (cellCount - 1)) / cellCount;
        
        for ( int i = 0; i < cellCount; i++ )
        {
            E_TAB eTab = listTab.at(i);
            
            double posX = cellWidth * i + cellPadding * i;
            double posY = 0;
            
            std::string pathCell = "Assets/ui_common/btn_tap_off.png";
            if ( eTab == _eTab )
            {
                pathCell = "Assets/ui_common/btn_tap_on.png";
            }
            auto uiBtnCell = ui::Button::create(pathCell);
            uiBtnCell->setScale9Enabled(true);
            uiBtnCell->setContentSize(Size(cellWidth, cellHeight));
            uiBtnCell->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBtnCell->setPosition(Vec2(posX, posY));
            uiBtnCell->setScale(cellWidth / uiBtnCell->getContentSize().width);
            uiBtnCell->setTag(enumToInt(eTab));
            uiBtnCell->addClickEventListener(CC_CALLBACK_1(UILayoutMineTab::onClickTab, this));
            uiTab->addChild(uiBtnCell);
            {
                bool bLock = false;
                
                if ( eTab == E_TAB::REFINE )
                {
                    if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_180_L )
                        bLock = true;
                }
                
                if ( bLock )
                {
                    auto uiLock = UtilsWidget::getInstance()->createIconText("Assets/ui_common/tap_lock.png", GAME_TEXT(listTabTextKey.at(i)), uiBtnCell->getContentSize().width - 10, 24, 0);
                    uiLock->setPosition(Vec2(uiBtnCell->getContentSize().width / 2, uiBtnCell->getContentSize().height / 2));
                    uiBtnCell->addChild(uiLock);
                    
                    auto uiText = (ui::CText*)uiLock->getChildByName("WIDGET_TEXT");
                    uiText->setTextColor(Color4B(79, 68, 44, 255));
                }
                else
                {
                    auto uiText = ui::CText::create(GAME_TEXT(listTabTextKey.at(i)), GAME_FONT, 24);
                    uiText->setPosition(Vec2(uiBtnCell->getContentSize().width / 2, uiBtnCell->getContentSize().height / 2));
                    uiText->setTextColor(Color4B(79, 68, 44, 255));
                    uiText->setTextAreaSize(Size(uiBtnCell->getContentSize().width - 10, uiText->getContentSize().height));
                    uiText->setTextOverflow(Label::Overflow::SHRINK);
                    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                    uiBtnCell->addChild(uiText);
                    if ( eTab == _eTab )
                    {
                        uiText->setTextColor(Color4B(255, 255, 255, 255));
                    }
                }
            }
        }
    }
}

void UILayoutMineTab::drawContents()
{
    auto layout = _uiContainerContents;;
    Size size = layout->getContentSize();
    
    auto uiContent = layout->getChildByName<ui::Layout*>("UI_CONTENT");
    if ( uiContent != nullptr )
    {
        uiContent->removeAllChildren();
        
        ui::Layout* uiContentChild = nullptr;
        switch (_eTab) {
            case E_TAB::MINE:       uiContentChild = UILayoutMine::create(); break;
            case E_TAB::REFINE:     uiContentChild = UILayoutMineRefine::create(); break;
                
            default:
                break;
        }
        
        if ( uiContentChild != nullptr )
        {
            uiContentChild->setContentSize(Size(uiContent->getContentSize()));
            uiContent->addChild(uiContentChild);
        }
    }
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
void UILayoutMineTab::onClickTab(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_TAB eTab = enumFromInt<E_TAB>(uiBtn->getTag());
    if ( eTab == _eTab )
    {
        return;
    }
    
    bool bLock = false;
    if ( eTab == E_TAB::REFINE )
    {
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_180_L )
            bLock = true;
    }
    
    if ( eTab == E_TAB::REFINE && bLock == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_mine_mk2_unlock_msg"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    _eTab = eTab;
    drawTab();
    drawContents();
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    auto popup = scene->getChildByName<PopupSubFunction*>("PopupSubFunction");
    if ( popup != nullptr )
    {
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::GEM);
        list.push_back(E_ITEMS::GOLD);
        list.push_back(E_ITEMS::MYTHRIL);
        
        popup->setCurrency(list);
    }
}
     
