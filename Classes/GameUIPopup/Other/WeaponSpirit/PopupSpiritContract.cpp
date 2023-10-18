//
//  PopupSpiritContract.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/09.
//

#include "PopupSpiritContract.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/WeaponSpirit/PopupWeaponSpiritDefault.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"

#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/ItemsManager.h"

PopupSpiritContract* PopupSpiritContract::create()
{
    PopupSpiritContract* pRet = new(std::nothrow) PopupSpiritContract();
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

PopupSpiritContract::PopupSpiritContract(void):
_lyTopUI(nullptr),
_lyMainUI(nullptr),
_table(nullptr)
{
}

PopupSpiritContract::~PopupSpiritContract(void)
{
}


#pragma mark - table
void PopupSpiritContract::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupSpiritContract::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 170);
}

extension::TableViewCell* PopupSpiritContract::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    cell->setContentSize(size);
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBg->setContentSize(Size(size.width - 30, 160));
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBg);
    
    int nIdx = idx + 1;
    std::string strPath = MafUtils::format("Assets/ui/spirit/spirit_contract_1_%d.png", nIdx);
    
    auto sprIcon = Sprite::create(strPath);
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIcon->getTexture()->setTexParameters(texParams);
    sprIcon->setScale(1.8f);
    sprIcon->setPosition(15, sprBg->getContentSize().height/2);
    sprBg->addChild(sprIcon);
    
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_floor_" + std::to_string(nIdx));
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_sprit_contract_title_"), GAME_FONT, 23);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTitle->setPosition(sprIcon->getPosition().x + sprIcon->getContentSize().width * sprIcon->getScale() + 10, sprBg->getContentSize().height - 20);
    lbTitle->setDimensions(sprBg->getContentSize().width - sprIcon->getContentSize().width * sprIcon->getScale() - 200, lbTitle->getContentSize().height);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    sprBg->addChild(lbTitle);
    
    float nIncrease = 0;
    if(idx == 0)
    {
        nIncrease = 2.5f;
    }
    else if(idx == 1)
    {
        nIncrease = 4;
    }
    else if(idx == 2)
    {
        nIncrease = 4.5f;
    }
    else if(idx == 3)
    {
        nIncrease = 5.5f;
    }
    else if(idx == 4)
    {
        nIncrease = 9.5f;
    }
    
    auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_sprit_contract_contents_" + std::to_string(nIdx), objSetting->getValueToInt(), nIncrease), GAME_FONT, 23);
    lbText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbText->setPosition(lbTitle->getPosition() + Vec2::DOWN * (lbTitle->getContentSize().height + 5));
    lbText->setDimensions(sprBg->getContentSize().width - sprIcon->getContentSize().width * sprIcon->getScale() - 200, sprBg->getContentSize().height - lbTitle->getContentSize().height - 10);
    lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbText->setOverflow(Label::Overflow::SHRINK);
    sprBg->addChild(lbText);
    
    auto btnContract = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    btnContract->setScale9Enabled(true);
    btnContract->setContentSize(Size(150, 116));
    btnContract->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnContract->setPosition(Vec2(sprBg->getContentSize().width - 10, sprBg->getContentSize().height/2));
    btnContract->setZoomScale(0.05f);
    btnContract->setTag((int)idx);
    btnContract->setEnabled(false);
    btnContract->addClickEventListener(CC_CALLBACK_1(PopupSpiritContract::onClickContract, this));
    sprBg->addChild(btnContract);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_sprit_contract_title"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnContract->getContentSize().width/2, btnContract->getContentSize().height - btnContract->getContentSize().height/4);
        lbText->setDimensions(btnContract->getContentSize().width - 10, btnContract->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnContract->addChild(lbText);
        
        auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_STON);
        auto sprCurrencyIcon = Sprite::create(item->getPath());
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCurrencyIcon->setPosition(btnContract->getContentSize().width/4, btnContract->getContentSize().height/4);
        btnContract->addChild(sprCurrencyIcon);
        
        objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_cost");
        int nCount = objSetting->getValueToInt();
        for(int i = 0; i < idx; ++i)
            nCount *= 2;
        
        auto lbCount = Label::createWithTTF(std::to_string(nCount), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnContract->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnContract->addChild(lbCount);
    }
    
    if(WeaponSpiritManager::getInstance()->getContractLv() >= nIdx)
    {
        btnContract->setVisible(false);
        
        auto sprFx = Sprite::create("Assets/ui/spirit/spirit_contract_2_0.png");
        sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprFx->getTexture()->setTexParameters(texParams);
        sprFx->setPosition(sprIcon->getContentSize()/2);
        sprIcon->addChild(sprFx);
    }
    
    if(WeaponSpiritManager::getInstance()->getContractLv() + 1 == nIdx && WeaponSpiritManager::getInstance()->isContractPossible(nIdx))
    {
        btnContract->setEnabled(true);
    }
    
    return cell;
}

ssize_t PopupSpiritContract::numberOfCellsInTableView(extension::TableView *table)
{
    int size = 5;
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_max_lv");
    if(objSetting != nullptr)
        size = objSetting->getValueToInt();
    
    return size;
}

#pragma mark - override

#pragma mark -init
bool PopupSpiritContract::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    initVar();
    initUI();
    
    return true;
}

void PopupSpiritContract::initVar()
{
    _spriteContainer->setContentSize(Size(742, 1200));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupSpiritContract::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_sprit_contract_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSpiritContract::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 215));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyMainUI = LayerColor::create();
    _lyMainUI->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - _lyTopUI->getContentSize().height - 85));
    _lyMainUI->setIgnoreAnchorPointForPosition(false);
    _lyMainUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMainUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyMainUI);
    
    
    uiTop();
    uiMain();
}

#pragma mark -ui
void PopupSpiritContract::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::SPIRIT_STON);

    auto uiCurrency = UICurrency::create(list, Size(100, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_lyTopUI->getContentSize().width / 2, _lyTopUI->getContentSize().height - 5));
    _lyTopUI->addChild(uiCurrency, E_ZORER::UI+1);
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setContentSize(Size(_lyTopUI->getContentSize().width - 40, 150));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBox->setPosition(uiCurrency->getPosition() + Vec2::DOWN * (uiCurrency->getContentSize().height + 5));
    _lyTopUI->addChild(sprBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_sprit_contract_msg"), GAME_FONT, 25);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprBox->getContentSize()/2);
        lbText->setDimensions(sprBox->getContentSize().width - 10, sprBox->getContentSize().height - 10);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        sprBox->addChild(lbText);
    }
    
    
}
void PopupSpiritContract::uiMain()
{
    _table = extension::TableView::create(this, Size(_lyMainUI->getContentSize().width, _lyMainUI->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyMainUI->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyMainUI->addChild(_table);
    _table->reloadData();
}

#pragma mark -utils
void PopupSpiritContract::onClickContract(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto idx = node->getTag();
    auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_cost");
    int nCount = objSetting->getValueToInt();
    for(int i = 0; i < idx; ++i)
        nCount *= 2;
    
    if(WeaponSpiritManager::getInstance()->getSpiritStonCount() < nCount)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        WeaponSpiritManager::getInstance()->requestContract(CC_CALLBACK_2(PopupSpiritContract::callbackContract, this));
    }
}
void PopupSpiritContract::onClickClose(Ref* sender)
{
    hide();
}


#pragma mark -callback
void PopupSpiritContract::callbackContract(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    //
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 0 )
        {

        }
        else if( nResult == 2 )
        {
            
        }
        else if( nResult == 3 )
        {
            
        }
        else if( nResult == 4 )
        {
            
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
    
    _table->reloadData();
}
