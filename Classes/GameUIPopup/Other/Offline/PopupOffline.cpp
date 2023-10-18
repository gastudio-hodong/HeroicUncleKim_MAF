//
//  PopupOffline.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupOffline.h"

#include "GameObject/InfoArtifact.h"
#include "ManagerGame/UserInfoManager.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/OfflineRewardManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/DefenseManager.h"

USING_NS_CC;

PopupOffline* PopupOffline::create()
{
    PopupOffline* pRet = new(std::nothrow) PopupOffline();
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

PopupOffline::PopupOffline(void) :
_nTime(0),
_nRewardFloor(0),

_nRewardGem(0),
_nRewardPoint(0),
_strRewardKey(""),
_strRewardGold(""),
_strRewardDefenseCoin(""),
_nRewardMine1(0),
_nRewardMine2(0),
_nRewardMine3(0),
_nRewardMine4(0),
_nRewardMine5(0),
_nRewardMine6(0),
_nRewardMineSpecial(0),
_bRewardAds(false),

_onCallbackResult(nullptr),

_layerContainerContentsTop(nullptr),
_layerContainerContentsBottom(nullptr),
_layerContainerButton(nullptr),
_layerStep1(nullptr),
_layerStep2(nullptr),
_layerStep3(nullptr),
_layerStep4(nullptr),
_layerStep5(nullptr)
{
    
}

PopupOffline::~PopupOffline(void)
{
    
}

bool PopupOffline::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupOffline::initVar()
{
    _nTime = OfflineRewardManager::getInstance()->getOfflineTime();
    
    _nRewardFloor = OfflineRewardManager::getInstance()->getRewardFloor();
    _nRewardGem = OfflineRewardManager::getInstance()->getRewardGem();
    _nRewardPoint = OfflineRewardManager::getInstance()->getRewardPoint();
    _strRewardKey = OfflineRewardManager::getInstance()->getRewardKey();
    _strRewardGold = OfflineRewardManager::getInstance()->getRewardGold();
    _nRewardMine1 = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_1);
    _nRewardMine2 = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_2);
    _nRewardMine3 = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_3);
    _nRewardMine4 = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_4);
    _nRewardMine5 = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_5);
    _nRewardMine6 = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_6);
    _nRewardMineSpecial = OfflineRewardManager::getInstance()->getRewardMine(E_MINE::MINE_7);
    _strRewardDefenseCoin = OfflineRewardManager::getInstance()->getRewardDefenseCoin();
}

void PopupOffline::initUi()
{
    Size size = Size(750, 1300);
    
    //
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_1.png", Rect::ZERO, Rect(77,23,2,2));
    _spriteContainer->setContentSize(size);
    _spriteContainer->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 13.5);
    addChild(_spriteContainer);
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContentsBottom = LayerColor::create();
    _layerContainerContentsBottom->setContentSize( Size(size.width - 50, 350) );
    _layerContainerContentsBottom->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerContentsBottom->setPosition(_spriteContainer->getContentSize().width / 2, 25 + _layerContainerButton->getContentSize().height + 25);
    _spriteContainer->addChild(_layerContainerContentsBottom);
    
    _layerContainerContentsTop = Layer::create();
    _layerContainerContentsTop->setContentSize( Size(size.width - 50, size.height - 25 - _layerContainerButton->getContentSize().height - 25 - _layerContainerContentsBottom->getContentSize().height - 25) );
    _layerContainerContentsTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContentsTop->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(_layerContainerContentsTop);
    
    
    // title
    auto spriteTitleBG = Sprite::create("Assets/ui/offline/offline_title.png");
    spriteTitleBG->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(spriteTitleBG);
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_offline_title"), GAME_FONT,28);
        labelTitle->setPosition(spriteTitleBG->getContentSize().width / 2, spriteTitleBG->getContentSize().height - 27);
        spriteTitleBG->addChild(labelTitle);
    }
    
    
    //
    uiContentsTop();
    uiContentsBottom();
    uiButton();
}

#pragma mark - ui
void PopupOffline::uiContentsTop()
{
    //
    _layerContainerContentsTop->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/offline/offline_bg_1.png");
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteBG->setPosition(_layerContainerContentsTop->getContentSize().width / 2, _layerContainerContentsTop->getContentSize().height - 3);
    _layerContainerContentsTop->addChild(spriteBG);
    
    //
    _layerStep1 = nullptr;
    _layerStep2 = nullptr;
    _layerStep3 = nullptr;
    _layerStep4 = nullptr;
    _layerStep5 = nullptr;
    uiStep1();
    uiStep2();
    uiStep3();
    uiStep4();
    uiStep5();
}

void PopupOffline::uiContentsBottom()
{
    //
    _layerContainerContentsBottom->removeAllChildren();

    auto spriteMineBG = Sprite::create("Assets/ui/offline/offline_bg_2.png");
    spriteMineBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteMineBG->setPosition(_layerContainerContentsBottom->getContentSize().width / 2, _layerContainerContentsBottom->getContentSize().height);
    _layerContainerContentsBottom->addChild(spriteMineBG);
    
    {
        bool bSkin = false;
        std::string strPath = "Assets/character_pet/normal/";
        if ( PetNewManager::getInstance()->getPetSkin(E_PET::MOLE_7) > 0 )
        {
            std::string strTemp = strPath;
            strTemp.append( MafUtils::format("mole_skin_%d", PetNewManager::getInstance()->getPetSkin(E_PET::MOLE_7)) );
            
            if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
            {
                bSkin = true;
                strPath = strTemp;
                strPath.append("/");
            }
        }
        
        if ( bSkin == false )
        {
            if ( PetNewManager::getInstance()->getPetEvolution(E_PET::MOLE_7) > 0 )
            {
                bSkin = true;
                strPath.append("mole_evolution/");
            }
            else
            {
                bSkin = true;
                strPath.append("mole/");
            }
        }
        
        if ( bSkin == false )
        {
            return;
        }
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.15);
        for ( int i = 1; i <= 4; i++ )
        {
            std::string strTemp = strPath + "pet_mole_attack_%d.png";
            strTemp = MafUtils::format(strTemp.c_str(), i);
            
            animation->addSpriteFrameWithFile(strTemp);
        }
    
        auto pritePet = Sprite::create("Assets/character_pet/normal/mole/pet_mole_walk_1.png");
        pritePet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        pritePet->setPosition(spriteMineBG->getContentSize().width * 0.45 + 10, 0);
        pritePet->runAction(RepeatForever::create(Animate::create(animation)));
        pritePet->setScale(0.5);
        spriteMineBG->addChild(pritePet);
        
    }

    //
    if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
    {
        int maxCol = 2;
        for ( int i = 0; i < 8; i++ )
        {
            int x = i % maxCol;
            int y = i / maxCol;
            double posX = (103) + 242 * x + 10 * x;
            double posY = (_layerContainerContentsBottom->getContentSize().height - 130) - 41 * y - 10 * y;
            
            auto spriteBG = Sprite::create("Assets/ui/offline/offline_item_box1.png");
            spriteBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            spriteBG->setPosition(posX, posY);
            _layerContainerContentsBottom->addChild(spriteBG);
            
            std::string strIconPath = "";
            std::string strText = "";
            std::string strTextAds = "0";
            switch (i) {
                case 0:
                {
                    E_MINERAL eMineral = E_MINERAL::MINERAL_7_SAPPHIRE;
                    std::string strMineSpecial = UserDefault::getInstance()->getStringForKey(KEY_MINE_SPECIAL);
                    if ( !strMineSpecial.empty() )
                    {
                        strMineSpecial = MafAes256::Decrypt(strMineSpecial);
                        eMineral = (E_MINERAL)atoi(strMineSpecial.c_str());
                    }
                    
                    strIconPath = DataManager::GetMineMineralPath(eMineral);
                    
                    int nReward = _nRewardMineSpecial;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
                case 1:
                {
                    strIconPath = "Assets/icon/icon_dimension.png";
                    
                    strText = _strRewardDefenseCoin;
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = _strRewardDefenseCoin;
                    }
                    break;
                }
                case 2:
                {
                    strIconPath = DataManager::GetMineMineralPath(E_MINERAL::MINERAL_1);
                    
                    int nReward = _nRewardMine1;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
                case 3:
                {
                    strIconPath = DataManager::GetMineMineralPath(E_MINERAL::MINERAL_4);
                    
                    int nReward = _nRewardMine4;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
                case 4:
                {
                    strIconPath = DataManager::GetMineMineralPath(E_MINERAL::MINERAL_2);
                    
                    int nReward = _nRewardMine2;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
                case 5:
                {
                    strIconPath = DataManager::GetMineMineralPath(E_MINERAL::MINERAL_5);
                    
                    int nReward = _nRewardMine5;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
                case 6:
                {
                    strIconPath = DataManager::GetMineMineralPath(E_MINERAL::MINERAL_3);
                    
                    int nReward = _nRewardMine3;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
                case 7:
                {
                    strIconPath = DataManager::GetMineMineralPath(E_MINERAL::MINERAL_6);
                    
                    int nReward = _nRewardMine6;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(nReward);
                    }
                    break;
                }
            }
            
            if ( strIconPath.length() == 0 )
            {
                continue;
            }
            
            auto spriteIcon = Sprite::create(strIconPath);
            spriteIcon->setPosition(40, spriteBG->getContentSize().height / 2);
            spriteBG->addChild(spriteIcon);
            
            auto labelText = Label::createWithTTF(strText, GAME_FONT, 20);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelText->setPosition(80, spriteBG->getContentSize().height / 2);
            spriteBG->addChild(labelText);
            if ( _bRewardAds == true )
            {
                auto labelTextAds = Label::createWithTTF(MafUtils::format("(+%s)", strTextAds.c_str()), GAME_FONT, 20);
                labelTextAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                labelTextAds->setPosition(labelText->getPositionX() + labelText->getContentSize().width + 10, labelText->getPositionY());
                labelTextAds->setColor(Color3B(0, 255, 13));
                spriteBG->addChild(labelTextAds);
            }
        }
        
        //
        auto spriteAdsIcon = Sprite::create("Assets/ui/offline/icon_adx200.png");
        spriteAdsIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        spriteAdsIcon->setPosition(103, _layerContainerContentsBottom->getContentSize().height - 130);
        _layerContainerContentsBottom->addChild(spriteAdsIcon);
    }
    else
    {
        auto spriteMineBG = Sprite::create("Assets/ui_common/base_text_1_2.png");
        spriteMineBG->setPosition(_layerContainerContentsBottom->getContentSize().width / 2, 120);
        _layerContainerContentsBottom->addChild(spriteMineBG);
        {
            auto spriteIcon = Sprite::create("Assets/character_pet/normal/mole/pet_mole_attack_1.png");
            spriteIcon->setPosition(0, spriteMineBG->getContentSize().height / 2);
            spriteMineBG->addChild(spriteIcon);
            
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_offline_msg11"), GAME_FONT, 20);
            labelText->setPosition(spriteMineBG->getContentSize().width / 2, spriteMineBG->getContentSize().height / 2);
            labelText->setDimensions(spriteMineBG->getContentSize().width * 0.9, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            spriteMineBG->addChild(labelText);
        }
    }
    
}

void PopupOffline::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto spriteConfirm = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteConfirm->setContentSize(Size(132, 88));
    auto spriteSelectConfirm = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectConfirm->setContentSize(Size(132, 88));
    
    auto itemConfirm = MafNode::MafMenuItemSprite::create(spriteConfirm, spriteSelectConfirm, nullptr, CC_CALLBACK_1(PopupOffline::onClickClose, this));
    menu->addChild(itemConfirm);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 26);
        label->setPosition( itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
        itemConfirm->addChild(label);
    }
    
    if ( _bRewardAds == false )
    {
        auto spriteAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_7_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteAds->setContentSize(Size(264, 88));
        auto spriteSelectAds = ui::Scale9Sprite::create("Assets/ui_common/btn_1_7_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteSelectAds->setContentSize(Size(264, 88));
        
        auto itemAds = MafNode::MafMenuItemSprite::create(spriteAds, spriteSelectAds, nullptr, CC_CALLBACK_1(PopupOffline::onClickAds, this));
        menu->addChild(itemAds);
        {
            auto sprite = Sprite::create("Assets/ui_common/btn_ads_play.png");
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            sprite->setPosition(70, itemAds->getContentSize().height / 2);
            itemAds->addChild(sprite);
            
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_offline_bt_ad"), GAME_FONT, 26);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            label->setPosition(80, itemAds->getContentSize().height / 2);
            label->setDimensions(170, label->getContentSize().height);
            label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            itemAds->addChild(label);
        }
    }
    
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupOffline::uiStep1()
{
    // 박스 상단 16, 박스 하단 31. 총 높이 47
    
    if ( _layerStep1 == nullptr )
    {
        _layerStep1 = Layer::create();
        _layerStep1->setContentSize(Size(275, 150));
        _layerStep1->setIgnoreAnchorPointForPosition(false);
        _layerStep1->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
        _layerStep1->setPosition( Vec2(_layerContainerContentsTop->getContentSize().width / 2 - 70, 500) );
        _layerContainerContentsTop->addChild(_layerStep1);
    }
    _layerStep1->removeAllChildren();
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui/offline/offline_text_box.png", Rect::ZERO, Rect(28,10,2,2));
    spriteBG->setContentSize(_layerStep1->getContentSize());
    spriteBG->setAnchorPoint(Vec2::ZERO);
    spriteBG->setPosition(0, 0);
    spriteBG->setFlippedX(true);
    _layerStep1->addChild(spriteBG);
    
    auto layerBox = Layer::create();
    layerBox->setContentSize( Size(_layerStep1->getContentSize().width - 32, _layerStep1->getContentSize().height - 47) );
    layerBox->setIgnoreAnchorPointForPosition(false);
    layerBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    layerBox->setPosition( 16, _layerStep1->getContentSize().height - 16);
    _layerStep1->addChild(layerBox);
    
    {
        //
        auto spriteIcon = Sprite::create("Assets/ui/offline/offline_bong.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteIcon->setPosition(80, layerBox->getContentSize().height / 2);
        layerBox->addChild(spriteIcon);
        
        // time floor
        auto spriteTimer = Sprite::create("Assets/ui/offline/icon_offline_time.png");
        spriteTimer->setPosition(110, layerBox->getContentSize().height * 0.75);
        spriteTimer->setScale(0.5);
        layerBox->addChild(spriteTimer);
        {
            std::string strTime = "";
            if ( _nTime >= 3600 ){
                strTime = GAME_TEXTFORMAT("t_ui_time_2", _nTime / 3600, _nTime % 3600 / 60);
            }
            else{
                strTime = GAME_TEXTFORMAT("t_ui_minute", _nTime / 60);
            }
            
            auto labelTimer = Label::createWithTTF(strTime, GAME_FONT, 20);
            labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelTimer->setPosition(135, layerBox->getContentSize().height * 0.75);
            layerBox->addChild(labelTimer);
        }
        
        auto spriteFloor = Sprite::create("Assets/ui/offline/icon_offline_normal.png");
        spriteFloor->setPosition(110, layerBox->getContentSize().height * 0.25);
        spriteFloor->setScale(0.5);
        layerBox->addChild(spriteFloor);
        {
            auto labelFloor = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_offline_msg4", _nRewardFloor), GAME_FONT, 20);
            labelFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelFloor->setPosition(135, layerBox->getContentSize().height * 0.25);
            layerBox->addChild(labelFloor);
        }
    }
}

void PopupOffline::uiStep2()
{
    if ( _layerStep2 == nullptr )
    {
        _layerStep2 = Layer::create();
        _layerStep2->setContentSize(Size(275, 173));
        _layerStep2->setIgnoreAnchorPointForPosition(false);
        _layerStep2->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        _layerStep2->setPosition(_layerContainerContentsTop->getContentSize().width / 2 + 70, 400);
        _layerContainerContentsTop->addChild(_layerStep2);
    }
    _layerStep2->removeAllChildren();
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui/offline/offline_text_box.png", Rect::ZERO, Rect(28,10,2,2));
    spriteBG->setContentSize(_layerStep2->getContentSize());
    spriteBG->setAnchorPoint(Vec2::ZERO);
    spriteBG->setPosition(0, 0);
    _layerStep2->addChild(spriteBG);
    
    auto layerBox = Layer::create();
    layerBox->setContentSize( Size(_layerStep2->getContentSize().width - 32, _layerStep2->getContentSize().height - 47) );
    layerBox->setIgnoreAnchorPointForPosition(false);
    layerBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    layerBox->setPosition(16, _layerStep2->getContentSize().height - 16);
    _layerStep2->addChild(layerBox);
    {
        for ( int i = 0; i < 4; i++ )
        {
            double posX = (0) + 0;
            double posY = (layerBox->getContentSize().height) - 30 * i - 2 * i;
            
            auto layerCurrencyBG = Layer::create();
            layerCurrencyBG->setContentSize(Size(layerBox->getContentSize().width, 30));
            layerCurrencyBG->setIgnoreAnchorPointForPosition(false);
            layerCurrencyBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            layerCurrencyBG->setPosition(posX, posY);
            layerBox->addChild(layerCurrencyBG);
            
            std::string strIconPath = "";
            std::string strText = "0";
            std::string strTextAds = "0";
            switch (i) {
                case 0:
                {
                    strIconPath = "Assets/icon/menu_key.png";
                    
                    strText = _strRewardKey;
                    MafUtils::convertNumberToShort(strText);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = _strRewardKey;
                        strTextAds = MafUtils::bigMulNum(strTextAds, "100");
                        if ( strTextAds.length() > 2 )
                        {
                            strTextAds.pop_back();
                            strTextAds.pop_back();
                        }
                        else
                        {
                            strTextAds = "0";
                        }
                        MafUtils::convertNumberToShort(strTextAds);
                    }
                    break;
                }
                case 1:
                {
                    strIconPath = "Assets/icon/icon_jewel.png";
                    
                    int nReward = _nRewardGem;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(_nRewardGem * 1);
                    }
                    break;
                }
                case 2:
                {
                    strIconPath = "Assets/icon/menu_coin.png";
                    
                    strText = _strRewardGold;
                    MafUtils::convertNumberToShort(strText);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = _strRewardGold;
                        strTextAds = MafUtils::bigMulNum(strTextAds, "100");
                        if ( strTextAds.length() > 2 )
                        {
                            strTextAds.pop_back();
                            strTextAds.pop_back();
                        }
                        else
                        {
                            strTextAds = "0";
                        }
                        MafUtils::convertNumberToShort(strTextAds);
                    }
                    break;
                }
                case 3:
                {
                    strIconPath = "Assets/icon/icon_point1.png";
                    
                    int nReward = _nRewardPoint;
                    strText = MafUtils::toString(nReward);
                    
                    if ( _bRewardAds == true )
                    {
                        strTextAds = MafUtils::toString(_nRewardPoint * 1);
                    }
                    break;
                }
            }
            
            if ( strIconPath.length() == 0 )
            {
                continue;
            }
            
            auto spriteIcon = Sprite::create(strIconPath);
            spriteIcon->setPosition(40, layerCurrencyBG->getContentSize().height / 2);
            layerCurrencyBG->addChild(spriteIcon);
            
            auto labelText = Label::createWithTTF(strText, GAME_FONT, 20);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelText->setPosition(80, layerCurrencyBG->getContentSize().height / 2);
            layerCurrencyBG->addChild(labelText);
            if ( _bRewardAds == true )
            {
                auto labelTextAds = Label::createWithTTF(MafUtils::format("(+%s)", strTextAds.c_str()), GAME_FONT, 20);
                labelTextAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                labelTextAds->setPosition(labelText->getPositionX() + labelText->getContentSize().width + 10, labelText->getPositionY());
                labelTextAds->setColor(Color3B(0, 255, 13));
                layerCurrencyBG->addChild(labelTextAds);
            }
        }
    }
    
    //
    auto spriteAdsIcon = Sprite::create("Assets/ui/offline/icon_adx200.png");
    spriteAdsIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteAdsIcon->setPosition(_layerStep2->getContentSize().width, _layerStep2->getContentSize().height);
    _layerStep2->addChild(spriteAdsIcon);
}

void PopupOffline::uiStep3()
{
    std::string strContents = "";
    for ( int j = 1; j <= C_COUNT_PET; j++ )
    {
        E_PET ePet = (E_PET)j;
        
        bool bEvolution = OfflineRewardManager::getInstance()->isRewardPetEvolution(ePet);
        if ( bEvolution == true )
        {
            std::string strPetName = PetNewManager::getInstance()->getPetName(ePet);
            std::string strMessage = GAME_TEXTFORMAT("t_ui_offline_msg16", strPetName.c_str());
            
            if ( strContents.length() != 0 )
                strContents.append("\n");
            
            strContents.append(strMessage);
        }
    }
    
    if ( strContents.length() == 0 )
    {
        strContents = GAME_TALK();
    }
    
    if ( _layerStep3 == nullptr )
    {
        auto label = Label::createWithTTF(strContents, GAME_FONT, 20);
        double nHeight = label->getContentSize().height + 47;
        if ( 150 > nHeight )
        {
            nHeight = 150;
        }
        
        _layerStep3 = Layer::create();
        _layerStep3->setContentSize(Size(275, nHeight));
        _layerStep3->setIgnoreAnchorPointForPosition(false);
        _layerStep3->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
        _layerStep3->setPosition( Vec2(_layerContainerContentsTop->getContentSize().width / 2 - 70, 300) );
        _layerContainerContentsTop->addChild(_layerStep3);
    }
    _layerStep3->removeAllChildren();
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui/offline/offline_text_box.png", Rect::ZERO, Rect(28,10,2,2));
    spriteBG->setContentSize(_layerStep3->getContentSize());
    spriteBG->setAnchorPoint(Vec2::ZERO);
    spriteBG->setPosition(0, 0);
    spriteBG->setFlippedX(true);
    _layerStep3->addChild(spriteBG);
    
    auto layerBox = Layer::create();
    layerBox->setContentSize( Size(_layerStep3->getContentSize().width - 32, _layerStep3->getContentSize().height - 47) );
    layerBox->setIgnoreAnchorPointForPosition(false);
    layerBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    layerBox->setPosition( 16, _layerStep3->getContentSize().height - 16);
    _layerStep3->addChild(layerBox);
    {
        auto labelContents = Label::createWithTTF(strContents, GAME_FONT, 20);
        labelContents->setPosition(layerBox->getContentSize().width / 2, layerBox->getContentSize().height / 2);
        labelContents->setDimensions(layerBox->getContentSize().width, layerBox->getContentSize().height);
        labelContents->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelContents->setOverflow(Label::Overflow::SHRINK);
        layerBox->addChild(labelContents);
    }
}

void PopupOffline::uiStep4()
{
    std::string strContents = "";
    bool bStoryOpen = OfflineRewardManager::getInstance()->isRewardStory();
    if ( bStoryOpen == true )
    {
        strContents = GAME_TEXT("t_ui_offline_msg17");
    }
    
    if ( strContents.length() == 0 )
    {
        strContents = GAME_TALK();
    }
    
    if ( _layerStep4 == nullptr )
    {
        _layerStep4 = Layer::create();
        _layerStep4->setContentSize(Size(275, 150));
        _layerStep4->setIgnoreAnchorPointForPosition(false);
        _layerStep4->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        _layerStep4->setPosition(_layerContainerContentsTop->getContentSize().width / 2 + 70, 100);
        _layerContainerContentsTop->addChild(_layerStep4);
    }
    _layerStep4->removeAllChildren();
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui/offline/offline_text_box.png", Rect::ZERO, Rect(28,10,2,2));
    spriteBG->setContentSize(_layerStep4->getContentSize());
    spriteBG->setAnchorPoint(Vec2::ZERO);
    spriteBG->setPosition(0, 0);
    _layerStep4->addChild(spriteBG);
    
    auto layerBox = Layer::create();
    layerBox->setContentSize( Size(_layerStep4->getContentSize().width - 32, _layerStep4->getContentSize().height - 47) );
    layerBox->setIgnoreAnchorPointForPosition(false);
    layerBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    layerBox->setPosition(16, _layerStep4->getContentSize().height - 16);
    _layerStep4->addChild(layerBox);
    {
        auto labelContents = Label::createWithTTF(strContents, GAME_FONT, 20);
        labelContents->setPosition(layerBox->getContentSize().width / 2, layerBox->getContentSize().height / 2);
        labelContents->setDimensions(layerBox->getContentSize().width, layerBox->getContentSize().height);
        labelContents->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelContents->setOverflow(Label::Overflow::SHRINK);
        layerBox->addChild(labelContents);
    }
}

void PopupOffline::uiStep5()
{
    if ( _layerStep5 == nullptr )
    {
        _layerStep5 = Layer::create();
        _layerStep5->setContentSize(Size(275, 150));
        _layerStep5->setIgnoreAnchorPointForPosition(false);
        _layerStep5->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
        _layerStep5->setPosition( Vec2(_layerContainerContentsTop->getContentSize().width / 2 - 70, 100) );
        _layerContainerContentsTop->addChild(_layerStep5);
    }
    _layerStep5->removeAllChildren();
    
    
    int nLevelArtifact_106 = UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_106);
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui/offline/offline_text_box.png", Rect::ZERO, Rect(28,10,2,2));
    spriteBG->setContentSize(_layerStep5->getContentSize());
    spriteBG->setAnchorPoint(Vec2::ZERO);
    spriteBG->setPosition(0, 0);
    spriteBG->setFlippedX(true);
    _layerStep5->addChild(spriteBG);
    
    auto layerBox = Layer::create();
    layerBox->setContentSize( Size(_layerStep5->getContentSize().width - 32, _layerStep5->getContentSize().height - 47) );
    layerBox->setIgnoreAnchorPointForPosition(false);
    layerBox->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    layerBox->setPosition(16, _layerStep5->getContentSize().height - 16);
    _layerStep5->addChild(layerBox);
    {
        auto objArtifact = DataManager::getInstance()->GetArtifactInfo(E_ARTIFACT::ARTI_106);
        
        auto spriteIcon = Sprite::create(objArtifact->getIconPath());
        spriteIcon->setPosition(spriteIcon->getContentSize().width / 2, layerBox->getContentSize().height / 2);
        layerBox->addChild(spriteIcon);
        
        double nEffect = DataManager::getInstance()->GetArtifactEffect(E_ARTIFACT::ARTI_106, nLevelArtifact_106, 0);
        std::string strContents = objArtifact->getTitle();
        strContents.append("\n");
        strContents.append(GAME_TEXTFORMAT("t_ui_offline_mag_1", (int)nEffect));
        
        auto labelContents = Label::createWithTTF(strContents, GAME_FONT, 24);
        labelContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelContents->setPosition(spriteIcon->getContentSize().width + 10, layerBox->getContentSize().height / 2);
        labelContents->setDimensions(layerBox->getContentSize().width - spriteIcon->getContentSize().width - 10, layerBox->getContentSize().height);
        labelContents->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelContents->setOverflow(Label::Overflow::SHRINK);
        layerBox->addChild(labelContents);
    }
}

#pragma mark - set, get, sort, add
void PopupOffline::setCallbackResult(const std::function<void(void)>& callback)
{
    _onCallbackResult = callback;
}

#pragma mark - click
void PopupOffline::onClickClose(Ref* sender)
{
    //
    OfflineRewardManager::getInstance()->reward(_bRewardAds);
    
    //
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult();
    }
    
    hide();
}

void PopupOffline::onClickAds(cocos2d::Ref* sender)
{
    if ( AdsUtilsManager::getInstance()->getAdsCooldown() > 0 && CashManager::getInstance()->isAdsfreeTime() == false )
    {
        std::string strMessage = GAME_TEXTFORMAT("t_ui_ads_cooldown_1", AdsUtilsManager::getInstance()->getAdsCooldown());
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupOffline::callbackAds, this));
    }
}

void PopupOffline::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotOffline, CashManager::getInstance()->isAdsfreeTime());
        
        //
        _bRewardAds = true;
        
        //
        uiContentsTop();
        uiContentsBottom();
        uiButton();
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupOffline::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotOffline);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}
