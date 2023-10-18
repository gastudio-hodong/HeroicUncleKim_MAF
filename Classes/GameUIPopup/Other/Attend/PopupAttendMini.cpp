//
//  PopupAttendMini.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 05/11/2019.
//

#include "PopupAttendMini.h"

#include "GameUIPopup/Other/Attend/PopupAttend.h"

USING_NS_CC;

PopupAttendMini* PopupAttendMini::create()
{
    PopupAttendMini* pRet = new(std::nothrow) PopupAttendMini();
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

PopupAttendMini::PopupAttendMini(void) :
_nTimeHide(0),
_nTimeHideMax(0),
_bHide(false),

_layerContainerContents(nullptr),
_labelButtonClose(nullptr)
{
    
}

PopupAttendMini::~PopupAttendMini(void)
{
    
}

bool PopupAttendMini::init()
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

void PopupAttendMini::onEnter()
{
    PopupBase::onEnter();
    
    //
    this->schedule(schedule_selector(PopupAttendMini::scheduleHide), 1.0);
}

#pragma mark - init
void PopupAttendMini::initVar()
{
    //
    _nTimeHide = 6;
    _nTimeHideMax = 6;
}
void PopupAttendMini::initUi()
{
    Size size = Size(500, 444);
    
    _spriteContainer->setContentSize( size );
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_attend"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    uiContents();
}

void PopupAttendMini::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    auto sprAttendance = Sprite::create("Assets/ui/attendance_icon.png");
    sprAttendance->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 40);
    sprAttendance->setScale(1.5);
    _layerContainerContents->addChild(sprAttendance);
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_attend_msg_4"), GAME_FONT, 24);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 130);
    labelText->setDimensions(_layerContainerContents->getContentSize().width * 0.9, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 4);
    _layerContainerContents->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupAttendMini::onClickClose, this));
    menu->addChild(itemClose);
    {
        std::string strTemp = GAME_TEXT("t_ui_no");
        strTemp.append(MafUtils::format("(%d)", _nTimeHide));
        
        _labelButtonClose = Label::createWithTTF(strTemp, GAME_FONT, 24);
        _labelButtonClose->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(_labelButtonClose);
    }
    
    auto itemDownload = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupAttendMini::onClickAttend, this));
    menu->addChild(itemDownload);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 24);
        labelText->setPosition(itemDownload->getContentSize().width / 2, itemDownload->getContentSize().height / 2);
        itemDownload->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupAttendMini::drawButton()
{
    std::string strTemp = GAME_TEXT("t_ui_no");
    strTemp.append(MafUtils::format("(%d)", _nTimeHide));
    
    _labelButtonClose->setString(strTemp);
}

#pragma mark - set, get, other


#pragma mark - schedule
void PopupAttendMini::scheduleHide(float dt)
{
    if(_bHide)
        return;
//    if ( this->getParent() == nullptr || this->isRunning() == false )
//        return;
    
    _nTimeHide -= 1;
    if ( _nTimeHide >= 0 )
    {
        drawButton();
    }
    if ( _nTimeHide < 0 )
    {
        _bHide = true;
        _nTimeHide = _nTimeHideMax;
        this->unschedule(schedule_selector(PopupAttendMini::scheduleHide));
        
        hide();
    }
}

#pragma mark - click
void PopupAttendMini::onClickClose(cocos2d::Ref* sender)
{
    _bHide = true;
    _nTimeHide = _nTimeHideMax;
    this->unschedule(schedule_selector(PopupAttendMini::scheduleHide));
    
    hide();
}

void PopupAttendMini::onClickAttend(cocos2d::Ref* sender)
{
    _bHide = true;
    _nTimeHide = _nTimeHideMax;
    this->unschedule(schedule_selector(PopupAttendMini::scheduleHide));
    
    auto popup = PopupAttend::create();
    popup->show();
    hide();
}



