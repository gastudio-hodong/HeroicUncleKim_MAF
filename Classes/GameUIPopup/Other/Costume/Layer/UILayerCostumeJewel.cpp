//
//  UILayerCostumeJewel.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayerCostumeJewel.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/Costume/PopupCostumeRent.h"
#include "GameUIPopup/Other/Costume/PopupLegendMake.h"
#include "GameUIPopup/Other/Costume/PopupCostumeReview.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayerCostumeJewel* UILayerCostumeJewel::create(std::string strType, cocos2d::Size size)
{
    UILayerCostumeJewel* pRet = new(std::nothrow) UILayerCostumeJewel();
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

UILayerCostumeJewel::UILayerCostumeJewel(void) :
_strType(""),
_eDetailType(E_DETAIL_TYPE::DEFAULT),


_layerContainerType(nullptr),
_layerContainerList(nullptr),
_table(nullptr)
{
    
}

UILayerCostumeJewel::~UILayerCostumeJewel(void)
{
    
}

bool UILayerCostumeJewel::init(std::string strType, cocos2d::Size size)
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

void UILayerCostumeJewel::setContentSize(const Size & size)
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

void UILayerCostumeJewel::refreshList()
{
    setList();// 장신구에서만.
    drawList();
}

#pragma mark - table
void UILayerCostumeJewel::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerCostumeJewel::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 150);
}

extension::TableViewCell* UILayerCostumeJewel::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    // bg
    std::string strBGPath = "Assets/ui_common/list_bg_1.png";
    auto spriteBG = ui::Scale9Sprite::create(strBGPath, Rect::ZERO, Rect(0,130,10,10));
    spriteBG->setContentSize(Size(size.width - 10, size.height - 10));
    spriteBG->setPosition(size.width / 2, size.height / 2);
    spriteBG->setTag((int)idx);
    cell->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    //
    auto itemBox = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/box_quest.png"), Sprite::create("Assets/ui_common/box_quest.png"), nullptr, CC_CALLBACK_1(UILayerCostumeJewel::onClickReview, this));
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
    auto itemAction = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1_max.png"), CC_CALLBACK_1(UILayerCostumeJewel::onClickAction, this));
    itemAction->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemAction->setPosition(spriteBG->getContentSize().width - 4, spriteBG->getContentSize().height / 2);
    itemAction->setEnabled(true);
    itemAction->setTag((int)idx);
    menu->addChild(itemAction);
    {
        if ( obj->getIdx() >= E_COSTUME_IDX::IDX_HONOR_1001 )
        {
            //
            itemAction->setCallback(CC_CALLBACK_1(UILayerCostumeJewel::onClickActionHonor, this));
            
            //
            E_HONOR eHonor = (E_HONOR)obj->getIdx();
            int nLevel = UserInfoManager::getInstance()->getHonorLevel(eHonor);
            if ( nLevel >= DataManager::GetHonorMaxLevel(eHonor) )
            {
                itemAction->setEnabled(false);
                
                auto label = Label::createWithTTF("MAX", GAME_FONT, 24);
                label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height / 2);
                label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
                label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                label->setOverflow(Label::Overflow::SHRINK);
                label->setColor(COLOR_COIN);
                itemAction->addChild(label);
            }
            else
            {
                int nPrice = DataManager::GetHonorPrice(eHonor, nLevel);
                int nCurrencyHonor = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::HONOR).c_str());
                if ( nCurrencyHonor < nPrice )
                {
                    ((Sprite*)itemAction->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_bg_1_max.png");
                }
                
                //
                int effectNow = DataManager::GetHonorEffect(eHonor, nLevel);
                int effectNext = DataManager::GetHonorEffect(eHonor, nLevel+1);
                
                auto label = Label::createWithTTF(MafUtils::format("+%d", effectNext-effectNow), GAME_FONT, 24);
                label->setPosition(itemAction->getContentSize().width / 2, itemAction->getContentSize().height * 0.70);
                label->setDimensions(itemAction->getContentSize().width * 0.9, label->getContentSize().height);
                label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                label->setOverflow(Label::Overflow::SHRINK);
                label->setColor(COLOR_COIN);
                itemAction->addChild(label);
                
                auto spriteCurrencyIcon = Sprite::create("Assets/icon/menu_honor.png");
                spriteCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                spriteCurrencyIcon->setPosition(55, itemAction->getContentSize().height * 0.30);
                itemAction->addChild(spriteCurrencyIcon);
                
                auto labelCurrency = Label::createWithTTF(MafUtils::format("x%d", nPrice), GAME_FONT, 24);
                labelCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                labelCurrency->setPosition(45, itemAction->getContentSize().height * 0.30);
                labelCurrency->setColor(COLOR_COIN);
                itemAction->addChild(labelCurrency);
            }
        }
        else
        {
            if ( bExist == true )
            {// 보유 중
                itemAction->setEnabled(false);
                
                std::string strText = GAME_TEXT("t_ui_costume_equip_now");
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
    }
    
    auto itemInfo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1.png"), Sprite::create("Assets/ui_common/btn_upgrade_bg_1_max.png"), CC_CALLBACK_1(UILayerCostumeJewel::onClickInfo, this));
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
        auto itemDibs = MafNode::MafMenuItemSprite::create(Sprite::create(strDibsPath), Sprite::create(strDibsPath), nullptr, CC_CALLBACK_1(UILayerCostumeJewel::onClickDips, this));
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
        else if ( obj->getIdx() >= E_COSTUME_IDX::IDX_HONOR_1001 )
        {
            E_HONOR eHonor = (E_HONOR)obj->getIdx();
            int nLevel = UserInfoManager::getInstance()->getHonorLevel(eHonor);
            
            strTitle = MafUtils::format("%s LV. %d", obj->getTitle().c_str(), nLevel);
        }
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
        
        if ( obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND )
        {
            std::string strDesc = DataManager::GetCostumeLegendDesc(obj->getType(), obj->getIdx());

            int nCount = CostumeManager::getInstance()->getExistTypeCount(obj->getType());
            int nCountMax = DataManager::GetCostumeLegendTermsCount(obj->getType());
            
            auto labelDesc = Label::createWithTTF(strDesc, GAME_FONT, 18);
            labelDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            labelDesc->setPosition(0, spriteBG->getContentSize().height - 75);
            labelDesc->setLineSpacing(-5);
            labelDesc->setDimensions(layerInfo->getContentSize().width, 50);
            labelDesc->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
            labelDesc->setOverflow(Label::Overflow::SHRINK);
            layerInfo->addChild(labelDesc);
        }
        else
        {
            //
            std::string strDesc = obj->getDesc();
            if ( obj->getIdx() >= E_COSTUME_IDX::IDX_HONOR_1001 )
            {
                E_HONOR eHonor = (E_HONOR)obj->getIdx();
                int nLevel = UserInfoManager::getInstance()->getHonorLevel(eHonor);
                
                strDesc = MafUtils::format(obj->getDesc().c_str(), DataManager::GetHonorEffect(eHonor, nLevel));
            }
            
            auto labelDesc = Label::createWithTTF(strDesc, GAME_FONT, 18);
            labelDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            labelDesc->setPosition(0, spriteBG->getContentSize().height - 75);
            labelDesc->setLineSpacing(-5);
            labelDesc->setDimensions(layerInfo->getContentSize().width, 50);
            labelDesc->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
            labelDesc->setOverflow(Label::Overflow::SHRINK);
            layerInfo->addChild(labelDesc);
        }
    }
    
    
    return cell;
}

ssize_t UILayerCostumeJewel::numberOfCellsInTableView(extension::TableView *table)
{
    
    return _listCostumeTemp.size();
}

#pragma mark - init
void UILayerCostumeJewel::initVar()
{
    _listCostume = CostumeManager::getInstance()->getCostumeType(_strType);
    setList();
}

void UILayerCostumeJewel::initUi()
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
void UILayerCostumeJewel::uiDetailType()
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
         
         auto itemType = MafNode::MafMenuItemSprite::create(layer, nullptr, CC_CALLBACK_1(UILayerCostumeJewel::onClickDetailType, this));
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

void UILayerCostumeJewel::uiList()
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
void UILayerCostumeJewel::drawDetailType()
{
    uiDetailType();
}

void UILayerCostumeJewel::drawList()
{
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}

#pragma mark - set, get, add
void UILayerCostumeJewel::setList()
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
            else if ( _eDetailType == E_DETAIL_TYPE::EXIST  )
            {
                if ( obj->getIdx() >= E_COSTUME_IDX::IDX_HONOR_1001 )
                {
                    E_HONOR eHonor = (E_HONOR)(obj->getIdx());
                    if ( UserInfoManager::getInstance()->getHonorLevel(eHonor) > 0 )
                    {
                        _listCostumeTemp.pushBack(obj);
                    }
                }
                else if ( CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx()) == true )
                {
                    _listCostumeTemp.pushBack(obj);
                }
            }
            else if ( _eDetailType == E_DETAIL_TYPE::NONE  )
            {
                if ( obj->getIdx() >= E_COSTUME_IDX::IDX_HONOR_1001 )
                {
                    E_HONOR eHonor = (E_HONOR)(obj->getIdx());
                    if ( UserInfoManager::getInstance()->getHonorLevel(eHonor) <= 0 )
                    {
                        _listCostumeTemp.pushBack(obj);
                    }
                }
                else if (CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx()) == false )
                {
                    _listCostumeTemp.pushBack(obj);
                }
                
            }
            else if ( _eDetailType == E_DETAIL_TYPE::DIBS && obj->isDips() == true )
            {
                _listCostumeTemp.pushBack(obj);
            }
        }
    }
 
    for (auto iter = _listCostumeTemp.rbegin(); iter != _listCostumeTemp.rend(); ++iter)
    {
        auto obj = *iter;
        
        if ( obj->getType() == E_COSTUME::JEWEL && obj->getIdx() == E_COSTUME_IDX::IDX_LEGEND )
        {
            if ( CostumeManager::getInstance()->isExistAll() == false )
            {
                _listCostumeTemp.eraseObject(obj);
            }
        }
        else if ( obj->getType() == E_COSTUME::JEWEL && obj->getIdx() == E_COSTUME_IDX::IDX_18 )
        {
            if( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18) == false )
            {
                _listCostumeTemp.eraseObject(obj);
            }
        }
        else if ( obj->getType() == E_COSTUME::JEWEL && obj->getIdx() == E_COSTUME_IDX::IDX_19 )
        {
            if( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18) == false )
            {
                _listCostumeTemp.eraseObject(obj);
            }
        }
        else if ( obj->getType() == E_COSTUME::JEWEL && obj->getIdx() == E_COSTUME_IDX::IDX_20 )
        {
            if( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_19) == false )
            {
                _listCostumeTemp.eraseObject(obj);
            }
        }
        else if ( obj->getType() == E_COSTUME::JEWEL && obj->getIdx() == E_COSTUME_IDX::IDX_21 )
        {
            if( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_20) == false )
            {
                _listCostumeTemp.eraseObject(obj);
            }
        }
        else if ( obj->getType() == E_COSTUME::JEWEL && obj->getIdx() == E_COSTUME_IDX::IDX_22 )
        {
            if( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_21) == false )
            {
                _listCostumeTemp.eraseObject(obj);
            }
        }
    }
    
}

#pragma mark - click
void UILayerCostumeJewel::onClickDetailType(Ref *sender)
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

void UILayerCostumeJewel::onClickReview(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    auto popup = PopupCostumeReview::create(obj->getType(), obj->getIdx());
    popup->show();
}

void UILayerCostumeJewel::onClickDips(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    bool bDips = obj->isDips();
    obj->setDips(!bDips);
    
    //
    drawList();
}

void UILayerCostumeJewel::onClickAction(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    E_COSTUME eType = obj->getType();
    E_COSTUME_IDX eIdx = obj->getIdx();
    
    bool bExist = CostumeManager::getInstance()->isExist(obj->getType(), obj->getIdx());
    if ( bExist == true )
    {
        return;
    }
    
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
                CostumeManager::getInstance()->requestBuy(eType, eIdx, CC_CALLBACK_2(UILayerCostumeJewel::callbackBuy, this));

            });
            popup->show();
        }
    }
}

void UILayerCostumeJewel::onClickActionHonor(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());
    
    E_HONOR eHonor = (E_HONOR)obj->getIdx();
    int nLevel = UserInfoManager::getInstance()->getHonorLevel(eHonor);
    int nLevelMax = DataManager::GetHonorMaxLevel(eHonor);
    
    if ( nLevel >= nLevelMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_msg_33"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int nPrice = DataManager::GetHonorPrice(eHonor, nLevel);
    int nCurrencyHonor = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::HONOR).c_str());
    if ( nPrice > nCurrencyHonor )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::HONOR);
        popup->show();
        return;
    }
    
    auto popup = PopupDefault::create(obj->getTitle(), GAME_TEXT("t_ui_costume_msg_32"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        PopupLoading::show();
        CostumeManager::getInstance()->requestHonorBuy(eHonor, nPrice, CC_CALLBACK_1(UILayerCostumeJewel::callbackBuyHonor, this));
    });
    popup->show();
}

void UILayerCostumeJewel::onClickInfo(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto obj = _listCostumeTemp.at(item->getTag());

    //
    auto popup = PopupLegendMake::create(obj->getType(), obj->getIdx());
    popup->setCallbackResult(CC_CALLBACK_1(UILayerCostumeJewel::callbackBuyLegend, this));
    popup->show();
}

#pragma mark - callback
void UILayerCostumeJewel::callbackBuy(bool bResult, int nResult)
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

void UILayerCostumeJewel::callbackBuyHonor(bool bResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawList();
}

void UILayerCostumeJewel::callbackBuyLegend(bool bResult)
{
    //
    drawList();
}
