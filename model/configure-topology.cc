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

// Implement an object to configure topology in raa-eval.

#include "configure-topology.h"
#include "ns3/log.h"

namespace ns3{

NS_LOG_COMPONENT_DEFINE ("ConfigureTopology");

NS_OBJECT_ENSURE_REGISTERED (ConfigureTopology);

TypeId
ConfigureTopology::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConfigureTopology")
    .SetParent<Object> ()
    .SetGroupName ("RaaEvaluationSuite")
    .AddAttribute ("EnergyDetectionThreshold",
                   "Enegy Detection Threshold in dbm",
                   DoubleValue(-95.0),
                   MakeDoubleAccessor (&ConfigureTopology::m_energyDetectionThreshold),
                   MakeDoubleChecker<double> (0))
    .AddAttribute ("CcaMode1Threshold",
                   "CcaMode1Threshold in dbm",
                   DoubleValue(-95.0),
                   MakeDoubleAccessor (&ConfigureTopology::m_ccaMode1Threshold),
                   MakeDoubleChecker<double> (0))
    .AddAttribute ("TxPowerStart",
                   "Minimum Available Transmission Level in dbm",
                   DoubleValue(23.0),
                   MakeDoubleAccessor (&ConfigureTopology::m_txPowerStart),
                   MakeDoubleChecker<double> (0))
    .AddAttribute ("TxPowerEnd",
                   "Maximum Available Transmission Level in dbm",
                   DoubleValue(23.0),
                   MakeDoubleAccessor (&ConfigureTopology::m_txPowerEnd),
                   MakeDoubleChecker<double> (0))
    .AddAttribute ("ChannelNumber",
                   "Channel Number to control Frequency and ChannelWidth",
                   UintegerValue(1),
                   MakeUintegerAccessor (&ConfigureTopology::m_channelNumber),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RxGain",
                   "Reception Gain in dB",
                   DoubleValue(-25.0),
                   MakeDoubleAccessor (&ConfigureTopology::m_rxGain),
                   MakeDoubleChecker<double> (0))
  ;
  return tid;
}

ConfigureTopology::ConfigureTopology (void)
{
}

ConfigureTopology::~ConfigureTopology (void)
{
}

/*void
ConfigureTopology::SetTopologyParameters (Ptr<TrafficParameters> traffic)
{
  useRtsCts = traffc->IsRtsCtsUsed ();
  m_rtsCtsThreshold = (useRtsCts ? UintegerValue (10) : UintegerValue (22000));
}*/

void
ConfigureTopology::SetRtsCtsThreshold (uint32_t rtsCtsThreshold)
{
  m_rtsCtsThreshold = rtsCtsThreshold;
}

uint32_t
ConfigureTopology::GetRtsCtsThreshold (void) const
{
  return m_rtsCtsThreshold;
}

void
ConfigureTopology::SetEnergyDetectionThreshold (double energyDetectionThreshold)
{
  m_energyDetectionThreshold = energyDetectionThreshold;
}

double
ConfigureTopology::GetEnergyDetectionThreshold (void) const
{
  return m_energyDetectionThreshold;
}

void
ConfigureTopology::SetCcaMode1Threshold (double ccaMode1Threshold)
{
  m_ccaMode1Threshold = ccaMode1Threshold;
}

double
ConfigureTopology::GetCcaMode1Threshold (void) const
{
  return m_ccaMode1Threshold;
}

void
ConfigureTopology::SetTxPowerStart (double txPowerStart)
{
  m_txPowerStart = txPowerStart;
}

double
ConfigureTopology::GetTxPowerStart (void) const
{
  return m_txPowerStart;
}

void
ConfigureTopology::SetTxPowerEnd (double txPowerEnd)
{
  m_txPowerEnd = txPowerEnd;
}

double
ConfigureTopology::GetTxPowerEnd (void) const
{
  return m_txPowerEnd;
}

void
ConfigureTopology::SetChannelNumber (uint32_t channelNumber)
{
  m_channelNumber = channelNumber;
}

uint32_t
ConfigureTopology::GetChannelNumber (void) const
{
  return m_channelNumber;
}

void
ConfigureTopology::SetRxGain (double rxGain)
{
  m_rxGain = rxGain;
}

double
ConfigureTopology::GetRxGain (void) const
{
  return m_rxGain;
}

}
