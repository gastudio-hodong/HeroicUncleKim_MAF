//
//  PopupUserLoginGuest.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupUserLoginGuest.h"

USING_NS_CC;

PopupUserLoginGuest::PopupUserLoginGuest():
_bInitUI(false),

//
_callbackResult(nullptr),

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr)
{
    
}

PopupUserLoginGuest::~PopupUserLoginGuest(void)
{
    
}

bool PopupUserLoginGuest::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    return true;
}

void PopupUserLoginGuest::onEnter()
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
void PopupUserLoginGuest::initVar()
{

}

void PopupUserLoginGuest::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(594, 538) );
    
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
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupUserLoginGuest::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    uiContainer();
    uiInfo();
    uiButton();
}

void PopupUserLoginGuest::uiContainer()
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
 
void PopupUserLoginGuest::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
   
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(554, 286));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height - 10));
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_popup_msg"), GAME_FONT, 24);
    uiText->setPositionType(ui::Widget::PositionType::PERCENT);
    uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiText->setTextAreaSize(Size(uiBG->getContentSize().width - 10, uiBG->getContentSize().height - 10));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiBG->addChild(uiText);
}

void PopupUserLoginGuest::uiButton()
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
        uiOk->addClickEventListener(CC_CALLBACK_1(PopupUserLoginGuest::onClickOk, this));
        uiButton->addChild(uiOk);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_popup_bt"), GAME_FONT, 24);
            uiText->setPosition(uiOk->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiOk->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiOk->addChild(uiText);
        }
        
        //
        auto uiCancel = ui::Button::create("Assets/ui_common/popup_btn_no.png");
        uiCancel->setScale9Enabled(true);
        uiCancel->setContentSize(Size(260, 88));
        uiCancel->addClickEventListener(CC_CALLBACK_1(PopupUserLoginGuest::onClickClose, this));
        uiButton->addChild(uiCancel);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(20, 0, 0, 0));
            uiCancel->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_cancel"), GAME_FONT, 24);
            uiText->setPosition(uiCancel->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiCancel->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiCancel->addChild(uiText);
        }
    }
}

#pragma mark - draw
void PopupUserLoginGuest::drawInfo()
{
    auto layout = _uiContentsInfo;


    
}

void PopupUserLoginGuest::drawButton()
{
    auto layout = _uiContentsButton;
    
    auto uiButton = layout->getChildByName<ui::Layout*>("UI_BUTTON");
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButton);
    uiButton->setContentSize(sizeButtons);
    uiButton->requestDoLayout();
}

#pragma mark - click
void PopupUserLoginGuest::onClickClose(Ref* sender)
{
    hide();
}

void PopupUserLoginGuest::onClickOk(Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult();
        _callbackResult = nullptr;
    }
    
    hide();
}

#pragma mark - callback

#pragma mark - set, get
void PopupUserLoginGuest::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}
