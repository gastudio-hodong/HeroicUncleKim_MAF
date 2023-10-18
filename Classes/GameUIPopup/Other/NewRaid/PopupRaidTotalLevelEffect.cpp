//
//  PopupRaidTotalLevelEffect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/11.
//

#include "PopupRaidTotalLevelEffect.h"

#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

PopupRaidTotalLevelEffect* PopupRaidTotalLevelEffect::create()
{
    PopupRaidTotalLevelEffect* pRet = new(std::nothrow) PopupRaidTotalLevelEffect();
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

PopupRaidTotalLevelEffect::PopupRaidTotalLevelEffect(void) :
_lyTop(nullptr),
_lyTotalLevel(nullptr),
_lyEffect(nullptr)
{
}

PopupRaidTotalLevelEffect::~PopupRaidTotalLevelEffect(void)
{
}

#pragma mark -init

bool PopupRaidTotalLevelEffect::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupRaidTotalLevelEffect::initVar()
{
    //
    Size size = Size(500, 554);
    _spriteContainer->setContentSize( size );
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_stat_reward_info_title_1"), GAME_FONT, 30);
    
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 45) );
    _spriteContainer->addChild(labelTitle);
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_x.png");
    btnClose->setPosition(Vec2(_spriteContainer->getContentSize().width - btnClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    btnClose->setZoomScale(0.05f);
    btnClose->addClickEventListener([=](Ref*){
        hide();
    });
    _spriteContainer->addChild(btnClose);
    
    _lyTop = Layer::create();
    _lyTop->setContentSize(Size(size.width - 10, 100));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTop->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTop);
    
    _lyTotalLevel = Layer::create();
    _lyTotalLevel->setContentSize(Size(size.width - 10, 120));
    _lyTotalLevel->setIgnoreAnchorPointForPosition(false);
    _lyTotalLevel->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTotalLevel->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _spriteContainer->addChild(_lyTotalLevel);
    
    _lyEffect = Layer::create();
    _lyEffect->setContentSize(Size(size.width - 10, size.height - _lyTop->getContentSize().height - _lyTotalLevel->getContentSize().height - 85));
    _lyEffect->setIgnoreAnchorPointForPosition(false);
    _lyEffect->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyEffect->setPosition(_lyTotalLevel->getPosition() + Vec2::DOWN * _lyTotalLevel->getContentSize().height);
    _spriteContainer->addChild(_lyEffect);
}

void PopupRaidTotalLevelEffect::initUi()
{
    uiTop();
    uiTotalLevel();
    uiEffect();
}

#pragma mark - ui
void PopupRaidTotalLevelEffect::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_stat_reward_info_msg_1"), GAME_FONT, 24);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbText->setPosition(_lyTop->getContentSize()/2);
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(_lyTop->getContentSize().width, _lyTop->getContentSize().height);
    lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTop->addChild(lbText);
}

void PopupRaidTotalLevelEffect::uiTotalLevel()
{
    _lyTotalLevel->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_8.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setContentSize(Size(324, 82));
    sprBox->setPosition(_lyTotalLevel->getContentSize()/2);
    _lyTotalLevel->addChild(sprBox);
    
    int nTotalSP = 0;
    nTotalSP += NewRaidManager::getInstance()->getAbilityPoint();
    auto listAbility = NewRaidManager::getInstance()->getInfoBongStatList();
    for(auto obj : listAbility)
    {
        nTotalSP += obj->getCurrentLv();
    }
    
    auto lbNowSeasonLv = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_stat_reward_lv_now", nTotalSP), GAME_FONT, 26);
    lbNowSeasonLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbNowSeasonLv->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    lbNowSeasonLv->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbNowSeasonLv->setDimensions(sprBox->getContentSize().width - 10, sprBox->getContentSize().height/2);
    lbNowSeasonLv->setPosition(sprBox->getContentSize().width/2, sprBox->getContentSize().height);
    lbNowSeasonLv->setColor(COLOR_COIN);
    sprBox->addChild(lbNowSeasonLv);
    
    auto lbTotalSeasonLv = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_stat_reward_lv_stack", NewRaidManager::getInstance()->getTotalAbilityPoint()), GAME_FONT, 26);
    lbTotalSeasonLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbTotalSeasonLv->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
    lbTotalSeasonLv->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTotalSeasonLv->setDimensions(sprBox->getContentSize().width - 10, sprBox->getContentSize().height/2);
    lbTotalSeasonLv->setPosition(sprBox->getContentSize().width/2, 0);
    lbTotalSeasonLv->setColor(COLOR_COIN);
    sprBox->addChild(lbTotalSeasonLv);
    
}

void PopupRaidTotalLevelEffect::uiEffect()
{
    _lyEffect->removeAllChildren();
    
    auto sprTitleBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprTitleBox->setContentSize(Size(436,50));
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTitleBox->setPosition(_lyEffect->getContentSize().width/2, _lyEffect->getContentSize().height);
    _lyEffect->addChild(sprTitleBox);
    {
        auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_stat_reward_apply"), GAME_FONT, 26);
        lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbContent->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbContent->setDimensions(sprTitleBox->getContentSize().width - 10, sprTitleBox->getContentSize().height);
        lbContent->setPosition(sprTitleBox->getContentSize()/2);
        sprTitleBox->addChild(lbContent);
    }
//    t_ui_stat_reward_1    던전 공격력 : %d (%d)
//    t_ui_stat_reward_2    처치 시 골드 : %d (%d)
//    t_ui_stat_reward_3    환생 시 열쇠 : %d (%d)
//    t_ui_stat_reward_info_msg_2    *+된 능력치는 다음시즌에 적용됩니다.
    auto sprContentBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprContentBox->setContentSize(Size(436,178));
    sprContentBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprContentBox->setPosition(sprTitleBox->getPosition() + Vec2::DOWN * sprTitleBox->getContentSize().height);
    _lyEffect->addChild(sprContentBox);
    {
        auto lbDamage = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_stat_reward_1", (int)NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE), (int)NewRaidManager::getInstance()->getAccrueEffectNowSeason(E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE)), GAME_FONT, 22);
        lbDamage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbDamage->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
        lbDamage->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbDamage->setDimensions(sprTitleBox->getContentSize().width - 10, lbDamage->getContentSize().height);
        lbDamage->setPosition(sprContentBox->getContentSize().width/2, sprContentBox->getContentSize().height - 10);
        sprContentBox->addChild(lbDamage);
        
        auto lbGold = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_stat_reward_2", (int)NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::GOLD), (int)NewRaidManager::getInstance()->getAccrueEffectNowSeason(E_RAID_ACCRUE_EFFECT_TYPE::GOLD)), GAME_FONT, 22);
        lbGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbGold->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
        lbGold->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbGold->setDimensions(sprTitleBox->getContentSize().width - 10, lbGold->getContentSize().height);
        lbGold->setPosition(lbDamage->getPosition() + Vec2::DOWN * (lbDamage->getContentSize().height + 5));
        sprContentBox->addChild(lbGold);
        
        auto lbKey = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_stat_reward_3", (int)NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::KEY), (int)NewRaidManager::getInstance()->getAccrueEffectNowSeason(E_RAID_ACCRUE_EFFECT_TYPE::KEY)), GAME_FONT, 22);
        lbKey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbKey->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
        lbKey->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbKey->setDimensions(sprTitleBox->getContentSize().width - 10, lbKey->getContentSize().height);
        lbKey->setPosition(lbGold->getPosition() + Vec2::DOWN * (lbGold->getContentSize().height + 5));
        sprContentBox->addChild(lbKey);
        
        auto lbWarning = Label::createWithTTF(GAME_TEXT("t_ui_stat_reward_info_msg_2"), GAME_FONT, 22);
        lbWarning->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbWarning->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
        lbWarning->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbWarning->setColor(Color3B::GREEN);
        lbWarning->setDimensions(sprTitleBox->getContentSize().width - 10, lbWarning->getContentSize().height);
        lbWarning->setPosition(lbKey->getPosition() + Vec2::DOWN * (lbKey->getContentSize().height + 5));
        sprContentBox->addChild(lbWarning);
    }
}

