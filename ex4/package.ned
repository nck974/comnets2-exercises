package ex4;

import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import inet.nodes.inet.StandardHost;


@license(LGPL);


network Ex4Network
{
    @display("bgb=257,302");
    
   	
	types:
        channel Channel extends ned.DatarateChannel
        {
            //datarate = 1Mbps;
            //delay = 200ms;
            //ber = 1e-7;
           datarate=default(1 Mbps);
           delay=default(200ms);
           ber=default(1e-7);
        }
    submodules:
        client: StandardHost {
            @display("p=52,46");
        }
        server: StandardHost {
            @display("p=194,186");
        }
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=56,180");
        }
    connections:
        client.pppg++ <--> Channel <--> server.pppg++;
 }
 
