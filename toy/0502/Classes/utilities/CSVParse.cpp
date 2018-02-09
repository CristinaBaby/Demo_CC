#include "CSVParse.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <io.h>
#pragma warning(disable:4996)
#endif

using namespace cocos2d;


CSVParse::~CSVParse()
{
    for (unsigned int i=0; i<data.size(); i++) {
        data[i].clear();
    }
    data.clear();
}

// split: split line into fields
void CSVParse::split(vector<string>& field,string line)
{
    string fld;
    unsigned int i, j;
    
    if (line.length() == 0)
        return ;
    i = 0;
    
    do {
        if (i < line.length() && line[i] == '"')
            j = advquoted(line, fld, ++i);    // skip quote
        else
            j = advplain(line, fld, i);
        
        field.push_back(fld);
        i = j + 1;
    } while (j < line.length());
    
}

// advquoted: quoted field; return index of next separator
int CSVParse::advquoted(const string& s, string& fld, int i)
{
    unsigned int j;
    
    fld = "";
    for (j = i; j < s.length(); j++)
    {
        if (s[j] == '"' && s[++j] != '"')
        {
            unsigned int k = s.find_first_of(fieldsep, j);
            if (k > s.length())    // no separator found
                k = s.length();
            for (k -= j; k-- > 0; )
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}

// advplain: unquoted field; return index of next separator
int CSVParse::advplain(const string& s, string& fld, int i)
{
    unsigned int j;
    
    j = s.find_first_of(fieldsep, i); // look for separator
    if (j > s.length())               // none found
        j = s.length();
    fld = string(s, i, j-i);
    return j;
}


// getfield: return n-th field
const char* CSVParse::getData(unsigned int rows,unsigned int cols)
{
    if (rows>=data.size()|| cols>=data[rows].size()) {
        return "";
    }
    
    return data[rows][cols].c_str();
}

const char* CSVParse::getDatas( unsigned int rows,const char* colsName )
{
	return getData(rows, findRowsData(0 , colsName) );
}


int  CSVParse::findColsData(unsigned int cols,const char* value)
{
    for (unsigned int i=0; i<data.size(); i++) {
        if (strcmp(getData(i, cols), value)==0 )
        {
            return i;
        }
    }
    return -1;
}
int CSVParse::findRowsData( unsigned int rows, const char* value )
{
	if ( rows < data.size() )
	{
		unsigned int size = data[0].size();
		for ( unsigned int i = 0; i <size;i++ )
		{
			if( data[0][i].compare(value) == 0 )
			{
				return i;
			}
		}
	}
	return -1;
}

bool CSVParse::openFile(const char* fileName)
{
    string pathKey = CCFileUtils::getInstance()->fullPathForFilename(fileName);
    ifstream fin(pathKey.c_str());
    string s;
    
    while(std::getline(fin,s, '\r'))
	{
		vector<string> field;
		split(field,s);
		data.push_back(field);
		cols=max(cols, (int)field.size());
    }
    return true;
}

bool CSVParse::openFile2(const char* fileName)
{
	string pathKey = CCFileUtils::getInstance()->fullPathForFilename(fileName);
	const char* xmlConfFullPath = pathKey.c_str();
	ssize_t size = 0;
    
    if (! CCFileUtils::getInstance()->isFileExist(xmlConfFullPath))
        return false;
    
    // NOT WORK ON ANDROID PLATFORM
    //	if ( access(xmlConfFullPath , 0 ) != 0 ) return false;
    
	char *pBuffer = (char*)CCFileUtils::getInstance()->getFileData(xmlConfFullPath , "rb" , &size );
	if ( pBuffer )
	{
		stringstream strm;
		strm.write(pBuffer, size);
		string s;
        
        //modify by luoxp
		while(std::getline(strm,s, '\r'))
		{
			vector<string> field;
			split(field,s);
			data.push_back(field);
			cols=max(cols, (int)field.size());
		}
		delete[] pBuffer;
		return true;
	}
	return false;
}

bool CSVParse::openMemery( const char* mem, unsigned int size )
{
	if ( mem && size > 0 )
	{
		stringstream strm;
		strm.write(mem, size);
		string s;
        
		while(std::getline(strm,s, '\r'))
		{
			vector<string> field;
			split(field,s);
			data.push_back(field);
			cols=max(cols, (int)field.size());
		}
		return true;
	}
	return false;
}

CSVParse* CSVParse::create( const char* csvFile )
{
	CSVParse* parse = new CSVParse;
	if ( parse && parse->openFile2(csvFile) )
	{
		parse->autorelease();
		return parse;
	}
	delete parse;
	return NULL;
}

CSVParse* CSVParse::createMemery( const char* mem, unsigned int size )
{
	CSVParse* parse = new CSVParse;
	if ( parse && parse->openMemery(mem , size) )
	{
		parse->autorelease();
		return parse;
	}
	delete parse;
	return NULL;
}


int CSVParse::getDataValue( unsigned int rows, unsigned int cols )
{
	std::string digitStr = getData(rows,cols);
	if ( !digitStr.empty() )
	{
		size_t spot = 0;
		do
		{
			spot = digitStr.find_first_of(',' , spot);
			if(spot != digitStr.npos ) digitStr.erase(spot , 1);
		} while ( spot != std::string::npos );
        
		return CCString::create(digitStr)->intValue();
	}
	return 0;
}

int CSVParse::getDataValues( unsigned int rows, const char* colsName )
{
	return getDataValue(rows, findRowsData(0,colsName));
}


float CSVParse::getDataSecond( unsigned int rows, unsigned int cols )
{
	std::string str = getData(rows , cols);
    
	if ( !str.empty() )
	{
		size_t posh = str.find_first_of(':');
		std::string hour = str.substr( 0 , posh );
        
		size_t posm = str.find_first_of(':', posh + 1);
		std::string min = str.substr(posh+1, posm - (posh+1));
        
		std::string second = str.substr(posm+1, str.length()- ( posm+1) );
        
		return (float)(CCString::create(hour)->uintValue() * 3600
                       + CCString::create(min)->uintValue() * 60
                       + CCString::create(second)->uintValue());
	}
	return 0;
}

float CSVParse::getDataSeconds( unsigned int rows, const char* colsName )
{
	return getDataSecond(rows, findRowsData(0,colsName));
}


float CSVParse::getDataFloat( unsigned int rows, unsigned int cols )
{
	std::string digitStr = getData(rows,cols);
	if ( !digitStr.empty() )
	{
		size_t spot = 0;
		do
		{
			spot = digitStr.find_first_of(',' , spot);
			if(spot != digitStr.npos ) digitStr.erase(spot , 1);
		} while ( spot != std::string::npos );
        
		return CCString::create(digitStr)->floatValue();
	}
	return 0;
}

float CSVParse::getDataFloats( unsigned int rows, const char* colsName )
{
	return getDataFloat(rows, findRowsData(0,colsName));
}
