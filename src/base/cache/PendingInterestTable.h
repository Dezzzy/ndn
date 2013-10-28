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

protected:
   virtual void initialize(int stage);
   virtual void handleMessage(cMessage *msg);
   virtual void handleSelfMsg(cMessage *msg);
   virtual void updateInsert(const char* msgData, int updateNotInsert);
   virtual int returnData(const char* msgData);
   virtual void removeFromCache(const char* msgData);
   virtual int checkCache(const char* msgData);
   int isSelfRequest(const char*  msgData);
   int isExternalRequest(const char* msgData);
};

#endif
