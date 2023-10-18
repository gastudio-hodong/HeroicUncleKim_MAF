//
//  PopupEquipDetail.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#include "PopupEquipDetail.h"

#include "GameScene/DevilDomScene.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupEquipDetail* PopupEquipDetail::create(ModelCellDevil::CellDevilEquip *cell)
{
    PopupEquipDetail* pRet = new(std::nothrow) PopupEquipDetail(cell);
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

PopupEquipDetail::PopupEquipDetail(ModelCellDevil::CellDevilEquip *cell) :
_layerContainerParent(nullptr),
_layerContainerContents(nullptr),
_itemReinforceEvolution(nullptr)
{
    _cell = cell;
}

PopupEquipDetail::~PopupEquipDetail(void)
{
    
}

bool PopupEquipDetail::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initUi();
    
    return true;
}
#pragma mark - table
void PopupEquipDetail::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}
Size PopupEquipDetail::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 120);
}

extension::TableViewCell* PopupEquipDetail::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    idx = table->getDataSource()->numberOfCellsInTableView(table) - idx - 1;
    
    
    E_EQUIP_OPTION option;

     if(idx%2==0){
         if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
             option = E_EQUIP_OPTION::E_D_CRITICAL;
         else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
             option = E_EQUIP_OPTION::E_D_QUEST_TIME;
         else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_BODY)
             option = E_EQUIP_OPTION::E_D_SPEED;
         else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
             option = E_EQUIP_OPTION::E_D_CRITICAL;
     }
     else{
         if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
             option = E_EQUIP_OPTION::E_D_DMG;
         else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
             option = E_EQUIP_OPTION::E_D_QUEST_REWARD;
         else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_BODY)
             option = E_EQUIP_OPTION::E_D_KEY;
         else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
             option = E_EQUIP_OPTION::E_D_CRI_DMG;
     }
         
     int needLevel = 50*(idx+1);
     E_EQUIP_IDX needEquipIdx = (E_EQUIP_IDX)(idx+1);
     auto explain = DataManager::GetEquipOptionExplainDevil(_cell->_eEquipType, needEquipIdx, needLevel, option);
     //등급제한 구하기.
     if(idx<3)         needEquipIdx = E_EQUIP_IDX::E_D_IDX_1;
     else if(idx<5)    needEquipIdx = E_EQUIP_IDX::E_D_IDX_3;
     else if(idx<7)    needEquipIdx = E_EQUIP_IDX::E_D_IDX_5;
     else if(idx<9)    needEquipIdx = E_EQUIP_IDX::E_D_IDX_7;
     else            needEquipIdx = E_EQUIP_IDX::E_D_IDX_10;
     auto strCondition = GAME_TEXTFORMAT("t_ui_equip_msg_2", (int)needEquipIdx, needLevel);
     
     bool open = false;
     int level = MafAes256::XorDecryptInt(_cell->_levelEncrypt);
     if(_cell->_eEquipIdx >= needEquipIdx && level >= needLevel)
         open = true;
     
     auto icon = Sprite::create("Assets/ui_devil/weapon/equip_box.png");
     icon->setPosition(Vec2(_layerContainerContents->getContentSize().width*0.20,0));
     icon->setAnchorPoint(Vec2(1,0));
     cell->addChild(icon);
     
     std::string name = "Assets/ui_devil/weapon/";
     if(_cell->_eEquipType==E_EQUIP_TYPE::E_D_WEAPON)
         name.append("w_icon_");
     else if(_cell->_eEquipType==E_EQUIP_TYPE::E_D_BODY)
         name.append("b_icon_");
     else if(_cell->_eEquipType==E_EQUIP_TYPE::E_D_HEAD)
         name.append("h_icon_");
     else if(_cell->_eEquipType==E_EQUIP_TYPE::E_D_CAPE)
         name.append("c_icon_");
     if(idx%2==0)  name.append("1.png");
     else        name.append("2.png");
     
     auto iconSpr = Sprite::create(name);
     iconSpr->setScale(DevilScale);
     iconSpr->getTexture()->setTexParameters(texParams);
     iconSpr->setPosition(icon->getContentSize()*0.5);
     iconSpr->setAnchorPoint(Vec2(0.5,0.5));
     if(open)    iconSpr->setOpacity(255);
     else        iconSpr->setOpacity(120);
     icon->addChild(iconSpr);
         
     int num = 1;
     if(idx<2) num = 1;
     else if(idx<4) num = 2;
     else if(idx<6) num = 3;
     else if(idx<8) num = 4;
     else if(idx<10) num = 5;
     name.clear();
     name = MafUtils::format("Assets/ui_devil/weapon/equip_num_%d.png",num);
     auto iconLevel = Sprite::create(name);
     iconLevel->setScale(DevilScale);
     iconLevel->getTexture()->setTexParameters(texParams);
     iconLevel->setPosition(icon->getContentSize().width-8,8);
     iconLevel->setAnchorPoint(Vec2(1,0));
     if(open)    iconLevel->setOpacity(255);
     else        iconLevel->setOpacity(120);
     icon->addChild(iconLevel);
     

     auto lbEffect = Label::createWithTTF(explain, GAME_FONT, 28);
     lbEffect->setAnchorPoint(Vec2(0,1));
     if(!open)
         lbEffect->setOpacity(120);
     
     lbEffect->setColor(Color3B::WHITE);
     lbEffect->setAlignment(TextHAlignment::LEFT);
     lbEffect->setPosition(Vec2(icon->getContentSize().width + 10,icon->getContentSize().height - 5));
     icon->addChild(lbEffect);
     
     
     auto lbCondition = Label::createWithTTF(strCondition, GAME_FONT, 25);
     lbCondition->setColor(Color3B::WHITE);
     lbCondition->setAnchorPoint(Vec2(0,0));
     lbCondition->setPosition(Vec2(icon->getContentSize().width + 10,40));
     lbCondition->setDimensions(table->getContentSize().width - 180, lbCondition->getContentSize().height);
     lbCondition->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
     lbCondition->setOverflow(Label::Overflow::SHRINK);
     icon->addChild(lbCondition);
     
     if(!open)
         lbCondition->setOpacity(120);
     
     
    
    return cell;
}

ssize_t PopupEquipDetail::numberOfCellsInTableView(extension::TableView *table)
{
    return 10;
}

#pragma mark - init

void PopupEquipDetail::initUi()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // size
    _spriteContainer->setContentSize( Size(729,visibleSize.height-DevilDomScene::getInstance()->_sizeMapPattern.height) );
    _spriteContainer->setAnchorPoint(Vec2(0.5,0));
    _spriteContainer->setPositionY(1);

    
    _layerContainerParent = Layer::create();
    _layerContainerParent->setContentSize( _spriteContainer->getContentSize() );
    _layerContainerParent->setIgnoreAnchorPointForPosition(false);
    _layerContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerParent->setPosition( _spriteContainer->getContentSize().width / 2, 0 );
    _spriteContainer->addChild(_layerContainerParent);
      
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_equip"), GAME_FONT, 36);
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
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEquipDetail::onClickClose, this));
    itemClose->setPosition( Vec2(_layerContainerParent->getContentSize().width - itemClose->getContentSize().width, _layerContainerParent->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);

    
    uiContents();
    
}
void PopupEquipDetail::uiContents()
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
    
    _menuBottom = MafNode::MafMenu::create();
    _menuBottom->setAnchorPoint(Vec2::ZERO);
    _menuBottom->setPosition(Vec2::ZERO);
    _layerContainerContents->addChild(_menuBottom);
    
    auto reinforceLv = UserInfoManager::getInstance()->getEquipReinforceLevelDevil(_cell->_eEquipType);
    
    auto iconBox = Sprite::create("Assets/ui_common/box_quest.png");
    iconBox->setPosition(Vec2(_layerContainerContents->getContentSize().width*0.20,_layerContainerContents->getContentSize().height*0.88));
    iconBox->setAnchorPoint(Vec2(1,1));
    _layerContainerContents->addChild(iconBox);
        
    std::string str;
    if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
        str = MafUtils::format("Assets/icon_equip/icon_weapon_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
        str = MafUtils::format("Assets/icon_equip/icon_head_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_BODY)
        str = MafUtils::format("Assets/icon_equip/icon_body_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
        str = MafUtils::format("Assets/icon_equip/icon_cape_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    _sprIcon = Sprite::create(str);
    _sprIcon->setPosition(Vec2(iconBox->getContentSize().width/2,iconBox->getContentSize().height/2));
    iconBox->addChild(_sprIcon,1);
    
    std::string strName = MafUtils::format("%s",_cell->_lbTitle->getString().c_str());
    _lbName = Label::createWithTTF(strName, GAME_FONT, 32);
    _lbName->setAnchorPoint(Vec2(0,1));
    _lbName->setAlignment(TextHAlignment::LEFT);
    _lbName->setColor(COLOR_COIN);
    _lbName->setPosition(Vec2(iconBox->getPosition().x + 20, iconBox->getPosition().y - 2));
    _layerContainerContents->addChild(_lbName);
        
    std::string strRein = GAME_TEXTFORMAT("t_ui_equip_msg_1", (int)_cell->_eEquipIdx, reinforceLv);
    _lbRein = Label::createWithTTF(strRein, GAME_FONT, 28);
    _lbRein->setAnchorPoint(Vec2(0,0.5));
    _lbRein->setAlignment(TextHAlignment::LEFT);
    _lbRein->setPosition(Vec2(iconBox->getPosition().x  + 20, iconBox->getPosition().y - iconBox->getContentSize().height*0.5));
    _layerContainerContents->addChild(_lbRein);
        
    std::string strEx;
    if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
        strEx = GAME_TEXT("t_equip_d2_contents_weapon");
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
        strEx = GAME_TEXT("t_equip_d2_contents_head");
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_BODY)
        strEx = GAME_TEXT("t_equip_d2_contents_body");
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
        strEx = GAME_TEXT("t_equip_d2_contents_cape");
        
    std::string strEffect = MafUtils::format("%s : %s",strEx.c_str(), _cell->_lbMoney->getString().c_str());
    _lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 30);
    _lbEffect->setAnchorPoint(Vec2(0,0));
    _lbEffect->setAlignment(TextHAlignment::LEFT);
    _lbEffect->setPosition(Vec2(iconBox->getPosition().x + 20, iconBox->getPosition().y - iconBox->getContentSize().height + 2));
    _layerContainerContents->addChild(_lbEffect);
        
    Size scSize = Size(_layerContainerContents->getContentSize().width,_layerContainerContents->getContentSize().height*0.55);
    _table = extension::TableView::create(this,scSize);
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setPosition(Vec2(0,140));
    _table->setClippingToBounds(true);
    _table->setBounceable(false);
    _layerContainerContents->addChild(_table);
    
    uiButton();
}
void PopupEquipDetail::uiButton()
{
    _menuBottom->removeAllChildren();
    _lbKeyRein = nullptr;
    
    auto reinforceLv = UserInfoManager::getInstance()->getEquipReinforceLevelDevil(_cell->_eEquipType);

    if(reinforceLv <C_MAX_EQUIP_REINFORCE_DEVIL)
    {
        auto needKey = DataManager::GetEquipReinforceKeyDevil(_cell->_eEquipIdx, reinforceLv);
        auto myKey = ItemsMoneyManager::getInstance()->getDevilKey();
        
        //
        auto item = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg.png"), nullptr, CC_CALLBACK_1(PopupEquipDetail::onEquipReinforce, this));
        item->setAnchorPoint(Vec2(0.5,0));
        item->setPosition(Vec2(_layerContainerContents->getContentSize().width*0.5, 10));
        _menuBottom->addChild(item);
        if ( MafUtils::compareStr(myKey, needKey) == false )
        {
            ((Sprite*)item->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_max.png");
        }
               
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_upgrade"), GAME_FONT, 28);
        label->setPosition(Vec2(item->getContentSize().width*0.5,item->getContentSize().height*0.70));
        label->setDimensions(item->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        item->addChild(label);
               
        auto sprKey = Sprite::create("Assets/icon/icon_gold_2.png");
        sprKey->setPosition(Vec2(item->getContentSize().width*0.5,item->getContentSize().height*0.25));
        sprKey->setAnchorPoint(Vec2(1,0.5));
        item->addChild(sprKey,1);
               
        needKey = MafUtils::convertNumberToShort(needKey);
        _lbKeyRein = MafNode::createBmFont(needKey, GAME_BM_FONT_AL, 30);
        _lbKeyRein->setAnchorPoint(Vec2(0,0.5));
        _lbKeyRein->setColor(COLOR_D_KEY);
        _lbKeyRein->setAlignment(TextHAlignment::LEFT);
        _lbKeyRein->setPosition(Vec2(sprKey->getContentSize().width - 10, sprKey->getContentSize().height*0.5));
        sprKey->addChild(_lbKeyRein);
    }
    else
    {
        if ( _cell->_eEquipIdx < C_COUNT_EQUIP_DEVIL )
        {
            auto spend = DataManager::GetEquipEvolutionSpendDevil(_cell->_eEquipIdx, reinforceLv);
            auto spendKey = MafUtils::doubleToString(spend.devilKey);
            auto spendCoin = MafUtils::toString(spend.costumeCoin);
            auto nCurrencyKey = ItemsMoneyManager::getInstance()->getDevilKey();
            auto nCurrencyCoin = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
            
            //
            auto item = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg.png"),
                                                Sprite::create("Assets/ui_common/btn_upgrade_bg.png"),
                                                [=](Ref* sender){
                                                       
                auto popup = PopupDefault::create(_cell->_lbTitle->getString().c_str(), GAME_TEXT("t_ui_equip_msg_3"));
                popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
                popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
                    requestEvolutionCostumeCoin();
                });
                popup->show();
            });
            item->setAnchorPoint(Vec2(0.5,0));
            item->setPosition(Vec2(_layerContainerContents->getContentSize().width*0.5, 10));
            _menuBottom->addChild(item);
            _itemReinforceEvolution = item;
            _itemReinforceEvolution->setVisible(false);
            if( MafUtils::compareStr(nCurrencyKey, spendKey) == false || MafUtils::compareStr(nCurrencyCoin, spendCoin) == false )
            {
                ((Sprite*)item->getNormalImage())->setTexture("Assets/ui_common/btn_upgrade_max.png");
            }
                
                   
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_promo"), GAME_FONT, 28);
            label->setAnchorPoint(Vec2(0.5,0.5));
            label->setAlignment(TextHAlignment::CENTER);
            label->setPosition(Vec2(item->getContentSize().width*0.5,item->getContentSize().height*0.75));
            item->addChild(label);
                   
            
               
            //
            spendKey = MafUtils::convertNumberToShort(spendKey);
            spendCoin = MafUtils::convertNumberToShort(spendCoin);
            
            //
            auto sprKey = Sprite::create("Assets/icon/icon_gold_2.png");
            sprKey->setPosition(Vec2(item->getContentSize().width*0.15,item->getContentSize().height*0.25));
            item->addChild(sprKey,1);
            
            auto lbKey = MafNode::createBmFont(spendKey, GAME_BM_FONT_AL, 20);
            lbKey->setAnchorPoint(Vec2(0,0.5));
            lbKey->setColor(COLOR_D_KEY);
            lbKey->setPosition(sprKey->getPositionX() + sprKey->getContentSize().width / 2 - 12, sprKey->getPositionY());
            item->addChild(lbKey);
                   
            auto sprCoin = Sprite::create("Assets/icon/icon_coin_2.png");
            sprCoin->setPosition(Vec2(item->getContentSize().width*0.7,item->getContentSize().height*0.25));
            item->addChild(sprCoin,1);
               
            auto lbCoin = MafNode::createBmFont(spendCoin, GAME_BM_FONT_AL, 20);
            lbCoin->setAnchorPoint(Vec2(0,0.5));
            lbCoin->setColor(COLOR_COSTUME_COIN);
            lbCoin->setPosition(sprCoin->getPositionX() + sprCoin->getContentSize().width / 2 - 12, sprCoin->getPositionY());
            item->addChild(lbCoin);
               
            requestCheckCostumeCoin();
        }
    }
}
#pragma mark - draw
void PopupEquipDetail::redrawContents()
{
    auto reinforceLv = UserInfoManager::getInstance()->getEquipReinforceLevelDevil(_cell->_eEquipType);
    
    std::string str;
    if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
        str = MafUtils::format("Assets/icon_equip/icon_weapon_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
        str = MafUtils::format("Assets/icon_equip/icon_head_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_BODY)
        str = MafUtils::format("Assets/icon_equip/icon_body_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
        str = MafUtils::format("Assets/icon_equip/icon_cape_%.2d.png",UserInfoManager::getInstance()->getEquipDevil(_cell->_eEquipType));
    
    _sprIcon->setTexture(str);
    
    std::string strName = MafUtils::format("%s",_cell->_lbTitle->getString().c_str());
    _lbName->setString(strName);
    
    std::string strRein = GAME_TEXTFORMAT("t_ui_equip_msg_1", (int)_cell->_eEquipIdx, reinforceLv);
    _lbRein->setString(strRein);
    
    std::string strEx;
    if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_WEAPON)
        strEx = GAME_TEXT("t_equip_d2_contents_weapon");
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_HEAD)
        strEx = GAME_TEXT("t_equip_d2_contents_head");
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_BODY)
        strEx = GAME_TEXT("t_equip_d2_contents_body");
    else if(_cell->_eEquipType == E_EQUIP_TYPE::E_D_CAPE)
        strEx = GAME_TEXT("t_equip_d2_contents_cape");
    std::string strEffect = MafUtils::format("%s : %s",strEx.c_str(), _cell->_lbMoney->getString().c_str());
    _lbEffect->setString(strEffect);
    
    if(reinforceLv <C_MAX_EQUIP_REINFORCE_DEVIL)
    {
        if(_lbKeyRein == nullptr)
        {
            uiButton();
        }
        else
        {
            auto key = DataManager::GetEquipReinforceKeyDevil(_cell->_eEquipIdx, reinforceLv);
            key = MafUtils::convertNumberToShort(key);
            _lbKeyRein->setString(key);
        }
    }
    else if(_cell->_eEquipIdx < C_COUNT_EQUIP_DEVIL)//강화에서 승급으로 넘어갈때는 무조건 새로 만들어줘야하므로 키값만 넣을 필요가 없음
    {
        uiButton();
    }
    
    _table->reloadData();

}
#pragma mark - click
void PopupEquipDetail::onClickClose(Ref* sender)
{
    hide();
}
void PopupEquipDetail::onEquipReinforce(cocos2d::Ref* pSender){
    
    auto reinforceLv = UserInfoManager::getInstance()->getEquipReinforceLevelDevil(_cell->_eEquipType);
    if(reinforceLv >= C_MAX_EQUIP_REINFORCE_DEVIL)
        return;
    
    auto needKey = DataManager::GetEquipReinforceKeyDevil(_cell->_eEquipIdx, reinforceLv);
    auto myKey = ItemsMoneyManager::getInstance()->getDevilKey();
    if ( MafUtils::compareStr(myKey, needKey) )
    {
        SoundManager::SoundEffectStart(SOUND_UPGRADE);
        
        reinforceLv ++;
        needKey.insert(0, "-");
        
        ItemsMoneyManager::getInstance()->setDevilKey(needKey);
        UserInfoManager::getInstance()->setEquipReinforceLevelDevil(_cell->_eEquipType, reinforceLv);
        
        //팝업, 테이블 다시그리기
        _cell->setState(ModelCell::E_CHANGE_STATE::LEVEL);
        
        redrawContents();
    }
    else
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::D_KEY);
        popup->show();
    }
        
    
}

#pragma mark - network

void PopupEquipDetail::requestCheckCostumeCoin()
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

void PopupEquipDetail::ResultCheckCostumeCoin(network::HttpResponse* response,std::string &data)
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
    
    if(data.length()>0)
    {
        rapidjson::Document jsondoc;
        jsondoc.ParseInsitu<0>((char*)(const char*)data.c_str()).HasParseError();
        auto costumeCoin = jsondoc[("_coin")].GetInt();
        
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, costumeCoin);
        if(_itemReinforceEvolution != nullptr)
        {
            _itemReinforceEvolution->setVisible(true);
        }
    }
}
void PopupEquipDetail::requestEvolutionCostumeCoin(){
    
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto reinforceLv = UserInfoManager::getInstance()->getEquipReinforceLevelDevil(_cell->_eEquipType);
    auto spend = DataManager::GetEquipEvolutionSpendDevil(_cell->_eEquipIdx, reinforceLv);
    auto spendKey = MafUtils::doubleToString(spend.devilKey);
    auto spendCoin = MafUtils::toString(spend.costumeCoin);
    
    auto myKey = ItemsMoneyManager::getInstance()->getDevilKey();
    auto myCoin = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
    
    if(reinforceLv < C_MAX_EQUIP_REINFORCE_DEVIL)
        return;
    if(_cell->_eEquipIdx >= C_COUNT_EQUIP_DEVIL)
        return;
    if(!MafUtils::compareStr(myKey, spendKey))
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::D_KEY);
        popup->show();
        return;
    }
        
    if(!MafUtils::compareStr(myCoin, spendCoin))
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::COSTUME_COIN);
        popup->show();
        return;
    }
        
    
    PopupLoading::show();
    
    std::string url = "/costume/consumecoin";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_category", 1);
    req->setData("_coin", spendCoin.c_str());
    req->setData("_key", spendKey.c_str());
    req->setData("_type", (int)_cell->_eEquipType);
    req->setData("_num", (int)_cell->_eEquipIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        ResultEvolutionCostumeCoin(response,dd);
    };
    req->send(cb);
}
void PopupEquipDetail::ResultEvolutionCostumeCoin(network::HttpResponse* response,std::string &data){
    
    PopupLoading::hide();
    
    bool bSuc = true;
    if (!response)
        bSuc = false;
    if(!response->isSucceed())
        bSuc = false;
    if(response->getResponseCode() != 200)
        bSuc = false;
    
    if(!bSuc)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if(data.length()>0){
        rapidjson::Document jsondoc;
        jsondoc.ParseInsitu<0>((char*)(const char*)data.c_str()).HasParseError();
        auto suc = jsondoc[("_suc")].GetBool();
        if(!suc)
            return;
        
        auto costumeCoin = jsondoc[("_coin")].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, costumeCoin);
        
        onSuccessReinforceEvolution();
    }
}
void PopupEquipDetail::onSuccessReinforceEvolution(){
    
    auto reinforceLv = UserInfoManager::getInstance()->getEquipReinforceLevelDevil(_cell->_eEquipType);
    auto spend = DataManager::GetEquipEvolutionSpendDevil(_cell->_eEquipIdx, reinforceLv);
    auto needKey = MafUtils::doubleToString(spend.devilKey);
    auto myKey = ItemsMoneyManager::getInstance()->getDevilKey();
    
    reinforceLv = 0;
    needKey.insert(0, "-");
    ItemsMoneyManager::getInstance()->setDevilKey(needKey);
    UserInfoManager::getInstance()->setEquipReinforceLevelDevil(_cell->_eEquipType, 0);

    auto idx = (int)_cell->_eEquipIdx;
    idx++;
    _cell->_eEquipIdx = (E_EQUIP_IDX)idx;
    _cell->_levelEncrypt = MafAes256::XorEncrypt(0);
    
    UserInfoManager::getInstance()->setEquipDevil(_cell->_eEquipType, _cell->_eEquipIdx);
    UserInfoManager::getInstance()->setEquipLevelDevil(_cell->_eEquipType, 0);
    
    //팝업, 테이블 다시그리기
    _cell->setState(ModelCell::E_CHANGE_STATE::ALL);
    _cell->_itemUpgradeBtn->setVisible(true);
    _cell->startSchedule();
    
    redrawContents();
    
    PopupLoading::show();
    SaveManager::getInstance()->requestSave(CC_CALLBACK_1(PopupEquipDetail::callbackSave, this));
}
#pragma mark -
void PopupEquipDetail::callbackSave(bool bResult)
{
    PopupLoading::hide();
}
