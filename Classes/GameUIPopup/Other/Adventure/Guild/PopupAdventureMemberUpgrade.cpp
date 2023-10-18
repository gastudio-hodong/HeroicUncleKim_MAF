//
//  PopupPartyUpgrade.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/22.
//

#include "PopupAdventureMemberUpgrade.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBuy.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureUpgradeComplete.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupAdventureMemberUpgrade* PopupAdventureMemberUpgrade::create(const int nParty, const int nIdx)
{
    PopupAdventureMemberUpgrade* pRet = new(std::nothrow) PopupAdventureMemberUpgrade();
    if ( pRet && pRet->init(nParty, nIdx) )
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

PopupAdventureMemberUpgrade::PopupAdventureMemberUpgrade() :
_nIdx(0)

,_layerContainerContents(nullptr)
,_layerContainerMine(nullptr)
,_layerContainerButton(nullptr)

,_callbackUpgrade(nullptr)
{
    
}

PopupAdventureMemberUpgrade::~PopupAdventureMemberUpgrade(void)
{
    
}
bool PopupAdventureMemberUpgrade::init(const int nParty, const int nIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _nParty = nParty;
    _nIdx = nIdx;
    
    // init
    initUi();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}
#pragma mark - Override
void PopupAdventureMemberUpgrade::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureMemberUpgrade::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventureMemberUpgrade::initUi()
{
    Size size = Size(729, 1000);
    
    // size
    _spriteContainer->setContentSize(size);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, 340) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _spriteContainer->addChild(_layerContainerContents);
    
    _layerContainerMine = LayerColor::create();
    _layerContainerMine->setContentSize(Size(size.width - 50, 470) ); //570, 350
    _layerContainerMine->setIgnoreAnchorPointForPosition(false);
    _layerContainerMine->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerMine->setPosition(_layerContainerContents->getPosition() + Vec2::DOWN * _layerContainerContents->getContentSize().height);
    _spriteContainer->addChild(_layerContainerMine);

    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 200) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition(_spriteContainer->getContentSize().width/2, 15);
    _spriteContainer->addChild(_layerContainerButton);
    
      
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_crew_title_1"), GAME_FONT, 36);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setDimensions(labelTitle->getContentSize().width * 0.75, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureMemberUpgrade::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);

    //
    uiContents();
    uiMine();
    uiButton();
    
}
void PopupAdventureMemberUpgrade::uiContents(bool isLevelup)
{
    _layerContainerContents->removeAllChildren();
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nParty);
    auto objMember = objParty->getMember(_nIdx);
    auto objProperty = objMember->getInfoUpgradeAbility();
    auto objPropertyNext = objMember->getInfoUpgradeAbility(objMember->getCurrentLv() + 1);
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::POINT);

    auto uiCurrency = UICurrency::create(list, Size(_layerContainerContents->getContentSize().width - 30, 40));
    uiCurrency->setPosition(Vec2(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height-25));
    _layerContainerContents->addChild(uiCurrency, E_ZORER::UI+1);
    
    //
    auto sprIconBox = Sprite::create("Assets/ui_common/box_quest.png");
    sprIconBox->setPosition(_layerContainerContents->getContentSize().width/2 - 70, _layerContainerContents->getContentSize().height - 140);
    _layerContainerContents->addChild(sprIconBox);
    
    std::string strIconPath = "Assets/ui/adventure/icon_crew_%d_%d.png";
    if(objMember->getCurrentLv() == 0)
    {
        strIconPath = MafUtils::format(strIconPath.c_str(), objMember->getImageIdx(), 1);
    }
    else
    {
        strIconPath = MafUtils::format(strIconPath.c_str(), objMember->getImageIdx(), objMember->getImageGrade());
    }
    
    auto sprMember = Sprite::create(strIconPath);
    sprMember->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprMember->getTexture()->setTexParameters(texParams);
    sprMember->setPosition(sprIconBox->getContentSize()/2);
    sprMember->setScale(2);
    sprIconBox->addChild(sprMember);
    
    auto lbMember = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_member_%d", objMember->getIdx())), GAME_FONT, 24);
    lbMember->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMember->setPosition(sprIconBox->getPosition().x +sprIconBox->getContentSize().width/2 + 20, _layerContainerContents->getContentSize().height - 95);
    lbMember->setDimensions(_layerContainerContents->getContentSize().width - 20, lbMember->getContentSize().height);
    lbMember->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbMember->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbMember);
    
    auto lbMemberLv = Label::createWithTTF(MafUtils::format("Lv.%d", objMember->getCurrentLv()), GAME_FONT, 24);
    lbMemberLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMemberLv->setPosition(lbMember->getPosition() + Vec2::DOWN * 40);
    _layerContainerContents->addChild(lbMemberLv);
    if(isLevelup)
    {
        auto spriteEffect = Sprite::create("Assets/ui_common/ef_level_1.png");
        spriteEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        spriteEffect->setPosition(lbMemberLv->getContentSize() / 2);
        spriteEffect->setScale(0.7f);
        lbMemberLv->addChild(spriteEffect);
            
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.05f);
        animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_1.png");
        animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_2.png");
        animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_3.png");
        animation->addSpriteFrameWithFile("Assets/ui_common/ef_level_4.png");
            
        auto action1 = Animate::create(animation);
        auto action2 = RemoveSelf::create();
        auto seq = Sequence::create(action1, action2, NULL);
        spriteEffect->runAction(seq);
    }
    
    auto sprArrow = Sprite::create("Assets/ui_common/btn_shortcuts_4.png");
    sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprArrow->setPosition(lbMemberLv->getPosition() + Vec2::RIGHT * (lbMemberLv->getContentSize().width + 20) + Vec2::DOWN * lbMemberLv->getContentSize().height/2);
    _layerContainerContents->addChild(sprArrow);
    
    auto lbMemberNextLv = Label::createWithTTF(MafUtils::format("Lv.%d", objMember->getCurrentLv() + 1), GAME_FONT, 24);
    lbMemberNextLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMemberNextLv->setPosition(lbMemberLv->getPosition() + Vec2::RIGHT * (sprArrow->getContentSize().width + sprArrow->getContentSize().width + 55));
    lbMemberNextLv->setColor(Color3B(0,255,26));
    _layerContainerContents->addChild(lbMemberNextLv);
    
    auto spriteLine = Sprite::create("Assets/ui/mine/week_weaponline.png");
    spriteLine->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 250);
    _layerContainerContents->addChild(spriteLine);
    
    auto lbMemberContent = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_member_%d", objMember->getIdx())), GAME_FONT, 20);
    lbMemberContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbMemberContent->setPosition(spriteLine->getPosition() + Vec2::UP * 10);
    lbMemberContent->setDimensions(_layerContainerContents->getContentSize().width - 20, lbMemberContent->getContentSize().height);
    lbMemberContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
    lbMemberContent->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbMemberContent);
    
    double fCurrentEffect = 0;
    
    if(objProperty != nullptr)
        fCurrentEffect = objProperty->getEffect();
    
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)objMember->getType()));
    std::string strEffectTmp = " %s {#00FF1A:(+%s)}";
    
    if(objMember->getType() == E_ADVENTURE_PROPERTY_TYPE::PARTY_GEM_REWARD_INCREASE)
        strEffectTmp = " %s {#00FF1A:(%s)}";
    strEffect += MafUtils::format(strEffectTmp.c_str(),
                                 AdventureManager::getPropertyEffectText(objMember->getType(), fCurrentEffect).c_str(),
                                 AdventureManager::getPropertyEffectText(objMember->getType(), objPropertyNext->getEffect() - fCurrentEffect).c_str());
    
    auto textMemberEffect = ui::CText::create(strEffect, GAME_FONT, 23);
    textMemberEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textMemberEffect->setPosition(spriteLine->getPosition() + Vec2::DOWN * 10);
    textMemberEffect->setTextAreaSize(Size(_layerContainerContents->getContentSize().width - 20, _layerContainerContents->getContentSize().height));
    textMemberEffect->setTextVerticalAlignment(TextVAlignment::TOP);
    textMemberEffect->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textMemberEffect->setTextOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(textMemberEffect);
}

void PopupAdventureMemberUpgrade::uiMine()
{
    _layerContainerMine->removeAllChildren();
    
    //
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nParty);
    if(objParty == nullptr)
        return;
    
    auto objMember = objParty->getMember(_nIdx);
    if(objMember == nullptr)
        return;
    
    
    //
    ui::Scale9Sprite* spBox = ui::Scale9Sprite::create("Assets/ui_common/box.png");
    spBox->setContentSize(Size(570, 350));
    spBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spBox->setPosition(_layerContainerMine->getContentSize().width / 2, _layerContainerMine->getContentSize().height - 10);
    _layerContainerMine->addChild(spBox);
    {
        ui::Scale9Sprite* spBoxTitle = ui::Scale9Sprite::create(PIXEL);
        spBoxTitle->setContentSize(Size(566, 36));
        spBoxTitle->setColor(Color3B(40, 49, 58));
        spBoxTitle->setAnchorPoint(Vec2(0.5, 1));
        spBoxTitle->setPosition(spBox->getContentSize().width/2, spBox->getContentSize().height - 2);//680,570
        spBox->addChild(spBoxTitle);
        
        Label* lbBoxTitle = Label::createWithTTF(GAME_TEXT("t_ui_equip_legend_meterial"), GAME_FONT, 24);
        lbBoxTitle->setPosition(spBoxTitle->getContentSize()/2);
        lbBoxTitle->setColor(COLOR_TITLE);
        spBoxTitle->addChild(lbBoxTitle);
    }
    
    //112(448), 89(267)
    auto layerItems = Layer::create();
    layerItems->setContentSize(Size(448, 267));
    layerItems->setIgnoreAnchorPointForPosition(false);
    layerItems->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    layerItems->setPosition(_layerContainerMine->getContentSize().width / 2, _layerContainerMine->getContentSize().height - 50);
    _layerContainerMine->addChild(layerItems);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    layerItems->addChild(menu);
    
    for ( int i = 0; i < C_COUNT_MINE_MINERAL; i++ )
    {
        E_MINERAL mineral = (E_MINERAL)(i+1);
        // 112, 89
        int nCostIdx = (int)E_ITEMS::MINE_STONE_IRON + i;
        
        int paddingX = 5;
        int paddingY = 5;
        int drawCol = 4;
        
        int x = i % drawCol;
        int y = i / drawCol;
        double posX = x * 112 + x * paddingX;
        double posY = layerItems->getContentSize().height - y * 89 - y * paddingY;

        int nowCount = MineralManager::getInstance()->getCount(mineral);
        auto objProperty = objMember->getInfoUpgradeAbility(objMember->getCurrentLv()+1);
        if(objProperty == nullptr)
            break;
        auto objItems = objProperty->getInfoItems(nCostIdx);
        
        int needCount = 0;
        if(objItems != nullptr)
            needCount = std::atoi(objItems->getCount().c_str());
        
        double nPercent = 0;
        if ( nowCount > 0 && needCount > 0 )
        {
            nPercent = (double)nowCount / needCount * 100;
            if ( nPercent > 100 )
                nPercent = 100;
        }
        
        // item bg
        auto layerBG = Layer::create();
        layerBG->setContentSize(Size(112, 89));
        
        auto item = MafNode::MafMenuItemSprite::create(layerBG, nullptr, CC_CALLBACK_1(PopupAdventureMemberUpgrade::onClickMine, this));
        item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        item->setPosition(posX, posY);
        item->setTag(mineral);
        menu->addChild(item);
        {
            auto spriteIcon = Sprite::create(DataManager::GetMineMineralPath(mineral));
            spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            spriteIcon->setPosition(layerBG->getContentSize().width / 2, 24);
            layerBG->addChild(spriteIcon);

            auto spriteProgressBG = Sprite::create("Assets/ui/mine/week_weapon_bar1.png");
            spriteProgressBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            spriteProgressBG->setPosition(layerBG->getContentSize().width / 2, 0);
            spriteProgressBG->setScaleX(1.4);
            layerBG->addChild(spriteProgressBG);
            
            auto progress = ProgressTimer::create(Sprite::create("Assets/ui/mine/week_weapon_bar2.png"));
            progress->setPosition(spriteProgressBG->getContentSize()/2);
            progress->setMidpoint(Vec2(0,0.5));
            progress->setBarChangeRate(Vec2(1,0));
            progress->setType(ProgressTimer::Type::BAR);
            progress->setPercentage(nPercent);
            spriteProgressBG->addChild(progress);
            
            auto labelCount = Label::createWithTTF("-", GAME_FONT, 16);
            labelCount->setPosition(layerBG->getContentSize().width / 2, 12);
            layerBG->addChild(labelCount);
            if ( needCount > 0 )
            {
                std::string strExistCount = MafUtils::toString(nowCount);
                std::string strNeedCount = MafUtils::toString(needCount);
                
                labelCount->setString(MafUtils::format("%s/%s",  MafUtils::convertNumberToShort(strExistCount).c_str(), MafUtils::convertNumberToShort(strNeedCount).c_str()));
            }
        }
        
        if ( needCount != 0 && nowCount < needCount )
        {
            auto spriteEffect = Sprite::create("Assets/ui_common/box_item_1_3.png");
            spriteEffect->setPosition(layerBG->getContentSize().width / 2, layerBG->getContentSize().height / 2);
            layerBG->addChild(spriteEffect);
            
            auto spriteReddot = Sprite::create("Assets/ui/exclamation_mark_icon_2.png");
            spriteReddot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            spriteReddot->setPosition(0, layerBG->getContentSize().height);
            layerBG->addChild(spriteReddot);
        }
    }
}

void PopupAdventureMemberUpgrade::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nParty);
    auto objMember = objParty->getMember(_nIdx);
    auto objProperty = objMember->getInfoUpgradeAbility(objMember->getCurrentLv() + 1);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(0,0);
    _layerContainerButton->addChild(menu);
    
    std::string strUpgradePath = "Assets/ui_common/btn_upgrade_bg.png";
    
    if(!isUpgradeCondition())
        strUpgradePath = "Assets/ui_common/btn_upgrade_max.png";
    
    auto btnUpgrade = ui::Button::create(strUpgradePath);
    btnUpgrade->addClickEventListener(CC_CALLBACK_1(PopupAdventureMemberUpgrade::onClickUpgrade, this));
    btnUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnUpgrade->setPosition(Vec2(_layerContainerButton->getContentSize().width/2, 20));
    _layerContainerButton->addChild(btnUpgrade);
    {
        auto spriteGold = Sprite::create("Assets/icon/icon_point1.png");
        spriteGold->setPosition(35, btnUpgrade->getContentSize().height*0.70);
        btnUpgrade->addChild(spriteGold);
        
        auto spriteGem = Sprite::create("Assets/icon/icon_jewel.png");
        spriteGem->setPosition(35, btnUpgrade->getContentSize().height*0.30);
        btnUpgrade->addChild(spriteGem);
        
        int nPrice = 0;
        auto objItem = objProperty->getInfoItems((int)E_ITEMS::POINT);
        if(objItem != nullptr)
            nPrice = std::atoi(objItem->getCount().c_str());
        
        auto labelPoint = Label::createWithTTF(MafUtils::format("%d",nPrice), GAME_FONT, 25);
        labelPoint->setColor(COLOR_COIN);
        labelPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelPoint->setPosition(60, btnUpgrade->getContentSize().height*0.70);
        btnUpgrade->addChild(labelPoint);
        
        objItem = objProperty->getInfoItems((int)E_ITEMS::GEM);
        if(objItem != nullptr)
            nPrice = std::atoi(objItem->getCount().c_str());
        
        auto labelGem = Label::createWithTTF(MafUtils::format("%d",nPrice), GAME_FONT, 25);
        labelGem->setColor(COLOR_COIN);
        labelGem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelGem->setPosition(60, btnUpgrade->getContentSize().height*0.30);
        btnUpgrade->addChild(labelGem);
    }
    
    auto lbAutoSave = Label::createWithTTF(GAME_TEXT("t_ui_advt_upgrade_mag_1"), GAME_FONT, 22);
    lbAutoSave->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbAutoSave->setPosition(btnUpgrade->getPosition() + Vec2::UP * (btnUpgrade->getContentSize().height + 25));
    lbAutoSave->setDimensions(_layerContainerButton->getContentSize().width - 20, lbAutoSave->getContentSize().height);
    lbAutoSave->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
    lbAutoSave->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _layerContainerButton->addChild(lbAutoSave);
}

#pragma mark - draw
void PopupAdventureMemberUpgrade::drawMine()
{
    uiMine();
}

#pragma mark - click
void PopupAdventureMemberUpgrade::onClickClose(Ref* const sender)
{
    hide();
}
void PopupAdventureMemberUpgrade::onClickUpgrade(cocos2d::Ref* const pSender)
{
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nParty);
    if(objParty == nullptr)
    {
        //없는 Ofiice
        return;
    }
    
    auto objMember = objParty->getMember(_nIdx);
    
    auto objProperty = objMember->getInfoUpgradeAbility(objMember->getCurrentLv() + 1);
    
    if(objProperty == nullptr)
    {
        //Max 레벨
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_spirit_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
    
    AdventureManager::getInstance()->requestPartyMemberUpgrade(CC_CALLBACK_2(PopupAdventureMemberUpgrade::callbackUpgrade, this), _nParty, _nIdx);
}

void PopupAdventureMemberUpgrade::onClickMine(cocos2d::Ref* const pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto item = (MafNode::MafMenuItemSprite*)pSender;
    
    
    E_MINERAL eMineral = (E_MINERAL)item->getTag();
    E_CASH eCash = E_CASH::CASH_MINE_1;
    switch (eMineral) {
         case E_MINERAL::MINERAL_1:     eCash = E_CASH::CASH_MINE_1;    break;
         case E_MINERAL::MINERAL_2:     eCash = E_CASH::CASH_MINE_2;    break;
         case E_MINERAL::MINERAL_3:     eCash = E_CASH::CASH_MINE_3;    break;
         case E_MINERAL::MINERAL_4:     eCash = E_CASH::CASH_MINE_4;    break;
         case E_MINERAL::MINERAL_5:     eCash = E_CASH::CASH_MINE_5;    break;
         case E_MINERAL::MINERAL_6:     eCash = E_CASH::CASH_MINE_6;    break;
         case E_MINERAL::MINERAL_7_SAPPHIRE:        eCash = E_CASH::CASH_MINE_7;        break;
         case E_MINERAL::MINERAL_8_OPAL:            eCash = E_CASH::CASH_MINE_8;        break;
         case E_MINERAL::MINERAL_9_AQUA:            eCash = E_CASH::CASH_MINE_9;        break;
         case E_MINERAL::MINERAL_10_EMERALD:        eCash = E_CASH::CASH_MINE_10;       break;
         case E_MINERAL::MINERAL_11_AMETHYST:       eCash = E_CASH::CASH_MINE_11;       break;

         default:
             break;
     }
    
    auto objProduct = CashManager::getInstance()->getProduct(eCash);
    if ( objProduct == nullptr )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
    
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(eCash, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(eCash, CC_CALLBACK_4(PopupAdventureMemberUpgrade::callbackPurchase, this));
            });
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
        else
        {
            auto popup = PopupPurchaseBuy::create(eCash);
            popup->setCallbackResult(CC_CALLBACK_1(PopupAdventureMemberUpgrade::callbackPurchaseOk, this));
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        }
    });
}

#pragma mark - utils

void PopupAdventureMemberUpgrade::setCallbackUpgrade(const std::function<void(void)>& callback)
{
    _callbackUpgrade = callback;
}
bool PopupAdventureMemberUpgrade::isUpgradeCondition()
{
    bool result = true;
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nParty);
    auto objMember = objParty->getMember(_nIdx);
    auto objProperty = objMember->getInfoUpgradeAbility(objMember->getCurrentLv() + 1);
    
    for ( int i = 0; i < C_COUNT_MINE_MINERAL; i++ )
    {
        E_MINERAL mineral = (E_MINERAL)(i+1);
        int needCount = 0;
        
        if(objProperty == nullptr)
            continue;
        auto objItems = objProperty->getInfoItems((int)E_ITEMS::MINE_STONE_IRON + i);
        
        if(objItems != nullptr)
            needCount = std::atoi(objItems->getCount().c_str());
        
        if ( MineralManager::getInstance()->getCount(mineral) < needCount )
        {
            result = false;
            break;
        }
    }
    
    auto objItemCost = objProperty->getInfoItems((int)E_ITEMS::GEM);
    if(std::atoi(objItemCost->getCount().c_str()) > ItemsMoneyManager::getInstance()->getGem())
        result = false;
    
    objItemCost = objProperty->getInfoItems((int)E_ITEMS::POINT);
    if(std::atoi(objItemCost->getCount().c_str()) > ItemsMoneyManager::getInstance()->getPoint())
        result = false;
    
    return result;
}
#pragma mark - callback
void PopupAdventureMemberUpgrade::callbackUpgrade(const bool bResult, const int nResult)
{
    if(nResult == 1)
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestSave(CC_CALLBACK_1(PopupAdventureMemberUpgrade::callbackLevelUp, this));
    }
    else
    {
        std::string strMessageKey = "t_ui_error_30";
        if(nResult == 2)
            strMessageKey = "";
        else if( nResult == 3 )
        {
            strMessageKey = "t_ui_error_spirit_3";
        }
        else if( nResult == 4 )
        {
            strMessageKey = "";
        }
        else if(nResult == 5)
        {
            return;
        }
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
}

void PopupAdventureMemberUpgrade::callbackPurchaseOk(const int nIdx)
{
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(PopupAdventureMemberUpgrade::callbackPurchase, this));
}

void PopupAdventureMemberUpgrade::callbackPurchase(const MafGooglePlay::RESULT eResult, const int nResult, const int nIdx, const std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        
        //
        drawMine();
        
        
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 #이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    }
}

void PopupAdventureMemberUpgrade::callbackLevelUp(const bool bResult)
{
    PopupLoading::hide();
    
    if(_callbackUpgrade != nullptr)
        _callbackUpgrade();
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(_nParty);
    auto objMember = objParty->getMember(_nIdx);
    
    auto popup = PopupAdventureUpgradeComplete::create(objMember);
    popup->show();
    
    if(objMember->getCurrentLv() >= objMember->getMaxLv())
    {
        _callbackUpgrade = nullptr;
        hide();
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    
    AdventureManager::getInstance()->checkLocalCompleteUpgrade();
    
    uiContents(true);
    uiMine();
    uiButton();
}
