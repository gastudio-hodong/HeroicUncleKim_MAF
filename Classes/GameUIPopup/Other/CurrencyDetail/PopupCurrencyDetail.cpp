//
//  PopupCurrencyDetail.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCurrencyDetail.h"

#include "Common/Observer/IMove.h"

#include "GameScene/Base/BaseGameScene.h"

#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"
#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicDefault.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupCurrencyDetail* PopupCurrencyDetail::create(E_ITEMS eItems, E_TYPE eType, int nTypeParams)
{
    PopupCurrencyDetail* pRet = new(std::nothrow) PopupCurrencyDetail();
    if ( pRet && pRet->init(eItems, eType, nTypeParams) )
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

PopupCurrencyDetail::PopupCurrencyDetail(void) :
_eItems(E_ITEMS::NONE),
_eType(E_TYPE::NONE),

_nTypeParams(0),
_nAdsTimeHide(6),
_nAdsTimeHideMax(6),

_sizeContainer(Size::ZERO),

_layerContainerContentsGet(nullptr),
_layerContainerContentsUse(nullptr),
_layerContainerAds(nullptr),

_labelAdsTime(nullptr),
_callbackTransferShop(nullptr)
{
    
}

PopupCurrencyDetail::~PopupCurrencyDetail(void)
{
    
}

bool PopupCurrencyDetail::init(E_ITEMS eItems, E_TYPE eType, int nTypeParams)
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    //
    _eItems = eItems;
    _eType = eType;
    _nTypeParams = nTypeParams;
    

    // init
    initVar();
    initUi();
    
    // touch
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        
        cocos2d::Rect rect = _spriteContainer->getBoundingBox();
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())) == false)
        {
            hide();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}

void PopupCurrencyDetail::onEnter()
{
    PopupBase::onEnter();
    
    // log
    MafAnalyticsManager::LogEvent(kAnalEventCurrencyDetailShow, kRepeatTrue);
}
void PopupCurrencyDetail::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->isAdventurePopupShow() && AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_TOAST);
}
#pragma mark - init
void PopupCurrencyDetail::initVar()
{
    if ( _eItems == E_ITEMS::GOLD )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_QUEST);
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_WEAPON);
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_TOWN);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_QUEST);
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_MYHOME);
    }
    else if ( _eItems == E_ITEMS::KEY )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_ARTIFACT);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL);
        _listGetArea.push_back(E_AREA_MOVE::AREA_PRISON);
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_REVIVE);
    }
    else if ( _eItems == E_ITEMS::POINT )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_ARTIFACT);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL);
        _listGetArea.push_back(E_AREA_MOVE::AREA_PRISON);
        _listGetArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
    }
    else if ( _eItems == E_ITEMS::GEM )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_ARTIFACT);
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_REVIVE);
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_PET);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL);
        _listGetArea.push_back(E_AREA_MOVE::AREA_DEVIL);
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_MYHOME);
    }
    else if ( _eItems == E_ITEMS::COSTUME_COIN )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_COSTUME);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_PRISON);
        _listGetArea.push_back(E_AREA_MOVE::AREA_COSTUME);
    }
    else if ( _eItems == E_ITEMS::MATERIAL )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_COSTUME);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
    }
    else if ( _eItems == E_ITEMS::HONOR )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_COSTUME);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
    }
    else if ( _eItems >= E_ITEMS::MINE_STONE_IRON && _eItems <= E_ITEMS::MINE_AMETHYST )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_WEAPON);
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_PET);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_MINE);
        _listGetArea.push_back(E_AREA_MOVE::AREA_ADVENTURE);
    }
    else if ( _eItems == E_ITEMS::CUBE )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_ARTIFACT);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_REVIVE);
    }
    else if ( _eItems == E_ITEMS::EXPEDI_POINT )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_DRAW);
        
    }
    else if ( _eItems == E_ITEMS::FRIEND_POINT )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_DRAW);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_FRIEND);
    }
    else if ( _eItems == E_ITEMS::D_GOLD )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_DEVIL_QUEST);
        _listUseArea.push_back(E_AREA_MOVE::AREA_DEVIL_WEAPON);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_DEVIL_QUEST);
    }
    else if ( _eItems == E_ITEMS::D_KEY )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_DEVIL_SLATE);
        _listUseArea.push_back(E_AREA_MOVE::AREA_DEVIL_WEAPON);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_DEVIL_REVIVE);
    }
    else if ( _eItems == E_ITEMS::SPIRIT_STON )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
    }
    else if ( _eItems == E_ITEMS::NEW_RAID_DRAGON_BALL )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
        _listUseArea.push_back(E_AREA_MOVE::AREA_DEFENSE);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NEW_RAID);
    }
    else if ( _eItems == E_ITEMS::TICKET )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_PRISON);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_PRISON);
    }
    else if ( _eItems == E_ITEMS::ADVENTURE_TICKET ||  _eItems == E_ITEMS::ADVENTURE_CORE)
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_ADVENTURE);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_ADVENTURE);
    }
    else if ( _eItems == E_ITEMS::FROST_PRISON_POINT )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_PRISON_FROST);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_PRISON_FROST);
    }
    else if ( _eItems == E_ITEMS::MYTHRIL )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_NORMAL_WEAPON);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_NORMAL_MINE);
    }
    else if ( _eItems == E_ITEMS::DEFENSE_COIN )
    {
        _listUseArea.push_back(E_AREA_MOVE::AREA_DEFENSE);
        
        _listGetArea.push_back(E_AREA_MOVE::AREA_DEFENSE);
    }
    
    //
    _sizeContainer = Size(506, 295);
    if ( _listGetArea.size() > 1 )
    {
        _sizeContainer.height += (_listGetArea.size() - 1) * 70;
    }
}

void PopupCurrencyDetail::initUi()
{
    if ( _eType == E_TYPE::QUEST)
    {
        _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png", Rect::ZERO, Rect(0,0,20,20));
        _spriteContainer->setContentSize(Size(504, 300));
        _spriteContainer->setPosition(getContentSize().width / 2, getContentSize().height / 2);
        addChild(_spriteContainer);
        
        _layerContainerAds = LayerColor::create(Color4B::BLUE);
        _layerContainerAds->setContentSize( Size(504, 300) );
        _layerContainerAds->setIgnoreAnchorPointForPosition(false);
        _layerContainerAds->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerAds->setPosition(_spriteContainer->getContentSize().width / 2, 0);
        _spriteContainer->addChild(_layerContainerAds);
        
        //
        uiQuestAds();
    }
    else
    {
        _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_2.png", Rect::ZERO, Rect(35,170,10,10));
        _spriteContainer->setContentSize(_sizeContainer);
        _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
        addChild(_spriteContainer);
        
        
        //
        auto objItems = ItemsManager::getInstance()->getInfoItems((int)_eItems);
        objItems->setTypeParams(_nTypeParams);

        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        _spriteContainer->addChild(menu);

        // item close
        auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupCurrencyDetail::onClickClose, this));
        itemClose->setPosition( _spriteContainer->getContentSize().width - itemClose->getContentSize().width / 2 - 25, _spriteContainer->getContentSize().height - 27.5);
        itemClose->setTouchRect( itemClose->getContentSize() * 2 );
        menu->addChild(itemClose);

        //
        auto spriteIconBG = Sprite::create("Assets/ui_common/base_box_2.png");
        spriteIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteIconBG->setPosition(25, _spriteContainer->getContentSize().height - 77);
        _spriteContainer->addChild(spriteIconBG);
        {// 재화 아이콘 출력
            auto spriteIcon = Sprite::create(objItems->getPath());
            spriteIcon->setScale(2);
            spriteIcon->setPosition(spriteIconBG->getContentSize().width / 2, spriteIconBG->getContentSize().height / 2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteIconBG->addChild(spriteIcon);
        }

        // label title
        auto labelTitle = Label::createWithTTF(objItems->getText(), GAME_FONT, 28);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTitle->setPosition(155, _spriteContainer->getContentSize().height - 27.5);
        labelTitle->setColor(COLOR_TITLE);
        _spriteContainer->addChild(labelTitle);

        _layerContainerContentsUse = Layer::create();
        _layerContainerContentsUse->setContentSize( Size(290, 80) );
        _layerContainerContentsUse->setIgnoreAnchorPointForPosition(false);
        _layerContainerContentsUse->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        _layerContainerContentsUse->setPosition(155, _spriteContainer->getContentSize().height - 55);
        _spriteContainer->addChild(_layerContainerContentsUse);
        
        _layerContainerContentsGet = Layer::create();
        _layerContainerContentsGet->setContentSize( Size(_sizeContainer.width - 50, _sizeContainer.height - 153 - 50) );
        _layerContainerContentsGet->setIgnoreAnchorPointForPosition(false);
        _layerContainerContentsGet->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerContentsGet->setPosition(_spriteContainer->getContentSize().width / 2, 25);
        _spriteContainer->addChild(_layerContainerContentsGet);
        
        //
        uiDefaultContentsUse();
        uiDefaultContentsGet();
        
        // 상점 이동
        std::string strPath = "Assets/ui/purchase/btn_purchase_shortcuts1.png";
        
        if(_eItems == E_ITEMS::NEW_RAID_DRAGON_BALL)
        {
            strPath = "Assets/ui/purchase/btn_purchase_shortcuts2.png";
        }
        
        auto itemShop = MafNode::MafMenuItemSprite::create(Sprite::create(strPath), nullptr, CC_CALLBACK_1(PopupCurrencyDetail::onClickShop, this));
        itemShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        itemShop->setPosition( _spriteContainer->getContentSize().width / 2, - 10);
        menu->addChild(itemShop);
        
        if ( _eType == E_TYPE::NONE )
        {
            uiDefaultWarning();
        }
    }
    
    
    
    //
//    if ( _eType == E_TYPE::QUEST)
//    {
//        int nTime = UserInfoManager::getInstance()->getAdsTime(E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL);
//        nTime = 0;
//        if ( nTime > 0 )
//        {
//            uiWarning();
//        }
//
//        uiAds();
//    }
//    else if ( _eType != E_TYPE::CURRENCY)
//    {
//        uiWarning();
//    }
}


#pragma mark - ui : default
void PopupCurrencyDetail::uiDefaultContentsUse()
{
    //
    _layerContainerContentsUse->removeAllChildren();
    
    // label area
    auto labelArea = Label::createWithTTF(GAME_TEXT("t_ui_currency_detail_msg_2"), GAME_FONT, 18);
    labelArea->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    labelArea->setPosition(0, _layerContainerContentsUse->getContentSize().height);
    _layerContainerContentsUse->addChild(labelArea);
    
    
    for ( int i = 0; i < _listUseArea.size(); i++ )
    {
        std::string strIconPath = "";
        E_AREA_MOVE eArea = _listUseArea.at(i);
        
        if ( eArea == E_AREA_MOVE::AREA_NORMAL )                     strIconPath = "menu_btn_normal.png";
        else if ( eArea == E_AREA_MOVE::AREA_DEVIL )                 strIconPath = "menu_btn_devil.png";
        else if ( eArea == E_AREA_MOVE::AREA_PRISON )                strIconPath = "menu_btn_prison.png";
        else if ( eArea == E_AREA_MOVE::AREA_COSTUME )               strIconPath = "menu_btn_costume.png";
        else if ( eArea == E_AREA_MOVE::AREA_FRIEND )                strIconPath = "menu_btn_friend.png";
        else if ( eArea == E_AREA_MOVE::AREA_DRAW )                  strIconPath = "menu_btn_draw.png";
        else if ( eArea == E_AREA_MOVE::AREA_ADVENTURE )             strIconPath = "menu_btn_adventure.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_QUEST )          strIconPath = "menu_btn_quest.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_ARTIFACT )       strIconPath = "menu_btn_treasure.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_WEAPON )         strIconPath = "menu_btn_weapon.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_PET )            strIconPath = "menu_btn_pet.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_MYHOME )         strIconPath = "menu_btn_home.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_TOWN )           strIconPath = "menu_btn_tower.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_REVIVE )         strIconPath = "menu_btn_relic.png";
        else if ( eArea == E_AREA_MOVE::AREA_NORMAL_MINE )           strIconPath = "menu_btn_mine.png";
        else if ( eArea == E_AREA_MOVE::AREA_DEVIL_QUEST )           strIconPath = "menu_btn_quest.png";
        else if ( eArea == E_AREA_MOVE::AREA_DEVIL_WEAPON )          strIconPath = "menu_btn_weapon.png";
        else if ( eArea == E_AREA_MOVE::AREA_DEVIL_SLATE )           strIconPath = "menu_btn_slab.png";
        else if ( eArea == E_AREA_MOVE::AREA_DEVIL_REVIVE )          strIconPath = "menu_btn_relic.png";
        else if ( eArea == E_AREA_MOVE::AREA_NEW_RAID )              strIconPath = "menu_btn_raid_3.png";
        else if ( eArea == E_AREA_MOVE::AREA_DEFENSE )               strIconPath = "menu_btn_defense.png";
        
        if ( strIconPath.length() == 0 )
        {
            continue;
        }
        strIconPath = std::string("Assets/ui/menu_bottom/").append(strIconPath);
        
        double posX = 50 * i + 5 * i;
        double posY = 0;

        auto spriteIconBG = Sprite::create("Assets/ui_common/box_item.png");
        spriteIconBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        spriteIconBG->setPosition(posX, posY);
        _layerContainerContentsUse->addChild(spriteIconBG);

        auto spriteAreaIcon = Sprite::create(strIconPath);
        spriteAreaIcon->setPosition(spriteIconBG->getContentSize().width / 2, spriteIconBG->getContentSize().height / 2 - 5);
        spriteAreaIcon->setScale(0.8);
        spriteIconBG->addChild(spriteAreaIcon);
    }

}

void PopupCurrencyDetail::uiDefaultContentsGet()
{
    //
    _layerContainerContentsGet->removeAllChildren();
    
    auto spriteContentsBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png", Rect::ZERO, Rect(0,0,20,20));
    spriteContentsBG->setContentSize(_layerContainerContentsGet->getContentSize());
    spriteContentsBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    spriteContentsBG->setPosition(0, 0);
    _layerContainerContentsGet->addChild(spriteContentsBG);
    
    auto spriteTitleBG = Sprite::create("Assets/ui/pet/pet_guide_2.png");
    spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    spriteTitleBG->setPosition(0, _layerContainerContentsGet->getContentSize().height);
    _layerContainerContentsGet->addChild(spriteTitleBG);
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_currency_detail_msg_1"), GAME_FONT, 18);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTitle->setPosition(10, spriteTitleBG->getContentSize().height / 2);
        spriteTitleBG->addChild(labelTitle);
    }
    
    for ( int i = 0; i < _listGetArea.size(); i++ )
    {
        std::string strIconPath = "";
        std::string strTextKey = "";
        E_AREA_MOVE eArea = _listGetArea.at(i);
        
        switch (eArea) {
            case E_AREA_MOVE::AREA_NORMAL:
                strIconPath = "menu_btn_normal.png";
                strTextKey = "t_ui_dungeon_normal";
                break;
            case E_AREA_MOVE::AREA_DEVIL:
                strIconPath = "menu_btn_devil.png";
                strTextKey = "t_ui_dungeon_devil";
                break;
            case E_AREA_MOVE::AREA_PRISON:
                strIconPath = "menu_btn_prison.png";
                strTextKey = "t_ui_prison";
                break;
            case E_AREA_MOVE::AREA_COSTUME:
                strIconPath = "menu_btn_costume.png";
                strTextKey = "t_ui_costume";
                break;
            case E_AREA_MOVE::AREA_FRIEND:
                strIconPath = "menu_btn_friend.png";
                strTextKey = "t_ui_menu_friend";
                break;
            case E_AREA_MOVE::AREA_DRAW:
                strIconPath = "menu_btn_draw.png";
                strTextKey = "t_ui_draw";
                break;
            case E_AREA_MOVE::AREA_ADVENTURE:
                strIconPath = "menu_btn_adventure.png";
                strTextKey = "t_ui_advt_info_title_1";
                break;
            case E_AREA_MOVE::AREA_NEW_RAID:
                strIconPath = "menu_btn_raid_3.png";
                strTextKey = "t_ui_new_raid_title";
                break;
            case E_AREA_MOVE::AREA_NORMAL_QUEST:
                strIconPath = "menu_btn_quest.png";
                strTextKey = "t_ui_quest";
                break;
            case E_AREA_MOVE::AREA_NORMAL_ARTIFACT:
                strIconPath = "menu_btn_treasure.png";
                strTextKey = "t_ui_artifact";
                break;
            case E_AREA_MOVE::AREA_NORMAL_WEAPON:
                strIconPath = "menu_btn_weapon.png";
                strTextKey = "t_ui_weapon";
                break;
            case E_AREA_MOVE::AREA_NORMAL_PET:
                strIconPath = "menu_btn_pet.png";
                strTextKey = "t_ui_pet";
                break;
            case E_AREA_MOVE::AREA_NORMAL_MYHOME:
                strIconPath = "menu_btn_home.png";
                strTextKey = "t_ui_myhome_name";
                break;
            case E_AREA_MOVE::AREA_NORMAL_TOWN:
                strIconPath = "menu_btn_tower.png";
                strTextKey = "t_ui_town";
                break;
            case E_AREA_MOVE::AREA_NORMAL_REVIVE:
                strIconPath = "menu_btn_relic.png";
                strTextKey = "t_ui_revival_1";
                break;
            case E_AREA_MOVE::AREA_NORMAL_MINE:
                strIconPath = "menu_btn_mine.png";
                strTextKey = "t_ui_mine";
                break;
            case E_AREA_MOVE::AREA_DEVIL_QUEST:
                strIconPath = "menu_btn_quest.png";
                strTextKey = "t_ui_quest";
                break;
            case E_AREA_MOVE::AREA_DEVIL_WEAPON:
                strIconPath = "menu_btn_weapon.png";
                strTextKey = "t_ui_equip";
                break;
            case E_AREA_MOVE::AREA_DEVIL_SLATE:
                strIconPath = "menu_btn_slab.png";
                strTextKey = "t_ui_slate";
                break;
            case E_AREA_MOVE::AREA_DEVIL_REVIVE:
                strIconPath = "menu_btn_relic.png";
                strTextKey = "t_ui_revival_1";
                break;
            case E_AREA_MOVE::AREA_PRISON_FROST:
                strIconPath = "menu_btn_frost_prison.png";
                strTextKey = "t_ui_frost_prison_title";
                break;
            case E_AREA_MOVE::AREA_DEFENSE:
                strIconPath = "menu_btn_defense.png";
                strTextKey = "t_ui_defense_title_1";
                break;
                
            default:
                break;
        }
            
        if ( strIconPath.length() == 0 )
        {
            continue;
        }
        strIconPath = std::string("Assets/ui/menu_bottom/").append(strIconPath);
        
        
        double posX = 0;
        double posY = _layerContainerContentsGet->getContentSize().height - 25 - 70 * i;
        
        auto layerCellBG = Layer::create();
        layerCellBG->setContentSize( Size(_layerContainerContentsGet->getContentSize().width, 70) );
        layerCellBG->setIgnoreAnchorPointForPosition(false);
        layerCellBG->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
        layerCellBG->setPosition(posX, posY);
        _layerContainerContentsGet->addChild(layerCellBG);
        
        auto spriteIconBG = Sprite::create("Assets/ui/menu_bottom/menu_btn_bg_off_mini.png");
        spriteIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteIconBG->setPosition(10, layerCellBG->getContentSize().height / 2);
        layerCellBG->addChild(spriteIconBG);
        {
            auto spriteIcon = Sprite::create(strIconPath);
            spriteIcon->setPosition(spriteIconBG->getContentSize().width / 2, spriteIconBG->getContentSize().height / 2 - 5);
            spriteIcon->setScale(0.8);
            spriteIconBG->addChild(spriteIcon);
        }
        
        auto labelText = Label::createWithTTF(GAME_TEXT(strTextKey), GAME_FONT, 24);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(80, layerCellBG->getContentSize().height / 2);
        layerCellBG->addChild(labelText);
        
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        layerCellBG->addChild(menu);
        
        auto itemMove = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_shortcuts_2.png"), nullptr, CC_CALLBACK_1(PopupCurrencyDetail::onClickMove, this));
        itemMove->setPosition(layerCellBG->getContentSize().width - 50, layerCellBG->getContentSize().height / 2);
        itemMove->setTag((int)eArea);
        menu->addChild(itemMove);
    }
}

void PopupCurrencyDetail::uiDefaultWarning()
{
    auto spriteWarningBG = Sprite::create("Assets/ui_common/base_text_1_4.png");
    spriteWarningBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteWarningBG->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height + 10);
    spriteWarningBG->setCascadeOpacityEnabled(true);
    spriteWarningBG->setOpacity(0);
    _spriteContainer->addChild(spriteWarningBG);
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_error_31"), GAME_FONT, 24);
    labelText->setPosition(spriteWarningBG->getContentSize().width / 2, spriteWarningBG->getContentSize().height / 2);
    spriteWarningBG->addChild(labelText);
    
    auto action1 = FadeIn::create(0.5);
    auto action2 = DelayTime::create(2.0);
    auto action3 = FadeOut::create(0.5);
    auto seq = Sequence::create(action1, action2, action3, NULL);
    spriteWarningBG->runAction(seq);
}

#pragma mark - ui : quest
void PopupCurrencyDetail::uiQuestAds()
{
    _layerContainerAds->removeAllChildren();
    
    {
        auto spriteBG = Sprite::create("Assets/ui/ads/bg_ad_2_1.png");
        spriteBG->setPosition(_layerContainerAds->getContentSize().width / 2, _layerContainerAds->getContentSize().height / 2);
        spriteBG->getTexture()->setTexParameters(texParams);
        spriteBG->setScale(4);
        _layerContainerAds->addChild(spriteBG);

        auto labelMessage = Label::createWithTTF(GAME_TEXT("t_ui_error_14"), GAME_FONT, 20);
        labelMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelMessage->setPosition(_layerContainerAds->getContentSize().width / 2, 135);
        _layerContainerAds->addChild(labelMessage);

        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(_layerContainerAds->getContentSize().width / 2, 55);
        _layerContainerAds->addChild(menu);

        auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_ok.png"),nullptr, CC_CALLBACK_1(PopupCurrencyDetail::onClickClose, this));
        menu->addChild(itemClose);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 24);
            labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
            itemClose->addChild(labelText);
        }

        auto itemAds = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_ok.png"), nullptr, CC_CALLBACK_1(PopupCurrencyDetail::onClickQuestAds, this));
        menu->addChild(itemAds);
        {
            auto spriteIcon = Sprite::create("Assets/icon/icon_ad_1_1.png");
            spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            spriteIcon->setPosition(itemAds->getContentSize().width / 2, itemAds->getContentSize().height / 2);
            itemAds->addChild(spriteIcon);

            _labelAdsTime = Label::createWithTTF(MafUtils::format("%d", _nAdsTimeHide), GAME_FONT, 24);
            _labelAdsTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _labelAdsTime->setPosition(itemAds->getContentSize().width / 2 + 10, itemAds->getContentSize().height / 2);
            itemAds->addChild(_labelAdsTime);

            auto action1 = DelayTime::create(1.0);
            auto action2 = CallFuncN::create([=](Node *sender){

                _nAdsTimeHide -= 1;

                ((Label*)sender)->setString(MafUtils::format("%d", _nAdsTimeHide));
                if ( _nAdsTimeHide <= 0 )
                {
                    sender->stopAllActions();

                    if ( CashManager::getInstance()->isAdsfreeTime() == true )
                    {
                        callbackAdsInterstitial(ADS_RESULT::SUCCESS);
                    }
                    else
                    {
                        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupCurrencyDetail::callbackAdsInterstitial, this));
                    }
                    return;
                }
            });
            auto seq = Sequence::create(action1, action2, NULL);
            _labelAdsTime->runAction(RepeatForever::create(seq));
        }

        menu->alignItemsHorizontallyWithPadding(10);
    }
}

#pragma mark - set, get, other
void PopupCurrencyDetail::setCallbackTransferShop(const std::function<void(void)>& callback)
{
    _callbackTransferShop = callback;
}

#pragma mark - click
void PopupCurrencyDetail::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupCurrencyDetail::onClickMove(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto eType = (E_AREA_MOVE)item->getTag();
    
    if(eType == E_AREA_MOVE::AREA_ADVENTURE)
    {
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
        {
            hide();
            return;
        }
    }
    
    //
    hide();
    
    //
    auto sceneGame = UtilsScene::getInstance()->getRunningScene();
    auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
    
    if ( dynamic_cast<BaseGameScene *>(sceneGameLayer) != nullptr )
    {
        dynamic_cast<BaseGameScene *>(sceneGameLayer)->hidePopupAll();
    }
    
    if ( dynamic_cast<IMove *>(sceneGameLayer) != nullptr )
    {
        dynamic_cast<IMove *>(sceneGameLayer)->move(eType);
        
    }
}

void PopupCurrencyDetail::onClickShop(cocos2d::Ref* sender)
{
    //
    
    if(_callbackTransferShop != nullptr)
    {
        _callbackTransferShop();
        _callbackTransferShop = nullptr;
        hide();
        return;
    }
    
    //
    auto sceneGame = UtilsScene::getInstance()->getRunningScene();
    auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
    
    if ( dynamic_cast<IMove *>(sceneGameLayer) != nullptr )
    {
        if(_eItems == E_ITEMS::NEW_RAID_DRAGON_BALL)
        {
            dynamic_cast<IMove *>(sceneGameLayer)->move(E_AREA_MOVE::AREA_NEW_RAID_DRAGON_BALL_SHOP);
        }
        else if(_eItems == E_ITEMS::ADVENTURE_CORE ||
                _eItems == E_ITEMS::ADVENTURE_TICKET)
        {
            dynamic_cast<IMove *>(sceneGameLayer)->move(E_AREA_MOVE::AREA_ADVENTURE_CORE_SHOP);
        }
        else
        {
            dynamic_cast<IMove *>(sceneGameLayer)->move(E_AREA_MOVE::AREA_SHOP);
        }
    }
    else
    {
        if(_eItems == E_ITEMS::NEW_RAID_DRAGON_BALL)
        {
            auto popup = PopupRaidShopDefault::create(PopupRaidShopDefault::E_TYPE::PURCHASE);
            popup->show();
        }
        else if(_eItems == E_ITEMS::ADVENTURE_CORE ||
                _eItems == E_ITEMS::ADVENTURE_TICKET)
        {
            if(AdventureManager::getInstance()->getTutorialStep() > 6)
            {
                auto popup = PopupAdventureRelicDefault::create(PopupAdventureRelicDefault::E_TYPE::SHOP);
                popup->show();
            }
        }
        else
        {
            auto popup = PopupCash::create();
            popup->show();
        }
        
    }
    hide();
}

void PopupCurrencyDetail::onClickQuestAds(cocos2d::Ref* sender)
{
    _nAdsTimeHide = _nAdsTimeHideMax;
    _labelAdsTime->stopAllActions();
    
    //
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAdsInterstitial(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupCurrencyDetail::callbackAdsInterstitial, this));
    }
}

#pragma mark - callback
void PopupCurrencyDetail::callbackAdsInterstitial(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotGoldPop, CashManager::getInstance()->isAdsfreeTime());
        
        //
        auto earnCount = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::ADS_SHOP);
        earnCount = MafUtils::bigMulNum(earnCount, MafUtils::toString(4)); // 광고시청 대기시간 증가로 보상 확장
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_11) )
        {
            earnCount = MafUtils::bigMulNum(earnCount, MafUtils::toString(11)); //1000%증가
        }
        
        ItemsManager::getInstance()->addItem(E_ITEMS::GOLD, 0, earnCount);
        
        //
        int count = ParameterManager::getInstance()->getParameterInteger("ads_buff_count");
        UserInfoManager::getInstance()->setAdsTime(E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL, count);
        
        //
        hide();
    }
    else
    {
        if (result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupCurrencyDetail::callbackAdsInterstitial, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotGoldPop);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            hide();
        }
    }
}
