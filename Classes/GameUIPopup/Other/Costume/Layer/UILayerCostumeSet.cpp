//
//  UILayerCostumeSet.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayerCostumeSet.h"

#include "GameObject/InfoProduct.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Costume/PopupCostumeRent.h"
#include "GameUIPopup/Other/Costume/PopupLegendMake.h"
#include "GameUIPopup/Other/Costume/PopupCostumeReview.h"

#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayerCostumeSet* UILayerCostumeSet::create(std::string strType, cocos2d::Size size)
{
    UILayerCostumeSet* pRet = new(std::nothrow) UILayerCostumeSet();
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

UILayerCostumeSet::UILayerCostumeSet(void) :
_strType(""),
_eDetailType(E_DETAIL_TYPE::DEFAULT),


_layerContainerType(nullptr),
_layerContainerList(nullptr),
_table(nullptr)
{
    
}

UILayerCostumeSet::~UILayerCostumeSet(void)
{
    
}

bool UILayerCostumeSet::init(std::string strType, cocos2d::Size size)
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

void UILayerCostumeSet::setContentSize(const Size & size)
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

void UILayerCostumeSet::refreshList()
{
    drawList();
}

#pragma mark - table
void UILayerCostumeSet::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerCostumeSet::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 224);
}

extension::TableViewCell* UILayerCostumeSet::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    auto spriteBox = ui::Scale9Sprite::create("Assets/ui_common/box_quest.png", Rect::ZERO, Rect(2,2,112,112));
    spriteBox->setContentSize(Size(116, 185));
    auto spriteSelectBox = ui::Scale9Sprite::create("Assets/ui_common/box_quest.png", Rect::ZERO, Rect(2,2,112,112));
    spriteBox->setContentSize(Size(116, 185));
    auto itemBox = MafNode::MafMenuItemSprite::create(spriteBox, spriteSelectBox, nullptr, CC_CALLBACK_1(UILayerCostumeSet::onClickReview, this));
    itemBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemBox->setPosition(120, spriteBG->getContentSize().height / 2);
    itemBox->setTag((int)idx);
    menu->addChild(itemBox);
    {
        auto spriteIcon = Sprite::create(MafUtils::format("Assets/character_hero/normal/body/body_%.2d_walk_1.png", obj->getIdx()));
        spriteIcon->setAnchorPoint(Vec2(0.25862,0.40172));
        spriteIcon->setPosition(itemBox->getContentSize().width / 2, itemBox->getContentSize().height / 2);
        spriteIcon->setScale(0.9);
        itemBox->addChild(spriteIcon);
        {
            auto spriteHead = Sprite::create(MafUtils::format("Assets/character_hero/normal/head/head_%.2d_walk_1.png", obj->getIdx()));
            spriteHead->setAnchorPoint(Vec2(0.5,0));
            spriteHead->setPosition(spriteIcon->getContentSize().width / 2, 0);
            spriteHead->getTexture()->setTexParameters(texParams);
            spriteHead->setScale(4);
            spriteIcon->addChild(spriteHead);
            
            auto spriteHand = Sprite::create(MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_walk_1.png", obj->getIdx()));
            spriteHand->setAnchorPoint(Vec2(0.5,0));
            spriteHand->setPosition(spriteIcon->getContentSize().width / 2, 0);
            spriteHand->getTexture()->setTexParameters(texParams);
            spriteHand->setScale(4);
            spriteIcon->addChild(spriteHand);
            
            auto spriteFoot = Sprite::create(MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_walk_1.png", obj->getIdx()));
            spriteFoot->setAnchorPoint(Vec2(0.5,0));
            spriteFoot->setPosition(spriteIcon->getContentSize().width / 2, 0);
            spriteFoot->getTexture()->setTexParameters(texParams);
            spriteFoot->setScale(4);
            spriteIcon->addChild(spriteFoot);
            
            auto spriteCape = Sprite::create(MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_walk_1.png", obj->getIdx()));
            spriteCape->setAnchorPoint(Vec2(0.5,0));
            spriteCape->setPosition(spriteIcon->getContentSize().width / 2, 0);
            spriteCape->getTexture()->setTexParameters(texParams);
            spriteCape->setScale(4);
            spriteIcon->addChild(spriteCape);
        }
        
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
    auto itemAction = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1_max.png"), CC_CALLBACK_1(UILayerCostumeSet::onClickAction, this));
    itemAction->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemAction->setPosition(spriteBG->getContentSize().width - 4, spriteBG->getContentSize().height / 2);
    itemAction->setEnabled(true);
    itemAction->setTag((int)idx);
    menu->addChild(itemAction);
    {
        if ( bEquip == true )
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
            if ( obj->getPrice() == 0 )
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
                int nCurrencyBong = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str());
                if ( nCurrencyBong < obj->getPrice() )
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
                
                auto spriteCurrencyIcon = Sprite::create("Assets/icon/evilcrystal.png");
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
   
    double nWidthInfo = spriteBG->getContentSize().width - 130 - 4 - 120 - 10;
    double nHeightInfo = spriteBG->getContentSize().height;
    
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
        auto itemDibs = MafNode::MafMenuItemSprite::create(Sprite::create(strDibsPath), Sprite::create(strDibsPath), nullptr, CC_CALLBACK_1(UILayerCostumeSet::onClickDips, this));
        itemDibs->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        itemDibs->setPosition(0, layerInfo->getContentSize().height - 48);
        itemDibs->setTag((int)idx);
        menuInfo->addChild(itemDibs);
        
        //
        std::string strTitle = obj->getTitle();
        auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 24);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelTitle->setPosition(40, layerInfo->getContentSize().height - 50);
        labelTitle->setAlignment(TextHAlignment::LEFT);
        labelTitle->setColor(COLOR_COIN);
        layerInfo->addChild(labelTitle);
        
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
        
        //
        auto labelDesc = Label::createWithTTF(obj->getDesc(), GAME_FONT, 18);
        labelDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelDesc->setPosition(0, spriteBG->getContentSize().height - 75);
        labelDesc->setLineSpacing(-5);
        labelDesc->setDimensions(layerInfo->getContentSize().width, 50);
        labelDesc->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
        labelDesc->setOverflow(Label::Overflow::SHRINK);
        layerInfo->addChild(labelDesc);
        
        auto spriteListBG = Sprite::create("Assets/ui/costume/list_bg_icon_bg.png");
        spriteListBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteListBG->setPosition(layerInfo->getContentSize().width / 2, 5);
        layerInfo->addChild(spriteListBG);
        {
            E_COSTUME listType[] = {E_COSTUME::HEAD, E_COSTUME::BODY, E_COSTUME::CAPE, E_COSTUME::HAND, E_COSTUME::FOOT};
            
            double nPosXStart = 0;
            double nPadding = 5;
            double nWidth = -nPadding;
            for ( int i = 0; i < sizeof(listType) / sizeof(E_COSTUME); i++ )
            {
                nWidth += 63.8 + nPadding;
            }
            
            nPosXStart = (491 - nWidth) / 2;
            for ( int i = 0; i < sizeof(listType) / sizeof(E_COSTUME); i++ )
            {
                auto spriteBox = Sprite::create("Assets/ui_common/box_quest.png");
                spriteBox->setScale(0.55);
                spriteBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                spriteBox->setPosition(nPosXStart + spriteBox->getBoundingBox().size.width * i + nPadding * i, spriteListBG->getContentSize().height / 2);
                spriteListBG->addChild(spriteBox);
                
                auto spriteBoxInCostume = Sprite::create(CostumeManager::getInstance()->getIconPath(listType[i], obj->getIdx()));
                spriteBoxInCostume->setPosition(spriteBox->getContentSize().width / 2, spriteBox->getContentSize().height / 2);
                spriteBoxInCostume->setScale(1.5);
                spriteBox->addChild(spriteBoxInCostume);
                if ( CostumeManager::getInstance()->isExist(listType[i], obj->getIdx()) == false )
                {
                    auto labelBoxInBlack = LayerColor::create(Color4B(0, 0, 0, 100), spriteBox->getContentSize().width, spriteBox->getContentSize().height);
                    labelBoxInBlack->setPosition(0, 0);
                    spriteBox->addChild(labelBoxInBlack);
                }
            }
        }
    }
    
    
    return cell;
}

ssize_t UILayerCostumeSet::numberOfCellsInTableView(extension::TableView *table)
{
    
    return _listCostumeTemp.size();
}

#pragma mark - init
void UILayerCostumeSet::initVar()
{
    _listCostume = CostumeManager::getInstance()->getCostumeType(_strType);
    setList();
}

void UILayerCostumeSet::initUi()
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
void UILayerCostumeSet::uiDetailType()
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
         
         auto itemType = MafNode::MafMenuItemSprite::create(layer, nullptr, CC_CALLBACK_1(UILayerCostumeSet::onClickDetailType, this));
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

void UILayerCostumeSet::uiList()
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
void UILayerCostumeSet::drawDetailType()
{
    uiDetailType();
}

void UILayerCostumeSet::drawList()
{
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}

#pragma mark - set, get, add
void UILayerCostumeSet::setList()
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
void UILayerCostumeSet::onClickDetailType(Ref *sender)
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

void UILayerCostumeSet::onClickReview(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    auto popup = PopupCostumeReview::create(obj->getType(), obj->getIdx());
    popup->show();
}

void UILayerCostumeSet::onClickDips(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    bool bDips = obj->isDips();
    obj->setDips(!bDips);
    
    //
    drawList();
}

void UILayerCostumeSet::onClickAction(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
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
        if ( CostumeManager::getInstance()->isExistSet((E_COSTUME_IDX)obj->getIdx()) == false )
        {
            std::string str = GAME_TEXT("t_ui_costume_msg_35");
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::HEAD, obj->getIdx()) == false )
            {
                auto objTemp = CostumeManager::getInstance()->getCostume(E_COSTUME::HEAD, obj->getIdx());
                str.append("\n").append(objTemp->getTitle());
            }
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::BODY, obj->getIdx()) == false )
            {
                auto objTemp = CostumeManager::getInstance()->getCostume(E_COSTUME::BODY, obj->getIdx());
                str.append("\n").append(objTemp->getTitle());
            }
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::CAPE, obj->getIdx()) == false )
            {
                auto objTemp = CostumeManager::getInstance()->getCostume(E_COSTUME::CAPE, obj->getIdx());
                str.append("\n").append(objTemp->getTitle());
            }
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::HAND, obj->getIdx()) == false )
            {
                auto objTemp = CostumeManager::getInstance()->getCostume(E_COSTUME::HAND, obj->getIdx());
                str.append("\n").append(objTemp->getTitle());
            }
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::FOOT, obj->getIdx()) == false )
            {
                auto objTemp = CostumeManager::getInstance()->getCostume(E_COSTUME::FOOT, obj->getIdx());
                str.append("\n").append(objTemp->getTitle());
            }

            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), str);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        int nCurrencyBong = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str());
        if ( obj->getPrice() > nCurrencyBong )
        {
            auto popup = PopupCurrencyDetail::create(E_ITEMS::MATERIAL);
            popup->show();
        }
        else
        {
            auto popup = PopupDefault::create(obj->getTitle(), GAME_TEXT("t_ui_costume_msg_30"));
            popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
                
                PopupLoading::show();
                CostumeManager::getInstance()->requestAuraBuy(eType, eIdx, CC_CALLBACK_2(UILayerCostumeSet::callbackBuy, this));
                
            });
            popup->show();
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

#pragma mark - callback
void UILayerCostumeSet::callbackBuy(bool bResult, int nResult)
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
