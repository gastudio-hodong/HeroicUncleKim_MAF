//
//  UISettingMenu.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UISettingMenu.h"

#include "GameUIPopup/Common/MenuPopup.h"
#include "GameUIPopup/Other/Attend/PopupAttend.h"
#include "GameUIPopup/Other/Settings/Mail/MailPopup.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexDefault.h"
#include "GameUIPopup/Other/Story/PopupStoryListDefault.h"
#include "GameUIPopup/Other/Settings/SaveLoad/PopupCloudSaveLoad.h"
#include "GameUIPopup/Other/Attend/PopupAttend2020.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerEvent/EventManager.h"

USING_NS_CC;

UISettingMenu* UISettingMenu::create(E_PLACE eType)
{
    UISettingMenu* pRet = new(std::nothrow) UISettingMenu();
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

UISettingMenu::UISettingMenu(void) :
_eType(E_PLACE::EMPTY),
_bExpand(false),
_sizeExpand(Size::ZERO),

_drawCol(0),
_drawRow(0),

_zorder(0)
{
    
}

UISettingMenu::~UISettingMenu(void)
{
    
}

bool UISettingMenu::init(E_PLACE eType)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(Size(54, 54));
    
    //
    _eType = eType;
    
    // init
    initVar();
    initUi();
     

    return true;
}
 

#pragma mark - init
void UISettingMenu::initVar()
{
    _listMenu.push_back(E_TYPE::SLEEP);
    _listMenu.push_back(E_TYPE::SAVE);
    _listMenu.push_back(E_TYPE::MAIL);
    _listMenu.push_back(E_TYPE::ATTEND);
    
    if ( EventManager::getInstance()->isActivatedEvent(15) == true )
    {
        _listMenu.push_back(E_TYPE::ATTEND_EVENT);
    }
    
    _listMenu.push_back(E_TYPE::SETTING);
    if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
    {
        _listMenu.push_back(E_TYPE::NAVER);
    }
    
    _listMenu.push_back(E_TYPE::FRIEND);
    _listMenu.push_back(E_TYPE::COLLECT);
    
    _drawCol = 5;
    _drawRow = (int)(_listMenu.size() / _drawCol);
    if ( _listMenu.size() % _drawCol != 0 )
    {
        _drawRow++;
    }
    
    _sizeExpand = Size(50, 50);
    _sizeExpand.width += 72 * _drawCol;
    _sizeExpand.width += 30 * (_drawCol - 1);
    _sizeExpand.height += 92 * _drawRow;
    _sizeExpand.height += 20 * (_drawRow - 1);
}

void UISettingMenu::initUi()
{
    Size size = getContentSize();
    
    auto uiBtnExpand = ui::Button::create("Assets/ui/main/main_top_menu_btn.png");
    uiBtnExpand->setPosition(Vec2(size.width / 2, size.height / 2));
    uiBtnExpand->addClickEventListener(CC_CALLBACK_1(UISettingMenu::onClickExpand, this));
    addChild(uiBtnExpand);
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition( Vec2(uiBtnExpand->getContentSize().width - 5, uiBtnExpand->getContentSize().height - 5) );
        uiReddot->setVisible(false);
        uiBtnExpand->addChild(uiReddot,100, 100);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            auto uiReddot = (UIReddot*)sender;
            
            bool bShow = false;
            bShow |= UserInfoManager::getInstance()->getGiftHave();
            bShow |= UserInfoManager::getInstance()->getAttendHave();
            
            uiReddot->setVisible(bShow);
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
    }
    
    _uiContainer = ui::Layout::create();
    _uiContainer->setBackGroundImage("Assets/ui/main/main_menu_list_bg_box.png");
    _uiContainer->setBackGroundImageScale9Enabled(true);
    _uiContainer->setBackGroundImageCapInsets(Rect(7, 7, 2, 2));
    _uiContainer->setContentSize(_sizeExpand);
    _uiContainer->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
    _uiContainer->setPosition(Vec2(size.width, -10));
    _uiContainer->setTouchEnabled(true);
    _uiContainer->setVisible(false);
    addChild(_uiContainer);
    
    //
    uiContainer();
}

#pragma mark - ui
void UISettingMenu::uiContainer()
{
    double paddingX = 30;
    double paddingY = 20;
    double posStartX = _uiContainer->getContentSize().width - 25;
    double posStartY = _uiContainer->getContentSize().height - 25;

    double width = 72;
    double height = 92;
    for ( int i = 0; i < _listMenu.size(); i++ )
    {
        auto eType = _listMenu.at(i);

        std::string name = getMenuName(eType);
        std::string pathIcon = getMenuPath(eType);
        
        int x = i % _drawCol;
        int y = i / _drawCol;
        
        double posX = posStartX - (x * width) - (x * paddingX);
        double posY = posStartY - (y * height) - (y * paddingY);
        
        
        auto uiBtnMenu = ui::Button::create(pathIcon);
        uiBtnMenu->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiBtnMenu->setPosition(Vec2(posX, posY));
        uiBtnMenu->addClickEventListener(CC_CALLBACK_1(UISettingMenu::onClickMenu, this));
        uiBtnMenu->setTag(i);
        _uiContainer->addChild(uiBtnMenu);

        auto uiTextName = ui::CText::create(name, GAME_FONT, 16);
        uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiTextName->setPosition(Vec2(uiBtnMenu->getContentSize().width / 2, 10));
        uiTextName->enableOutline(Color4B::BLACK, 2);
        uiTextName->setTextAreaSize(Size(uiBtnMenu->getContentSize().width + 20, uiTextName->getContentSize().height));
        uiTextName->setTextOverflow(Label::Overflow::SHRINK);
        uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnMenu->addChild(uiTextName);
        
        if ( eType == E_TYPE::ATTEND )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition( Vec2(uiBtnMenu->getContentSize().width - 5, uiBtnMenu->getContentSize().height - 5) );
            uiReddot->setVisible(false);
            uiBtnMenu->addChild(uiReddot, 100, 100);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                
                auto uiReddot = (UIReddot*)sender;
                
                bool bShow = false;
                bShow |= UserInfoManager::getInstance()->getAttendHave();
                
                uiReddot->setVisible(bShow);
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
        }
        else if ( eType == E_TYPE::MAIL )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition( Vec2(uiBtnMenu->getContentSize().width - 5, uiBtnMenu->getContentSize().height - 5) );
            uiReddot->setVisible(false);
            uiBtnMenu->addChild(uiReddot, 100, 100);
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                
                auto uiReddot = (UIReddot*)sender;
                
                bool bShow = false;
                bShow |= UserInfoManager::getInstance()->getGiftHave();
                
                uiReddot->setVisible(bShow);
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
        }
    }
}

void UISettingMenu::drawExpand()
{
    _uiContainer->setVisible(_bExpand);
    
    //
    if ( _bExpand == false )
    {
        setLocalZOrder(_zorder);
        
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
        setLocalZOrder(9999);
        
        //
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

#pragma mark - set, get
std::string UISettingMenu::getMenuName(E_TYPE eType)
{
    std::string name = "";
    switch (eType) {
        case E_TYPE::ATTEND:        name = "t_ui_menu_attend"; break;
        case E_TYPE::MAIL:          name = "t_ui_menu_present"; break;
        case E_TYPE::SAVE:          name = "t_ui_menu_data_save"; break;
        case E_TYPE::STORY:         name = "t_ui_menu_store"; break;
        case E_TYPE::SLEEP:         name = "t_ui_battery"; break;
        case E_TYPE::COLLECT:       name = "t_ui_menu_bonsikdex"; break;
        case E_TYPE::FRIEND:        name = "t_ui_menu_friend"; break;
        case E_TYPE::NAVER:         name = "t_ui_menu_naver"; break;
        case E_TYPE::SETTING:       name = "t_ui_menu_setting"; break;
        case E_TYPE::ATTEND_EVENT:  name = "t_ui_season_attendent"; break;
            
        default:
            break;
    }
    
    return GAME_TEXT(name);
}

std::string UISettingMenu::getMenuPath(E_TYPE eType)
{
    std::string path = "";
    switch (eType) {
        case E_TYPE::ATTEND:        path = "Assets/ui/main/main_menu_attendance.png"; break;
        case E_TYPE::MAIL:          path = "Assets/ui/main/main_menu_mail.png"; break;
        case E_TYPE::SAVE:          path = "Assets/ui/main/main_menu_save.png"; break;
        case E_TYPE::STORY:         path = "Assets/ui/main/main_menu_story.png"; break;
        case E_TYPE::SLEEP:         path = "Assets/ui/main/main_menu_sleep.png"; break;
        case E_TYPE::COLLECT:       path = "Assets/ui/main/main_menu_collect.png"; break;
        case E_TYPE::FRIEND:        path = "Assets/ui/main/main_menu_friend.png"; break;
        case E_TYPE::NAVER:         path = "Assets/ui/main/main_menu_naver.png"; break;
        case E_TYPE::SETTING:       path = "Assets/ui/main/main_menu_setting.png"; break;
        case E_TYPE::ATTEND_EVENT:
        {
            time_t nTime = UtilsDate::getInstance()->getTime();
            struct tm *info = localtime(&nTime);
            
            int month = info->tm_mon + 1;
            switch (month) {
                case 3:case 4:case 5:
                    path = "Assets/ui/main/icon_attendance_01.png"; break;
                case 6:case 7:case 8:
                    path = "Assets/ui/main/icon_attendance_02.png"; break;
                case 9:case 10:case 11:
                    path = "Assets/ui/main/icon_attendance_03.png"; break;
                case 12:case 1:case 2:
                    path = "Assets/ui/main/icon_attendance_04.png"; break;
                default:
                    break;
            }
        }
            
        default:
            break;
    }
    
    return path;
}

#pragma mark - schedule

#pragma mark - click
void UISettingMenu::onClickExpand(cocos2d::Ref* sender)
{
    onExpand();
}

void UISettingMenu::onClickMenu(cocos2d::Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_TYPE eType = _listMenu.at(uiBtn->getTag());
    
    switch (eType) {
        case E_TYPE::ATTEND:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            else
            {
                auto popup = PopupAttend::create();
                popup->show();
            }
        }
            break;;
        case E_TYPE::MAIL:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            else
            {
                auto popup = MailPopup::create();
                popup->show();
            }
        }
            break;;
        case E_TYPE::SAVE:
        {
            auto popup = PopupCloudSaveLoad::create(true);
            popup->show();
            break;
        }
        case E_TYPE::STORY:
        {
            auto popup = PopupStoryListDefault::create();
            popup->show();
        }
            break;;
        case E_TYPE::SLEEP:
        {
            PopupScreenLock::show();
        }
            break;;
        case E_TYPE::COLLECT:
        {
            auto popup = PopupBonsikDexDefault::create();
            popup->show();
        }
            break;;
        case E_TYPE::FRIEND:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            else
            {
                auto popup = PopupFriend::create();
                popup->show();
            }
        }
            break;;
        case E_TYPE::NAVER:
        {
            MafNative::OpenUrl("https://cafe.naver.com/gimbongsik");
        }
            break;;
        case E_TYPE::SETTING:
        {
            auto popup = MenuPopup::create();
            popup->show();
        }
            break;
        case E_TYPE::ATTEND_EVENT:
        {
            auto popup = PopupAttend2020::create();
            popup->show();
        }
            
        default:
            break;
    }
    
    
    // close
    onExpand();
}


#pragma mark - event
void UISettingMenu::onExpand()
{
    _bExpand = !_bExpand;
    drawExpand();
}
