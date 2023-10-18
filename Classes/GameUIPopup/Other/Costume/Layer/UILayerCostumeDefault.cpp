//
//  UILayerCostumeDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayerCostumeDefault.h"

#include "GameObject/InfoProduct.h"

#include "GameUIPopup/Other/Costume/PopupCostumeRent.h"
#include "GameUIPopup/Other/Costume/PopupLegendMake.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Costume/PopupCostumeReview.h"

#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayerCostumeDefault* UILayerCostumeDefault::create(std::string strType, cocos2d::Size size)
{
    UILayerCostumeDefault* pRet = new(std::nothrow) UILayerCostumeDefault();
    if ( pRet && pRet->init(strType, size) )
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

UILayerCostumeDefault::UILayerCostumeDefault(void) :
_strType(""),
_eDetailType(E_DETAIL_TYPE::DEFAULT),


_layerContainerType(nullptr),
_layerContainerList(nullptr),
_table(nullptr)
{
    
}

UILayerCostumeDefault::~UILayerCostumeDefault(void)
{
    
}

bool UILayerCostumeDefault::init(std::string strType, cocos2d::Size size)
{
    if ( !UILayerCostumeParent::init() )
    {
        return false;
    }
    
    setContentSize(size);
    
    //
    _strType = strType;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayerCostumeDefault::setContentSize(const Size & size)
{
    LayerColor::setContentSize(size);
    
    //
    if ( _layerContainerType != nullptr )
    {
        _layerContainerType->setPositionY(getContentSize().height);
    }
    
    if ( _layerContainerList != nullptr )
    {
        double nWidth = _layerContainerList->getContentSize().width;
        double nHeight = getContentSize().height - _layerContainerType->getContentSize().height;
    
        _layerContainerList->setContentSize(Size(nWidth, nHeight));
        _table->setViewSize(Size(nWidth, nHeight));
        _table->setPosition(nWidth / 2, nHeight / 2);
    }
}

void UILayerCostumeDefault::refreshList()
{
    drawList();
}

#pragma mark - table
void UILayerCostumeDefault::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerCostumeDefault::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 150);
}

extension::TableViewCell* UILayerCostumeDefault::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    
    // obj
    auto obj = _listCostumeTemp.at(idx);
    
    bool bExist = CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx());
    bool bRent = CostumeManager::getInstance()->isRent(obj->getType(), obj->getIdx());
    bool bEquip = false;
    if ( obj->getIdx() == CostumeManager::getInstance()->getEquip(obj->getType()) )
    {
        bEquip = true;
    }
    
    // bg
    std::string strBGPath = "Assets/ui_common/list_bg_1.png";
    if ( bEquip == true )
    {
        strBGPath = "Assets/ui_common/list_bg_2.png";
    }
    auto spriteBG = ui::Scale9Sprite::create(strBGPath, Rect::ZERO, Rect(0,130,10,10));
    spriteBG->setContentSize(Size(size.width - 10, size.height - 10));
    spriteBG->setPosition(size.width / 2, size.height / 2);
    spriteBG->setTag((int)idx);
    cell->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    //
    auto itemBox = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/box_quest.png"), Sprite::create("Assets/ui_common/box_quest.png"), nullptr, CC_CALLBACK_1(UILayerCostumeDefault::onClickReview, this));
    itemBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemBox->setPosition(120, spriteBG->getContentSize().height / 2);
    itemBox->setTag((int)idx);
    menu->addChild(itemBox);
    {
        auto spriteIcon = Sprite::create(CostumeManager::getInstance()->getIconPath(obj->getType(), (E_COSTUME_IDX)obj->getIdx()));
        spriteIcon->setPosition(itemBox->getContentSize().width / 2, itemBox->getContentSize().height / 2);
        spriteIcon->setScale(itemBox->getContentSize().width / spriteIcon->getContentSize().width);
        itemBox->addChild(spriteIcon);
        
        Vector<Sprite*> listType;
        if ( obj->isTypeNormal() == true )      listType.pushBack(Sprite::create("Assets/ui/costume/icon_normal.png"));
        if ( obj->isTypePrison() == true )      listType.pushBack(Sprite::create("Assets/ui/costume/icon_prison.png"));
        
        for ( int i = 0; i < listType.size(); i++ )
        {
            auto spriteIcon = (Sprite*)listType.at(i);
            spriteIcon->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            spriteIcon->setPosition(itemBox->getContentSize().width - i * spriteIcon->getContentSize().width - i * 2, itemBox->getContentSize().height);
            itemBox->addChild(spriteIcon);
        }
        
        auto spriteMagnifier = Sprite::create("Assets/ui/review/icon_magnifier.png");
        spriteMagnifier->setPosition(itemBox->getContentSize().width - 15, 15);
        itemBox->addChild(spriteMagnifier);
    }
    
    //
    auto itemAction = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1_max.png"), CC_CALLBACK_1(UILayerCostumeDefault::onClickAction, this));
    itemAction->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemAction->setPosition(spriteBG->getContentSize().width - 4, spriteBG->getContentSize().height / 2);
    itemAction->setEnabled(true);
    itemAction->setTag((int)idx);
    menu->addChild(itemAction);
    {
        if ( bEquip == true && bRent == false )
        {
            itemAction->setEnabled(false);
            
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_equip_now"), GAME_FONT, 24);
            label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height / 2);
            label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setColor(COLOR_COIN);
            itemAction->addChild(label);
        }
        else if ( bExist == true )
        {// 보유 중
            std::string strText = GAME_TEXT("t_ui_costume_equip");
            auto label = Label::createWithTTF(strText, GAME_FONT, 24);
            label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height / 2);
            label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setColor(COLOR_COIN);
            itemAction->addChild(label);
        }
        else
        {
            if ( obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND )
            {
                auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_making"), GAME_FONT, 24);
                label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height / 2);
                label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
                label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                label->setOverflow(Label::Overflow::SHRINK);
                label->setColor(COLOR_COIN);
                itemAction->addChild(label);
            }
            else if ( obj->getPrice() == 0 )
            {
                auto label = Label::createWithTTF(GAME_TEXT("t_ui_relic_free"), GAME_FONT, 24);
                label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height / 2);
                label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
                label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                label->setOverflow(Label::Overflow::SHRINK);
                label->setColor(COLOR_COIN);
                itemAction->addChild(label);
            }
            else
            {
                int nCurrencyCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
                if ( nCurrencyCoin < obj->getPrice() )
                {
                    ((Sprite*)itemAction->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_1_max.png");
                }
                    
                auto label = Label::createWithTTF(GAME_TEXT("t_ui_buy"), GAME_FONT, 24);
                label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height * 0.30);
                label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
                label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                label->setOverflow(Label::Overflow::SHRINK);
                label->setColor(COLOR_COIN);
                itemAction->addChild(label);
                
                auto spriteCurrencyIcon = Sprite::create("Assets/icon/icon_coin_2.png");
                spriteCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                spriteCurrencyIcon->setPosition(55, itemAction->getContentSize().height * 0.70);
                itemAction->addChild(spriteCurrencyIcon);
                
                auto labelCurrency = Label::createWithTTF(MafUtils::format("x%d", obj->getPrice()), GAME_FONT, 24);
                labelCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                labelCurrency->setPosition(45, itemAction->getContentSize().height * 0.70);
                labelCurrency->setColor(COLOR_COIN);
                itemAction->addChild(labelCurrency);
            }
        }
    }
    
    auto itemRent = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1_max.png"), CC_CALLBACK_1(UILayerCostumeDefault::onClickRent, this));
    itemRent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemRent->setPosition(spriteBG->getContentSize().width - 4 - 120 - 4, spriteBG->getContentSize().height / 2);
    itemRent->setEnabled(true);
    itemRent->setTag((int)idx);
    menu->addChild(itemRent);
    if ( bExist == true || obj->getPrice() == 0 || obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND )
    {
        itemRent->setVisible(false);
        itemRent->setEnabled(false);
    }
    {
        if ( bRent == true )
        {
            std::string strText = GAME_TEXT("t_ui_costume_equip");
            if ( bEquip == true )
            {
                strText = GAME_TEXT("t_ui_costume_equip_now");
                itemRent->setEnabled(false);
            }
                
            auto label = Label::createWithTTF(MafUtils::convertNumberToTime(CostumeManager::getInstance()->getRentTime(obj->getType()), true), GAME_FONT, 24);
            label->setPosition(itemRent->getContentSize().width / 2, itemRent->getContentSize().height * 0.30);
            label->setDimensions(itemRent->getContentSize().width * 0.9, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setColor(COLOR_COIN);
            itemRent->addChild(label);
                      
            auto labelEquip = Label::createWithTTF(strText, GAME_FONT, 24);
            labelEquip->setPosition(itemRent->getContentSize().width / 2, itemRent->getContentSize().height  * 0.70);
            labelEquip->setColor(COLOR_COIN);
            itemRent->addChild(labelEquip);
        }
        else
        {
            int nCurrencyCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
            if ( nCurrencyCoin < obj->getPriceRent() )
            {
                ((Sprite*)itemRent->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_1_max.png");
            }
            
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_rental_1"), GAME_FONT, 24);
            label->setPosition(itemRent->getContentSize().width / 2, itemRent->getContentSize().height * 0.30);
            label->setDimensions(itemRent->getContentSize().width * 0.9, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setColor(COLOR_COIN);
            itemRent->addChild(label);
            int nRentCount = CostumeManager::getInstance()->getRentCount(obj->getType(), obj->getIdx());
            if ( nRentCount > 0 )
            {
                label->setString(GAME_TEXTFORMAT("t_ui_costume_rental_2", nRentCount));
            }
            
            auto spriteCurrencyIcon = Sprite::create("Assets/icon/icon_coin_2.png");
            spriteCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            spriteCurrencyIcon->setPosition(55, itemRent->getContentSize().height * 0.70);
            itemRent->addChild(spriteCurrencyIcon);
            
            auto labelCurrency = Label::createWithTTF(MafUtils::format("x%d", obj->getPriceRent()), GAME_FONT, 24);
            labelCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelCurrency->setPosition(45, itemRent->getContentSize().height * 0.70);
            labelCurrency->setColor(COLOR_COIN);
            itemRent->addChild(labelCurrency);
        }
    }
    
    auto itemInfo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1_max.png"), CC_CALLBACK_1(UILayerCostumeDefault::onClickInfo, this));
    itemInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemInfo->setPosition(spriteBG->getContentSize().width - 4 - 120 - 4, spriteBG->getContentSize().height / 2);
    itemInfo->setTag((int)idx);
    itemInfo->setVisible(false);
    itemInfo->setEnabled(false);
    menu->addChild(itemInfo);
    if ( obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND && bExist == true )
    {
        itemInfo->setVisible(true);
        itemInfo->setEnabled(true);
    }
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_info"), GAME_FONT, 24);
        label->setPosition(itemInfo->getContentSize().width / 2, itemInfo->getContentSize().height / 2);
        label->setDimensions(itemInfo->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setColor(COLOR_COIN);
        itemInfo->addChild(label);
    }
    
    double nWidthInfo = spriteBG->getContentSize().width - 130 - 4 - 120 - 10;
    double nHeightInfo = spriteBG->getContentSize().height;
    if ( itemRent->isVisible() == true || itemInfo->isVisible() == true )
    {
        nWidthInfo = nWidthInfo - 4 - 120;
    }
    auto layerInfo = Layer::create();
    layerInfo->setContentSize(Size(nWidthInfo, nHeightInfo));
    layerInfo->setIgnoreAnchorPointForPosition(false);
    layerInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    layerInfo->setPosition(130, spriteBG->getContentSize().height);
    spriteBG->addChild(layerInfo);
    {
        auto menuInfo = MafNode::MafMenu::create();
        menuInfo->setPosition(Vec2::ZERO);
        layerInfo->addChild(menuInfo);
        
        //
        std::string strDibsPath = "Assets/ui/costume/btn_wishlist_1_2.png";
        if ( obj->isDips() == true )
            strDibsPath = "Assets/ui/costume/btn_wishlist_1_1.png";
        auto itemDibs = MafNode::MafMenuItemSprite::create(Sprite::create(strDibsPath), Sprite::create(strDibsPath), nullptr, CC_CALLBACK_1(UILayerCostumeDefault::onClickDips, this));
        itemDibs->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        itemDibs->setPosition(0, layerInfo->getContentSize().height - 48);
        itemDibs->setTag((int)idx);
        menuInfo->addChild(itemDibs);
        
        //
        std::string strTitle = obj->getTitle();
        if ( obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND && bExist == true )
        {
            strTitle = DataManager::GetCostumeLegendName(obj->getType(), obj->getIdx());
        }
        auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 24);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelTitle->setPosition(40, layerInfo->getContentSize().height - 50);
        labelTitle->setColor(COLOR_COIN);
        layerInfo->addChild(labelTitle);
        double nScale = (layerInfo->getContentSize().width - 100) / labelTitle->getContentSize().width;
        if ( nScale < 1 )
        {
            labelTitle->setScale(nScale);
        }
        {// stack
            std::string strStack = "";
            bool bStackReset = false;
            if (obj->getType() == E_COSTUME::HEAD && obj->getIdx() == E_COSTUME_IDX::IDX_11)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getRevivalAttackBuff());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::CAPE && obj->getIdx() == E_COSTUME_IDX::IDX_14)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getHitBuff());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::CAPE && obj->getIdx() == E_COSTUME_IDX::IDX_15)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getEnchantRevivalAttackBuff());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::FOOT && obj->getIdx() == E_COSTUME_IDX::IDX_12)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getDragonBuff());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::HAND && obj->getIdx() == E_COSTUME_IDX::IDX_15)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getRevivalQuestBuff());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::FOOT && obj->getIdx() == E_COSTUME_IDX::IDX_14)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getHellBuff());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::BODY && obj->getIdx() == E_COSTUME_IDX::IDX_8)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getQuest25Buff());
            }
            else if (obj->getType() == E_COSTUME::HEAD && obj->getIdx() == E_COSTUME_IDX::IDX_32)
            {
                strStack = MafUtils::format("x%d", UserInfoManager::getInstance()->getAttendDay());
            }
            else if (obj->getType() == E_COSTUME::HEAD && obj->getIdx() == E_COSTUME_IDX::IDX_17)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getRevivalKeyBuff1());
                bStackReset = true;
            }
            else if (obj->getType() == E_COSTUME::HAND && obj->getIdx() == E_COSTUME_IDX::IDX_35)
            {
                strStack = MafUtils::format("x%d", UserInfoStackManager::getInstance()->getRevivalKeyBuff2());
            }
            
            if ( strStack.length() != 0 )
            {
                auto labelStack = Label::createWithTTF(strStack, GAME_FONT, 20);
                labelStack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                labelStack->setPosition(labelTitle->getPositionX() + labelTitle->getBoundingBox().size.width, labelTitle->getPositionY());
                layerInfo->addChild(labelStack);
                
                if ( bStackReset == true && CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_11) == false )
                {
                    auto itemStackReset = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/icon_reset.png"), Sprite::create("Assets/ui/costume/icon_reset.png"), nullptr, CC_CALLBACK_1(UILayerCostumeDefault::onClickStackReset, this));
                    itemStackReset->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    itemStackReset->setPosition(labelStack->getPositionX() + labelStack->getContentSize().width, labelStack->getPositionY());
                    itemStackReset->setTouchRect(itemStackReset->getContentSize() * 2);
                    itemStackReset->setTag((int)idx);
                    menuInfo->addChild(itemStackReset);
                }
            }
            
        }
        
        //
        std::string strTypeDetail = "";
        if ( obj->isTypeNormal() == true )
            strTypeDetail = GAME_TEXT("t_ui_dungeon_normal");
        if ( obj->isTypePrison() == true )
        {
            if ( strTypeDetail.length() != 0 )    strTypeDetail.append(", ");
            strTypeDetail.append(GAME_TEXT("t_ui_prison"));
        }
        auto labelTypeDetail = Label::createWithTTF(strTypeDetail, GAME_FONT, 16);
        labelTypeDetail->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelTypeDetail->setPosition(0, layerInfo->getContentSize().height - 50);
        layerInfo->addChild(labelTypeDetail);
        
        if ( obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND )
        {
            std::string strDesc = DataManager::GetCostumeLegendDesc(obj->getType(), obj->getIdx());

            int nCount = CostumeManager::getInstance()->getExistTypeCount(obj->getType());
            int nCountMax = DataManager::GetCostumeLegendTermsCount(obj->getType());
            
            auto labelDesc = Label::createWithTTF(strDesc, GAME_FONT, 18);
            labelDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            labelDesc->setPosition(0, spriteBG->getContentSize().height - 75);
            labelDesc->setLineSpacing(-5);
            labelDesc->setDimensions(layerInfo->getContentSize().width, 30);
            labelDesc->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
            labelDesc->setOverflow(Label::Overflow::SHRINK);
            layerInfo->addChild(labelDesc);
            
            if ( bExist == true )
            {
                labelDesc->setDimensions(labelDesc->getDimensions().width, 50);
            }
            else if ( bExist == false )
            {
                //
                if      (obj->getType() == E_COSTUME::HEAD)   strDesc = GAME_TEXTFORMAT("t_ui_costume_msg_23", nCountMax);
                else if (obj->getType() == E_COSTUME::BODY)   strDesc = GAME_TEXTFORMAT("t_ui_costume_msg_24", nCountMax);
                else if (obj->getType() == E_COSTUME::CAPE)   strDesc = GAME_TEXTFORMAT("t_ui_costume_msg_25", nCountMax);
                else if (obj->getType() == E_COSTUME::HAND)   strDesc = GAME_TEXTFORMAT("t_ui_costume_msg_26", nCountMax);
                else if (obj->getType() == E_COSTUME::FOOT)   strDesc = GAME_TEXTFORMAT("t_ui_costume_msg_27", nCountMax);
                labelDesc->setString(strDesc);

                
                //
                auto spriteProgressBG = ui::Scale9Sprite::create("Assets/ui/prison/quest_gauge_bg.png", Rect::ZERO, Rect(0,0,1,1));
                spriteProgressBG->setContentSize(Size(spriteProgressBG->getContentSize().width * 2, spriteProgressBG->getContentSize().height));
                spriteProgressBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                spriteProgressBG->setPosition(0, 5);
                layerInfo->addChild(spriteProgressBG);
                
                int nPercent = (double)nCount / nCountMax * 100;
                if ( nPercent > 100 )
                    nPercent = 100;
                
                auto spriteProgressIn = Sprite::create("Assets/ui/prison/quest_gauge_a.png");
                
                auto progressPurchase = ProgressTimer::create(spriteProgressIn);
                progressPurchase->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                progressPurchase->setPosition(0, spriteProgressBG->getContentSize().height/2);
                progressPurchase->setType(ProgressTimer::Type::BAR);
                progressPurchase->setBarChangeRate(Vec2(1,0));
                progressPurchase->setMidpoint(Vec2(0, 0.5));
                progressPurchase->setPercentage(nPercent);
                progressPurchase->setScaleX(2);
                spriteProgressBG->addChild(progressPurchase);
                
                
                auto labelProgressCount = Label::createWithTTF(MafUtils::format("%d / %d", nCount, nCountMax), GAME_FONT, 18);
                labelProgressCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                labelProgressCount->setPosition(10, spriteProgressBG->getContentSize().height / 2);
                labelProgressCount->enableBold();
                spriteProgressBG->addChild(labelProgressCount);
                
                auto labelProgressPercent = Label::createWithTTF(MafUtils::format("%d %%", nPercent), GAME_FONT, 18);
                labelProgressPercent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                labelProgressPercent->setPosition(spriteProgressBG->getContentSize().width - 10, spriteProgressBG->getContentSize().height / 2);
                labelProgressPercent->enableBold();
                spriteProgressBG->addChild(labelProgressPercent);
            }
        }
        else
        {
            //
            auto labelDesc = Label::createWithTTF(obj->getDesc(), GAME_FONT, 18);
            labelDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            labelDesc->setPosition(0, spriteBG->getContentSize().height - 75);
            labelDesc->setLineSpacing(-5);
            labelDesc->setDimensions(layerInfo->getContentSize().width, 50);
            labelDesc->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
            labelDesc->setOverflow(Label::Overflow::SHRINK);
            layerInfo->addChild(labelDesc);
        }
        
        // 코스튬 유저 주인 닉네임
        if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
        {
            std::string strNick = "";
            if (obj->getIdx() == E_COSTUME_IDX::IDX_32 && obj->getType() != E_COSTUME::JEWEL && obj->getType() != E_COSTUME::SETITEM) {
                strNick = "by 차민";
            }else if (obj->getIdx() == E_COSTUME_IDX::IDX_33 && obj->getType() != E_COSTUME::JEWEL && obj->getType() != E_COSTUME::SETITEM) {
                strNick = "by Oopso";
            }else if (obj->getIdx() == E_COSTUME_IDX::IDX_34 && obj->getType() != E_COSTUME::JEWEL && obj->getType() != E_COSTUME::SETITEM) {
                strNick = "by 페리틴크";
            }
            
            if ( strNick.length() != 0 )
            {
                auto labelUserNick = Label::createWithTTF(strNick, GAME_FONT, 16);
                labelUserNick->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                labelUserNick->setPosition(layerInfo->getContentSize().width, layerInfo->getContentSize().height);
                labelUserNick->setColor(COLOR_COIN);
                layerInfo->addChild(labelUserNick);
            }
        }
    }
    
    
    return cell;
}

ssize_t UILayerCostumeDefault::numberOfCellsInTableView(extension::TableView *table)
{
    
    return _listCostumeTemp.size();
}

#pragma mark - init
void UILayerCostumeDefault::initVar()
{
    _listCostume = CostumeManager::getInstance()->getCostumeType(_strType);
    setList();
}

void UILayerCostumeDefault::initUi()
{
    // top layer
    _layerContainerType = Layer::create();
    _layerContainerType->setContentSize(Size(getContentSize().width, 60));
    _layerContainerType->setIgnoreAnchorPointForPosition(false);
    _layerContainerType->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerType->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_layerContainerType);
    
    // button layer
    _layerContainerList = Layer::create();
    _layerContainerList->setContentSize(Size(getContentSize().width, getContentSize().height - _layerContainerType->getContentSize().height));
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerList->setPosition(getContentSize().width / 2, 0);
    addChild(_layerContainerList);
   
    
    //
    uiDetailType();
    uiList();
}


#pragma mark - ui
void UILayerCostumeDefault::uiDetailType()
{
    _layerContainerType->removeAllChildren();
    
    //
    E_DETAIL_TYPE listType[] = {
        E_DETAIL_TYPE::DEFAULT,
        E_DETAIL_TYPE::NORMAL,
        E_DETAIL_TYPE::PRISON,
        E_DETAIL_TYPE::EXIST,
        E_DETAIL_TYPE::NONE,
        E_DETAIL_TYPE::DIBS
    };
     
    int nWidthChild = _layerContainerType->getContentSize().width / (sizeof(listType)/sizeof(int));
    int nHeightChild = _layerContainerType->getContentSize().height;

    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerType->addChild(menu);
     
     for ( int i = 0; i < sizeof(listType)/sizeof(int); i++ )
     {
         std::string strCheckPath = "Assets/ui/costume/menu_btn_list_5.png";
         std::string strIconPath = "";
         std::string strText = "";
     
         if ( _eDetailType == listType[i] )
         {
             strCheckPath = "Assets/ui/costume/menu_btn_list_4.png";
         }
         
         switch (listType[i]) {
             case E_DETAIL_TYPE::DEFAULT:
             {
                 strIconPath = "Assets/ui/costume/menu_btn_list_3.png";
                 strText = GAME_TEXT("t_ui_costume_all");
                 break;
             }
             case E_DETAIL_TYPE::NORMAL:
             {
                 strIconPath = "Assets/ui/costume/menu_btn_list_6.png";
                 strText = GAME_TEXT("t_ui_dungeon_normal");
                 break;
             }
             case E_DETAIL_TYPE::PRISON:
             {
                 strIconPath = "Assets/ui/costume/menu_btn_list_7.png";
                 strText = GAME_TEXT("t_ui_prison");
                 break;
             }
             case E_DETAIL_TYPE::EXIST:
             {
                 strIconPath = "Assets/ui/costume/menu_btn_list_8.png";
                 strText = GAME_TEXT("t_ui_buy_yes");
                 break;
             }
             case E_DETAIL_TYPE::NONE:
             {
                 strIconPath = "Assets/ui/costume/menu_btn_list_9.png";
                 strText = GAME_TEXT("t_ui_buy_no");
                 break;
             }
             case E_DETAIL_TYPE::DIBS:
             {
                 strIconPath = "Assets/ui/costume/menu_btn_list_10.png";
                 strText = GAME_TEXT("t_ui_dibs");
                 break;
             }
    
             default:
                 break;
         }
         
         float posX = i * nWidthChild;
         float posY = nHeightChild;
         
         auto layer = Layer::create();
         layer->setContentSize(Size(nWidthChild, nHeightChild));
         
         auto itemType = MafNode::MafMenuItemSprite::create(layer, nullptr, CC_CALLBACK_1(UILayerCostumeDefault::onClickDetailType, this));
         itemType->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
         itemType->setPosition(posX, posY);
         itemType->setTag((int)listType[i]);
         menu->addChild(itemType);
         
         auto spriteCheck = Sprite::create(strCheckPath);
         spriteCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
         spriteCheck->setPosition(5, itemType->getContentSize().height / 2);
         spriteCheck->setScale(2);
         spriteCheck->getTexture()->setTexParameters(texParams);
         itemType->addChild(spriteCheck);
         
         auto spriteIcon = Sprite::create(strIconPath);
         spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
         spriteIcon->setPosition(35, itemType->getContentSize().height / 2);
         spriteIcon->setScale(2);
         spriteIcon->getTexture()->setTexParameters(texParams);
         itemType->addChild(spriteIcon);
         
         auto labelText = Label::createWithTTF(strText, GAME_FONT, 14);
         labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
         labelText->setPosition(65, itemType->getContentSize().height / 2);
         labelText->setDimensions(nWidthChild - 65, labelText->getContentSize().height);
         labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
         labelText->setOverflow(Label::Overflow::SHRINK);
         labelText->setLineBreakWithoutSpace(false);
         itemType->addChild(labelText);
     }
}

void UILayerCostumeDefault::uiList()
{
    _layerContainerList->removeAllChildren();
    
    //
    _table = extension::TableView::create(this, Size(_layerContainerList->getContentSize().width, _layerContainerList->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _table->setPosition(_layerContainerList->getContentSize().width / 2, _layerContainerList->getContentSize().height / 2);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _layerContainerList->addChild(_table);
    
    //
    _table->reloadData();
}

#pragma mark - ui draw
void UILayerCostumeDefault::drawDetailType()
{
    uiDetailType();
}

void UILayerCostumeDefault::drawList()
{
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}

#pragma mark - set, get, add
void UILayerCostumeDefault::setList()
{
    _listCostumeTemp.clear();
    if ( _eDetailType == E_DETAIL_TYPE::DEFAULT )
    {
        _listCostumeTemp = _listCostume;
    }
    else
    {
        for ( auto& obj : _listCostume )
        {
            if ( _eDetailType == E_DETAIL_TYPE::NORMAL && obj->isTypeNormal() == true )
            {
                _listCostumeTemp.pushBack(obj);
            }
            else if ( _eDetailType == E_DETAIL_TYPE::PRISON && obj->isTypePrison() == true )
            {
                _listCostumeTemp.pushBack(obj);
            }
            else if ( _eDetailType == E_DETAIL_TYPE::EXIST && CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx()) == true )
            {
                _listCostumeTemp.pushBack(obj);
            }
            else if ( _eDetailType == E_DETAIL_TYPE::NONE && CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx()) == false )
            {
                _listCostumeTemp.pushBack(obj);
            }
            else if ( _eDetailType == E_DETAIL_TYPE::DIBS && obj->isDips() == true )
            {
                _listCostumeTemp.pushBack(obj);
            }
        }
    }
    
}

#pragma mark - click
void UILayerCostumeDefault::onClickDetailType(Ref *sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    E_DETAIL_TYPE eDetailType  = (E_DETAIL_TYPE)item->getTag();
    if ( _eDetailType == eDetailType )
    {
        if ( _eDetailType == E_DETAIL_TYPE::DEFAULT )
        {
            return;
        }
        _eDetailType = E_DETAIL_TYPE::DEFAULT;
    }
    else
    {
        _eDetailType = eDetailType;
    }
   
    //
    setList();
    
    //
    drawDetailType();
    drawList();
}

void UILayerCostumeDefault::onClickReview(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    auto popup = PopupCostumeReview::create(obj->getType(), obj->getIdx());
    popup->show();
}

void UILayerCostumeDefault::onClickDips(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    bool bDips = obj->isDips();
    obj->setDips(!bDips);
    
    //
    drawList();
}

void UILayerCostumeDefault::onClickAction(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    E_COSTUME eType = obj->getType();
    E_COSTUME_IDX eIdx = obj->getIdx();
    
    bool bExist = CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx());
    bool bEquip = false;
    if ( obj->getIdx() == CostumeManager::getInstance()->getEquip(obj->getType()) )
    {
        bEquip = true;
    }
    
    if ( bExist == false )
    {
        if ( obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND )
        {
            auto popup = PopupLegendMake::create(obj->getType(), obj->getIdx());
            popup->show();
        }
        else
        {
            int nCurrencyCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
            if ( obj->getPrice() > nCurrencyCoin )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
                popup->show();
            }
            else
            {
                auto popup = PopupDefault::create(obj->getTitle(), GAME_TEXT("t_ui_costume_msg_31"));
                popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
                popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
                    
                    PopupLoading::show();
                    CostumeManager::getInstance()->requestBuy(eType, eIdx, CC_CALLBACK_2(UILayerCostumeDefault::callbackBuy, this));

                });
                popup->show();
            }
        }
    }
    else
    {// 소유 중
        CostumeManager::getInstance()->setEquip(obj->getType(), obj->getIdx());
        SaveManager::saveCostume();
        
        //
        drawList();
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
    }
    
    
}

void UILayerCostumeDefault::onClickRent(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    bool bRent = CostumeManager::getInstance()->isRent(obj->getType(), obj->getIdx());
    bool bEquip = false;
    if ( obj->getIdx() == CostumeManager::getInstance()->getEquip(obj->getType()) )
    {
        bEquip = true;
    }
    
    if ( bRent == true && bEquip == false )
    {
        CostumeManager::getInstance()->setEquip(obj->getType(), obj->getIdx());
        SaveManager::saveCostume();
        
        //
        drawList();
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
    }
    else
    {
        auto popup = PopupCostumeRent::create(obj->getType(), obj->getIdx());
        popup->setCallbackYes([=](int eType, int eIdx, bool isCoupon){
            
            auto objCostume = CostumeManager::getInstance()->getCostume((E_COSTUME)eType, (E_COSTUME_IDX)eIdx);
            if ( objCostume == nullptr )
            {
                return;
            }
            
            if ( isCoupon == false )
            {
                int nPrice = objCostume->getPriceRent();
                int nCurrencyCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
                if ( nPrice > nCurrencyCoin )
                {
                    auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
                    popup->show();
                    return;
                }
                
                PopupLoading::show();
                CostumeManager::getInstance()->requestRent(objCostume->getType(), objCostume->getIdx(), objCostume->getPriceRent(), CC_CALLBACK_1(UILayerCostumeDefault::callbackRent, this));
            }
            else
            {
                int nCoupon = ItemsMoneyManager::getInstance()->getCostumeCoupon();
                if ( nCoupon <= 0 )
                {
                    return;
                }
                
                //
                CostumeManager::getInstance()->setEquip(objCostume->getType(), objCostume->getIdx());
                CostumeManager::getInstance()->setRent(objCostume->getType(), objCostume->getIdx());
                CostumeManager::getInstance()->setRentTime(objCostume->getType(), TIME_COSTUME_RENT);
                ItemsMoneyManager::getInstance()->setCostumeCoupon(nCoupon - 1);
                
                //
                SaveManager::saveCostume();
                SaveManager::saveCostumeRentList();
                SaveManager::saveCostumeRentTime();

                //
                if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_15) == true )
                {
                    DataManager::getInstance()->initArtifactSaveData(DG_NORMAL);
                }
                
                //
                PopupLoading::show();
                callbackRent(true);
            }
            
        });
        popup->show();
    }
}

void UILayerCostumeDefault::onClickInfo(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());

    //
    auto popup = PopupLegendMake::create(obj->getType(), obj->getIdx());
    popup->setCallbackResult(CC_CALLBACK_1(UILayerCostumeDefault::callbackBuyLegend, this));
    popup->show();
}

void UILayerCostumeDefault::onClickStackReset(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_11) == true )
    {
        return;
    }
    
    E_COSTUME eType = obj->getType();
    E_COSTUME_IDX eIdx = obj->getIdx();
    
    auto popup = PopupDefault::create(obj->getTitle(), GAME_TEXT("t_ui_costume_msg_28"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        if (eType == E_COSTUME::HEAD && eIdx == E_COSTUME_IDX::IDX_11)
        {
            UserInfoStackManager::getInstance()->setRevivalAttackBuff(0);
        }
        else if (eType == E_COSTUME::CAPE && eIdx == E_COSTUME_IDX::IDX_14)
        {
            UserInfoStackManager::getInstance()->setHitBuff(0);
        }
        else if (eType == E_COSTUME::CAPE && eIdx == E_COSTUME_IDX::IDX_15)
        {
            UserInfoStackManager::getInstance()->setEnchantRevivalAttackBuff(0);
        }
        else if (eType == E_COSTUME::FOOT && eIdx == E_COSTUME_IDX::IDX_12)
        {
            UserInfoStackManager::getInstance()->setDragonBuff(0);
        }
        else if (eType == E_COSTUME::HAND && eIdx == E_COSTUME_IDX::IDX_15)
        {
            UserInfoStackManager::getInstance()->setRevivalQuestBuff(0);
        }
        else if (eType == E_COSTUME::FOOT && eIdx == E_COSTUME_IDX::IDX_14)
        {
            UserInfoStackManager::getInstance()->setHellBuff(0);
        }
        else if (eType == E_COSTUME::BODY && eIdx == E_COSTUME_IDX::IDX_8)
        {
            
        }
        else if (eType == E_COSTUME::HEAD && eIdx == E_COSTUME_IDX::IDX_32)
        {
            
        }
        else if (eType == E_COSTUME::HEAD && eIdx == E_COSTUME_IDX::IDX_17)
        {
            UserInfoStackManager::getInstance()->setRevivalKeyBuff1(0);
        }
        else if (eType == E_COSTUME::HAND && eIdx == E_COSTUME_IDX::IDX_35)
        {
            
        }
        
        //
        drawList();
    });
    popup->show();
}

#pragma mark - callback
void UILayerCostumeDefault::callbackRent(bool bResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }

    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_msg_36"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    drawList();
}

void UILayerCostumeDefault::callbackBuy(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )
        {
            strMessage = GAME_TEXT("t_ui_error_31");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawList();
}

void UILayerCostumeDefault::callbackBuyLegend(bool bResult)
{
    //
    drawList();
}
