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

// Implement an object to set traffic parameters in raa-eval.

#include <iostream>

#include "traffic-parameters.h"
#include "ns3/log.h"
#include "ns3/core-module.h"

namespace ns3{

NS_LOG_COMPONENT_DEFINE ("TrafficParameters");

NS_OBJECT_ENSURE_REGISTERED (TrafficParameters);

TypeId
TrafficParameters::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TrafficParameters")
    .setParent<Object> ()
    .setGroupName ("RaaEvaluationSuite")
    .AddAttribute ("UseRtsCts", "Enable or Disable RTS/CTS in stations",
                   BooleanValue (false),
                   MakeBooleanAccessor (&TrafficParameters::m_useRtsCts),
                   MakeBooleanChecker ())
    .AddAttribute ("SimulationTime", "Total Simulation Time in seconds",
                   TimeValue (Seconds (10)),
                   MakeTimeAccessor (&TrafficParameters::m_simulationTime),
                   MakeTimeChecker ())
    .AddAttribute ("UplinkDownlink", "Use both Uplink and Downlink",
                   BooleanValue (false),
                   MakeBooleanAccessor (&TrafficParameters::m_uplinkDownlink)
                   MakeBooleanChecker ())
  ;
  return tid;
}

TrafficParameters::TrafficParameters (void)
{
}

TrafficParameters::~TrafficParameters (void)
{
}

void
TrafficParameters::SetRtsCtsUsed (bool useRtsCts)
{
  m_useRtsCts = useRtsCts;
}

bool
TrafficParameters::IsRtsCtsUsed (void) const
{
  return m_useRtsCts;
}

void
TrafficParameters::SetUplinkDownlink (bool useUplinkDownlink)
{
  m_uplinkDownlink = useUplinkDownlink;
}

bool
TrafficParameters::IsUplinkDownlink (void) const
{
  return m_uplinkDownlink;
}

void
TrafficParameters::SetSimulationTime (Time simulationTime)
{
  m_simulationTime = simulationTime;

}
Time
TrafficParameters::GetSimulationTime (void) const
{
  return m_simulationTime;
}

}
