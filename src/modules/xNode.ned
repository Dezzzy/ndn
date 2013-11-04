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

package ndn.src.modules;

import org.mixim.modules.node.WirelessNode;
import org.mixim.base.modules.IMobility;
import org.mixim.modules.nic.IWirelessNicUpperCtrl;
import org.mixim.base.modules.IBaseArp;
import org.mixim.base.modules.IBaseApplLayer;
import org.mixim.base.modules.IBaseNetwLayer;
import ndn.src.base.application.IBaseNdnApplLayer;
import ndn.src.base.cache.IBaseNdnCacheLayer;


module XNode
{
    parameters:
        string networkType = default("BaseNetwLayer");       //type of the network layer
        string mobilityType = default("ConstSpeedMobility"); //type of the mobility module
        string arpType = default("BaseArp"); //type of address resolution module
        string nicType = default("NicCSMA");
        string applicationType = default("BaseNdnApplLayer");
        string pitType = default("PendingInterestTable");
        string csType = default("ContentStore");
        string fibType = default("ForwardingInfoBase");
        @display("bgb=210,450,white;i=device/palm;b=40,40,rect");

    gates:
        input radioIn; // gate for sendDirect

    submodules:
        arp: <arpType> like IBaseArp {
            @display("p=159,100;i=block/network2");
        }
        mobility: <mobilityType> like IMobility {
            parameters:
                @display("p=159,30;i=block/cogwheel");
        }
        nic: <nicType> like IWirelessNicUpperCtrl {
            parameters:
                @display("p=70,384;i=block/wrxtx,#008000");
        }
        // OSI network layer
        netwl: <networkType> like IBaseNetwLayer {
            parameters:
                @display("p=70,283;i=block/layer");
        }

        appl: <applicationType> like IBaseNdnApplLayer {

            @display("p=159,283");
        }
        pit: <pitType> like IBaseNdnCacheLayer {

            @display("p=70,30");
        }
        cs: <csType> like IBaseNdnCacheLayer {

            @display("p=70,100");
        }
        fib: <fibType> like IBaseNdnCacheLayer {

            @display("p=70,165");
        }
    connections allowunconnected:
        nic.upperLayerOut --> netwl.lowerLayerIn;
        nic.upperLayerIn <-- netwl.lowerLayerOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> netwl.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- netwl.lowerControlOut;


        netwl.upperLayerOut --> appl.lowerLayerIn;
        netwl.upperLayerIn <-- appl.lowerLayerOut;
        netwl.upperControlOut --> appl.lowerControlIn;
        netwl.upperControlIn <-- appl.lowerControlOut;

        radioIn --> nic.radioIn;
}
