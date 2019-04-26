#ifndef JSON_READER_HPP
#define JSON_READER_HPP


#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>

#include "linked_list.hpp"
#include "common_tools.hpp"


class JSONDict;
class JSONDictItem;

typedef std::map<std::string, JSONDictItem*> json_dict_map;


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
		return iStartValuePos != -1
			&& iEndValuePos != -1
			&& iStartKeyPos != -1
			&& iEndKeyPos != -1;
	}
};


class JSONDict {
public:
	JSONDict() {};
	~JSONDict() {};

	void print(int level=0) {
		json_dict_map::iterator it = this->m_pContentDict.begin();

		while (it != this->m_pContentDict.end()) {
			std::string szKey = it->first;

			JSONDictItem* pItemValue = it->second;

			for (int i = 0; i < level; i++) printf("  ");

			switch (pItemValue->eType) {
			case EContentType_INT:
			{
				int* iValue = (int*) pItemValue->pValue;
				printf("%s: %i\n", szKey.c_str(), *iValue);
				break;
			}

			case EContentType_STR:
			{
				char* szValue = (char*) pItemValue->pValue;
				printf("%s: %s\n", szKey.c_str(), szValue);
				break;
			}

			case EContentType_DICT:
			{
				JSONDict* pJsonDict = (JSONDict*)pItemValue->pValue;
				printf("%s: \n", szKey.c_str());
				pJsonDict->print(level+1);
				break;
			}
			case EContentType_LIST:
			{
				LinkedList* pLinkedList = (LinkedList*)pItemValue->pValue;
				printf("%s: \n", szKey.c_str());
				//pJsonDict->print(level + 1);
				break;
			}
			}

			it++;
		}
	}

	JSONDict* m_pParentDict = NULL;
	JSONDictItem* m_pParentItem = NULL;
	json_dict_map m_pContentDict;
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
};


#endif