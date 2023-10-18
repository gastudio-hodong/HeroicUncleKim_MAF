//
//  LegendMake.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 16/10/2019.
//

#include "PopupLegendMake.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupLegendMake* PopupLegendMake::create(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    PopupLegendMake* pRet = new(std::nothrow) PopupLegendMake();
    if ( pRet && pRet->init(eType, eIdx) )
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

PopupLegendMake::PopupLegendMake(void) :
_eType(E_COSTUME::HEAD),
_eIdx(E_COSTUME_IDX::IDX_1_DEFAULT),
_callbackResult(nullptr),

_layerContainerParent(nullptr),
_layerContainerContents(nullptr)
{
    
}

PopupLegendMake::~PopupLegendMake(void)
{
    
}

bool PopupLegendMake::init(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    _eType = eType;
    _eIdx = eIdx;
    
    //
    initUi();
    
    return true;
}

#pragma mark - init
void PopupLegendMake::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(729, 848) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _spriteContainer->setPositionY(getContentSize().height*0.39);
    
    _layerContainerParent = Layer::create();
    _layerContainerParent->setContentSize( _spriteContainer->getContentSize() );
    _layerContainerParent->setIgnoreAnchorPointForPosition(false);
    _layerContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerParent->setPosition( _spriteContainer->getContentSize().width / 2, 0 );
    _spriteContainer->addChild(_layerContainerParent);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_costume_msg_14"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _layerContainerParent->getContentSize().height - 37.5) );
    labelTitle->setDimensions(labelTitle->getContentSize().width * 0.75, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    _layerContainerParent->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerParent->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupLegendMake::onClickClose, this));
    itemClose->setPosition( Vec2(_layerContainerParent->getContentSize().width - itemClose->getContentSize().width, _layerContainerParent->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
       
    //
    uiContents();
}

void PopupLegendMake::uiContents()
{
    if ( _layerContainerContents == nullptr )
    {
        _layerContainerContents = Layer::create();
        _layerContainerContents->setContentSize(_spriteContainer->getContentSize());
        _layerContainerContents->setIgnoreAnchorPointForPosition(false);
        _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerContents->setPosition( _spriteContainer->getContentSize().width / 2, 0 );
        _layerContainerParent->addChild(_layerContainerContents);
    }
    _layerContainerContents->removeAllChildren();
    
    // obj
    auto objCostume = CostumeManager::getInstance()->getCostume(_eType, _eIdx);
    
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerContents->addChild(menu);
    
    auto itemMake = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_big.png"), Sprite::create("Assets/ui_common/btn_big.png"), nullptr, CC_CALLBACK_1(PopupLegendMake::onMakeLegend, this));
    itemMake->setPosition(Vec2(_layerContainerContents->getContentSize().width*0.5, _layerContainerContents->getContentSize().height*0.1));
    menu->addChild(itemMake);
    
    Sprite* spDisable = Sprite::create("Assets/ui/costume/btn_big_disable.png");
    spDisable->setPosition(itemMake->getContentSize()/2);
    itemMake->addChild(spDisable);
    
    auto spButtonIcon1 = Sprite::create("Assets/icon/icon_coin_2.png");
    spButtonIcon1->setPosition(Vec2(0, 0));
    itemMake->addChild(spButtonIcon1);
    
    auto spButtonIcon2 = Sprite::create("Assets/icon/evilcrystal.png");
    spButtonIcon2->setPosition(Vec2(0, 0));
    itemMake->addChild(spButtonIcon2);
    
    auto lbButtonPrice1 = Label::createWithTTF("", GAME_FONT, 24);
    lbButtonPrice1->setColor(COLOR_COIN);
    lbButtonPrice1->setPosition(Vec2(0, 0));
    lbButtonPrice1->setAnchorPoint(Vec2(0, 0.5));
    itemMake->addChild(lbButtonPrice1);
    
    auto lbButtonPrice2 = Label::createWithTTF("", GAME_FONT, 24);
    lbButtonPrice2->setColor(COLOR_COIN);
    lbButtonPrice2->setPosition(Vec2(0, 0));
    lbButtonPrice2->setAnchorPoint(Vec2(0, 0.5));
    itemMake->addChild(lbButtonPrice2);
    
    auto lbNext = Label::createWithTTF("", GAME_FONT, 24);
    lbNext->setColor(COLOR_TITLE);
    lbNext->setPosition(Vec2(0, 0));
    itemMake->addChild(lbNext);
    
    
    int nLevel = CostumeManager::getInstance()->getLegendLevel(_eType);
    if ( nLevel >= DataManager::GetCostumeLegendMaxLevel() )
    {
        //만렙
        itemMake->setEnabled(false);
        spDisable->setVisible(false);

        spButtonIcon1->setVisible(false);
        spButtonIcon2->setVisible(false);
        lbButtonPrice1->setVisible(false);
        lbButtonPrice2->setVisible(false);
        
        lbNext->setPosition(Vec2(itemMake->getContentSize().width/2, itemMake->getContentSize().height/2));
        lbNext->setString("MAX");
        lbNext->setDimensions(itemMake->getContentSize().width * 0.9, lbNext->getContentSize().height);
        lbNext->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbNext->setOverflow(Label::Overflow::SHRINK);
    }
    else
    {
        //제작 및 업그레이드
        
        lbNext->setPosition(Vec2(itemMake->getContentSize().width/2, itemMake->getContentSize().height*0.7));
        spButtonIcon1->setPosition(Vec2(itemMake->getContentSize().width*0.15, itemMake->getContentSize().height*0.3));
        spButtonIcon2->setPosition(Vec2(itemMake->getContentSize().width*0.6, itemMake->getContentSize().height*0.3));
        lbButtonPrice1->setPosition(spButtonIcon1->getPosition().x + 20, spButtonIcon1->getPosition().y);
        lbButtonPrice2->setPosition(spButtonIcon2->getPosition().x + 20, spButtonIcon2->getPosition().y);
        
        double cur = DataManager::GetCostumeLegendEffect(objCostume->getType(), nLevel);
        double nxt = DataManager::GetCostumeLegendEffect(objCostume->getType(), nLevel+1);

        lbNext->setString(MafUtils::format("+ %.0f%%", (nxt-cur)*100));
        lbButtonPrice1->setString(MafUtils::format("x%d", objCostume->getPrice()));
        lbButtonPrice2->setString(MafUtils::format("x%d",DataManager::GetCostumeLegendMaterial(objCostume->getType())));

        if ( nLevel >= CostumeManager::getInstance()->getLegendEnableLevel(objCostume->getType()) )
        {
            spDisable->setVisible(true);
            itemMake->setEnabled(false);
            spButtonIcon1->setVisible(false);
            spButtonIcon2->setVisible(false);
            lbButtonPrice1->setVisible(false);
            lbButtonPrice2->setVisible(false);
            
            std::string name[C_COUNT_COSTUME_TYPE+1]={
                "",
                GAME_TEXT("t_ui_costume_helmet"),
                GAME_TEXT("t_ui_costume_armor"),
                GAME_TEXT("t_ui_costume_cape"),
                GAME_TEXT("t_ui_costume_gloves"),
                GAME_TEXT("t_ui_costume_shoes"),
                GAME_TEXT("t_ui_costume_jewel")
            };
            
            
            int checkLevel;
            if (nLevel == 0) {
                checkLevel = DataManager::GetCostumeLegendTermsCount(objCostume->getType())+nLevel;
            }else{
                checkLevel = DataManager::GetCostumeLegendTermsCount(objCostume->getType())+nLevel+1;
            }
            
            lbNext->setPosition(itemMake->getContentSize()/2);
            lbNext->setString( GAME_TEXTFORMAT("t_ui_costume_msg_13", name[objCostume->getType()].c_str(), checkLevel) );
            lbNext->setDimensions(itemMake->getContentSize().width * 0.9, lbNext->getContentSize().height);
            lbNext->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbNext->setOverflow(Label::Overflow::SHRINK);
        }
        else
        {
            itemMake->setEnabled(true);
            spDisable->setVisible(false);
        }
    }
    
    Sprite* spCostumeBg = Sprite::create("Assets/ui/mine/week_weaponbox.png");
    spCostumeBg->setPosition(_layerContainerContents->getContentSize().width/2, _layerContainerContents->getContentSize().height-200);
    _layerContainerContents->addChild(spCostumeBg);
    
    Sprite* spCostume = Sprite::create(CostumeManager::getInstance()->getIconPath(objCostume->getType(), objCostume->getIdx()));
    spCostume->setPosition(spCostumeBg->getContentSize().width/2, spCostumeBg->getContentSize().height/2);
    spCostume->getTexture()->setTexParameters(texParams);
    spCostume->setScale(spCostumeBg->getContentSize().width / spCostume->getContentSize().width);
    spCostumeBg->addChild(spCostume);
    
    Sprite* spCostumeLine = Sprite::create("Assets/ui/mine/week_weaponline.png");
    spCostumeLine->setPosition(spCostumeBg->getContentSize().width/2, -52);
    spCostumeBg->addChild(spCostumeLine);
    
    auto lbName = Label::createWithTTF("", GAME_FONT, 28);
    lbName->setString(DataManager::GetCostumeLegendName(_eType, _eIdx));
    lbName->setColor(Color3B(212,34,204));
    lbName->setPosition(Vec2(spCostumeBg->getContentSize().width/2, -28));
    spCostumeBg->addChild(lbName);

    auto lbDesc = Label::createWithTTF("", GAME_FONT, 24);
    lbDesc->setString(DataManager::GetCostumeLegendDesc(_eType, _eIdx));
    lbDesc->setPosition(Vec2(spCostumeBg->getContentSize().width/2, -110));
    lbDesc->setAlignment(TextHAlignment::CENTER);
    spCostumeBg->addChild(lbDesc);
    

    std::string stSetTitle = GAME_TEXTFORMAT("t_ui_costume_msg_15", CostumeManager::getInstance()->getLegendCount());
    std::string strSetDesc[5]{
         GAME_TEXT("t_ui_costume_msg_16"),
         GAME_TEXT("t_ui_costume_msg_17"),
         GAME_TEXT("t_ui_costume_msg_18"),
         GAME_TEXT("t_ui_costume_msg_19"),
         GAME_TEXT("t_ui_costume_msg_20")
    };
    
    Label* lbSetTitle = Label::createWithTTF(stSetTitle, GAME_FONT, 24);
    lbSetTitle->setPosition(Vec2(_layerContainerContents->getContentSize().width/2, _layerContainerContents->getContentSize().height*0.48));
    lbSetTitle->setColor(Color3B(212,34,204));
    _layerContainerContents->addChild(lbSetTitle);
    
    for (int i=0; i<5; i++) {
        Label* lbSetDesc = Label::createWithTTF(strSetDesc[i], GAME_FONT, 22);
        lbSetDesc->setPosition(Vec2(_layerContainerContents->getContentSize().width/2, lbSetTitle->getPosition().y - 50 - (40*i)));
        _layerContainerContents->addChild(lbSetDesc);
        
        if (i < CostumeManager::getInstance()->getLegendCount()) {
            lbSetDesc->setColor(Color3B::WHITE);
        }else{
            lbSetDesc->setColor(Color3B(100,100,100));
        }
    }
}

#pragma mark - set, get, sort, add
void PopupLegendMake::setCallbackResult(const std::function<void(bool)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupLegendMake::onClickClose(Ref* sender)
{
    hide();
}

void PopupLegendMake::onMakeLegend(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);

    if ( _eType != E_COSTUME::JEWEL )
    {// 장신구 제외
        int nNeedCount = DataManager::GetCostumeLegendTermsCount(_eType);
        int nNowCount = CostumeManager::getInstance()->getExistTypeCount(_eType);
        if ( nNowCount < nNeedCount )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_24"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    
    int nLevelMax = DataManager::GetCostumeLegendMaxLevel();
    int nLevel = CostumeManager::getInstance()->getLegendLevel(_eType);
    if ( nLevel >= nLevelMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_msg_33"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( nLevel >= CostumeManager::getInstance()->getLegendEnableLevel(_eType) )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_msg_34"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto objCostume = CostumeManager::getInstance()->getCostume(_eType, _eIdx);
    
    int nPrice = objCostume->getPrice();
    int nPriceBong = DataManager::GetCostumeLegendMaterial(_eType);
    
    int nCurrencyCoin = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
    int nCurrencyBong = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::MATERIAL).c_str());
    if ( nPrice > nCurrencyCoin )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
        popup->show();
        return;
    }
    
    if ( nPriceBong > nCurrencyBong )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::MATERIAL);
        popup->show();
        return;
    }

    PopupLoading::show();
    CostumeManager::getInstance()->requestLegendBuy(_eType, _eIdx, nPrice, nPriceBong, CC_CALLBACK_1(PopupLegendMake::callbackBuy, this));
}

#pragma mark - callback
void PopupLegendMake::callbackBuy(bool bResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    uiContents();
    
    //
    if ( _callbackResult != nullptr )
    {
        _callbackResult(true);
    }
}
