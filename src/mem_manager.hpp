//
//  mem_manager.hpp
//  Muggine
//
//  Created by DiFFtY on 30/08/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#ifndef mem_manager_hpp
#define mem_manager_hpp

#include <stdio.h>

class MemManager {
public:
    MemManager(unsigned int pageSize);
    ~MemManager();
    void createMemTable();
    void allocatePage(unsigned int pageSize = 0);
    
private:
    unsigned int m_pageSize;
    unsigned int* m_pMemoryToc;
};

#endif /* mem_manager_hpp */
