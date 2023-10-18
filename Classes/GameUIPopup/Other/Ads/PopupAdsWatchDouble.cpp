//
//  PopupAdsWatchDouble.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupAdsWatchDouble.h"

USING_NS_CC;

PopupAdsWatchDouble* PopupAdsWatchDouble::create(E_ADS_WATCH eType)
{
    PopupAdsWatchDouble* pRet = new(std::nothrow) PopupAdsWatchDouble();
    if ( pRet && pRet->init(eType) )
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

PopupAdsWatchDouble::PopupAdsWatchDouble(void) :
_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupAdsWatchDouble::~PopupAdsWatchDouble(void)
{
    
}

bool PopupAdsWatchDouble::init(E_ADS_WATCH eType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _eType = eType;
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupAdsWatchDouble::initVar()
{
    
}

void PopupAdsWatchDouble::initUi()
{
    Size size = Size(497, 469);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    // 높이 230
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_notice"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdsWatchDouble::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupAdsWatchDouble::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    std::string strPath = "";
    switch (_eType) {
        case E_ADS_WATCH::BUFF_COIN:
            strPath = "Assets/ui/ads/notice_buff_01.png";
            break;
        case E_ADS_WATCH::BUFF_ATK:
            strPath = "Assets/ui/ads/notice_buff_02.png";
            break;
        case E_ADS_WATCH::BUFF_SPD:
            strPath = "Assets/ui/ads/notice_buff_03.png";
            break;
        case E_ADS_WATCH::BUFF_ALL:
            strPath = "Assets/ui/ads/notice_buff_04.png";
            break;
        case E_ADS_WATCH::BUFF_RAND:
            strPath = "Assets/ui/ads/notice_buff_05.png";
            break;
        case E_ADS_WATCH::BUFF_ATK_9:
            strPath = "Assets/ui/ads/notice_buff_06.png";
            break;
        case E_ADS_WATCH::BUFF_ATK_10:
            strPath = "Assets/ui/ads/notice_buff_07.png";
            break;
        case E_ADS_WATCH::BUFF_DEVIL_RAND:
            strPath = "Assets/ui/ads/notice_buff_08.png";
            break;
        case E_ADS_WATCH::BUFF_D_ATK_DMG:
            strPath = "Assets/ui/ads/notice_buff_10.png";
            break;
        case E_ADS_WATCH::BUFF_D_ATK_SPD:
            strPath = "Assets/ui/ads/notice_buff_11.png";
            break;
        case E_ADS_WATCH::BUFF_D_MOVE_SPD:
            strPath = "Assets/ui/ads/notice_buff_13.png";
            break;
        case E_ADS_WATCH::BUFF_D_QUEST_GOLD:
            strPath = "Assets/ui/ads/notice_buff_12.png";
            break;
        case E_ADS_WATCH::BUFF_EVENT_TIME:
            strPath = "Assets/ui/ads/notice_buff_09.png";
            break;
        case E_ADS_WATCH::ITEM_TICKET:
            strPath = "Assets/ui/ads/notice_item_01.png";
            break;
        case E_ADS_WATCH::ITEM_GOLD:
            strPath = "Assets/ui/ads/notice_item_02.png";
            break;
        case E_ADS_WATCH::ITEM_GEM:
            strPath = "Assets/ui/ads/notice_item_03.png";
            break;
        case E_ADS_WATCH::ITEM_CUBE:
            strPath = "Assets/ui/ads/notice_item_04.png";
            break;
        case E_ADS_WATCH::ITEM_DEVILCOIN:
            strPath = "Assets/ui/ads/notice_item_05.png";
            break;
        case E_ADS_WATCH::ITEM_KEY:
            strPath = "Assets/ui/ads/notice_item_06.png";
            break;
        case E_ADS_WATCH::ITEM_POINT:
            strPath = "Assets/ui/ads/notice_item_07.png";
            break;
        case E_ADS_WATCH::ITEM_DEVIL_GOLD:
            strPath = "Assets/ui/ads/notice_item_08.png";
            break;
        case E_ADS_WATCH::ITEM_PTICKET:
            strPath = "Assets/ui/ads/notice_item_09.png";
            break;
        case E_ADS_WATCH::ITEM_PET_EQUIP_BOX:
            strPath = "Assets/ui/ads/notice_item_10.png";
            break;
        case E_ADS_WATCH::ITEM_BONG:
            strPath = "Assets/ui/ads/notice_item_11.png";
            break;
        case E_ADS_WATCH::MINE_RAND_1:
            strPath = "Assets/ui/ads/notice_mineral_01.png";
            break;
        case E_ADS_WATCH::MINE_RAND_2:
            strPath = "Assets/ui/ads/notice_mineral_02.png";
            break;
        case E_ADS_WATCH::MINE_RAND_3:
            strPath = "Assets/ui/ads/notice_mineral_03.png";
            break;
        case E_ADS_WATCH::MINE_RAND_4:
            strPath = "Assets/ui/ads/notice_mineral_04.png";
            break;
        case E_ADS_WATCH::ITEM_NEW_RAID_DRAGON_BALL:
            strPath = "Assets/ui/ads/notice_item_12.png";
            break;
        default:
            break;
    }
    
    if ( strPath.length() != 0 )
    {
        auto spriteIcon = Sprite::create(strPath);
        spriteIcon->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2);
        _layerContainerContents->addChild(spriteIcon);
        
    }
}

void PopupAdsWatchDouble::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto itemOk = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_popup_mini_2_1.png"), nullptr, CC_CALLBACK_1(PopupAdsWatchDouble::onClickOk, this));
    menu->addChild(itemOk);
 
    auto itemChange = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_popup_mini_2_2.png"), nullptr, CC_CALLBACK_1(PopupAdsWatchDouble::onClickAds, this));
    menu->addChild(itemChange);
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}


#pragma mark - set, get, sort, add
void PopupAdsWatchDouble::setCallbackResult(const std::function<void(bool)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupAdsWatchDouble::onClickClose(Ref* sender)
{
    hide();
}

void PopupAdsWatchDouble::onClickOk(Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult(false);
    }
    
    hide();
}

void PopupAdsWatchDouble::onClickAds(cocos2d::Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult(true);
    }
    
    hide();
}

