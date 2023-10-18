//
//  PopupUserLoginGuestAgain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupUserLoginGuestAgain.h"

#include "GameUIPopup/Common/PopupHelp.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupUserLoginGuestAgain::PopupUserLoginGuestAgain():
_bInitUI(false),

//

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr)
{
    
}

PopupUserLoginGuestAgain::~PopupUserLoginGuestAgain(void)
{
    
}

bool PopupUserLoginGuestAgain::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    return true;
}

void PopupUserLoginGuestAgain::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        initVar();
        initUI();

        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawButton();
}

#pragma mark - ui
void PopupUserLoginGuestAgain::initVar()
{

}

void PopupUserLoginGuestAgain::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(608, 670) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_bt"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    //
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupUserLoginGuestAgain::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    uiContainer();
    uiInfo();
    uiButton();
}

void PopupUserLoginGuestAgain::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsButton = ui::Layout::create();
    _uiContentsButton->setContentSize(Size(size.width, 89));
    _uiContentsButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsButton->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButton);
    
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, size.height - _uiContentsButton->getContentSize().height));
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsInfo->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiContentsInfo);
}
 
void PopupUserLoginGuestAgain::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
   
    auto uiDescBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
    uiDescBG->setScale9Enabled(true);
    uiDescBG->setContentSize(Size(558, 202));
    uiDescBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiDescBG->setPosition(Vec2(size.width / 2, 30));
    layout->addChild(uiDescBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_account_popup_msg"), GAME_FONT, 20);
        uiText->setPositionType(ui::Widget::PositionType::PERCENT);
        uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiText->setTextAreaSize(Size(uiDescBG->getContentSize().width - 10, uiDescBG->getContentSize().height - 10));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiDescBG->addChild(uiText);
    }
    
    //
    auto uiInfoBG = ui::Layout::create();
    uiInfoBG->setContentSize(Size(558, 160));
    uiInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoBG->setPosition(Vec2(size.width / 2, size.height - 30));
    layout->addChild(uiInfoBG);
    {
        auto uiTitle = ui::ImageView::create("Assets/ui_common/reward_box_2.png");
        uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiTitle->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, uiInfoBG->getContentSize().height));
        uiInfoBG->addChild(uiTitle);
        
        auto uiItems = ui::Layout::create();
        uiItems->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiItems->setContentSize(Size(90, 90));
        uiItems->setPosition(Vec2(uiInfoBG->getContentSize().width / 2, 0));
        uiItems->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiInfoBG->addChild(uiItems);
        
        std::string strText = ParameterManager::getInstance()->getParameterString("guest_login_reward");
        auto listItem = ItemsManager::getInstance()->getConvertArray(strText);
        for ( int i = 0; i < listItem.size(); i++ )
        {
            auto objItem = listItem.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(90,90));
            uiItems->addChild(uiItemBG);
            
            auto uiItem = ui::ImageView::create(objItem->getPath());
            uiItem->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 10));
            uiItem->setScale(1.2);
            uiItemBG->addChild(uiItem);
            
            auto uiItemCount = ui::CText::create(objItem->getCount(), GAME_FONT, 18);
            uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 0));
            uiItemBG->addChild(uiItemCount);
            
            if ( i != 0 )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiItemBG->setLayoutParameter(params);
            }
        }
        
        auto sizeItems = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiItems);
        uiItems->setContentSize(sizeItems);
        uiItems->requestDoLayout();
    }
    
}

void PopupUserLoginGuestAgain::uiButton()
{
    auto layout = _uiContentsButton;
    auto size = layout->getContentSize();
   
    auto uiButton = ui::Layout::create();
    uiButton->setIgnoreAnchorPointForPosition(false);
    uiButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButton->setPosition(Vec2(size.width / 2, size.height / 2));
    uiButton->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButton->setName("UI_BUTTON");
    layout->addChild(uiButton);
    {
        //
        auto uiOk = ui::Button::create("Assets/ui_common/popup_btn_no.png");
        uiOk->setScale9Enabled(true);
        uiOk->setContentSize(Size(260, 88));
        uiOk->addClickEventListener(CC_CALLBACK_1(PopupUserLoginGuestAgain::onClickOk, this));
        uiButton->addChild(uiOk);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_account_popup_bt_1"), GAME_FONT, 24);
            uiText->setPosition(uiOk->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiOk->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiOk->addChild(uiText);
        }
        
        //
        auto uiHelp = ui::Button::create("Assets/ui_common/popup_btn_no.png");
        uiHelp->setScale9Enabled(true);
        uiHelp->setContentSize(Size(260, 88));
        uiHelp->addClickEventListener(CC_CALLBACK_1(PopupUserLoginGuestAgain::onClickHelp, this));
        uiButton->addChild(uiHelp);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(20, 0, 0, 0));
            uiHelp->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_account_popup_bt_2"), GAME_FONT, 24);
            uiText->setPosition(uiHelp->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiHelp->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiHelp->addChild(uiText);
        }
    }
}

#pragma mark - draw
void PopupUserLoginGuestAgain::drawInfo()
{
    auto layout = _uiContentsInfo;


    
}

void PopupUserLoginGuestAgain::drawButton()
{
    auto layout = _uiContentsButton;
    
    auto uiButton = layout->getChildByName<ui::Layout*>("UI_BUTTON");
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButton);
    uiButton->setContentSize(sizeButtons);
    uiButton->requestDoLayout();
}

#pragma mark - click
void PopupUserLoginGuestAgain::onClickClose(Ref* sender)
{
    hide();
}

void PopupUserLoginGuestAgain::onClickOk(Ref* sender)
{
    auto sicialID = AccountManager::getInstance()->getUserSocialID();
    if ( sicialID.empty() == true )
    {
        AccountManager::getInstance()->onSocialLoginAgain([=](){
            
            auto socialID = AccountManager::getInstance()->getUserSocialID();
            if ( socialID.empty() == true )
            {
                auto platform = AccountManager::getInstance()->getUserPlatform();
                if ( platform.compare("ios") == 0)
                {
                    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_guest_login_error"));
                    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                    popup->show();
                }
                return;
            }
                
            AccountManager::getInstance()->requestGuestCheck(true, CC_CALLBACK_1(PopupUserLoginGuestAgain::callbackNetworkCheck, this));
        });
    }
    else
    {
        AccountManager::getInstance()->requestGuestCheck(true, CC_CALLBACK_1(PopupUserLoginGuestAgain::callbackNetworkCheck, this));
    }
}

void PopupUserLoginGuestAgain::onClickHelp(Ref* sender)
{
    auto platform = AccountManager::getInstance()->getUserPlatform();
    if ( platform.compare("ios") == 0)
    {
        auto popup = PopupHelp::create(1, "t_ui_login_guest_account_popup_info_ios_title_%d", "t_ui_login_guest_account_popup_info_ios_msg_%d");
        popup->show();
    }
    else
    {
        auto popup = PopupHelp::create(1, "t_ui_login_guest_account_popup_info_aos_title_%d", "t_ui_login_guest_account_popup_info_aos_msg_%d");
        popup->show();
    }
}

#pragma mark - callback
void PopupUserLoginGuestAgain::callbackNetworkCheck(bool bResult)
{
    if ( bResult == false )
    {
        auto socialID = AccountManager::getInstance()->getUserSocialID();

        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_login_guest_login_account_popup_msg_case_2", socialID.c_str()));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_login_guest_login_account_popup_msg_case_1"));
        popup->addButton(GAME_TEXT("t_ui_login_guest_login_account_popup_bt_1"), false, [=](Ref* sender){
            AccountManager::getInstance()->requestGuestLink(true, CC_CALLBACK_1(PopupUserLoginGuestAgain::callbackNetworkLink, this));
        });
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->show();
    }
}

void PopupUserLoginGuestAgain::callbackNetworkLink(bool bResult)
{
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    SaveManager::getInstance()->requestSave([=](bool bResult){
        
        //
        MafHttpNew::_vecRequest.clear();
        
        //
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->unscheduleAllWithMinPriority(Scheduler::PRIORITY_NON_SYSTEM_MIN);
        
        MafGoogleRewardAds::HideGoogleBanner();
        Director::getInstance()->pause();
        UtilsScene::getInstance()->scenePopRoot();
        Director::getInstance()->resume();
    });
}

#pragma mark - set, get

#pragma mark - event
