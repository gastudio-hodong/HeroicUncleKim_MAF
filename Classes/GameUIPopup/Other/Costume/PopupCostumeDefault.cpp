//
//  PopupCostumeDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/06/2019.
//

#include "PopupCostumeDefault.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/BattlePrison/PopupPrison.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBanner.h"
#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Costume/Layer/UILayerCostumeDefault.h"
#include "GameUIPopup/Other/Costume/Layer/UILayerCostumeSet.h"
#include "GameUIPopup/Other/Costume/Layer/UILayerCostumeJewel.h"
#include "GameUIPopup/Other/CostumeSkin/PopupCostumeSkin.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"
#include "GameUIPopup/Other/Stone/PopupStone.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/StoryManager.h"

USING_NS_CC;

PopupCostumeDefault* PopupCostumeDefault::create()
{
    PopupCostumeDefault* pRet = new(std::nothrow) PopupCostumeDefault();
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

PopupCostumeDefault::PopupCostumeDefault(void) :
_strType("HEAD"),


_layerContainerParent(nullptr),
_layerContainerTop(nullptr),
_layerContainerContent(nullptr),
_layerContainerContentBar(nullptr),
_layerContainerContentCurrency(nullptr),
_layerContainerContentChild(nullptr),
_layerContainerButton(nullptr),

_layerHero(nullptr),


_itemAdsSimple(nullptr)
{
    
}

PopupCostumeDefault::~PopupCostumeDefault(void)
{
    
}

bool PopupCostumeDefault::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME, this);
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME_BUY, this);
    
    //
    bool bClear = StoryManager::getInstance()->isStoryOpen((int)E_COMIC::COSTUME_ROOM);
    if ( bClear == false )
    {
        StoryManager::getInstance()->showStory((int)E_COMIC::COSTUME_ROOM);
    }
    else
    {
        if ( CostumeManager::getInstance()->isExistAll() )
        {
            StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_REALENDING);
        }
    }
    
    // 로그 이벤트
    MafAnalyticsManager::LogEvent(kAnalEventCostumeEnter, kRepeatTrue);
    
    return true;
}

void PopupCostumeDefault::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    CostumeManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupCostumeDefault::callbackInfo, this));
}

#pragma mark - override
void PopupCostumeDefault::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::COSTUME )
    {
        drawHero();
    }
}
void PopupCostumeDefault::hide()
{
    PopupBase::hide();
}

#pragma mark - init
void PopupCostumeDefault::initVar()
{
    
}

void PopupCostumeDefault::initUi()
{
    _layerContainerParent = Layer::create();
    _layerContainerParent->setContentSize( getContentSize() );
    _layerContainerParent->setIgnoreAnchorPointForPosition(false);
    _layerContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainerParent->setPosition( getContentSize().width / 2, getContentSize().height / 2 );
    addChild(_layerContainerParent);
    
    // bg
    double nBGHeight = 400 + UserInfoManager::getInstance()->_fHeightIphoneX;
    
    auto spriteBG = Sprite::create("Assets/battle_bg/prison/dressroom_bg.png");
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteBG->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height - nBGHeight);
    spriteBG->getTexture()->setTexParameters(texParams);
    spriteBG->setScale(MAX(_layerContainerParent->getContentSize().width / spriteBG->getContentSize().width, nBGHeight / spriteBG->getContentSize().height));
    _layerContainerParent->addChild(spriteBG);
    
    // top layer
    _layerContainerTop = Layer::create();
    _layerContainerTop->setContentSize(Size(_layerContainerParent->getContentSize().width, nBGHeight - UserInfoManager::getInstance()->_fHeightIphoneX));
    _layerContainerTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerTop->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerParent->getContentSize().height - nBGHeight);
    _layerContainerTop->setOpacity(200);
    _layerContainerParent->addChild(_layerContainerTop);
    
    // button layer
    _layerContainerButton = LayerColor::create(Color4B(19, 26, 33, 255));
    _layerContainerButton->setContentSize(Size(_layerContainerParent->getContentSize().width, 105));
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerButton->setPosition(_layerContainerParent->getContentSize().width / 2, 0);
    _layerContainerParent->addChild(_layerContainerButton);
    
    
    // bottom layer
    _sizeContent = Size(_layerContainerParent->getContentSize().width, _layerContainerParent->getContentSize().height - nBGHeight - _layerContainerButton->getContentSize().height);
    _layerContainerContent = LayerColor::create(Color4B(19, 26, 33, 255));
    _layerContainerContent->setContentSize(_sizeContent);
    _layerContainerContent->setIgnoreAnchorPointForPosition(false);
    _layerContainerContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerContent->setPosition(_layerContainerParent->getContentSize().width / 2, _layerContainerButton->getContentSize().height);
    _layerContainerParent->addChild(_layerContainerContent);
    
    
    //
    uiTop();
    uiButton();
    uiContent();
}

#pragma mark - ui
void PopupCostumeDefault::uiTop()
{
    _layerContainerTop->removeAllChildren();
    
    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_costume_title"), GAME_FONT, 28);
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelTitle->setPosition(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height - 10);
    labelTitle->enableOutline(Color4B::BLACK, 1);
    _layerContainerTop->addChild(labelTitle);
    
    // hero layer
    _layerHero = Layer::create();
    _layerHero->setContentSize(Size(232, 232));
    _layerHero->setIgnoreAnchorPointForPosition(false);
    _layerHero->setPosition(_layerContainerTop->getContentSize().width / 2, 150);
    _layerContainerTop->addChild(_layerHero);
    {
        //
        auto pHero = UnitBehaviorHero::create(E_PLACE::EMPTY);
        pHero->setSkinUse(false);
        pHero->setMotion();
        pHero->actionRun();
        pHero->setTalkShow(false);
        pHero->setPosition(_layerHero->getContentSize().width / 2 + 175, 10);
        pHero->setName("HERO");
        _layerHero->addChild(pHero);
    }
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerTop->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/btn_exit_2.png"), Sprite::create("Assets/ui/costume/btn_exit_2.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickClose, this));
    itemClose->setPosition(_layerContainerTop->getContentSize().width - 70, _layerContainerTop->getContentSize().height - 80);
    menu->addChild(itemClose);
    
    auto itemCash = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/event_1.png"), Sprite::create("Assets/ui/costume/event_1.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickCashBanner, this));
    itemCash->setPosition(_layerContainerTop->getContentSize().width - 70, _layerContainerTop->getContentSize().height  - 180);
    menu->addChild(itemCash);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for ( int i = 1; i <= 4; i++ )
        {
            animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/costume/event_%d.png", i));
        }
        
        itemCash->getNormalImage()->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    _itemAdsSimple = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/ad_time_remaining1_2.png"), Sprite::create("Assets/ui_common/ad_time_remaining1_2.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickAds, this));
    _itemAdsSimple->setPosition(_layerContainerTop->getContentSize().width - _itemAdsSimple->getContentSize().width / 2, _layerContainerTop->getContentSize().height  - 280);
    menu->addChild(_itemAdsSimple);
    
    auto itemSkin = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_01.png"), Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_01.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickSkin, this));
    itemSkin->setPosition(itemSkin->getContentSize().width / 2, 120);
    menu->addChild(itemSkin);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_skin.png");
        spriteIcon->setPosition(25, itemSkin->getContentSize().height / 2);
        itemSkin->addChild(spriteIcon);
        
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_costume_skin"), GAME_FONT, 20);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(50, itemSkin->getContentSize().height / 2);
        labelText->setDimensions(100, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemSkin->addChild(labelText);
        
        bool bShow = UserDefault::getInstance()->getBoolForKey(KEY_REDDOT_COSTUME_SKIN2, false);
        if ( bShow == false )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_RIGHT);
            uiReddot->setPosition(itemSkin->getContentSize().width - 5, itemSkin->getContentSize().height - 5);
            itemSkin->addChild(uiReddot);
        }
    }
    
    auto itemPrison = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_01.png"), Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_01.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickPrison, this));
    itemPrison->setPosition(itemPrison->getContentSize().width / 2, 180);
    menu->addChild(itemPrison);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_prison_2.png");
        spriteIcon->setPosition(25, itemPrison->getContentSize().height / 2);
        itemPrison->addChild(spriteIcon);
        
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_prison"), GAME_FONT, 20);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(50, itemPrison->getContentSize().height / 2);
        labelText->setDimensions(100, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemPrison->addChild(labelText);
    }
    
    auto itemRaid = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_01.png"),  Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_01.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickRaid, this));
    itemRaid->setPosition(itemRaid->getContentSize().width / 2, 240);
    menu->addChild(itemRaid);
    {
        auto spriteIcon = Sprite::create("Assets/ui/menu_bottom/menu_btn_raid_3.png");
        spriteIcon->setPosition(25, itemRaid->getContentSize().height / 2);
        itemRaid->addChild(spriteIcon);
        
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_new_raid_title"), GAME_FONT, 20);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(50, itemRaid->getContentSize().height / 2);
        labelText->setDimensions(100, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemRaid->addChild(labelText);
    }
    
    //
    drawHero();
}

void PopupCostumeDefault::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    menu->setName("MENU");
    _layerContainerButton->addChild(menu);

    //
    std::string strPathItemBg = "Assets/ui/menu_bottom/costume_menu_btn_bg_off_new.png";
    std::string strPathItemBgSelect = "Assets/ui/menu_bottom/costume_menu_btn_bg_on_new.png";

    auto spriteTemp = Sprite::create(strPathItemBg);
    Size sizeItemBg = spriteTemp->getContentSize();


    struct element
    {
        std::string tag;
        std::string strImage;
        std::string strNameKey;
    };

    element listElement[] = {//아래줄
        { "HEAD",       "Assets/ui/costume/icon_head.png",      "t_ui_costume_helmet"        },
        { "BODY",       "Assets/ui/costume/icon_body.png",      "t_ui_costume_armor"      },
        { "CAPE",       "Assets/ui/costume/icon_cape.png",      "t_ui_costume_cape"          },
        { "HAND",       "Assets/ui/costume/icon_hand.png",      "t_ui_costume_gloves"    },
        { "FOOT",       "Assets/ui/costume/icon_foot.png",      "t_ui_costume_shoes"      },
        { "JEWEL",      "Assets/ui/costume/icon_jewel.png",     "t_ui_costume_jewel"      },
        { "SETITEM",      "Assets/ui/costume/icon_set.png",     "t_ui_costume_set"      },
    };
    
    for ( int i = 0; i < sizeof(listElement)/sizeof(element); i++ )
    {
        element ele = listElement[i];

        MafNode::MafMenuItemSprite *item = MafNode::MafMenuItemSprite::create(Sprite::create(strPathItemBg),  Sprite::create(strPathItemBgSelect), CC_CALLBACK_1(PopupCostumeDefault::onClickButton, this));
        item->setName(ele.tag);
        item->getSelectedImage()->setScale(1);
        menu->addChild(item);

        auto labelName = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 22);
        labelName->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        labelName->setPosition( Vec2(item->getContentSize().width / 2, 3) );
        labelName->setDimensions(item->getContentSize().width * 0.9, 40);
        labelName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelName->setOverflow(Label::Overflow::SHRINK);
        labelName->setTextColor(Color4B(115, 116, 118, 255));
        labelName->setTag(10000);
        item->addChild(labelName);

        auto sprIcon = Sprite::create(ele.strImage);
        sprIcon->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2 + 5);
        item->addChild(sprIcon);
    }
    
    menu->alignItemsHorizontallyWithPadding(0);
    
    //
    drawButton();
}

void PopupCostumeDefault::uiContent()
{
    uiContentBar();
    uiContentCurrency();
    
    //
    drawContent();
}

void PopupCostumeDefault::uiContentBar()
{
    double nWidth = _layerContainerContent->getContentSize().width;
    double nHeight = 55;
    
    if ( _layerContainerContentBar != nullptr )
    {
        _layerContainerContentBar->removeFromParent();
    }
    
    _layerContainerContentBar = Layer::create();
    _layerContainerContentBar->setContentSize( Size(nWidth, nHeight) );
    _layerContainerContentBar->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentBar->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerContentBar->setPosition( _layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height );
    _layerContainerContent->addChild(_layerContainerContentBar);
    
    // bar
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerContentBar->addChild(menu);
    
    auto itemBar = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/menu_btn_list_bar.png"), Sprite::create("Assets/ui/costume/menu_btn_list_bar.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickContentBar, this));
    itemBar->setPosition(_layerContainerContentBar->getContentSize().width / 2, _layerContainerContentBar->getContentSize().height / 2);
    menu->addChild(itemBar);
    {
        auto spriteBarArrow = Sprite::create("Assets/ui/costume/menu_btn_list_1.png");
        spriteBarArrow->setPosition(itemBar->getContentSize().width / 2, itemBar->getContentSize().height / 2);
        itemBar->addChild(spriteBarArrow);
    }
    
    auto itemStone = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/menu_btn_list_bar2.png"), Sprite::create("Assets/ui/costume/menu_btn_list_bar2.png"), nullptr, CC_CALLBACK_1(PopupCostumeDefault::onClickButtonStone, this));
    itemStone->setPosition(0 + itemStone->getContentSize().width / 2, 22 + itemStone->getContentSize().height /2);
    menu->addChild(itemStone);
    {
        auto spriteIcon = Sprite::create("Assets/ui/costume/menu_btn_stone.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteIcon->setPosition(20, itemStone->getContentSize().height / 2);
        itemStone->addChild(spriteIcon);
        
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_stone_reinforce"), GAME_FONT, 20);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        label->setPosition(80, itemStone->getContentSize().height / 2);
        itemStone->addChild(label);
    }
}

void PopupCostumeDefault::uiContentCurrency()
{
    double nWidth = _layerContainerContent->getContentSize().width;
    double nHeight = 60;

    if ( _layerContainerContentCurrency != nullptr )
    {
        _layerContainerContentCurrency->removeFromParent();
    }

    _layerContainerContentCurrency = Layer::create();
    _layerContainerContentCurrency->setContentSize( Size(nWidth, nHeight) );
    _layerContainerContentCurrency->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContentCurrency->setPosition( _layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height );
    _layerContainerContent->addChild(_layerContainerContentCurrency);

    //
    auto layerBg = LayerColor::create(Color4B::BLACK, _layerContainerContentCurrency->getContentSize().width, _layerContainerContentCurrency->getContentSize().height);
    layerBg->setPosition(Vec2::ZERO);
    _layerContainerContentCurrency->addChild(layerBg);


    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::MATERIAL);
    list.push_back(E_ITEMS::HONOR);
    list.push_back(E_ITEMS::COSTUME_COUPON);

    auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 40));
    uiCurrency->setPosition(Vec2(_layerContainerContentCurrency->getContentSize().width / 2, _layerContainerContentCurrency->getContentSize().height / 2));
    _layerContainerContentCurrency->addChild(uiCurrency, E_ZORER::UI+1);
}

#pragma mark - ui draw
void PopupCostumeDefault::drawHero()
{
    //
    auto objHero = (UnitBehaviorHero*)_layerHero->getChildByName("HERO");
    if ( objHero == nullptr )
    {
        return;
    }
    
    objHero->setChangeParts();
    objHero->actionRun();
}

void PopupCostumeDefault::drawContent()
{
    //
    _layerContainerContentBar->setPositionY(_layerContainerContent->getContentSize().height);
    _layerContainerContentCurrency->setPositionY(_layerContainerContent->getContentSize().height);
    
    //
    double nWidth = _layerContainerContent->getContentSize().width;
    double nHeight = _layerContainerContent->getContentSize().height -_layerContainerContentCurrency->getContentSize().height ;
    Layer* layerTemp = nullptr;
    if ( _strType.compare("SETITEM") == 0 )
    {
        layerTemp = (Layer*)_layerContainerContent->getChildByName(_strType);
        if ( layerTemp == nullptr )
        {
            layerTemp = UILayerCostumeSet::create(_strType, Size(nWidth, nHeight));
            layerTemp->setName(_strType);
            _layerContainerContent->addChild(layerTemp);
        }
    }
    else if ( _strType.compare("JEWEL") == 0 )
    {
        layerTemp = (Layer*)_layerContainerContent->getChildByName(_strType);
        if ( layerTemp == nullptr )
        {
            layerTemp = UILayerCostumeJewel::create(_strType, Size(nWidth, nHeight));
            layerTemp->setName(_strType);
            _layerContainerContent->addChild(layerTemp);
        }
    }
    else
    {
        layerTemp = (Layer*)_layerContainerContent->getChildByName(_strType);
        if ( layerTemp == nullptr )
        {
            layerTemp = UILayerCostumeDefault::create(_strType, Size(nWidth, nHeight));
            layerTemp->setName(_strType);
            _layerContainerContent->addChild(layerTemp);
        }
    }
    
    if ( layerTemp == nullptr )
    {
        return;
    }
    
    if ( layerTemp == _layerContainerContentChild )
    {
        _layerContainerContentChild->setContentSize(Size(nWidth, nHeight));
        ((UILayerCostumeParent*)_layerContainerContentChild)->refreshList();
        return;
    }
    
    if ( _layerContainerContentChild != nullptr )
    {
        _layerContainerContentChild->setVisible(false);
        _layerContainerContentChild = nullptr;
    }
        
    _layerContainerContentChild = layerTemp;
    _layerContainerContentChild->setVisible(true);
    _layerContainerContentChild->setContentSize(Size(nWidth, nHeight));
    ((UILayerCostumeParent*)_layerContainerContentChild)->refreshList();
}

void PopupCostumeDefault::drawButton()
{
    auto menu = (MafNode::MafMenu*)_layerContainerButton->getChildByName("MENU");
    
    auto listItem = (Vector<MafNode::MafMenuItemSprite*>&)menu->getChildren();
    for ( auto item : listItem )
    {
        auto label = (Label*)item->getChildByTag(10000);
        if ( label == nullptr )
        {
            continue;
        }
            
        if ( item->getName() == _strType )
        {
            item->selected();
            label->setTextColor(Color4B::WHITE);
        }
        else
        {
            item->unselected();
            label->setTextColor(Color4B(115, 116, 118, 255));
        }
    }
}

void PopupCostumeDefault::drawAdsSimple()
{
    std::string strPath = "Assets/ui_common/ad_time_remaining1_1.png";
    if ( CostumeManager::getInstance()->getAdsCount() <= 0 )
    {
        strPath = "Assets/ui_common/ad_time_remaining1_2.png";
    }
        
    ((Sprite*)_itemAdsSimple->getNormalImage())->setTexture(strPath);
    ((Sprite*)_itemAdsSimple->getSelectedImage())->setTexture(strPath);
}

void PopupCostumeDefault::drawRedDotAction(cocos2d::Node *widget)
{
    auto spriteSpotLight = Sprite::create("Assets/ui_common/icon_red_1_2.png");
    spriteSpotLight->setPosition(widget->getContentSize().width / 2, widget->getContentSize().height / 2);
    widget->addChild(spriteSpotLight);

    auto fideOut = FadeOut::create(0.5f);
    auto fideIn = FadeIn::create(0.5f);
    auto seq = Sequence::create(fideOut, fideIn, NULL);
    spriteSpotLight->runAction(RepeatForever::create(seq));
}

#pragma mark - set, get, other

#pragma mark - click
void PopupCostumeDefault::onClickClose(Ref *sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    item->setEnabled(false);
    
    hide();
}

void PopupCostumeDefault::onClickCashBanner(Ref *sender)
{
    bool bEvent = false;
    if ( EventManager::getInstance()->isActivatedEvent(21) || EventManager::getInstance()->isActivatedEvent(22) )
    {
        bEvent = true;
        
        auto list = CashManager::getInstance()->getBannerAll();
        for ( const auto &obj : list )
        {
            if ( obj->getItemIdx() == 0 && obj->isReceive() == true )
            {
                bEvent = false;
            }
        }
    }
        
    if ( bEvent == true )
    {
        auto popup = PopupPurchaseBanner::create();
        popup->show();
    }
    else
    {
        auto popup = PopupCash::create();
        popup->show();
    }
}

void PopupCostumeDefault::onClickAds(Ref *sender)
{
    if ( CostumeManager::getInstance()->getAdsCount() <= 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_15"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }

    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        auto popup = PopupAdsWatch::create(E_ADS_WATCH::ITEM_DEVILCOIN, 5);
        popup->setCallbackResult([=](){
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupCostumeDefault::callbackAds, this));
        });
        popup->show();
    }
}

void PopupCostumeDefault::onClickSkin(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    //
    item->removeChildByName("UIReddot");
    UserDefault::getInstance()->setBoolForKey(KEY_REDDOT_COSTUME_SKIN2, true);
    
    //
    auto popup = PopupCostumeSkin::create();
    popup->show();
}

void PopupCostumeDefault::onClickPrison(cocos2d::Ref* sender)
{
    auto popup = PopupPrison::create();
    popup->show();
    
    hide();
}

void PopupCostumeDefault::onClickRaid(cocos2d::Ref* sender)
{
    bool bEnter = false;
    
    int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
    if ( nRevival > 1 )
    {
        bEnter = true;
    }
    
    if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
    {
        bEnter = true;
    }
    
    if ( bEnter == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupRaidDefault::create();
    popup->show();
    
    hide();
}

void PopupCostumeDefault::onClickButton(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    _strType = item->getName();
    
    //
    drawButton();
    drawContent();
}

void PopupCostumeDefault::onClickButtonStone(cocos2d::Ref* sender)
{
    auto popup = PopupStone::create();
    popup->show();
}

void PopupCostumeDefault::onClickContentBar(cocos2d::Ref* sender)
{
    double nWidth = _sizeContent.width;
    double nHeight = _sizeContent.height;
    
    bool bExpand = false;
    if ( _sizeContent.height == _layerContainerContent->getContentSize().height )
    {
        bExpand = true;
        nHeight = nHeight + _layerContainerTop->getContentSize().height * 0.7;
    }
    _layerContainerContent->setContentSize(Size(nWidth, nHeight));
    
    //
    if ( bExpand == true )
    {
        auto layerExpandLock = Layer::create();
        layerExpandLock->setContentSize(_layerContainerTop->getContentSize());
        layerExpandLock->setName("EXPAND");
        _layerContainerTop->addChild(layerExpandLock, 1000);
        
        // event
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
            cocos2d::Rect rect = cocos2d::Rect(0, 0, event->getCurrentTarget()->getContentSize().width, event->getCurrentTarget()->getContentSize().height);
            if (rect.containsPoint(event->getCurrentTarget()->convertToNodeSpace(touch->getLocation())))
                return true;

            return false;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layerExpandLock);
    }
    else
    {
        auto layerExpandLock = (Layer*)_layerContainerTop->getChildByName("EXPAND");
        if ( layerExpandLock != nullptr )
        {
            layerExpandLock->removeFromParent();
        }
    }
    
    //
    drawContent();
}
 

#pragma mark - callback
void PopupCostumeDefault::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
  
    //
    drawAdsSimple();
    drawContent();
}

void PopupCostumeDefault::callbackAds(ADS_RESULT eResult)
{
    if ( eResult == ADS_RESULT::SUCCESS )
    {
        PopupLoading::show();
        CostumeManager::getInstance()->requestAds(CC_CALLBACK_2(PopupCostumeDefault::callbackAdsResult, this));
    }
    else
    {
        if ( eResult == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupCostumeDefault::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotCostumeShop);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

void PopupCostumeDefault::callbackAdsResult(bool bResult, int nCurrency)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_costume_msg_22", nCurrency, CostumeManager::getInstance()->getAdsCount()));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    drawAdsSimple();
}

