#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "linked_list.hpp"


typedef struct JSONNode {
	char* szKey;
	void* pValue;
	int iStartBlockPos = -1;
	int iEndBlockPos = -1;
	int iStartKeyPos = -1;
	int iEndKeyPos = -1;
	int iStartValuePos = -1;
	int iEndValuePos = -1;
} JSONNode;


class JSONReader
{
public:
	JSONReader(const char* path);
	~JSONReader();

	JSONNode* createNode(const char* szKey, void* pValue);

private:
	LinkedList m_data;
};

