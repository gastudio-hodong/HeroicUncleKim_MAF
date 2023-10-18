//
//  PopupAdsWatch.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupAdsWatch.h"

USING_NS_CC;

PopupAdsWatch* PopupAdsWatch::create(E_ADS_WATCH eType, int nCount)
{
    return create(eType, MafUtils::toString(nCount));
}
PopupAdsWatch* PopupAdsWatch::create(E_ADS_WATCH eType, std::string strCount)
{
    PopupAdsWatch* pRet = new(std::nothrow) PopupAdsWatch();
    if ( pRet && pRet->init(eType, strCount) )
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

PopupAdsWatch::PopupAdsWatch(void) :
_eType(E_ADS_WATCH::NONE),
_strCount(""),
_strPath(""),

_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupAdsWatch::~PopupAdsWatch(void)
{
    
}

bool PopupAdsWatch::init(E_ADS_WATCH eType, std::string strCount)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _eType = eType;
    _strCount = strCount;
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupAdsWatch::initVar()
{
    _strPath = "";
    switch (_eType) {
        case E_ADS_WATCH::BUFF_COIN:
            _strPath = "Assets/ui/ads/notice_buff_01.png";
            break;
        case E_ADS_WATCH::BUFF_ATK:
            _strPath = "Assets/ui/ads/notice_buff_02.png";
            break;
        case E_ADS_WATCH::BUFF_SPD:
            _strPath = "Assets/ui/ads/notice_buff_03.png";
            break;
        case E_ADS_WATCH::BUFF_ALL:
            _strPath = "Assets/ui/ads/notice_buff_04.png";
            break;
        case E_ADS_WATCH::BUFF_RAND:
            _strPath = "Assets/ui/ads/notice_buff_05.png";
            break;
        case E_ADS_WATCH::BUFF_ATK_9:
            _strPath = "Assets/ui/ads/notice_buff_06.png";
            break;
        case E_ADS_WATCH::BUFF_ATK_10:
            _strPath = "Assets/ui/ads/notice_buff_07.png";
            break;
        case E_ADS_WATCH::BUFF_DEVIL_RAND:
            _strPath = "Assets/ui/ads/notice_buff_08.png";
            break;
        case E_ADS_WATCH::BUFF_D_ATK_DMG:
            _strPath = "Assets/ui/ads/notice_buff_10.png";
            break;
        case E_ADS_WATCH::BUFF_D_ATK_SPD:
            _strPath = "Assets/ui/ads/notice_buff_11.png";
            break;
        case E_ADS_WATCH::BUFF_D_MOVE_SPD:
            _strPath = "Assets/ui/ads/notice_buff_13.png";
            break;
        case E_ADS_WATCH::BUFF_D_QUEST_GOLD:
            _strPath = "Assets/ui/ads/notice_buff_12.png";
            break;
        case E_ADS_WATCH::BUFF_EVENT_TIME:
            _strPath = "Assets/ui/ads/notice_buff_09.png";
            break;
        case E_ADS_WATCH::ITEM_TICKET:
            _strPath = "Assets/ui/ads/notice_item_01.png";
            break;
        case E_ADS_WATCH::ITEM_GOLD:
            _strPath = "Assets/ui/ads/notice_item_02.png";
            break;
        case E_ADS_WATCH::ITEM_GEM:
            _strPath = "Assets/ui/ads/notice_item_03.png";
            break;
        case E_ADS_WATCH::ITEM_CUBE:
            _strPath = "Assets/ui/ads/notice_item_04.png";
            break;
        case E_ADS_WATCH::ITEM_DEVILCOIN:
            _strPath = "Assets/ui/ads/notice_item_05.png";
            break;
        case E_ADS_WATCH::ITEM_KEY:
            _strPath = "Assets/ui/ads/notice_item_06.png";
            break;
        case E_ADS_WATCH::ITEM_POINT:
            _strPath = "Assets/ui/ads/notice_item_07.png";
            break;
        case E_ADS_WATCH::ITEM_DEVIL_GOLD:
            _strPath = "Assets/ui/ads/notice_item_08.png";
            break;
        case E_ADS_WATCH::ITEM_PTICKET:
            _strPath = "Assets/ui/ads/notice_item_09.png";
            break;
        case E_ADS_WATCH::ITEM_PET_EQUIP_BOX:
            _strPath = "Assets/ui/ads/notice_item_10.png";
            break;
        case E_ADS_WATCH::ITEM_BONG:
            _strPath = "Assets/ui/ads/notice_item_11.png";
            break;
        case E_ADS_WATCH::MINE_RAND_1:
            _strPath = "Assets/ui/ads/notice_mineral_01.png";
            break;
        case E_ADS_WATCH::MINE_RAND_2:
            _strPath = "Assets/ui/ads/notice_mineral_02.png";
            break;
        case E_ADS_WATCH::MINE_RAND_3:
            _strPath = "Assets/ui/ads/notice_mineral_03.png";
            break;
        case E_ADS_WATCH::MINE_RAND_4:
            _strPath = "Assets/ui/ads/notice_mineral_04.png";
            break;
        case E_ADS_WATCH::MINE_RAND_5:
            _strPath = "Assets/ui/ads/notice_mineral_05.png";
            break;
        case E_ADS_WATCH::MINE_RAND_6:
            _strPath = "Assets/ui/ads/notice_mineral_06.png";
            break;
        case E_ADS_WATCH::ITEM_NEW_RAID_DRAGON_BALL:
            _strPath = "Assets/ui/ads/notice_item_12.png";
            break;
        default:
            break;
    }
}

void PopupAdsWatch::initUi()
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
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupAdsWatch::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    if ( _strPath.length() == 0 )
    {
        return;
    }
   
    auto spriteIcon = Sprite::create(_strPath);
    spriteIcon->setPosition(_layerContainerContents->getContentSize() / 2);
    _layerContainerContents->addChild(spriteIcon);
    
    if ( _strCount.compare("0") != 0 )
    {
        if ( _eType == E_ADS_WATCH::BUFF_COIN || _eType == E_ADS_WATCH::BUFF_ATK ||
            _eType == E_ADS_WATCH::BUFF_SPD || _eType == E_ADS_WATCH::BUFF_ALL ||
            _eType == E_ADS_WATCH::BUFF_RAND || _eType == E_ADS_WATCH::BUFF_ATK_9 ||
            _eType == E_ADS_WATCH::BUFF_ATK_10 || _eType == E_ADS_WATCH::BUFF_DEVIL_RAND ||
            _eType == E_ADS_WATCH::BUFF_D_ATK_DMG || _eType == E_ADS_WATCH::BUFF_D_ATK_SPD ||
            _eType == E_ADS_WATCH::BUFF_D_MOVE_SPD || _eType == E_ADS_WATCH::BUFF_D_QUEST_GOLD)
        {
            _strCount.append("sec");
        }
        
        auto spriteCountBG = Sprite::create("Assets/ui/ads/notice_quantity_01.png");
        spriteCountBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteCountBG->setPosition(spriteIcon->getContentSize().width / 2, 0);
        spriteIcon->addChild(spriteCountBG);
        
        auto labelCount = Label::createWithTTF(_strCount, GAME_FONT_AL, 30);
        labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelCount->setPosition(spriteCountBG->getBoundingBox().size.width / 2, 10);
        labelCount->setColor(Color3B::WHITE);
        labelCount->enableOutline(Color4B(67, 107, 106, 255), 4);
        spriteCountBG->addChild(labelCount);
    }
}

void PopupAdsWatch::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_popup_mini_close.png"), nullptr, CC_CALLBACK_1(PopupAdsWatch::onClickClose, this));
    menu->addChild(itemClose);
 
    auto itemChange = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_popup_mini_ad.png"), nullptr, CC_CALLBACK_1(PopupAdsWatch::onClickAds, this));
    menu->addChild(itemChange);
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}


#pragma mark - set, get, sort, add
void PopupAdsWatch::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupAdsWatch::onClickClose(Ref* sender)
{
    hide();
}

void PopupAdsWatch::onClickAds(cocos2d::Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult();
        _callbackResult = nullptr;
    }
    
    hide();
}

