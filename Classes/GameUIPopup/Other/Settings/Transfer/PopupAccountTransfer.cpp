//
//  PopupAccountTransfer.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupAccountTransfer.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Settings/Transfer/PopupAccountTransferAutoCheck.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

PopupAccountTransfer* PopupAccountTransfer::create(void)
{
    PopupAccountTransfer* pRet = new(std::nothrow) PopupAccountTransfer();
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

PopupAccountTransfer::PopupAccountTransfer(void) :
_strUUID(""),
_strUUIDConfirm(""),

_layerContainerPrecautions(nullptr),
_layerContainerContent(nullptr),
_labelPrecautions(nullptr),
_edit(nullptr)
{
    
}

PopupAccountTransfer::~PopupAccountTransfer(void)
{
    
}

bool PopupAccountTransfer::init(void)
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
void PopupAccountTransfer::initVar()
{

    
}

void PopupAccountTransfer::initUi()
{
    Size size = Size(630, 726);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_transfer"), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAccountTransfer::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContent();
    uiPrecautions();
}

#pragma mark - ui
void PopupAccountTransfer::uiPrecautions()
{
    if ( _layerContainerPrecautions == nullptr )
    {
        _layerContainerPrecautions = Layer::create();
        _layerContainerPrecautions->setContentSize( Size(_spriteContainer->getContentSize().width, 251) );
        _layerContainerPrecautions->setIgnoreAnchorPointForPosition(false);
        _layerContainerPrecautions->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerPrecautions->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - _layerContainerContent->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerPrecautions);
    }
    _layerContainerPrecautions->removeAllChildren();
    
    _labelPrecautions = Label::createWithTTF(GAME_TEXT("t_ui_transfer_msg_01_info"), GAME_FONT, 22);
    _labelPrecautions->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _labelPrecautions->setPosition( 20, _layerContainerPrecautions->getContentSize().height - 20);
    _labelPrecautions->setDimensions(_layerContainerPrecautions->getContentSize().width - 130, _layerContainerPrecautions->getContentSize().height - 20 );
    _labelPrecautions->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    _labelPrecautions->setOverflow(Label::Overflow::SHRINK);
    _labelPrecautions->setLineBreakWithoutSpace(true);
    _layerContainerPrecautions->addChild(_labelPrecautions);
    
}

void PopupAccountTransfer::uiContent()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, 400) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();

    auto labelSubTitle = Label::createWithTTF(GAME_TEXT("t_ui_transfer_msg_01"), GAME_FONT, 28);
    labelSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelSubTitle->setPosition(_layerContainerContent->getContentSize().width/2, _layerContainerContent->getContentSize().height - 15);
    labelSubTitle->setDimensions(_layerContainerContent->getContentSize().width - 60, labelSubTitle->getContentSize().height);
    labelSubTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelSubTitle->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelSubTitle);
    
    
    auto btnTransfer = ui::Button::create("Assets/ui_common/box.png");
    btnTransfer->setScale9Enabled(true);
    btnTransfer->addClickEventListener(CC_CALLBACK_1(PopupAccountTransfer::onClickTransfer, this));
    btnTransfer->setContentSize(Size(264,256));
    btnTransfer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnTransfer->setZoomScale(0.05f);
    btnTransfer->setPosition(Vec2(40, _layerContainerContent->getContentSize().height - 100));
    _layerContainerContent->addChild(btnTransfer);
    {
        std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = "Assets/ui/menu/data_2_1.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        path = "Assets/ui/menu/data_1_1.png";
#endif
        auto sprIcon = Sprite::create(path);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(Vec2(btnTransfer->getContentSize()/2) + Vec2::UP* 20);
        btnTransfer->addChild(sprIcon);
        
        auto labelContent = Label::createWithTTF(GAME_TEXT("t_ui_menu_bt_transfer"), GAME_FONT, 24);
        labelContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelContent->setPosition(btnTransfer->getContentSize().width/2, 50);
        labelContent->setDimensions(btnTransfer->getContentSize().width - 40, labelContent->getContentSize().height);
        labelContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        labelContent->setOverflow(Label::Overflow::SHRINK);
        btnTransfer->addChild(labelContent);
    }
    
    
    auto btnTransferSame = ui::Button::create("Assets/ui_common/box.png");
    btnTransferSame->setScale9Enabled(true);
    btnTransferSame->addClickEventListener([=](Ref*){
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_bt_link_info"));
        popup->addButton(GAME_TEXT("t_ui_transfer_bt_howto"), false, [=](Ref*){
            std::string strLang = TextManager::getInstance()->getLang();
            std::string strText = ParameterManager::getInstance()->getParameterString("data_transfer_link");
            
            rapidjson::Document jsonParser;
            jsonParser.Parse<0>((char*)strText.c_str());
            if ( strText.length() != 0 && jsonParser.HasParseError() == false )
            {
                std::string strKey = "other";
                if ( jsonParser.HasMember(strLang.c_str()) == true )
                {
                    strKey = strLang;
                }
                
                const rapidjson::Value& jsonArray = jsonParser[strKey.c_str()];
                
                for ( int i = 0; i < jsonArray.Size(); i++ )
                {
                    const rapidjson::Value& jsonValue = jsonArray[i];
                    std::string strLink = "";
                    
                    if(strKey.compare("ko") == 0)
                    {
                        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        strLink = jsonValue["ios"].GetString();
                        #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        strLink = jsonValue["aos"].GetString();
                        #endif
                        
                    }
                    else
                    {
                        strLink = jsonValue["link"].GetString();
                    }
                        
                    SoundManager::SoundEffectStart(SOUND_SELECT);
                        
                    MafNative::OpenUrl(strLink.c_str());
                    break;
                }
            }
        });
        
        popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
        popup->show();
        
    });
    btnTransferSame->setContentSize(Size(264,256));
    btnTransferSame->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnTransferSame->setZoomScale(0.05f);
    btnTransferSame->setPosition(Vec2(_layerContainerContent->getContentSize().width - 40, _layerContainerContent->getContentSize().height - 100));
    _layerContainerContent->addChild(btnTransferSame);
    {
        std::string path = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        path = "Assets/ui/menu/data_2_2.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        path = "Assets/ui/menu/data_1_2.png";
#endif
        auto sprIcon = Sprite::create(path);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(Vec2(btnTransferSame->getContentSize()/2) + Vec2::UP* 20);
        btnTransferSame->addChild(sprIcon);
        
        auto labelContent = Label::createWithTTF(GAME_TEXT("t_ui_transfer_bt_link"), GAME_FONT, 24);
        labelContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelContent->setPosition(btnTransferSame->getContentSize().width/2, 50);
        labelContent->setDimensions(btnTransferSame->getContentSize().width - 40, labelContent->getContentSize().height);
        labelContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        labelContent->setOverflow(Label::Overflow::SHRINK);
        btnTransferSame->addChild(labelContent);
    }
    
}

void PopupAccountTransfer::uiSelect()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, 400) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    auto labelSubTitle = Label::createWithTTF(GAME_TEXT("t_ui_transfer_msg_02"), GAME_FONT, 28);
    labelSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelSubTitle->setPosition(_layerContainerContent->getContentSize().width/2, _layerContainerContent->getContentSize().height - 15);
    labelSubTitle->setDimensions(_layerContainerContent->getContentSize().width - 60, labelSubTitle->getContentSize().height);
    labelSubTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelSubTitle->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelSubTitle);
    
    
    auto btnSend = ui::Button::create("Assets/ui_common/box.png");
    btnSend->setScale9Enabled(true);
    btnSend->addClickEventListener(CC_CALLBACK_1(PopupAccountTransfer::onClickSend, this));
    btnSend->setContentSize(Size(264,256));
    btnSend->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnSend->setZoomScale(0.05f);
    btnSend->setPosition(Vec2(40, _layerContainerContent->getContentSize().height - 100));
    _layerContainerContent->addChild(btnSend);
    {
        std::string strPath1 = "";
        std::string strPath2 = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        strPath1 = "Assets/ui/menu/data_4_2.png";
        strPath2 = "Assets/ui/menu/data_3_1.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        strPath1 = "Assets/ui/menu/data_4_1.png";
        strPath2 = "Assets/ui/menu/data_3_2.png";
#endif
        auto sprPhon = Sprite::create(strPath1);
        sprPhon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprPhon->setPosition(Vec2(btnSend->getContentSize()/2) + Vec2::LEFT * 15 + Vec2::UP * 20);
        btnSend->addChild(sprPhon);
        
        auto sprArrow = Sprite::create("Assets/ui_common/arrow_R03.png");
        sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprArrow->setPosition(Vec2(btnSend->getContentSize()/2) + Vec2::UP * 20);
        btnSend->addChild(sprArrow);
        
        auto sprData = Sprite::create(strPath2);
        sprData->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprData->setPosition(Vec2(btnSend->getContentSize()/2) + Vec2::RIGHT * 15 + Vec2::UP * 20);
        btnSend->addChild(sprData);
        
        auto labelContent = Label::createWithTTF(GAME_TEXT("t_ui_transfer_bt_send"), GAME_FONT, 24);
        labelContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelContent->setPosition(btnSend->getContentSize().width/2, 50);
        labelContent->setDimensions(btnSend->getContentSize().width - 40, labelContent->getContentSize().height);
        labelContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        labelContent->setOverflow(Label::Overflow::SHRINK);
        btnSend->addChild(labelContent);
    }
    
    auto btnReceive = ui::Button::create("Assets/ui_common/box.png");
    btnReceive->setScale9Enabled(true);
    btnReceive->addClickEventListener(CC_CALLBACK_1(PopupAccountTransfer::onClickReceive, this));
    btnReceive->setContentSize(Size(264,256));
    btnReceive->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnReceive->setZoomScale(0.05f);
    btnReceive->setPosition(Vec2(_layerContainerContent->getContentSize().width - 40, _layerContainerContent->getContentSize().height - 100));
    _layerContainerContent->addChild(btnReceive);
    {
        std::string strPath1 = "";
        std::string strPath2 = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        strPath1 = "Assets/ui/menu/data_4_2.png";
        strPath2 = "Assets/ui/menu/data_3_1.png";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        strPath1 = "Assets/ui/menu/data_4_1.png";
        strPath2 = "Assets/ui/menu/data_3_2.png";
#endif
        auto sprPhon = Sprite::create(strPath1);
        sprPhon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprPhon->setPosition(Vec2(btnReceive->getContentSize()/2) + Vec2::LEFT * 15 + Vec2::UP * 20);
        btnReceive->addChild(sprPhon);
        
        auto sprArrow = Sprite::create("Assets/ui_common/arrow_L03.png");
        sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprArrow->setPosition(Vec2(btnReceive->getContentSize()/2) + Vec2::UP * 20);
        btnReceive->addChild(sprArrow);
        
        auto sprData = Sprite::create(strPath2);
        sprData->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprData->setPosition(Vec2(btnReceive->getContentSize()/2) + Vec2::RIGHT * 15 + Vec2::UP * 20);
        btnReceive->addChild(sprData);
        
        auto labelContent = Label::createWithTTF(GAME_TEXT("t_ui_transfer_bt_receive"), GAME_FONT, 24);
        labelContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelContent->setPosition(btnReceive->getContentSize().width/2, 20);
        labelContent->setDimensions(btnReceive->getContentSize().width - 40, labelContent->getContentSize().height);
        labelContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        labelContent->setOverflow(Label::Overflow::SHRINK);
        btnReceive->addChild(labelContent);
    }
    
    if(_labelPrecautions != nullptr)
    {
        _labelPrecautions->setString(GAME_TEXT("t_ui_transfer_msg_02_info"));
    }
    
    auto btnBack = ui::Button::create("Assets/ui_common/btn_back1_2.png");
    btnBack->addClickEventListener([=](Ref*){
        uiContent();
    });
    btnBack->setScale(1.5f);
    btnBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnBack->setZoomScale(0.05f);
    btnBack->setPosition(Vec2(_layerContainerContent->getContentSize().width - 20, -210));
    _layerContainerContent->addChild(btnBack);
}

void PopupAccountTransfer::uiSend()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, 400) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    std::string strTitle = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strTitle = GAME_TEXT("t_ui_transfer_msg_03_I");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strTitle = GAME_TEXT("t_ui_transfer_msg_03_A");
#endif
    
    auto labelSubTitle = Label::createWithTTF(strTitle, GAME_FONT, 28);
    labelSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelSubTitle->setPosition(_layerContainerContent->getContentSize().width/2, _layerContainerContent->getContentSize().height - 15);
    labelSubTitle->setDimensions(_layerContainerContent->getContentSize().width - 60, labelSubTitle->getContentSize().height);
    labelSubTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelSubTitle->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelSubTitle);
    
    auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_transfer_msg_05"), GAME_FONT, 24);
    labelMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg->setPosition(_layerContainerContent->getContentSize().width/2, labelSubTitle->getPosition().y - labelSubTitle->getContentSize().height - 20);
    labelMsg->setDimensions(_layerContainerContent->getContentSize().width - 60, labelMsg->getContentSize().height);
    labelMsg->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    labelMsg->setOverflow(Label::Overflow::SHRINK);
    labelMsg->setColor(Color3B(255,60,0));
    _layerContainerContent->addChild(labelMsg);
    
    auto spriteBox = Sprite::create("Assets/ui/transfer/input.png");
    spriteBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteBox->setPosition(_layerContainerContent->getContentSize().width / 2, 120);
    _layerContainerContent->addChild(spriteBox);
    {
        auto labelContent = Label::createWithTTF(GAME_TEXT("t_ui_confirm_number"), GAME_FONT, 30);
        labelContent->setPosition(spriteBox->getContentSize().width / 2, spriteBox->getContentSize().height / 2);
        spriteBox->addChild(labelContent);
    }
    
    auto labelUUID = Label::createWithTTF(_strUUID, GAME_FONT, 40);
    labelUUID->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelUUID->setPosition(_layerContainerContent->getContentSize().width/2, spriteBox->getPosition().y - 20);
    labelUUID->setColor(COLOR_COIN);
    _layerContainerContent->addChild(labelUUID);
    
    if(_labelPrecautions != nullptr)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _labelPrecautions->setString(GAME_TEXT("t_ui_transfer_msg_03_info_I"));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        _labelPrecautions->setString(GAME_TEXT("t_ui_transfer_msg_03_info_A"));
#endif
    }
    
    auto btnBack = ui::Button::create("Assets/ui_common/btn_back1_2.png");
    btnBack->addClickEventListener([=](Ref*){
        uiSelect();
    });
    btnBack->setScale(1.5f);
    btnBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnBack->setZoomScale(0.05f);
    btnBack->setPosition(Vec2(_layerContainerContent->getContentSize().width - 20, -210));
    _layerContainerContent->addChild(btnBack);
}

void PopupAccountTransfer::uiReceive()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, 400) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    std::string strTitle = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strTitle = GAME_TEXT("t_ui_transfer_msg_04_I");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strTitle = GAME_TEXT("t_ui_transfer_msg_04_A");
#endif
    
    auto labelSubTitle = Label::createWithTTF(strTitle, GAME_FONT, 28);
    labelSubTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelSubTitle->setPosition(_layerContainerContent->getContentSize().width/2, _layerContainerContent->getContentSize().height - 15);
    labelSubTitle->setDimensions(_layerContainerContent->getContentSize().width - 60, labelSubTitle->getContentSize().height);
    labelSubTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelSubTitle->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelSubTitle);
    
    auto labelMsg = Label::createWithTTF(GAME_TEXT("t_ui_transfer_msg_05"), GAME_FONT, 24);
    labelMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg->setPosition(_layerContainerContent->getContentSize().width/2, labelSubTitle->getPosition().y - labelSubTitle->getContentSize().height - 20);
    labelMsg->setDimensions(_layerContainerContent->getContentSize().width - 60, labelMsg->getContentSize().height);
    labelMsg->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    labelMsg->setOverflow(Label::Overflow::SHRINK);
    labelMsg->setColor(Color3B(255,60,0));
    _layerContainerContent->addChild(labelMsg);
    
    auto layerBox = Layer::create();
    layerBox->setContentSize( Size(_layerContainerContent->getContentSize().width, 250) );
    layerBox->setIgnoreAnchorPointForPosition(false);
    layerBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layerBox->setPosition(_layerContainerContent->getContentSize().width / 2, labelMsg->getPosition().y - labelMsg->getContentSize().height - 50);
    _layerContainerContent->addChild(layerBox);
    {
        _edit = ui::EditBox::create(Size(448, 78), "Assets/ui/transfer/input.png");
        _edit->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        _edit->setPosition(Vec2(layerBox->getContentSize().width/2, layerBox->getContentSize().height - 20));
        _edit->setMaxLength(40);
        _edit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        _edit->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
        _edit->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
        _edit->setEnabled(true);
        _edit->setFontColor(Color3B::WHITE);
        _edit->setPlaceholderFont(GAME_FONT, 28);
        _edit->setFont(GAME_FONT, 28);
        _edit->setPlaceHolder(GAME_TEXT("t_ui_transfer_input").c_str());
        layerBox->addChild(_edit);
        
        auto menu = Menu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        layerBox->addChild(menu);
 
        auto itemConfirm = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_ok.png"), nullptr, [=](Ref*){
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_msg_04_check"));
            popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(PopupAccountTransfer::onClickConfirm, this));
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->show();
            
            
        });
        itemConfirm->setPosition(layerBox->getContentSize().width/2, _edit->getPosition().y - _edit->getContentSize().height - 30);
        menu->addChild(itemConfirm);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 28);
            label->setPosition(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
            itemConfirm->addChild(label);
        }
    }
    
    if(_labelPrecautions != nullptr)
    {
        _labelPrecautions->setString(GAME_TEXT("t_ui_transfer_msg_04_info"));
    }
    
    auto btnBack = ui::Button::create("Assets/ui_common/btn_back1_2.png");
    btnBack->addClickEventListener([=](Ref*){
        uiSelect();
    });
    btnBack->setScale(1.5f);
    btnBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnBack->setZoomScale(0.05f);
    btnBack->setPosition(Vec2(_layerContainerContent->getContentSize().width - 20, -210));
    _layerContainerContent->addChild(btnBack);
}

#pragma mark - set, get, sort, add

#pragma mark - click
void PopupAccountTransfer::onClickClose(Ref* sender)
{
    hide();
}

void PopupAccountTransfer::onClickTransfer(Ref* sender)
{
    uiSelect();
}

void PopupAccountTransfer::onClickSend(Ref* sender)
{
    PopupLoading::show();
    SaveManager::getInstance()->requestSave(CC_CALLBACK_1(PopupAccountTransfer::callbackSave, this));
}

void PopupAccountTransfer::onClickReceive(Ref* sender)
{
    uiReceive();
}

void PopupAccountTransfer::onClickCancel(Ref* sender)
{
    uiSelect();
}

void PopupAccountTransfer::onClickConfirm(Ref* sender)
{
    _strUUIDConfirm = _edit->getText();
    _strUUIDConfirm = MafUtils::TrimRight(_strUUIDConfirm);
    if ( _strUUIDConfirm.length() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    // network
    requestAuthCheck();
}


#pragma mark - network
void PopupAccountTransfer::requestAuth()
{
    // network
    PopupLoading::show();
    
    
    std::string url = "/account/transfer";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAuth(response,dd);
    };
    req->send(cb);
}

void PopupAccountTransfer::responseAuth(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_result") == false )
    {
        bError = true;
    }

    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto result = jsonParser[("_result")].GetInt();
    if ( result == 1 && jsonParser.HasMember("_uuid") == true )
    {
        // 성공
        _strUUID = jsonParser[("_uuid")].GetString();
        
        uiSend();
    }
    else if ( result == 4 && jsonParser.HasMember("_to_userid") == true )
    {
        _strUUIDConfirm = jsonParser[("_uuid")].GetString();
        
        auto popup = PopupAccountTransferAutoCheck::create(jsonParser["_to_userid"].GetString());
        popup->setCallback([=](){
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_msg_04_check"));
            popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_0(PopupAccountTransfer::requestAuthCheck, this));
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->show();
            
            
        });
        popup->show();
    }
    else
    {
        std::string strMessageKey = "t_ui_transfer_error_2";
        if ( result == 3 )
        {
            strMessageKey = "t_ui_transfer_error_3";
        }
        
        // 실패
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();

    }
}

void PopupAccountTransfer::requestAuthCheck()
{
    // network
    PopupLoading::show();
    
    
    std::string url = "/account/transfer/complete";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_uuid", _strUUIDConfirm.c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAuthCheck(response,dd);
    };
    req->send(cb);
}

void PopupAccountTransfer::responseAuthCheck(cocos2d::network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    //
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_result") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        // 성공
        PopupLoading::show();
        SaveManager::getInstance()->requestLoad(CC_CALLBACK_2(PopupAccountTransfer::callbackLoad, this));
    }
    else if ( result == 4 )
    {// 자동 계정이전 완료
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_msg_13"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            //
            MafHttpNew::_vecRequest.clear();
            
            //
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->unscheduleAllWithMinPriority(Scheduler::PRIORITY_NON_SYSTEM_MIN);
            
            MafGoogleRewardAds::HideGoogleBanner();
            Director::getInstance()->pause();
            UtilsScene::getInstance()->scenePopRoot();
            Director::getInstance()->resume();
        });
        popup->show();
    }
    else
    {
        std::string strMessageKey = "t_ui_error_30";
        if ( result == 0 )
            strMessageKey = "t_ui_transfer_error_1";
        else if ( result == 3 )
            strMessageKey = "t_ui_transfer_error_3";
        else if ( result == 6 )
            strMessageKey = "t_ui_transfer_error_5";
        else if ( result == 7 )
            strMessageKey = "t_ui_transfer_error_4";

        // 실패
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();

    }
}

#pragma mark - callback
void PopupAccountTransfer::callbackSave(bool bResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            hide();
        });
        popup->show();
        return;
    }
    
    //
    requestAuth();
}

void PopupAccountTransfer::callbackLoad(bool bResult, bool bData)
{
    //
    PopupLoading::hide();
      
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( bData == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_10"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            SaveManager::getInstance()->loadCloudData();
        });
        popup->show();
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_11"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}
