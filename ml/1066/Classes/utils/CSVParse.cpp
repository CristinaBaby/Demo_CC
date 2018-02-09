//
//  CSVParse.cpp
//  CocosFramework
//
//  Created by hujie2 on 14-8-5.
//
//

#include "CSVParse.h"


CSVParse::CSVParse(std::string sep):fieldsep(sep),m_iCols(0)
{
    
}

CSVParse::~CSVParse()
{
    // release datas, free memory
    for(auto itr = m_vDatas.begin(); itr != m_vDatas.end(); ++itr)
    {
        (*itr).clear();
    }
    m_vDatas.clear();
    
}

CSVParse* CSVParse::create(const char *csvFileName)
{
    CSVParse* pRet = new CSVParse;
    if (pRet && pRet->loadFile(csvFileName)) {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool CSVParse::loadFile(const char *fileName)
{
    string pathKey = FileUtils::getInstance()->fullPathForFilename(fileName);
	const char* xmlConfFullPath = pathKey.c_str();
    ssize_t size = 0;
    
	//if ( access(xmlConfFullPath , 0 ) != 0 ) return false;
    
	char *pBuffer = (char*)FileUtils::getInstance()->getFileData(xmlConfFullPath , "rb" , &size );
	if ( pBuffer )
	{
		stringstream strm;
		strm.write(pBuffer, size);
        
		string s;
        
		while(getline(strm,s))
		{
            std::string::size_type p = s.find_last_of('\r');
            if(p != std::string::npos) s.erase(p);
			vector<string> field;
			split(field,s);
			m_vDatas.push_back(field);
			m_iCols=max(m_iCols, (int)field.size());
		}
		delete[] pBuffer;
		return true;
	}
	return false;
}

//bool CSVParse::getline(std::istream &is,std::string &str){
//	bool b = (std::getline(is,str) != NULL);
//	std::string::size_type p = str.find_last_of('\r');
//	if(p != std::string::npos) str.erase(p);
//    
////    std::getline

//    
//	return b;
//}

void CSVParse::split(vector<string>& field,string line)
{
    string fld;
    unsigned int i, j;
    
    if (line.length() == 0)
        return ;
    i = 0;
    
    do {
        if (i < line.length() && line[i] == '"')
            j = advquoted(line, fld, i+1);    // skip quote
        else
            j = advplain(line, fld, i);
        
        field.push_back(fld);
        i = j + 1;
    } while (j < line.length());
    
}

// advquoted: quoted field; return index of next separator
int CSVParse::advquoted(const string& s, string& fld, int i)
{
//    unsigned int j;
//    
//    fld = "";
//    for (j = i; j < s.length(); j++)
//    {
//        if (s[j] == '"' && s[++j] != '"')
//        {
//            unsigned int k = s.find_first_of(fieldsep, j);
//            if (k > s.length())    // no separator found
//                k = s.length();
//            for (k -= j; k-- > 0; )
//                fld += s[j++];
//            break;
//        }
//        if(s[j] != '}')
//        {
//           fld += s[j];
//        }
//        
//    }
//    return j;
    
    fld = "";
    
    int k = s.find_first_of('"', i);
    if(k == string::npos)
    {
        return i - 1;
    }else
    {
        fld  = string(s, i, k-i);
    }
    
    return k + 1;
}

// advplain: unquoted field; return index of next separator
int CSVParse::advplain(const string& s, string& fld, int i)
{
    unsigned int j;
    
    j = s.find_first_of(fieldsep, i); // look for separator
    if (j > s.length())               // none found
    {
        j = s.length();
    }
    fld = string(s, i, j-i);
    return j;
}


const char* CSVParse::getDatas( unsigned int rows,const char* colsName )
{
	return getData(rows, findColsData(0,colsName) );
}

int  CSVParse::findColsData(unsigned int rows,const char* value)
{
	if ( rows < m_vDatas.size() )
	{
		unsigned int size = m_vDatas[rows].size();
		for ( unsigned int i = 0; i <size;i++ )
		{
			if( m_vDatas[0][i].compare(value) == 0 )
			{
				return i;
			}
		}
	}
	return -1;
}

// getfield: return n-th field
const char* CSVParse::getData(unsigned int rows,unsigned int cols)
{
    if (rows>=m_vDatas.size()|| cols>=m_vDatas[rows].size()) {
        return "";
    }
    
    return m_vDatas[rows][cols].c_str();
}