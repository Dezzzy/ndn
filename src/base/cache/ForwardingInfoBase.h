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

#ifndef __NDN_FORWARDINGINFOBASE_H_
#define __NDN_FORWARDINGINFOBASE_H_

#include <omnetpp.h>
#include "CacheLayer.h"
/**
 * TODO - Generated class
 */
class ForwardingInfoBase : public CacheLayer
{
public:
    virtual int checkCache(const char* msgData);
    virtual void updateCache(const char* msgData,int mode);
    virtual const char* retreiveCacheData(const char* msgData);

    virtual void updateBloomFilter(const char* key);
    virtual int checkBloomFilter(const char* key);
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
};

#endif
