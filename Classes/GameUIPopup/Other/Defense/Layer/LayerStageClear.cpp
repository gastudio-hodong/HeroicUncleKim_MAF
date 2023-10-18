//
//  LayerStageClear.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/30.
//

#include "LayerStageClear.h"
#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "Common/ObjectPool/MafPool.h"

#include "GameObject/Grid/Grid.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/Defense/InfoDefenseFloor.h"
#include "GameObject/Defense/InfoDefenseMap.h"

#include "GameUnit/Defense/DFMonster.h"
#include "GameUnit/Defense/DFTrench.h"
#include "GameUnit/Defense/DFBattleManager.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "GameUIPopup/Other/Defense/Layer/LayerDefenseMain.h"
#include "GameUIPopup/Other/Defense/PopupDefenseTrenchSetting.h"
#include "GameUIPopup/Other/Defense/PopupDefenseObjectList.h"
#include "GameUIPopup/Other/Defense/Shop/PopupDefenseShopDefault.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPDefault.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"



USING_NS_CC;

LayerStageClear* LayerStageClear::create(const cocos2d::Size size)
{
    LayerStageClear* pRet = new(std::nothrow) LayerStageClear();
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

LayerStageClear::LayerStageClear(void):
_isPlay(false),
_lyBG(nullptr),
_lyMiddle(nullptr)
{
}

LayerStageClear::~LayerStageClear(void)
{
    DefenseManager::getInstance()->_onUpdateClear.Clear(this);
    DefenseManager::getInstance()->_onUpdateFail.Clear(this);
}

bool LayerStageClear::init(const cocos2d::Size size)
{
    if (!LayerColor::init())
    {
        return false;
    }
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    
    DefenseManager::getInstance()->_onUpdateClear += new MafFunction<void(void)>(CC_CALLBACK_0(LayerStageClear::onPlayClear, this), this);
    DefenseManager::getInstance()->_onUpdateFail += new MafFunction<void(void)>(CC_CALLBACK_0(LayerStageClear::onPlayFail, this), this);
    
    // init
    initVar();
    initUi();
    
    setVisible(false);
    return true;
}

#pragma mark - init
void LayerStageClear::initVar()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, getContentSize().width, getContentSize().height);
        if (_isPlay)
            return true;
        
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LayerStageClear::initUi()
{
    // UI
    _lyBG = LayerColor::create(Color4B::BLACK);
    _lyBG->setContentSize(Size(750, 1350));
    _lyBG->setIgnoreAnchorPointForPosition(false);
    _lyBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->setPosition(getContentSize()/2);
    _lyBG->setOpacity(0);
    addChild(_lyBG);
    
    // UI
    _lyMiddle = LayerColor::create();
    _lyMiddle->setContentSize(Size(750, 360));
    _lyMiddle->setIgnoreAnchorPointForPosition(false);
    _lyMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyMiddle->setPosition(getContentSize()/2);
    addChild(_lyMiddle);
}

#pragma mark - UI
void LayerStageClear::uiMiddle(bool isSuccess)
{
    _lyMiddle->removeAllChildren();
    
    std::string path = "";
    if(isSuccess)
        path = "Assets/ui/defense/defense_success%02d.png";
    else
        path = "Assets/ui/defense/defense_fail%02d.png";
    
    auto bg = Sprite::create(MafUtils::format(path.c_str(), 0));
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(_lyMiddle->getContentSize()/2);
    bg->setOpacity(0);
    _lyMiddle->addChild(bg);
    
    auto anim = Animation::create();
    for(int i = 0; i <= 3; ++i)
    {
        std::string strPath = MafUtils::format(path.c_str(), i);
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    anim->setDelayPerUnit(0.1f);
    auto animate = Animate::create(anim);
    auto seq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7f), FadeOut::create(0.3f), nullptr);
    bg->runAction(RepeatForever::create(animate));
    bg->runAction(RepeatForever::create(seq));
    
    
    std::string text = "";
    if(isSuccess)
    {
        text = GAME_TEXTFORMAT("t_ui_defense_success", (int)DefenseManager::getInstance()->getFailStage());
    }
    else
    {
        text = GAME_TEXT("t_ui_defense_fail");
    }
    
    auto lbText = Label::createWithTTF(text, GAME_FONT, 20);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbText->setPosition(_lyMiddle->getContentSize().width/2, 10);
    lbText->setDimensions(_lyMiddle->getContentSize().width - 10, lbText->getContentSize().height);
    lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
    lbText->setOverflow(Label::Overflow::SHRINK);
    lbText->setOpacity(0);
    lbText->setVisible(false);
    _lyMiddle->addChild(lbText);
    
    if(isSuccess)
    {
        int64_t highFloor = DefenseManager::getInstance()->getHighFloor();
        int serverUpdateInterval = 100;
        if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_floor_update_interval"))
            serverUpdateInterval = setting->getValueToInt();
        
        if(highFloor >= serverUpdateInterval && highFloor % serverUpdateInterval == 0)
            lbText->setVisible(true);
        
        int count = 0;
        if(auto data = DefenseManager::getInstance()->getFloorData(DefenseManager::getInstance()->getCurrentFloor() - 1))
        {
            auto itemList =ItemsManager::getInstance()->getConvertArray(data->getRewardRepeat());
            if(itemList.size() >= 1)
            {
                auto items = itemList.at(0);
                count = std::atoi(items->getCount().c_str());
            }
        }
        auto lbCount = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_msg_get_5", count), GAME_FONT, 25);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbCount->setPosition(lbText->getPosition() + Vec2::UP * (lbText->getContentSize().height));
        lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        lbCount->setOpacity(0);
        lbCount->setColor(Color3B(255,50,255));
        lbCount->enableBold();
        _lyMiddle->addChild(lbCount);
        
        auto textSeq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7f), FadeOut::create(0.3f), nullptr);
        lbCount->runAction(RepeatForever::create(textSeq));
        
        auto icon = Sprite::create("Assets/icon/icon_dimension.png");
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        icon->setPosition(lbCount->getPosition() + Vec2::LEFT * lbCount->getContentSize().width/2 + Vec2::UP * lbCount->getContentSize().height/2);
        icon->getTexture()->setTexParameters(texParams);
        icon->setOpacity(0);
        _lyMiddle->addChild(icon);
        
        auto iconSeq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7f), FadeOut::create(0.3f), nullptr);
        icon->runAction(RepeatForever::create(iconSeq));
    }
    else
    {
        lbText->setVisible(true);
        
        int count = DefenseManager::getInstance()->getFailRewardCount();
        
        auto lbCount = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_msg_get_5", count), GAME_FONT, 25);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbCount->setPosition(lbText->getPosition() + Vec2::UP * (lbText->getContentSize().height));
        lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        lbCount->setOpacity(0);
        lbCount->setColor(Color3B(255,50,255));
        lbCount->enableBold();
        _lyMiddle->addChild(lbCount);
        
        auto textSeq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7f), FadeOut::create(0.3f), nullptr);
        lbCount->runAction(RepeatForever::create(textSeq));
        
        auto icon = Sprite::create("Assets/icon/icon_dimension.png");
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        icon->setPosition(lbCount->getPosition() + Vec2::LEFT * lbCount->getContentSize().width/2 + Vec2::UP * lbCount->getContentSize().height/2);
        icon->getTexture()->setTexParameters(texParams);
        icon->setOpacity(0);
        _lyMiddle->addChild(icon);
        
        auto iconSeq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7f), FadeOut::create(0.3f), nullptr);
        icon->runAction(RepeatForever::create(iconSeq));
    }
    
    if(lbText->isVisible())
    {
        auto textSeq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(1.7f), FadeOut::create(0.3f), nullptr);
        lbText->runAction(RepeatForever::create(textSeq));
    }
}

void LayerStageClear::play(bool isSuccess)
{
    setVisible(true);
    uiMiddle(isSuccess);
    _isPlay = true;
    
    auto bgFadeIn = FadeTo::create(0.3f, 255);
    auto bgFadeOut = FadeOut::create(0.3f);
    
    auto onFinishFunc = CallFunc::create([=](){
        _onFinishedProduction.Invoke();
        setVisible(false);
        _isPlay = false;
    });
    
    auto bgSeq = Sequence::create(bgFadeIn, DelayTime::create(1.7f), bgFadeOut, nullptr);
    auto seq = Sequence::create(DelayTime::create(2.3f), onFinishFunc, nullptr);
    
    _lyBG->runAction(bgSeq);
    runAction(seq);
}
#pragma mark - Event
void LayerStageClear::onPlayClear()
{
    play(true);
}

void LayerStageClear::onPlayFail()
{
    play(false);
}
