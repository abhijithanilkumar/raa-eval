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
#include <fstream>
#include "ns3/gnuplot.h"
using namespace std;
using namespace ns3;

// Default Network Topology
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n4   n3   n2   n0 -------------- n1
//                   point-to-point


NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");
Ptr<PacketSink> apSink, staSink;
const uint32_t nWifi = 30;
uint64_t lastTotalRx[nWifi] = {};    /* The value of the last total received bytes */

uint64_t lastTotalRxAp = 0;
uint32_t MacTxDropCount, PhyTxDropCount, PhyRxDropCount;
ApplicationContainer sinkApps;
std::fstream file;

void initialize ()
{
  for (uint32_t i = 0; i < nWifi; i++)
    lastTotalRx[i] = 0;
}

void
CalculateThroughput ()
{
  Time now = Simulator::Now ();
  double sum = 0, avg = 0;
  sum = 0;
  for(uint32_t i = 0; i< nWifi; i++)
  {
    staSink = StaticCast<PacketSink>(sinkApps.Get(i));
    double cur = (staSink->GetTotalRx() - lastTotalRx[i]) * (double) 8/1e5;     /* Convert Application RX Packets to MBits. */
    lastTotalRx[i] = staSink->GetTotalRx ();
    sum += cur;
  }
  avg = sum/nWifi;
  double cur = (apSink->GetTotalRx() - lastTotalRxAp) * (double) 8/1e5;     /* Convert Application RX Packets to MBits. */
  lastTotalRxAp = apSink->GetTotalRx ();

  //Write the value of avg Throughput to a file.
  file << now.GetSeconds () << "\t" <<avg << std::endl;
  std::cout << now.GetSeconds () << "s: \t" << " " << avg << " Mbit/s" << " " << cur << std::endl;
  Simulator::Schedule (MilliSeconds (100), &CalculateThroughput);
}

int
experiment (std::string rate)
{
  double simulationTime = 10.0;

  /* No fragmentation and no RTS/CTS */
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("999999"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("999999"));
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = p2pNodes.Get (0);
  NodeContainer ServerNode = p2pNodes.Get (1);

  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (10.0),
                                 "MinY", DoubleValue (10.0),
                                 "DeltaX", DoubleValue (10.0),
                                 "DeltaY", DoubleValue (-10.0),
                                 "GridWidth", UintegerValue (6),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");


  mobility.Install (wifiStaNodes);

  mobility.Install (wifiApNode);

  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel> ();
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (400); // set default loss to 200 dB (no link)
  for (size_t i = 0; i < nWifi; ++i) {
    lossModel->SetLoss (wifiStaNodes.Get (i)->GetObject<MobilityModel>(), p2pNodes.Get (0)->GetObject<MobilityModel>(), 0);
  }
  wifiChannel->SetPropagationLossModel (lossModel);
  wifiChannel->SetPropagationDelayModel (CreateObject <ConstantSpeedPropagationDelayModel> ());
  phy.SetChannel (wifiChannel);
  //phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);

  WifiHelper wifi;
  wifi.SetRemoteStationManager (rate);
  wifi.SetStandard(WIFI_PHY_STANDARD_80211g);

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
  Ipv4InterfaceContainer apInterface;
  apInterface = address.Assign (apDevices);
  Ipv4InterfaceContainer staInterfaces;
  staInterfaces = address.Assign (staDevices);

  /* Install Downlink Traffic */
  for(uint32_t i = 0; i< nWifi; i++)
  {
    BulkSendHelper source ("ns3::TcpSocketFactory",
                          InetSocketAddress(staInterfaces.GetAddress (i), 5555));
    source.SetAttribute ("MaxBytes", UintegerValue (0));
    ApplicationContainer sourceApps = source.Install (p2pNodes.Get(1));
    sourceApps.Start(Seconds (0.0));
    sourceApps.Stop(Seconds (simulationTime));
  }
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny (), 5555));
  sinkApps = sink.Install (wifiStaNodes);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (simulationTime));

  /* Install TCP Receiver on the access point - Uplink Traffic*/
 PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
 ApplicationContainer sinkApp = sinkHelper.Install (p2pNodes.Get(0));
 apSink = StaticCast<PacketSink> (sinkApp.Get (0));

 /* Install TCP/UDP Transmitter on the station - Uplink Traffic*/
 OnOffHelper server ("ns3::TcpSocketFactory", (InetSocketAddress (apInterface.GetAddress (0), 8080)));
 server.SetAttribute ("PacketSize", UintegerValue (1472));
 server.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
 server.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
 server.SetAttribute ("DataRate", DataRateValue (DataRate ("10Kbps")));

 for (uint32_t i = 0; i< nWifi; i++ )
 {
   ApplicationContainer serverApp = server.Install (wifiStaNodes.Get (i));
   serverApp.Start (Seconds(1.0));
 }

 /* Start Applications */
 sinkApp.Start (Seconds (0.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Schedule (MilliSeconds(100), CalculateThroughput);

  Simulator::Stop (Seconds (simulationTime));
  //phy.EnablePcapAll(rate);
  Simulator::Run ();
  Simulator::Destroy ();

  for(uint32_t i = 0; i< nWifi; i++)
  {
    staSink = StaticCast<PacketSink>(sinkApps.Get(i));
    std::cout << rate <<" Node "<<i+1<<std::endl;
    std::cout << "Total Bytes Received: " << staSink->GetTotalRx() << std::endl;
    std::cout << "Average Throughput: " << ((staSink->GetTotalRx() * 8) / (1e6  * simulationTime)) << " Mbit/s" << std::endl;
  }
  return 0;
}

int
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t index = 0;
  
  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);
  cmd.AddValue ("index", "index of the RAAs", index);
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
                   "ns3::AparfWifiManager",
                   "ns3::OnoeWifiManager",
                   "ns3::RraaWifiManager"};
    file.open ("./raa-eval-plots/" + raas[index] + " Average Throughput.txt", std::fstream::out);
    experiment(raas[index]);

    file.close();
  return 0;
}
