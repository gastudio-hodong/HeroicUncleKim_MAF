//
//  PopupFishingBoardReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#include "PopupFishingBoardReward.h"

#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupFishingReward.h"

#include "ManagerGame/ItemsManager.h"


USING_NS_CC;

PopupFishingBoardReward* PopupFishingBoardReward::create(std::string strReward, bool bFinish)
{
    PopupFishingBoardReward* pRet = new(std::nothrow) PopupFishingBoardReward();
    if ( pRet && pRet->init(strReward, bFinish))
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

PopupFishingBoardReward::PopupFishingBoardReward() :

_strReward(""),
_bFinish(false),

_objItems(nullptr)
{
    
}

PopupFishingBoardReward::~PopupFishingBoardReward()
{
    if ( _objItems != nullptr )
    {
        _objItems->release();
        _objItems = nullptr;
    }
}

bool PopupFishingBoardReward::init(std::string strReward, bool bFinish)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _strReward = strReward;
    _bFinish = bFinish;
    
    //
    initVar();
    initUI();
    
    return true;
    
}

#pragma mark - init
void PopupFishingBoardReward::initVar()
{
    _objItems = nullptr;
    auto listItems = ItemsManager::getInstance()->getConvertArray(_strReward);
    if ( listItems.size() != 0 )
    {
        _objItems = listItems.at(0);
        _objItems->retain();
    }
}

void PopupFishingBoardReward::initUI()
{
    Size size = Size(550, 640);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    std::string strTitle = GAME_TEXT("t_ui_event_dice_reward_1_title");
    if ( _objItems == nullptr )
    {
        strTitle = GAME_TEXT("t_ui_event_dice_reward_2_title");
    }
    else if ( _bFinish == true )
    {
        strTitle = GAME_TEXT("t_ui_event_dice_complete_1");
    }
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    //
    uiReward();
    
 
}

void PopupFishingBoardReward::uiReward()
{
    //
    auto layerButton = Layer::create();
    layerButton->setContentSize(Size(_spriteContainer->getContentSize().width - 50, 89));
    layerButton->setIgnoreAnchorPointForPosition(false);
    layerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    layerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(layerButton);
    {
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(layerButton->getContentSize().width / 2, layerButton->getContentSize().height / 2);
        layerButton->addChild(menu);
        
        // item
        auto spriteClose = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteClose->setContentSize(Size(150, 80));
        
        auto itemClose = MafNode::MafMenuItemSprite::create(spriteClose, nullptr, CC_CALLBACK_1(PopupFishingBoardReward::onClickClose, this));
        menu->addChild(itemClose);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
            labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
            labelText->setOverflow(Label::Overflow::SHRINK);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setDimensions(itemClose->getContentSize().width - 20, labelText->getContentSize().height);
            labelText->setColor( Color3B(255,216,0) );
            itemClose->addChild(labelText);
        }
        
        //
        menu->alignItemsHorizontallyWithPadding(10);
    }
    
    //
    auto layerContents = Layer::create();
    layerContents->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - 25 - 25 - layerButton->getContentSize().height - 25));
    layerContents->setIgnoreAnchorPointForPosition(false);
    layerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    layerContents->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25);
    _spriteContainer->addChild(layerContents);
    {
        if ( _objItems == nullptr )
        {
            auto spriteBG = Sprite::create("Assets/ui/event/event_game_img_1_1.png");
            spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            spriteBG->setPosition(layerContents->getContentSize().width / 2, layerContents->getContentSize().height);
            layerContents->addChild(spriteBG);
            
            
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_reward_2"), GAME_FONT, 25);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelText->setPosition(layerContents->getContentSize().width / 2, 0);
            labelText->setDimensions(layerContents->getContentSize().width - 10, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            labelText->setColor(Color3B(255, 64, 64));
            layerContents->addChild(labelText);
        }
        else
        {
            auto spriteEffect = Sprite::create("Assets/ui/event/event_fx1_1.png");
            spriteEffect->setPosition(layerContents->getContentSize().width / 2,  layerContents->getContentSize().height / 2);
            layerContents->addChild(spriteEffect);
            
            auto spriteIcon = Sprite::create(_objItems->getPath());
            if ( spriteIcon != nullptr )
            {
                spriteIcon->setPosition(spriteEffect->getPosition());
                spriteIcon->getTexture()->setTexParameters(texParams);
                spriteIcon->setScale(4.0);
                layerContents->addChild(spriteIcon);
            }
            
            std::string strText = _objItems->getText();
            strText.append("\n");
            strText.append(GAME_TEXTFORMAT("t_ui_event_dice_reward_1_1", std::atoi(_objItems->getCount().c_str())));
            if ( _bFinish == true )
            {
                strText = GAME_TEXT("t_ui_event_dice_complete_2");
            }
            
            auto labelText = Label::createWithTTF(strText, GAME_FONT, 40);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelText->setPosition(layerContents->getContentSize().width / 2, 0);
            labelText->setDimensions(layerContents->getContentSize().width - 10, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            labelText->setColor(Color3B(255, 212, 0));
            layerContents->addChild(labelText);
        }
    }
}

#pragma mark - click
void PopupFishingBoardReward::onClickClose(cocos2d::Ref* sender)
{
    hide();
}
