//
//  TextManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "TextManager.h"

#include "Common/ConfigDefault.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerSystem/TableManager.h"

USING_NS_CC;

TextManager::TextManager(void) :
_strLanguage(""),

_strTalkTipText(""),
_strBadWordText("")
{
    _listText.clear();
    _listDialogText.clear();
    _listTalk.clear();
}

TextManager::~TextManager(void)
{
    _listText.clear();
    _listDialogText.clear();
    _listTalk.clear();
}

bool TextManager::init()
{
    
    return true;
}

#pragma mark - set, get, add
std::string TextManager::getLang()
{
    if ( _strLanguage.length() != 0 )
    {
        return _strLanguage;
    }
    
    std::string strLanguageCountry = "";
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    switch (currentLanguageType)
    {
        case LanguageType::KOREAN:
            strLanguageCountry = "ko";
            break;
        case LanguageType::ENGLISH:
            strLanguageCountry = "en";
            break;
        case LanguageType::JAPANESE:
            strLanguageCountry = "jp";
            break;
        case LanguageType::FRENCH:
            strLanguageCountry = "fr";
            break;
        case LanguageType::TURKISH:
            strLanguageCountry = "tr";
            break;
        case LanguageType::CHINESE_TRADITIONAL:
            strLanguageCountry = "zh_tw";
            break;
        case LanguageType::PORTUGUESE:
            strLanguageCountry = "pt";
            break;
        case LanguageType::RUSSIAN:
            strLanguageCountry = "ru";
            break;
        case LanguageType::SPANISH:
            strLanguageCountry = "es";
            break;
        case LanguageType::GERMAN:
            strLanguageCountry = "de";
            break;
        default:
            strLanguageCountry = "en";
            break;
    }
    
    _strLanguage = UserDefault::getInstance()->getStringForKey("lang", "");
    if ( _strLanguage.length() == 0 )
    {
        setLang(strLanguageCountry);
    }
    
    return _strLanguage;
}

std::string TextManager::getLangFileName(std::string strPath)
{
    std::string strResult = StringUtils::format(strPath.c_str(), getLang().c_str());
    if ( FileUtils::getInstance()->isFileExist(strResult) == false )
    {
        strResult = StringUtils::format(strPath.c_str(), "en");
    }
    
    if ( FileUtils::getInstance()->isFileExist(strResult) == false )
    {
        strResult = StringUtils::format(strPath.c_str(), "ko");
    }
    
    if ( FileUtils::getInstance()->isFileExist(strResult) == false )
    {
        strResult = StringUtils::format(strPath.c_str(), "jp");
    }
    
    return strResult;
}

void TextManager::setLang(std::string strLang)
{
    _strLanguage = strLang;
    UserDefault::getInstance()->setStringForKey("lang", strLang);
}

#pragma mark - text
void TextManager::textInit()
{
    // clear
    _listText.clear();
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::LANGUAGE_TEXT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        std::string strKey1 = MafUtils::format("text_%s", "en");
        std::string strKey2 = MafUtils::format("text_%s", getLang().c_str());
        
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];

            std::string strKey = jsonValue["key"].GetString();
            std::string strValue1 = "";
            std::string strValue2 = "";
            if ( jsonValue.HasMember(strKey1.c_str()) )
            {
                strValue1 = jsonValue[strKey1.c_str()].GetString();
            }
            
            if ( jsonValue.HasMember(strKey2.c_str()) )
            {
                strValue2 = jsonValue[strKey2.c_str()].GetString();
            }
            
            _listText[strKey] = strValue1;
            if ( strValue2.length() > 0 )
            {
                _listText[strKey] = strValue2;
            }
        }
    }
}

std::string TextManager::text(std::string strKey)
{
    if ( strKey.length() == 0 )
        return "";
    
    std::string strResult = "";
    
    if ( _listText.find(strKey) != _listText.end() )
    {
        strResult = std::string(_listText.find(strKey)->second);
        strResult = UtilsString::getInstance()->replaceAll(strResult.c_str(), "\\n", "\n");
    }
    
    return strResult;
}

std::string TextManager::textFormat(std::string strKey, ...)
{
    std::string ret = "";
    std::string format = text(strKey);
    if ( format.length() == 0 )
        return "";
    
    int32_t length = 1024 * 100;
    
    va_list ap;
    va_start(ap, strKey);
    
    char* buf = (char*)malloc(length);
    if ( buf != nullptr )
    {
        vsnprintf(buf, length, format.c_str(), ap);
        ret = std::string(buf);
        free(buf);
    }
    va_end(ap);
    
    return ret;
}

#pragma mark - Dialog
void TextManager::dialogTextInit()
{
    // clear
    _listDialogText.clear();
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::LANGUAGE_DIALOG_TEXT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        std::string strKey1 = MafUtils::format("text_%s", "en");
        std::string strKey2 = MafUtils::format("text_%s", getLang().c_str());
        
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];

            std::string strKey = jsonValue["key"].GetString();
            std::string strValue1 = "";
            std::string strValue2 = "";
            if ( jsonValue.HasMember(strKey1.c_str()) )
            {
                strValue1 = jsonValue[strKey1.c_str()].GetString();
            }
            
            if ( jsonValue.HasMember(strKey2.c_str()) )
            {
                strValue2 = jsonValue[strKey2.c_str()].GetString();
            }
            
            _listDialogText[strKey] = strValue1;
            if ( strValue2.length() > 0 )
            {
                _listDialogText[strKey] = strValue2;
            }
        }
    }
}

std::string TextManager::dialogText(std::string strKey)
{
    if ( strKey.length() == 0 )
        return "";
    
    std::string strResult = "";
    
    if ( _listDialogText.find(strKey) != _listDialogText.end() )
    {
        strResult = std::string(_listDialogText.find(strKey)->second);
        strResult = UtilsString::getInstance()->replaceAll(strResult.c_str(), "\\n", "\n");
    }
    
    return strResult;
}

std::string TextManager::dialogTextFormat(std::string strKey, ...)
{
    std::string ret = "";
    std::string format = dialogText(strKey);
    if ( format.length() == 0 )
        return "";
    
    int32_t length = 1024 * 100;
    
    va_list ap;
    va_start(ap, strKey);
    
    char* buf = (char*)malloc(length);
    if ( buf != nullptr )
    {
        vsnprintf(buf, length, format.c_str(), ap);
        ret = std::string(buf);
        free(buf);
    }
    va_end(ap);
    
    return ret;
}

#pragma mark - talk
void TextManager::talkInit()
{
    // clear
    _listTalk.clear();
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::LANGUAGE_TALK);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        std::string strLang = getLang();
        
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            if ( strLang.compare(jsonValue["lang"].GetString()) == 0 )
            {
                std::string strVal = jsonValue["text"].GetString();
                _listTalk.push_back(strVal);
            }
        }
    }
}


std::string TextManager::talk()
{
    std::string strResult = "";
    
    if ( _listTalk.size() != 0 )
    {
        int nRand = random(0, (int)_listTalk.size()-1);
        strResult = _listTalk.at(nRand);
        strResult = UtilsString::getInstance()->replaceAll(strResult.c_str(), "\\n", "\n");
    }
    
    return strResult;
}

std::string TextManager::talkTip()
{
    std::string strResult = "";
    if ( _strTalkTipText.length() == 0 )
    {
        _strTalkTipText = TableManager::getInstance()->getTableData(E_TABLE::LANGUAGE_TALK_TIP);
    }
    
    //
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)_strTalkTipText.c_str());
    if( jsonParser.HasParseError()  )
    {
        return strResult;
    }
    
    //
    std::vector<std::string> listTalkTip;
    
    E_PLACE ePlace = UserInfoManager::getInstance()->getCurrentPlace();
    int nFloor = UserInfoManager::getInstance()->getFloor(ePlace);
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        std::string strArea = jsonValue["type_area"].GetString();
        std::string strCondition = jsonValue["type_condition"].GetString();
        std::string strParams = jsonValue["params"].GetString();
        std::string strTextKey = jsonValue["text_key"].GetString();
        
        if ( ePlace == E_PLACE::DG_NORMAL && strArea.compare("NORMAL") == 0 )
        {
            if ( strCondition.compare("NONE") == 0 )
            {
                listTalkTip.push_back(strTextKey);
            }
            else
            {
                rapidjson::Document jsonParserTemp;
                jsonParserTemp.ParseInsitu<0>((char*)strParams.c_str());
                
                if ( strCondition.compare("FLOOR") == 0 && jsonParserTemp.HasParseError() == false )
                {
                    int nMin = jsonParserTemp["min"].GetInt();
                    int nMax = jsonParserTemp["max"].GetInt();
                    if ( nFloor >= nMin && nFloor <= nMax )
                    {
                        listTalkTip.push_back(strTextKey);
                    }
                }
            }
        }
        else if ( ePlace == E_PLACE::DG_DEVIL_DOM && strArea.compare("DEVIL") == 0 )
        {
            if ( strCondition.compare("NONE") == 0 )
            {
                listTalkTip.push_back(strTextKey);
            }
            else
            {
                rapidjson::Document jsonParserTemp;
                jsonParserTemp.ParseInsitu<0>((char*)strParams.c_str());
                
                if ( strCondition.compare("FLOOR") == 0 && jsonParserTemp.HasParseError() == false )
                {
                    int nMin = jsonParserTemp["min"].GetInt();
                    int nMax = jsonParserTemp["max"].GetInt();
                    if ( nFloor >= nMin && nFloor <= nMax )
                    {
                        listTalkTip.push_back(strTextKey);
                    }
                }
            }
        }
    }
    
    if ( listTalkTip.size() > 0 )
    {
        ssize_t randIdx = RandomHelper::random_int<int>(0, static_cast<int>(listTalkTip.size()) - 1);
        strResult = text(listTalkTip.at(randIdx));
    }
    
    return strResult;
}

#pragma mark - bad word
bool TextManager::isBadWord(std::string strText)
{
    bool bResult = false;
    if ( _strBadWordText.length() == 0 )
    {
        _strBadWordText = TableManager::getInstance()->getTableData(E_TABLE::LANGUAGE_BADWORD);
    }
    
    //
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)_strBadWordText.c_str());
    if( jsonParser.HasParseError()  )
    {
        return bResult;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        std::string strTemp = jsonValue["text"].GetString();
        if ( strText.find(strTemp) != std::string::npos )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}
