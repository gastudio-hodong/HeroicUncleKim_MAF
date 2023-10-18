//
//  PopupReviveDetailDevil.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 28/06/2019.
//

#include "PopupReviveDetailDevil.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupReviveDetailDevil* PopupReviveDetailDevil::create(int nType)
{
    PopupReviveDetailDevil* pRet = new(std::nothrow) PopupReviveDetailDevil();
    if ( pRet && pRet->init(nType) )
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

PopupReviveDetailDevil::PopupReviveDetailDevil(void) :
_bAutoSave(false),
_nType(0),

_onCallbackResult(nullptr),

_layerContainerContent(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupReviveDetailDevil::~PopupReviveDetailDevil(void)
{
    
}

bool PopupReviveDetailDevil::init(int nType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _nType = nType;
    
    _bAutoSave = UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_REVIVE_DEVIL",false);
    UserDefault::getInstance()->setBoolForKey("AUTO_SAVE_REVIVE_DEVIL", _bAutoSave);
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupReviveDetailDevil::initVar()
{
    
}

void PopupReviveDetailDevil::initUi()
{
    //
    Size size = Size(500, 75);
    size.height += 200;     // content
    size.height += 88;     // button
    size.height += 25;      // ton padding
    size.height += 25;      // middle padding
    size.height += 25;      // buttom padding
    _spriteContainer->setContentSize( size );
    
    //
    std::string strTitle = "";
    if ( _nType == 0 )
    {
        strTitle = GAME_TEXT("t_ui_revival_1");
    }
    else if ( _nType == 1 )
    {
        strTitle = GAME_TEXT("t_ui_revival_2");
    }
    else
    {
        strTitle = GAME_TEXT("t_ui_revival_3");
    }
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    
    //
    _layerContainerContent = Layer::create();
    _layerContainerContent->setContentSize( Size(size.width - 50, 200) );
    _layerContainerContent->setIgnoreAnchorPointForPosition(false);
    _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContent);
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 88) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    
    //
    uiContent();
    uiButton();
    uiAutoSave();
}

#pragma mark - ui
void PopupReviveDetailDevil::uiContent()
{
    _layerContainerContent->removeAllChildren();
    
    std::string strTitle = "";
    if ( _nType == 0 )
    {
        strTitle = GAME_TEXT("t_ui_revival_1");
    }
    else if ( _nType == 1 )
    {
        strTitle = GAME_TEXT("t_ui_revival_2");
    }
    else
    {
        strTitle = GAME_TEXT("t_ui_revival_3");
    }

    auto labelText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_revive_msg_12", strTitle.c_str()), GAME_FONT, 28);
    labelText->setPosition(_layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height / 2);
    labelText->setDimensions(_layerContainerContent->getContentSize().width, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelText);
}

void PopupReviveDetailDevil::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);


    //
    auto spriteNo = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteNo->setContentSize(Size(132, 88));
    auto spriteSelectNo = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectNo->setContentSize(Size(132, 88));
    
    auto itemNo = MafNode::MafMenuItemSprite::create(spriteNo, spriteSelectNo, nullptr, CC_CALLBACK_1(PopupReviveDetailDevil::onClickClose, this));
    menu->addChild(itemNo);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 24);
        label->setPosition( Vec2(itemNo->getContentSize().width / 2, itemNo->getContentSize().height / 2) );
        itemNo->addChild(label);
    }

    //
    auto spriteYes = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteYes->setContentSize(Size(132, 88));
    auto spriteSelectYes = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectYes->setContentSize(Size(132, 88));
    
    auto itemYes = MafNode::MafMenuItemSprite::create(spriteYes, spriteSelectYes, nullptr, CC_CALLBACK_1(PopupReviveDetailDevil::onClickYes, this));
    menu->addChild(itemYes);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 24);
        label->setPosition( Vec2(itemYes->getContentSize().width / 2, itemYes->getContentSize().height / 2) );
        itemYes->addChild(label);
    }
    
    //
    auto spriteAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_4_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteAds->setContentSize(Size(132, 88));
    auto spriteSelectAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_4_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectAds->setContentSize(Size(132, 88));
    
    auto itemAds = MafNode::MafMenuItemSprite::create(spriteAds, spriteSelectAds, nullptr, CC_CALLBACK_1(PopupReviveDetailDevil::onClickAds, this));
    menu->addChild(itemAds);
    {
        auto spriteIcon = Sprite::create("Assets/ui_common/btn_ads_play.png");
        spriteIcon->setPosition(itemAds->getContentSize().width / 2, 55);
        itemAds->addChild(spriteIcon);
        
        auto spriteSoul = Sprite::create("Assets/icon/icon_gold_2.png");
        spriteSoul->setPosition(itemAds->getContentSize().width / 2 - 15, 25);
        itemAds->addChild(spriteSoul);
        
        auto labelText = Label::createWithTTF("+10%", GAME_FONT, 16);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition( Vec2(itemAds->getContentSize().width / 2, 25) );
        itemAds->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupReviveDetailDevil::uiAutoSave()
{
    std::string strCheckPath = "Assets/ui/mission/mission_off.png";
    if ( _bAutoSave == true )
    {
        strCheckPath = "Assets/ui/mission/mission_on.png";
    }
    
    auto btnAutoSaveCheck = ui::Button::create(strCheckPath);
    btnAutoSaveCheck->addClickEventListener(CC_CALLBACK_1(PopupReviveDetailDevil::onClickAutoSaveCheck, this));
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


#pragma mark - click
void PopupReviveDetailDevil::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupReviveDetailDevil::onClickYes(cocos2d::Ref* sender)
{
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult(_nType, 1);
    }
    
    if(_bAutoSave)
    {
        SaveManager::getInstance()->requestSave(nullptr);
    }
    
    hide();
}

void PopupReviveDetailDevil::onClickAds(cocos2d::Ref* sender)
{
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult(_nType, 2);
    }
    
    if(_bAutoSave)
    {
        SaveManager::getInstance()->requestSave(nullptr);
    }
    
    hide();
}

void PopupReviveDetailDevil::onClickAutoSaveCheck(Ref *sender)
{
    auto btn = (ui::Button*)sender;
    _bAutoSave = !UserDefault::getInstance()->getBoolForKey("AUTO_SAVE_REVIVE_DEVIL",false);
    UserDefault::getInstance()->setBoolForKey("AUTO_SAVE_REVIVE_DEVIL", _bAutoSave);
    
    std::string strCheckPath = "Assets/ui/mission/mission_off.png";
    if ( _bAutoSave == true )
    {
        strCheckPath = "Assets/ui/mission/mission_on.png";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_auto_save_pop_reconfirmation"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    btn->getRendererNormal()->setTexture(strCheckPath);
    
}

#pragma mark - set, get, other
void PopupReviveDetailDevil::setCallbackResult(const std::function<void(int, int)>& callback)
{
    _onCallbackResult = callback;
}


#pragma mark - callback
