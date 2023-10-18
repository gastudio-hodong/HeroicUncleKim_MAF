//
//  PopupDefenseTrackSetting.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/31.
//

#include "PopupDefenseTrenchSetting.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"

#include "GameObject/Defense/InfoDefenseTrench.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "GameObject/InfoSetting.h"


#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/Defense/PopupDefenseTrenchSelect.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;


PopupDefenseTrenchSetting* PopupDefenseTrenchSetting::create()
{
    PopupDefenseTrenchSetting* pRet = new(std::nothrow) PopupDefenseTrenchSetting();
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

PopupDefenseTrenchSetting::PopupDefenseTrenchSetting(void):
_lyTop(nullptr),
_lyMiddle(nullptr),
_lyBottom(nullptr),
_table(nullptr)
{
}

PopupDefenseTrenchSetting::~PopupDefenseTrenchSetting(void)
{
    DefenseManager::getInstance()->_onUpdateCharacter.Clear(this);
    DefenseManager::getInstance()->_onUpdateWeapon.Clear(this);
    DefenseManager::getInstance()->_onServerEquip.Clear(this);
}

#pragma mark - table
void PopupDefenseTrenchSetting::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupDefenseTrenchSetting::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 160);
}

extension::TableViewCell* PopupDefenseTrenchSetting::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    
    auto objTrench = DefenseManager::getInstance()->getInfoTrench(idx+1);
    
    if(objTrench == nullptr)
        return cell;
    
    auto sprBG = Sprite::create("Assets/ui/defense/Ui_list_box.png");
    sprBG->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBG);
    
    auto sprTrench = Sprite::create("Assets/ui/defense/Defense_trenches_01_lv1.png");
    sprTrench->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprTrench->setPosition(15, sprBG->getContentSize().height/2 + 20);
    sprTrench->getTexture()->setTexParameters(texParams);
    sprTrench->setScale(0.8f);
    sprBG->addChild(sprTrench);
    
    if(objTrench->getCharacterIdx() > 0)
    {
        auto sprCharacter = Sprite::create(MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_00.png", objTrench->getCharacterIdx()));
        sprCharacter->setPosition(sprTrench->getPosition() + Vec2::RIGHT * (sprTrench->getContentSize().width * sprTrench->getScale() / 2 - 15) + Vec2::UP * 15);
        sprCharacter->getTexture()->setTexParameters(texParams);
        sprBG->addChild(sprCharacter);
        
        auto anim = Animation::create();
        for(int i = 0; i < 2; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_%02d.png", objTrench->getCharacterIdx(), i);
            
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        
        anim->setDelayPerUnit(0.3f);
        auto animate = Animate::create(anim);
        sprCharacter->runAction(RepeatForever::create(animate));
    }
    
    auto sprTrenchFront = Sprite::create("Assets/ui/defense/Defense_trenches_02_lv1.png");
    sprTrenchFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprTrenchFront->setPosition(15, sprBG->getContentSize().height/2 + 20);
    sprTrenchFront->getTexture()->setTexParameters(texParams);
    sprTrenchFront->setScale(0.8f);
    sprBG->addChild(sprTrenchFront);
    
    auto sprTrenchSandBag = Sprite::create("Assets/ui/defense/Defense_trenches_03_lv1.png");
    sprTrenchSandBag->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprTrenchSandBag->setPosition(15, sprBG->getContentSize().height/2 + 20);
    sprTrenchSandBag->getTexture()->setTexParameters(texParams);
    sprTrenchSandBag->setScale(0.8f);
    sprBG->addChild(sprTrenchSandBag);
    
    if(objTrench->getWeaponIdx() > 0)
    {
        auto weaponData = objTrench->getInfoDefenseWeapon();
        int count = 0;
        
        auto sprWeapon = Sprite::create(MafUtils::format("Assets/ui/defense/Defense_gun_lv%d_00.png", objTrench->getWeaponIdx()));
        sprWeapon->setPosition(sprTrench->getPosition() + Vec2::RIGHT * sprTrench->getContentSize().width * sprTrench->getScale() / 2);
        sprWeapon->setScale(0.8f);
        sprWeapon->getTexture()->setTexParameters(texParams);
        sprBG->addChild(sprWeapon);
        
        auto anim = Animation::create();
        for(int i = 0; i <= 100; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/defense/Defense_gun_lv%d_%02d.png", weaponData->getIndex(), i);
            std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
            
            if (fullpath.size() == 0)
                break;
            
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
            count++;
        }
        {
            auto sprite = Sprite::create(MafUtils::format("Assets/ui/defense/Defense_gun_lv%d_%02d.png", weaponData->getIndex(), 0));
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        
        float delay = 0.15f / weaponData->getBaseDFAttackSpeed()/(float)count;
        
        if(delay <= 0.05f)
            delay = 0.05f;
        anim->setDelayPerUnit(delay);
        auto animate = Animate::create(anim);
        
        auto func = CallFunc::create([&, weaponData, sprWeapon, sprTrench, sprBG]{
           Vec2 fxLocation = Vec2(sprWeapon->getPosition() + Vec2::RIGHT * sprWeapon->getContentSize().width/2 * sprWeapon->getScale());
            playMuzzleEffect(DefenseManager::getInstance()->getBulletData(weaponData->getBulletType()), sprBG, fxLocation);
        });
        auto seq = Sequence::create(Spawn::create(animate, func, nullptr), DelayTime::create(0.5f), nullptr);
        
        if(objTrench->getCharacterIdx() > 0)
            sprWeapon->runAction(RepeatForever::create(seq));
    }
    
    auto buttonWeapon = ui::Button::create("Assets/ui/defense/Ui_list_big_box.png");
    buttonWeapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    buttonWeapon->setPosition(Vec2(sprBG->getContentSize().width - 10, sprBG->getContentSize().height / 2));
    buttonWeapon->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSetting::onClickWeaponChange, this));
    buttonWeapon->setTag(objTrench->getNumber());
    buttonWeapon->setZoomScale(0.01f);
    sprBG->addChild(buttonWeapon, 1);
    {
        auto sprNameBox = Sprite::create("Assets/ui/defense/Ui_list_big_namebox.png");
        sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprNameBox->setPosition(buttonWeapon->getContentSize().width/2, 3);
        buttonWeapon->addChild(sprNameBox);
        
        if(objTrench->getWeaponIdx() > 0)
        {
            auto sprIcon = Sprite::create(MafUtils::format("Assets/ui/defense/Icon_gun_lv%d.png", objTrench->getWeaponIdx()));
            sprIcon->setPosition(buttonWeapon->getContentSize()/2);
            buttonWeapon->addChild(sprIcon);
            
            auto lbName = Label::createWithTTF(objTrench->getInfoDefenseWeapon()->getName(), GAME_FONT, 20);
            lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbName->setPosition(sprNameBox->getContentSize()/2);
            lbName->setDimensions(sprNameBox->getContentSize().width - 10, lbName->getContentSize().height);
            lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbName->setOverflow(Label::Overflow::SHRINK);
            sprNameBox->addChild(lbName);
        }
    }
    
    auto buttonCharacter = ui::Button::create("Assets/ui/defense/Ui_list_big_box.png");
    buttonCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    buttonCharacter->setPosition(buttonWeapon->getPosition() + Vec2::LEFT * (10 + buttonWeapon->getContentSize().width));
    buttonCharacter->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSetting::onClickCharacterChange, this));
    buttonCharacter->setTag(objTrench->getNumber());
    buttonCharacter->setZoomScale(0.01f);
    sprBG->addChild(buttonCharacter, 1);
    {
        auto sprNameBox = Sprite::create("Assets/ui/defense/Ui_list_big_namebox.png");
        sprNameBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprNameBox->setPosition(buttonCharacter->getContentSize().width/2, 5);
        buttonCharacter->addChild(sprNameBox, 1);
        
        if(objTrench->getCharacterIdx() > 0)
        {
            auto sprIcon = Sprite::create(MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_00.png", objTrench->getCharacterIdx()));
            sprIcon->setPosition(Vec2(buttonCharacter->getContentSize()/2) + Vec2::UP * 20);
            buttonCharacter->addChild(sprIcon);
            
            auto lbName = Label::createWithTTF(objTrench->getInfoDefenseCharacter()->getName(), GAME_FONT, 20);
            lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbName->setPosition(sprNameBox->getContentSize()/2);
            lbName->setDimensions(sprNameBox->getContentSize().width - 10, lbName->getContentSize().height);
            lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbName->setOverflow(Label::Overflow::SHRINK);
            sprNameBox->addChild(lbName);
        }
    }
    
    auto sprFrame = Sprite::create("Assets/ui/defense/Ui_linenumber_box.png");
    sprFrame->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprFrame->setPosition(Vec2(0,sprBG->getContentSize().height));
    sprFrame->getTexture()->setTexParameters(texParams);
    sprBG->addChild(sprFrame);
    
    //18,16
    auto lbTrenchNum = MafNode::createBmFont(MafUtils::format("%d", objTrench->getNumber()), GAME_BM_FONT_AL, 22);
    lbTrenchNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbTrenchNum->setPosition(Vec2(20,sprFrame->getContentSize().height - 16));
    lbTrenchNum->setColor(Color3B(210,230,255));
    sprFrame->addChild(lbTrenchNum);
    
    return cell;
}

ssize_t PopupDefenseTrenchSetting::numberOfCellsInTableView(extension::TableView *table)
{
    return DefenseManager::getInstance()->getMaxTrenchCount();
}


#pragma mark -init
bool PopupDefenseTrenchSetting::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    initVar();
    initUI();
    
    DefenseManager::getInstance()->_onUpdateCharacter +=  new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseTrenchSetting::uiRefresh, this), this);
    DefenseManager::getInstance()->_onUpdateWeapon += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseTrenchSetting::uiRefresh, this), this);
    
    DefenseManager::getInstance()->_onServerEquip += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupDefenseTrenchSetting::onServerEquip, this), this);
    
    return true;
}

void PopupDefenseTrenchSetting::initVar()
{
    _spriteContainer->setContentSize(Size(650, 1240));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupDefenseTrenchSetting::initUI()
{
    // title
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_squad_setting"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDefenseTrenchSetting::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    // UI
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(_spriteContainer->getContentSize().width, 70));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTop);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(_spriteContainer->getContentSize().width, 110));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(_spriteContainer->getContentSize().width/2, 10);
    _spriteContainer->addChild(_lyBottom);
    
    _lyMiddle = LayerColor::create();
    _lyMiddle->setContentSize(Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - _lyTop->getContentSize().height - _lyBottom->getContentSize().height));
    _lyMiddle->setIgnoreAnchorPointForPosition(false);
    _lyMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyMiddle->setPosition(_lyTop->getPosition() + Vec2::DOWN * (_lyTop->getContentSize().height));
    _spriteContainer->addChild(_lyMiddle);
    
    uiTop();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupDefenseTrenchSetting::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/defense/Ui_listname_box.png");
    sprBG->setPosition(_lyTop->getContentSize()/2);
    _lyTop->addChild(sprBG);
    
    auto lbWeapon = Label::createWithTTF(GAME_TEXT("t_ui_defense_squad_weapon"), GAME_FONT, 22);
    lbWeapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbWeapon->setPosition(sprBG->getContentSize().width - 94, sprBG->getContentSize().height/2);
    lbWeapon->setDimensions(140, sprBG->getContentSize().height);
    lbWeapon->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbWeapon->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbWeapon);
    
    auto lbCharacter = Label::createWithTTF(GAME_TEXT("t_ui_defense_squad_member"), GAME_FONT, 22);
    lbCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbCharacter->setPosition(sprBG->getContentSize().width - 285, sprBG->getContentSize().height/2);
    lbCharacter->setDimensions(140, sprBG->getContentSize().height);
    lbCharacter->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbCharacter->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbCharacter);
    
    auto lbTrench = Label::createWithTTF(GAME_TEXT("t_ui_defense_squad_line"), GAME_FONT, 22);
    lbTrench->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbTrench->setPosition(105, sprBG->getContentSize().height/2);
    lbTrench->setDimensions(200, sprBG->getContentSize().height);
    lbTrench->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbTrench->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbTrench);
}

void PopupDefenseTrenchSetting::uiTable()
{
    _lyMiddle->removeAllChildren();
    
    auto lyEffectInfo = LayerColor::create();
    lyEffectInfo->setContentSize( Size(_lyMiddle->getContentSize().width, _lyMiddle->getContentSize().height - 10));
    lyEffectInfo->setIgnoreAnchorPointForPosition(false);
    lyEffectInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyEffectInfo->setPosition( Vec2(_lyMiddle->getContentSize().width / 2, 20));
    _lyMiddle->addChild(lyEffectInfo);
    
    _table = extension::TableView::create(this, Size(lyEffectInfo->getContentSize().width, lyEffectInfo->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(lyEffectInfo->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    lyEffectInfo->addChild(_table);
    _table->reloadData();
}

void PopupDefenseTrenchSetting::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto btnAutoEquip = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnAutoEquip->setScale9Enabled(true);
    btnAutoEquip->setContentSize(Size(190,90));
    btnAutoEquip->setPosition(Vec2(_lyBottom->getContentSize()/2) + Vec2::UP * 10);
    btnAutoEquip->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSetting::onClickAutoSetting, this));
    btnAutoEquip->setTitleText(GAME_TEXT("t_ui_defense_squad_auto"));
    btnAutoEquip->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnAutoEquip->getTitleLabel()->setDimensions(btnAutoEquip->getContentSize().width, btnAutoEquip->getContentSize().height);
    btnAutoEquip->setTitleFontSize(30);
    btnAutoEquip->setZoomScale(0.05f);
    _lyBottom->addChild(btnAutoEquip);

}

void PopupDefenseTrenchSetting::uiRefresh()
{
    uiTop();
    _table->reloadData();
}
#pragma mark -utils
void PopupDefenseTrenchSetting::onClickClose(Ref* sender)
{
    hide();
}
void PopupDefenseTrenchSetting::onClickWeaponChange(Ref* sender)
{
    auto node = (Node*) sender;
    
    auto popup = PopupDefenseTrenchSelect::create(E_DF_Trench_SettingType::WEAPON, node->getTag());
    popup->show();
}
void PopupDefenseTrenchSetting::onClickCharacterChange(Ref* sender)
{
    auto node = (Node*) sender;
    
    auto popup = PopupDefenseTrenchSelect::create(E_DF_Trench_SettingType::CHARACTER, node->getTag());
    popup->show();
}
void PopupDefenseTrenchSetting::onClickAutoSetting(Ref* sender)
{
    int maxTrench = 6;
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("max_track_count"))
        maxTrench = setting->getValueToInt();
    
    auto weaponList = DefenseManager::getInstance()->getTopStrongWeaponList();
    auto characterList = DefenseManager::getInstance()->getTopStrongCharacterList();
    
    
    for(int i = 1; i <= 6; ++i)
    {
        if(weaponList.size() >= i)
        {
            auto data = weaponList.at(i-1);
            DefenseManager::getInstance()->requestEquip(E_DF_Trench_SettingType::WEAPON, data->getIndex(), i);
        }
        if(characterList.size() >= i)
        {
            auto data = characterList.at(i-1);
            DefenseManager::getInstance()->requestEquip(E_DF_Trench_SettingType::CHARACTER, data->getIndex(), i);
        }
    }
}
void PopupDefenseTrenchSetting::playMuzzleEffect(InfoDefenseBullet *bulletData, Node* parent, Vec2 location)
{
    if(bulletData == nullptr)
        return;
    
    std::string muzzlePath = "";
    int rand = 0;
    switch(bulletData->getMuzzleEffectType())
    {
        case E_DF_MuzzleEffectType::SMALL_TYPE:
        {
            rand = random(1,3);
        }break;
        case E_DF_MuzzleEffectType::STLONG_TYPE:
        {
            rand = random(4,6);
        }break;
        case E_DF_MuzzleEffectType::BIG_STLONG_TYPE:
        {
            rand = 7;
        }break;
        case E_DF_MuzzleEffectType::IMAGE_NUM_TYPE:
        {
            rand = bulletData->getImageNum();
        }break;
        default: break;
    }
    
    muzzlePath = MafUtils::format("Assets/ui/defense/Gun_effect_%02d_01.png",rand);
    auto muzzleFx = Sprite::create(muzzlePath);
    muzzleFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    muzzleFx->setPosition(location);
    muzzleFx->setScale(0.8f);
    parent->addChild(muzzleFx);
    
    auto anim = Animation::create();
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Gun_effect_%02d_%02d.png", rand, i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    anim->setDelayPerUnit(0.05f);
    auto animate = Animate::create(anim);
    auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
    muzzleFx->runAction(seq);
}
void PopupDefenseTrenchSetting::onServerEquip(bool bResult, int nResult)
{
    if(!bResult || nResult != 1)
    {
        hide();
        return;
    }
    uiRefresh();
}
