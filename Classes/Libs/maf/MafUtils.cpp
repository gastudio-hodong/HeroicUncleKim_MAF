//
//  Maf.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2015. 4. 15..
//
//

#include "MafUtils.h"

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

USING_NS_CC;

static int S_res_len = 1024;
//static int *S_res = new int[S_res_len];
static int *S_resAdd = new int[S_res_len];
static int *S_resSub = new int[S_res_len];
static int *S_resMul = new int[S_res_len];

//std::string units = "KMBTABCDEFGHIGKLMNOPQRSTUVWXYZ";
std::string units = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string fir_units = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string zeroLength = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

//static bool bTest = true;
static int iValue = 7;

std::mutex _mutexObserver;

namespace MafUtils {
    
    int toInt(std::string &str)
    {
        std::stringstream ss(str);
        int num;
        ss >> num;
        return num;
    }
    

    bool isDigit(const char value)
    {
        return std::isdigit(value);
    }
    
    bool isNumeric(const std::string& value)
    {
        return std::all_of(value.begin(), value.end(), isDigit);
    }

    std::string addCommaToString(std::string &input,int interval,char separator)
    {
        std::string::iterator i;
        
        size_t idx = input.find(".");
        if (idx != std::string::npos) {
            idx = input.size() - idx;
            i = input.end() - (interval+idx);
        }else{
            i = input.end() - interval;
        }
        
        while (i > input.begin())
            i = input.insert(i, separator) - interval;
        
        return input;
    }
    
    std::string convertNumberToShort(std::string &input,bool bGap){
        //input.clear();
        //input.append("500000000000000000000000000000000000000000000000000000000000000000000000000000");//Q
        int width = (int)input.size();
        
        if(width >= 54760)
        {
            input = "MAX";
            return input;
        }
        if(width >3){
            //int dif = 3;
            int v = width/3;
            int r = width%3;
            if(r!=0)
                v++;
            int val = (v-1)*3-1;
            input.erase(width-val,val);
            input.insert(input.size()-1, ".");
            
            std::stringstream str;
            if(bGap)
                str << " ";
            //            str << units.at(v-2);
            
            int temp = v-2;
            std::vector<int> vecIdx;
            
            int count = 0;
            while(temp >= 0)
            {
                count++;
                int value1 = temp / 26;
                int value2 = temp % 26;
                
                if(count > 1)
                {
                    if(temp != 0 && value2 == 0)
                    {
                        if(value1 > 26)
                        {
                            vecIdx.push_back(value2);
                        }
                        else
                        {
                            vecIdx.push_back(temp / value1);
                        }
                    }
                    else
                        vecIdx.push_back(value2);
                }
                else
                {
                    vecIdx.push_back(value2);
                }
                
                if(value1 <= 26)
                {
                    if(temp >= 26)
                    {
                        if(count > 1)
                        {
                            if(value2 == 0)
                                vecIdx.push_back(value1-1);
                            else
                                vecIdx.push_back(value1);
                        }
                        else
                            vecIdx.push_back(value1);
                    }
                    break;
                }
                
                temp /= 26;
                
                if(temp == 26)
                {
                    vecIdx.push_back(temp);
                    break;
                }
                else if(temp == 0)
                    break;
            }
            
            
            std::reverse(vecIdx.begin(), vecIdx.end());
            
            for(int i = 0; i < vecIdx.size(); ++i)
            {
                if(i == vecIdx.size()-1 && vecIdx[i] != 26)
                    str << units.at(vecIdx[i]);
                else
                    str << fir_units.at(vecIdx[i]);
                
//                if(i == 0)
//                    str << fir_units.at(vecIdx[i]);
//                else
//                    str << units.at(vecIdx[i]);
                
            }
            
//            원본
//            int value1 = (v-2)%26;
//            int value2 = (v-2)/26
//            if (value2 == 0){
//                str << units.at(value1);
//            }else{
//                str << units.at(value2-1) << units.at(value1);
//            }
            
            
            input.append(str.str());
        }
        
        return input;
    }
    std::string convertNumberToTime(double time, bool bHour){
        if (bHour) {
            return format("%02lld:%02lld:%02lld", (long long)time/3600, (long long)time%3600/60, (long long)time%3600%60);
        }else{
            return format("%02lld:%02lld", (long long)time/60, (long long)time%60);
        }
    }
    
    int CharToInt(const char c)
    {
        switch (c)
        {
            case '0':
                return 0;
            case '1':
                return 1;
            case '2':
                return 2;
            case '3':
                return 3;
            case '4':
                return 4;
            case '5':
                return 5;
            case '6':
                return 6;
            case '7':
                return 7;
            case '8':
                return 8;
            case '9':
                return 9;
            default:
                return 0;
        }
    }
    
    std::string bigAddNum(std::string &aaa2,std::string bbb,bool bIgnore)
    {
        //
        _mutexObserver.lock();
        
        //
        std::string sAddLength = "";
        std::string aaa = aaa2;
        
        if ( aaa.compare("0") == 0 )
        {
            //
            _mutexObserver.unlock();
            
            return bbb;
        }
        
        if ( aaa.length() > bbb.length() + iValue )
        {
            //
            _mutexObserver.unlock();
            
            return aaa2;
        }
        
        if ( aaa.length() > iValue )
        {
            int length = (int)aaa.length()-iValue;
            sAddLength.append(aaa.substr(iValue, length));
            aaa = aaa.substr(0,iValue);
        }
        
        if ( bbb.length() > sAddLength.length() )
        {
            if ( sAddLength.length() > 0 )
            {
                bbb = bbb.substr(0,bbb.length()-sAddLength.length());
            }
        }
        else
        {
            //
            _mutexObserver.unlock();
            
            return aaa2;
        }
        
        //
        std::string result;
        int diff = abs((int)aaa.length()-(int)bbb.length());
        if ( diff > 0 )
        {
            if(aaa.length() > bbb.length()){
                bbb.insert(0,zeroLength.substr(0,diff));
            }
            else if(aaa.length() < bbb.length()){
                aaa.insert(0,zeroLength.substr(0,diff));
            }
        }
        
        int len = (int)aaa.length()+1;
        memset(S_resAdd, 0, sizeof(int)*len);

        int *res = S_resAdd;
        
        for ( int i = (int)bbb.length()-1; i >= 0; i-- )
        {
            int sum = CharToInt(aaa[i]) + CharToInt(bbb[i]);
            if ( sum >= 10 )
            {
                res[i] += 1;
                sum -= 10;
            }
            res[i+1] += sum;

            int val = res[i+1]*0.10;
            if ( val > 0 )
            {
                res[i] += val;
                res[i+1] -= val*10;
            }
        }
        
        std::ostringstream streamResult;
        for ( int i = 0; i < len; i++ )
        {
            streamResult << res[i];
        }
        
        if ( sAddLength.length() > 0 )
        {
            streamResult << sAddLength;
        }
        result = streamResult.str();
        
        int zero = 0;
        for ( int j = 0; j < result.length(); j++, zero++ )
        {
            if (result[j] != '0')
                break;
        }
           
        if ( zero != 0 )
        {
            result = result.substr(zero, result.length() - zero);
        }
            
        
        //
        _mutexObserver.unlock();
        
        return result;
    }
    
    std::string bigSubNum(std::string &aaa2,std::string bbb,bool bIgnore)
    {
        //
        _mutexObserver.lock();

        //
        std::string sAddLength = "";
        std::string aaa = aaa2;
        
        if ( aaa.compare(bbb) == 0 )
        {
            //
            _mutexObserver.unlock();
            
            return "0";
        }
        
        if ( aaa.length()  > bbb.length() + iValue )
        {
            //
            _mutexObserver.unlock();
            
            return aaa2;
        }
        
        if ( aaa.length() > iValue )
        {
            int length = (int)aaa.length()-iValue;
            sAddLength.append(aaa.substr(iValue, length));
            aaa = aaa.substr(0,iValue);
        }
        
        if ( bbb.length() > sAddLength.length() )
        {
            if ( sAddLength.length() > 0 )
            {
                bbb = bbb.substr(0,bbb.length()-sAddLength.length());
            }
        }
        else
        {
            //
            _mutexObserver.unlock();
            
            return aaa2;
        }
        
        if (bbb[0] == '-')
            bbb = bbb.substr(1, bbb.length()-1);
        
        bool bPass = true;
        if ( aaa.length() < bbb.length() )
        {
            bPass = false;
        }
        else if ( aaa.length() == bbb.length() )
        {
            for( int i = 0; i < bbb.length(); i++ )
            {
                if ( aaa[i] > bbb[i] )
                {
                    bPass = true;
                    break;
                }
                else if ( aaa[i] < bbb[i] )
                {
                    bPass = false;
                    break;
                }
            }
        }
        
        if(!bPass)
        {
            //
            _mutexObserver.unlock();
            
            return "0"; //정확한 값이 아님.단순 크다 작다 판단여부.
        }
        
        std::string result;
        int diff = abs((int)aaa.length()-(int)bbb.length());
        if ( diff > 0 )
        {
            if ( aaa.length() > bbb.length() )
            {
                bbb.insert(0,zeroLength.substr(0,diff));
            }
            else if ( aaa.length() < bbb.length() )
            {
                aaa.insert(0,zeroLength.substr(0,diff));
            }
        }
        
        int len = (int)aaa.length()+1;
        memset(S_resSub, 0, sizeof(int)*len);

        int *res = S_resSub;
        for ( int i = (int)bbb.length()-1; i >= 0; i-- )
        {
            int sub = CharToInt(aaa[i]) - CharToInt(bbb[i]);
            if ( sub < 0 )
            {
                res[i] -= 1;
                sub += 10;
            }
            
            res[i+1] += sub;
            if ( res[i+1] < 0 )
            {
                res[i] -= 1;
                res[i+1] += 10;
            }
        }
        
        std::ostringstream streamResult;
        for ( int i = 0; i < len; i++ )
        {
            if ( res[i] >= 0 )
                streamResult << res[i];
            else
                streamResult << (res[i]+10);
        }
        
        if ( sAddLength.length() > 0 )
        {
            streamResult << sAddLength;
        }
        result = streamResult.str();
        
        int zero = 0;
        for ( int j = 0; j < result.length(); j++, zero++ )
        {
            if (result[j] != '0')
                break;
        }
            
        if ( zero != 0 )
            result = result.substr(zero, result.length() - zero);
        
        if ( result.empty() )
            result = "0";
        
        //
        _mutexObserver.unlock();
        
        return result;
    }
    
    std::string bigMulNum(std::string &aaa2,std::string bbb2,bool bIgnore)
    {
        //
        _mutexObserver.lock();
        
        //
        std::string sAddLength = "";
        std::string aaa = aaa2;
        std::string bbb = bbb2;
        
        if ( aaa.length() > iValue )
        {
            int length = (int)aaa.length()-iValue;
            sAddLength.append(aaa.substr(iValue, length));
            aaa = aaa.substr(0,iValue);
        }
        
        if ( bbb.length() > iValue )
        {
            int length = (int)bbb.length()-iValue;
            sAddLength.append(bbb.substr(iValue, length));
            bbb = bbb.substr(0,iValue);
        }
        
        bool bPass = true;
        if ( aaa=="0" || bbb=="0" )
            bPass = false;
        
        if ( !bPass )
        {
            //
            _mutexObserver.unlock();
            
            return "0"; //정확한 값이 아님.단순 크다 작다 판단여부.
        }
        
        std::string result;
        int len = (int)aaa.length() + (int)bbb.length();//len길이가 문제
        memset(S_resMul, 0, sizeof(int)*len);

        int *res = S_resMul;
        for(int i=(int)bbb.length()-1; i>=0; i--)
        {
            int tempB = CharToInt(bbb[i]);
            if ( tempB == 0 )
            {
                continue;
            }
            
            for ( int j = (int)aaa.length()-1; j >= 0; j-- )
            {
                int tempA = CharToInt(aaa[j]);
                if ( tempA == 0 )
                {
                    continue;
                }
                
                int mul = tempB * tempA;
                if ( mul >= 10 )
                {
                    int val = mul*0.1;
                    res[j+i] += val;
                    mul -= val*10;
                    if ( res[j+i] >= 10 )
                    {
                        int vv = res[j+i]*0.1;
                        res[j+i-1] +=vv;
                        res[j+i] -= vv*10;
                    }
                }
                res[j+i+1] += mul;

                int val = res[j+i+1]*0.1;
                if ( val > 0 )
                {
                    res[j+i] += val;
                    res[j+i+1] -= val*10;
                }
            }
        }
        
        for ( int i = len-1; i >= 0; i-- )
        {
            if ( res[i] >= 10 )
            {
                int vv = res[i]*0.1;
                res[i] -= vv*10;
                res[i-1] += vv;
            }
        }
        
        
        std::ostringstream streamResult;
        for ( int i = 0; i < len; i++ )
        {
            streamResult << res[i];
        }
        
        if ( sAddLength.length() > 0 )
        {
            streamResult << sAddLength;
        }
        result = streamResult.str();
        
        int zero = 0;
        for ( int j = 0; j < result.length(); j++, zero++ )
        {
            if (result[j] != '0')
                break;
        }

        if ( zero != 0 )
            result = result.substr(zero, result.length() - zero);
        
        //
        _mutexObserver.unlock();
        
        return result;
        
    }
    
    //    std::string bigDivNum(std::string strA,std::string strB){
    //
    //        return "";
    //    }
    
    bool compareStr(std::string &aaa,std::string bbb){
        // aaa >= bbb ? true
        bool bPass = true;

        if(aaa==bbb)
            return bPass;
        
        if(aaa.length() > bbb.length()){
            bPass = true;
        }
        else if(aaa.length()<bbb.length()){
            bPass = false;
        }
        else if(aaa.length()==bbb.length()){
            for(int i=0; i< bbb.length();i++){
                if(aaa[i] > bbb[i]){
                    bPass = true;
                    break;
                }
                else if(aaa[i] < bbb[i]){
                    bPass = false;
                    break;
                }
            }
        }
        
        return bPass;
        
    }
    
    
    //    std::string trim_right(const std::string& source, const std::string& whitespaces=" \t\r\n")
    //    {
    //        std::string str(source);
    //        return str.erase(str.find_last_not_of(whitespaces) + 1);
    //    }
    
    std::string TrimRight(const std::string &s, const std::string& whitespaces){
        
        std::string str(s);
        return str.erase(str.find_last_not_of(whitespaces) + 1);
    }
    
    
    
    std::string format(const char* format, ...)
    {
#define CC_MAX_STRING_LENGTH (1024*100)
        std::string ret;
        
        va_list ap;
        va_start(ap, format);
        
        char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
        if (buf != nullptr)
        {
            vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
            ret = buf;
            free(buf);
        }
        va_end(ap);
        
        return ret;
    }
    
    void startCheckTime(){
        time = std::chrono::system_clock::now();
    }
    void endCheckTime(){
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> sec = end - time;
        auto diff = sec.count();
        log("%f", diff / (double)1000.0);
    }
    
    int GetTimeData(E_TIME etime){
        struct tm *tm;
        time_t timep;
        
        timeval tv;
        gettimeofday(&tv, nullptr);
        timep = tv.tv_sec;
        
        tm = localtime(&timep);
        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int yday = tm->tm_yday;
        int wday = tm->tm_wday;
        int day = tm->tm_mday;
        int hour = tm->tm_hour;
        int min = tm->tm_min;
        int second = tm->tm_sec;
        int week = day - 1 - wday;
        if (week <= 0) {
            week = 0;
        }else{
            week = week/7+1;
        }
        
        
        if      (E_TIME::YEAR == etime)     return year;
        else if (E_TIME::MONTH == etime)    return month;
        else if (E_TIME::WEEK == etime)     return week;
        else if (E_TIME::YDAY == etime)     return yday;
        else if (E_TIME::WDAY == etime)     return wday;
        else if (E_TIME::DAY == etime)      return day;
        else if (E_TIME::HOUR == etime)     return hour;
        else if (E_TIME::MIN == etime)      return min;
        else if (E_TIME::SECOND == etime)   return second;
        
        return 0;
    }
    int Get1970Day(){
        time_t seconds;
        
        seconds = std::time (NULL);
        seconds += 60*60*9;
        long daysSinceEpoch = seconds/(60*60*24);
        return (int)daysSinceEpoch;
    }
    int Get1970Week(int dayofweek){
        time_t seconds;
        dayofweek += 3;
        
        seconds = std::time (NULL);
        seconds += 60*60*9;
        long daysSinceEpoch = (seconds - (60*60*24*dayofweek))/(60*60*24*7);
        return (int)daysSinceEpoch;
    }
    
    bool WriteTextFile(std::string path, std::string buf)
    {
        CheckFile((char*)path.c_str());
        
        FILE *f = fopen(path.c_str(), "wt");
        if(f != NULL){
            fwrite(buf.c_str(), 1, buf.length(), f);
            fclose(f);
            return true;
        }else{
            return false;
        }
    }
    bool WriteDataFile(std::string path, const std::vector<char>* buf)
    {
        CheckFile((char*)path.c_str());
        
        FILE *f = fopen(path.c_str(), "wb");
        if(f != NULL){
            fwrite(buf->data(), 1, buf->size(), f);
            fclose(f);
            return true;
        }else{
            return false;
        }
    }
    void CheckFile(char *full_path)
    {
        char temp[256], *sp;
        
        strcpy(temp, full_path); // 경로문자열을 복사
        sp = temp; // 포인터를 문자열 처음으로
        
        while((sp = strchr(sp, '/'))) { // 디렉토리 구분자를 찾았으면
            if(sp > temp && *(sp - 1) != ':') { // 루트디렉토리가 아니면
                *sp = '\0'; // 잠시 문자열 끝으로 설정
                mkdir(temp, S_IFDIR | S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
                // 디렉토리를 만들고 (존재하지 않을 때)
                *sp = '/'; // 문자열을 원래대로 복귀
            }
            sp++; // 포인터를 다음 문자로 이동
        }
    }
    
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        
        return elems;
        
    }
    
    std::string splitCreate(const std::vector<std::string> &s, char delim){
        std::stringstream str;
        str.str("");
        for (int i=0; i<s.size(); i++) {
            if (i<(s.size()-1)) {
                str << s.at(i) << delim;
            }else{
                str << s.at(i);
            }
        }
        return str.str();
    }
    
//    bool strCheck(const char *str)
//    {
//        //48~57 숫자
//        //65~90 영문대문자
//        //97~122 영문소문자
//        //-22~-19 한글 완성 된 글자(iOS)
//        //234~237 한글 완성 된 글자(Android)
//        //nickName이 true 일 땐 한글(완성된 한글),숫자,영문 대소문자 전부 입력가능
//        //nickName이 false 일 땐 숫자,영문 소문자만 입력가능
//
//        int count=0;
//        for (int i=0;i<strlen(str);i++){
//            if ((str[i] >= 48 && str[i] <= 57) || (str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122))
//                count++;
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            if(str[i] >= 234 && str[i] <= 237){
//                count++;
//                i+=2;
//            }
//#else
//            if(str[i] >= -22 && str[i] <= -19){
//                count++;
//                i+=2;
//            }
//#endif
//
//            if (str[i] == 0)
//                break;
//
//        }
//        if(count == strlen_ko(str)){
//            return true;
//        }
//        return false;
//    }
    
//    int strlen_ko(const char *str){
//        size_t size = 0;
//        size_t i = 0;
//        for (;;i++){
//            if (str[i] & 0x80){
//                size++;
//                i+=2;
//            }
//            else if (str[i] == 0)   break;
//            else                    size++;
//        }
//        return (int)size;
//    }
    
    std::string eraseCharcter(std::string& str, char c){
        str.erase(remove(str.begin(), str.end(), c), str.end());
        return str;
    }
    
    std::string doubleToString(const double value){
        //static char charTitle[512] = {0,};
//        memset(charTitle, 0, 512);
        
        char strTemp[1024] = {0,};
        
        sprintf(strTemp, "%0.lf", value);
        return strTemp;
    }
    std::string ChangeLPAD(int num, int digits)
    {
        std::string str = "";
        
        
        while (digits > 0)
        {
            digits--;
            
            str += MafUtils::toString(num % 10);
            
            num /= 10;
        }
        
        std::reverse(str.begin(), str.end());
        
        return str;
    }
}


void ChangeLabel(Label* label, std::string findText, Color3B color, bool all)
{
    std::u16string utf16LabelText;
    std::u16string utf16FindText;
    
    StringUtils::UTF8ToUTF16(label->getString(), utf16LabelText);
    StringUtils::UTF8ToUTF16(findText, utf16FindText);
    
    size_t start = 0, last = 0;
    while (1)
    {
        start = utf16LabelText.find(utf16FindText.c_str(), start);
        last = utf16FindText.length() + start;
        
        if (start == std::string::npos) {
            break;
        }
        
        for (size_t i=start; i<last; i++) {
            Sprite* letter = (Sprite*)label->getLetter((int)i);
            if (letter != nullptr) {
                letter->setColor(color);
            }
        }
        
        if (!all) {
            break;
        }else{
            start += utf16FindText.length();
        }
    }
}



