//
//  PopupUserNickname.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupUserNickname.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupUserNickname* PopupUserNickname::create()
{
    PopupUserNickname* pRet = new(std::nothrow) PopupUserNickname();
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

PopupUserNickname::PopupUserNickname(void) :
_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr),
_editBox(nullptr)
{
    
}

PopupUserNickname::~PopupUserNickname(void)
{
    
}

bool PopupUserNickname::init()
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

#pragma mark - init
void PopupUserNickname::initVar()
{

}

void PopupUserNickname::initUi()
{
    Size size = Size(497, 400);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_nick_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupUserNickname::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_nick_msg_2"), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    labelText->setDimensions(_layerContainerContents->getContentSize().width * 0.9, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);
    
    // editbox
    _editBox = ui::EditBox::create(Size(395,74), ui::Scale9Sprite::create("Assets/ui/raid/nickname.png"));
    _editBox->setPosition(Vec2(_layerContainerContents->getContentSize().width / 2, 20 + _editBox->getContentSize().height / 2));
    _editBox->setFont(GAME_FONT, 34);
    _editBox->setMaxLength(8);
    _editBox->setInputMode(ui::EditBox::InputMode::URL);
    _editBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    _editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _layerContainerContents->addChild(_editBox);
    {
        auto labelEditboxLength = Label::createWithTTF(GAME_TEXT("t_ui_nick_msg_4"), GAME_FONT, 18);
        labelEditboxLength->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        labelEditboxLength->setPosition(_editBox->getContentSize().width, 0);
        _editBox->addChild(labelEditboxLength);
    }
    
}

void PopupUserNickname::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
   
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupUserNickname::onClickClose, this));
    menu->addChild(itemClose);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 24);
        labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(labelText);
    }
    
    auto itemChange = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupUserNickname::onClickNickChange, this));
    menu->addChild(itemChange);
    {
        if ( AccountManager::getInstance()->isUserNickFirst() == true )
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_nick_msg_5"), GAME_FONT, 24);
            labelText->setPosition(itemChange->getContentSize().width / 2, itemChange->getContentSize().height / 2);
            itemChange->addChild(labelText);
        }
        else
        {
            auto spriteIcon = Sprite::create("Assets/icon/icon_coin_2.png");
            spriteIcon->setPosition(itemChange->getContentSize().width * 0.25, itemChange->getContentSize().height / 2);
            itemChange->addChild(spriteIcon);
            
            auto labelText = Label::createWithTTF("X 120", GAME_FONT, 24);
            labelText->setAnchorPoint(Vec2(0.0,0.5));
            labelText->setPosition(itemChange->getContentSize().width * 0.5, itemChange->getContentSize().height / 2);
            itemChange->addChild(labelText);
        }
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}


#pragma mark - set, get, sort, add
void PopupUserNickname::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupUserNickname::onClickClose(Ref* sender)
{
    hide();
}

void PopupUserNickname::onClickNickChange(cocos2d::Ref* sender)
{
    if (_editBox->getText() == nullptr)
    {
        return;
    }
    
    if (strlen(_editBox->getText()) <= 0)
    {
        return;
    }
    
    std::string strNickname = _editBox->getText();
    MafUtils::eraseCharcter(strNickname, ' ');
    
    strNickname = UtilsString::getInstance()->trim(strNickname);
    
    if ( TextManager::getInstance()->isBadWord(strNickname) == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_nick_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( AccountManager::getInstance()->isUserNickFirst() == false )
    {
        int nCurrencyCoin = atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
        int nPrice = 120;
        if ( nCurrencyCoin < nPrice )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
            popup->show();
            
            hide();
            return;
        }
    }
    
    //
    _editBox->setText(strNickname.c_str());
    
    //
    AccountManager::getInstance()->requestNickChange(true, strNickname, CC_CALLBACK_2(PopupUserNickname::callbackNick, this));
}

#pragma mark - callback
void PopupUserNickname::callbackNick(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        
        if ( nResult == 0 )
            strMessage = GAME_TEXT("t_ui_error_31");
        else if ( nResult == 4 )
            strMessage = GAME_TEXT("t_ui_error_39");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    if ( _callbackResult != nullptr )
    {
        _callbackResult();
    }
    
    hide();
}
 
