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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/propagation-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
using namespace std;
using namespace ns3;

// Default Network Topology
//
// Number of wifi or csma nodes can be increased up to 250
//                          |
//                 Rank 0   |   Rank 1
// -------------------------|----------------------------
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   ================
//                                     LAN 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");
Ptr<PacketSink> sink1;
uint64_t lastTotalRx = 0;                     /* The value of the last total received bytes */
uint32_t MacTxDropCount, PhyTxDropCount, PhyRxDropCount;
int
experiment (std::string rate)
{
  bool verbose = true;
  uint32_t nCsma = 3;
  uint32_t nWifi = 30;
  //bool tracing = false;
  double simulationTime = 10.0;

 if (nWifi > 250 || nCsma > 250)
    {
      std::cout << "Too many wifi or csma nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = p2pNodes.Get (0);
  NodeContainer ServerNode = p2pNodes.Get (1);

  //YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (10.0),
                                 "MinY", DoubleValue (10.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (-10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  

  mobility.Install (wifiStaNodes);

  mobility.Install (wifiApNode);

  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel> ();
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (200); // set default loss to 200 dB (no link)
  for (size_t i = 0; i < nWifi; ++i) {
    lossModel->SetLoss (wifiStaNodes.Get (i)->GetObject<MobilityModel>(), p2pNodes.Get (0)->GetObject<MobilityModel>(), 0);
  }
  wifiChannel->SetPropagationLossModel (lossModel);
  wifiChannel->SetPropagationDelayModel (CreateObject <ConstantSpeedPropagationDelayModel> ());
  phy.SetChannel (wifiChannel);
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  WifiHelper wifi;
  wifi.SetRemoteStationManager (rate);
  wifi.SetStandard(WIFI_PHY_STANDARD_80211b);

  //Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/MacTxDrop", MakeCallback(&MacTxDrop));
  //Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxDrop", MakeCallback(&PhyRxDrop));
  //Config::ConnectWithoutContext("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyTxDrop", MakeCallback(&PhyTxDrop));
  
  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);



  InternetStackHelper stack;
  stack.Install (ServerNode);
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer staInterfaces;
  staInterfaces = address.Assign (staDevices);
  address.Assign (apDevices);

  for(uint32_t i = 0; i< nWifi; i++)
  {
  BulkSendHelper source ("ns3::TcpSocketFactory",
                        InetSocketAddress(staInterfaces.GetAddress (i), 5555));
  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (p2pNodes.Get(1));
  sourceApps.Start(Seconds (0.0));
  sourceApps.Stop(Seconds (10.0));
  } 
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny (), 5555));
  ApplicationContainer sinkApps = sink.Install (wifiStaNodes);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (10.0));
  
  
  //}
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));
  phy.EnablePcapAll("ad");
  Simulator::Run ();
  Simulator::Destroy ();
  for(uint32_t i = 0; i< nWifi; i++)
  {
  sink1 = DynamicCast<PacketSink>(sinkApps.Get(i));
  std::cout << rate <<i+1<<std::endl;
  std::cout << "Total Bytes Received: " << sink1->GetTotalRx() << std::endl;
  std::cout << "Average Throughput: " << ((sink1->GetTotalRx() * 8) / (1e6  * simulationTime)) << " Mbit/s" << std::endl;
  }
  return 0;
}

int
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  /* ... */
  string raas[] = {"ns3::ArfWifiManager",
                   "ns3::AarfWifiManager",
                   "ns3::AarfcdWifiManager",
                   "ns3::AmrrWifiManager",
                   "ns3::CaraWifiManager",
                   "ns3::IdealWifiManager",
                   "ns3::MinstrelWifiManager",
                   "ns3::ParfWifiManager",
                   "ns3::OnoeWifiManager",
                   "ns3::RraaWifiManager"};
  for (unsigned int i = 0; i < sizeof(raas)/sizeof(raas[0]); i++ )
  {
    experiment(raas[i]);
  }
  //experiment(raas[0]);
  return 0;
}
