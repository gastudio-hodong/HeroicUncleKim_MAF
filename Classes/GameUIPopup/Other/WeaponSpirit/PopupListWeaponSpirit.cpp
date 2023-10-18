//
//  PopupListWeaponSpirit.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/19.
//

#include "PopupListWeaponSpirit.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupLoadingSequentialUpgrade.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupWeaponSpiritDefault.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupSpiritContract.h"

#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/NewRaidManager.h"

PopupListWeaponSpirit* PopupListWeaponSpirit::create()
{
    PopupListWeaponSpirit* pRet = new(std::nothrow) PopupListWeaponSpirit();
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

PopupListWeaponSpirit::PopupListWeaponSpirit(void):
_isCloseCondition(false),
_nCurrentRankUpIdx(0),
_lyTopUI(nullptr),
_lySpirits(nullptr),
_lyBottomUI(nullptr),
_table(nullptr)
{
    _listInfoSpirit.clear();
}

PopupListWeaponSpirit::~PopupListWeaponSpirit(void)
{
    _listInfoSpirit.clear();
}


#pragma mark - table
void PopupListWeaponSpirit::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupListWeaponSpirit::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 350);
}

extension::TableViewCell* PopupListWeaponSpirit::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    // obj
    ssize_t nIdxStart = idx * 4;
    ssize_t nIdxEnd = nIdxStart + 4;
    
    if ( nIdxEnd > _listInfoSpirit.size() )
    {
        nIdxEnd = _listInfoSpirit.size();
    }
    auto sprTitleBox = Sprite::create("Assets/ui/spirit/spirit_bg2_1.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTitleBox->setPosition(cell->getContentSize().width/2, cell->getContentSize().height);
    cell->addChild(sprTitleBox);
    {
        auto objSpirit = _listInfoSpirit.at(nIdxStart);
        
        if(objSpirit != nullptr)
        {
            auto lbGrade = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_spirit_grade_%d", objSpirit->getGrade())), GAME_FONT, 24);
            lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbGrade->setPosition(65, sprTitleBox->getContentSize().height/2);
            lbGrade->setDimensions(120, sprTitleBox->getContentSize().height - 5);
            lbGrade->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbGrade->setOverflow(Label::Overflow::SHRINK);
            lbGrade->enableBold();
            sprTitleBox->addChild(lbGrade);
            
            std::string strEffect = std::to_string((int)WeaponSpiritManager::getInstance()->getSpiritSynergy(objSpirit->getGrade()));
            
            strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
            auto lbSynergy = Label::createWithTTF(GAME_TEXT("t_ui_spirit_synergy") + MafUtils::format(" : %s%%", strEffect.c_str()), GAME_FONT, 24);
            lbSynergy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbSynergy->setPosition(150, sprTitleBox->getContentSize().height/2);
            lbSynergy->setDimensions(300, sprTitleBox->getContentSize().height - 5);
            lbSynergy->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbSynergy->setOverflow(Label::Overflow::SHRINK);
            lbSynergy->setColor(Color3B(207, 170, 85));
            sprTitleBox->addChild(lbSynergy);
            
            auto list = WeaponSpiritManager::getInstance()->getListWeaponSpirit(objSpirit->getGrade());
            int nTotalLv = 0;
            int nTotalSmelting = 0;
            
            for(auto obj : list)
            {
                if(obj->isHas())
                {
                    nTotalLv += obj->getCurrentLv();
                    nTotalSmelting += obj->getCurrentSmelting();
                }
            }
            
            auto lbSmelting = Label::createWithTTF(MafUtils::format("%d", nTotalSmelting), GAME_FONT, 24);
            lbSmelting->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            lbSmelting->setPosition(sprTitleBox->getContentSize().width - 23, sprTitleBox->getContentSize().height/2);
            lbSmelting->setColor(COLOR_TITLE);
            sprTitleBox->addChild(lbSmelting);
            
            auto sprIcon = Sprite::create("Assets/ui_common/icon_star1_2.png");
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            sprIcon->setPosition(lbSmelting->getPosition() + Vec2::LEFT * (lbSmelting->getContentSize().width + 3));
            sprTitleBox->addChild(sprIcon);
            
            auto lbLv = Label::createWithTTF(MafUtils::format("Lv.%d", nTotalLv), GAME_FONT, 24);
            lbLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            lbLv->setPosition(sprIcon->getPosition() + Vec2::LEFT * (sprIcon->getContentSize().width + 20));
            lbLv->setColor(Color3B(207, 170, 85));
            sprTitleBox->addChild(lbLv);
        }
    }
    auto sprSpiritBox = Sprite::create("Assets/ui/spirit/spirit_bg2_2.png");
    sprSpiritBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprSpiritBox->setPosition(sprTitleBox->getPosition() + Vec2::DOWN * sprTitleBox->getContentSize().height);
    cell->addChild(sprSpiritBox);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    sprSpiritBox->addChild(menu);
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdx = (int)nIdxStart + i;
        
        auto objSpirit = _listInfoSpirit.at(nIdx);
        if ( objSpirit == nullptr )
        {
            continue;
        }
        
        double nPosX = 29 + 159 * i + 10 * i;
        double nPosY = sprSpiritBox->getContentSize().height/2 + 33;
        
        bool bMaximum = WeaponSpiritManager::getInstance()->getSpiritMaximum(objSpirit->getIdx());
        
        std::string strBoxPath = "Assets/ui/spirit/spirit_box1_%d.png";
        std::string strNameBoxPath = "Assets/ui/spirit/spirit_box2_%d.png";
        if(!objSpirit->isHas())
        {
            strBoxPath = "Assets/ui/spirit/spirit_box3_%d.png";
            strNameBoxPath = "Assets/ui/spirit/spirit_box4_%d.png";
        }
        auto itemBG = MafNode::MafMenuItemSprite::create(Sprite::create(MafUtils::format(strBoxPath.c_str(), objSpirit->getGrade())), nullptr, CC_CALLBACK_1(PopupListWeaponSpirit::onClickSpirit, this));
        itemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        itemBG->setPosition(nPosX, nPosY);
        itemBG->setTag(objSpirit->getIdx());
        menu->addChild(itemBG);
        
        if ( bMaximum == true )
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1);
            for ( int j = 1; j <= 4; j++ )
            {
                animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/spirit/spirit_box_full_%d_%d.png", objSpirit->getGrade(), j).c_str());
            }
            
            auto uiAni = Sprite::create(MafUtils::format("Assets/ui/spirit/spirit_box_full_%d_1.png", objSpirit->getGrade()));
            uiAni->setAnchorPoint(Vec2::ZERO);
            uiAni->setPosition(Vec2(0, 0));
            uiAni->stopAllActions();
            uiAni->runAction(RepeatForever::create(Animate::create(animation)));
            itemBG->addChild(uiAni);
        }
     
        //
        auto spriteSpirit = Sprite::create(objSpirit->getResourcePath());
        spriteSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        spriteSpirit->setPosition(itemBG->getContentSize()/2);
        itemBG->addChild(spriteSpirit);
        if ( bMaximum == true && objSpirit->getGrade() >= 6 )
        {
            auto animation2 = Animation::create();
            animation2->setDelayPerUnit(0.2);
            for ( int k = 1; k <= 8; k++ )
            {
                animation2->addSpriteFrameWithFile(MafUtils::format("Assets/ui/spirit/spirit_box_full_fx_1_%d.png", k));
            }
            
            auto uiAni2 = Sprite::create("Assets/ui/spirit/spirit_box_full_fx_1_1.png");
            uiAni2->setAnchorPoint(Vec2::ZERO);
            uiAni2->setPosition(Vec2(0, 0));
            uiAni2->stopAllActions();
            uiAni2->runAction(RepeatForever::create(Animate::create(animation2)));
            itemBG->addChild(uiAni2);
        }
        
        
        auto sprNameBox = Sprite::create(MafUtils::format(strNameBoxPath.c_str(), objSpirit->getGrade()));
        sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprNameBox->setPosition(itemBG->getContentSize().width/2, 0);
        itemBG->addChild(sprNameBox);
        
        auto lbName = Label::createWithTTF(objSpirit->getText(), GAME_FONT, 25);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbName->setPosition(sprNameBox->getContentSize().width/2, sprNameBox->getContentSize().height - 5);
        lbName->setDimensions(70, sprNameBox->getContentSize().height/2);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setOverflow(Label::Overflow::SHRINK);
        lbName->enableBold();
        sprNameBox->addChild(lbName);
        
        auto lbCount = Label::createWithTTF(std::to_string(objSpirit->getCurrentCount()), GAME_FONT, 25);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbCount->setPosition(sprNameBox->getContentSize().width/2, 5);
        lbCount->setDimensions(70, sprNameBox->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        lbCount->enableBold();
        sprNameBox->addChild(lbCount);
        
        if(!objSpirit->isHas())
        {
            spriteSpirit->setColor(Color3B(15, 15, 15));
            lbName->setString("???");
            lbCount->setVisible(false);
        }
        if(objSpirit->getCurrentCount() <= 0)
            lbCount->setVisible(false);
            
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
        uiReddot->setPosition(5, itemBG->getContentSize().height - 5);
        itemBG->addChild(uiReddot);
        uiReddot->setVisible(false);
        
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            auto uiReddot = (UIReddot*)sender;
            
            auto objRankUpSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_rank_up_cost");
            if((objSpirit->getIdx() != _listInfoSpirit.size() && objSpirit->getCurrentCount() / objRankUpSetting->getValueToInt() >= 1) ||
               (!WeaponSpiritManager::getInstance()->isFirstSummon(objSpirit->getIdx() - 1) && objSpirit->isHas()))
            {
                uiReddot->setVisible(true);
            }
            else
            {
                uiReddot->setVisible(false);
            }
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
        
        
        
        if(objSpirit->isWear())
        {
            auto sprEffect = Sprite::create("Assets/ui/spirit/spirit_box0_1.png");
            sprEffect->getTexture()->setTexParameters(texParams);
            sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            sprEffect->setPosition(itemBG->getContentSize().width/2, itemBG->getContentSize().height + 5);
            itemBG->addChild(sprEffect);
            auto fadeOut = FadeOut::create(1.f);
            auto fadeIn = FadeIn::create(1.f);
            auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
            sprEffect->runAction(RepeatForever::create(sequience));
        }
        
        auto sprSmeltingBox = ui::ImageView::create("Assets/ui_common/btn_2_6_on.png");
        sprSmeltingBox->setScale9Enabled(true);
        sprSmeltingBox->setContentSize(Size(70, 46));
        sprSmeltingBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprSmeltingBox->setPosition(Vec2(itemBG->getContentSize().width, itemBG->getContentSize().height - 15));
        itemBG->addChild(sprSmeltingBox);
        {
            if ( bMaximum == true )
            {
                sprSmeltingBox->loadTexture("Assets/ui_common/btn_2_8_on.png");
            }
            
            auto sprIcon = Sprite::create("Assets/ui_common/icon_star1_2.png");
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprIcon->setPosition(5, sprSmeltingBox->getContentSize().height/2);
            sprSmeltingBox->addChild(sprIcon);
            
            auto lbCount = Label::createWithTTF(MafUtils::format("%02d",objSpirit->getCurrentSmelting()), GAME_FONT, 25);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbCount->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width + 3));
            lbCount->setDimensions(sprSmeltingBox->getContentSize().width - 5 - sprIcon->getContentSize().width - 8, sprSmeltingBox->getContentSize().height);
            lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbCount->setOverflow(Label::Overflow::SHRINK);
            lbCount->enableBold();
            lbCount->setColor(COLOR_COIN);
            sprSmeltingBox->addChild(lbCount);
            
            
        }
        
        auto sprLevelBox = Sprite::create("Assets/ui/spirit/spirit_lv_box1_1.png");
        sprLevelBox->setContentSize(Size(55, 35));
        sprLevelBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprLevelBox->setPosition(itemBG->getContentSize().width/2, 0);
        itemBG->addChild(sprLevelBox);
        {
            auto lbCount = Label::createWithTTF(MafUtils::format("Lv.%02d",objSpirit->getCurrentLv()), GAME_FONT, 25);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbCount->setPosition(sprLevelBox->getContentSize()/2);
            lbCount->setDimensions(sprLevelBox->getContentSize().width - 10, sprLevelBox->getContentSize().height);
            lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbCount->setOverflow(Label::Overflow::SHRINK);
            sprLevelBox->addChild(lbCount);
        }
    }
    
    return cell;
}

ssize_t PopupListWeaponSpirit::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _listInfoSpirit.size()/4;
    if ( _listInfoSpirit.size() % 4 != 0 )
        size++;
    return size;
}

#pragma mark - override
void PopupListWeaponSpirit::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
    uiTop();
    uiBottom();
}
void PopupListWeaponSpirit::hide()
{
    auto list = WeaponSpiritManager::getInstance()->getListWeaponSpirit();
    for(auto obj : list)
    {
        WeaponSpiritManager::getInstance()->setFirstSummon(obj->getIdx() - 1);
    }
    
    PopupBase::hide();
}
#pragma mark -init
bool PopupListWeaponSpirit::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _listInfoSpirit = WeaponSpiritManager::getInstance()->getListWeaponSpirit();
    initVar();
    initUI();
    
    RefreshManager::getInstance()->addUI(E_REFRESH::SPIRIT, this);
    
    return true;
}

void PopupListWeaponSpirit::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    WeaponSpiritManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupListWeaponSpirit::callbackInfo, this));
}

void PopupListWeaponSpirit::initVar()
{
    _spriteContainer->setContentSize(Size(742, 1290));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupListWeaponSpirit::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_weapon_spirit_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupListWeaponSpirit::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 100));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lySpirits = LayerColor::create();
    _lySpirits->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 385));
    _lySpirits->setIgnoreAnchorPointForPosition(false);
    _lySpirits->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lySpirits->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lySpirits);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - _lySpirits->getContentSize().height - 10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lySpirits->getPosition() + Vec2::DOWN * _lySpirits->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiSpirit();
    uiBottom();
}

#pragma mark -ui
void PopupListWeaponSpirit::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto btnShop = ui::Button::create("Assets/ui/raid/raid_shop/raid_shop_shortcuts.png");
    btnShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnShop->setPosition(Vec2(0, _lyTopUI->getContentSize().height));
    btnShop->setZoomScale(0.05f);
    btnShop->addClickEventListener(CC_CALLBACK_1(PopupListWeaponSpirit::onClickShop, this));
    _lyTopUI->addChild(btnShop);
    
    
    auto btnContract = ui::Button::create("Assets/ui/spirit/spirit_contract_border_1.png");
    btnContract->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnContract->setPosition(Vec2(_lyTopUI->getContentSize()/2));
    btnContract->setZoomScale(0.05f);
    btnContract->addClickEventListener(CC_CALLBACK_1(PopupListWeaponSpirit::onClickContract, this));
    _lyTopUI->addChild(btnContract);
    {
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_contract_max_lv");
        float posX = 102;
        for(int i = 0; i < objSetting->getValueToInt(); ++i)
        {
            std::string strPath = "Assets/ui/spirit/spirit_contract_1_0.png";
            int lv = i+1;
            
            if(WeaponSpiritManager::getInstance()->getContractLv() >= lv)
                strPath = MafUtils::format("Assets/ui/spirit/spirit_contract_1_%d.png", lv);
            
            auto sprStone = Sprite::create(strPath);
            sprStone->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprStone->setPosition(posX, btnContract->getContentSize().height/2);
            btnContract->addChild(sprStone);
            {
                if(WeaponSpiritManager::getInstance()->getContractLv() >= lv)
                {
                    auto sprFx = Sprite::create("Assets/ui/spirit/spirit_contract_2_0.png");
                    sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    sprFx->setPosition(sprStone->getContentSize()/2);
                    sprStone->addChild(sprFx);
                }
            }
            
            if(WeaponSpiritManager::getInstance()->getContractLv() + 1 == lv && WeaponSpiritManager::getInstance()->isContractPossible(lv))
            {
                auto sprFx = Sprite::create("Assets/ui/spirit/spirit_contract_2_0.png");
                sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprFx->setPosition(sprStone->getContentSize()/2);
                sprStone->addChild(sprFx);
                
                auto fadeOut = FadeOut::create(1.f);
                auto fadeIn = FadeIn::create(1.f);
                auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
                sprFx->runAction(RepeatForever::create(sequience));
            }
                
            
            posX += sprStone->getContentSize().width + 5;
        }
    }
}
void PopupListWeaponSpirit::uiSpirit()
{
    _table = extension::TableView::create(this, Size(_lySpirits->getContentSize().width, _lySpirits->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lySpirits->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lySpirits->addChild(_table);
    _table->reloadData();
}

void PopupListWeaponSpirit::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_5.png");
    sprBox->setContentSize(Size(_lyBottomUI->getContentSize().width - 20, 214));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBox->setPosition(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height);
    _lyBottomUI->addChild(sprBox);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_total_stat_1"), GAME_FONT, 28);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(20, sprBox->getContentSize().height - 27.5) );
    sprBox->addChild(lbTitle);
    
    auto sprSynergyBox = Sprite::create("Assets/ui_common/base_text_2_6.png");
    sprSynergyBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprSynergyBox->setPosition(3, sprBox->getContentSize().height - 60);
    sprBox->addChild(sprSynergyBox);
    {
        auto btnInfo = ui::Button::create("Assets/ui_common/icon_info_1_2.png");
        btnInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        btnInfo->setPosition(Vec2(10, sprSynergyBox->getContentSize().height/2));
        btnInfo->setZoomScale(0.05f);
        btnInfo->addClickEventListener(CC_CALLBACK_1(PopupListWeaponSpirit::onClickSynergyInfo, this));
        sprSynergyBox->addChild(btnInfo);
        
        
        std::string strTotalSynergy = MafUtils::doubleToString(WeaponSpiritManager::getInstance()->getSpiritSynergyAll());
        MafUtils::convertNumberToShort(strTotalSynergy);
        
        auto lbTotalSynergy = Label::createWithTTF(GAME_TEXT("t_ui_spirit_synergy_total") + MafUtils::format(" %s%%", strTotalSynergy.c_str()), GAME_FONT, 24);
        lbTotalSynergy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTotalSynergy->setPosition(btnInfo->getPosition() + Vec2::RIGHT * (btnInfo->getContentSize().width + 5));
        lbTotalSynergy->setDimensions(sprSynergyBox->getContentSize().width - btnInfo->getContentSize().width, sprSynergyBox->getContentSize().height);
        lbTotalSynergy->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTotalSynergy->setOverflow(Label::Overflow::SHRINK);
        sprSynergyBox->addChild(lbTotalSynergy);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbTotalSynergy->addChild(menu);
            
            auto area = Layer::create();
            area->setContentSize(lbTotalSynergy->getContentSize());
            
            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = _lyBottomUI->getParent()->convertToWorldSpace(_lyBottomUI->getPosition());
                posToast.y -= _lyBottomUI->getContentSize().height / 4.f;
                
                std::string strEffect =  MafUtils::doubleToString(WeaponSpiritManager::getInstance()->getSpiritSynergyAll());
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbTotalSynergy->getContentSize()/2);
            menu->addChild(btn);
        }
    }
    
    double effectDmg = 0;
    double effectGold = 0;
    double effectKey = 0;
    
    for(auto obj : WeaponSpiritManager::getInstance()->getListWeaponSpirit())
    {
        if(!obj->isHas())
            continue;
        
        effectDmg += obj->getEffectDmg();
        effectGold += obj->getEffectGold();
        effectKey += obj->getEffectKey();
    }
    
    
    std::vector<std::pair<std::string, double>> listEffect = {
        std::make_pair(GAME_TEXT("t_ui_town_msg_2"), effectDmg),
        std::make_pair(GAME_TEXT("t_ui_town_msg_5"), effectGold),
        std::make_pair(GAME_TEXT("t_ui_town_msg_3"), effectKey)
    };
    
    for( int i = 0; i < listEffect.size(); i++)
    {
        auto obj = listEffect[i];
        
        double posY = (sprSynergyBox->getPosition().y - sprSynergyBox->getContentSize().height) - 5 * i - 25 * i;
        
        auto lbTitle = Label::createWithTTF(obj.first, GAME_FONT, 20);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTitle->setPosition(20, posY);
        sprBox->addChild(lbTitle);
        
        std::string strEffect = "%s%%";
        
        std::string strEffectTmp = MafUtils::doubleToString(obj.second);
        //strEffectTmp = MafUtils::addCommaToString(strEffectTmp, 3, ',');
        
        MafUtils::convertNumberToShort(strEffectTmp);
        strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());
        
        auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 20);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        lbEffect->setPosition(sprBox->getContentSize().width - 140, posY);
        lbEffect->setDimensions(sprBox->getContentSize().width/2 - 140, lbEffect->getContentSize().height);
        lbEffect->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
        lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbEffect->addChild(menu);
            
            auto area = Layer::create();
            area->setContentSize(lbEffect->getContentSize());
            
            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = _lyBottomUI->getParent()->convertToWorldSpace(_lyBottomUI->getPosition());
                posToast.y -= _lyBottomUI->getContentSize().height / 4.f;
                
                std::string strEffect = MafUtils::doubleToString(obj.second);
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbEffect->getContentSize()/2);
            menu->addChild(btn);
        }
        
        sprBox->addChild(lbEffect);
    }
    
    auto btnAllRankUp = ui::Button::create("Assets/ui_common/btn_upgrade_bg_1.png");
    btnAllRankUp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnAllRankUp->setPosition(Vec2(sprBox->getContentSize().width - 10, 25));
    btnAllRankUp->setZoomScale(0.05f);
    btnAllRankUp->addClickEventListener(CC_CALLBACK_1(PopupListWeaponSpirit::onClickAllRankUP, this));
    btnAllRankUp->setTitleText(GAME_TEXT("t_ui_spirit_rank_up_2"));
    btnAllRankUp->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnAllRankUp->getTitleLabel()->setDimensions(btnAllRankUp->getContentSize().width, btnAllRankUp->getContentSize().height);
    btnAllRankUp->setTitleFontSize(30);
    btnAllRankUp->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(btnAllRankUp);
}

#pragma mark -utils
void PopupListWeaponSpirit::onClickShop(Ref* sender)
{
    if(_isCloseCondition)
    {
        hide();
        return;
    }
    auto popup = PopupRaidShopDefault::create();
    popup->setCloseCondition(true);
    popup->show();
}
void PopupListWeaponSpirit::onClickContract(Ref* sender)
{
    auto popup = PopupSpiritContract::create();
    popup->show();
}

void PopupListWeaponSpirit::onClickClose(Ref* sender)
{
    hide();
}

void PopupListWeaponSpirit::onClickSpirit(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto popup = PopupWeaponSpiritDefault::create(WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(node->getTag()));
    popup->show();
}

void PopupListWeaponSpirit::onClickAllRankUP(Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_spirit_rank_up_mag_2"));
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref*){
        _nCurrentRankUpIdx = 0;
        PopupLoadingSequentialUpgrade::show();
        PopupLoadingSequentialUpgrade::setTextKey("t_ui_spirit_rank_up_mag_3");
        PopupLoadingSequentialUpgrade::setCurrentIdx(_nCurrentRankUpIdx + 1);
        requestAllRankUp();
    });
    popup->show();
    
}
void PopupListWeaponSpirit::onClickSynergyInfo(Ref* sender)
{
    double effectTotal = 0;
    
    double effectDmg = 0;
    double effectSynergy = WeaponSpiritManager::getInstance()->getSpiritSynergyAll();
    int effectMaximumCount = WeaponSpiritManager::getInstance()->getSpiritMaximumAll();
    
    
    for(auto obj : WeaponSpiritManager::getInstance()->getListWeaponSpirit())
    {
        if(!obj->isHas())
            continue;
        effectDmg += obj->getEffectDmg();
    }
    effectTotal = effectDmg + (effectDmg * effectSynergy * 0.01);
    effectTotal = effectTotal * std::pow(10, effectMaximumCount);
    
    
    std::string strSpiritOrigin = MafUtils::doubleToString(effectDmg);
    MafUtils::convertNumberToShort(strSpiritOrigin);
    
    std::string strSynergy = MafUtils::doubleToString(effectSynergy);
    MafUtils::convertNumberToShort(strSynergy);
    
    std::string strTotal = MafUtils::doubleToString(effectTotal);
    MafUtils::convertNumberToShort(strTotal);
    
    // 정령의 레벨과 제련의 수준에 따라 합산하여\n 정령의 공격력 효과가 강화됩니다.\n\n[정령 공격력]%s%%에\n[시너지 효과]%s%% 적용\n\n[최종 적용 공격력]%s%%
    // 정령의 레벨과 제련의 수준에 따라 합산하여\n 정령의 공격력 효과가 강화됩니다.\n\n[정령 공격력]%s%%에\n[시너지 효과]%s%% 적용\n[최대 성장 정령]%d종% 적용\n\n[최종 적용 공격력]%s%%
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_spirit_synergy_info2", strSpiritOrigin.c_str(), strSynergy.c_str(), effectMaximumCount, strTotal.c_str()));
    popup->setContentSize(Size(550, 350));
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->show();
}
void PopupListWeaponSpirit::setCloseCondition(bool value)
{
    _isCloseCondition = value;
}

void PopupListWeaponSpirit::requestAllRankUp()
{
    auto objRankUpSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_rank_up_cost");
    
    for(; _nCurrentRankUpIdx < _listInfoSpirit.size() -1; ++_nCurrentRankUpIdx)
    {
        auto obj = _listInfoSpirit.at(_nCurrentRankUpIdx);
        
        if(!obj->isHas())
            continue;
        
        int nMakeCount = obj->getCurrentCount() / objRankUpSetting->getValueToInt();
        
        if(nMakeCount > 0)
        {
            PopupLoadingSequentialUpgrade::setCurrentIdx(_nCurrentRankUpIdx + 1);
            WeaponSpiritManager::getInstance()->requestSpiritCompose(CC_CALLBACK_2(PopupListWeaponSpirit::callbackRankUp, this), obj->getIdx(), nMakeCount);
            return;
        }
    }
    
    if(_nCurrentRankUpIdx >= _listInfoSpirit.size() - 1)
    {
        PopupLoadingSequentialUpgrade::hide();
        auto offset = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(offset);
        uiTop();
        uiBottom();
    }
}
#pragma mark -callback
void PopupListWeaponSpirit::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(bResult == false)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 2)
            strMessageKey = "t_ui_cash_msg_9_2";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
    uiTop();
    uiBottom();
}

void PopupListWeaponSpirit::callbackRankUp(bool bResult, int nResult)
{
    
    if(bResult == false)
    {
        PopupLoading::hide();
        
        std::string strMessageKey = "t_ui_error_30";
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        auto offset = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(offset);
        uiTop();
        uiBottom();
        
        return;
    }
    auto func = CallFunc::create([=](){
        requestAllRankUp();
    });
    
    runAction(Sequence::create(DelayTime::create(0.01f), func, NULL));
}
