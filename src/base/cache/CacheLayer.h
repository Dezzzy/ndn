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

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.


#ifndef __NDN_CACHELAYER_H_
#define __NDN_CACHELAYER_H_

#include <omnetpp.h>
#include "BatteryAccess.h"
#include "stdint.h"
#include "winsupport.h"
#include "string.h"
#include "BatteryAccess.h"

#define ALLOW_UNALIGNED_READS 1

class MIXIM_API CacheLayer : public BatteryAccess
{
public:
    enum CACHE_MODE_KINDS{
        UPDATE = 0,
        INSERT = 1,
        DEL = 2
    };

    enum CACHE_MESSAGE_KINDS{
        CACHE_SCHEDULING_MESSAGE_TYPE = 25300
    };

    enum CACHE_TTL_KINDS{
        FULL_TTL = 100,
        HALF_TTL
    };

    char** Cache;
    int* CacheMem;
    int* BloomFilter;
    int* TTL;

    int intPacketTTL;
    cMessage* timingMsg;




    static inline uint64_t Rot64(uint64 x, int k)
    {
        return (x << k) | (x >> (64 - k));
    }

    static inline void ShortEnd(uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3)
    {
        h3 ^= h2;  h2 = Rot64(h2,15);  h3 += h2;
        h0 ^= h3;  h3 = Rot64(h3,52);  h0 += h3;
        h1 ^= h0;  h0 = Rot64(h0,26);  h1 += h0;
        h2 ^= h1;  h1 = Rot64(h1,51);  h2 += h1;
        h3 ^= h2;  h2 = Rot64(h2,28);  h3 += h2;
        h0 ^= h3;  h3 = Rot64(h3,9);   h0 += h3;
        h1 ^= h0;  h0 = Rot64(h0,47);  h1 += h0;
        h2 ^= h1;  h1 = Rot64(h1,54);  h2 += h1;
        h3 ^= h2;  h2 = Rot64(h2,32);  h3 += h2;
        h0 ^= h3;  h3 = Rot64(h3,25);  h0 += h3;
        h1 ^= h0;  h0 = Rot64(h0,63);  h1 += h0;
    }

    static inline void EndPartial(
            uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3,
            uint64_t &h4, uint64_t &h5, uint64_t &h6, uint64_t &h7,
            uint64_t &h8, uint64_t &h9, uint64_t &h10,uint64_t &h11)
    {
        h11+= h1;    h2 ^= h11;   h1 = Rot64(h1,44);
        h0 += h2;    h3 ^= h0;    h2 = Rot64(h2,15);
        h1 += h3;    h4 ^= h1;    h3 = Rot64(h3,34);
        h2 += h4;    h5 ^= h2;    h4 = Rot64(h4,21);
        h3 += h5;    h6 ^= h3;    h5 = Rot64(h5,38);
        h4 += h6;    h7 ^= h4;    h6 = Rot64(h6,33);
        h5 += h7;    h8 ^= h5;    h7 = Rot64(h7,10);
        h6 += h8;    h9 ^= h6;    h8 = Rot64(h8,13);
        h7 += h9;    h10^= h7;    h9 = Rot64(h9,38);
        h8 += h10;   h11^= h8;    h10= Rot64(h10,53);
        h9 += h11;   h0 ^= h9;    h11= Rot64(h11,42);
        h10+= h0;    h1 ^= h10;   h0 = Rot64(h0,54);
    }

    static inline void End(
            uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3,
            uint64_t &h4, uint64_t &h5, uint64_t &h6, uint64_t &h7,
            uint64_t &h8, uint64_t &h9, uint64_t &h10,uint64_t &h11)
    {
        EndPartial(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
        EndPartial(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
        EndPartial(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
    }

    static inline void ShortMix(uint64_t &h0, uint64_t &h1, uint64_t &h2, uint64_t &h3)
    {
        h2 = Rot64(h2,50);  h2 += h3;  h0 ^= h2;
        h3 = Rot64(h3,52);  h3 += h0;  h1 ^= h3;
        h0 = Rot64(h0,30);  h0 += h1;  h2 ^= h0;
        h1 = Rot64(h1,41);  h1 += h2;  h3 ^= h1;
        h2 = Rot64(h2,54);  h2 += h3;  h0 ^= h2;
        h3 = Rot64(h3,48);  h3 += h0;  h1 ^= h3;
        h0 = Rot64(h0,38);  h0 += h1;  h2 ^= h0;
        h1 = Rot64(h1,37);  h1 += h2;  h3 ^= h1;
        h2 = Rot64(h2,62);  h2 += h3;  h0 ^= h2;
        h3 = Rot64(h3,34);  h3 += h0;  h1 ^= h3;
        h0 = Rot64(h0,5);   h0 += h1;  h2 ^= h0;
        h1 = Rot64(h1,36);  h1 += h2;  h3 ^= h1;
    }

    static inline void Mix(
            const uint64_t *data,
            uint64_t &s0, uint64_t &s1, uint64_t &s2, uint64_t &s3,
            uint64_t &s4, uint64_t &s5, uint64_t &s6, uint64_t &s7,
            uint64_t &s8, uint64_t &s9, uint64_t &s10,uint64_t &s11)
    {
        s0 += data[0];    s2 ^= s10;    s11 ^= s0;    s0 = Rot64(s0,11);    s11 += s1;
        s1 += data[1];    s3 ^= s11;    s0 ^= s1;    s1 = Rot64(s1,32);    s0 += s2;
        s2 += data[2];    s4 ^= s0;    s1 ^= s2;    s2 = Rot64(s2,43);    s1 += s3;
        s3 += data[3];    s5 ^= s1;    s2 ^= s3;    s3 = Rot64(s3,31);    s2 += s4;
        s4 += data[4];    s6 ^= s2;    s3 ^= s4;    s4 = Rot64(s4,17);    s3 += s5;
        s5 += data[5];    s7 ^= s3;    s4 ^= s5;    s5 = Rot64(s5,28);    s4 += s6;
        s6 += data[6];    s8 ^= s4;    s5 ^= s6;    s6 = Rot64(s6,39);    s5 += s7;
        s7 += data[7];    s9 ^= s5;    s6 ^= s7;    s7 = Rot64(s7,57);    s6 += s8;
        s8 += data[8];    s10 ^= s6;    s7 ^= s8;    s8 = Rot64(s8,55);    s7 += s9;
        s9 += data[9];    s11 ^= s7;    s8 ^= s9;    s9 = Rot64(s9,54);    s8 += s10;
        s10 += data[10];    s0 ^= s8;    s9 ^= s10;    s10 = Rot64(s10,22);    s9 += s11;
        s11 += data[11];    s1 ^= s9;    s10 ^= s11;    s11 = Rot64(s11,46);    s10 += s0;
    }

    /*
     * ROTL, this function is used to rotate the hashed before final hash is complete
     */
    inline uint32_t ROTL ( uint32_t x, int8_t r )
    {
        return (x << r) | (x >> (32 - r));
    }

    /*
     * fmix, final mixing of bytes for releasing hash
     */
    inline uint32_t fmix ( uint32_t h )
    {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
    }

    /*
     * GetBlock, used as a byte block read for MurmurHash3, used to seperate key, allowing easy folding of data when used in hashing
     */
    inline uint32_t getblock ( const uint32_t * p, int i )
    {
        return p[i];
    }

    // end of inline functions


    virtual void initialize(int stage);
    virtual int checkCache(const char* msgData,uint32_t* k1, uint32_t* k2);
    virtual void updateCache(const char* msgData,int mode,uint32_t k1, uint32_t k2);
    virtual const char* retreiveCacheData(const char* msgData,uint32_t k1, uint32_t k2);

    virtual void updateBloomFilter(uint32_t hash1, uint32_t hash2);
    virtual int checkBloomFilter(uint32_t hash1, uint32_t hash2);



protected:

    int CacheSize;
    int WordSize;
    // number of uint64's in internal state
    static const size_t sc_numVars = 12;

    // size of the internal state
    static const size_t sc_blockSize = sc_numVars*8;

    // size of buffer of unhashed data, in bytes
    static const size_t sc_bufSize = 2*sc_blockSize;

    //
    // sc_const: a constant which:
    //  * is not zero
    //  * is odd
    //  * is a not-very-regular mix of 1's and 0's
    //  * does not need any other special mathematical properties
    //
    static const uint64 sc_const = 0xdeadbeefdeadbeefULL;

    uint64_t m_data[2*sc_numVars];   // unhashed data, for partial messages
    uint64_t m_state[sc_numVars];  // internal state of the hash
    size_t m_length;             // total length of the input so far
    uint8_t  m_remainder;          // length of unhashed data stashed in m_data

    virtual void handleMessage(cMessage *msg);

    virtual void handleSelfMsg(cMessage *msg);




    // Hashing related functions
    uint32_t MurmurHash3(const char* key, int len, uint32_t seed);
    uint32_t SpookyHash(const char* key, int len, uint32_t seed);

    void SpookyHash128(const char* message ,size_t length, uint64_t* hash1, uint64_t* hash2)
    {
        if (length < sc_bufSize)
        {
            SpookyShortHash(message, length, hash1, hash2);
            return;
        }

        uint64_t h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11;
        uint64_t buf[sc_numVars];
        uint64_t *end;
        union
        {
            const uint8_t *p8;
            uint64_t *p64;
            size_t i;
        } u;
        size_t remainder;

        h0=h3=h6=h9  = *hash1;
        h1=h4=h7=h10 = *hash2;
        h2=h5=h8=h11 = sc_const;

        u.p8 = (const uint8_t *)message;
        end = u.p64 + (length/sc_blockSize)*sc_numVars;

        // handle all whole sc_blockSize blocks of bytes
        if (ALLOW_UNALIGNED_READS || ((u.i & 0x7) == 0))
        {
            while (u.p64 < end)
            {
                Mix(u.p64, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
                u.p64 += sc_numVars;
            }
        }
        else
        {
            while (u.p64 < end)
            {
                memcpy(buf, u.p64, sc_blockSize);
                Mix(buf, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
                u.p64 += sc_numVars;
            }
        }

        // handle the last partial block of sc_blockSize bytes
        remainder = (length - ((const uint8_t *)end-(const uint8_t *)message));
        memcpy(buf, end, remainder);
        memset(((uint8_t *)buf)+remainder, 0, sc_blockSize-remainder);
        ((uint8_t *)buf)[sc_blockSize-1] = remainder;
        Mix(buf, h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);

        // do some final mixing
        End(h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11);
        *hash1 = h0;
        *hash2 = h1;
    }

    void SpookyShortHash(const char* message, size_t length, uint64_t* hash1, uint64_t* hash2)
    {
        uint64_t buf[sc_numVars];
        union
        {
            const uint8_t *p8;
            uint32_t *p32;
            uint64_t *p64;
            size_t i;
        } u;

        u.p8 = (const uint8_t *)message;

        if (!ALLOW_UNALIGNED_READS && (u.i & 0x7))
        {
            memcpy(buf, message, length);
            u.p64 = buf;
        }

        size_t remainder = length%32;
        uint64_t a=*hash1;
        uint64_t b=*hash2;
        uint64_t c=sc_const;
        uint64_t d=sc_const;

        if (length > 15)
        {
            const uint64_t *end = u.p64 + (length/32)*4;

            // handle all complete sets of 32 bytes
            for (; u.p64 < end; u.p64 += 4)
            {
                c += u.p64[0];
                d += u.p64[1];
                ShortMix(a,b,c,d);
                a += u.p64[2];
                b += u.p64[3];
            }

            //Handle the case of 16+ remaining bytes.
            if (remainder >= 16)
            {
                c += u.p64[0];
                d += u.p64[1];
                ShortMix(a,b,c,d);
                u.p64 += 2;
                remainder -= 16;
            }
        }

        // Handle the last 0..15 bytes, and its length
        d = ((uint64_t)length) << 56;
        switch (remainder)
        {
        case 15:
            d += ((uint64_t)u.p8[14]) << 48;
        case 14:
            d += ((uint64_t)u.p8[13]) << 40;
        case 13:
            d += ((uint64_t)u.p8[12]) << 32;
        case 12:
            d += u.p32[2];
            c += u.p64[0];
            break;
        case 11:
            d += ((uint64_t)u.p8[10]) << 16;
        case 10:
            d += ((uint64_t)u.p8[9]) << 8;
        case 9:
            d += (uint64_t)u.p8[8];
        case 8:
            c += u.p64[0];
            break;
        case 7:
            c += ((uint64_t)u.p8[6]) << 48;
        case 6:
            c += ((uint64_t)u.p8[5]) << 40;
        case 5:
            c += ((uint64_t)u.p8[4]) << 32;
        case 4:
            c += u.p32[0];
            break;
        case 3:
            c += ((uint64_t)u.p8[2]) << 16;
        case 2:
            c += ((uint64_t)u.p8[1]) << 8;
        case 1:
            c += (uint64_t)u.p8[0];
            break;
        case 0:
            c += sc_const;
            d += sc_const;
        }
        ShortEnd(a,b,c,d);
        *hash1 = a;
        *hash2 = b;
    }

    void HashCollisionResolution(const char* key);
};

#endif
