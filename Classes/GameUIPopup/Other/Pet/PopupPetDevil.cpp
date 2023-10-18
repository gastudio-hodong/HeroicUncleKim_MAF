//
//  PopupPetDevil.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 12/06/2019.
//

#include "PopupPetDevil.h"

#include "Common/ConfigGameNormal.h"
#include "Common/ConfigGameDevil.h"

#include "GameScene/DevilDomScene.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupPetDevil* PopupPetDevil::create(ModelCellDevil::CellDevilPet &celldevil)
{
    PopupPetDevil* pRet = new(std::nothrow) PopupPetDevil();
    if ( pRet && pRet->init(celldevil) )
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
PopupPetDevil::PopupPetDevil(void)
{
    
}

PopupPetDevil::~PopupPetDevil(void)
{
    
}

bool PopupPetDevil::init(ModelCellDevil::CellDevilPet &celldevil)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initUi(celldevil);
    
    return true;
}


void PopupPetDevil::initUi(ModelCellDevil::CellDevilPet &celldevil)
{
    _selectPetDevil = &celldevil;
    
   
    _spriteContainer->setContentSize( Size(674, 778) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _layerContainerParent = Layer::create();
    _layerContainerParent->setContentSize( _spriteContainer->getContentSize());
    _layerContainerParent->setIgnoreAnchorPointForPosition(false);
    _layerContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainerParent->setPosition(_spriteContainer->getPositionX() , _spriteContainer->getPositionY() );
    addChild(_layerContainerParent);
    
    auto lbTitle = Label::createWithTTF("", GAME_FONT, 36);
    if(_selectPetDevil->_ePet != E_PET::PET_NONE )
    {
        _ePet = _selectPetDevil->_ePet;
        lbTitle->setString(celldevil._lbTitle->getString());
        DevilDomScene::getInstance()->_selectPet = _selectPetDevil;
    }
    lbTitle->setAnchorPoint(Vec2(0.5,0.5));
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_layerContainerParent->getContentSize().width/2,_layerContainerParent->getContentSize().height-37.5) );
    _layerContainerParent->addChild(lbTitle);
    
    
    MafNode::MafMenu *menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerParent->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),
                                               Sprite::create("Assets/ui_common/btn_x.png"),
                                               CC_CALLBACK_1(PopupPetDevil::onClickClose, this));
    itemClose->setPosition(Vec2(_layerContainerParent->getContentSize().width-itemClose->getContentSize().width,
                                _layerContainerParent->getContentSize().height-itemClose->getContentSize().height*0.8));
    itemClose->setAnchorPoint(Vec2(0.5,0.5));
    itemClose->setTouchRect(itemClose->getContentSize()*2);
    menu->addChild(itemClose);
    
    const char* strBg[12] = {
        "Assets/battle_bg/normal/bg_1_1_1.png",
        "Assets/battle_bg/normal/bg_1_1_1.png",
        "Assets/battle_bg/normal/bg_1_1_1.png",
        "Assets/battle_bg/normal/bg_1_1_1.png",
        "Assets/battle_bg/normal/bg_1000_3_3.png",
        "Assets/battle_bg/normal/bg_3000_1_1.png",
        "Assets/battle_bg/normal/mine_big_bg.png",
        "Assets/battle_bg/normal/bg_6000_1_1.png",
        "Assets/battle_bg/devil/map_pet_01_2.png",
        "Assets/battle_bg/devil/map_pet_02_2.png",
        "Assets/battle_bg/devil/map_pet_03_2.png",
        "Assets/battle_bg/devil/map_pet_04_2.png",
    };
    std::string strBgSelect = strBg[_ePet-1];
    
    std::string strInfo1 = "";
    std::string strInfo2 = "";
    std::string strSkill = "";
    std::string strEvolution = "";
    
    int positionX = 0;
    int positionY = 0;
    
    switch (_ePet) {
        case E_PET::D_1_PENGUIN:
            strInfo1 = GAME_TEXT("t_pet_d1_detail_penguin_1");
            strInfo2 = GAME_TEXT("t_pet_d1_detail_penguin_2");
            strSkill = GAME_TEXT("t_pet_d1_detail_penguin_3");
            strEvolution = GAME_TEXT("t_pet_d1_detail_penguin_4");
            _objPet = Penguin::create();
            _objPet->setScale(3);
            positionY = -140;
            positionX = 30;
            break;
        case E_PET::D_2_HEDGEHOG:
            strInfo1 = GAME_TEXT("t_pet_d1_detail_hedgehog_1");
            strInfo2 = GAME_TEXT("t_pet_d1_detail_hedgehog_2");
            strSkill = GAME_TEXT("t_pet_d1_detail_hedgehog_3");
            strEvolution = GAME_TEXT("t_pet_d1_detail_hedgehog_4");
            _objPet = Hedgehog::create();
            _objPet->setScale(3);
            positionY = -250;
            break;
        case E_PET::D_3_GATEKEEPER:
            strInfo1 = GAME_TEXT("t_pet_d1_detail_gatekeeper_1");
            strInfo2 = GAME_TEXT("t_pet_d1_detail_gatekeeper_2");
            strSkill = GAME_TEXT("t_pet_d1_detail_gatekeeper_3");
            strEvolution = GAME_TEXT("t_pet_d1_detail_gatekeeper_4");
            _objPet = Gatekeeper::create();
            _objPet->setScale(3);
            positionY = -100;
            positionX = 100;
            break;
        case E_PET::D_4_FLASH:
            strInfo1 = GAME_TEXT("t_pet_d1_detail_flash_1");
            strInfo2 = GAME_TEXT("t_pet_d1_detail_flash_2");
            strSkill = GAME_TEXT("t_pet_d1_detail_flash_3");
            strEvolution = GAME_TEXT("t_pet_d1_detail_flash_4");
            _objPet = Flash::create();
            _objPet->setScale(3);
            positionY = -80;
            break;
        default:
            break;
    }
    
    _sprBack = Sprite::create(strBgSelect);
    _sprBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _sprBack->setPosition(Vec2(_layerContainerParent->getContentSize().width/2,_layerContainerParent->getContentSize().height-75) );
    _sprBack->setScale(3);
    _layerContainerParent->addChild(_sprBack);
    
    auto sprGuideTitle = Sprite::create("Assets/ui/pet/pet_guide_title.png");
    sprGuideTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprGuideTitle->setPosition(Vec2(_layerContainerParent->getContentSize().width*2/3-10,_layerContainerParent->getContentSize().height-100) );
    _layerContainerParent->addChild(sprGuideTitle);
    
    auto lbGuideTitle = Label::createWithTTF(strInfo1, GAME_FONT, 20);
    lbGuideTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbGuideTitle->setAlignment(TextHAlignment::RIGHT);
    lbGuideTitle->setPosition(Vec2(sprGuideTitle->getContentSize().width/2,sprGuideTitle->getContentSize().height/2));
    lbGuideTitle->setColor(Color3B::YELLOW);
    sprGuideTitle->addChild(lbGuideTitle);
    
    auto sprGuideExplain = Sprite::create("Assets/ui/pet/pet_guide_1.png");
    sprGuideExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprGuideExplain->setPosition(Vec2(_layerContainerParent->getContentSize().width*2/3+12,_layerContainerParent->getContentSize().height-135));
    _layerContainerParent->addChild(sprGuideExplain);
    
    auto lbGuideExplain = Label::createWithTTF(strInfo2, GAME_FONT, 20);
    lbGuideExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbGuideExplain->setAlignment(TextHAlignment::CENTER);
    lbGuideExplain->setDimensions(sprGuideExplain->getContentSize().width-40, lbGuideExplain->getContentSize().height);
    lbGuideExplain->setOverflow(Label::Overflow::SHRINK);
    lbGuideExplain->setPosition(Vec2(40,sprGuideExplain->getContentSize().height/2));
    sprGuideExplain->addChild(lbGuideExplain);
    
    auto sprMagicCircle = Sprite::create("Assets/ui/pet/pet_circle1_1.png");
    sprMagicCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprMagicCircle->setPosition(Vec2(_layerContainerParent->getContentSize().width*1/4,_layerContainerParent->getContentSize().height*2/3+15));
    _layerContainerParent->addChild(sprMagicCircle);
    
    _objPet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _objPet->setPosition(Vec2(_layerContainerParent->getContentSize().width*1/4+5+positionX,_layerContainerParent->getContentSize().height+positionY));
    _layerContainerParent->addChild(_objPet);
    
    auto aniMagicCircle = Animation::create();
    aniMagicCircle->setDelayPerUnit(0.15f);
    for (int i = 2; i <= 3; i++)
    {
        aniMagicCircle->addSpriteFrameWithFile(MafUtils::format("Assets/ui/pet/pet_circle1_%d.png",i));
    }
    
    auto sprMagicCircleOverray = Sprite::create("Assets/ui/pet/pet_circle1_2.png");
    sprMagicCircleOverray->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprMagicCircleOverray->setPosition(Vec2(_layerContainerParent->getContentSize().width*1/4,_layerContainerParent->getContentSize().height*2/3+15));
    sprMagicCircleOverray->runAction(RepeatForever::create(Animate::create(aniMagicCircle)));
    _layerContainerParent->addChild(sprMagicCircleOverray);
    
    if(_ePet >= E_PET::D_1_PENGUIN)
    {
        sprMagicCircle->setColor(Color3B(201,32,247));
        sprMagicCircleOverray->setColor(Color3B(201,32,247));
    }
    
    initBottomUi(strSkill);
    strPromoExplain = strEvolution;
}

void PopupPetDevil::initBottomUi(std::string strSkill)
{
    
    int menucount = 1;
    int buttonsize = 152;
    
    const char* strbuttonlabel[] = {
        "t_ui_capability",
        "t_ui_evolution",
        "t_ui_upgrade",
        "t_ui_skin"
    };
    
    if(_ePet < E_PET::D_1_PENGUIN)
    {
        menucount = 4;
    }
    
    //
    auto sprBottomBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_2.png",Rect::ZERO, Rect(3,3,112,120));
    sprBottomBox->setContentSize(Size(_sprBack->getBoundingBox().size.width + 20, 368));
    sprBottomBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprBottomBox->setPosition(_sprBack->getPositionX() - _sprBack->getContentSize().width/2 * 3 - 10,22);
    _layerContainerParent->addChild(sprBottomBox);
    
    //
    for(int i = 0; i < menucount; i++)
    {
        auto spriteOff = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_off.png", Rect::ZERO, Rect(35,25,2,10));
        spriteOff->setContentSize(Size(buttonsize, 64));
        spriteOff->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        {
            auto label = Label::createWithTTF(GAME_TEXT(strbuttonlabel[i]), GAME_FONT, 24);
            label->setPosition( Vec2(spriteOff->getContentSize().width /2, spriteOff->getContentSize().height / 2) );
            label->setDimensions(spriteOff->getContentSize().width * 0.9, label->getContentSize().height);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            spriteOff->addChild(label);
        }
        spriteOff->setPosition(sprBottomBox->getPositionX() + (150 * i), _layerContainerParent->getContentSize().height*7/13 -0.3);
        _layerContainerParent->addChild(spriteOff);
    }
    
    auto menubottom = MafNode::MafMenu::create();
    menubottom->setAnchorPoint(Vec2::ZERO);
    menubottom->setPosition(Vec2::ZERO);
    _layerContainerParent->addChild(menubottom);
    
    for(int i = 0; i< menucount; i++)
    {
        auto spriteOn = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_on.png", Rect::ZERO, Rect(35,25,2,10));
        spriteOn->setContentSize(Size(buttonsize, 64));
        
        MafNode::MafMenuItemSprite* item = MafNode::MafMenuItemSprite::create(spriteOn, nullptr, spriteOn, CC_CALLBACK_1(PopupPetDevil::switchMenu, this));
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        {
            auto label = Label::createWithTTF(GAME_TEXT(strbuttonlabel[i]), GAME_FONT, 24);
            label->setPosition( Vec2(item->getContentSize().width /2, item->getContentSize().height / 2) );
            label->setDimensions(item->getContentSize().width * 0.9, label->getContentSize().height);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            item->addChild(label);
        }
        item->setPosition(_sprBack->getPositionX() - _sprBack->getContentSize().width/2 * 3 + 150*i - 10 , _layerContainerParent->getContentSize().height*7/13 -0.3);
        item->setTag(i);
        item->setOpacity(0);
        menubottom->addChild(item);
        _vecButton.pushBack(item);
        
        auto layer = Layer::create();
        layer->setContentSize(Size(_sprBack->getContentSize().width * 3 + 20,_sprBack->getContentSize().height * 4));
        layer->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        
        sprBottomBox->addChild(layer);
        _vecLayer.pushBack(layer);
    }
    
    // reinforce reddot
    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
    uiReddot->setPosition(_sprBack->getPositionX() - _sprBack->getContentSize().width/2 * 3 + 150 * 2 - 10, _layerContainerParent->getContentSize().height*7/13 + 32);
    uiReddot->setVisible(false);
    _layerContainerParent->addChild(uiReddot);
    {
        if ( _ePet == E_PET::CHICK_1 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_CHICK, uiReddot);
        }
        else if ( _ePet == E_PET::HAMSTER_2 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER, uiReddot);
        }
        else if ( _ePet == E_PET::DRAGON_3 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_DRAGON, uiReddot);
        }
        else if ( _ePet == E_PET::PRINCESS_4 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS, uiReddot);
        }
        else if ( _ePet == E_PET::PURIRING_5 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_PURIRING, uiReddot);
        }
        else if ( _ePet == E_PET::HONG_6 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HONG, uiReddot);
        }
        else if ( _ePet == E_PET::MOLE_7 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_MOLE, uiReddot);
        }
        else if ( _ePet == E_PET::HANDSOME_8 )
        {
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME, uiReddot);
        }
    }
    
    
    //능력 탭 UI
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprBox->setContentSize(Size(574,160));
    sprBox->setPosition(_sprBack->getPositionX() - _sprBack->getContentSize().width/2 * 3 - 15,180);
    _vecLayer.at(0)->addChild(sprBox);
    
    auto lbEffectExplain = Label::createWithTTF(strSkill, GAME_FONT, 22);
    lbEffectExplain->setPosition(Vec2(sprBox->getContentSize().width/2,sprBox->getContentSize().height/2));
    lbEffectExplain->setDimensions(sprBox->getContentSize().width * 0.9, sprBox->getContentSize().height-20);
    lbEffectExplain->setAlignment(TextHAlignment::LEFT,TextVAlignment::CENTER);
    
    lbEffectExplain->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbEffectExplain);
    
    auto sprBox2 = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png");
    sprBox2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprBox2->setContentSize(Size(574,166));
    sprBox2->setPosition(_sprBack->getPositionX() - _sprBack->getContentSize().width/2 * 3 - 15,10);
    _vecLayer.at(0)->addChild(sprBox2);
    
    auto sprGuide = Sprite::create("Assets/ui/pet/pet_guide_2.png");
    sprGuide->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprGuide->setPosition(Vec2(0,sprBox2->getContentSize().height - 10));
    sprBox2->addChild(sprGuide);
    
    auto lbGuide = Label::createWithTTF("", GAME_FONT, 20);
    lbGuide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbGuide->setPositionX(sprGuide->getPositionX()+5);
    
    {// 능력 탭 UI - 마왕성
        lbGuide->setString( GAME_TEXT("t_ui_promo") );
    
        sprGuide->addChild(lbGuide);
        
        int petLevel = PetNewDevilManager::getInstance()->getPetLevel(_selectPetDevil->_ePet);
        int grade = PetNewDevilManager::getInstance()->getPetGrade(_selectPetDevil->_ePet);
        
        auto lbGuideExplain = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_equip_msg_5",grade),GAME_FONT,20);
        
        lbGuideExplain->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        lbGuideExplain->setPositionX(lbGuide->getPositionX()+10);
        lbGuideExplain->setPositionY(lbGuide->getPositionY()-40);
        sprGuide->addChild(lbGuideExplain);
        
        MafNode::MafMenu* menu = MafNode::MafMenu::create();
        menu->setPosition(0,0);
        sprBottomBox->addChild(menu);
        
        auto btnPromoExplain = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/pet/btn_pet_info.png"),
                                                         Sprite::create("Assets/ui/pet/btn_pet_info.png"),
                                                         CC_CALLBACK_1(PopupPetDevil::showPromoExplainPopup, this));
        
        btnPromoExplain->setPosition(sprBottomBox->getContentSize().width*0.3, 155);
        btnPromoExplain->setTouchRect(btnPromoExplain->getContentSize()*2);
        menu->addChild(btnPromoExplain);
   
        
        if ( petLevel == PetNewDevilManager::getInstance()->getPetLevelMax(_selectPetDevil->_ePet, grade) )
        {
            if ( grade < C_MAX_PET_GRADE && _selectPetDevil->_ePet != E_PET::D_3_GATEKEEPER )
            {
                int spendCoin = PetNewDevilManager::getInstance()->getPetGradeUpgradePrice(_selectPetDevil->_ePet, grade);
                
                auto item = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg.png"),
                                                      Sprite::create("Assets/ui_common/btn_upgrade_bg.png"),
                                                      CC_CALLBACK_1(PopupPetDevil::showPopupPetGrade, this));
                item->setAnchorPoint(Vec2(0.5,0));
                item->setScale(0.7);
                item->setPosition(Vec2(sprBottomBox->getContentSize().width*0.8, 50));
                menu->addChild(item);
                DevilDomScene::getInstance()->_itemPetGrade = item;
                DevilDomScene::getInstance()->_itemPetGrade->setVisible(false);
                
                auto label = Label::createWithTTF(GAME_TEXT("t_ui_promo"), GAME_FONT, 28);
                label->setAnchorPoint(Vec2(0.5,0.5));
                label->setAlignment(TextHAlignment::CENTER);
                label->setPosition(Vec2(item->getContentSize().width*0.5,item->getContentSize().height*0.72));
                item->addChild(label);
                
                auto sprCoin = Sprite::create("Assets/icon/icon_coin_2.png");
                sprCoin->setPosition(Vec2(item->getContentSize().width*0.30,item->getContentSize().height*0.26));
                item->addChild(sprCoin,1);
                
                auto lbCoin = MafNode::createBmFont(MafUtils::toString(spendCoin), GAME_BM_FONT_AL, 30);
                lbCoin->setAnchorPoint(Vec2(0,0.5));
                lbCoin->setColor(COLOR_COSTUME_COIN);
                lbCoin->setAlignment(TextHAlignment::LEFT);
                lbCoin->setPosition(Vec2(sprCoin->getContentSize().width*getScale() + 15,
                                         sprCoin->getContentSize().height*0.5));
                sprCoin->addChild(lbCoin);
                
                requestCheckCostumeCoin();
            }
        }
        
    }
    
    _vecButton.at(0)->setOpacity(255);
}

#pragma mark - set, get

#pragma mark - click
void PopupPetDevil::onClickClose(Ref *sender)
{
    hide();
}

void PopupPetDevil::switchMenu(Ref* sender)
{
    MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)sender;
    int selected = item->getTag();
    for(int i = 0;i<_vecLayer.size();i++)
    {
        _vecLayer.at(i)->setVisible(false);
        _vecButton.at(i)->setOpacity(0);
    }
    _vecButton.at(selected)->setOpacity(255);
    _vecLayer.at(selected)->setVisible(true);
}



#pragma mark -
void PopupPetDevil::showPopupPetGrade(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupDefault::create("", GAME_TEXT("t_ui_pet_msg_11"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        DevilDomScene::getInstance()->requestPetGradeCostumeCoin();
    });
    popup->show();
}

void PopupPetDevil::showPromoExplainPopup(cocos2d::Ref* pSender)
{
    
    auto popupExplain = PopupDefault::create(GAME_TEXT("t_ui_promo"), strPromoExplain);
    popupExplain->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popupExplain->show();
}

#pragma mark - callback

#pragma mark - network
void PopupPetDevil::requestCheckCostumeCoin()
{
    PopupLoading::show();
    
    std::string url = "/costume/currentcoin";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        ResultCheckCostumeCoin(response,dd);
    };
    req->send(cb);
}

void PopupPetDevil::ResultCheckCostumeCoin(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bSuc = true;
    if (!response)
        bSuc = false;
    if(!response->isSucceed())
        bSuc = false;
    if(response->getResponseCode() != 200)
        bSuc = false;
    
    
    auto current = DevilDomScene::getInstance();
    if ( current == nullptr || current->isRunning() == false )
    {
        return;
    }
    
    if(!bSuc)
    {
        return;
    }
    
    if(data.length()>0){
        rapidjson::Document jsondoc;
        jsondoc.ParseInsitu<0>((char*)(const char*)data.c_str()).HasParseError();
        auto costumeCoin = jsondoc[("_coin")].GetInt();
        
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, costumeCoin);
        current->_itemPetGrade->setVisible(true);
    }
}
