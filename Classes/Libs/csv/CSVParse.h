//
//  CSVParse.h
//  DragonVillageAce
//
//  Created by 이명열 on 2016. 4. 26..
//
//

#ifndef CSVParse_h
#define CSVParse_h

#include <vector>
#include <iostream>

class CSVParse
{
public:
    CSVParse(std::istream& fin=std::cin, std::string sep=",");
    virtual ~CSVParse(void);
    


    std::vector<std::vector<std::string>> data;
    
private:
    std::string fieldsep;
    int cols;
    
    void StringSplit(const std::string& str, std::vector<std::string>& tokens, const char& delimiters);
    void split(std::vector<std::string>& field, std::string line);
    int advplain(const std::string& line, std::string& fld, int);
    int advquoted(const std::string& line, std::string& fld, int);
    
public:
    bool openFile(std::string fileName);
    std::string getData(unsigned int rows, unsigned int cols);
    int findColsData(int cols, const char* value);
    
    inline int getCols() { return cols; }
    inline int getRows() { return (int)data.size(); };
};


#endif /* CSVParse_h */
