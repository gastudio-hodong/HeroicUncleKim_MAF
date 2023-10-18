//
//  PopupUserLogin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupUserLogin.h"

#include "GameUIPopup/Other/User/PopupUserLoginGuest.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

PopupUserLogin::PopupUserLogin():
_bInitUI(false),

//
_callbackResult(nullptr),

//
_uiContentsInfo(nullptr)
{
    
}

PopupUserLogin::~PopupUserLogin(void)
{
    
}

bool PopupUserLogin::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    setHideBackKey(false);
    
    return true;
}

void PopupUserLogin::onEnter()
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
}

#pragma mark - ui
void PopupUserLogin::initVar()
{
    

}

void PopupUserLogin::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(556, 416) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_menu_login"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    //
    uiContainer();
    uiInfo();
}

void PopupUserLogin::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 10;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 10));
    _spriteContainer->addChild(layout);
    
    // 184 * 241
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, size.height));
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsInfo->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsInfo);
}
 
void PopupUserLogin::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
   
    auto uiButtons = ui::Layout::create();
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPositionType(ui::Widget::PositionType::PERCENT);
    uiButtons->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiButtons->setLayoutType(ui::Layout::Type::VERTICAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    
    auto uiGooglePlay = ui::Button::create("Assets/ui_common/btn_game_login.png");
    uiGooglePlay->addClickEventListener(CC_CALLBACK_1(PopupUserLogin::onClickLogin, this));
    uiGooglePlay->setName("UI_GOOGLEPLAY");
    uiButtons->addChild(uiGooglePlay);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui_common/icon_google_play.png");
        uiIcon->setPosition(Vec2(60, uiGooglePlay->getContentSize().height / 2));
        uiGooglePlay->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_bt_aos"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(125, uiGooglePlay->getContentSize().height / 2));
        uiText->setTextColor(Color4B::BLACK);
        uiGooglePlay->addChild(uiText);
    }
    
    auto uiGameCenter = ui::Button::create("Assets/ui_common/btn_game_login.png");
    uiGameCenter->addClickEventListener(CC_CALLBACK_1(PopupUserLogin::onClickLogin, this));
    uiGameCenter->setName("UI_GAMECENTER");
    uiButtons->addChild(uiGameCenter);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui_common/icon_game_center.png");
        uiIcon->setPosition(Vec2(60, uiGameCenter->getContentSize().height / 2));
        uiGameCenter->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_bt_ios"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(125, uiGameCenter->getContentSize().height / 2));
        uiText->setTextColor(Color4B::BLACK);
        uiGameCenter->addChild(uiText);
    }
    
    auto uiGuest = ui::Button::create("Assets/ui_common/btn_game_login.png");
    uiGuest->addClickEventListener(CC_CALLBACK_1(PopupUserLogin::onClickGuest, this));
    uiGuest->setName("UI_GUEST");
    uiButtons->addChild(uiGuest);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(0, 30, 0, 0));
        uiGuest->setLayoutParameter(params);
        
        auto uiIcon = ui::ImageView::create("Assets/ui_common/icon_guest.png");
        uiIcon->setPosition(Vec2(60, uiGuest->getContentSize().height / 2));
        uiGuest->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_bt"), GAME_FONT, 28);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(125, uiGuest->getContentSize().height / 2));
        uiText->setTextColor(Color4B::BLACK);
        uiGuest->addChild(uiText);
    }
}

#pragma mark - draw
void PopupUserLogin::drawInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();

    if ( auto uiButtons = utils::findChild<ui::Layout*>(layout, "UI_BUTTONS"); uiButtons != nullptr )
    {
        auto uiGooglePlay = utils::findChild<ui::Button*>(uiButtons, "UI_GOOGLEPLAY");
        auto uiGameCenter = utils::findChild<ui::Button*>(uiButtons, "UI_GAMECENTER");
        
        auto platform = AccountManager::getInstance()->getUserPlatform();
        if ( platform.compare("ios") == 0)
        {
            uiGooglePlay->setVisible(false);
            uiGameCenter->setVisible(true);
        }
        else
        {
            uiGooglePlay->setVisible(true);
            uiGameCenter->setVisible(false);
        }
        
        auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
        uiButtons->setContentSize(sizeButtons);
        uiButtons->requestDoLayout();
    }
}

#pragma mark - click
void PopupUserLogin::onClickLogin(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;
    
    auto socialID = AccountManager::getInstance()->getUserSocialID();
    if ( socialID.empty() == true )
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

            if ( _callbackResult != nullptr )
                _callbackResult();
            
            hide();
        });
        return;
    }
    
    if ( _callbackResult != nullptr )
        _callbackResult();
    
    hide();
}

void PopupUserLogin::onClickGuest(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupUserLoginGuest::create();
    popup->setCallbackResult(CC_CALLBACK_0(PopupUserLogin::callbackGuest, this));
    popup->show();
}

#pragma mark - callback
void PopupUserLogin::callbackGuest()
{
    AccountManager::getInstance()->requestGuestRegister(true, [=](bool bResult){
        
        if ( bResult == false )
        {
            std::string strMessage = GAME_TEXT("t_ui_error_30");

            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        
        if ( _callbackResult != nullptr )
            _callbackResult();
        
        hide();
    });
}

#pragma mark - set, get
void PopupUserLogin::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - event
