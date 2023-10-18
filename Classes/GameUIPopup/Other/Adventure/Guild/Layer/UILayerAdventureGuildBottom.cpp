//
//  UILayerAdventureGuildBottom.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#include "UILayerAdventureGuildBottom.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/InfoItems.h"
#include "GameObject/Adventure/InfoAdventureStage.h"
#include "GameObject/Adventure/InfoAdventureMember.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Adventure/Map/PopupAdventureWorldMap.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureFriend.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTimeReduction.h"
#include "GameUIPopup/Other/Adventure/PopupPartyMiniSpec.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Common/PopupButtonForceTutorial.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayerAdventureGuildBottom* UILayerAdventureGuildBottom::create(const cocos2d::Size size)
{
    UILayerAdventureGuildBottom* pRet = new(std::nothrow) UILayerAdventureGuildBottom();
    if ( pRet && pRet->init(size) )
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

UILayerAdventureGuildBottom::UILayerAdventureGuildBottom(void):
_lyTable(nullptr)
,_lyBottom(nullptr)
,_table(nullptr)
,_btnTutorial(nullptr)

,_callbackHide(nullptr)
{
}

UILayerAdventureGuildBottom::~UILayerAdventureGuildBottom(void)
{
}

bool UILayerAdventureGuildBottom::init(const cocos2d::Size size)
{
    if (!UILayerAdventureParent::init())
    {
        return false;
    }
    setContentSize(size);
    
    //
    
    
    RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE, this);
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerAdventureGuildBottom::initVar()
{
}

void UILayerAdventureGuildBottom::initUi()
{
    // top layer
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(getContentSize()/2);
    sprBox->setContentSize(getContentSize());
    addChild(sprBox);
    // content layer
    _lyTable = LayerColor::create();
    _lyTable->setContentSize( Size(700, getContentSize().height - 190) );
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTable->setPosition(getContentSize().width/2, getContentSize().height - 10);
    addChild(_lyTable);
    
    // Bottom layer
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize( Size(700, getContentSize().height - _lyTable->getContentSize().height - 10) );
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottom->setPosition( _lyTable->getPosition() + Vec2::DOWN * _lyTable->getContentSize().height);
    addChild(_lyBottom);
    //
    uiTable();
    uiBottom();
    
    if(AdventureManager::getInstance()->getTutorialStep() == 1)
    {
        showTutorial();
    }
}
#pragma mark - table
void UILayerAdventureGuildBottom::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UILayerAdventureGuildBottom::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 159 + 10);
}

extension::TableViewCell* UILayerAdventureGuildBottom::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    Size sizeLayout = Size(696, 159); //232 * 53(696 * 159)
    
    // set size
    cell->setContentSize(size);
    
    auto objParty = AdventureManager::getInstance()->getInfoParty((int)idx + 1);
    if(objParty == nullptr)
        return cell;
    
    enum class E_PARTY_STATE
    {
        SEAL,
        OPEN,
        RUNNING,
        REWARD
    };
    
    E_PARTY_STATE eType = E_PARTY_STATE::SEAL;
    std::string strBoxPath = "Assets/ui/adventure/adventure_main_list_lock.png";
    std::string strBtnPath = "Assets/ui_common/btn_upgrade_bg_2_max.png";
    const double decreseTimePer = AdventureManager::getInstance()->getEffectDecreseTime(objParty->getIdx(), true) * 0.01;
    auto listMember = objParty->getListMemeber();
    
    if(objParty->isOpen())
    {
        eType = E_PARTY_STATE::OPEN;
    }
    if(objParty->isRunning())
    {
        eType = E_PARTY_STATE::RUNNING;
        auto time = objParty->getEndTime() - UtilsDate::getInstance()->getTime();
        
        if(time < 0)
        {
            eType = E_PARTY_STATE::REWARD;
        }
    }
    
    switch (eType)
    {
        case E_PARTY_STATE::OPEN:
        {
            auto objMember = listMember.at(0);
            strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d.png", 1, objMember->getImageGrade());
            
            if(AdventureManager::getInstance()->getTutorialStep() == 1 || AdventureManager::getInstance()->getTicket() > 0)
                strBtnPath = "Assets/ui_common/btn_upgrade_bg_2.png";
        }break;
        case E_PARTY_STATE::RUNNING:
        {
            if(objParty->getStage() == 0)
                strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", objParty->getArea(), 1);
            else
                strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", objParty->getArea(), objParty->getStage());
        }break;
        case E_PARTY_STATE::REWARD:
        {
            if(objParty->getStage() == 0)
                strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", objParty->getArea(), 1);
            else
                strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", objParty->getArea(), objParty->getStage());
            
            strBtnPath = "Assets/ui_common/btn_weaponkeep.png";
        }break;
        default : break;
    }
    
    //
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(sizeLayout);
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    cell->addChild(uiLayout);
    
    auto sprBox = Sprite::create(strBoxPath);
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(sizeLayout.width / 2, sizeLayout.height / 2);
    sprBox->getTexture()->setTexParameters(texParams);
    sprBox->setScale(3);
    uiLayout->addChild(sprBox);
    
    if(eType == E_PARTY_STATE::OPEN)
    {
        Vec2 pos = Vec2(10, 2);
        for(int i = 1; i < listMember.size(); ++i)
        {
            auto objMember = listMember.at(i);
            if(objMember == nullptr || !objMember->isOpen())
                continue;
            
            std::string strPath = "Assets/ui/adventure/adventure_crew_%d_%d_1.png";
            auto sprMember = Sprite::create(MafUtils::format(strPath.c_str(), objMember->getImageIdx(), objMember->getImageGrade()));
            sprMember->setPosition(pos);
            sprMember->getTexture()->setTexParameters(texParams);
            sprMember->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprBox->addChild(sprMember);
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.25f);
                for ( int j = 1; j <= 2; j++ )
                {
                    auto sprFrame = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_crew_%d_%d_%d.png", objMember->getImageIdx(), objMember->getImageGrade(), j));
                    sprFrame->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(sprFrame->getSpriteFrame());
                }
                sprMember->runAction(RepeatForever::create(Animate::create(animation)));
            }
            pos += Vec2::RIGHT * (sprMember->getContentSize().width / 2 + 12);
        }
    }
    else if(eType == E_PARTY_STATE::SEAL)
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_sendslot_null"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbText->setPosition( 30, sizeLayout.height/2);
        lbText->setDimensions(sizeLayout.width - sizeLayout.width/2.5, lbText->getContentSize().height);
        lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
        lbText->setOverflow(Label::Overflow::SHRINK);
        uiLayout->addChild(lbText);
    }
    
    auto btnTitleBox = ui::Button::create("Assets/ui_common/base_text_2_7.png");
    btnTitleBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnTitleBox->setPosition(Vec2(3, sizeLayout.height -6));
    btnTitleBox->setTag(objParty->getIdx());
    uiLayout->addChild(btnTitleBox);
    {
        auto sprPresetIcon = Sprite::create(MafUtils::format("Assets/ui/adventure/btn_preset_1_%d.png", objParty->getPresetIdx()));
        sprPresetIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprPresetIcon->setPosition(5, btnTitleBox->getContentSize().height/2);
        btnTitleBox->addChild(sprPresetIcon);
        if(!objParty->isOpen())
            sprPresetIcon->setVisible(false);
        
        std::string strTitleText = GAME_TEXT("t_ui_advt_crew_title_1") + MafUtils::format(" %d", objParty->getIdx());
        if(eType == E_PARTY_STATE::RUNNING || eType == E_PARTY_STATE::REWARD)
        {
            strTitleText = GAME_TEXT(MafUtils::format("t_ui_advt_worldmap_place_%d_%d", objParty->getArea(), objParty->getStage()));
        }
        
        auto lbText = Label::createWithTTF(strTitleText, GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(sprPresetIcon->getPosition() + Vec2::RIGHT * (sprPresetIcon->getContentSize().width + 10));
        lbText->setDimensions(btnTitleBox->getContentSize().width - 40 - btnTitleBox->getContentSize().width, btnTitleBox->getContentSize().height - 10);
        lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnTitleBox->addChild(lbText);
    }
    
    if(eType != E_PARTY_STATE::SEAL)
        btnTitleBox->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickPartyMiniSpec, this));
    
    auto btnLunch = ui::Button::create(strBtnPath);
    btnLunch->setScale9Enabled(true);
    btnLunch->setContentSize(Size(btnLunch->getContentSize().width + 20, sprBox->getContentSize().height * sprBox->getScale() - 20));
    btnLunch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnLunch->setPosition(Vec2(sizeLayout.width - 10, sizeLayout.height/2));
    btnLunch->setZoomScale(0.05f);
    btnLunch->setTag((int)idx + 1);
    cell->addChild(btnLunch);
    {
        switch (eType)
        {
            case E_PARTY_STATE::SEAL:
            {
                auto sprSeal = Sprite::create("Assets/ui_common/tap_lock_2.png");
                sprSeal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprSeal->setPosition(btnLunch->getContentSize()/2);
                btnLunch->addChild(sprSeal);
            }break;
            case E_PARTY_STATE::OPEN:
            {
                btnLunch->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickLunch, this));
                
                auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_ready_bt"), GAME_FONT, 24);
                lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbText->setPosition( btnLunch->getContentSize()/2);
                lbText->setDimensions(btnLunch->getContentSize().width - 10, btnLunch->getContentSize().height/2);
                lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbText->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbText);
                
                auto sprCurrencyIcon = Sprite::create("Assets/icon/icon_adventure.png");
                sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprCurrencyIcon->setPosition(btnLunch->getContentSize().width/2.6f, btnLunch->getContentSize().height/4);
                btnLunch->addChild(sprCurrencyIcon);
                
                auto lbCount = Label::createWithTTF(MafUtils::format("- %d", 1), GAME_FONT, 26);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
                lbCount->setDimensions(100, btnLunch->getContentSize().height/2);
                lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
                lbCount->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbCount);
                
                if(AdventureManager::getInstance()->getTicket() > 0)
                {
                    auto sprEffect = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_box_on.png");
                    sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    sprEffect->setContentSize(btnLunch->getContentSize());
                    sprEffect->setPosition(btnLunch->getContentSize()/2);
                    btnLunch->addChild(sprEffect);

                    auto fadeOut = FadeOut::create(1.f);
                    auto fadeIn = FadeIn::create(1.f);
                    auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
                    sprEffect->runAction(RepeatForever::create(sequience));
                }
            }break;
            case E_PARTY_STATE::RUNNING:
            {
                btnLunch->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickTimeReduction, this));
                
                auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_ing_bt"), GAME_FONT, 24);
                lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbText->setPosition( Vec2(btnLunch->getContentSize()/2) + Vec2::UP * 10);
                lbText->setDimensions(btnLunch->getContentSize().width - 10, btnLunch->getContentSize().height/2 - 10);
                lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
                lbText->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbText);
                
                auto sprTimer = Sprite::create("Assets/ui/offline/icon_offline_time.png");
                sprTimer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                sprTimer->setScale(0.5f);
                sprTimer->setPosition(25, btnLunch->getContentSize().height/2 - 10);
                btnLunch->addChild(sprTimer);
                
                auto lbTimer = Label::createWithTTF("", GAME_FONT, 18);
                lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbTimer->setPosition( sprTimer->getPosition() + Vec2::DOWN * (sprTimer->getContentSize().height/2 * sprTimer->getScale()) + Vec2::RIGHT * (sprTimer->getContentSize().width * sprTimer->getScale() + 10));
                lbTimer->setDimensions(75, lbTimer->getContentSize().height);
                lbTimer->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
                lbTimer->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbTimer);
                if(decreseTimePer > 0)
                    lbTimer->setColor(Color3B(0,255,26));
                
                auto callFunc = CallFunc::create([&, lbTimer, objParty](){
                    auto time = objParty->getEndTime() - UtilsDate::getInstance()->getTime();
                    
                    if(time < 0)
                    {
                        lbTimer->stopAllActions();
                        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
                        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
                        return;
                    }
                    int hour = time / 3600;
                    int min = time % 3600 / 60;
                    int sec = time % 3600 % 60;
                    
                    if(hour < 0)
                        hour = 0;
                    if(min < 0)
                        min = 0;
                    if(sec < 0)
                        sec = 0;
                    
                    lbTimer->setString(MafUtils::format("%02d:%02d:%02d", hour, min, sec));
                });
                auto delayTime = DelayTime::create(1);
                auto seq = Sequence::create(callFunc, delayTime, nullptr);
                lbTimer->runAction(RepeatForever::create(seq));
            }break;
            case E_PARTY_STATE::REWARD:
            {
                btnLunch->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickReward, this));
                
                auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_reward_bt"), GAME_FONT, 24);
                lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbText->setPosition( btnLunch->getContentSize()/2);
                lbText->setDimensions(btnLunch->getContentSize().width - 10, btnLunch->getContentSize().height - 10);
                lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbText->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbText);
                
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                uiReddot->setPosition(10, btnLunch->getContentSize().height - 10);
                btnLunch->addChild(uiReddot);
                
                auto sprEffect = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_box_on.png");
                sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprEffect->setContentSize(btnLunch->getContentSize());
                sprEffect->setPosition(btnLunch->getContentSize()/2);
                btnLunch->addChild(sprEffect);

                auto fadeOut = FadeOut::create(1.f);
                auto fadeIn = FadeIn::create(1.f);
                auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
                sprEffect->runAction(RepeatForever::create(sequience));
            }break;
            default : break;
        }
    }
    
    switch (eType)
    {
        case E_PARTY_STATE::REWARD:
        case E_PARTY_STATE::RUNNING:
        {
            auto lyReward = LayerColor::create();
            lyReward->setContentSize( Size(sizeLayout.width - 180, 58));
            lyReward->setIgnoreAnchorPointForPosition(false);
            lyReward->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
            lyReward->setPosition(10, 2);
            uiLayout->addChild(lyReward);
            {
                auto layerScroll = ui::ScrollView::create();
                layerScroll->setBounceEnabled(false);
                layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
                layerScroll->setContentSize(lyReward->getContentSize());
                layerScroll->setIgnoreAnchorPointForPosition(false);
                layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                layerScroll->setPosition(Vec2(0, lyReward->getContentSize().height / 2));
                layerScroll->setSwallowTouches(false);
                lyReward->addChild(layerScroll);
                
                double innerWidth = 0;
                double innerHeight = layerScroll->getContentSize().height;
                
                auto listItems = objParty->getReward();
                if ( objParty->getExp() > 0 )
                {
                    auto objItemExp = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_EXP);
                    if ( objItemExp != nullptr )
                    {
                        objItemExp->setCount(objParty->getExp());
                        
                        listItems.insert(0, objItemExp);
                    }
                }
                
                for ( int i = 0; i < listItems.size(); i++ )
                {
                    auto objItem = listItems.at(i);
                    
                    double posX = 58 * i + 5 * i;
                    double posY = layerScroll->getContentSize().height / 2;
                    innerWidth = posX + 58;
                    
                    auto pathBoxBG = "Assets/ui_common/box_item.png";
                    if ( objItem->getIdx() == (int)E_ITEMS::ADVENTURE_EXP )
                    {
                        pathBoxBG = "Assets/ui_common/box_item_2_1.png";
                    }
                    
                    auto spriteItemBG = ui::Scale9Sprite::create(pathBoxBG);
                    spriteItemBG->setContentSize(Size(58,58));
                    spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    spriteItemBG->setPosition(posX, posY);
                    layerScroll->addChild(spriteItemBG);
                    
                    auto spriteItems = Sprite::create(objItem->getPath());
                    if ( spriteItems != nullptr )
                    {
                        switch((E_ITEMS)objItem->getIdx())
                        {
                            case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                            case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                        }
                        
                        spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                        spriteItems->getTexture()->setTexParameters(texParams);
                        spriteItemBG->addChild(spriteItems);
                    }
                    
                    std::string strItemsCount = objItem->getCount();
                    if ( objItem != nullptr && objItem->isString() == true )
                    {
                        MafUtils::convertNumberToShort(strItemsCount);
                    }

                    auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 16);
                    labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
                    labelCount->enableOutline(Color4B::BLACK, 2);
                    spriteItemBG->addChild(labelCount);
                }
                layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
            }
            
            auto lyRewardBoss = LayerColor::create();
            lyRewardBoss->setContentSize( Size(sizeLayout.width - 180, 46));
            lyRewardBoss->setIgnoreAnchorPointForPosition(false);
            lyRewardBoss->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
            lyRewardBoss->setPosition(10, 62);
            uiLayout->addChild(lyRewardBoss);
            {
                auto layerScroll = ui::ScrollView::create();
                layerScroll->setBounceEnabled(false);
                layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
                layerScroll->setContentSize(lyRewardBoss->getContentSize());
                layerScroll->setIgnoreAnchorPointForPosition(false);
                layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                layerScroll->setPosition(Vec2(0, lyRewardBoss->getContentSize().height / 2));
                layerScroll->setSwallowTouches(false);
                lyRewardBoss->addChild(layerScroll);
                
                double innerWidth = 0;
                double innerHeight = layerScroll->getContentSize().height;
                
                auto listItems = objParty->getRewardBoss();
                
                if ( AdventureManager::getInstance()->isAdventureBullet(objParty->getArea(), objParty->getIdx()) == true )
                {
                    if ( auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_BOSS_BULLET_ALL); objItem != nullptr )
                    {
                        objItem->setCount(1);
                        
                        listItems.pushBack(objItem);
                    }
                }
                
                for ( int i = 0; i < listItems.size(); i++ )
                {
                    auto objItem = listItems.at(i);
                    
                    double posX = 46 * i + 5 * i;
                    double posY = layerScroll->getContentSize().height / 2;
                    innerWidth = posX + 46;

                    auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
                    spriteItemBG->setContentSize(Size(46, 46));
                    spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    spriteItemBG->setPosition(posX, posY);
                    layerScroll->addChild(spriteItemBG);
                    
                    auto spriteItems = Sprite::create(objItem->getPath());
                    if ( spriteItems != nullptr )
                    {
                        spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 5);
                        spriteItems->getTexture()->setTexParameters(texParams);
                        spriteItemBG->addChild(spriteItems);
                    }
                    
                    std::string strItemsCount = objItem->getCount();
                    if ( objItem != nullptr && objItem->isString() == true )
                    {
                        MafUtils::convertNumberToShort(strItemsCount);
                    }

                    auto labelCount = ui::CText::create(strItemsCount, GAME_FONT, 14);
                    labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    labelCount->setPosition(Vec2(spriteItemBG->getContentSize().width / 2, 0));
                    labelCount->enableOutline(Color4B::BLACK, 2);
                    spriteItemBG->addChild(labelCount);
                }
                layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
            }
            
            int guildLevel = AdventureManager::getInstance()->getLevel();
            int guildLevelCondition = AdventureManager::getInstance()->getSetting("boss_open_map_lv")->getValueToInt();
            if ( guildLevel < guildLevelCondition )
            {
                lyRewardBoss->setVisible(false);
            }
            
        }break;
            
        default:
            break;
    }
    
    if(objParty->getIdx() == 1 && AdventureManager::getInstance()->getTutorialStep() == 1)
    {
        _btnTutorial = btnLunch;
    }
    
    return cell;
}

ssize_t UILayerAdventureGuildBottom::numberOfCellsInTableView(extension::TableView *table)
{
    
    return AdventureManager::getInstance()->getListInfoParty().size();
}
void UILayerAdventureGuildBottom::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(listData.size() <= 0)
    {
        refreshUI();
    }
    else
    {
        const int nInspectionCode = listData.at("inspection_code").asInt();
        if(nInspectionCode == 8)
        {
            refreshUI();
            return;
        }
        
        std::string strText = "";
        if( nInspectionCode == 7 )
        {
            strText = "t_ui_new_raid_error_1";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strText));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
    }
}
#pragma mark - override
void UILayerAdventureGuildBottom::refreshUI()
{
    uiBottom();
    _table->reloadData();
}
void UILayerAdventureGuildBottom::showButtonForceTutorial()
{
    if(_btnTutorial == nullptr)
        return;
    
    auto popup = PopupButtonForceTutorial::create();
    popup->setButton(_btnTutorial);
    if(AdventureManager::getInstance()->getTutorialStep() == 1)
        popup->setCallbackListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickLunch, this));
    popup->show();
    
    _btnTutorial = nullptr;
}
void UILayerAdventureGuildBottom::showTutorial()
{
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}
#pragma mark -
void UILayerAdventureGuildBottom::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark - ui
void UILayerAdventureGuildBottom::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyTable->getContentSize().width / 2, _lyTable->getContentSize().height);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTable->addChild(_table);
    
    //
    _table->reloadData();
}
void UILayerAdventureGuildBottom::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto listFriend = AdventureManager::getInstance()->getListInfoFriend();
    
    InfoAdventureStage* objStage = nullptr;
    InfoAdventureFriend* objFriend = nullptr;
    
    for(const auto obj : listFriend)
    {
        if(obj->getInfoStage() != nullptr)
        {
            objFriend = obj;
            objStage = obj->getInfoStage();
            break;
        }
    }
    
    if(objStage == nullptr)
        uiFriendDefault();
    else
        uiFriendRunning(objFriend);
}
void UILayerAdventureGuildBottom::uiFriendRunning(InfoAdventureFriend* const infoFriend)
{
    
    enum class E_PARTY_STATE
    {
        RUNNING,
        REWARD
    };
    InfoAdventureStage* objStage = infoFriend->getInfoStage();
    E_PARTY_STATE eType = E_PARTY_STATE::RUNNING;
    std::string strBoxPath = "Assets/ui/adventure/adventure_main_list_lock.png";
    std::string strBtnPath = "Assets/ui_common/btn_upgrade_bg_2_max.png";
    const double decreseTimePer = infoFriend->getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE) * 0.01;
    
    auto time = objStage->getEndTime() - UtilsDate::getInstance()->getTime();
        
    if(time < 0)
    {
        eType = E_PARTY_STATE::REWARD;
    }
    
    switch (eType)
    {
        case E_PARTY_STATE::RUNNING:
        {
            strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", objStage->getArea(), objStage->getIdx());
        }break;
        case E_PARTY_STATE::REWARD:
        {
            strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", objStage->getArea(), objStage->getIdx());
            strBtnPath = "Assets/ui_common/btn_weaponkeep.png";
        }break;
        default : break;
    }
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(696, 159));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(_lyBottom->getContentSize().width / 2, _lyBottom->getContentSize().height / 2));
    _lyBottom->addChild(uiLayout);
    
    auto sprBox = Sprite::create(strBoxPath);
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(uiLayout->getContentSize()/2);
    sprBox->getTexture()->setTexParameters(texParams);
    sprBox->setScale(3);
    uiLayout->addChild(sprBox);
    
    auto btnLunch = ui::Button::create(strBtnPath);
    btnLunch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnLunch->setPosition(Vec2(uiLayout->getContentSize().width - 10, uiLayout->getContentSize().height/2));
    btnLunch->setTag(infoFriend->getPartyIdx());
    btnLunch->setZoomScale(0.05f);
    uiLayout->addChild(btnLunch);
    {
        switch (eType)
        {
            case E_PARTY_STATE::RUNNING:
            {
                auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_ing_bt"), GAME_FONT, 24);
                lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbText->setPosition( Vec2(btnLunch->getContentSize()/2) + Vec2::UP * 10);
                lbText->setDimensions(btnLunch->getContentSize().width - 10, btnLunch->getContentSize().height/2 - 10);
                lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
                lbText->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbText);
                
                auto sprTimer = Sprite::create("Assets/ui/offline/icon_offline_time.png");
                sprTimer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                sprTimer->setScale(0.5f);
                sprTimer->setPosition(25, btnLunch->getContentSize().height/2 - 10);
                btnLunch->addChild(sprTimer);
                
                auto lbTimer = Label::createWithTTF("", GAME_FONT, 18);
                lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbTimer->setPosition( sprTimer->getPosition() + Vec2::DOWN * (sprTimer->getContentSize().height/2 * sprTimer->getScale()) + Vec2::RIGHT * (sprTimer->getContentSize().width * sprTimer->getScale() + 10));
                lbTimer->setDimensions(75, lbTimer->getContentSize().height);
                lbTimer->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
                lbTimer->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbTimer);
                if(decreseTimePer > 0)
                    lbTimer->setColor(Color3B(0,255,26));
                
                auto callFunc = CallFunc::create([&, lbTimer, objStage](){
                    auto time = objStage->getEndTime() - UtilsDate::getInstance()->getTime();
                    
                    if(time < 0)
                    {
                        lbTimer->stopAllActions();
                        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
                        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
                        return;
                    }
                    int hour = time / 3600;
                    int min = time % 3600 / 60;
                    int sec = time % 3600 % 60;
                    
                    if(hour < 0)
                        hour = 0;
                    if(min < 0)
                        min = 0;
                    if(sec < 0)
                        sec = 0;
                    
                    lbTimer->setString(MafUtils::format("%02d:%02d:%02d", hour, min, sec));
                });
                auto delayTime = DelayTime::create(1);
                auto seq = Sequence::create(callFunc, delayTime, nullptr);
                lbTimer->runAction(RepeatForever::create(seq));
            }break;
            case E_PARTY_STATE::REWARD:
            {
                btnLunch->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickFriendReward, this));
                
                auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_reward_bt"), GAME_FONT, 24);
                lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbText->setPosition( btnLunch->getContentSize()/2);
                lbText->setDimensions(btnLunch->getContentSize().width - 10, btnLunch->getContentSize().height - 10);
                lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbText->setOverflow(Label::Overflow::SHRINK);
                btnLunch->addChild(lbText);
            }break;
            default : break;
        }
    }
    switch (eType)
    {
        case E_PARTY_STATE::REWARD:
        case E_PARTY_STATE::RUNNING:
        {
            auto sprStageBox = Sprite::create("Assets/ui_common/base_text_2_7.png");
            sprStageBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprStageBox->setPosition(5, uiLayout->getContentSize().height - 6);
            uiLayout->addChild(sprStageBox);
            {
                auto lbStage = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_worldmap_place_%d_%d", objStage->getArea(), objStage->getIdx())), GAME_FONT, 26);
                lbStage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbStage->setPosition( 10, sprStageBox->getContentSize().height/2);
                lbStage->setDimensions(sprStageBox->getContentSize().width - 10, sprStageBox->getContentSize().height-5);
                lbStage->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
                lbStage->setOverflow(Label::Overflow::SHRINK);
                sprStageBox->addChild(lbStage);
            }
            
            auto lyReward = LayerColor::create();
            lyReward->setContentSize( Size(uiLayout->getContentSize().width - 180, 58));
            lyReward->setIgnoreAnchorPointForPosition(false);
            lyReward->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
            lyReward->setPosition(10, 2);
            uiLayout->addChild(lyReward);
            {
                auto layerScroll = ui::ScrollView::create();
                layerScroll->setBounceEnabled(false);
                layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
                layerScroll->setContentSize(lyReward->getContentSize());
                layerScroll->setIgnoreAnchorPointForPosition(false);
                layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                layerScroll->setPosition(Vec2(0, lyReward->getContentSize().height / 2));
                layerScroll->setSwallowTouches(false);
                lyReward->addChild(layerScroll);
                
                double innerWidth = 0;
                double innerHeight = layerScroll->getContentSize().height;
                
                auto listItems = objStage->getListReward();
                if ( objStage->getExp() > 0 )
                {
                    auto objItemExp = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_EXP);
                    if ( objItemExp != nullptr )
                    {
                        objItemExp->setCount(objStage->getExp());
                        
                        listItems.insert(0, objItemExp);
                    }
                }
                
                for ( int i = 0; i < listItems.size(); i++ )
                {
                    auto objItem = listItems.at(i);
                    
                    double posX = 58 * i + 5 * i;
                    double posY = layerScroll->getContentSize().height / 2;
                    innerWidth = posX + 58;
                    
                    auto pathBoxBG = "Assets/ui_common/box_item.png";
                    if ( objItem->getIdx() == (int)E_ITEMS::ADVENTURE_EXP )
                    {
                        pathBoxBG = "Assets/ui_common/box_item_2_1.png";
                    }
                    
                    auto spriteItemBG = ui::Scale9Sprite::create(pathBoxBG);
                    spriteItemBG->setContentSize(Size(58,58));
                    spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    spriteItemBG->setPosition(posX, posY);
                    layerScroll->addChild(spriteItemBG);
                    
                    auto spriteItems = Sprite::create(objItem->getPath());
                    if ( spriteItems != nullptr )
                    {
                        switch((E_ITEMS)objItem->getIdx())
                        {
                            case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                            case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                        }
                        
                        spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                        spriteItems->getTexture()->setTexParameters(texParams);
                        spriteItemBG->addChild(spriteItems);
                    }
                    
                    int nItemsCount = std::atoi(objItem->getCount().c_str())/2;
                    if(nItemsCount <= 0)
                        nItemsCount = 1;
                    
                    std::string strCount = std::to_string(nItemsCount);
                    if ( objItem != nullptr && objItem->isString() == true )
                    {
                        MafUtils::convertNumberToShort(strCount);
                    }

                    auto labelCount = Label::createWithTTF(strCount, GAME_FONT, 16);
                    labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
                    labelCount->enableOutline(Color4B::BLACK, 2);
                    spriteItemBG->addChild(labelCount);
                }
                layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
            }
            
            auto lyRewardBoss = LayerColor::create();
            lyRewardBoss->setContentSize( Size(uiLayout->getContentSize().width - 180, 46));
            lyRewardBoss->setIgnoreAnchorPointForPosition(false);
            lyRewardBoss->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
            lyRewardBoss->setPosition(10, 62);
            uiLayout->addChild(lyRewardBoss);
            {
                auto layerScroll = ui::ScrollView::create();
                layerScroll->setBounceEnabled(false);
                layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
                layerScroll->setContentSize(lyRewardBoss->getContentSize());
                layerScroll->setIgnoreAnchorPointForPosition(false);
                layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                layerScroll->setPosition(Vec2(0, lyRewardBoss->getContentSize().height / 2));
                layerScroll->setSwallowTouches(false);
                lyRewardBoss->addChild(layerScroll);
                
                double innerWidth = 0;
                double innerHeight = layerScroll->getContentSize().height;
                
                auto listItems = objStage->getListRewardBossPoint();
                
                if ( AdventureManager::getInstance()->isAdventureBullet(objStage->getArea(), objStage->getIdx()) == true )
                {
                    if ( auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_BOSS_BULLET_ALL); objItem != nullptr )
                    {
                        objItem->setCount(1);
                        
                        listItems.pushBack(objItem);
                    }
                }
                
                for ( int i = 0; i < listItems.size(); i++ )
                {
                    auto objItem = listItems.at(i);
                    
                    double posX = 46 * i + 5 * i;
                    double posY = layerScroll->getContentSize().height / 2;
                    innerWidth = posX + 46;

                    auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
                    spriteItemBG->setContentSize(Size(46, 46));
                    spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    spriteItemBG->setPosition(posX, posY);
                    layerScroll->addChild(spriteItemBG);
                    
                    auto spriteItems = Sprite::create(objItem->getPath());
                    if ( spriteItems != nullptr )
                    {
                        spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 5);
                        spriteItems->getTexture()->setTexParameters(texParams);
                        spriteItemBG->addChild(spriteItems);
                    }
                    
                    std::string strItemsCount = objItem->getCount();
                    if ( objItem != nullptr && objItem->isString() == true )
                    {
                        MafUtils::convertNumberToShort(strItemsCount);
                    }

                    auto labelCount = ui::CText::create(strItemsCount, GAME_FONT, 14);
                    labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    labelCount->setPosition(Vec2(spriteItemBG->getContentSize().width / 2, 0));
                    labelCount->enableOutline(Color4B::BLACK, 2);
                    spriteItemBG->addChild(labelCount);
                }
                layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
            }
            
            int guildLevel = AdventureManager::getInstance()->getLevel();
            int guildLevelCondition = AdventureManager::getInstance()->getSetting("boss_open_map_lv")->getValueToInt();
            if ( guildLevel < guildLevelCondition )
            {
                lyRewardBoss->setVisible(false);
            }
            
    
        }break;
            
        default:
            break;
    }
}
void UILayerAdventureGuildBottom::uiFriendDefault()
{
    auto sprBox = Sprite::create("Assets/ui/adventure/adventure_main_list_friend.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_lyBottom->getContentSize()/2);
    sprBox->getTexture()->setTexParameters(texParams);
    sprBox->setScale(3);
    _lyBottom->addChild(sprBox);
    
    auto sprTitleBox = Sprite::create("Assets/ui_common/base_text_2_7.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprTitleBox->setPosition(5, _lyBottom->getContentSize().height - 12);
    _lyBottom->addChild(sprTitleBox);
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_slot_friend"), GAME_FONT, 26);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbTitle->setPosition( 10, sprTitleBox->getContentSize().height/2);
        lbTitle->setDimensions(sprTitleBox->getContentSize().width - 50, sprTitleBox->getContentSize().height-5);
        lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        sprTitleBox->addChild(lbTitle);
    }
    
    auto btnLunch = ui::Button::create("Assets/ui_common/btn_weaponkeep_2.png");
    btnLunch->setScale9Enabled(true);
    btnLunch->setContentSize(Size(btnLunch->getContentSize().width + 20, sprBox->getContentSize().height * sprBox->getScale() - 20));
    btnLunch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnLunch->setPosition(Vec2(_lyBottom->getContentSize().width - 10, _lyBottom->getContentSize().height/2));
    btnLunch->setZoomScale(0.05f);
    _lyBottom->addChild(btnLunch);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_launch_ready_bt"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition( btnLunch->getContentSize()/2);
        lbText->setDimensions(btnLunch->getContentSize().width - 10, btnLunch->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnLunch->addChild(lbText);
        
        int nMaxCount = 3;
        auto objSetting = AdventureManager::getInstance()->getSetting("friend_daily_max_count");
        if(objSetting != nullptr)
            nMaxCount = objSetting->getValueToInt();
        
        auto lbCount = Label::createWithTTF(MafUtils::format("%d/%d", nMaxCount - AdventureManager::getInstance()->getDailyFriendCount(), nMaxCount), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbCount->setPosition(btnLunch->getContentSize().width/2, btnLunch->getContentSize().height/4);
        lbCount->setDimensions(100, btnLunch->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnLunch->addChild(lbCount);
        
        int nFriendOpenLv = -1;
        auto objSettingFriendLv = AdventureManager::getInstance()->getSetting("advt_open_lv_friend");
        if(objSettingFriendLv != nullptr)
            nFriendOpenLv = objSettingFriendLv->getValueToInt();
        
        if(nFriendOpenLv != -1 && AdventureManager::getInstance()->getLevel() >= nFriendOpenLv)
        {
            btnLunch->addClickEventListener(CC_CALLBACK_1(UILayerAdventureGuildBottom::onClickFriend, this));
        }
        else
        {
            btnLunch->loadTextureNormal("Assets/ui_common/btn_weaponkeep_off.png");
            btnLunch->addClickEventListener([=](Ref*){
                Vec2 posToast = _lyBottom->getParent()->convertToWorldSpace(_lyBottom->getPosition() + Vec2::DOWN * _lyBottom->getContentSize().height/2);
                
                PopupToast::showImmediately(GAME_TEXTFORMAT("t_ui_advt_arror_6", nFriendOpenLv), posToast.x, posToast.y);
                return;
            });
        }
    }
}
#pragma mark -Click
void UILayerAdventureGuildBottom::onClickLunch(Ref* const pSender)
{
    if(AdventureManager::getInstance()->getTicket() <= 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_worldmap_error_title_1"), GAME_TEXT("t_ui_advt_worldmap_error_mag_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
    auto node = (Node*)pSender;
    
    auto popup = PopupAdventureWorldMap::create(node->getTag());
    popup->show();
}

void UILayerAdventureGuildBottom::onClickReward(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(node->getTag());
    if(objParty == nullptr)
        return;
    
    PopupLoading::show();
    AdventureManager::getInstance()->requestAdventureReward(CC_CALLBACK_2(UILayerAdventureGuildBottom::callbackReward, this), node->getTag());
}
void UILayerAdventureGuildBottom::onClickFriendReward(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    PopupLoading::show();
    AdventureManager::getInstance()->requestAdventureReward(CC_CALLBACK_2(UILayerAdventureGuildBottom::callbackReward, this), node->getTag(), true);
}
void UILayerAdventureGuildBottom::onClickFriend(Ref* const pSender)
{
    int nMaxCount = 3;
    auto objSetting = AdventureManager::getInstance()->getSetting("friend_daily_max_count");
    if(objSetting != nullptr)
        nMaxCount = objSetting->getValueToInt();
    
    if(AdventureManager::getInstance()->getDailyFriendCount() >= nMaxCount)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_worldmap_error_title_1"), GAME_TEXT("t_ui_advt_friend_complete_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        return;
    }
    
    auto popup = PopupAdventureFriend::create();
    popup->show();
}

void UILayerAdventureGuildBottom::onClickTimeReduction(Ref* const pSender)
{
    auto node = (Node*)pSender;
    if(AdventureManager::getInstance()->getTutorialStep() < 6)
    {
        auto cell = node->getParent();
        Vec2 posToast = cell->getParent()->convertToWorldSpace(cell->getPosition () +  cell->getContentSize()/2);
        PopupToast::showImmediately(GAME_TEXT("t_ui_advt_error_7"), posToast.x, posToast.y);
        return;
    }
    
    auto popup = PopupAdventureTimeReduction::create(node->getTag());
    popup->setCallbackResult(CC_CALLBACK_2(UILayerAdventureGuildBottom::callbackTimeReduction, this));
    popup->show();
}
void UILayerAdventureGuildBottom::onClickPartyMiniSpec(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    auto popup = PopupPartyMiniSpec::create(node->getTag());
    popup->show();
}
#pragma mark-callback
void UILayerAdventureGuildBottom::callbackReward(const bool bResult, const int nResult)
{
    PopupLoading::hide();
    
    if(AdventureManager::getInstance()->getTutorialStep() == 2)
    {
        AdventureManager::getInstance()->setTutorialStep(3);
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](const bool bResult){
            PopupLoading::hide();
        });
    }
    
    if(nResult != 1 && nResult != 8)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 7 )
        {
            strMessageKey = "t_ui_advt_error_2";
        }
        else if( nResult == 4 )
        {
            strMessageKey = "t_ui_advt_error_5";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
        return;
    }
    //refreshUI();
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void UILayerAdventureGuildBottom::callbackTimeReduction(const bool bResult, const int nResult)
{
    PopupLoading::hide();
    
    if(AdventureManager::getInstance()->getTutorialStep() == 2)
    {
        AdventureManager::getInstance()->setTutorialStep(3);
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](const bool bResult){
            PopupLoading::hide();
        });
    }
    
    if(nResult != 1 && nResult != 8)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 7 )
        {
            strMessageKey = "t_ui_advt_error_2";
        }
        else if( nResult == 4 )
        {
            strMessageKey = "t_ui_error_31";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(_callbackHide != nullptr)
            _callbackHide();
        _callbackHide = nullptr;
        return;
    }
    refreshUI();
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
