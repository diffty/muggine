#include <stdlib.h>

#include "common_tools.hpp"


int intFromStr(const char* szStr) {
	int i = 0;
	long lStrLen = strlen(szStr);
	long iDigitNum = 0;
	int iRes = 0;

	char c;

	while ((c = szStr[i]) != '\0') {
		iDigitNum = (lStrLen - 1) - i;
		iRes += atoi(&c) * pow(10, iDigitNum);

		i++;
	}

	return iRes;
}


void spliceString(const char* szSrcString, int iStartPos, int iEndPos, char* szDstString) {
	for (int i = iStartPos; i <= iEndPos; i++) {
		szDstString[i - iStartPos] = szSrcString[i];
	}

	szDstString[iEndPos - iStartPos + 1] = '\0';
}