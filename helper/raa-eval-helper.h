/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef RAA_EVAL_HELPER_H
#define RAA_EVAL_HELPER_H

#include "ns3/raa-eval.h"
#include "ns3/hidden-station-topology.h"

namespace ns3 {

/**
 * \brief Helper class for running the CCI Scenarios
 */
class CCIScenarioHelper
{
public:
  /**
   * \brief Constructor
   */
  CCIScenarioHelper ();

  /**
   * \brief Destructor
   */
  virtual ~CCIScenarioHelper ();

  /**
   * \brief Run simulation for the specified duration
   *
   * \param simtime The simulation time
   */
  void RunSimulation (Time simulationTime);

  /**
   * \brief Function that can be used to add a new RAA to the list
   *
   * \param raa The name of RAA algorithm
   */
  void addRAA (std::string raa);


  virtual void ConfigureWifiChannel (double energyDetectionThreshold, double ccaMode1Threshold, double rxGain,
              double txPowerStart, double txPowerEnd, uint32_t channelNumber);

protected:
  /**
   * \brief Create simulation scenario
   *
   * \param raa The name of RAA algorithm
   */
  virtual void CreateScenario (std::string raa, Time simulationTime) = 0;

private:
  std::vector<std::string> m_RAA;		// Array of all RAAs used
  uint32_t m_nRAA;				// Number of RAAs used
};

}

#endif /* RAA_EVAL_HELPER_H */
