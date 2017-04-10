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

// This example is a part of RAA evaluation suite and
// creates a hidden station scenario.

#include "ns3/core-module.h"
#include "ns3/configure-topology.h"
#include "ns3/traffic-parameters.h"
#include "ns3/hidden-station-topology.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RaaEvalHiddenStationExample");

int
main (int argc, char *argv[])
{
  // Set default values for topology
  double energyDetectionThreshold = -95.0;
  double ccaMode1Threshold = -95.0;
  double txPowerStart = 23.0;
  double txPowerEnd = 23.0;
  double rxGain = -25.0;
  uint32_t channelNumber = 1;


  // Set default values for traffic
  bool uplinkDownlink = true;
  bool useRtsCts = true;
  double simTime = 50;
  Time simulationTime;

  // Default filename to store results
  std::string fileName = "RaaEvalHiddenStation";

  // Allow the user to change values by command line arguments
  CommandLine cmd;
  cmd.AddValue ("energyDetectionThreshold", "Enegy Detection Threshold in dbm", energyDetectionThreshold);
  cmd.AddValue ("ccaMode1Threshold", "CcaMode1Threshold in dbm", ccaMode1Threshold);
  cmd.AddValue ("txPowerStart", "Minimum Available Transmission Level in dbm", txPowerStart);
  cmd.AddValue ("txPowerEnd", "Maximum Available Transmission Level in dbm", txPowerEnd);
  cmd.AddValue ("rxGain", "Reception Gain in dB", rxGain);
  cmd.AddValue ("channelNumber", "Channel Number to control Frequency and ChannelWidth", channelNumber);
  cmd.AddValue ("uplinkDownlink", "Enable or Disable both Uplink and Downlink", uplinkDownlink);
  cmd.AddValue ("useRtsCts", "Enable or Disable RTS-CTS in Stations", useRtsCts);
  cmd.AddValue ("simulationTime", "Total simulation time in seconds", simTime);
  cmd.AddValue ("fileName", "File to store the results", fileName);
  cmd.Parse (argc, argv);

  // Convert time from double to seconds
  simulationTime = Time::FromDouble (simTime, Time::S);

  // Set topology parameters
  Config::SetDefault ("ns3::ConfigureTopology::EnergyDetectionThreshold", DoubleValue (energyDetectionThreshold));
  Config::SetDefault ("ns3::ConfigureTopology::CcaMode1Threshold", DoubleValue (ccaMode1Threshold));
  Config::SetDefault ("ns3::ConfigureTopology::TxPowerStart", DoubleValue (txPowerStart));
  Config::SetDefault ("ns3::ConfigureTopology::TxPowerEnd", DoubleValue (txPowerEnd));
  Config::SetDefault ("ns3::ConfigureTopology::RxGain", DoubleValue (rxGain));
  Config::SetDefault ("ns3::ConfigureTopology::ChannelNumber", UintegerValue (channelNumber));

  // Set traffic parameters
  Config::SetDefault ("ns3::TrafficParameters::UplinkDownlink", BooleanValue (uplinkDownlink));
  Config::SetDefault ("ns3::TrafficParameters::UseRtsCts", BooleanValue (useRtsCts));
  Config::SetDefault ("ns3::TrafficParameters::SimulationTime", TimeValue (simulationTime));


  Ptr<TrafficParameters> trafficParams = CreateObject <TrafficParameters> ();
  Ptr<HiddenStationTopology> hiddenStation = CreateObject <HiddenStationTopology> ();
  //hiddenStation->CreateHiddenStationTopology (trafficParams, fileName);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
