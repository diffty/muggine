#ifndef JSON_READER_HPP
#define JSON_READER_HPP


#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>

#include "linked_list.hpp"
#include "common_tools.hpp"


class JSONDict;
class JSONDictItem;

typedef std::map<std::string, JSONDictItem*> json_dict;
typedef std::vector<JSONDictItem*> json_list;

enum EContentType {
	EContentType_UNDEFINED,
	EContentType_INT,
	EContentType_STR,
	EContentType_DICT,
	EContentType_LIST,
};



class JSONDictItem {
public:
	JSONDictItem() {};
	~JSONDictItem() {};

	char* szKey;
	char* szValue;
	void* pValue = NULL;

	EContentType eType = EContentType_UNDEFINED;

	int iStartBlockPos = -1;
	int iEndBlockPos = -1;
	int iStartKeyPos = -1;
	int iEndKeyPos = -1;
	int iStartValuePos = -1;
	int iEndValuePos = -1;

	JSONDict* m_pParent = NULL;

	bool isValid() {
		return iEndValuePos != -1
			&& iStartKeyPos != -1;
	}
};


class JSONDict {
public:
	JSONDict() {};
	~JSONDict() {};

    void print() {
        print_dict(this);
    }
    
	void print_value(JSONDictItem* pCurrItemValue, int level=0) {
        switch (pCurrItemValue->eType) {
            case EContentType_INT:
            {
                int* iValue = (int*) pCurrItemValue->pValue;
                printf("%i\n", *iValue);
                break;
            }
                
            case EContentType_STR:
            {
                char* szValue = (char*) pCurrItemValue->pValue;
                printf("%s\n", szValue);
                break;
            }
                
            case EContentType_DICT:
            {
                JSONDict* pJsonDict = (JSONDict*)pCurrItemValue->pValue;
                this->print_dict(pJsonDict);
                break;
            }
            case EContentType_LIST:
            {
                JSONDict* pJsonDict = (JSONDict*)pCurrItemValue->pValue;
                this->print_dict(pJsonDict);
                break;
            }
        }
	}
    
    void print_dict(JSONDict* pJsonDict, int level=0) {
        switch (pJsonDict->m_eNodeType) {
            case EContentType_DICT:
            {
                json_dict::iterator it = pJsonDict->m_pContentDict.begin();
                
                while (it != pJsonDict->m_pContentDict.end()) {
                    std::string szKey = it->first;
                    
                    JSONDictItem* pItemValue = it->second;
                    
                    JSONDict* pJsonDict = (JSONDict*)pItemValue->pValue;
                    for (int i = 0; i < level; i++) printf("  ");
                    
                    printf("%s: ", szKey.c_str());
                    
                    this->print_value(pItemValue, level+1);
                    
                    it++;
                }
                break;
            }
            case EContentType_LIST:
            {
                json_list::iterator it = pJsonDict->m_pContentList.begin();
                while (it != pJsonDict->m_pContentList.end()) {
                    JSONDictItem* pItemValue = *it;
                    
                    for (int i = 0; i < level; i++) printf("  ");
                    this->print_value(pItemValue, level+1);
                    
                    it++;
                }
                break;
            }
        }
    }
    
	JSONDict* m_pParentDict = NULL;
	JSONDictItem* m_pParentItem = NULL;
	json_dict m_pContentDict;
	json_list m_pContentList;
    EContentType m_eNodeType = EContentType_UNDEFINED;
};


class JSONReader
{
public:
	JSONReader(const char* path);
	~JSONReader();

	JSONDictItem* createItem(const char* szKey, void* pValue);
	void parseNode(JSONDictItem*);
	void trunctStr(char* str, char** result);
	void fillNode(JSONDictItem* pJsonNode, FILE* fp);

	JSONDict* m_pRootDict = NULL;
    JSONDictItem* m_pRootItem = NULL;
};


#endif
