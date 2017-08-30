//
//  mem_manager.cpp
//  Muggine
//
//  Created by DiFFtY on 30/08/2017.
//  Copyright © 2017 RIX. All rights reserved.
//

#include "mem_manager.hpp"

MemManager::MemManager(unsigned int pageSize) :
    m_pageSize(pageSize)
    {
    printf("Initializing memory manager\n");
    
}

MemManager::~MemManager() {
    printf("Destroying memory manager\n");
}

void MemManager::createMemTable() {
    allocatePage();
}

void MemManager::allocatePage(unsigned int pageSize) {
    void* pMemPage = (void *) malloc(pageSize);
}
