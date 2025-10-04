/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * coded by Seungmin Lim (smilm789@korea.ac.kr)
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"

/*          BUS Topology
//
//      
//           n1   n3   n5   
//            |    |    |    
//    ========================  LAN
//    10.1.1.0   |    |    |
//              n2   n4   n6
//
//       
//
*/


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  float clientapptime = 2.0;
  float simTime = 11;  
  uint32_t nCsma = 6;
  
//Change throughputtest variable from false to true during throughput test  
  bool throughputtest = true;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

//////////////////////////////// Topology Creation ////////////////////////////////
//**Fill in the code  

// 1. Create NodeContainer
NodeContainer csmaNodes;
csmaNodes.Create (6);


// 2. Create CSMA link and set attributes

CsmaHelper csma;

if (!throughputtest)
   {
        csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
   }
if (throughputtest)
   {
        csma.SetChannelAttribute ("DataRate", StringValue ("15kbps"));
   }

// 3. Create NetDeviceContainer

NetDeviceContainer csmaDevices;
csmaDevices = csma.Install (csmaNodes);

// 4. Create InternetStack and assign IP addresses

InternetStackHelper stack;
stack.Install (csmaNodes);

Ipv4AddressHelper address;
address.SetBase ("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer csmaInterfaces;
csmaInterfaces = address.Assign (csmaDevices);

// 5-1. Application for delay test
  if(!throughputtest){

  // Packet 1
   UdpEchoServerHelper echoServer1 (9);
ApplicationContainer serverApps1 = echoServer1.Install (csmaNodes.Get (3));
   serverApps1.Start (Seconds (1.0));
   serverApps1.Stop (Seconds (30.0));

UdpEchoClientHelper echoClient1 (csmaInterfaces.GetAddress (3), 9);
   echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps1 = echoClient1.Install (csmaNodes.Get (0));
   clientApps1.Start (Seconds (clientapptime));
   clientApps1.Stop (Seconds (30.0));
  }

// 5-2. Application for throughput test 
  if(throughputtest){

  // Packet 1
  UdpEchoServerHelper echoServer1 (9);
ApplicationContainer serverApps1 = echoServer1.Install (csmaNodes.Get (0));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (simTime));

UdpEchoClientHelper echoClient1 (csmaInterfaces.GetAddress (0), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps1 = echoClient1.Install (csmaNodes.Get (3));
  clientApps1.Start (Seconds (clientapptime));
  clientApps1.Stop (Seconds (simTime));

  // Packet 2
  UdpEchoServerHelper echoServer2 (10);
ApplicationContainer serverApps2 = echoServer2.Install (csmaNodes.Get (4));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (simTime));

UdpEchoClientHelper echoClient2 (csmaInterfaces.GetAddress (4), 10);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps2 = echoClient2.Install (csmaNodes.Get (1));
  clientApps2.Start (Seconds (clientapptime));
  clientApps2.Stop (Seconds (simTime));

  // Packet 3
  UdpEchoServerHelper echoServer3 (11);
ApplicationContainer serverApps3 = echoServer3.Install (csmaNodes.Get (5));
  serverApps3.Start (Seconds (1.0));
  serverApps3.Stop (Seconds (simTime));

UdpEchoClientHelper echoClient3 (csmaInterfaces.GetAddress (5), 11);
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps3 = echoClient3.Install (csmaNodes.Get (2));
  clientApps3.Start (Seconds (clientapptime));
  clientApps3.Stop (Seconds (simTime));
  }  

//////////////////////////////// End ////////////////////////////////

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  FlowMonitorHelper flowmonHelper;
  Ptr<FlowMonitor> monitor = flowmonHelper.InstallAll();

  Simulator::Stop(Seconds(100.0));
  Simulator::Run ();
  csma.EnablePcap ("second", csmaDevices.Get (1), true); 
  

  // Flowmonitor for throughput and delay output

  monitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmonHelper.GetClassifier());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
  float rxDuration = (simTime - clientapptime);


  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);

    std::cout << "Flow " << t.sourceAddress << " -> " << t.destinationAddress << "\n";
    std::cout << "  Tx Bytes: " << iter->second.txBytes << "\n";
    std::cout << "  Throughput: " << iter->second.rxBytes * 8.0 / rxDuration/ 1000.0 << "Kbps\n";
    std::cout << "  Mean delay:  " << 1000 * iter->second.delaySum.GetSeconds () / iter->second.rxPackets << " ms\n";
    
    
  }


  Simulator::Destroy ();

  return 0;
}
