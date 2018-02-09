//
//  CSVParse.h
//  CocosFramework
//
//  Created by hujie2 on 14-7-31.
//
//

#ifndef _CocosFramework__CSVParse_
#define _CocosFramework__CSVParse_


#include <iostream>
#include "cocos2d.h"
#include <stdio.h>
#include <fstream>

using namespace std;
USING_NS_CC;

class CSVParse : public Ref
{
public:
    
    /** constructor **/
    CSVParse(std::string sep = ",");
    
    /** distructor **/
    ~ CSVParse();
    
    /** creator **/
    static CSVParse* create(const char* csvFileName);
    
    /** get data by rows & columns **/
    const char* getData(unsigned int rows,unsigned int cols);
    
	const char* getDatas(unsigned int rows,const char* colsName);
    
    int findColsData(unsigned int cols,const char* value);
    
    inline vector<vector<string>> getAllDatas()
    {
        return m_vDatas;
    };
    
    inline  int getCols(){return m_iCols;}
    
    inline  int getRows(){return m_vDatas.size();}
    
    
protected:
    
    /** load csv file **/
    bool loadFile(const char* fileName);
    
//    bool getline(std::istream &is,std::string &str);
protected:
    int m_iCols;
    /** separator characters **/
    string fieldsep;
    
    /** data container **/
    vector<vector<string>> m_vDatas;
private:
    /** split strings **/
    void split(vector<string>& field,string line);
    
    /** return next index of separator &  spited string **/
    int advplain(const string& line, string& fld, int);
    
    /** split quoted string  return next index of separator **/
    int advquoted(const string& line, string& fld, int);
    
};

#endif /* defined(__libMake__CSVParse__) */
