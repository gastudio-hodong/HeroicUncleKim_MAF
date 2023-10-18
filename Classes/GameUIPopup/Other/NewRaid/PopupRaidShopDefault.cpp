//
//  PopupRaidShopDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/30.
//

#include "PopupRaidShopDefault.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/NewRaid/Layer/UILayerSpiritSummon.h"
#include "GameUIPopup/Other/NewRaid/Layer/UILayerRaidPurchase.h"
#include "GameUIPopup/Other/NewRaid/Layer/UILayerRaidTrading.h"

#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

PopupRaidShopDefault* PopupRaidShopDefault::create(PopupRaidShopDefault::E_TYPE eType)
{
    PopupRaidShopDefault* pRet = new(std::nothrow) PopupRaidShopDefault();
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

PopupRaidShopDefault::PopupRaidShopDefault(void):
_isCloseCondition(false)
,_eType(PopupRaidShopDefault::E_TYPE::SUMMON)
,_lyTopUI(nullptr)
,_lyContentArea(nullptr)
,_lyContainerNow(nullptr)
,_lyContainerSummon(nullptr)
,_lyContainerPurchase(nullptr)
,_lyContainerTrading(nullptr)
{
}

PopupRaidShopDefault::~PopupRaidShopDefault(void)
{
}

#pragma mark -init
bool PopupRaidShopDefault::init(PopupRaidShopDefault::E_TYPE eType)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _eType = eType;
    initVar();
    initUI();
    
    
    
    return true;
}

void PopupRaidShopDefault::initVar()
{
    _spriteContainer->setContentSize(Size(730, 1268));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupRaidShopDefault::initUI()
{
    // title
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_raid_bt_shop"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupRaidShopDefault::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 150));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyContentArea = LayerColor::create();
    _lyContentArea->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height));
    _lyContentArea->setIgnoreAnchorPointForPosition(false);
    _lyContentArea->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyContentArea->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyContentArea);
    
    uiTop();
    uiMiddle();
}

#pragma mark -init
void PopupRaidShopDefault::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::NEW_RAID_DRAGON_BALL);
    list.push_back(E_ITEMS::SPIRIT_STON);

    auto uiCurrency = UICurrency::create(list, Size(_lyTopUI->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_lyTopUI->getContentSize().width / 2, _lyTopUI->getContentSize().height - 5));
    _lyTopUI->addChild(uiCurrency, E_ZORER::UI+1);
    
    std::vector<PopupRaidShopDefault::E_TYPE> listTab = {
        PopupRaidShopDefault::E_TYPE::SUMMON, PopupRaidShopDefault::E_TYPE::PURCHASE,
    };
    
    auto objTradingSetting = NewRaidManager::getInstance()->getRaidSetting("trading_market_switch");
    
    if(objTradingSetting != nullptr)
    {
        if(objTradingSetting->getValueToBool() == true)
            listTab.push_back(PopupRaidShopDefault::E_TYPE::TRADING);
    }
    
    int size = (int)listTab.size();
    
    float width = _lyTopUI->getContentSize().width / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        std::string strIconPath = "Assets/ui/raid/raid_shop/raid_shop_category_%d_2.png";
        if(obj == _eType)
        {
            strPath = "Assets/ui_common/btn_tap_2_on.png";
            strIconPath = "Assets/ui/raid/raid_shop/raid_shop_category_%d_1.png";
        }
        std::string strText = "";
        switch (obj) {
            case PopupRaidShopDefault::E_TYPE::SUMMON: strText = GAME_TEXT("t_ui_raidshop_spiritbox_bt"); break;
            case PopupRaidShopDefault::E_TYPE::PURCHASE: strText = GAME_TEXT("t_ui_raidshop_purchase_bt"); break;
            case PopupRaidShopDefault::E_TYPE::TRADING: strText = GAME_TEXT("t_ui_raidshop_exchange_bt"); break;
            default:  break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 90));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnTab->setPosition(Vec2(posX, 0));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupRaidShopDefault::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        _lyTopUI->addChild(btnTab);
        {
            auto sprIcon = Sprite::create(MafUtils::format(strIconPath.c_str(), (int)obj));
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIcon->setPosition(Vec2(btnTab->getContentSize()/2));
            btnTab->addChild(sprIcon);
            
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbText->setPosition(btnTab->getContentSize().width/2, btnTab->getContentSize().height/2 - 5);
            lbText->setDimensions(btnTab->getContentSize().width - 10, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbText);
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        posX += width + 10;
    }
}

void PopupRaidShopDefault::uiMiddle()
{
    _lyContentArea->removeAllChildren();
    
    uiLoadLayer();
}

void PopupRaidShopDefault::uiReload()
{
    uiTop();
    uiLoadLayer();
}

void PopupRaidShopDefault::uiLoadLayer()
{
    if(_lyContainerNow != nullptr)
        _lyContainerNow->setVisible(false);
    
    switch(_eType)
    {
        case PopupRaidShopDefault::E_TYPE::SUMMON:
        {
            if(_lyContainerSummon == nullptr)
            {
                auto layer = UILayerSpiritSummon::create(_lyContentArea->getContentSize());
                layer->setCallbackTransferShop(CC_CALLBACK_1(PopupRaidShopDefault::callbackSetTab, this));
                _lyContentArea->addChild(layer);
                
                _lyContainerSummon = layer;
            }
            _lyContainerNow =  _lyContainerSummon;
            
            ((UILayerSpiritSummon*) _lyContainerNow)->refreshUI();
        }break;
        case PopupRaidShopDefault::E_TYPE::PURCHASE:
        {
            if(_lyContainerPurchase == nullptr)
            {
                auto layer = UILayerRaidPurchase::create(_lyContentArea->getContentSize());
                _lyContentArea->addChild(layer);
                
                _lyContainerPurchase = layer;
            }
            _lyContainerNow = _lyContainerPurchase;
            
            ((UILayerRaidPurchase*) _lyContainerNow)->refreshUI();
        }break;
        case PopupRaidShopDefault::E_TYPE::TRADING:
        {
            if(_lyContainerTrading == nullptr)
            {
                auto layer = UILayerRaidTrading::create(_lyContentArea->getContentSize());
                _lyContentArea->addChild(layer);
                
                _lyContainerTrading = layer;
            }
            _lyContainerNow = _lyContainerTrading;
            
            ((UILayerRaidTrading*) _lyContainerNow)->requestInfo();
        }break;
    }
    
    _lyContainerNow->setVisible(true);
}

#pragma mark -utils
void PopupRaidShopDefault::onClickClose(Ref* sender)
{
    hide();
}

void PopupRaidShopDefault::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    _eType = (PopupRaidShopDefault::E_TYPE)node->getTag();
    
    uiReload();
}

void PopupRaidShopDefault::setCloseCondition(bool value)
{
    _isCloseCondition = value;
    
    if(_lyContainerSummon != nullptr)
    {
        ((UILayerSpiritSummon*) _lyContainerSummon)->setCloseCondition(_isCloseCondition);
        ((UILayerSpiritSummon*) _lyContainerSummon)->setCallbackHide(CC_CALLBACK_0(PopupRaidShopDefault::callbackHide, this));
    }
}

void PopupRaidShopDefault::callbackHide()
{
    hide();
}

void PopupRaidShopDefault::callbackSetTab(PopupRaidShopDefault::E_TYPE value)
{
    _eType = value;
    
    uiReload();
}
