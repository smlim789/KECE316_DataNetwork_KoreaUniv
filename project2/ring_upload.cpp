// ring topology
// coded by Seungmin Lim (smlim789@korea.ac.kr)

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





using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  float clientapptime = 2.0;
  float simTime = 11;  
  
//Change throughputtest variable from false to true during throughput test  
  bool throughputtest = false;


  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }




//////////////////////////////// Topology Creation ////////////////////////////////
//**Fill in the code  

// 1. Create NodeContainer

NodeContainer p2pNodes, p2pNodes1, p2pNodes2, p2pNodes3, p2pNodes4, p2pNodes5, p2pNodes6;
p2pNodes.Create (6);

// Use node 0 as a switch node

p2pNodes1.Add (p2pNodes.Get (0));
p2pNodes1.Add (p2pNodes.Get (1));

p2pNodes2.Add (p2pNodes.Get (1));
p2pNodes2.Add (p2pNodes.Get (2));

p2pNodes3.Add (p2pNodes.Get (2));
p2pNodes3.Add (p2pNodes.Get (3));

p2pNodes4.Add (p2pNodes.Get (3));
p2pNodes4.Add (p2pNodes.Get (4));

p2pNodes5.Add (p2pNodes.Get (4));
p2pNodes5.Add (p2pNodes.Get (5));

p2pNodes6.Add (p2pNodes.Get (5));
p2pNodes6.Add (p2pNodes.Get (0));

// 2. Create P2P links and set attributes

PointToPointHelper pointToPoint;

if (!throughputtest)
   {
        pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
   }
if (throughputtest)
   {
        pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("15kbps"));
   }
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2s"));

// 3. Create NetDeviceContainer

NetDeviceContainer p2pDevices1, p2pDevices2, p2pDevices3, p2pDevices4, p2pDevices5, p2pDevices6;
  p2pDevices1 = pointToPoint.Install(p2pNodes1);
  p2pDevices2 = pointToPoint.Install(p2pNodes2);
  p2pDevices3 = pointToPoint.Install(p2pNodes3);
  p2pDevices4 = pointToPoint.Install(p2pNodes4);
  p2pDevices5 = pointToPoint.Install(p2pNodes5);
  p2pDevices6 = pointToPoint.Install(p2pNodes6);  

// 4. Create InternetStack and assign IP addresses

InternetStackHelper stack;
stack.Install (p2pNodes1.Get(0));
stack.Install (p2pNodes2.Get(0));
stack.Install (p2pNodes3.Get(0));
stack.Install (p2pNodes4.Get(0));
stack.Install (p2pNodes5.Get(0));
stack.Install (p2pNodes6.Get(0));

Ipv4AddressHelper address;
address.SetBase ("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces1;
p2pInterfaces1 = address.Assign (p2pDevices1);

address.SetBase ("10.1.2.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces2;
p2pInterfaces2 = address.Assign (p2pDevices2);

address.SetBase ("10.1.3.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces3;
p2pInterfaces3 = address.Assign (p2pDevices3);

address.SetBase ("10.1.4.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces4;
p2pInterfaces4 = address.Assign (p2pDevices4);

address.SetBase ("10.1.5.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces5;
p2pInterfaces5 = address.Assign (p2pDevices5);

address.SetBase ("10.1.6.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces6;
p2pInterfaces6 = address.Assign (p2pDevices6);

// 5-1. Application for delay test 
 if (!throughputtest){
 
// Packet 1
  UdpEchoServerHelper echoServer1 (9);
ApplicationContainer serverApps1 = echoServer1.Install (p2pNodes3.Get (0)); 
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (30.0));

UdpEchoClientHelper echoClient1 (p2pInterfaces3.GetAddress (0), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));


ApplicationContainer clientApps1 = echoClient1.Install (p2pNodes6.Get (0));
  clientApps1.Start (Seconds (clientapptime));
  clientApps1.Stop (Seconds (30.0));
 }


// 5-2. Application for throughput test
 if (throughputtest){
 
 // Packet 1
  UdpEchoServerHelper echoServer1 (9);
ApplicationContainer serverApps1 = echoServer1.Install (p2pNodes1.Get (0));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (simTime));
  
UdpEchoClientHelper echoClient1 (p2pInterfaces1.GetAddress (0), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps1 = echoClient1.Install (p2pNodes4.Get (0));
  clientApps1.Start (Seconds (clientapptime));
  clientApps1.Stop (Seconds (simTime));


// Packet 2
  UdpEchoServerHelper echoServer2 (10);
ApplicationContainer serverApps2 = echoServer2.Install (p2pNodes5.Get (0));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (simTime));

UdpEchoClientHelper echoClient2 (p2pInterfaces5.GetAddress (0), 10);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps2 = echoClient2.Install (p2pNodes1.Get (1));
  clientApps2.Start (Seconds (clientapptime));
  clientApps2.Stop (Seconds (simTime));

// Packet 3
  UdpEchoServerHelper echoServer3 (11);
ApplicationContainer serverApps3 = echoServer3.Install (p2pNodes6.Get (0));
  serverApps3.Start (Seconds (1.0));
  serverApps3.Stop (Seconds (simTime));

UdpEchoClientHelper echoClient3 (p2pInterfaces6.GetAddress (0), 11);
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps3 = echoClient3.Install (p2pNodes2.Get (1));
  clientApps3.Start (Seconds (clientapptime));
  clientApps3.Stop (Seconds (simTime));
 }
 
//////////////////////////////// End ////////////////////////////////

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  FlowMonitorHelper flowmonHelper;
  Ptr<FlowMonitor> monitor = flowmonHelper.InstallAll();

  Simulator::Stop(Seconds(100.0));
  Simulator::Run ();
  pointToPoint.EnablePcapAll ("second");


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
