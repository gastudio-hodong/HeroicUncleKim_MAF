//
//  PopupSummer.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupSummer.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

PopupSummer* PopupSummer::create()
{
    PopupSummer* pRet = new(std::nothrow) PopupSummer();
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

PopupSummer::PopupSummer(void) :
_nPearl(0),
_onCallbackShortcuts(nullptr),

_layerContainerContents(nullptr),
_labelPearl(nullptr)
{
    
}

PopupSummer::~PopupSummer(void)
{
    
}

bool PopupSummer::init()
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

void PopupSummer::onEnter()
{
    PopupBase::onEnter();
    
    //
    requestInfo();
}

#pragma mark - init
void PopupSummer::initVar()
{
    
}

void PopupSummer::initUi()
{
    Size size = Size(562, 600);

    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSummer::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiContents();
}


#pragma mark - ui
void PopupSummer::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
  
    // top contents
    auto labelMsg1 = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_1"), GAME_FONT, 20);
    labelMsg1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg1->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    labelMsg1->setDimensions(_layerContainerContents->getContentSize().width, labelMsg1->getContentSize().height);
    labelMsg1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg1->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelMsg1);
    
    auto spritePearl = Sprite::create("Assets/ui/daily_event/icon_pearl.png");
    spritePearl->setPosition(_layerContainerContents->getContentSize().width / 2, labelMsg1->getPositionY() - labelMsg1->getContentSize().height - 25);
    _layerContainerContents->addChild(spritePearl);
    
    auto labelMsg2 = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_2"), GAME_FONT, 20);
    labelMsg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg2->setPosition(_layerContainerContents->getContentSize().width / 2, spritePearl->getPositionY() - 25);
    labelMsg2->setDimensions(_layerContainerContents->getContentSize().width, labelMsg1->getContentSize().height);
    labelMsg2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg2->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelMsg2);
    
    // line
    auto spriteLine = Sprite::create("Assets/ui/mine/week_weaponline.png");
    spriteLine->setPosition(_layerContainerContents->getContentSize().width / 2, labelMsg2->getPositionY() - labelMsg2->getContentSize().height - 25);
    _layerContainerContents->addChild(spriteLine);
    
    // bottom contents
    auto labelMsg3 = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_3"), GAME_FONT, 20);
    labelMsg3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg3->setPosition(_layerContainerContents->getContentSize().width / 2, spriteLine->getPositionY() - 25);
    labelMsg3->setDimensions(_layerContainerContents->getContentSize().width, labelMsg3->getContentSize().height);
    labelMsg3->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg3->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelMsg3);
    
    auto labelMsg4 = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_4"), GAME_FONT, 20);
    labelMsg4->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg4->setPosition(_layerContainerContents->getContentSize().width / 2, labelMsg3->getPositionY() - labelMsg3->getContentSize().height - 25);
    labelMsg4->setDimensions(_layerContainerContents->getContentSize().width, labelMsg4->getContentSize().height);
    labelMsg4->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg4->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelMsg4);
    
    auto labelMsg5 = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_5"), GAME_FONT, 20);
    labelMsg5->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg5->setPosition(_layerContainerContents->getContentSize().width / 2, labelMsg4->getPositionY() - labelMsg4->getContentSize().height);
    labelMsg5->setDimensions(_layerContainerContents->getContentSize().width, labelMsg5->getContentSize().height);
    labelMsg5->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg5->setOverflow(Label::Overflow::SHRINK);
    labelMsg5->setColor(Color3B::RED);
    _layerContainerContents->addChild(labelMsg5);
    
    // 소지량
    {
        auto spriteHaveIcon = Sprite::create("Assets/ui/daily_event/icon_pearl.png");
        spriteHaveIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteHaveIcon->setPosition(0, 27);
        spriteHaveIcon->setScale(1.5);
        spriteHaveIcon->getTexture()->setTexParameters(texParams);
        _layerContainerContents->addChild(spriteHaveIcon);
        
        _labelPearl = Label::createWithTTF("0", GAME_FONT, 24);
        _labelPearl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _labelPearl->setPosition(spriteHaveIcon->getPositionX() + 70, spriteHaveIcon->getPositionY());
        _layerContainerContents->addChild(_labelPearl);
        
        auto labelHave = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_6"), GAME_FONT, 20);
        labelHave->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelHave->setPosition(30, 70);
        _layerContainerContents->addChild(labelHave);
    }
    
    // 스킨 구매 하러 가기
    {
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        _layerContainerContents->addChild(menu);
        
        // item
        auto itemShortcuts = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_shotcut.png"), nullptr, CC_CALLBACK_1(PopupSummer::onClickShortcuts, this));
        itemShortcuts->setPosition(_layerContainerContents->getContentSize().width - 100, 27);
        menu->addChild(itemShortcuts);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_shortcuts"), GAME_FONT, 20);
            label->setPosition(itemShortcuts->getContentSize().width / 2, itemShortcuts->getContentSize().height / 2);
            label->setDimensions(itemShortcuts->getContentSize().width, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            itemShortcuts->addChild(label);
        }
        
        //
        auto labelShortcuts = Label::createWithTTF(GAME_TEXT("t_ui_event_summer_msg_7"), GAME_FONT, 20);
        labelShortcuts->setPosition(itemShortcuts->getPositionX(), 70);
        _layerContainerContents->addChild(labelShortcuts);
    }
}

#pragma mark - ui
void PopupSummer::drawPearl()
{
    _labelPearl->setString(MafUtils::toString(_nPearl));
}


#pragma mark - set, get, other
void PopupSummer::setCallbackShortcuts(const std::function<void(void)>& callback)
{
    _onCallbackShortcuts = callback;
}

#pragma mark - click
void PopupSummer::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupSummer::onClickShortcuts(cocos2d::Ref* sender)
{
    if ( _onCallbackShortcuts != nullptr )
    {
        _onCallbackShortcuts();
    }
}

#pragma mark - click
void PopupSummer::requestInfo()
{
    // network
    PopupLoading::show();
    
    std::string url = "/pearl/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void PopupSummer::responseInfo(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_total_pearl") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        hide();
        return;
    }
    
    _nPearl = jsonParser["_total_pearl"].GetInt();
    
    
    //
    drawPearl();
}
