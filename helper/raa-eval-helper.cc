/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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
  m_nRAA = 11;
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
  for (uint32_t i = 0; i < m_nRAA; i++)
    {
      Config::SetDefault ("ns3::TrafficParameters::SimulationTime", TimeValue (simulationTime));
      CreateScenario (m_RAA[i], simulationTime);
    }
}
}
