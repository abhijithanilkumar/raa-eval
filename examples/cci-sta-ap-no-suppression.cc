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

 /* This example creates a scenario that has no Station-AP Link
  * suppression in a condition where CCI in involved.
  */

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/raa-eval-module.h"

using namespace ns3;

class StaApSuppression : public CCIScenarioHelper
{
public:
  StaApSuppression ();
  ~StaApSuppression ();

protected:
  virtual void CreateScenario (std::string raa, Time simulationTime);
};

StaApSuppression::StaApSuppression ()
{
}

StaApSuppression::~StaApSuppression ()
{
}

void
StaApSuppression::CreateScenario (std::string raa, Time simulationTime)
{
  bool uplinkDownlink = true;
  bool useRtsCts = true;
  size_t apNumber = 2;
  size_t staNumber = 2;
  std::string fileName = "StaApNoSuppression/raa-eval-cci-sta-ap-no-suppression-" + raa.substr (5);
  std::cout << "Using "+raa.substr (5) << "\n";

  // Set traffic parameters
  Config::SetDefault ("ns3::TrafficParameters::UplinkDownlink", BooleanValue (uplinkDownlink));
  Config::SetDefault ("ns3::TrafficParameters::UseRtsCts", BooleanValue (useRtsCts));
  Config::SetDefault ("ns3::TrafficParameters::SimulationTime", TimeValue (simulationTime));

  // Create the scenario and run it
  Ptr<TrafficParameters> trafficParams = CreateObject <TrafficParameters> ();
  Ptr<HiddenStationTopology> hiddenStation = CreateObject <HiddenStationTopology> ();
  hiddenStation->CreateHiddenStationTopology (trafficParams, apNumber, staNumber, 2, raa, fileName);
}

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  StaApSuppression sce;
  sce.ConfigureWifiChannel (-95.0, -95.0, -25.0, 23.0, 23.0, 1);
  sce.RunSimulation (Seconds (50));
}
