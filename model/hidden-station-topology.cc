/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 NITK Surathkal
 *
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
 *
 * Authors: Abhijith Anilkumar <abhijithabhayam@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

// Implement an object to create a hidden station topology in raa-eval.

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "hidden-station-topology.h"
#include "ns3/core-module.h"
#include "ns3/propagation-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/wifi-module.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("HiddenStationTopology");

NS_OBJECT_ENSURE_REGISTERED (HiddenStationTopology);

TypeId
HiddenStationTopology::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HiddenStationTopology")
    .SetParent<ConfigureTopology> ()
    .SetGroupName ("RaaEvaluationSuite")
  ;
  return tid;
}

void
CreateHiddenStationTopology (Ptr<TrafficParameters> traffic, std::string fileName)
{
  //Set Topology Parameters
  SetTopologyParameters (traffic);

  size_t apNumber = 2;
  size_t staNumber = 2;
  /*m_radius = 60;*/

  //Create Nodes
  NodeContainer nodes;
  nodes.Create (apNumber+staNumber);

  //Set RTS-CTS Threshold
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", GetRtsCtsThreshold ());

  //Create Mobility Model
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> apPosAlloc = CreateObject<ListPositionAllocator> ();
  Ptr<ListPositionAllocator> staPosAlloc = CreateObject<ListPositionAllocator> ();
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  for (size_t i=0; i<apNumber; ++i)
  {
    apPosAlloc->Add (Vector ((160*i), 0, 1));
  }
  mobility.SetPositionAllocator (apPosAlloc);
  for (size_t i=0; i<apNumber; ++i)
  {
    mobility.Install (nodes.Get (i));
  }
  for (size_t i=0; i<staNumber; ++i)
  {
    staPosAlloc->Add (Vector (80, 0, 1));
  }
  mobility.SetPositionAllocator (staPosAlloc);
  for (size_t i=0; i<staNumber; ++i)
  {
    mobility.Install (nodes.Get (apNumber+i));
  }

  //Install Wifi Channel, Wifi Physical Layer and Mac Layer
  WifiHelper wifi;
  YansWifiChannelHelper wifiChannel;
  YansWifiPhyHelper wifiPhy;
  Ssid ssid = Ssid ("ns-3-ssid");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                              "DataMode", StringValue ("DsssRate11Mbps"),
                              "ControlMode", StringValue ("DsssRate5_5Mbps"));
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::TwoRayGroundPropagationLossModel",
                                  "Frequency", DoubleValue(2.400e9));
  wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create());
  wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue (m_energyDetectionThreshold));
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (m_ccaMode1Threshold));
  wifiPhy.Set ("TxPowerStart", DoubleValue (m_txPowerStart));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (m_txPowerEnd));
  wifiPhy.Set ("ChannelNumber", UintegerValue (m_channelNumber));
  wifiPhy.Set ("RxGain", DoubleValue (m_rxGain));

  //Install the Wifi Devices
  NqosWifiMacHelper wifiMac;
  NetDeviceContainer devices;
  wifiMac.SetType ("ns3::AdhocWifiMac"); // use ad-hoc MAC
  devices = wifi.Install (wifiPhy, wifiMac, nodes);

  //Assign IP Addresses
  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.0.0.0");
  address.Assign (devices);


  //Install Application
  uint16_t cbrPort = 12345;
  uint16_t  echoPort = 9;
  ApplicationContainer cbrApps;
  ApplicationContainer pingApps;
  ApplicationContainer sinkApps;
  for(size_t j=1; j<=apNumber; ++j)
  {
    for(size_t i=apNumber+staNumber/apNumber*(j-1);
        i<apNumber+staNumber/apNumber*j ; ++i)
    {
      std::string s;
      std::stringstream ss(s);
      if(traffic->IsUplinkDownlink ())
      {
         ss << i+1;
      }else
      {
        ss << j;
      }
      s = "10.0.0."+ss.str();
      OnOffHelper onOffHelper ("ns3::TcpSocketFactory",
               InetSocketAddress (Ipv4Address (s.c_str()), cbrPort));
      onOffHelper.SetAttribute ("PacketSize", UintegerValue (1024));
      std::string s2;
      std::stringstream ss2(s2);

      PacketSinkHelper sink ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address(s.c_str()),  cbrPort));
      sinkApps = sink.Install (nodes);
      sinkApps.Start (Seconds (0.00));
      sinkApps.Stop (Seconds (traffic->GetSimulationTime ()));

      if(traffic->IsUplinkDownlink ())
      {
        ss2 << 5500000+i*100;
      }else
      {
        ss2 << 5500000+i*100;
      }
      s2 = ss2.str() + "bps";
      onOffHelper.SetAttribute ("DataRate", StringValue (s2));
      if(traffic->IsUplinkDownlink ())
      {
        onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.00+static_cast<double>(i)/100)));
        onOffHelper.SetAttribute ("StopTime", TimeValue (Seconds (50.000+static_cast<double>(i)/100)));
        cbrApps.Add (onOffHelper.Install (nodes.Get (j-1)));
      }
      else
      {
        onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.00)));
        onOffHelper.SetAttribute ("StopTime", TimeValue (Seconds (50.000+static_cast<double>(j)/100)));
        cbrApps.Add (onOffHelper.Install (nodes.Get (i)));
      }
    }
  }

  //again using different start times to workaround Bug 388 and Bug 912
  for(size_t j=1; j<=apNumber; ++j)
  {
    for(size_t i=apNumber+staNumber/apNumber*(j-1);
        i<apNumber+staNumber/apNumber*j ; ++i){
      std::string s;
      std::stringstream ss(s);
      if(traffic->IsUplinkDownlink ())
      {
         ss << i+1;
      }
      else
      {
        ss << j;
      }
      s = "10.0.0."+ss.str();
      UdpEchoClientHelper echoClientHelper (Ipv4Address (s.c_str()), echoPort);
      echoClientHelper.SetAttribute ("MaxPackets", UintegerValue (1));
      echoClientHelper.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
      echoClientHelper.SetAttribute ("PacketSize", UintegerValue (10));
      if(traffic->IsUplinkDownlink ())
      {
        echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.001)));
        echoClientHelper.SetAttribute ("StopTime", TimeValue (Seconds (50.001)));
        pingApps.Add (echoClientHelper.Install (nodes.Get (j-1)));
      }
      else
      {
        echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.001)));
        echoClientHelper.SetAttribute ("StopTime", TimeValue (Seconds (50.001)));
        pingApps.Add (echoClientHelper.Install (nodes.Get (i)));
      }
    }
  }

  Simulator::Stop (Time::FromDouble (((traffic->GetSimulationTime ()).ToDouble (Time::S) + 5), Time::S));
  Simulator::Run ();
  Simulator::Destroy ();
}
}
