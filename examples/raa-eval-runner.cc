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

// Runner file that runs the scenarios in the RAA Evaluation Suite.

#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include <sys/stat.h>

using namespace ns3;

std::vector<std::string> RAA = {"ns3::ConstantRateWifiManager",
         "ns3::ArfWifiManager",
         "ns3::AarfWifiManager",
         "ns3::AarfcdWifiManager",
         "ns3::AmrrWifiManager",
         "ns3::CaraWifiManager",
         "ns3::IdealWifiManager",
         "ns3::MinstrelWifiManager",
         "ns3::ParfWifiManager",
         "ns3::AparfWifiManager",
         //"ns3::OnoeWifiManager",
         //"ns3::RraaWifiManager",
         //"ns3::MinstrelHtWifiManager",
        };
uint32_t nRAA = 10;

void RunOneScenario (std::string scenarioName)
{
  mkdir((std::string("raa-eval-output/")+scenarioName).c_str (), 0700);
  mkdir((std::string("raa-eval-graph/")+scenarioName).c_str (), 0700);
  std::cout << "<Running Scenario : " << scenarioName << ">\n";
  std::string commandToRun = std::string ("./waf --run ") + scenarioName;
  system(commandToRun.c_str());
}

int main (int argc, char *argv[])
{
  mkdir("raa-eval-output", 0700);
  mkdir("raa-eval-graph", 0700);
  std::map<std::string, std::string> ScenarioNumberMapping;
  ScenarioNumberMapping["1.1"] = "StaApLinkSuppression";
  ScenarioNumberMapping["1.2"] = "StaApNoSuppression";
  ScenarioNumberMapping["1.3"] = "ApApInterference";
  ScenarioNumberMapping["1.4"] = "ApApNoInterference";

  std::string scenarioName = "";
  std::string scenarioNumber = "";

  CommandLine cmd;
  cmd.AddValue ("number", "Scenario number (1.1 - 1.4)", scenarioNumber);
  cmd.AddValue ("name", "Name of the scenario (eg: ApApInterference)", scenarioName);

  cmd.Parse (argc, argv);

  if(scenarioName == "" && scenarioNumber == "")
  {
    std::cout << "No value given";
    exit(-1);
  }

  if (scenarioName == "")
  {
    scenarioName = ScenarioNumberMapping[scenarioNumber];
  }

  if (scenarioName != "All")
  {
    RunOneScenario (scenarioName);
  }
  else
  {
    for (std::map<std::string, std::string>::iterator it=ScenarioNumberMapping.begin(); it!=ScenarioNumberMapping.end(); ++it)
      {
        RunOneScenario (it->second);
      }
  }
}
