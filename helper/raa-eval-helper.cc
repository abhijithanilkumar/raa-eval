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

#include "raa-eval-helper.h"

namespace ns3 {

CCIScenarioHelper::CCIScenarioHelper ()
{
  m_RAA = {"ns3::ConstantRateWifiManager",
           "ns3::ArfWifiManager",
           "ns3::AarfWifiManager",
           "ns3::AarfcdWifiManager",
           "ns3::CaraWifiManager",
           "ns3::IdealWifiManager",
           "ns3::MinstrelWifiManager",
           "ns3::ParfWifiManager",
           "ns3::AparfWifiManager",
           "ns3::OnoeWifiManager",
           "ns3::RraaWifiManager"};
  m_nRAA = 3;
}

CCIScenarioHelper::~CCIScenarioHelper ()
{
}

void
CCIScenarioHelper::addRAA (std::string raa)
{
  m_RAA.push_back (raa);
  m_nRAA++;
}

void
CCIScenarioHelper::ConfigureWifiChannel (double energyDetectionThreshold, double ccaMode1Threshold, double rxGain,
            double txPowerStart, double txPowerEnd, uint32_t channelNumber)
{
  Config::SetDefault ("ns3::ConfigureTopology::EnergyDetectionThreshold", DoubleValue (energyDetectionThreshold));
  Config::SetDefault ("ns3::ConfigureTopology::CcaMode1Threshold", DoubleValue (ccaMode1Threshold));
  Config::SetDefault ("ns3::ConfigureTopology::RxGain", DoubleValue (rxGain));
  Config::SetDefault ("ns3::ConfigureTopology::TxPowerStart", DoubleValue (txPowerStart));
  Config::SetDefault ("ns3::ConfigureTopology::TxPowerEnd", DoubleValue (txPowerEnd));
  Config::SetDefault ("ns3::ConfigureTopology::ChannelNumber", UintegerValue (channelNumber));
}

void
CCIScenarioHelper::RunSimulation (Time simulationTime)
{
  for (uint32_t i = 1; i < m_nRAA; i++)
    {
      Config::SetDefault ("ns3::TrafficParameters::SimulationTime", TimeValue (simulationTime));
      CreateScenario (m_RAA[i], simulationTime);
    }
}
}
