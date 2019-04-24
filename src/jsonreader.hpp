#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>

#include "linked_list.hpp"
#include "common_tools.hpp"


enum EContentType {
	EContentType_INT,
	EContentType_STR,
	EContentType_DICT,
	EContentType_LIST,
};


class JSONNode {
public:
	JSONNode() {}
	~JSONNode() {}

	char* szKey;
	char* szValue;
	void* pValue = NULL;
	EContentType eType;

	int iStartBlockPos = -1;
	int iEndBlockPos = -1;
	int iStartKeyPos = -1;
	int iEndKeyPos = -1;
	int iStartValuePos = -1;
	int iEndValuePos = -1;
};


class JSONReader
{
public:
	JSONReader(const char* path);
	~JSONReader();

	JSONNode* createNode(const char* szKey, void* pValue);
	void parseNode(JSONNode*);
	void trunctStr(char* str, char** result);
	void fillNode(JSONNode* pJsonNode, FILE* fp);

//private:
	//LinkedList m_data;
	std::map<std::string, JSONNode*>* pm_mContentDict;
};
