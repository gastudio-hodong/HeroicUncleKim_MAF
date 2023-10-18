//
//  PopupUserAgree.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#include "PopupUserAgree.h"

#include "Common/ConfigGameNormal.h"

USING_NS_CC;

PopupUserAgree* PopupUserAgree::create()
{
    PopupUserAgree* pRet = new(std::nothrow) PopupUserAgree();
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

PopupUserAgree::PopupUserAgree(void) :
_onCallbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupUserAgree::~PopupUserAgree(void)
{
    
}

bool PopupUserAgree::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();

    return true;
}

#pragma mark - init
void PopupUserAgree::initVar()
{
    std::string strLang = TextManager::getInstance()->getLang();
    
    //
    {
        std::string strText = ParameterManager::getInstance()->getParameterString("terms_link");
        
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)strText.c_str());
        if ( strText.length() != 0 && jsonParser.HasParseError() == false )
        {
            std::string strKey = "en";
            if ( jsonParser.HasMember(strLang.c_str()) == true )
            {
                strKey = strLang;
            }
            
            _listUserAgree.push_back( {"t_ui_terms_link", jsonParser[strKey.c_str()].GetString(), true} );
        }
    }
    
    //
    {
        std::string strText = ParameterManager::getInstance()->getParameterString("privacy_link");
        
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)strText.c_str());
        if ( strText.length() != 0 && jsonParser.HasParseError() == false )
        {
            std::string strKey = "en";
            if ( jsonParser.HasMember(strLang.c_str()) == true )
            {
                strKey = strLang;
            }
            
            _listUserAgree.push_back( {"t_ui_privacy_link", jsonParser[strKey.c_str()].GetString(), true} );
        }
    }
    
    
    //
    _listUserAgree.push_back( {"t_ui_system_per_explain", "https://moneyhero-cs.appspot.com/permission/user_hero", true} );
}

void PopupUserAgree::initUi()
{
    Size size = Size(567,495);
    
    //
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale_2.png", Rect::ZERO, Rect(70,110,10,10));
    _spriteContainer->setContentSize( size );
    _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    addChild(_spriteContainer);
    
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
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_system_policy_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    labelTitle->setDimensions(labelTitle->getContentSize().width * 0.75, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelTitle);

    //
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupUserAgree::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerContents->addChild(menu);
 

    for ( int i = 0; i < _listUserAgree.size(); i++ )
    {
        auto obj = _listUserAgree.at(i);
        double posY = _layerContainerContents->getContentSize().height - 50 - i * 80;
        
        auto spriteCheck = Sprite::create("Assets/ui/costume/checkbox_on.png");
        spriteCheck->setScale(1.3);
        spriteCheck->getTexture()->setTexParameters(texParams);
        
        auto itemCheck = MafNode::MafMenuItemSprite::create(spriteCheck, nullptr, CC_CALLBACK_1(PopupUserAgree::onClickOption, this));
        itemCheck->setPosition(50 - itemCheck->getContentSize().width / 2, posY);
        itemCheck->setTag(i);
        menu->addChild(itemCheck);
              
        auto labelExplain = Label::createWithTTF(GAME_TEXT(obj.strText), GAME_FONT, 30);
        labelExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelExplain->setPosition(60, posY);
        labelExplain->setDimensions(_layerContainerContents->getContentSize().width - 150 - 10, labelExplain->getContentSize().height);
        labelExplain->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelExplain->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelExplain);
        
        auto itemLink = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_magnifying_glass.png"), nullptr, CC_CALLBACK_1(PopupUserAgree::onClickLink, this));
        itemLink->setPosition(_layerContainerContents->getContentSize().width - 100 + itemLink->getContentSize().width / 2, posY);
        itemLink->setTag(i);
        menu->addChild(itemLink);
    }
}

void PopupUserAgree::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    // item Ok
    auto spriteOk = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(32,35,10,2));
    spriteOk->setContentSize(Size(180, 78));
    
    auto itemOk = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupUserAgree::onClickOk, this));
    menu->addChild(itemOk);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_system_policy_agree"), GAME_FONT, 24);
        labelText->setPosition(itemOk->getContentSize().width / 2, itemOk->getContentSize().height / 2);
        labelText->setColor(Color3B(255,223,28));
        itemOk->addChild(labelText);
    }
}

#pragma mark - set, get
void PopupUserAgree::setCallbackResult(const std::function<void(void)>& callback)
{
    _onCallbackResult = callback;
}

#pragma mark - click
void PopupUserAgree::onClickOption(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto &obj = _listUserAgree.at(item->getTag());
    
    obj.bCheck = obj.bCheck ? false : true;
    
    std::string strPath = "Assets/ui/costume/checkbox_on.png";
    if ( obj.bCheck == false )
    {
        strPath = "Assets/ui/costume/checkbox_off.png";
    }
    
    auto spriteCheck = (Sprite*)item->getNormalImage();
    spriteCheck->setTexture(strPath);
}

void PopupUserAgree::onClickLink(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listUserAgree.at(item->getTag());
    
    MafNative::OpenUrl(obj.strLink.c_str());
}

void PopupUserAgree::onClickOk(cocos2d::Ref* sender)
{
    bool bAllAgree = false;
    
    for ( int i = 0; i < _listUserAgree.size(); i++ )
    {
        auto obj = _listUserAgree.at(i);
        bAllAgree = obj.bCheck;
        
        if ( bAllAgree == false )
        {
            break;
        }
    }
    
    if ( bAllAgree == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_38"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult();
    }
    
    hide();
}
