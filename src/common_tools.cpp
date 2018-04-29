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
