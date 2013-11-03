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

#ifndef __NDN_BASENDNAPPLLAYER_H_
#define __NDN_BASENDNAPPLLAYER_H_

#include <omnetpp.h>
#include "FindModule.h"
#include "BaseNetwLayer.h"
#include "AddressingInterface.h"
#include "ArpInterface.h"
#include "BaseModule.h"
#include "NetwControlInfo.h"
#include "ApplPkt_m.h"
#include "BaseApplLayer.h"
// Cache Layer includes
#include "CacheLayer.h"
#include "PendingInterestTable.h"
#include "ContentStore.h"
#include "ForwardingInfoBase.h"


class BaseNdnApplLayer : public BaseApplLayer
{
public:
    enum BASE_NDN_MESSAGE_KINDS{
        BASE_NDN_DATA_MESSAGE = 25251,
        BASE_NDN_INTEREST_MESSAGE,
        BASE_NDN_START_MESSAGE
    };

    enum BASE_NDN_CONTROL_KINDS{

    };
cMessage *startMessage;

protected:
    PendingInterestTable* pit;
    ContentStore* cs;
    ForwardingInfoBase* fib;
    int packetTiming;
    int headerLength;

    LAddress::L3Type myAppAddress;
    LAddress::L3Type destAddress;
    int packetsSent;
    int packetsReceived;
    bool debug;

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleSelfMsg(cMessage *msg);
    virtual void handleLowerMsg(cMessage *msg);
    virtual void handleUpperMsg(cMessage *msg);
    virtual void handleLowerControl(cMessage *msg){
        opp_error("Network Layer should not be sending control data up, check configuration");
        delete msg;
    };
    virtual void handleUpperControl(cMessage *msg);
    virtual const LAddress::L3Type myApplAddr() const {
        return LAddress::L3Type( getParentModule()->getIndex() );
    };
    virtual void sendNextMessage(int messageType, const char*  data);

    virtual void finish();
};

#endif
