
#ifndef _StringHelp_h
#define _StringHelp_h

class StringHelp {
    
    
public:
    //字符串分割函数
    inline static std::vector<std::string> split(std::string str,std::string pattern)
    {
        std::string::size_type pos;
        std::vector<std::string> result;
        str+=pattern;//扩展字符串以方便操作
        int size=str.size();
        
        for(int i=0; i<size; i++)
        {
            pos=str.find(pattern,i);
            if(pos<size)
            {
                std::string s=str.substr(i,pos-i);
                result.push_back(s);
                i=pos+pattern.size()-1;
            }  
        }  
        return result;  
    }
};

#endif
