//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "ContentStore.h"

Define_Module(ContentStore);

void ContentStore::initialize(int stage)
{
    CacheLayer::initialize(stage);
    if(stage == 0){

    }
}

void ContentStore::updateCache(const char* msgData, uint32_t k1, uint32_t k2, int lifeSpan)
{
    if(mode == UPDATE){                                      //update element
        TTL[k1] = lifeSpan;
    } else if(mode == INSERT){                               //insert element
        strcpy(Cache[k1], msgData);
        updateBloomFilter(k1,k2);
        CacheMem[k1] = 1;
        TTL[k1] = lifeSpan;
    } else if(mode == DEL){                                //delete element
        CacheMem[k1] = 0;
    } else{
        opp_error("wrong mode command given to cache, recheck cache call");
    }
}
