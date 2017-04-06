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

//Implement an object to set traffic parameters in raa-eval

#ifdef TRAFFIC_PARAMETERS_H
#define TRAFFIC_PARAMETERS_H

#include <stdint.h>
#include "ns3/object.h"
#include "ns3/core-module.h"

namespace ns3{

/**
 * \brief Initializes the traffic parameters
 *
 * This class consists of setters and getters for traffic parameters
 * The object of this class is passed to the methods whrere traffic
 * parameters are used.
 */
class TrafficParameters : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return TypeID of the object
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  TrafficParameters (void);

  /**
   * \brief Destructor
   */
  ~TrafficParameters (void);

  /**
   * \brief Set whether RTS-CTS is to be used
   *
   * \param useRtsCts True if RTS-CTS is enabled
   */
  void SetRtsCtsUsed (bool useRtsCts);

  /**
   * \brief Check whether RTS-CTS is used
   *
   * \return True if RTS-CTS is enabled
   */
  bool IsRtsCtsUsed (void) const;

  /**
   * \brief Set whether both Uplink and Downlink are active
   *
   * \param useRtsCts True if both Uplink and Downlink are active
   */
  void SetUplinkDownlink (bool useUplinkDownlink);

  /**
   * \brief Check whether both Uplink and Downlink are active
   *
   * \return True if both Uplink and Downlink are active
   */
  bool IsUplinkDownlink (void) const;

  /**
   * \brief Set the simulation time in seconds
   *
   * \param simulationTime Time when the simulation should stop
   */
  void SetSimulationTime (Time simulationTime);

  /**
   * \brief Get the simulation time in seconds
   *
   * \return The simulation time in seconds
   */
  Time GetSimulationTime (void) const;

private:
  bool m_useRtsCts;
  Time m_simulationTime;
  bool m_uplinkDownlink;
};

}

#endif /* TRAFFIC_PARAMETERS_H */
