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
 * Authors: Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *          S Giritheja <s.giritheja@gmail.com>
 */

// Implement an object that collects statistics for RAA eval suite.

#include "eval-stats.h"

namespace ns3 {

EvalStats::EvalStats (size_t apNumber, size_t nodeNumber, std::string fileName)
{
  this->m_apNumber = apNumber;
  this->m_nodeNumber = nodeNumber;
  m_evalStatsFileName.assign (fileName);
  m_accumulatedThroughput = 0;
}

EvalStats::~EvalStats ()
{
  m_evalStatsFile.close ();
}

// Computes throughput, accumalted throughput and packet loss ratio.
void
EvalStats::ComputeMetrics ()
{
  // 8. Install FlowMonitor on all nodes
  //Simulator::Schedule (Seconds (1.1), &CalculateThroughput);

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  // 9. Run simulation
  Simulator::Stop (Seconds (m_simTime));
  AnimationInterface anim ("multiAp.xml");
  Simulator::Run ();

  // 10. Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  
  m_evalStatsFile.open (m_evalStatsFileName.c_str (), std::ios::app);
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i=stats.begin();
        i!=stats.end(); ++i)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
    m_evalStatsFile << "Flow " << i->first<< " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
    m_txBytes = i->second.txBytes;
    m_rxBytes = i->second.rxBytes;
    m_txPackets = i->second.txPackets;
    m_rxPackets = i->second.rxPackets;
    m_lostPackets = i->second.lostPackets;
    m_pktLostRatio = ((double)i->second.txPackets-(double)i->second.rxPackets)/(double)i->second.txPackets;
    m_throughput = i->second.rxBytes * 8.0 / m_simTime / 1024 / 1024; //Conversion to Mbps
    m_accumulatedThroughput+=(i->second.rxBytes*8.0/m_simTime/1024/1024);
    InsertIntoFile();
  }
  m_evalStatsFile << "apNumber=" <<m_apNumber << " nodeNumber=" << m_nodeNumber << "\n" << std::flush;
  m_evalStatsFile << "throughput=" << m_accumulatedThroughput << "\n" << std::flush;
 // std::cout << "tx=" << m_txOkCount << " RXerror=" <<m_rxErrorCount <<
 //              " Rxok=" << m_rxOkCount << "\n" << std::flush;
  m_evalStatsFile << "===========================\n" << std::flush;
  // 11. Cleanup
  Simulator::Destroy ();
}

// Inserts the values computed in the ComputeMetrics into the file
void
EvalStats::InsertIntoFile ()
{
  m_evalStatsFile << "  Tx Bytes:   " << m_txBytes << "\n";
  m_evalStatsFile << "  Rx Bytes:   " << m_rxBytes << "\n";
  m_evalStatsFile << "  Tx Packets: " << m_txPackets << "\n";
  m_evalStatsFile << "  Rx Packets: " << m_rxPackets << "\n";
  m_evalStatsFile << "  Lost Packets: " << m_lostPackets << "\n";
  m_evalStatsFile << "  Pkt Lost Ratio: " << m_pktLostRatio << "\n";
  m_evalStatsFile << "  Throughput: " << m_throughput << " Mbps\n";

  m_evalStatsFile << std::endl;
}

void
EvalStats::Install (NodeContainer nodes, Ptr<TrafficParameters> traffic)
{
  Time simulationTime = traffic->GetSimulationTime ();
  this->m_simTime = simulationTime.ToInteger (Time::S);
  this->m_nodes = nodes;
  ComputeMetrics();
}
}