//
//  PopupEnterCount.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 11/10/2019.
//

#include "PopupEnterCount.h"

#include "ManagerGame/PrisonManager.h"

USING_NS_CC;

PopupEnterCount::PopupEnterCount() :
_onCallbackEnter(nullptr),
_onCallbackEnterBafometz(nullptr),

_nHeartCount(0),
_nCount(0),

_fUpgradeTime(0),
_fUpgradeTimeMax(0.2),

// widget
_uiContentsUIInfo(nullptr),
_uiContentsUIButtons(nullptr),

_uiTextCount(nullptr),

_uiContentArrowL(nullptr),
_uiContentArrowR(nullptr),

_uiBtnEnter(nullptr),
_uiBtnEnterBafometz(nullptr)
{
    
}

PopupEnterCount::~PopupEnterCount(void)
{
    
}

bool PopupEnterCount::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupEnterCount::onEnter()
{
    PopupBase::onEnter();
    
    //
    scheduleUpdate();
}

void PopupEnterCount::update(float dt)
{
    if ( _uiContentArrowL != nullptr && _uiContentArrowL->isHighlighted() == true )
    {
        _fUpgradeTime += dt;
        if (_fUpgradeTimeMax <= _fUpgradeTime)
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax -= 0.02f;
            
            onClickCountUpDown(_uiContentArrowL);
        }
    }
    else if ( _uiContentArrowR != nullptr && _uiContentArrowR->isHighlighted() == true )
    {
        _fUpgradeTime += dt;
        if (_fUpgradeTimeMax <= _fUpgradeTime)
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax -= 0.02f;
            
            onClickCountUpDown(_uiContentArrowR);
        }
    }
    else
    {
        _fUpgradeTime = 0;
        _fUpgradeTimeMax = 0.2f;
    }
}

void PopupEnterCount::initVar()
{
    _nHeartCount = PrisonManager::getInstance()->getHeart();
    if ( _nHeartCount > 0 )
    {
        _nCount = 1;
    }
}

void PopupEnterCount::initUi()
{
    Size size = Size(500, 444);
    
    // size
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison_enter_bt"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    labelTitle->setDimensions(size.width * 0.75, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEnterCount::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    {
        double width = 492;
        double heightUIButton = 89;
        double heightUIInfo = size.height - 75 - heightUIButton - 25;
        
        _uiContentsUIInfo = ui::Layout::create();
        _uiContentsUIInfo->setContentSize(Size(width, heightUIInfo));
        _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiContentsUIInfo->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
        _spriteContainer->addChild(_uiContentsUIInfo);
        
        _uiContentsUIButtons = ui::Layout::create();
        _uiContentsUIButtons->setContentSize(Size(width, heightUIButton));
        _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIButtons->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
        _uiContentsUIButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
        _spriteContainer->addChild(_uiContentsUIButtons);
    }

    // ui
    uiInfo();
    uiButtons();
}

void PopupEnterCount::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    auto size = _uiContentsUIInfo->getContentSize();
    
    
    auto uiTextDescBG = ui::ImageView::create("Assets/ui_common/base_box_1_3.png");
    uiTextDescBG->setScale9Enabled(true);
    uiTextDescBG->setContentSize(Size(size.width, 124));
    uiTextDescBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTextDescBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiTextDescBG);
    {
        auto uiTextDesc = ui::CText::create(GAME_TEXT("t_ui_prison_enter_msg"), GAME_FONT, 28);
        uiTextDesc->setPositionType(ui::Widget::PositionType::PERCENT);
        uiTextDesc->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiTextDesc->setTextAreaSize(Size(uiTextDescBG->getContentSize().width - 10, uiTextDescBG->getContentSize().height - 10));
        uiTextDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiTextDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTextDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTextDescBG->addChild(uiTextDesc);
    }
    
    auto uiLayoutEnter = ui::Layout::create();
    uiLayoutEnter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutEnter->setPosition(Vec2(size.width / 2, 100));
    uiLayoutEnter->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayoutEnter->setName("UI_LAYOUT_ENTER");
    layout->addChild(uiLayoutEnter);
    
    
    auto uiTextCountBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_numbox.png");
    uiTextCountBG->setPosition(Vec2(size.width / 2, 50));
    layout->addChild(uiTextCountBG);
    {
        _uiTextCount = ui::CText::create("0", GAME_FONT, 32);
        _uiTextCount->setPositionType(ui::Widget::PositionType::PERCENT);
        _uiTextCount->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        _uiTextCount->setTextColor(Color4B(255, 216, 0, 255));
        uiTextCountBG->addChild(_uiTextCount);
    }
    
    _uiContentArrowL = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_l.png");
    _uiContentArrowL->setPosition(Vec2(size.width / 2 - 90, 50));
    _uiContentArrowL->addClickEventListener(CC_CALLBACK_1(PopupEnterCount::onClickCountUpDown, this));
    _uiContentArrowL->setTag(-1);
    layout->addChild(_uiContentArrowL);
    
    auto uiContentArrowLD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_l.png");
    uiContentArrowLD->setPosition(Vec2(size.width / 2 - 150, 50));
    uiContentArrowLD->addClickEventListener(CC_CALLBACK_1(PopupEnterCount::onClickCountUpDown, this));
    uiContentArrowLD->setTag(-10);
    layout->addChild(uiContentArrowLD);
    
    _uiContentArrowR = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_r.png");
    _uiContentArrowR->setPosition(Vec2(size.width / 2 + 90, 50));
    _uiContentArrowR->addClickEventListener(CC_CALLBACK_1(PopupEnterCount::onClickCountUpDown, this));
    _uiContentArrowR->setTag(1);
    layout->addChild(_uiContentArrowR);
    
    auto uiContentArrowRD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_r.png");
    uiContentArrowRD->setPosition(Vec2(size.width / 2 + 150, 50));
    uiContentArrowRD->addClickEventListener(CC_CALLBACK_1(PopupEnterCount::onClickCountUpDown, this));
    uiContentArrowRD->setTag(10);
    layout->addChild(uiContentArrowRD);
    
    // draw
    drawInfo();
}

void PopupEnterCount::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    auto size = _uiContentsUIButtons->getContentSize();
    
    {
        _uiBtnEnter = ui::Button::create("Assets/ui_common/popup_btn_no.png");
        _uiBtnEnter->addClickEventListener(CC_CALLBACK_1(PopupEnterCount::onClickEnter, this));
        layout->addChild(_uiBtnEnter);
        {
            auto uiTextCount = ui::CText::create(GAME_TEXT("t_ui_prison_enter"), GAME_FONT, 28);
            uiTextCount->setPositionType(ui::Widget::PositionType::PERCENT);
            uiTextCount->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiTextCount->setTextAreaSize(Size(200, uiTextCount->getContentSize().height));
            uiTextCount->setTextOverflow(Label::Overflow::SHRINK);
            uiTextCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
            _uiBtnEnter->addChild(uiTextCount);
        }
    }
    
    {
        _uiBtnEnterBafometz = ui::Button::create("Assets/ui_common/popup_btn_no.png");
        _uiBtnEnterBafometz->addClickEventListener(CC_CALLBACK_1(PopupEnterCount::onClickEnterBafometz, this));
        layout->addChild(_uiBtnEnterBafometz);
        {
            auto uiTextCount = ui::CText::create(GAME_TEXT("t_ui_prison_bafometz_gift_bt"), GAME_FONT, 28);
            uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextCount->setPosition(Vec2(70, _uiBtnEnterBafometz->getContentSize().height / 2));
            uiTextCount->setTextAreaSize(Size(130, uiTextCount->getContentSize().height));
            uiTextCount->setTextOverflow(Label::Overflow::SHRINK);
            uiTextCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
            _uiBtnEnterBafometz->addChild(uiTextCount);

            auto uiImgIcon = ui::ImageView::create("Assets/ui/prison/icon_baformetz_1.png");
            uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiImgIcon->setPosition(Vec2(70, _uiBtnEnterBafometz->getContentSize().height / 2));
            uiImgIcon->setScale(0.8);
            _uiBtnEnterBafometz->addChild(uiImgIcon);
        }
        
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        _uiBtnEnterBafometz->setLayoutParameter(params);
    }
    
    // draw
    drawButtons();
}

void PopupEnterCount::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    
    //
    auto uiLayoutEnter = layout->getChildByName<ui::Layout*>("UI_LAYOUT_ENTER");
    uiLayoutEnter->removeAllChildren();
    {
        auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_freepass_2.png");
        uiLayoutEnter->addChild(uiImgIcon);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
            uiImgIcon->setLayoutParameter(params);
        }
        
        auto uiTextEnter = ui::CText::create(MafUtils::format("x%d", PrisonManager::getInstance()->getHeart()), GAME_FONT_AL, 28);
        uiLayoutEnter->addChild(uiTextEnter);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
            uiTextEnter->setLayoutParameter(params);
        }
        
        //
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutEnter);
        uiLayoutEnter->setContentSize(size);
        uiLayoutEnter->setPositionX(layout->getContentSize().width / 2);
        uiLayoutEnter->requestDoLayout();
    }
    
    //
    _uiTextCount->setString(MafUtils::toString(_nCount));
    
}
void PopupEnterCount::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT )
    {
        _uiBtnEnter->setVisible(true);
        _uiBtnEnterBafometz->setVisible(true);
    }
    else
    {
        _uiBtnEnter->setVisible(true);
        _uiBtnEnterBafometz->setVisible(false);
    }
    
    //
    auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(layout);
    layout->setContentSize(size);
    layout->setPositionX(layout->getParent()->getContentSize().width / 2);
    layout->requestDoLayout();
}

#pragma mark - click
void PopupEnterCount::onClickClose(Ref* sender)
{
    hide();
}

void PopupEnterCount::onClickCountUpDown(Ref* sender)
{
    auto uiButton = (ui::Button *)sender;
    
    int count = uiButton->getTag();;
    if ( count <= 0 )
    {
        _nCount += count;
        if ( _nCount <= 0 )
        {
            _nCount = _nHeartCount;
        }
    }
    else
    {
        _nCount += count;
        if ( _nCount > _nHeartCount )
        {
            _nCount = 1;
        }
    }
    
    if ( _nCount > _nHeartCount )
    {
        _nCount = _nHeartCount;
    }
    
    //
    _uiTextCount->setString(MafUtils::toString(_nCount));
}

void PopupEnterCount::onClickEnter(Ref* sender)
{
    if ( _nHeartCount <= 0 || _nCount <= 0 )
    {
        return;
    }
    
    if ( _onCallbackEnter != nullptr )
    {
        _onCallbackEnter(_nCount);
        hide();
    }
}

void PopupEnterCount::onClickEnterBafometz(Ref* sender)
{
    if ( _nHeartCount <= 0 || _nCount <= 0 )
    {
        return;
    }
    
    if ( _onCallbackEnterBafometz != nullptr )
    {
        _onCallbackEnterBafometz(_nCount);
        hide();
    }
}

void PopupEnterCount::setCallbackEnter(const std::function<void(int)>& callback)
{
    _onCallbackEnter = callback;
}

void PopupEnterCount::setCallbackEnterBafometz(const std::function<void(int)>& callback)
{
    _onCallbackEnterBafometz = callback;
}
