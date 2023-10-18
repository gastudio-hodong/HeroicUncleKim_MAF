//
//  PopupDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 05/06/2019.
//

#include "PopupDefault.h"

USING_NS_CC;

PopupDefault* PopupDefault::create(std::string title, std::string text)
{
    auto ret = new (std::nothrow) PopupDefault();
    if (ret && ret->init(title, text))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

PopupDefault::PopupDefault(void) :
_bInitUI(false),

_title(""),
_text(""),

_callbackBack(nullptr),

_sizeContents(Size(500, 250)),
_hAlignment(TextHAlignment::CENTER),
_vAlignment(TextVAlignment::CENTER),

//
_uiContentsTitle(nullptr),
_uiContentsText(nullptr),
_uiContentsButtons(nullptr)
{
    
}

PopupDefault::~PopupDefault(void)
{
    
}

bool PopupDefault::init(std::string title, std::string text)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _title = title;
    _text = text;

    //
    initVar();
    initUi();
    
    return true;
}

void PopupDefault::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        uiContainer();
        uiTitle();
        uiText();
        uiButtons();
        
        _bInitUI = true;
    }
    
    //
    drawTitle();
    drawText();
    drawButtons();
}

void PopupDefault::backkey()
{
    if ( _callbackBack != nullptr )
    {
        _callbackBack(this);
    }
    
    hide();
}

#pragma mark - init
void PopupDefault::initVar()
{
    
}

void PopupDefault::initUi()
{
    
}

#pragma mark - ui
void PopupDefault::uiContainer()
{
    double width = _sizeContents.width;
    double height = _sizeContents.height + 75 + 119;
    _spriteContainer->setContentSize(Size(width, height));
    
    auto layout = _spriteContainer;
    auto size = layout->getContentSize();
    
    //
    _uiContentsTitle = ui::Layout::create();
    _uiContentsTitle->setContentSize( Size(size.width, 75) );
    _uiContentsTitle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsTitle->setPositionType(ui::Widget::PositionType::PERCENT);
    _uiContentsTitle->setPositionPercent(Vec2::ANCHOR_MIDDLE_TOP);
    layout->addChild(_uiContentsTitle);
    
    _uiContentsButtons = ui::Layout::create();
    _uiContentsButtons->setContentSize( Size(size.width, 119) );
    _uiContentsButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsButtons->setPositionType(ui::Widget::PositionType::PERCENT);
    _uiContentsButtons->setPositionPercent(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->addChild(_uiContentsButtons);
    
    _uiContentsText = ui::Layout::create();
    _uiContentsText->setContentSize( Size(size.width, size.height - _uiContentsTitle->getContentSize().height - _uiContentsButtons->getContentSize().height) );
    _uiContentsText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsText->setPosition(Vec2(size.width / 2, _uiContentsButtons->getContentSize().height));
    layout->addChild(_uiContentsText);
    
    
}

void PopupDefault::uiTitle()
{
    auto layout = _uiContentsTitle;
    auto size = layout->getContentSize();
    
    //
    auto uiTitle = ui::CText::create(" ", GAME_FONT, 28);
    uiTitle->setPositionType(ui::Widget::PositionType::PERCENT);
    uiTitle->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiTitle->setTextAreaSize(Size(size.width - 10, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setColor(Color3B(255,223,28));
    uiTitle->enableBold();
    uiTitle->setName("UI_TITLE");
    layout->addChild(uiTitle);
}

void PopupDefault::uiText()
{
    auto layout = _uiContentsText;
    auto size = layout->getContentSize();
    
    //
    auto uiText = ui::CText::create(" ", GAME_FONT, 24);
    uiText->setPositionType(ui::Widget::PositionType::PERCENT);
    uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiText->setTextAreaSize(Size(size.width - 10, size.height - 10));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiText->setName("UI_TEXT");
    layout->addChild(uiText);
}

void PopupDefault::uiButtons()
{
    auto layout = _uiContentsButtons;
    auto size = layout->getContentSize();
    
    auto uiBtns = ui::Layout::create();
    uiBtns->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBtns->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtns->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiBtns->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiBtns->setName("UI_BTNS");
    layout->addChild(uiBtns);
}

#pragma mark - ui draw
void PopupDefault::drawTitle()
{
    auto layout = _uiContentsTitle;
    
    if ( auto uiTitle = layout->getChildByName<ui::CText*>("UI_TITLE"); uiTitle != nullptr )
    {
        uiTitle->setString(_title);
    }
}

void PopupDefault::drawText()
{
    auto layout = _uiContentsText;
    
    if ( auto uiText = layout->getChildByName<ui::CText*>("UI_TEXT"); uiText != nullptr )
    {
        uiText->setString(_text);
        uiText->setTextVerticalAlignment(_vAlignment);
        uiText->setTextHorizontalAlignment(_hAlignment);
    }
}

void PopupDefault::drawButtons()
{
    auto layout = _uiContentsButtons;
    
    if ( auto uiBtns = layout->getChildByName<ui::Layout*>("UI_BTNS"); uiBtns != nullptr )
    {
        uiBtns->removeAllChildren();
        
        for ( int i = 0; i < _listButton.size(); i++ )
        {
            auto uiBtn = _listButton.at(i);
            uiBtns->addChild(uiBtn);
            
            if ( i != 0 )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiBtn->setLayoutParameter(params);
            }
        }
        
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiBtns);
        uiBtns->setContentSize(size);
        uiBtns->requestDoLayout();
    }
}

#pragma mark - set, get
void PopupDefault::setContentSize(cocos2d::Size size)
{
    // size
    _sizeContents = size;
    
    if ( _bInitUI == true )
    {
        double width = _sizeContents.width;
        double height = _sizeContents.height + _uiContentsTitle->getContentSize().height + _uiContentsButtons->getContentSize().height;
        _spriteContainer->setContentSize(Size(width, height));
        _uiContentsText->setContentSize( Size(width, height - _uiContentsTitle->getContentSize().height - _uiContentsButtons->getContentSize().height) );
    }
}

void PopupDefault::setContentPosition(double x, double y)
{
    _spriteContainer->setPosition(x, y);
}

void PopupDefault::setContentAlignment(TextHAlignment hAlignment,TextVAlignment vAlignment)
{
    if ( hAlignment == _hAlignment && vAlignment == _vAlignment )
    {
        return;
    }
    
    _hAlignment = hAlignment;
    _vAlignment = vAlignment;
    
    //
    if ( _bInitUI == true )
    {
        drawText();
    }
}

ui::Button* PopupDefault::addButton(std::string strText, bool bDefault, const std::function<void(Ref*)>& callback, bool bHide)
{
    //
    auto uiBtn = ui::Button::create("Assets/ui_common/popup_btn_no.png");
    uiBtn->addClickEventListener([=](Ref* sender){

        if ( callback != nullptr )
        {
            callback(this);
        }

        if ( bHide == true )
        {
            hide();
        }
    });
    _listButton.pushBack(uiBtn);

    //
    auto labelText = Label::createWithTTF(strText, GAME_FONT, 24);
    labelText->setPosition( Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2) );
    uiBtn->addChild(labelText);


    // callback back key
    if ( bDefault == true )
    {
        _callbackBack = callback;
    }


    if ( _bInitUI == true )
    {
        drawButtons();
    }

    return uiBtn;
}
