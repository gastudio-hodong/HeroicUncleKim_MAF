//
//  CSVParse.cpp
//  DragonVillageAce
//
//  Created by 이명열 on 2016. 4. 26..
//
//

#include "Libs/csv/CSVParse.h"
#include "cocos2d.h"

USING_NS_CC;

CSVParse::CSVParse(std::istream& fin, std::string sep)
: fieldsep(sep)
, cols(0)
{
    
}

CSVParse::~CSVParse(void)
{
}

void CSVParse::StringSplit( const std::string& str, std::vector<std::string>& tokens, const char& delimiters )
{
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos-lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void CSVParse::split( std::vector<std::string>& field, std::string line )
{
    std::string fld;
    unsigned int i,j=0;
    
    if( line.length() ==0 )
        return;
    i=0;
    
    do
    {
        if(j<line.length() && line[i]=='"')
            j = advquoted(line, fld, ++i);
        else
            j = advplain(line, fld, i);
        
        field.push_back(fld);
        i = j+1;
    } while (j<line.length());
}

int CSVParse::advplain( const std::string& s, std::string& fld, int i)
{
    size_t j;
    j = s.find_first_of(fieldsep, i);
    if(j>s.length())
        j=s.length();
    fld = std::string(s,i,j-i);
    return (int)j;
}

int CSVParse::advquoted( const std::string& s, std::string& fld, int i)
{
    unsigned int j;
    fld = "";
    for (j=i; j<s.length(); ++j)
    {
        if(s[j]=='"' && s[++j]!='"')
        {
            size_t k = s.find_first_of(fieldsep, j);
            if(k>s.length())
                k = s.length();
            for(k-=j; k-->0;)
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}

//解析 CVS 文件
bool CSVParse::openFile(std::string fileName)
{
    std::string strJsonPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string strContent = FileUtils::getInstance()->getStringFromFile(strJsonPath);
    
    std::vector<std::string> line;
    StringSplit(strContent, line, '\n');
    for(unsigned int i=0; i<line.size(); ++i)
    {
        std::vector<std::string> field;
        split(field, line[i]);
        data.push_back(field);
        cols = std::max(cols, (int)field.size());
    }
    
    return true;
}

//获取指定行列的数据
std::string CSVParse::getData(unsigned int rows, unsigned int cols )
{
    if (rows<0 || rows>=data.size() || cols<0 || cols>=data[rows].size())
    {
        return "";
    }
    
    return data[rows][cols];
}

//获取指定数据的列下标
int CSVParse::findColsData( int cols, const char* value )
{
    for (unsigned int i=0; i<data.size(); ++i)
    {
        if(strcmp(getData(i,cols).c_str(),value)==0)
            return i;
    }
    return -1;
}
