//
//  MailReceivePopup.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 25/06/2019.
//

#include "MailReceivePopup.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Settings/SaveLoad/PopupToastAutoSave.h"

#include "ManagerGame/SaveManager.h"

USING_NS_CC;

MailReceivePopup* MailReceivePopup::create(cocos2d::Vector<InfoItems*> listReward, int nSuccess, int nError)
{
    MailReceivePopup* pRet = new(std::nothrow) MailReceivePopup();
    if ( pRet && pRet->init(listReward, nSuccess, nError) )
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

MailReceivePopup::MailReceivePopup(void) :
_bAutoSave(false),
_nSuccess(0),
_nError(0),
_sizeContents(cocos2d::Size::ZERO),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

MailReceivePopup::~MailReceivePopup(void)
{
    
}

bool MailReceivePopup::init(cocos2d::Vector<InfoItems*> listReward, int nSuccess, int nError)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _listReward = listReward;
    _nSuccess = nSuccess;
    _nError = nError;
    
    _bAutoSave = UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_GIFT",false);
    UserDefault::getInstance()->setBoolForKey("AUTO_SAVE_GIFT", _bAutoSave);
    
    // init
    initVar();
    initUi();
    
    
    return true;
}

void MailReceivePopup::initVar()
{
    const int paddingX = 20;
    const int paddingY = 20;
    const int maxCol = 3;
    
    int drawRow = (int)_listReward.size() / maxCol;
    if ( _listReward.size() % maxCol != 0 )
    {
        drawRow += 1;
    }
    
    int drawCol = maxCol;
    if ( drawRow == 1 && _listReward.size() % maxCol != 0 )
    {
        drawCol = _listReward.size() % maxCol;
    }
    
    int width = 110 * drawCol + paddingX * (drawCol - 1);
    int height = 110 * drawRow + paddingY * (drawRow - 1);
    
    _sizeContents = Size(width, height);
}

void MailReceivePopup::initUi()
{
    Size size = _sizeContents;
    size.height += 75;      // title
    size.height += 25;      // ton padding
    size.height += 25;      // middle padding
    size.height += 25;      // buttom padding
    
    // button
    double heightButton = 88;
    size.height += heightButton;
    
    // text
    double heightText = 100;
    if ( _nError != 0 )
    {
        heightText += 50;
    }
    size.height += heightText;
    
    size.width = 450;
    if ( size.height < 568 )
        size.height = 568;
    
    
    //
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_present"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 75 - heightButton - heightText) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    //
    _layerContainerText = Layer::create();
    _layerContainerText->setContentSize( Size(size.width - 50, heightText) );
    _layerContainerText->setIgnoreAnchorPointForPosition(false);
    _layerContainerText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerText->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _layerContainerContents->getPositionY() - _layerContainerContents->getContentSize().height) );
    _spriteContainer->addChild(_layerContainerText);
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 88) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    
    //
    uiContents();
    uiText();
    uiButton();
    uiAutoSave();
}

#pragma mark - ui
void MailReceivePopup::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    auto layerReward = Layer::create();
    layerReward->setContentSize( _sizeContents );
    layerReward->setIgnoreAnchorPointForPosition(false);
    layerReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerReward->setPosition( _layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2 );
    _layerContainerContents->addChild(layerReward);
    
    //
    const int paddingX = 20;
    const int paddingY = 20;
    const int maxCol = 3;
    
    int drawRow = (int)_listReward.size() / maxCol;
    if ( _listReward.size() % maxCol != 0 )
    {
        drawRow += 1;
    }
    
    int drawCol = maxCol;
    if ( drawRow == 1 && _listReward.size() % maxCol != 0 )
    {
        drawCol = _listReward.size() % maxCol;
    }
    
    for ( int i = 0; i < _listReward.size(); i++ )
    {
        int x = i % maxCol;
        int y = i / maxCol;
        int posX = x * 110 + x * paddingX;
        int posY = layerReward->getContentSize().height - y * 110 - y * paddingY;
        
        auto spriteRewardBg = Sprite::create("Assets/ui_common/box_reward.png");
        spriteRewardBg->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        spriteRewardBg->setPosition(posX, posY);
        layerReward->addChild(spriteRewardBg);
        
        auto obj = _listReward.at(i);
        std::string strPathIcon = obj->getPath();
        std::string strCount = obj->getCount();
        if ( obj->isString() == true )
        {
            MafUtils::convertNumberToShort(strCount);
        }
        
        if ( strPathIcon.length() == 0 || strCount.length() == 0 )
        {
            continue;
        }
        
        auto spriteIcon = Sprite::create(strPathIcon);
        if(spriteIcon != nullptr)
        {
            spriteIcon->setPosition(spriteRewardBg->getContentSize().width / 2, spriteRewardBg->getContentSize().height / 2 + 10);
            spriteIcon->setScale(1.5);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteRewardBg->addChild(spriteIcon);
        }
        
        auto label = Label::createWithTTF(strCount, GAME_FONT, 24);
        label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        label->setPosition(spriteRewardBg->getContentSize().width / 2, 5);
        label->enableBold();
        spriteRewardBg->addChild(label);
    }
}

void MailReceivePopup::uiText()
{
    //
    _layerContainerText->removeAllChildren();
    
    auto spriteTip = Sprite::create("Assets/ui/mail/icon_tip.png");
    spriteTip->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    spriteTip->setPosition( Vec2(50, _layerContainerText->getContentSize().height) );
    _layerContainerText->addChild(spriteTip);
    
    auto labelContents = Label::createWithTTF(GAME_TEXT("t_ui_mail_msg_2"), GAME_FONT, 28);
    labelContents->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    labelContents->setPosition( Vec2(50, _layerContainerText->getContentSize().height - spriteTip->getContentSize().height) );
    labelContents->setColor(COLOR_COIN);
    labelContents->setDimensions(_layerContainerText->getContentSize().width - 100, labelContents->getContentSize().height);
    labelContents->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelContents->setOverflow(Label::Overflow::SHRINK);
    _layerContainerText->addChild(labelContents);


    if ( _nError > 0 )
    {
        auto labelError = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_mail_msg_5", _nError), GAME_FONT, 20);
        labelError->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelError->setPosition(_layerContainerText->getContentSize().width / 2, 0);
        labelError->setDimensions(_layerContainerText->getContentSize().width, labelError->getContentSize().height);
        labelError->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelError->setOverflow(Label::Overflow::SHRINK);
        _layerContainerText->addChild(labelError);
    }
    
}

void MailReceivePopup::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto spriteConfirm = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteConfirm->setContentSize(Size(165, 89));
    
    auto itemConfirm = MafNode::MafMenuItemSprite::create(spriteConfirm, nullptr, CC_CALLBACK_1(MailReceivePopup::onClickClose, this));
    menu->addChild(itemConfirm);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
        label->setPosition(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
        label->setDimensions(itemConfirm->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        itemConfirm->addChild(label);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(40);
}

void MailReceivePopup::uiAutoSave()
{
    std::string strCheckPath = "Assets/ui/mission/mission_off.png";
    if ( _bAutoSave == true )
    {
        strCheckPath = "Assets/ui/mission/mission_on.png";
    }
    
    auto btnAutoSaveCheck = ui::Button::create(strCheckPath);
    btnAutoSaveCheck->addClickEventListener(CC_CALLBACK_1(MailReceivePopup::onClickAutoSaveCheck, this));
    btnAutoSaveCheck->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnAutoSaveCheck->setScale(0.8f);
    btnAutoSaveCheck->setZoomScale(0.05f);
    btnAutoSaveCheck->setPosition(Vec2(_spriteContainer->getContentSize().width, -10));
    _spriteContainer->addChild(btnAutoSaveCheck);
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_auto_save_check"), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    labelText->setPosition(btnAutoSaveCheck->getPosition() + Vec2::LEFT * (btnAutoSaveCheck->getContentSize().width * btnAutoSaveCheck->getScale() + 5));
    labelText->setDimensions(labelText->getContentSize().width, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::RIGHT, TextVAlignment::TOP);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelText);
}

#pragma mark - set, get, other

#pragma mark - click
void MailReceivePopup::onClickClose(Ref *sender)
{
    //
    if(_bAutoSave)
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestSave(CC_CALLBACK_1(MailReceivePopup::callbackSave, this));
        return;
    }
    hide();
}

void MailReceivePopup::onClickAutoSaveCheck(Ref *sender)
{
    auto btn = (ui::Button*)sender;
    _bAutoSave = !UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_GIFT",false);
    UserDefault::getInstance()->setBoolForKey("AUTO_SAVE_GIFT", _bAutoSave);
    
    std::string strCheckPath = "Assets/ui/mission/mission_off.png";
    if ( _bAutoSave == true )
    {
        strCheckPath = "Assets/ui/mission/mission_on.png";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_auto_save_pop_gift"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    btn->getRendererNormal()->setTexture(strCheckPath);
    
}

void MailReceivePopup::callbackSave(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    showAutoSaveToast();
    hide();
}

void MailReceivePopup::showAutoSaveToast()
{
    auto toast = PopupToastAutoSave::create();
    toast->setPosition(getContentSize().width/2, getContentSize().height*0.75);
    toast->show();
}
