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
 * Authors: S Giritheja  <s.giritheja@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

// Define an object that collects statistics for RAA eval suite.

#ifndef STATS_RAA_H
#define STATS_RAA_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"

#include "ns3/applications-module.h"

namespace ns3 {

/**
 * \brief Calculates metrics and outputs it to files.
 *
 *  When installed on a node, it computes throughput, accumulated throughput
 *  and packet loss ratio.
 */
class EvalStats : public Object
{
public:

  /**
   * \brief Constructor
   *
   * \param stats FlowStats used in the current simulation.
   * \param in_simTime simulation time used in the current simulation.
   * \param filename The file to which the computed values are stored.
   */
  EvalStats (std::map<FlowId, FlowMonitor::FlowStats> stats, size_t in_simTime, std::string fileName);

  /**
   * \brief Destructor
   */
  ~EvalStats ();

  /**
   * \brief Calculates metrics for each flow.
   *
   * Computes metrics such as throughput, accumalted throughput and packet loss ratio
   * accross each flow and stores the output in a file.
   * 
   *\param stat FlowStats of a flow used in the current simulation.
   */
  void ComputeMetrics (std::map<FlowId, FlowMonitor::FlowStats> stat);

  /**
   * \brief inserts throughput, accumalted throughput and packet loss ratio into the file.
   *
   * It is called for each flow and outputs the stats into a file.
   *
   */
  void InsertIntoFile ();

  /**
    * \brief Iterates through each flow.
    *
    * Obtains the flow monitor on each flow.
    *
    * \param stats FlowStats used in the current simulation.
    *
    */
  void Install (std::map<FlowId, FlowMonitor::FlowStats> stats);

private:
  size_t                      m_simTime;
  uint64_t                    m_txBytes;
  uint64_t                    m_rxBytes;
  uint32_t                    m_txPackets;
  uint32_t                    m_rxPackets;
  uint32_t                    m_lostPackets;
  double                      m_pktLostRatio;
  double                      m_throughput;
  double                      m_accumulatedThroughput;  //!< The value of accumulated throughput across all flows.
  std::string                 m_evalStatsFileName;	//!< Name of file where the output is stored
  std::ofstream               m_evalStatsFile;		//!< The file for storing the output

};

}

#endif /*STATS_RAA_H*/