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

#ifndef HIDDEN_STATION_TOPOLOGY_H
#define HIDDEN_STATION_TOPOLOGY_H

#include "configure-topology.h"
#include "traffic-parameters.h"
#include "create-traffic.h"

namespace ns3 {

/*
 * \brief Configures hidden station topology and simulates the traffic accordingly.
 */
class HiddenStationTopology : public ConfigureTopology
{
public:
  /**
   * \brief Get the type ID
   * \return Object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  HiddenStationTopology (void);

  /**
   * \brief Destructor
   */
  ~HiddenStationTopology (void);

  /**
   * \brief Invokes methods for creating a hidden station topology and simulating traffic
   *
   * It configures hidden station topology with 1 AP and 2 nodes , invokes topology creation
   * class and then calls its methods to create traffic in this topology. Finally, this method
   * invokes Stats class to trace the required statistics.
   *
   * \param traffic Object of TrafficParameters class that contains the information of
   *                          traffic related parameters.
   * \param fileName The name of the file whtere stats are to be dumped.
   */
  void CreateHiddenStationTopology (Ptr <TrafficParameters> traffic, std::string fileName);

};

}

#endif /* HIDDEN_STATION_TOPOLOGY_H */
