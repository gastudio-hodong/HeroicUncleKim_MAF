//
//  PopupEventDaily.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupEventDaily.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeManager.h"

USING_NS_CC;

PopupEventDaily* PopupEventDaily::create()
{
    PopupEventDaily* pRet = new(std::nothrow) PopupEventDaily();
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

PopupEventDaily::PopupEventDaily(void) :
_nRewardCoin(0),
_nRewardDay(0),
_bRewardNew(false),

_layerContainerContents(nullptr),
_layerContainerBottom(nullptr)
{
    
}

PopupEventDaily::~PopupEventDaily(void)
{
    
}

bool PopupEventDaily::init()
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

void PopupEventDaily::onEnter()
{
    PopupBase::onEnter();
    
    // network
    requestInfo();
}

#pragma mark - init
void PopupEventDaily::initVar()
{
    
}

void PopupEventDaily::initUi()
{
    Size size = Size(728, 436);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_daily_title"), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventDaily::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37) );
    itemClose->setScale(0.8);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    uiContents();
    uiBottom();
}

#pragma mark - ui
void PopupEventDaily::uiContents()
{
    if ( _layerContainerContents == nullptr )
    {
        _layerContainerContents = Layer::create();
        _layerContainerContents->setContentSize( Size(672, 130) );
        _layerContainerContents->setIgnoreAnchorPointForPosition(false);
        _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 20) );
        _spriteContainer->addChild(_layerContainerContents);
    }
    _layerContainerContents->removeAllChildren();
    
    int listReward[] = {10, 20, 30, 40, 100};
    for ( int i = 0; i < 5; i++ )
    {
        auto spriteBg = Sprite::create("Assets/ui_common/box_daily.png");
        spriteBg->setAnchorPoint( Vec2::ZERO );
        spriteBg->setPosition( Vec2(i * spriteBg->getContentSize().width + i * 17, 0) );
        spriteBg->setTag(PopupEventDaily::tag_ui_contents_box + i);
        _layerContainerContents->addChild(spriteBg);
        
        auto spriteIcon = Sprite::create("Assets/ui/daily_event/icon_stamp_off.png");
        spriteIcon->setPosition( Vec2(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height * 0.6) );
        spriteIcon->setTag(PopupEventDaily::tag_ui_contents_box_icon);
        spriteBg->addChild(spriteIcon);
        
        auto labelReward = Label::createWithTTF(MafUtils::format("x %d", listReward[i]), GAME_FONT, 32);
        labelReward->setColor(COLOR_TITLE);
        labelReward->setPosition( Vec2(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height*0.2));
        spriteBg->addChild(labelReward);
        
        auto labelDay = Label::createWithTTF(MafUtils::format("D-%d", i+1), GAME_FONT, 24);
        labelDay->setColor(Color3B(162, 172, 184));
        labelDay->setPosition( Vec2(spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height+15) );
        labelDay->setTag(PopupEventDaily::tag_ui_contents_box_day);
        spriteBg->addChild(labelDay);
    }

}

void PopupEventDaily::uiBottom()
{
    if ( _layerContainerBottom == nullptr )
    {
        _layerContainerBottom = LayerColor::create(Color4B::BLUE);
        _layerContainerBottom->setContentSize( Size(672, 154) );
        _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
        _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerBottom->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 40) );
        _spriteContainer->addChild(_layerContainerBottom);
    }
    _layerContainerBottom->removeAllChildren();
    
    
    auto spriteBg = ui::Scale9Sprite::create("Assets/ui/daily_event/daily_box_2.png", Rect::ZERO, Rect(10,10,10,10));
    spriteBg->setContentSize( Size(spriteBg->getContentSize().width, 154) );
    spriteBg->setPosition( Vec2(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height / 2) );
    _layerContainerBottom->addChild(spriteBg);
    
    auto labelTitle = Label::createWithTTF("REWARD", GAME_FONT, 24);
    labelTitle->setColor(COLOR_COIN);
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelTitle->setPosition( spriteBg->getContentSize().width / 2, spriteBg->getContentSize().height - 5 );
    spriteBg->addChild(labelTitle);
    
    auto spriteBoxCoin = Sprite::create("Assets/ui_common/box_reward.png");
    spriteBoxCoin->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
    spriteBoxCoin->setPosition( _layerContainerBottom->getContentSize().width / 2 - 5, 5 );
    spriteBoxCoin->setTag(PopupEventDaily::tag_ui_bottom_box_coin);
    _layerContainerBottom->addChild(spriteBoxCoin);
    {
        auto spriteIcon = Sprite::create("Assets/icon/icon_coin_2.png");
        spriteIcon->setPosition( Vec2(spriteBoxCoin->getContentSize().width / 2, spriteBoxCoin->getContentSize().height / 2 + 10) );
        spriteBoxCoin->addChild(spriteIcon);
        
        auto labelReward = Label::createWithTTF("0", GAME_FONT, 24);
        labelReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelReward->setPosition( Vec2(spriteBoxCoin->getContentSize().width / 2, 2));
        labelReward->setColor(Color3B::WHITE);
        labelReward->setTag(PopupEventDaily::tag_ui_bottom_box_in_label);
        spriteBoxCoin->addChild(labelReward);
    }
    
    auto spriteBoxTicket = Sprite::create("Assets/ui_common/box_reward.png");
    spriteBoxTicket->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    spriteBoxTicket->setPosition( _layerContainerBottom->getContentSize().width / 2 + 5, 5 );
    spriteBoxTicket->setTag(PopupEventDaily::tag_ui_bottom_box_ticket);
    _layerContainerBottom->addChild(spriteBoxTicket);
    {
        auto spriteIcon = Sprite::create("Assets/ui/prison/icon_freepass.png");
        spriteIcon->setPosition( Vec2(spriteBoxTicket->getContentSize().width / 2, spriteBoxTicket->getContentSize().height / 2 + 10) );
        spriteBoxTicket->addChild(spriteIcon);
        
        auto labelReward = Label::createWithTTF("0", GAME_FONT, 24);
        labelReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelReward->setPosition( Vec2(spriteBoxTicket->getContentSize().width / 2, 2));
        labelReward->setColor(Color3B::WHITE);
        labelReward->setTag(PopupEventDaily::tag_ui_bottom_box_in_label);
        spriteBoxTicket->addChild(labelReward);
    }
    
    
}

void PopupEventDaily::drawContents()
{
    for ( int i = 0; i < _nRewardDay; i++ )
    {
        auto spriteBg = (Sprite*)_layerContainerContents->getChildByTag(PopupEventDaily::tag_ui_contents_box + i);
        if ( spriteBg == nullptr )
            continue;
        
        auto spriteIcon = (Sprite*)spriteBg->getChildByTag(PopupEventDaily::tag_ui_contents_box_icon);
        if ( spriteIcon != nullptr )
        {
            spriteIcon->setTexture("Assets/ui/daily_event/icon_stamp_on.png");
        }
        
        
        auto spriteCheck = Sprite::create("Assets/ui/attendance/attendance_check.png");
        spriteCheck->setPosition( Vec2(spriteBg->getContentSize().width / 2, spriteCheck->getContentSize().height / 2) );
        spriteBg->addChild(spriteCheck);
        
        if ( _bRewardNew == true && i+1 == _nRewardDay )
        {
            spriteCheck->runAction(Sequence::create(Blink::create(3, 3) , nullptr));
            
            
            auto labelDay = (Label*)spriteBg->getChildByTag(PopupEventDaily::tag_ui_contents_box_day);
            if ( labelDay != nullptr )
            {
                labelDay->setColor(Color3B::GREEN);
            }
            
        }
    }
}

void PopupEventDaily::drawBottom()
{
    int listReward[] = {10, 20, 30, 40, 100};
    int nCount = listReward[0];
    if ( _nRewardDay != 0 )
    {
        nCount = listReward[_nRewardDay-1];
    }
    
    // coin
    auto spriteBoxCoin = (Sprite*)_layerContainerBottom->getChildByTag(PopupEventDaily::tag_ui_bottom_box_coin);
    if ( spriteBoxCoin != nullptr )
    {
        auto label = (Label*)spriteBoxCoin->getChildByTag(PopupEventDaily::tag_ui_bottom_box_in_label);
        if ( label != nullptr )
        {
            label->setString(MafUtils::toString(nCount));
        }
    }
    
    // point
    auto spriteBoxTicket = (Sprite*)_layerContainerBottom->getChildByTag(PopupEventDaily::tag_ui_bottom_box_ticket);
    if ( spriteBoxTicket != nullptr )
    {
        auto label = (Label*)spriteBoxTicket->getChildByTag(PopupEventDaily::tag_ui_bottom_box_in_label);
        if ( label != nullptr )
        {
            label->setString(MafUtils::toString(nCount));
        }
    }
}

#pragma mark - set, get, sort, add


#pragma mark - click
void PopupEventDaily::onClickClose(Ref* sender)
{
    hide();
}


#pragma mark - network
void PopupEventDaily::requestInfo()
{
    // network
    PopupLoading::show();
    
    int nCostume1 = 0;
    int nCostume2 = 0;
    int nCostume3 = 0;
    int nCostume4 = 0;
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_10)) {
        nCostume1 = 1;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_12)) {
        nCostume2 = 1;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_15)) {
        nCostume3 = 1;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_17)) {
        nCostume4 = 1;
    }
    
    std::string url = "/v3/dailySpecial/%d/%d/%d/%d/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), nCostume1, nCostume2, nCostume3, nCostume4);
    
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void PopupEventDaily::responseInfo(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && (jsonParser.HasMember("_suc") == false || jsonParser.HasMember("_day") == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        hide();
        return;
    }
    
    _bRewardNew = jsonParser["_suc"].GetInt();
    _nRewardDay = jsonParser["_day"].GetInt();

    // draw
    drawContents();
    drawBottom();
}
