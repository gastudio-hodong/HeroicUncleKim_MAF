//
//  PopupAccountTransferAutoCheck.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/09/28.
//

#include "PopupAccountTransferAutoCheck.h"

USING_NS_CC;

PopupAccountTransferAutoCheck* PopupAccountTransferAutoCheck::create(std::string strID)
{
    PopupAccountTransferAutoCheck* pRet = new(std::nothrow) PopupAccountTransferAutoCheck();
    if ( pRet && pRet->init(strID) )
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

PopupAccountTransferAutoCheck::PopupAccountTransferAutoCheck(void) :
_strUUID(""),
_onCallbackReward(nullptr)
{
    
}

PopupAccountTransferAutoCheck::~PopupAccountTransferAutoCheck(void)
{
    
}

bool PopupAccountTransferAutoCheck::init(std::string strID)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _strUUID = strID;
    // init
    initVar();
    initUi();

    return true;
}

#pragma mark - init
void PopupAccountTransferAutoCheck::initVar()
{

    
}

void PopupAccountTransferAutoCheck::initUi()
{
    Size size = Size(500, 480);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_notice"), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAccountTransferAutoCheck::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = "Assets/ui/menu/data_3_1.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path = "Assets/ui/menu/data_3_2.png";
#endif
    auto sprIcon = Sprite::create(path);
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprIcon->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 85);
    _spriteContainer->addChild(sprIcon);
    
    auto uiMsg = ui::CText::create(GAME_TEXTFORMAT("t_ui_transfer_msg_15", _strUUID.c_str()), GAME_FONT, 22);
    uiMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiMsg->setPosition(sprIcon->getPosition() + Vec2::DOWN * (sprIcon->getContentSize().height - 10));
    uiMsg->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 10, 180));
    uiMsg->setTextOverflow(Label::Overflow::SHRINK);
    uiMsg->setTextVerticalAlignment(TextVAlignment::TOP);
    uiMsg->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _spriteContainer->addChild(uiMsg);
    
    auto btnConfirm = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    btnConfirm->setScale9Enabled(true);
    btnConfirm->addClickEventListener(CC_CALLBACK_1(PopupAccountTransferAutoCheck::onClickConfirm, this));
    btnConfirm->setContentSize(Size(366,82));
    btnConfirm->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnConfirm->setZoomScale(0.05f);
    btnConfirm->setPosition(Vec2(_spriteContainer->getContentSize().width/2, 20));
    btnConfirm->setTitleText(GAME_TEXT("t_ui_confirm"));
    btnConfirm->setTitleFontName(GAME_FONT);
    btnConfirm->setTitleFontSize(34);
    _spriteContainer->addChild(btnConfirm);
}

#pragma mark -click
void PopupAccountTransferAutoCheck::onClickClose(cocos2d::Ref* sender)
{
    hide();
}
void PopupAccountTransferAutoCheck::onClickConfirm(cocos2d::Ref* sender)
{
    if(_onCallbackReward != nullptr)
        _onCallbackReward();
    
    hide();
}

#pragma mark -utils

void PopupAccountTransferAutoCheck::setCallback(const std::function<void(void)>& callback)
{
    _onCallbackReward = callback;
}
