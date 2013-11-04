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


/*
 * Current Version:
 * CacheLater uses to well formed hashing algorithms for providing fixed lengths indexs for the Cache.
 * Collision Resolution is non-existent at the moment, complex issue that needs to be able to handle dynamic cleaning of
 * Cache.
 *
 * CacheLayer implementation:
 * Cache is checked: first bloom filter, then Cache
 */
#include "CacheLayer.h"


// Block of inline functions used in both hashing algorithms




Define_Module(CacheLayer);

void CacheLayer::initialize(int stage)
{
    BatteryAccess::initialize(stage);
    if(stage == 0){
        intPacketTTL = par("intPacketTTL");
        CacheSize = par("CacheSize");
        WordSize = par("WordSize");

        Cache = new char*[CacheSize];
        for(int i  = 0; i < CacheSize;i++){
            Cache[i] = new char[WordSize];
        }

        BloomFilter = new int[CacheSize];
        CacheMem = new int[CacheSize];
        TTL = new int[CacheSize];
        timingMsg = new cMessage("Cache scheduling packet", CACHE_SCHEDULING_MESSAGE_TYPE);

    }
    if(stage == 1){
        scheduleAt(simTime() + uniform(0,intPacketTTL),timingMsg);
    }
}

void CacheLayer::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        handleSelfMsg(msg);
    }
}

void CacheLayer::handleSelfMsg(cMessage *msg)
{
    if(msg->getKind() == CACHE_SCHEDULING_MESSAGE_TYPE){

        for(int i = 0; i < CacheSize;i++){
            if(CacheMem[i] == 1){
                TTL[i] -= 1;
                if(TTL[i] == 0){
                    CacheMem[i] = 0;
                }
            }

        }
        scheduleAt(simTime() + intPacketTTL, timingMsg);
    }else {
        opp_error("Wrong message type received, recheck pit configurations");
    }


}
int CacheLayer::checkCache(const char* msgData,uint32_t* k1, uint32_t* k2)
{
    *k1 = MurmurHash3(msgData, WordSize , 1) / CacheSize;
    *k2 = SpookyHash(msgData, WordSize, 1) / CacheSize;

    if(checkBloomFilter(*k1,*k2)){

        if(strcmp(msgData, Cache[*k1]) == 0){
            return 1;
        } else
            return 0;
    }
    else
        return 0;

}

void CacheLayer::updateCache(const char* msgData,int mode,uint32_t k1, uint32_t k2)
{
    if(mode == UPDATE){                                      //update element
        TTL[k1] = FULL_TTL;
    } else if(mode == INSERT){                               //insert element
        strcpy(Cache[k1], msgData);
        updateBloomFilter(k1,k2);
        CacheMem[k1] = 1;
        TTL[k1] = FULL_TTL;
    } else if(mode == DEL){                                //delete element
        CacheMem[k1] = 0;
    } else{
        opp_error("wrong mode command given to cache, recheck cache call");
    }
}

const char* CacheLayer::retreiveCacheData(const char* msgData,uint32_t k1, uint32_t k2)
{
    return Cache[k1];
}

void CacheLayer::updateBloomFilter(uint32_t hash1, uint32_t hash2)
{
    BloomFilter[hash1] = 1;
    BloomFilter[hash2] = 1;
}

int CacheLayer::checkBloomFilter(uint32_t hash1, uint32_t hash2)
{
    if(BloomFilter[hash1] && BloomFilter[hash2]){
        return 1;
    } else
        return 0;
}

/*
 * MurmurHash3, created by Austin Appleby for use in non-cryptographic hashing, is a uniformly distributed and independent function
 * which good performance
 */

uint32_t CacheLayer::MurmurHash3(const char* key, int len, uint32_t seed)
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

    for(int i = -nblocks; i; i++)
    {
        uint32_t k1 = getblock(blocks,i);

        k1 *= c1;
        k1 = ROTL(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3)
    {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
    k1 *= c1; k1 = ROTL(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 = fmix(h1);

    return h1;
}


/*
 * Bob Jenkin's Spooky Hash, a uniformly distibuted and independent function used for its simplicity and efficiency in implementation
 * the 32-bit variant is used to provide hash keys from the original string keys
 */

uint32_t CacheLayer::SpookyHash(const char* message, int len, uint32_t seed)
{
    uint64_t hash1 = seed, hash2 = seed;
    SpookyHash128(message, len, &hash1, &hash2);
    return (uint32_t)hash1;
}

void CacheLayer::HashCollisionResolution(const char* key)
{

}
