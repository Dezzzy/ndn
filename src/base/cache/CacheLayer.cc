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

#include "CacheLayer.h"

inline uint32_t ROTL ( uint32_t x, int8_t r )
{
    return (x << r) | (x >> (32 - r));
}

inline uint32_t fmix ( uint32_t h )
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

inline uint32_t getblock ( const uint32_t * p, int i )
{
    return p[i];
}

Define_Module(CacheLayer);

void CacheLayer::initialize(int stage)
{
    Battery::initialize(stage);
    if(stage == 0){
        CacheSize = par("CacheSize");
        WordSize = par("WordSize");
    }
    if(stage == 1){

    }
}

void CacheLayer::handleMessage(cMessage *msg)
{
    opp_error("module should not be receiving messages from other modules, check connections and configurations");
}

void CacheLayer::handleSelfMsg(cMessage *msg)
{
    opp_error("no self messages should be received, check configuration");
}

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
