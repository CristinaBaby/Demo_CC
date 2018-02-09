
#include <iostream>

class GameDataAdapter {
    
    
public:
    static bool getBoolValue(const std::string str);
    
    static int getIntValue(const std::string str);
    
    static std::string getStringValue(const std::string str);
};
