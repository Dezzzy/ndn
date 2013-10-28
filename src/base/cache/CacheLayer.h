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


class MIXIM_API CacheLayer : public Battery
{
public:
    char** simpleCache;
protected:
    int CacheSize;
    int WordSize;
public:
    virtual void initialize(int stage);

protected:

    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage *msg);
    virtual void updateInsert(const char* msgData, int updateNotInsert);
    virtual int returnData(const char* msgData);
    virtual void removeFromCache(const char* msgData);
    virtual int checkCache(const char* msgData);
    uint32_t MurmurHash3(const char* key, int len, uint32_t seed);
    uint32_t Spooky(const char* key, int len, uint32_t seed);



};

#endif
