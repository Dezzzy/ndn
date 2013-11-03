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

#ifndef __NDN_PENDINGINTERESTTABLE_H_
#define __NDN_PENDINGINTERESTTABLE_H_

#include <omnetpp.h>

#include "CacheLayer.h"


class PendingInterestTable : public CacheLayer
{
public:
    enum INTEREST_REQUEST_TYPES{
        NO_REQ = 10,
        SELF_REQ ,
        EXTERNAL_REQ,
        SELF_EXT_REQ

    };

    int* reqType;
public:
    int isSelfRequest(const char*  msgData);
    int isExternalRequest(const char* msgData);
    void setRequestType(short type);
    virtual int checkCache(const char* msgData,uint32_t* k1, uint32_t* k2);
    virtual void updateCache(const char* msgData,int mode,uint32_t k1, uint32_t k2,type);
    virtual const char* retreiveCacheData(const char* msgData,uint32_t k1, uint32_t k2);

    virtual void updateBloomFilter(uint32_t hash1, uint32_t hash2);
    virtual int checkBloomFilter(uint32_t hash1, uint32_t hash2);

protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage *msg);


};

#endif
