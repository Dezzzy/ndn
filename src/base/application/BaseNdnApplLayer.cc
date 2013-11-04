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
        //DataSetSize = par("DataSetSize");
        //debug = par("debug");
        // pointer to NDN cache components
        pit = FindModule<PendingInterestTable*>::findSubModule(findHost());
        cs = FindModule<ContentStore*>::findSubModule(findHost());
        fib = FindModule<ForwardingInfoBase*>::findSubModule(findHost());

        startMessage = new cMessage("first packet", BASE_NDN_START_MESSAGE);
        for(int i  = 0; i < 10;i++){
            intData[i] = new char[100];
        }
    }
    if(stage == 1){
        strcpy(intData[0],"car");
        strcpy(intData[1],"place");
        strcpy(intData[2],"tool");
        strcpy(intData[3],"person");
        strcpy(intData[4],"state");
        strcpy(intData[5],"animal");
        strcpy(intData[6],"emotion");
        strcpy(intData[7],"car");
        strcpy(intData[8],"car");
        strcpy(intData[9],"car");

        scheduleAt(simTime() +uniform(0, packetTiming), startMessage);
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

void BaseNdnApplLayer::handleLowerMsg(cMessage *msg)
{
    ApplPkt* m;
    int Hit = 0;
    uint32_t pitHash1,pitHash2;
    uint32_t csHash1,csHash2;

    m = static_cast<ApplPkt *>(msg);

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

        Hit = pit->checkCache(m->getName(), &pitHash1, &pitHash2);
        if(Hit){
            if(pit->isSelfRequest(pitHash1)){
                cs->checkCache(m->getName(),&csHash1, &csHash2);
                cs->updateCache(m->getName(),CacheLayer::INSERT,csHash1,csHash2,CacheLayer::FULL_TTL);
            }
            if(pit->isExternalRequest(pitHash1)){
                sendNextMessage(BASE_NDN_DATA_MESSAGE,m->getName());
            }
            pit->updateCache(m->getName(),PendingInterestTable::DEL,pitHash1,pitHash2,PendingInterestTable::NO_REQ);

        } else{
            Hit = 0;
            Hit = cs->checkCache(m->getName(), &csHash1,&csHash2);
            if(Hit){
                cs->updateCache(m->getName(),CacheLayer::UPDATE, csHash1,csHash2,CacheLayer::HALF_TTL);
            } else {
                cs->updateCache(m,getName(),CacheLayer::INSERT,csHash1,csHash2,CacheLayer::HALF_TTL);
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

        Hit = cs->checkCache(m->getName(),&csHash1,&csHash2);

        if(Hit){
            sendNextMessage(BASE_NDN_DATA_MESSAGE,m->getName());
        }else{
            Hit = 0;
            Hit = pit->checkCache(m->getName(),&pitHash1, &pitHash2);

            if(Hit){
                pit->updateCache(m->getName(),PendingInterestTable::UPDATE,k1,k2,PendingInterestTable::EXTERNAL_REQ);
            } else{
                pit->updateCache(m->getName(),PendingInterestTable::INSERT,k1,k2,PendingInterestTable::EXTERNAL_REQ);
                sendNextMessage(BASE_NDN_INTEREST_MESSAGE,m.getName());

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

    simtime_t delay;

    delay = uniform(0, 0.01);

    ApplPkt *pkt = new ApplPkt(data, messageType);

    pkt->setDestAddr(LAddress::L3BROADCAST);
    pkt->setSrcAddr(myApplAddr());
    pkt->setByteLength(headerLength);
    NetwControlInfo::setControlInfo(pkt, pkt->getDestAddr());
    sendDelayedDown(pkt,delay);
}

void BaseNdnApplLayer::finish(){

}

void BaseNdnApplLayer::genInterestPacket()
{
    uint32_t h1,h2;
    int selector;
    srand(time(NULL));
    selector = rand() % 10 + 1;
    if(!pit->checkCache(intData[selector],&h1,&h2)){
        sendNectMessage(BASE_NDN_INTEREST_MESSAGE,intData);
        pit->updateCache(intData[selector],PendingInterestTable::INSERT,h1,h2,PendingInterestTable::SELF_REQ);
    }
}
