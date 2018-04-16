//
//  platform_tools.cpp
//  K-Mion
//
//  Created by DiFFtY on 11/12/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#include "platform_tools.hpp"


char* platformConformPath(const char* szPath) {
    char* szConformedPath = NULL;
    
#ifdef TARGET_3DS
    szConformedPath = new char[strlen(szPath)+1+7];
    strcpy(szConformedPath, "romfs:/");
    strcat(szConformedPath, szPath);
#else
    szConformedPath = new char[strlen(szPath)+1];
    strcpy(szConformedPath, szPath);
#endif
    
    return szConformedPath;
}
