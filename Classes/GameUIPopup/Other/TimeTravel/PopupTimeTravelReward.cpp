//
//  PopupTimeTravelReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupTimeTravelReward.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/TimeTravelManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupTimeTravelReward* PopupTimeTravelReward::create()
{
    PopupTimeTravelReward* pRet = new(std::nothrow) PopupTimeTravelReward();
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

PopupTimeTravelReward::PopupTimeTravelReward(void) :
_nRewardFloor(0),
_nRewardGem(0),
_nRewardPoint(0),
_strRewardKey(""),
_strRewardGold(""),

_onCallbackResult(nullptr),

_layerContainerFloor(nullptr),
_layerContainerRewardCurrency(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupTimeTravelReward::~PopupTimeTravelReward(void)
{
    
}

bool PopupTimeTravelReward::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupTimeTravelReward::initVar()
{
    _nRewardFloor = TimeTravelManager::getInstance()->getRewardFloor();
    _nRewardGem = TimeTravelManager::getInstance()->getRewardGem();
    _nRewardPoint = TimeTravelManager::getInstance()->getRewardPoint();
    _strRewardKey = TimeTravelManager::getInstance()->getRewardKey();
    _strRewardGold = TimeTravelManager::getInstance()->getRewardGold();
}

void PopupTimeTravelReward::initUi()
{
    Size size = Size(630, 564);
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_time_jump_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupTimeTravelReward::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerFloor = LayerColor::create();
    _layerContainerFloor->setContentSize( Size(size.width - 50, 86) );
    _layerContainerFloor->setIgnoreAnchorPointForPosition(false);
    _layerContainerFloor->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerFloor->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25);
    _spriteContainer->addChild(_layerContainerFloor);
    
    _layerContainerRewardCurrency = LayerColor::create();
    _layerContainerRewardCurrency->setContentSize( Size(size.width - 50, 225) );
    _layerContainerRewardCurrency->setIgnoreAnchorPointForPosition(false);
    _layerContainerRewardCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerRewardCurrency->setPosition(_spriteContainer->getContentSize().width / 2, 25 + _layerContainerButton->getContentSize().height + 25);
    _spriteContainer->addChild(_layerContainerRewardCurrency);
    
    
    //
    uiFloor();
    uiRewardCurrency();
    uiButton();
}

#pragma mark - ui
void PopupTimeTravelReward::uiFloor()
{
    //
    _layerContainerFloor->removeAllChildren();
    
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    int floorPlus = UserInfoManager::getInstance()->getFloor(_ePlace) + _nRewardFloor;
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png", Rect::ZERO, Rect(0,0,20,20));
    spriteBG->setContentSize(Size(_layerContainerFloor->getContentSize().width, 86));
    spriteBG->setPosition(_layerContainerFloor->getContentSize().width / 2, _layerContainerFloor->getContentSize().height / 2);
    _layerContainerFloor->addChild(spriteBG);
    
    
    auto spriteIcon = Sprite::create("Assets/ui/offline/icon_offline_normal.png");
    spriteIcon->setPosition(100, spriteBG->getContentSize().height / 2);
    spriteBG->addChild(spriteIcon);
    
    auto spriteArrow = Sprite::create("Assets/ui_common/btn_shortcuts_4.png");
    spriteArrow->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
    spriteBG->addChild(spriteArrow);
    
    auto labelFloorPrev = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_town_floor", floor), GAME_FONT, 28);
    labelFloorPrev->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    labelFloorPrev->setPosition(spriteBG->getContentSize().width / 2 - 40, spriteBG->getContentSize().height / 2);
    spriteBG->addChild(labelFloorPrev);
    
    auto labelFloorNext = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_town_floor", floorPlus), GAME_FONT, 28);
    labelFloorNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelFloorNext->setPosition(spriteBG->getContentSize().width / 2 + 40, spriteBG->getContentSize().height / 2);
    spriteBG->addChild(labelFloorNext);
    
    auto spriteFloorBG = Sprite::create("Assets/ui_common/text_box_s1.png");
    spriteFloorBG->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height);
    spriteBG->addChild(spriteFloorBG);
    
    auto labelFloorPlus = Label::createWithTTF(MafUtils::format("+ %d", _nRewardFloor), GAME_FONT, 26);
    labelFloorPlus->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height + 10);
    labelFloorPlus->setColor(Color3B(81, 255, 0));
    spriteBG->addChild(labelFloorPlus);
}

void PopupTimeTravelReward::uiRewardCurrency()
{
    //
    _layerContainerRewardCurrency->removeAllChildren();

    //
    auto spriteTitleBG = Sprite::create("Assets/ui_common/base_text_1_4.png");
    spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteTitleBG->setPosition(_layerContainerRewardCurrency->getContentSize().width / 2, _layerContainerRewardCurrency->getContentSize().height);
    _layerContainerRewardCurrency->addChild(spriteTitleBG);
    {
        auto labelText = Label::createWithTTF("REWARD", GAME_FONT, 24);
        labelText->setPosition(spriteTitleBG->getContentSize().width / 2, spriteTitleBG->getContentSize().height / 2);
        spriteTitleBG->addChild(labelText);
    }
    
 
    //
    int maxCol = 2;
    for ( int i = 0; i < 4; i++ )
    {
        int x = i % maxCol;
        int y = i / maxCol;
        double posX = (48) + 242 * x + 10 * x;
        double posY = (_layerContainerRewardCurrency->getContentSize().height - 70) - 41 * y - 10 * y;

        auto spriteBG = Sprite::create("Assets/ui/offline/offline_item_box1.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteBG->setPosition(posX, posY);
        _layerContainerRewardCurrency->addChild(spriteBG);

        std::string strIconPath = "";
        std::string strText = "";
        switch (i) {
            case 0:
            {
                strIconPath = "Assets/icon/menu_key.png";
                strText = _strRewardKey;
                MafUtils::convertNumberToShort(strText);
                break;
            }
            case 1:
            {
                strIconPath = "Assets/icon/icon_point1.png";
                int nReward = _nRewardPoint;
                strText = MafUtils::toString(nReward);
                break;
            }
            case 2:
            {
                strIconPath = "Assets/icon/icon_jewel.png";
                int nReward = _nRewardGem;
                strText = MafUtils::toString(nReward);
                break;
            }
            case 3:
            {
                strIconPath = "Assets/icon/menu_coin.png";
                strText = _strRewardGold;
                MafUtils::convertNumberToShort(strText);
                break;
            }
        }

        if ( strIconPath.length() == 0 )
        {
            continue;
        }

        auto spriteIcon = Sprite::create(strIconPath);
        spriteIcon->setPosition(40, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteIcon);

        auto labelText = Label::createWithTTF(strText, GAME_FONT, 20);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(80, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(labelText);
    }
    
    //
    auto labelInfo = Label::createWithTTF(GAME_TEXT("t_ui_time_jump_reward_mag"), GAME_FONT, 24);
    labelInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelInfo->setPosition(_layerContainerRewardCurrency->getContentSize().width / 2, 0);
    labelInfo->setDimensions(_layerContainerRewardCurrency->getContentSize().width * 0.9, labelInfo->getContentSize().height);
    labelInfo->setOverflow(Label::Overflow::SHRINK);
    labelInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _layerContainerRewardCurrency->addChild(labelInfo);
}

void PopupTimeTravelReward::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto itemConfirm = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_ok.png"), nullptr, CC_CALLBACK_1(PopupTimeTravelReward::onClickClose, this));
    menu->addChild(itemConfirm);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 26);
        label->setPosition( itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
        itemConfirm->addChild(label);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

#pragma mark - set, get, sort, add


#pragma mark - click
void PopupTimeTravelReward::onClickClose(Ref* sender)
{
    //
    TimeTravelManager::getInstance()->OnReward();
    
    //
    SaveManager::getInstance()->saveAllData();
    
    //
    UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_normal);
}
