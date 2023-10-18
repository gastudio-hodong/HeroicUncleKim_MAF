//
//  PopupWithdrawal.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/04.
//

#include "PopupWithdrawal.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

PopupWithdrawal* PopupWithdrawal::create()
{
    PopupWithdrawal* pRet = new(std::nothrow) PopupWithdrawal();
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

PopupWithdrawal::PopupWithdrawal(void) :
_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr),
_editBox(nullptr)
{
    
}

PopupWithdrawal::~PopupWithdrawal(void)
{
    
}

bool PopupWithdrawal::init()
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
void PopupWithdrawal::initVar()
{

}

void PopupWithdrawal::initUi()
{
    Size size = Size(550, 500);
    
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
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_withdrawal_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupWithdrawal::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    std::string strKey = ParameterManager::getInstance()->getParameterString("withdrawal_key");
    
    auto labelText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_withdrawal_info_2", strKey.c_str()), GAME_FONT, 24);
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

void PopupWithdrawal::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
   
    
    auto itemWithdrawal = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupWithdrawal::onClickWithdrawal, this));
    menu->addChild(itemWithdrawal);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_withdrawal_bt_1"), GAME_FONT, 24);
        labelText->setPosition(itemWithdrawal->getContentSize().width / 2, itemWithdrawal->getContentSize().height / 2);
        itemWithdrawal->addChild(labelText);
    }
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupWithdrawal::onClickClose, this));
    menu->addChild(itemClose);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_cancel"), GAME_FONT, 24);
        labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        itemClose->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

#pragma mark - click
void PopupWithdrawal::onClickClose(Ref* sender)
{
    hide();
}

void PopupWithdrawal::onClickWithdrawal(cocos2d::Ref* sender)
{
    if (_editBox->getText() == nullptr)
    {
        return;
    }
    
    if (strlen(_editBox->getText()) <= 0)
    {
        return;
    }
    
    std::string strKey = ParameterManager::getInstance()->getParameterString("withdrawal_key");
    
    std::string strText = _editBox->getText();
    if (strText.compare(strKey.c_str()) != 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_withdrawal_error_1", strKey.c_str()));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    
    if( AccountManager::getInstance()->getUserID().length() != 0 )
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestDestroyUserData(CC_CALLBACK_1(PopupWithdrawal::callbackWithdrawal, this));
    }
    else
    {
        SaveManager::getInstance()->destroyAllUserData();
        
        Director::getInstance()->pause();
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_withdrawal_title_1"), GAME_TEXT("t_ui_withdrawal_complete_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref*){
            Director::getInstance()->resume();
            Director::getInstance()->end();
        });
        popup->show();
    }
}

#pragma mark - callback
void PopupWithdrawal::callbackWithdrawal(int nResult)
{
    PopupLoading::hide();
    
    if ( nResult != 1)
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    SaveManager::getInstance()->destroyAllUserData();
    
    Director::getInstance()->pause();
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_withdrawal_title_1"), GAME_TEXT("t_ui_withdrawal_complete_1"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref*){
        Director::getInstance()->resume();
        Director::getInstance()->end();
    });
    popup->show();
    
    hide();
}
 
