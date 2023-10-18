//
//  UISubMenu.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UISubMenu.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"
#include "GameUIPopup/Other/PurchaseCash/PopupAdsSkip.h"
#include "GameUIPopup/Other/Stone/PopupStone.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

UISubMenu* UISubMenu::create(E_PLACE eType)
{
    UISubMenu* pRet = new(std::nothrow) UISubMenu();
    if ( pRet && pRet->init(eType) )
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

UISubMenu::UISubMenu(void) :
_eType(E_PLACE::EMPTY),
_bExpand(false),
_sizeExpand(Size::ZERO),

_zorder(0)
{
    
}

UISubMenu::~UISubMenu(void)
{
    
}

bool UISubMenu::init(E_PLACE eType)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint( Vec2(0.5f, 0.5f) );
    setContentSize(Size(60, 60));
    
    //
    _eType = eType;
    
    // init
    initVar();
    initUi();
     

    return true;
}

void UISubMenu::onEnter()
{
    LayerColor::onEnter();
    
    //
    onExpand();
}
 

#pragma mark - init
void UISubMenu::initVar()
{
    _listMenu.push_back(E_TYPE::MYHOME);
    _listMenu.push_back(E_TYPE::SHOP_ITEM);
    _listMenu.push_back(E_TYPE::MINE);
    _listMenu.push_back(E_TYPE::TOWN);
    _listMenu.push_back(E_TYPE::STONE);
    _listMenu.push_back(E_TYPE::ADS);
    
    _sizeExpand = Size(60, 60);
    _sizeExpand.width += 68 * _listMenu.size();
    _sizeExpand.width += 20 * _listMenu.size();
}

void UISubMenu::initUi()
{
    Size size = getContentSize();
    
    _uiContainer = ui::Layout::create();
    _uiContainer->setBackGroundImage("Assets/ui/main/main_side_menu_btn_off.png");
    _uiContainer->setBackGroundImageScale9Enabled(true);
    _uiContainer->setBackGroundImageCapInsets(Rect(8, 8, 2, 44));
    _uiContainer->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
    _uiContainer->setPosition(Vec2(size.width, size.height / 2));
    addChild(_uiContainer);
    
    auto uiBtnExpand = ui::Layout::create();
    uiBtnExpand->setContentSize(Size(60, 60));
    uiBtnExpand->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnExpand->setPosition(Vec2(size.width, size.height / 2));
    uiBtnExpand->addClickEventListener(CC_CALLBACK_1(UISubMenu::onClickExpand, this));
    uiBtnExpand->setTouchEnabled(true);
    addChild(uiBtnExpand);
    
    //
    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
    uiReddot->setPosition(getContentSize().width + 10, getContentSize().height + 10);
    uiReddot->setVisible(false);
    addChild(uiReddot);
    {
        auto action1 = CallFuncN::create([=](cocos2d::Node* sender){
        
            auto bResult = false;
            if ( _bExpand == false )
            {
                bResult = isReddotMyHome() || isReddotAds();
            }
            
            sender->setVisible(bResult);
        });
        auto action2 = DelayTime::create(1.0);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
    }
   
    //
    drawContainer();
}

#pragma mark - ui
void UISubMenu::drawContainer()
{
    _uiContainer->stopAllActions();
    _uiContainer->removeAllChildren();
    if ( _bExpand == false )
    {
        setLocalZOrder(_zorder);
        
        //
        _uiContainer->setBackGroundImage("Assets/ui/main/main_side_menu_btn_off.png");
        
        //
        auto action1 = ResizeTo::create(0.1, Size(60, 60));
        auto seq = Sequence::create(action1, NULL);
        _uiContainer->runAction(seq);
        
        //
        auto uiContainerTouchHide = getChildByName<ui::Layout*>("UI_LAYOUT_HIDE");
        if ( uiContainerTouchHide != nullptr )
        {
            uiContainerTouchHide->removeFromParent();
        }
    }
    else
    {
        _zorder = getLocalZOrder();
        // 20220707 : 메뉴 위치 변경으로 인해 사용하지 않음.
        // setLocalZOrder(9999);
        
        //
        _uiContainer->setBackGroundImage("Assets/ui/main/main_side_menu_btn_on.png");
        
        //
        Size size = _sizeExpand;
        auto action1 = ResizeTo::create(0.1, Size(size.width * 1.1, size.height));
        auto action2 = ResizeTo::create(0.1, Size(size.width * 1.1, size.height * 1.1));
        auto action3 = ResizeTo::create(0.1, Size(size.width, size.height));
        auto action4 = ResizeTo::create(0.1, Size(size.width, size.height * 1.1));
        auto action5 = ResizeTo::create(0.1, Size(size.width, size.height));
        auto actionDraw = CallFunc::create(CC_CALLBACK_0(UISubMenu::drawExpand, this));
        auto seq = Sequence::create(action1, action2, action3, actionDraw, action4, action5, NULL);
        _uiContainer->runAction(seq);
        
        // 20220707 : 메뉴 위치 변경으로 인해 사용하지 않음.
        if ( false )
        {
            Size sizeVisible = UtilsRect::getVisibleSize();
            Vec2 pos = this->convertToNodeSpace(Vec2::ZERO);
           
            auto uiContainerTouchHide = ui::Layout::create();
            uiContainerTouchHide->setContentSize(sizeVisible);
            uiContainerTouchHide->setAnchorPoint(Vec2::ZERO);
            uiContainerTouchHide->setPosition(pos);
            uiContainerTouchHide->setName("UI_LAYOUT_HIDE");
            uiContainerTouchHide->setTouchEnabled(true);
            uiContainerTouchHide->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
                auto uiContainerTouchHide = (ui::Layout*)sender;
                uiContainerTouchHide->removeFromParent();
                
                if ( type == ui::Widget::TouchEventType::BEGAN )
                {
                    onExpand();
                }
            });
            addChild(uiContainerTouchHide, -1);
        }
    }
}

void UISubMenu::drawExpand()
{
    double padding = 20;
    double posX = _uiContainer->getContentSize().width - 70;
    double posY = 15;
    
    for ( int i = 0; i < _listMenu.size(); i++ )
    {
        auto eType = _listMenu.at(i);
        
        std::string name = getMenuName(eType);
        std::string pathIcon = getMenuPath(eType);
        std::string pathIconLock = getMenuLockPath(eType);
        
        
        auto uiBtnMenu = ui::Button::create(pathIcon);
        uiBtnMenu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiBtnMenu->setPosition(Vec2(posX, posY));
        uiBtnMenu->addClickEventListener(CC_CALLBACK_1(UISubMenu::onClickMenu, this));
        uiBtnMenu->setTag(i);
        _uiContainer->addChild(uiBtnMenu);
        
        // lock
        if ( eType == E_TYPE::TOWN || eType == E_TYPE::MINE || eType == E_TYPE::STONE )
        {
            auto uiBtnMenuLock = ui::Button::create(pathIconLock);
            uiBtnMenuLock->setPosition(Vec2(uiBtnMenu->getContentSize().width / 2, uiBtnMenu->getContentSize().height / 2));
            uiBtnMenuLock->addClickEventListener(CC_CALLBACK_1(UISubMenu::onClickMenuLock, this));
            uiBtnMenuLock->setTag(i);
            uiBtnMenu->addChild(uiBtnMenuLock);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                auto uiBtnMenuLock = (ui::Button*)sender;
                
                bool bLock = true;
                
                auto tutoMgr = TutorialManager::getInstance();
                if ( eType == E_TYPE::MINE && tutoMgr->getOpenData("WEAPON_76") )
                {
                    bLock = false;
                }
                else if ( eType == E_TYPE::TOWN && tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34) )
                {
                    bLock = false;
                }
                else if ( eType == E_TYPE::STONE && tutoMgr->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38) )
                {
                    bLock = false;
                }
                
                if ( bLock == false)
                {
                    uiBtnMenuLock->removeFromParent();
                }
            });
            auto action2 = DelayTime::create(1);
            auto seq = Sequence::create(action1, action2, NULL);
            uiBtnMenuLock->runAction(RepeatForever::create(seq));
        }
        
        // reddot
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
        uiReddot->setPosition(uiBtnMenu->getContentSize().width + 10, uiBtnMenu->getContentSize().height + 10);
        uiReddot->setVisible(false);
        uiBtnMenu->addChild(uiReddot);
        {
            auto action1 = CallFuncN::create([=](cocos2d::Node* sender){
            
                auto bResult = false;
                if ( eType == E_TYPE::MYHOME )
                {
                    bResult = isReddotMyHome();
                }
                else if ( eType == E_TYPE::ADS )
                {
                    bResult = isReddotAds();
                }
                else if ( eType == E_TYPE::MINE )
                {
                    bResult = isReddotMine();
                }
                
                sender->setVisible(bResult);
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
        }
        
        
        //
        auto uiTextName = ui::CText::create(name, GAME_FONT, 16);
        uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiTextName->setPosition(Vec2(uiBtnMenu->getContentSize().width / 2, 10));
        uiTextName->enableOutline(Color4B::BLACK, 2);
        uiTextName->setTextAreaSize(Size(uiBtnMenu->getContentSize().width + 10, uiTextName->getContentSize().height));
        uiTextName->setTextOverflow(Label::Overflow::SHRINK);
        uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTextName->setLineSpacing(-10);
        uiBtnMenu->addChild(uiTextName);
        
        //
        posX -= uiBtnMenu->getContentSize().width;
        posX -= padding;
    }
}

#pragma mark - set, get
std::string UISubMenu::getMenuName(E_TYPE eType)
{
    std::string name = "";
    switch (eType) {
        case E_TYPE::ADS:           name = "t_ui_adskip"; break;
        case E_TYPE::MYHOME:        name = "t_ui_myhome_name"; break;
        case E_TYPE::SHOP_ITEM:     name = "t_ui_item"; break;
        case E_TYPE::MINE:          name = "t_ui_mine"; break;
        case E_TYPE::TOWN:          name = "t_ui_town"; break;
        case E_TYPE::STONE:         name = "t_ui_stone_reinforce"; break;
            
        default:
            break;
    }
    
    return GAME_TEXT(name);
}

std::string UISubMenu::getMenuPath(E_TYPE eType)
{
    std::string path = "";
    switch (eType) {
        case E_TYPE::ADS:           path = "Assets/ui/main/menu_side_icon_ad.png"; break;
        case E_TYPE::MYHOME:        path = "Assets/ui/main/menu_side_icon_home.png"; break;
        case E_TYPE::SHOP_ITEM:     path = "Assets/ui/main/menu_side_icon_item.png"; break;
        case E_TYPE::MINE:          path = "Assets/ui/main/menu_side_icon_mine.png"; break;
        case E_TYPE::TOWN:          path = "Assets/ui/main/menu_side_icon_tower.png"; break;
        case E_TYPE::STONE:         path = "Assets/ui/main/menu_side_icon_stones.png"; break;
            
        default:
            break;
    }
    
    return path;
}

std::string UISubMenu::getMenuLockPath(E_TYPE eType)
{
    std::string path = "";
    switch (eType) {
        case E_TYPE::MINE:          path = "Assets/ui/main/menu_side_icon_mine_lock.png"; break;
        case E_TYPE::TOWN:          path = "Assets/ui/main/menu_side_icon_tower_lock.png"; break;
        case E_TYPE::STONE:         path = "Assets/ui/main/menu_side_icon_stones_lock.png"; break;
            
        default:
            break;
    }
    
    return path;
}

bool UISubMenu::isReddotMyHome()
{
    bool bResult = false;
    
    E_SHOP_ADS list[] = {E_SHOP_ADS::SHOP_GEM_ADS, E_SHOP_ADS::SHOP_GOLD_ADS, E_SHOP_ADS::SHOP_BUFF_ADS, E_SHOP_ADS::SHOP_MINE_ADS};
    for ( int i = 0; i < sizeof(list)/sizeof(E_SHOP_ADS); i++ )
    {
        int nTime = UserInfoManager::getInstance()->getAdsTime(list[i]);
        if ( nTime <= 0 )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

bool UISubMenu::isReddotAds()
{
    bool bResult = false;
    
    time_t freetime = CashManager::getInstance()->getAdsfreeTime();
    int day = (int)(freetime / (24 * 3600));
    if( day < 4 && freetime > 0)
    {
        bResult = true;
    }
    
    return bResult;
}

bool UISubMenu::isReddotMine()
{
    bool bResult = false;
    
    int itemNow = MineralManager::getInstance()->getRefineProductionItem();
    
    if ( itemNow > 0 )
    {
        bResult = true;
    }
    
    return bResult;
}

#pragma mark - schedule

#pragma mark - click
void UISubMenu::onClickExpand(cocos2d::Ref* sender)
{
    onExpand();
}

void UISubMenu::onClickMenu(cocos2d::Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_TYPE eType = _listMenu.at(uiBtn->getTag());
    
    switch (eType) {
        case E_TYPE::ADS:
        {
            auto popup = PopupAdsSkip::create();
            popup->show();
        }
            break;
        case E_TYPE::MYHOME:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MYHOME);
            popup->show();
        }
            break;
        case E_TYPE::SHOP_ITEM:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::SHOPITEM);
            popup->show();
        }
            break;
        case E_TYPE::MINE:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MINE);
            popup->show();
        }
            break;
        case E_TYPE::TOWN:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::TOWN);
            popup->show();
        }
            break;
        case E_TYPE::STONE:
        {
            auto popup = PopupStone::create();
            popup->show();
        }
            break;
            
        default:
            break;
    }
}

void UISubMenu::onClickMenuLock(cocos2d::Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_TYPE eType = _listMenu.at(uiBtn->getTag());
    int nMissionNeed = 0;
    
    if ( eType == E_TYPE::MINE )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_12"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eType == E_TYPE::TOWN )
    {
        nMissionNeed = 34;
    }
    else if ( eType == E_TYPE::STONE )
    {
        nMissionNeed = 38;
    }
    
    if ( nMissionNeed != 0 )
    {
        std::string strMessage = GAME_TEXTFORMAT("t_ui_locked_menu", nMissionNeed);
        
        Size size = UtilsRect::getVisibleSize();
        
        Vec2 posToast = Vec2(size.width / 2, size.height - 255 - UserInfoManager::getInstance()->_fHeightIphoneX);
        PopupToast::showImmediately(strMessage, posToast.x, posToast.y);
    }
}

#pragma mark - event
void UISubMenu::onExpand()
{
    _bExpand = !_bExpand;
    drawContainer();
}
