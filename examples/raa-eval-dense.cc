/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/raa-eval-helper.h"
#include "ns3/propagation-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include <string.h>
using namespace ns3;
using namespace std;

Ptr<PacketSink> sink;                         /* Pointer to the packet sink application */
uint64_t lastTotalRx = 0;                     /* The value of the last total received bytes */

void
CalculateThroughput ()
{
  Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
  double cur = (sink->GetTotalRx() - lastTotalRx) * (double) 8/1e5;     /* Convert Application RX Packets to MBits. */
  std::cout << now.GetSeconds () << "s: \t" << cur << " Mbit/s" << std::endl;
  lastTotalRx = sink->GetTotalRx ();
  Simulator::Schedule (MilliSeconds (100), &CalculateThroughput);
}

void
experiment(std::string rate)
{
  WifiHelper wifi;
  NodeContainer staNodes;
  NodeContainer ap;
  NetDeviceContainer staDevices;
  NetDeviceContainer apDevice;

  ap.Create(1);
  staNodes.Create(30);

  //Place Nodes randomly
  ap.Get (0)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
  for (size_t i = 0; i < 30; ++i)
    staNodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());

  //Limit distanc b/w the nodes using propagation loss.
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (200); // set default loss to 200 dB (no link)
  for (size_t i=0; i<30; ++i)
    lossModel->SetLoss (staNodes.Get (i)->GetObject<MobilityModel>(), ap.Get (0)->GetObject<MobilityModel>(), 50); // set symmetric loss 0 <-> 1 to 50 dB

  Ptr<YansWifiChannel>wifiChannel = CreateObject <YansWifiChannel> ();
  wifiChannel->SetPropagationLossModel(lossModel);
  wifiChannel->SetPropagationDelayModel(CreateObject <ConstantSpeedPropagationDelayModel> ());
  WifiMacHelper wifiMac;
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
  wifiPhy.SetChannel(wifiChannel);
  Ssid ssid = Ssid("raa-test");
  wifi.SetStandard(WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager(rate);
  //setup Stations
  wifiMac.SetType("ns3::StaWifiMac",
                  "Ssid", SsidValue(ssid));
  staDevices = wifi.Install(wifiPhy, wifiMac, staNodes);
  //setup ap
  wifiMac.SetType("ns3::ApWifiMac",
                  "Ssid", SsidValue(ssid));
  apDevice = wifi.Install(wifiPhy, wifiMac, ap);

  InternetStackHelper stack;
  stack.Install(ap);
  stack.Install(staNodes);

  /* Internet Stack */
  Ipv4AddressHelper address;
  address.SetBase("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterface;
  apInterface = address.Assign(apDevice);
  Ipv4InterfaceContainer staInterface;
  staInterface = address.Assign(staDevices);

  /* Populate Routing Table */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  /* Design the flow */
  uint64_t port = 9;

  BulkSendHelper source ("ns3::TcpSocketFactory",
                        InetSocketAddress(apInterface.GetAddress(0), port));
  source.SetAttribute ("MaxBytes", UintegerValue (5120));
  ApplicationContainer sourceApps = source.Install (ap.Get(0));
  sourceApps.Start(Seconds (0.0));
  sourceApps.Stop(Seconds (10.0));

  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (staNodes);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (10.0));

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

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
