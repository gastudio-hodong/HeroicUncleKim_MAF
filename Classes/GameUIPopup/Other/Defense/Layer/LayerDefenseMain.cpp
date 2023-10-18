//
//  LayerDefenseMain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#include "LayerDefenseMain.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "Common/ObjectPool/MafPool.h"

#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseFloor.h"
#include "GameObject/Defense/InfoDefenseMap.h"

#include "GameUnit/Defense/DFMonster.h"
#include "GameUnit/Defense/DFTrench.h"
#include "GameUnit/Defense/DFBattleManager.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Common/PopupDefault.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "GameUIPopup/Other/Defense/Layer/LayerDefenseMain.h"
#include "GameUIPopup/Other/Defense/PopupDefenseTrenchSetting.h"
#include "GameUIPopup/Other/Defense/PopupDefenseObjectList.h"
#include "GameUIPopup/Other/Defense/Shop/PopupDefenseShopDefault.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPDefault.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPList.h"
#include "GameUIPopup/Other/Defense/Layer/LayerStageClear.h"
#include "GameUIPopup/Other/Chat/PopupChat.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/ChatManager.h"



USING_NS_CC;

LayerDefenseMain* LayerDefenseMain::create(const cocos2d::Size size)
{
    LayerDefenseMain* pRet = new(std::nothrow) LayerDefenseMain();
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

LayerDefenseMain::LayerDefenseMain(void):
_isManaAMPOn(false),
_battleManager(nullptr),
_lyTop(nullptr),
_sprFloor(nullptr),
_textFloor(nullptr),
_sprHighFloor(nullptr),
_textHighFloor(nullptr),
_sprMonsterCount(nullptr),
_textMonsterCount(nullptr),
_manaAMP(nullptr),
_sprBg(nullptr)
{
}

LayerDefenseMain::~LayerDefenseMain(void)
{
    DefenseManaAMPManager::getInstance()->_onServerInfo.Clear(this);
}

bool LayerDefenseMain::init(const cocos2d::Size size)
{
    if (!LayerColor::initWithColor(Color4B::BLACK))
    {
        return false;
    }
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    
    
    // init
    initVar();
    initUi();
    
    DefenseManager::getInstance()->_onUpdateFloor += new MafFunction<void(void)>(CC_CALLBACK_0(LayerDefenseMain::onUpdateFloor, this), this);
    DefenseManaAMPManager::getInstance()->_onServerInfo += new MafFunction<void(bool, int)>(CC_CALLBACK_2(LayerDefenseMain::onManaAMPUpdate, this), this);
    _battleManager = DFBattleManager::create();
    addChild(_battleManager);
    _battleManager->_onKillMonster += new MafFunction<void(void)>(CC_CALLBACK_0(LayerDefenseMain::uiMonsterCount, this), this);
    uiMonsterCount();
    
    return true;
}

#pragma mark - init
void LayerDefenseMain::initVar()
{
    
}

void LayerDefenseMain::initUi()
{
    _stageClearProduction = LayerStageClear::create(getContentSize());
    _stageClearProduction->setPosition(getContentSize().width/2, 0);
    _stageClearProduction->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    addChild(_stageClearProduction, 999);
    
    uiBG();
    uiFloor();
    uiMonsterCount();
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(getContentSize().width, 80));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyTop, 2);
    
    uiTop();
    uiManaAMP();
}

#pragma mark - UI
void LayerDefenseMain::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto bg = Sprite::create("Assets/ui/defense/Defense_bg_top_menubox.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    bg->setPosition(_lyTop->getContentSize().width/2, _lyTop->getContentSize().height);
    bg->getTexture()->setTexParameters(texParams);
    bg->setScale(3);
    _lyTop->addChild(bg);
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_x.png");
    btnClose->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnClose->setPosition(Vec2(_lyTop->getContentSize().width - 10, _lyTop->getContentSize().height - 10));
    btnClose->setZoomScale(0.02f);
    btnClose->addClickEventListener(CC_CALLBACK_1(LayerDefenseMain::onClickClose, this));
    _lyTop->addChild(btnClose);
    
    auto btnChat = ui::Button::create("Assets/ui/chatting/btn_chatting.png");
    btnChat->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnChat->setPosition(btnClose->getPosition() + Vec2::LEFT * (btnClose->getContentSize().width + 16));
    btnChat->setScale(0.5f);
    btnChat->addClickEventListener(CC_CALLBACK_1(LayerDefenseMain::onClickChat, this));
    _lyTop->addChild(btnChat);
    
    auto btnBackground = ui::Button::create("Assets/ui_common/btn_defense_mini.png");
    btnBackground->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnBackground->setPosition(btnChat->getPosition() + Vec2::LEFT * (btnChat->getContentSize().width/2 + 20));
    btnBackground->setZoomScale(0.02f);
    btnBackground->addClickEventListener(CC_CALLBACK_1(LayerDefenseMain::onClickBackground, this));
    _lyTop->addChild(btnBackground);
    
    auto btnHelp = ui::Button::create("Assets/ui_common/icon_info_1_3.png");
    btnHelp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnHelp->setPosition(Vec2(10, _lyTop->getContentSize().height - 10));
    btnHelp->setZoomScale(0.02f);
    btnHelp->addClickEventListener(CC_CALLBACK_1(LayerDefenseMain::onClickHelp, this));
    _lyTop->addChild(btnHelp);
    
    auto failStageIcon = Sprite::create("Assets/icon/icon_defense_camp.png");
    failStageIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    failStageIcon->setPosition(btnHelp->getPosition() + Vec2::RIGHT * (btnHelp->getContentSize().width + 10) + Vec2::UP * 11);
    _lyTop->addChild(failStageIcon);
    
    auto lbFailStage = MafNode::createBmFont(MafUtils::format("B%llu", DefenseManager::getInstance()->getFailStage()), GAME_BM_FONT_AL, 22);
    lbFailStage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbFailStage->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbFailStage->setPosition(failStageIcon->getPosition() + Vec2::RIGHT * failStageIcon->getContentSize().width + Vec2::DOWN * failStageIcon->getContentSize().height/2);
    _lyTop->addChild(lbFailStage);
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::DEFENSE_COIN);

    auto uiCurrency = UICurrency::create(list, Size(_lyTop->getContentSize().width/2, 40));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiCurrency->setPosition(btnHelp->getPosition() + Vec2::RIGHT * (failStageIcon->getContentSize().width + lbFailStage->getContentSize().width + 50));
    _lyTop->addChild(uiCurrency, E_ZORER::UI+1);
}

void LayerDefenseMain::uiBG()
{
    if(_sprBg == nullptr)
    {
        _sprBg = Sprite::create("Assets/ui/defense/Defense_bg.png");
        _sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _sprBg->setPosition(getContentSize()/2);
        _sprBg->setScale(3);
        addChild(_sprBg);
        
        auto cutLine = Sprite::create("Assets/ui/defense/Defense_cut_line.png");
        cutLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        cutLine->setPosition(Vec2(0,190));
        addChild(cutLine,1);
    }
   
    auto floorData = DefenseManager::getInstance()->getCurrentFloorData();
    if(floorData == nullptr)
        return;
    
    auto mapData = DefenseManager::getInstance()->getMapData(floorData->getMapNumber());
    if(mapData == nullptr)
        return;
    
    _sprBg->setTexture(mapData->getImageFilePath());
    _sprBg->getTexture()->setTexParameters(texParams);
}

void LayerDefenseMain::uiFloor()
{
    if(_sprFloor == nullptr)
    {
        _sprFloor = Sprite::create("Assets/ui/defense/Stage_b_box.png");
        _sprFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _sprFloor->setPosition(Vec2(getContentSize().width/2, getContentSize().height - 60));
        addChild(_sprFloor, 1);
    }
    if(_textFloor == nullptr)
    {
        _textFloor = ui::Text::create("", GAME_FONT, 30);
        _textFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textFloor->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        _textFloor->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        _textFloor->setPosition(Vec2(_sprFloor->getContentSize().width/2, 55));
        _textFloor->setTextAreaSize(Size(180, 60));
        _textFloor->setColor(Color3B(0,200,240));
        ((Label*)_textFloor->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        _sprFloor->addChild(_textFloor);
    }
    if(_sprHighFloor == nullptr)
    {
        _sprHighFloor = Sprite::create("Assets/ui/defense/Stage_b_box_best.png");
        _sprHighFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _sprHighFloor->setPosition(_sprFloor->getPosition() + Vec2::DOWN * (_sprFloor->getContentSize().height - 12));
        addChild(_sprHighFloor);
    }
    if(_textHighFloor == nullptr)
    {
        auto textBest = ui::Text::create("BEST", GAME_FONT, 22);
        textBest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        textBest->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        textBest->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        textBest->setPosition(Vec2(20, 33));
        textBest->setTextAreaSize(Size(40, textBest->getContentSize().height));
        textBest->setColor(Color3B(255,220,215));
        ((Label*)textBest->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        _sprHighFloor->addChild(textBest);
        
        _textHighFloor = ui::Text::create("", GAME_FONT, 22);
        _textHighFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _textHighFloor->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        _textHighFloor->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        _textHighFloor->setPosition(Vec2(_sprHighFloor->getContentSize().width - 25, 30));
        _textHighFloor->setTextAreaSize(Size(90, _textHighFloor->getContentSize().height));
        _textHighFloor->setColor(Color3B(255,200,35));
        ((Label*)_textHighFloor->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        _sprHighFloor->addChild(_textHighFloor);
    }
    _textFloor->setString(MafUtils::format("B%lld", DefenseManager::getInstance()->getCurrentFloor()));
    _textHighFloor->setString(MafUtils::format("B%lld", DefenseManager::getInstance()->getHighFloor()));
    
    //test
//    auto btnReset = ui::Button::create("Assets/ui/defense/Ui_bottom_btn_on.png");
//    btnReset->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
//    btnReset->setPosition(Vec2(getContentSize().width/2 - 10, getContentSize().height - 300));
//    btnReset->addClickEventListener([=](Ref*){
//        DefenseManager::getInstance()->testclear(true);
//    });
//    btnReset->setZoomScale(0.05f);
//    btnReset->setTitleText("리셋");
//    btnReset->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
//    btnReset->getTitleLabel()->setDimensions(btnReset->getContentSize().width - 10, btnReset->getContentSize().height - 10);
//    btnReset->setTitleFontSize(26);
//    btnReset->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
//    addChild(btnReset);
//    
//    auto btnTest = ui::Button::create("Assets/ui/defense/Ui_bottom_btn_on.png");
//    btnTest->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    btnTest->setPosition(Vec2(getContentSize().width/2 + 10, getContentSize().height - 300));
//    btnTest->addClickEventListener([=](Ref*){
//        DefenseManager::getInstance()->testclear();
//    });
//    btnTest->setZoomScale(0.05f);
//    btnTest->setTitleText("+100층");
//    btnTest->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
//    btnTest->getTitleLabel()->setDimensions(btnTest->getContentSize().width - 10, btnTest->getContentSize().height - 10);
//    btnTest->setTitleFontSize(26);
//    btnTest->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
//    addChild(btnTest);
    
}
void LayerDefenseMain::uiManaAMP()
{
    if(_manaAMP == nullptr)
    {
        //화살표 때문에 PopupDefenseDefault에 자식 추가
        _manaAMP = Sprite::create("Assets/ui/defense/Horse_power_amplifier_off.png");
        _manaAMP->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _manaAMP->setPosition(Vec2(1, getContentSize().height - 360));
        _manaAMP->getTexture()->setTexParameters(texParams);
        _manaAMP->setScale(3);
        addChild(_manaAMP);
        
        auto arrow = Sprite::create("Assets/ui/prison/arrow_1.png");
        arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        arrow->setPosition(_manaAMP->getPosition() + Vec2::RIGHT * (_manaAMP->getContentSize().width/2 * _manaAMP->getScale() + 16) + Vec2::UP * (_manaAMP->getContentSize().height * _manaAMP->getScale()));
        arrow->setName("ARROW");
        arrow->setVisible(false);
        addChild(arrow);
        
        auto anim = Animation::create();
        for(int i = 1; i <= 6; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/prison/arrow_%d.png", i);
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        anim->setDelayPerUnit(0.2f);
        auto animate = Animate::create(anim);
        arrow->runAction(RepeatForever::create(animate));
    }
    
    if(DefenseManager::getInstance()->getHighFloor() < 11)
        return;
    
    _manaAMP->stopAllActions();
    
    _manaAMP->setTexture("Assets/ui/defense/Horse_power_amplifier_00.png");
    
    auto anim = Animation::create();
    for(int i = 0; i <= 4; ++i)
    {
        std::string strPath = "";
        
        if(DefenseManaAMPManager::getInstance()->isReddotCondition())
            strPath = MafUtils::format("Assets/ui/defense/Horse_power_amplifier_white%02d.png", i);
        else
            strPath = MafUtils::format("Assets/ui/defense/Horse_power_amplifier_%02d.png", i);
            
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    anim->setDelayPerUnit(0.2f);
    auto animate = Animate::create(anim);
    auto seq = Sequence::create(animate, DelayTime::create(0.1f), nullptr);
    _manaAMP->runAction(RepeatForever::create(seq));
    
    if(_isManaAMPOn)
        return;
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _manaAMP->addChild(menu);
    
    auto layerBG = Layer::create();
    layerBG->setContentSize(_manaAMP->getContentSize());
    
    auto itemBG = MafNode::MafMenuItemSprite::create(layerBG, nullptr, CC_CALLBACK_1(LayerDefenseMain::onClickManaAMP, this));
    itemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemBG->setPosition(_manaAMP->getContentSize()/2);
    menu->addChild(itemBG);
    
    if(auto arrow = getChildByName("ARROW"))
    {
        arrow->setVisible(true);
    }
    _isManaAMPOn = true;
}

void LayerDefenseMain::uiMonsterCount()
{
    if(_sprMonsterCount == nullptr)
    {
        _sprMonsterCount = Sprite::create("Assets/ui/defense/Mon_number_box.png");
        _sprMonsterCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _sprMonsterCount->setPosition(Vec2(getContentSize().width - 90, getContentSize().height - 320));
        addChild(_sprMonsterCount);
    }
    if(_textMonsterCount == nullptr)
    {
        //MafNode::createBmFont(MafUtils::format("%d", trenchNumber), GAME_BM_FONT_AL, 22);
        _textMonsterCount = MafNode::createBmFont("", GAME_BM_FONT_AL, 36);
        _textMonsterCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textMonsterCount->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        _textMonsterCount->setPosition(Vec2(74, 62));
        _textMonsterCount->setDimensions(78, 36);
        _textMonsterCount->setColor(Color3B(255,0,0));
        ((Label*)_textFloor->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        _sprMonsterCount->addChild(_textMonsterCount);
    }
    if(_battleManager == nullptr)
        return;
    
    _textMonsterCount->setString(std::to_string(_battleManager->getCurrentStageMonsterLeftCount()));
}
#pragma mark - Click

void LayerDefenseMain::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(1, "t_ui_defense_info_title_%d", "t_ui_defense_info_msg_%d");
    popup->show();
}

void LayerDefenseMain::onClickManaAMP(Ref* sender)
{
    
    auto popup = PopupManaAMPList::create();
    popup->show();
}
void LayerDefenseMain::onClickClose(Ref* sender)
{
    _onClose.Invoke();
}
void LayerDefenseMain::onClickBackground(Ref* sender)
{
    DefenseManager::getInstance()->setIsBackground(true);
}
void LayerDefenseMain::onClickChat(Ref* sender)
{
    if ( ChatManager::getInstance()->isBan() == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_7"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupChat::create(E_CHAT_TYPE::NORMAL);
    popup->show();
}
#pragma mark - Event
void LayerDefenseMain::onUpdateFloor()
{
    uiTop();
    uiBG();
    uiFloor();
    uiMonsterCount();
    uiManaAMP();
}
void LayerDefenseMain::onManaAMPUpdate(bool bResult, int nResult)
{
    if(bResult)
        uiManaAMP();
}
