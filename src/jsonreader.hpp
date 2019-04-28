#ifndef JSON_READER_HPP
#define JSON_READER_HPP


#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>

#include "linked_list.hpp"
#include "common_tools.hpp"


class JSONItem;
class JSONItemKeyValue;

typedef std::map<std::string, JSONItemKeyValue*> json_dict_t;
typedef std::vector<JSONItemKeyValue*> json_list_t;


enum EContentType {
	EContentType_UNDEFINED,
	EContentType_INT,
	EContentType_STR,
	EContentType_DICT,
	EContentType_LIST,
	EContentType_EMPTY,
};


class JSONItemKeyValue {
public:
	JSONItemKeyValue() {};
	~JSONItemKeyValue() {};

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

	JSONItem* m_pParent = NULL;

	bool isValid() {
		return iEndValuePos != -1
			&& iStartKeyPos != -1;
	}
};


class JSONItem {
public:
	JSONItem() {};
	~JSONItem() {};

	JSONItem* m_pParentDict = NULL;
	JSONItemKeyValue* m_pParentItem = NULL;
	EContentType m_eNodeType = EContentType_UNDEFINED;
	json_dict_t m_contentDict;
	json_list_t m_contentList;

	template <typename T>
	T get(char* szKeyName) {
		if (m_contentDict.count(szKeyName) > 0 ) {
			return (T) m_contentDict[szKeyName]->pValue;
		}
		else {
			return NULL;
		}

	}
	
	template <typename T>
	T get(int iItemNum) {
		if (m_contentList.size() > iItemNum) {
			return (T)m_contentList[iItemNum]->pValue;
		}
		else {
			return NULL;
		}
	}

	void print() {
        print_dict(this);
    }
    
	void print_value(JSONItemKeyValue* pCurrItemValue, int level=0) {
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
                JSONItem* pJsonDict = (JSONItem*)pCurrItemValue->pValue;
                this->print_dict(pJsonDict, level + 1);
                break;
            }
            case EContentType_LIST:
            {
                JSONItem* pJsonDict = (JSONItem*)pCurrItemValue->pValue;
                this->print_dict(pJsonDict, level + 1);
                break;
            }
        }
	}
    
    void print_dict(JSONItem* pJsonDict, int level=0) {
        switch (pJsonDict->m_eNodeType) {
            case EContentType_DICT:
            {
                json_dict_t::iterator it = pJsonDict->m_contentDict.begin();
                
                while (it != pJsonDict->m_contentDict.end()) {
                    std::string szKey = it->first;
                    
                    JSONItemKeyValue* pItemValue = it->second;
                    
                    JSONItem* pJsonDict = (JSONItem*)pItemValue->pValue;
                    for (int i = 0; i < level; i++) printf("  ");
                    
                    printf("%s: ", szKey.c_str());
                    
                    this->print_value(pItemValue, level + 1);
                    
                    it++;
                }
                break;
            }
            case EContentType_LIST:
            {
                json_list_t::iterator it = pJsonDict->m_contentList.begin();
                while (it != pJsonDict->m_contentList.end()) {
                    JSONItemKeyValue* pItemValue = *it;
                    
                    for (int i = 0; i < level; i++) printf("  ");
                    this->print_value(pItemValue, level + 1);
                    
                    it++;
                }
                break;
            }
        }
    }
};


class JSONReader
{
public:
	JSONReader(const char* path);
	~JSONReader();

	JSONItemKeyValue* createItem(const char* szKey, void* pValue);
	bool parseNode(JSONItemKeyValue*);
	void fillNode(JSONItemKeyValue* pJsonNode, FILE* fp);
	bool trunctStr(char* str, char** result);

	template <typename T>
	T get(char* szKeyName) {
		return m_pRootDict->get<T>(szKeyName);
	}

	JSONItem* m_pRootDict = NULL;
    JSONItemKeyValue* m_pRootItem = NULL;
};


#endif
