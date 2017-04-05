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
 */

// Implement an object to create a hidden station topology in raa-eval.

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "hidden-station-topology.h"
#include "eval-stats.h"
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
    .setParent<ConfigureTopology> ()
    .setGroupName ("RaaEvaluationSuite")
  ;
  return tid;
}

void
CreateHiddenStationTopology (Ptr<TrafficParameters> traffic, std::string fileName)
{
  //Set Topology Parameters
  SetTopologyParameters (traffic);

  /*m_apNumber = 2;
  m_nodes = 2;
  m_radius = 60;*/

  //Create Nodes
  NodeContainer apNodes, staNodes;
  apNodes.Create (2);
  staNodes.Create (2);

  //Set RTS-CTS Threshold
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", traffic->GetRtsCtsThreshold ());

  //Create Mobility Model
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> apPosAlloc = CreateObject<ListPositionAllocator> ();
  Ptr<ListPositionAllocator> staPosAlloc = CreateObject<ListPositionAllocator> ();
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  apPosAlloc->Add (Vector (0, 0, 1));
  apPosAlloc->Add (Vector (160, 0, 1));
  mobility.SetPositionAllocator (apPosAlloc);
  mobility.Install (apNodes);
  staPosAlloc->Add (80, 0, 1);
  staPosAlloc->Add (80, 0, 1);
  mobility.SetPositionAllocator (staPosAlloc);
  mobility.Install (staNodes);

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
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));
  NetDeviceContainer staDevices;
  staDevices = wifi.Install (wifiPhy, wifiMac, staNodes);
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));
  NetDeviceContainer apDevices;
  apDevices = wifi.Install (wifiPhy, wifiMac, apNodes);

  //Assign IP Addresses
  InternetStackHelper stack;
  stack.Install (staNodes);
  stack.Install (apNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.0.0.0");
  address.assign (staDevices);
  address.assign (apDevices);


}
}
