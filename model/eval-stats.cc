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
            Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

// Implement an object that collects statistics for RAA eval suite.

#include "eval-stats.h"

namespace ns3 {

EvalStats::EvalStats (std::map<FlowId, FlowMonitor::FlowStats> stats, size_t in_simTime, std::string fileName)
{
  m_simTime = in_simTime;
  m_evalStatsFileName.assign (fileName);
  m_accumulatedThroughput = 0;
  Install(stats);
}

EvalStats::~EvalStats ()
{
  m_evalStatsFile.close ();
}

// Computes throughput, accumalted throughput and packet loss ratio.
void
EvalStats::ComputeMetrics (std::map<FlowId, FlowMonitor::FlowStats> stat)
{
  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (stat->first);
  //this->flow = i->first<< " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
  m_txBytes = stat->second.txBytes;
  m_rxBytes = stat->second.rxBytes;
  m_txPackets = stat->second.txPackets;
  m_rxPackets = stat->second.rxPackets;
  m_lostPackets = stat->second.lostPackets;
  m_pktLostRatio = ((double)stat->second.txPackets-(double)stat->second.rxPackets)/(double)stat->second.txPackets;
  m_throughput = stat->second.rxBytes * 8.0 / m_simTime / 1024 / 1024; //Conversion to Mbps
  m_accumulatedThroughput+=(stat->second.rxBytes*8.0/m_simTime/1024/1024);
  InsertIntoFile();
}

// Inserts the values computed in the ComputeMetrics into the file
void
EvalStats::InsertIntoFile ()
{
  m_evalStatsFile.open (m_evalStatsFileName.c_str (), std::ios::app);
  m_evalStatsFile << "  Tx Bytes:   " << m_txBytes << "\n";
  m_evalStatsFile << "  Rx Bytes:   " << m_rxBytes << "\n";
  m_evalStatsFile << "  Tx Packets: " << m_txPackets << "\n";
  m_evalStatsFile << "  Rx Packets: " << m_rxPackets << "\n";
  m_evalStatsFile << "  Lost Packets: " << m_lostPackets << "\n";
  m_evalStatsFile << "  Pkt Lost Ratio: " << m_pktLostRatio << "\n";
  m_evalStatsFile << "  Throughput: " << m_throughput << " Mbps\n";
  m_evalStatsFile << " Accumulated Throughput: " << m_accumulatedThroughput << "Mbps\n";  

  m_evalStatsFile << std::endl;
}


// It takes flowmonitor stats as input and computes metrics for each flow.
void
EvalStats::Install (std::map<FlowId, FlowMonitor::FlowStats> stats)
{
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i=stats.begin();
        i!=stats.end(); ++i)
  {
    ComputeMetrics(i);
  }
}