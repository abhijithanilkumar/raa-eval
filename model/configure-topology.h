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

// Define an object to configure the topology in raa-eval.

#ifndef CONFIGURE_TOPOLOGY_H
#define CONFIGURE_TOPOLOGY_H

#include "ns3/object.h"

namespace ns3 {

/**
 * \brief Base class to configure topology
 *
 * It initializes parameters used in the congfiguration of the topology.
 * Other classes should extend this class.
 */
class ConfigureTopology : public Object
{
public:

  /**
   * \brief Get the type ID.
   * \return the object TypeID
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  ConfigureTopology (void);

  /**
   * \brief Destructor
   */
  ~ConfigureTopology (void);

  /**
   * \brief Initializes the topology parameters
   *
   * \param traffic Object of TrafficParameters class that contains the traffic information
   */
  void SetTopologyParameters(Ptr<TrafficParameters> traffic);
}

}
