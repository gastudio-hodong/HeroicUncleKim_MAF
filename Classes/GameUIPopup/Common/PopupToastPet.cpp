//
//  PopupToastPet.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupToastPet.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

int PopupToastPet::_nCountVisible = 0;
PopupToastPet* PopupToastPet::create(E_PET ePet)
{
    PopupToastPet* pRet = new(std::nothrow) PopupToastPet();
    if ( pRet && pRet->init(ePet) )
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

PopupToastPet::PopupToastPet(void) :
_ePlace(E_PLACE::DG_NORMAL),
_ePet(E_PET::PET_NONE),

_layerContainerPet(nullptr)
{
    
}

PopupToastPet::~PopupToastPet(void)
{
    
}

bool PopupToastPet::init(E_PET ePet)
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    _ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    _ePet = ePet;
    
    // init
    initVar();
    initUi();
    
    return true;
}


void PopupToastPet::initVar()
{
    
}

void PopupToastPet::initUi()
{
    _layerContainerPet = Layer::create();
    _layerContainerPet->setContentSize( Size(getContentSize().width, 600) );
    _layerContainerPet->setIgnoreAnchorPointForPosition(false);
    _layerContainerPet->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerPet->setPosition(getContentSize().width / 2, getContentSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    _layerContainerPet->setCascadeOpacityEnabled(true);
    addChild(_layerContainerPet);
    
    
    
    // action
    {
        double nDelay = 2.0;
        
        runAction( FadeTo::create(0.4, 165) );
        
        auto seq = Sequence::create(FadeIn::create(0.15f), DelayTime::create(nDelay), FadeOut::create(0.15f), nullptr);
        _layerContainerPet->runAction(seq);
    }
    {
        double nDelay = 2.3;
        
        auto seqParent = Sequence::create(DelayTime::create(nDelay), RemoveSelf::create(), nullptr);
        runAction(seqParent);
    }

    //
    uiPet();
}

#pragma mark - ui
void PopupToastPet::uiPet()
{
    _layerContainerPet->removeAllChildren();
    
    auto spriteEffect = Sprite::create("Assets/ui_common/fx_halo1_1.png");
    spriteEffect->setPosition(_layerContainerPet->getContentSize().width / 2, _layerContainerPet->getContentSize().height / 2);
    spriteEffect->getTexture()->setTexParameters(texParams);
    spriteEffect->setScale(3);
    spriteEffect->runAction(RepeatForever::create(RotateBy::create(10, 360)));
    _layerContainerPet->addChild(spriteEffect);
    
    //
    Pet* uiPet = nullptr;
    double nPosPetX = 0;
    double nPosPetY = 0;
    switch (_ePet) {
        case E_PET::CHICK_1:
            uiPet = Chick::create();
            uiPet->setScale(2.1);
            nPosPetY = -337;
            break;
        case E_PET::HAMSTER_2:
            uiPet = Hamster::create();
            uiPet->setScale(2.1);
            nPosPetX = 0;
            nPosPetY = -307;
            break;
        case E_PET::DRAGON_3:
            uiPet = Dragon::create();
            uiPet->setScale(2.1);
            nPosPetX = 0;
            nPosPetY = -332;
            break;
        case E_PET::PRINCESS_4:
            uiPet = Princess::create();
            uiPet->setScale(1.5);
            nPosPetX = 55;
            nPosPetY = -267;
            break;
        case E_PET::PURIRING_5:
            uiPet = Puriring::create();
            uiPet->setScale(2.1);
            nPosPetX = 0;
            nPosPetY = -227;
            break;
        case E_PET::HONG_6:
            uiPet = Hong::create();
            uiPet->setScale(2.1);
            nPosPetX = 5;
            nPosPetY = -322;
            break;
        case E_PET::MOLE_7:
            uiPet = Mole::create();
            uiPet->setScale(2.1);
            nPosPetX = 0;
            nPosPetY = -307;
            break;
        case E_PET::HANDSOME_8:
            uiPet = Handsome::create();
            uiPet->setScale(2.1);
            nPosPetX = 0;
            nPosPetY = -327;
            break;
            
        default:
            break;
    }
    uiPet->setPosition(_layerContainerPet->getContentSize().width / 2 + nPosPetX, _layerContainerPet->getContentSize().height + nPosPetY);
    _layerContainerPet->addChild(uiPet);
    
    
    //
    auto spriteTitle = Sprite::create("Assets/ui/purchase/base_title_1_1.png");
    spriteTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteTitle->setPosition(_layerContainerPet->getContentSize().width / 2, 50);
    spriteTitle->setCascadeOpacityEnabled(true);
    _layerContainerPet->addChild(spriteTitle);
    {
        std::string strName = PetNewManager::getInstance()->getPetName(_ePet);
        strName = MafUtils::format("Lv%d. %s", PetNewManager::getInstance()->getPetLevel(_ePet), strName.c_str());
        
        auto labelText = Label::createWithTTF(strName, GAME_FONT, 28);
        labelText->setPosition(spriteTitle->getContentSize().width / 2, 55);
        labelText->setDimensions(450, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        spriteTitle->addChild(labelText);
    }
}

#pragma mark - show, lock, hide
void PopupToastPet::show(E_PET ePet)
{
    // notification
    auto node = UtilsScene::getInstance()->getRunningScene();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_SCENE_TOAST_PET) == nullptr )
    {
        PopupToastPet::_nCountVisible = 0;
    }
    
    // push
    PopupToastPet::_nCountVisible += 1;
    
    // toast add
    auto action1 = DelayTime::create((PopupToastPet::_nCountVisible - 1) * 1.0);
    auto action2 = CallFunc::create([=](){
        
        //
        auto popup = PopupToastPet::create(ePet);
        node->addChild(popup, ZORDER_SCENE_TOAST_PET, TAG_SCENE_TOAST_PET);
    });
    auto action3 = DelayTime::create(0.5);
    auto action4 = CallFunc::create([=](){
        //
        PopupToastPet::_nCountVisible -= 1;
    });
    auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
    node->runAction(seq);
}

#pragma mark - scheduler
