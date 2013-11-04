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

#include "PendingInterestTable.h"

Define_Module(PendingInterestTable);

void PendingInterestTable::initialize(int stage)
{
    CacheLayer::initialize(stage);
    if(stage == 0){

        reqType = new int[CacheSize];
        for(int i= 0; i < CacheSize; i++){
            reqType = NO_REQ;
        }

    }
    if(stage == 1){


    }
}

int PendingInterestTable::isSelfRequest(uint32_t h1, uint32_t h2)
{
    if(reqType[h1]== SELF_REQ || reqType[h1 == SELF_EXT_REQ]){
        return 1;
    } else
        return 0;
}

int PendingInterestTable::isExternalRequest(const char* msgData)
{
    if(reqType[h1]== EXT_REQ || reqType[h1] == SELF_EXT_REQ){
        return 1;
    } else
        return 0;
}

void PendingInterestTable::setRequestType(short type)
{
    if(type == NO_REQ){
        reqType = type;
    }else if (reqType != type){
        if(reqType == NO_REQ){
            reqType = type;
        }
        else
            reqType = SELF_EXT_REQ;
    }
}

virtual void PendingInterestTable::updateCache(const char* msgData,int mode,uint32_t k1, uint32_t k2,short type)
{
    CacheLayer::updateCache(msgData,mode,k1,k2);
    setRequestType(type);
    TTL[k1] = 10;
}


