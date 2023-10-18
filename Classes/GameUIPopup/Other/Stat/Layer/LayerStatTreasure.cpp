//
//  LayerStatTreasure.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 10/01/2020.
//

#include "LayerStatTreasure.h"

#include "GameObject/InfoArtifact.h"
#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

StatTreasure* StatTreasure::create(E_PLACE ePlace)
{
    StatTreasure* pRet = new(std::nothrow) StatTreasure();
    if ( pRet && pRet->init(ePlace) )
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

StatTreasure::StatTreasure(void):
_layerContainerTreasure(nullptr)
{
    
}

StatTreasure::~StatTreasure(void)
{
    
}
bool StatTreasure::init(E_PLACE ePlace)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    //
    _ePlace = ePlace;
    
    // init
    initUI();
    
    return true;
}
void StatTreasure::initUI()
{
    Size size = Size(750, Director::getInstance()->getVisibleSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);
    
    _layerContainerTreasure = Layer::create();
    _layerContainerTreasure->setContentSize( size );
    _layerContainerTreasure->setIgnoreAnchorPointForPosition(false);
    _layerContainerTreasure->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    addChild(_layerContainerTreasure);
      
    initHighestTreasure();
    initTreasure();
    initBottom();
}
void StatTreasure::initVar()
{
    _lbCurrentValue.clear();
}

void StatTreasure::initHighestTreasure()
{
    Sprite* sprBackgroundExplain = Sprite::create("Assets/ui_common/base_text_1_3.png");
    sprBackgroundExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBackgroundExplain->setPosition(_layerContainerTreasure->getContentSize().width/2, _layerContainerTreasure->getContentSize().height * 0.72);
    _layerContainerTreasure->addChild(sprBackgroundExplain);
    
    Label* lbExplain = Label::createWithTTF(GAME_TEXT("t_ui_menu_stat_treasure"), GAME_FONT, 28);
    lbExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbExplain->setPosition(sprBackgroundExplain->getContentSize().width / 2, sprBackgroundExplain->getContentSize().height / 2);
    lbExplain->setColor(COLOR_TITLE);
    sprBackgroundExplain->addChild(lbExplain);
    
    std::vector<E_ARTIFACT> _vecArtifact;
    
    _vecArtifact.push_back(E_ARTIFACT::ARTI_1_EVOL1);
    _vecArtifact.push_back(E_ARTIFACT::ARTI_3_EVOL1);
    _vecArtifact.push_back(E_ARTIFACT::ARTI_4_EVOL1);
    _vecArtifact.push_back(E_ARTIFACT::ARTI_8_EVOL1);
    _vecArtifact.push_back(E_ARTIFACT::ARTI_12_CROSS_EVOL1);
    
    auto infiniteMgr = InfiniteArtifactManager::getInstance();
    if(infiniteMgr->isActive())
    {
        auto vecInfiniteInfo = infiniteMgr->getInfiniteArtiInfoFromGrade(infiniteMgr->getCurGrade());
        int i = 0;
        for (auto info : vecInfiniteInfo)
        {
            std::string pathBG = "Assets/ui/stat/stat_box_6.png";
            if ( info->getGrade() > 7 )
            {
                pathBG = "Assets/ui/stat/stat_box_7.png";
            }
            Sprite* sprStatbox = Sprite::create(pathBG);
            sprStatbox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprStatbox->setPosition(_layerContainerTreasure->getContentSize().width*0.04 + (sprStatbox->getContentSize().width + 35) * i, _layerContainerTreasure->getContentSize().height * 0.69);
            _layerContainerTreasure->addChild(sprStatbox);
            
            std::string pathEffect = "Assets/ui_common/box_transcendence1_2.png";
            if ( info->getGrade() > 7 )
            {
                pathEffect = "Assets/ui_common/box_transcendence2_2.png";
            }
            auto sprIconEffect = Sprite::create(pathEffect);
            sprIconEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIconEffect->setPosition(sprStatbox->getContentSize()/2);
            sprIconEffect->setScale(0.8f);
            sprStatbox->addChild(sprIconEffect);
            
            
            auto rotate = RotateBy::create(10, 360);
            sprIconEffect->runAction(RepeatForever::create(rotate));
            
            Sprite* sprTresureIcon = Sprite::create(info->getIconPath());
            sprTresureIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprTresureIcon->setPosition(sprStatbox->getContentSize().width/2, sprStatbox->getContentSize().height/2 + 5);
            sprTresureIcon->setScale(0.7f);
            sprStatbox->addChild(sprTresureIcon);
            
            auto labelGrade = MafNode::createBmFont(MafUtils::toString(info->getGrade()), GAME_BM_FONT_AL, 20);
            labelGrade->setPosition(sprStatbox->getContentSize().width/2,sprStatbox->getContentSize().height-10);
            sprStatbox->addChild(labelGrade);
            
            auto labelLevel = MafNode::createBmFont("Lv."+MafUtils::toString(info->getCurLv()), GAME_BM_FONT_AL, 20);
            labelLevel->setPosition(sprStatbox->getContentSize().width/2, 23.5f);
            sprStatbox->addChild(labelLevel);
            i++;
        }
    }
    else
    {
        for ( int i = 0; i < 5; i++ )
        {
            E_ARTIFACT eType = _vecArtifact.at(i);
            
            auto info = UserInfoManager::getInstance()->getHighArtifactInfo(_ePlace, eType);
            if ( info == nullptr )
            {
                continue;
            }
            
            Sprite* sprStatbox = Sprite::create(MafUtils::format("Assets/ui/stat/stat_box_%d.png", i+1));
            sprStatbox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprStatbox->setPosition(_layerContainerTreasure->getContentSize().width*0.04 + (sprStatbox->getContentSize().width + 35) * i, _layerContainerTreasure->getContentSize().height * 0.69);
            _layerContainerTreasure->addChild(sprStatbox);
            
            Sprite* sprTresureIcon = Sprite::create(info->getIconPath());
            sprTresureIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprTresureIcon->setPosition(sprStatbox->getContentSize().width/2, sprStatbox->getContentSize().height*2/3);
            sprStatbox->addChild(sprTresureIcon);
            
            auto labelGrade = MafNode::createBmFont(MafUtils::toString(info->getGeneration()), GAME_BM_FONT_AL, 26);
            labelGrade->setPosition(sprStatbox->getContentSize().width-15,sprStatbox->getContentSize().height-15);
            labelGrade->setColor(COLOR_COIN);
            sprStatbox->addChild(labelGrade);
            
            auto labelLevel = MafNode::createBmFont("Lv." + MafUtils::toString(UserInfoManager::getInstance()->getArtifactLevel(_ePlace, (E_ARTIFACT)info->getIdx())), GAME_BM_FONT_AL, 20);
            labelLevel->setPosition(sprStatbox->getContentSize().width/2,15);
            labelLevel->setColor(COLOR_COIN);
            sprStatbox->addChild(labelLevel);
            
        }
    }
}
void StatTreasure::initTreasure()
{
    std::vector<E_ARTIFACT> _vecArtifact;
    for( int i = 1; i <=C_COUNT_ARTIFACT; i++)
    {
        if ( DataManager::GetArtifactType((E_ARTIFACT)i) != E_ARTIFACT_TYPE::ARTI_TYPE_NONE && DataManager::GetArtifactType((E_ARTIFACT)i) != E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT )
        {
            _vecArtifact.push_back((E_ARTIFACT)i);
        }
    }
    
    std::vector<E_ARTIFACT> _vecArtifactHigh;
    
    _vecArtifactHigh.push_back(E_ARTIFACT::ARTI_1_EVOL1);
    _vecArtifactHigh.push_back(E_ARTIFACT::ARTI_3_EVOL1);
    _vecArtifactHigh.push_back(E_ARTIFACT::ARTI_4_EVOL1);
    _vecArtifactHigh.push_back(E_ARTIFACT::ARTI_8_EVOL1);
    _vecArtifactHigh.push_back(E_ARTIFACT::ARTI_12_CROSS_EVOL1);

        
    for ( int i = 0; i < 15 * 5; i++ )// 15차까지 미리 준비
    {
        Sprite* sprBackground = Sprite::create("Assets/ui/stat/box_3.png");
        sprBackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprBackground->setScale(0.55);
        sprBackground->setPosition(_layerContainerTreasure->getContentSize().width*0.04 + (sprBackground->getBoundingBox().size.width + 2) * (i / 5), _layerContainerTreasure->getContentSize().height * 0.55 - ( (sprBackground->getBoundingBox().size.height + 20) * (i % 5) ));
        _layerContainerTreasure->addChild(sprBackground);
            
        if ( i < _vecArtifact.size() )
        {
            auto infoArtifact = DataManager::GetArtifactInfo(_vecArtifact.at(i));
            
            Sprite* sprTreasureIcon = Sprite::create(infoArtifact->getIconPath());
            sprTreasureIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            sprTreasureIcon->setPosition(sprBackground->getBoundingBox().size.width*3.5/10, sprBackground->getBoundingBox().size.height*1.4);
            sprTreasureIcon->setScale(0.9);
            sprBackground->addChild(sprTreasureIcon);
                
            auto infoArtifactHigh = UserInfoManager::getInstance()->getHighArtifactInfo(_ePlace, _vecArtifactHigh.at(i%5));
            if ( infoArtifact->getIdx() == infoArtifactHigh->getIdx() && !InfiniteArtifactManager::getInstance()->isActive())
            {
                sprBackground->setTexture("Assets/ui/stat/box_3_2.png");
            }
            else if ( infoArtifact->getIdx() > infoArtifactHigh->getIdx() )
            {
                sprBackground->setOpacity(100);
                sprTreasureIcon->setOpacity(100);
            }
        }
        else
        {
            sprBackground->setOpacity(100);
        }
    }
}

void StatTreasure::initBottom()
{
    std::vector<E_ARTIFACT> _vecArtifact;
    std::vector<std::string> _vecIconpath;
    std::vector<Vec2> _vecIconPosition;
    std::vector<std::pair<std::string,int>> _vecIconLevel;
    std::vector<float> _vecIconScale;
    
    for( int i = 1; i <=C_COUNT_ARTIFACT; i++)
    {
        if ( DataManager::GetArtifactType((E_ARTIFACT)i) == E_ARTIFACT_TYPE::ARTI_TYPE_NONE )
        {
            _vecArtifact.push_back((E_ARTIFACT)i);
        }
    }
    
    for (int i = 0; i < 10; i++)
    {
        auto infoArtifact = DataManager::GetArtifactInfo(_vecArtifact.at(i));
        
        _vecIconpath.push_back(infoArtifact->getIconPath());
        _vecIconPosition.push_back(Vec2(0,0));
        
        int lv = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _vecArtifact.at(i));
        _vecIconLevel.push_back( std::pair<std::string,int> ("TREASURE",lv) );
        _vecIconScale.push_back(1.f);
    }
    
    //봉식 마을
    for ( int i =  1; i <=C_COUNT_BUILDING; i++)
    {
        _vecIconpath.push_back(TownManager::getInstance()->getTownPathIcon((E_TOWN)i));
        if ( i == 2 )
        {
            _vecIconPosition.push_back(Vec2(-8,0));
        }
        else if ( i == 3 )
        {
            _vecIconPosition.push_back(Vec2(-3,0));
        }
        else
        {
            _vecIconPosition.push_back(Vec2(0,0));
        }
        
        int lv = TownManager::getInstance()->getTownLevel((E_TOWN)i);
        _vecIconLevel.push_back( std::pair<std::string,int> ("BUILDING",lv) );
        _vecIconScale.push_back(1.f);
        
    }
    
    //강화석
    for ( int i =  1; i <=C_COUNT_ENCHANT_STONE; i++)
    {
        _vecIconpath.push_back(StoneManager::getInstance()->getPath((E_STONE)i));
        _vecIconPosition.push_back(Vec2(0,0));
        
        int lv = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL,(E_STONE)i);
        _vecIconLevel.push_back( std::pair<std::string,int> ("STONE",lv) );
        _vecIconScale.push_back(1.f);
    }
    
    //펫
    for ( int i =  1; i <=C_COUNT_PET; i++)
    {
        _vecIconpath.push_back(PetNewManager::getInstance()->getPetImagePath((E_PET)i));
        _vecIconPosition.push_back(Vec2(-10,7));
        
        int lv = PetNewManager::getInstance()->getPetLevel((E_PET)i);
        _vecIconLevel.push_back( std::pair<std::string,int> ("PET",lv) );
        _vecIconScale.push_back(1.f);
    }
    
    //보물 잔여 1차
    for (int i = 10; i < 12; i++)
    {
        auto infoArtifact = DataManager::GetArtifactInfo(_vecArtifact.at(i));
        
        _vecIconpath.push_back(infoArtifact->getIconPath());
        _vecIconPosition.push_back(Vec2(0,0));
        
        int lv = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _vecArtifact.at(i));
        _vecIconLevel.push_back( std::pair<std::string,int> ("TREASURE",lv) );
        _vecIconScale.push_back(1.f);
        
    }
    
    //고대 유물
    for ( int i =  1; i <=C_COUNT_RELIC; i++)
    {
        _vecIconpath.push_back(StringUtils::format("Assets/icon_artifact/relic_%d.png", i));
        _vecIconPosition.push_back(Vec2(0,0));
        
        int lv = UserInfoManager::getInstance()->getRelicLevel(_ePlace, (E_RELIC)i);
        _vecIconLevel.push_back( std::pair<std::string,int> ("RELIC",lv) );
        _vecIconScale.push_back(1);
    }
    
    for (int i = 12; i < _vecArtifact.size(); i++)
    {
        if(_vecArtifact.at(i) >= E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT)
            continue;
        
        auto info = DataManager::GetArtifactInfo(_vecArtifact.at(i));
        if(info != nullptr)
            _vecIconpath.push_back(info->getIconPath());
        _vecIconPosition.push_back(Vec2(0,0));
        
        int lv = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, _vecArtifact.at(i));
        _vecIconLevel.push_back( std::pair<std::string,int> ("TREASURE",lv) );
        
        if(_vecArtifact.at(i) >= E_ARTIFACT::ARTI_103_BERSERKER_SPRIT)
            _vecIconScale.push_back(0.5f);
        else
            _vecIconScale.push_back(1.f);
        
    }
    int reliccount = 1;
    int petcount = 1;
    int treasurecount = 0;
    int bulidingcount = 1;
    for ( int i = 0; i < _vecIconpath.size(); i++ )
    {
        Sprite* sprBackground = Sprite::create("Assets/ui/stat/box_1.png");
        sprBackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprBackground->setScale(0.8);
                sprBackground->setPosition(_layerContainerTreasure->getContentSize().width*0.045 + ( (sprBackground->getBoundingBox().size.width + 5) * (i % 10) ), _layerContainerTreasure->getContentSize().height * 0.3 - (sprBackground->getBoundingBox().size.height + 2) * (i / 10));
        _layerContainerTreasure->addChild(sprBackground);
        
        auto sprTreasureIcon = ui::Scale9Sprite::create(_vecIconpath.at(i));
        sprTreasureIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprTreasureIcon->setPosition(sprBackground->getBoundingBox().size.width*2/10 + _vecIconPosition.at(i).x , sprBackground->getBoundingBox().size.height*1.1 + _vecIconPosition.at(i).y);
        sprTreasureIcon->setScale(_vecIconScale[i]);
        sprBackground->addChild(sprTreasureIcon);
        
        Label* lbLevel = Label::createWithSystemFont("", SYSTEM_FONT, 17);
        lbLevel->enableBold();
        lbLevel->setPosition(sprBackground->getContentSize().width/2, sprBackground->getContentSize().height*0.125);
        lbLevel->setColor(COLOR_KEY);
        sprBackground->addChild(lbLevel);
        
        auto pair = _vecIconLevel.at(i);
        std::string strLevel;
        
        if(pair.first == "TREASURE")
        {
            auto artInfo = DataManager::GetArtifactInfo(_vecArtifact.at(treasurecount));
            
            if ( artInfo->getLevelMax() == 0 )
            {
                std::string sLv = MafUtils::toString(pair.second);
                MafUtils::convertNumberToShort(sLv);
                strLevel = StringUtils::format("Lv.%s", sLv.c_str());
            }
            else
            {
                if (pair.second >= artInfo->getLevelMax())
                {
                    strLevel = "MAX";
                }
                else
                {
                    std::string sLv = MafUtils::toString(pair.second);
                    MafUtils::convertNumberToShort(sLv);
                    strLevel = StringUtils::format("Lv.%s", sLv.c_str());
                }
            }
            treasurecount++;
        }
        else if(pair.first == "BUILDING")
        {
            if (pair.second >= TownManager::getInstance()->getTownLevelMax((E_TOWN)bulidingcount))
            {
                strLevel = "MAX";
            }
            else
            {
                strLevel = GAME_TEXTFORMAT("t_ui_town_floor", pair.second);
            }
            bulidingcount++;
        }
        else if(pair.first == "STONE")
        {
            strLevel = MafUtils::format("x%d", pair.second);
        }
        else if(pair.first == "BONG")
        {
            if (pair.second >= DataManager::GetBonggiokMaxLevel())
            {
                strLevel = "MAX";
            }
            else
            {
                strLevel = StringUtils::format("Lv.%d", pair.second);
            }
        }
        else if(pair.first == "PET")
        {
            int reinforceLevel = PetNewManager::getInstance()->getPetReinforce((E_PET)petcount);
            if (pair.second >= PetNewManager::getInstance()->getPetLevelMax((E_PET)petcount))
            {
                strLevel = MafUtils::format("MAX(%d)",reinforceLevel);
            }
            else
            {
                std::string sLv = MafUtils::toString(pair.second);
                strLevel = StringUtils::format("Lv.%s(%d)", sLv.c_str(),reinforceLevel);
            }
            petcount++;
        }
        else if(pair.first == "RELIC")
        {
            if ( pair.second >= DataManager::GetRelicMaxLevel((E_RELIC)reliccount))
            {
                strLevel = "MAX";
            }
            else
            {
                std::string sLv = MafUtils::toString(pair.second);
                MafUtils::convertNumberToShort(sLv);
                strLevel = StringUtils::format("Lv.%s", sLv.c_str());
            }
        }

        lbLevel->setString(strLevel);
        if (pair.second == 0)
        {
            sprTreasureIcon->setColor(Color3B(50,50,50));
        }
    }
}
