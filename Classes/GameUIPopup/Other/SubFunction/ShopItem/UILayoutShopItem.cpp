//
//  UILayoutShopItem.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutShopItem.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/SubFunction/MyHome/UILayoutShopAds.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/ShopItemManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

UILayoutShopItem* UILayoutShopItem::create()
{
    UILayoutShopItem* pRet = new(std::nothrow) UILayoutShopItem();
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

UILayoutShopItem::UILayoutShopItem():
_ePlace(E_PLACE::EMPTY),
_eShopBuy(E_SHOP::SHOP_NONE),

_uiContainerShopAds(nullptr),
_uiContainerList(nullptr),

_uiList(nullptr)
{
    
}

bool UILayoutShopItem::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutShopItem::onEnter()
{
    ui::Layout::onEnter();
    
    //
    uiContainer();
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        uiShopAds();
        uiList();
    }
    else
    {
        uiList();
    }
}

#pragma mark - init
void UILayoutShopItem::initVar()
{
    _ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _listShop.push_back(E_SHOP::SHOP_D_MOVE_SPD);
        _listShop.push_back(E_SHOP::SHOP_D_ATK_SPD);
        _listShop.push_back(E_SHOP::SHOP_D_ATK_DMG);
        _listShop.push_back(E_SHOP::SHOP_D_QUEST_GOLD);
        _listShop.push_back(E_SHOP::SHOP_D_GOLD_GEM);
    }
    else
    {
        _listShop.push_back(E_SHOP::SHOP_HERO_GEM);
        _listShop.push_back(E_SHOP::SHOP_FEVER);
        _listShop.push_back(E_SHOP::SHOP_ATTACK_BONUS);
        _listShop.push_back(E_SHOP::SHOP_MOVE_BONUS);
        _listShop.push_back(E_SHOP::SHOP_COIN_BONUS);
        _listShop.push_back(E_SHOP::SHOP_GOLD_GEM);
        _listShop.push_back(E_SHOP::SHOP_GOLD_GEM_BIG);
    }
    
}

void UILayoutShopItem::initUI()
{
    // size
    double height = 0;
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        height = _listShop.size() * 135;
        height += 135;
    }
    else
    {
        height = _listShop.size() * 135;
    }
    
    Size size = Size(750, height);
    setContentSize(size);
    setAnchorPoint(Vec2::ZERO);
}

#pragma mark - override DelegateListView
void UILayoutShopItem::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size UILayoutShopItem::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void UILayoutShopItem::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto eType = (E_SHOP)_listShop.at(idx);

    std::string pathIcon = ShopItemManager::getInstance()->getPathIcon(eType);
    std::string name = ShopItemManager::getInstance()->getName(eType);
    std::string desc = ShopItemManager::getInstance()->getDesc(eType);
    
    int buyPrice = ShopItemManager::getInstance()->getBuyPrice(eType);
    
    // ui
    childview->removeAllChildren();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    // bg
    auto uiImgBG = ui::ImageView::create("Assets/ui_common/list_bg_1.png");
    uiImgBG->setContentSize(uiLayout->getContentSize());
    uiImgBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiImgBG->setScale9Enabled(true);
    uiImgBG->setCapInsets(Rect(0,51,150,10));
    uiLayout->addChild(uiImgBG);
   
    //
    auto uiBtnIcon = ui::Button::create("Assets/ui_common/box_quest.png");
    uiBtnIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiBtnIcon->setPosition(Vec2(3, uiLayout->getContentSize().height / 2));
    uiBtnIcon->addClickEventListener(CC_CALLBACK_1(UILayoutShopItem::onClickItemIcon, this));
    uiBtnIcon->setZoomScale(0.05f);
    uiBtnIcon->setTag((int)idx);
    uiLayout->addChild(uiBtnIcon);
    {
        auto uiImgIcon = ui::ImageView::create(pathIcon);
        uiImgIcon->setPosition(Vec2(uiBtnIcon->getContentSize().width / 2, uiBtnIcon->getContentSize().height / 2));
        uiImgIcon->setScale(uiBtnIcon->getContentSize().width / uiImgIcon->getContentSize().width);
        uiBtnIcon->addChild(uiImgIcon);
        
        if ( eType == E_SHOP::SHOP_HERO_GEM )
        {
            auto uiImgOnOff = ui::ImageView::create("Assets/ui/fever/fever_on.png");
            uiImgOnOff->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiImgOnOff->setPosition(Vec2(uiBtnIcon->getContentSize().width - 5, 5));
            uiBtnIcon->addChild(uiImgOnOff);
            if ( UserInfoManager::getInstance()->getBuffVisible(E_BUFF_TYPE::BUFF_HERO_BONUS) == false )
            {
                uiImgOnOff->loadTexture("Assets/ui/fever/fever_off.png");
            }
        }
    }

    // name
    auto uiTextName = ui::CText::create(name, GAME_FONT, 26);
    uiTextName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiTextName->setPosition(Vec2(125, uiLayout->getContentSize().height - 7));
    uiTextName->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTextName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiTextName);
    
    // desc
    auto uiTextDesc = ui::CText::create(desc, GAME_FONT, 22);
    uiTextDesc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiTextDesc->setPosition(Vec2(125, 7));
    uiTextDesc->setTextVerticalAlignment(TextVAlignment::TOP);
    uiTextDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTextDesc->setTextAreaSize(Size(uiLayout->getContentSize().width - 279, 64));
    uiTextDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiLayout->addChild(uiTextDesc);
    {
        auto action1 = CallFuncN::create([=](Node* sender){
        
            std::string desc = ShopItemManager::getInstance()->getDesc(eType);
            
            auto uiTextDesc = (ui::CText*)sender;
            uiTextDesc->setString(desc);
        });
        auto action2 = DelayTime::create(1.0);
        auto seq = Sequence::create(action1, action2, NULL);
        uiTextDesc->runAction(RepeatForever::create(seq));
    }
    
    if ( eType == E_SHOP::SHOP_ATTACK_BONUS )
    {
        if ( PassManager::getInstance()->isBuyPass(E_PASS_TYPE::PASS_DUNGEON) == true )
        {
            uiTextName->setTextColor(Color4B(255, 115, 0, 255));
            uiTextDesc->setTextColor(Color4B(255, 115, 0, 255));
        }            
    }
    
    // buy
    auto uiBtnBuy = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
    uiBtnBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnBuy->setPosition(Vec2(uiLayout->getContentSize().width - 5, uiLayout->getContentSize().height / 2));
    uiBtnBuy->addClickEventListener(CC_CALLBACK_1(UILayoutShopItem::onClickItemBuyAsk, this));
    uiBtnBuy->setZoomScale(0.05f);
    uiBtnBuy->setTag((int)idx);
    uiLayout->addChild(uiBtnBuy);
    {
        auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
        uiImgIcon->setPosition(Vec2(uiBtnBuy->getContentSize().width * 0.175, uiBtnBuy->getContentSize().height * 0.5));
        uiBtnBuy->addChild(uiImgIcon);
        
        auto uiTextPrice = ui::CText::createWithBMFont(MafUtils::toString(buyPrice), GAME_BM_FONT_AL, 30);
        uiTextPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextPrice->setPosition(Vec2(uiBtnBuy->getContentSize().width * 0.350, uiBtnBuy->getContentSize().height * 0.5));
        uiTextPrice->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTextPrice->setColor(COLOR_COIN);
        uiTextPrice->setTextAreaSize(Size(85, 25));
        uiTextPrice->setTextOverflow(Label::Overflow::SHRINK);
        uiBtnBuy->addChild(uiTextPrice);
        
        auto uiTextMax = ui::CText::create("MAX", GAME_FONT, 26);
        uiTextMax->setPosition(Vec2(uiBtnBuy->getContentSize().width / 2, uiBtnBuy->getContentSize().height / 2));
        uiTextMax->setVisible(false);
        uiBtnBuy->addChild(uiTextMax);
        
        if ( eType == E_SHOP::SHOP_HERO_GEM )
        {
            int count = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
            if ( count >= C_MAX_HERO_COUNT )
            {
                uiImgIcon->setVisible(false);
                uiTextPrice->setVisible(false);
                uiTextMax->setVisible(true);
            }
        }
    }
    
    {
        auto action1 = CallFuncN::create([=](Node* sender){
        
            auto uiBtnBuy = (ui::Button*)sender;
            
            int buyPrice = ShopItemManager::getInstance()->getBuyPrice(eType);
            int currency = ItemsMoneyManager::getInstance()->getGem();

            //
            if ( currency < buyPrice )
            {
                uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2_max.png");
            }
            else
            {
                uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2.png");
            }
            
            //
            if ( eType == E_SHOP::SHOP_HERO_GEM )
            {
                int count = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
                if ( count >= C_MAX_HERO_COUNT )
                {
                    uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2_max.png");
                }
            }
            else if ( eType == E_SHOP::SHOP_FEVER )
            {
                int count = UserInfoManager::getInstance()->getItemFeverCount();
                int countMax = ParameterManager::getInstance()->getParameterInteger("item_fever_max_count");
                
                double timeItemFever = UserInfoManager::getInstance()->getItemFeverTime();
                double timeFever = UserInfoManager::getInstance()->getFeverTime();
                
                if ( count >= countMax || timeItemFever > 0  || timeFever > 0 )
                {
                    uiBtnBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2_max.png");
                }
            }
        });
        auto action2 = DelayTime::create(1.0);
        auto seq = Sequence::create(action1, action2, NULL);
        uiBtnBuy->runAction(RepeatForever::create(seq));
    }
}

int64_t UILayoutShopItem::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listShop.size();
   
    return count;
}
 

#pragma mark - ui
void UILayoutShopItem::uiContainer()
{
    Size size = getContentSize();
    
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _uiContainerShopAds = ui::Layout::create();
        _uiContainerShopAds->setContentSize( Size(size.width - 12, 135) );
        _uiContainerShopAds->setIgnoreAnchorPointForPosition(false);
        _uiContainerShopAds->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _uiContainerShopAds->setPosition( Vec2(size.width / 2, size.height) );
        _uiContainerShopAds->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        _uiContainerShopAds->setBackGroundColor(Color3B(29, 41, 51));
        addChild(_uiContainerShopAds);
        
        _uiContainerList = ui::Layout::create();
        _uiContainerList->setContentSize( Size(size.width - 12, size.height - _uiContainerShopAds->getContentSize().height - 25) );
        _uiContainerList->setIgnoreAnchorPointForPosition(false);
        _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _uiContainerList->setPosition( Vec2(size.width / 2, _uiContainerShopAds->getPositionY() - _uiContainerShopAds->getContentSize().height) );
        addChild(_uiContainerList);
    }
    else
    {
        _uiContainerList = ui::Layout::create();
        _uiContainerList->setContentSize( Size(size.width - 12, size.height) );
        _uiContainerList->setIgnoreAnchorPointForPosition(false);
        _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _uiContainerList->setPosition( Vec2(size.width / 2, size.height) );
        addChild(_uiContainerList);
    }
}

void UILayoutShopItem::uiShopAds()
{
    _uiContainerShopAds->removeAllChildren();
    
    auto layout = _uiContainerShopAds;
    
    auto size = layout->getContentSize();
    
    //
    auto uiLayoutShopAds = UILayoutShopAds::create(_ePlace);
    uiLayoutShopAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiLayoutShopAds->setPosition(Vec2(10, size.height / 2));
    layout->addChild(uiLayoutShopAds);
}

void UILayoutShopItem::uiList()
{
    _uiContainerList->removeAllChildren();
    
    auto layout = _uiContainerList;
    
    //
    _uiList = ui::CListView::create(this, Size(layout->getContentSize().width, layout->getContentSize().height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(layout->getContentSize().width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
     
}

#pragma mark - draw
void UILayoutShopItem::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
void UILayoutShopItem::onClickItemIcon(Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_SHOP eType = _listShop.at(uiBtn->getTag());
    E_BUFF_TYPE eTypeBuff = E_BUFF_TYPE::BUFF_HERO_BONUS;
    if ( eType != E_SHOP::SHOP_HERO_GEM )
    {
        return;
    }
    
    //
    bool bVisible = UserInfoManager::getInstance()->getBuffVisible(eTypeBuff);
    UserInfoManager::getInstance()->setBuffVisible(eTypeBuff, !bVisible);
    
    // draw
    drawList();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
}

void UILayoutShopItem::onClickItemBuyAsk(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_BUY_GOLD);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_SHOP eType = _listShop.at(uiBtn->getTag());
    _eShopBuy = eType;
    
    //
    if ( eType == E_SHOP::SHOP_HERO_GEM )
    {
        int count = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
        if ( count >= C_MAX_HERO_COUNT )
        {
            return;
        }
    }
    else if ( eType == E_SHOP::SHOP_FEVER )
    {
        int count = UserInfoManager::getInstance()->getItemFeverCount();
        if ( count >= ParameterManager::getInstance()->getParameterInteger("item_fever_max_count") )
        {
            return;
        }
        else if ( UserInfoManager::getInstance()->getItemFeverTime() > 0 || UserInfoManager::getInstance()->getFeverTime() > 0 )
        {
            return;
        }
    }
    
    //
    if ( eType == E_SHOP::SHOP_GOLD_GEM || eType == E_SHOP::SHOP_GOLD_GEM_BIG || eType == E_SHOP::SHOP_D_GOLD_GEM )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(UILayoutShopItem::onClickItemBuyGold, this));
        popup->show();
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(UILayoutShopItem::onClickItemBuy, this));
        popup->show();
    }
}

void UILayoutShopItem::onClickItemBuy(Ref* sender)
{
    E_SHOP eType = _eShopBuy;
     
    int result = ShopItemManager::getInstance()->onBuy(eType);
    if ( result != 0 )
    {
        if ( result == 1 )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
        }
        return;
    }
    
    // draw
    drawList();    
}

void UILayoutShopItem::onClickItemBuyGold(Ref* sender)
{
    E_SHOP eType = _eShopBuy;
    
    int result = ShopItemManager::getInstance()->onBuyGold(eType);
    if ( result != 0 )
    {
        if ( result == 1 )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
            popup->show();
        }
        return;
    }
    
    // draw
    drawList();
}


