//
//  PopupFishingReward.cpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/13.
//

#include "PopupFishingReward.h"

#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupFishingReward.h"

#include "ManagerEvent/EventFishingManager.h"

USING_NS_CC;

PopupFishingReward* PopupFishingReward::create(std::string strReward)
{
    PopupFishingReward* pRet = new(std::nothrow) PopupFishingReward();
    if ( pRet && pRet->init(strReward) )
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

PopupFishingReward::PopupFishingReward() :
_strReward("")
{
    
}

PopupFishingReward::~PopupFishingReward()
{
    
}

bool PopupFishingReward::init(std::string strReward)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _strReward = strReward;
    
    //
    initVar();
    initUI();
    
    return true;
    
}

#pragma mark - init
void PopupFishingReward::initVar()
{
    
}

void PopupFishingReward::initUI()
{
    Size size = Size(550, 740);
    
    //
    _spriteContainer->setContentSize( size );
    
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)_strReward.c_str());
    if ( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_logo"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    
    // item close
    auto spriteClose = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteClose->setContentSize(Size(150, 80));
    
    auto itemConfirm = MafNode::MafMenuItemSprite::create(spriteClose, nullptr, CC_CALLBACK_1(PopupFishingReward::onClickClose, this));
    itemConfirm->setPosition(_spriteContainer->getContentSize().width / 2, 25 + itemConfirm->getContentSize().height / 2);
    menu->addChild(itemConfirm);
    {
        auto labelCount = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_success_btn"), GAME_FONT, 25);
        labelCount->setPosition( Vec2(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2 ) );
        labelCount->setDimensions(itemConfirm->getContentSize().width - 20, itemConfirm->getContentSize().height - 10);
        labelCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelCount->setOverflow(Label::Overflow::SHRINK);
        labelCount->setColor( Color3B(255,216,0) );
        itemConfirm->addChild(labelCount);
    }
    
    int nTotalPoint = 0;
    if ( jsonParser.Size() == 1 )
    {
        int nType = jsonParser[0]["type"].GetInt();
        int nPoint = jsonParser[0]["point"].GetInt();
        nTotalPoint += nPoint;
        
        std::string strTitle = "";
        std::string strIconPath = "";
        if(nType == 1)
        {
            strTitle = GAME_TEXT("t_ui_event_fish_success_1");
            strIconPath = "Assets/ui/event/event_fish_1_1.png";
        }
        else if(nType == 2)
        {
            strTitle = GAME_TEXT("t_ui_event_fish_success_2");
            strIconPath = "Assets/ui/event/event_fish_1_2.png";
        }
        else if(nType == 3)
        {
            strTitle = GAME_TEXT("t_ui_event_fish_success_3");
            strIconPath = "Assets/ui/event/event_fish_1_3.png";
        }
        else if(nType == 4)
        {
            strTitle = GAME_TEXT("t_ui_event_fish_success_4");
            strIconPath = "Assets/ui/event/event_fish_1_4.png";
        }
        
        if ( strTitle.length() != 0 )
        {
            labelTitle->setString(strTitle);
        }
        
        auto spriteEffect = Sprite::create("Assets/ui/event/event_fx1_1.png");
        spriteEffect->setPosition(_spriteContainer->getContentSize().width / 2,  _spriteContainer->getContentSize().height / 2 + 50);
        _spriteContainer->addChild(spriteEffect);
        
        auto spriteIcon = Sprite::create(strIconPath);
        spriteIcon->setPosition(spriteEffect->getPosition());
        spriteIcon->getTexture()->setTexParameters(texParams);
        spriteIcon->setScale(4.f);
        _spriteContainer->addChild(spriteIcon);
    }
    else
    {
        std::string strText = "";
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            int nType = jsonValue["type"].GetInt();
            int nPoint = jsonValue["point"].GetInt();
            nTotalPoint += nPoint;
            
            if ( strText.length() != 0 )
            {
                strText.append("\n");
            }
            
            if(nType == 1)
            {
                strText.append(GAME_TEXT("t_ui_event_fish_reward_point_1"));
            }
            else if(nType == 2)
            {
                strText.append(GAME_TEXT("t_ui_event_fish_reward_point_2"));
            }
            else if(nType == 3)
            {
                strText.append(GAME_TEXT("t_ui_event_fish_reward_point_3"));
            }
            else if(nType == 4)
            {
                strText.append(GAME_TEXT("t_ui_event_fish_reward_point_4"));
            }
        }
        
        auto labelText = Label::createWithTTF(strText, GAME_FONT, 30);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelText->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
        labelText->setDimensions(_spriteContainer->getContentSize().width - 50, 400);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        _spriteContainer->addChild(labelText);
    }
    

    
    //
    auto spriteIconPoint = Sprite::create("Assets/ui/event/icon_event_fishing_point.png");
    spriteIconPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteIconPoint->setPosition(_spriteContainer->getContentSize().width / 2, itemConfirm->getPosition().y + itemConfirm->getContentSize().height / 2 + 10);
    spriteIconPoint->getTexture()->setTexParameters(texParams);
    spriteIconPoint->setScale(2.5f);
    _spriteContainer->addChild(spriteIconPoint);
    
    auto labelPoint = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_event_fish_success_ptn", nTotalPoint), GAME_FONT, 30);
    labelPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelPoint->setPosition(_spriteContainer->getContentSize().width / 2, itemConfirm->getPosition().y + itemConfirm->getContentSize().height / 2 + 10);
    labelPoint->setColor(Color3B(255, 212, 0));
    _spriteContainer->addChild(labelPoint);
}

#pragma mark - draw

#pragma mark - set, get, sort, add

#pragma mark - click
void PopupFishingReward::onClickClose(cocos2d::Ref* sender)
{
    hide();
}
