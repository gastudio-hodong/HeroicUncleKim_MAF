//
//  PopupDefenseShopDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/11.
//

#include "PopupDefenseShopDefault.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Defense/Shop/Layer/UILayerDefenseSummon.h"
#include "GameUIPopup/Other/Defense/Shop/Layer/UILayerDefensePurchase.h"

#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

PopupDefenseShopDefault* PopupDefenseShopDefault::create(PopupDefenseShopDefault::E_TYPE eType)
{
    PopupDefenseShopDefault* pRet = new(std::nothrow) PopupDefenseShopDefault();
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

PopupDefenseShopDefault::PopupDefenseShopDefault(void):
_isCloseCondition(false)
,_eType(PopupDefenseShopDefault::E_TYPE::SUMMON)
,_lyTopUI(nullptr)
,_lyContentArea(nullptr)
,_lyContainerNow(nullptr)
,_lyContainerSummon(nullptr)
,_lyContainerPurchase(nullptr)
{
}

PopupDefenseShopDefault::~PopupDefenseShopDefault(void)
{
}

#pragma mark -init
bool PopupDefenseShopDefault::init(PopupDefenseShopDefault::E_TYPE eType)
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

void PopupDefenseShopDefault::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1335));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupDefenseShopDefault::initUI()
{
    // title
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_shop_title_1"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDefenseShopDefault::onClickClose, this));
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
void PopupDefenseShopDefault::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::NEW_RAID_DRAGON_BALL);

    auto uiCurrency = UICurrency::create(list, Size(_lyTopUI->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_lyTopUI->getContentSize().width / 2, _lyTopUI->getContentSize().height - 5));
    _lyTopUI->addChild(uiCurrency, E_ZORER::UI+1);
    
    std::vector<PopupDefenseShopDefault::E_TYPE> listTab = {
        PopupDefenseShopDefault::E_TYPE::SUMMON, PopupDefenseShopDefault::E_TYPE::PURCHASE,
    };
    
    int size = (int)listTab.size();
    
    float width = _lyTopUI->getContentSize().width / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        std::string strIconPath = "";
        if(obj == _eType)
        {
            strPath = "Assets/ui_common/btn_tap_2_on.png";
        }
        std::string strText = "";
        switch (obj) {
            case PopupDefenseShopDefault::E_TYPE::SUMMON:
            {
                strText = GAME_TEXT("t_ui_defense_shop_button_1");
                
                strIconPath = "Assets/ui/raid/raid_shop/raid_shop_defense_1_2.png";
                
                if(obj == _eType)
                    strIconPath = "Assets/ui/raid/raid_shop/raid_shop_defense_1_1.png";
            }break;
            case PopupDefenseShopDefault::E_TYPE::PURCHASE:
            {
                strText = GAME_TEXT("t_ui_raidshop_purchase_bt");
                
                strIconPath = "Assets/ui/raid/raid_shop/raid_shop_category_2_2.png";
                
                if(obj == _eType)
                    strIconPath = "Assets/ui/raid/raid_shop/raid_shop_category_2_1.png";
            }break;
            default:  break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 90));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnTab->setPosition(Vec2(posX, 0));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupDefenseShopDefault::onClickTab, this));
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

void PopupDefenseShopDefault::uiMiddle()
{
    _lyContentArea->removeAllChildren();
    
    uiLoadLayer();
}

void PopupDefenseShopDefault::uiReload()
{
    uiTop();
    uiLoadLayer();
}

void PopupDefenseShopDefault::uiLoadLayer()
{
    if(_lyContainerNow != nullptr)
        _lyContainerNow->setVisible(false);
    
    switch(_eType)
    {
        case PopupDefenseShopDefault::E_TYPE::SUMMON:
        {
            if(_lyContainerSummon == nullptr)
            {
                auto layer = UILayerDefenseSummon::create(_lyContentArea->getContentSize());
                layer->_onTrensferShop += new MafFunction<void(void)>([=](){
                    callbackSetTab(PopupDefenseShopDefault::E_TYPE::PURCHASE);
                }, this);
                layer->_onHide += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseShopDefault::callbackHide, this), this);
                _lyContentArea->addChild(layer);

                _lyContainerSummon = layer;
            }
            _lyContainerNow =  _lyContainerSummon;

            ((UILayerDefenseSummon*) _lyContainerNow)->refreshUI();
        }break;
        case PopupDefenseShopDefault::E_TYPE::PURCHASE:
        {
            if(_lyContainerPurchase == nullptr)
            {
                auto layer = UILayerDefensePurchase::create(_lyContentArea->getContentSize());
                _lyContentArea->addChild(layer);

                _lyContainerPurchase = layer;
            }
            _lyContainerNow = _lyContainerPurchase;

            ((UILayerDefensePurchase*) _lyContainerNow)->refreshUI();
        }break;
    }
    
    _lyContainerNow->setVisible(true);
}

#pragma mark -utils
void PopupDefenseShopDefault::onClickClose(Ref* sender)
{
    hide();
}

void PopupDefenseShopDefault::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    _eType = (PopupDefenseShopDefault::E_TYPE)node->getTag();
    
    uiReload();
}

void PopupDefenseShopDefault::setCloseCondition(bool value)
{
    _isCloseCondition = value;
//
//    if(_lyContainerSummon != nullptr)
//    {
//        ((UILayerSpiritSummon*) _lyContainerSummon)->setCloseCondition(_isCloseCondition);
//        ((UILayerSpiritSummon*) _lyContainerSummon)->setCallbackHide(CC_CALLBACK_0(PopupDefenseShopDefault::callbackHide, this));
//    }
}

void PopupDefenseShopDefault::callbackHide()
{
    hide();
}

void PopupDefenseShopDefault::callbackSetTab(PopupDefenseShopDefault::E_TYPE value)
{
    _eType = value;
    
    uiReload();
}
