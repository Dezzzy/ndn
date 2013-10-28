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

#include "BaseNdnApplLayer.h"


Define_Module(BaseNdnApplLayer);

void BaseNdnApplLayer::initialize(int stage)
{
    BaseApplLayer::initialize(stage);
    if(stage == 0){
        packetTiming = par("packetTiming");
        headerLength = par("headerLength");
        //debug = par("debug");
        // pointer to NDN cache components
        PendingInterestTable* pit = FindModule<PendingInterestTable*>::findSubModule(findHost());
        ContentStore* cs = FindModule<ContentStore*>::findSubModule(findHost());
        ForwardingInfoBase* fib = FindModule<ForwardingInfoBase*>::findSubModule(findHost());

        startingPacket = new cMessage("first packet", BASE_NDN_START_MESSAGE);
    }
    if(stage == 1){
       scheduleAt(simTime() +uniform(0, packetTiming), startingPacket);
    }

}

void BaseNdnApplLayer::handleSelfMsg(cMessage *msg){
    switch(msg->getKind()){
    case BASE_NDN_START_MESSAGE:
        sendNextMessage(11,msg->getName());
        break;
    default:
        opp_error("self message must be starting message, recheck message pathing");
        break;
    }
}

void BaseNdnApplLayer::handleLowerMsg(cMessage *msg){
    ApplPkt* m;
    int Hit = 0;
    switch(msg->getKind()){
    case BASE_NDN_DATA_MESSAGE:
        /*
         * Hit = pit->checkCache(message_data)
         * else
         *     do nothing
         * if(Hit)
         *      if(pit->isSelfReq(message_data))
         *          cs->addToCache(message_data)
         *      if(pit->isExtReq(message_data))
         *          sendNextMessage(data, message_data)
         */
        Hit = 0;
        m = static_cast<ApplPkt *>(msg);
        Hit = pit->checkCache(m->getName());
        if(Hit){
            if(pit->isSelfRequest(m->getName())){

            }
            if(pit->isExtReqeust(m.getName())){

            }

        }
        delete msg;
        break;
    case BASE_NDN_INTEREST_MESSAGE:
        /*
         * Hit = cs->checkCache(message_data)
         * if(Hit)
         *      sendNextMessage(data, message_data)
         * else
         *      Hit = 0
         *      Hit = pit->checkCache(message_data)
         *      if(Hit)
         *          pit->updateCache(message_data)
         *      else
         *          pit->addToCache(message_data)
         *
         *      sendNextMessage(interest, message_data)
         */

        Hit = 0;
        m = static_cast<ApplPkt *>(msg);
        Hit = cs->checkCache(m.getName());
        if(Hit){
            sendNextMessage(m.getName(),m.getName());

        }else{
            Hit = 0;
            Hit = pit->checkCache(m.getName());
            if(Hit){

            } else{

            }

        }
        delete msg;
        break;
    }
}

/* Sends next message, until simulation stops.
 *
 */
void BaseNdnApplLayer::sendNextMessage(int messageType, const char* data)
{

    ApplPkt *pkt = new ApplPkt(data, messageType);
    pkt->setDestAddr(LAddress::L3BROADCAST);
    pkt->setSrcAddr(myAppAddr());
    pkt->setByteLength(headerLength);
    NetwControlInfo::setControlInfo(pkt, pkt->getDestAddr());
    sendDelayedDown(pkt);
}

void BaseNdnApplLayer::finish(){

}
