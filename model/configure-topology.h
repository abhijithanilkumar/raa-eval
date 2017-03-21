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
  void SetTopologyParameters (Ptr<TrafficParameters> traffic);

  /**
   * \brief Set the RTS-CTS Threshold
   *
   * \param rtsCtsThreshold the RTS-CTS Threshold for the topology
   */
  void SetRtsCtsThreshold (uint32_t rtsCtsThreshold);

  /**
   * \brief Get the RTS-CTS Threshold for the topology
   *
   * \return the RTS-CTS Threshold for the topology
   */
  uint32_t GetRtsCtsThreshold (void) const;

  /**
   * \brief Set the EnergyDetectionThreshold of the Wifi channel
   *
   * \param energyDetectionThreshold the Energy Detection Threshold of the Wifi channel
   */
  void SetEnergyDetectionThreshold (double energyDetectionThreshold);

  /**
   * \brief Get the Energy Detection Threshold  of the Wifi channel
   *
   * \return the Energy Detection Threshold of the Wifi channel
   */
  double GetEnergyDetectionThreshold (void) const;

  /**
   * \brief Set the Cca Mode1 Threshold of the Wifi channel
   *
   * \param ccaMode1Threshold the Cca Mode1 Threshold of the Wifi channel
   */
  void SetCcaMode1Threshold (double ccaMode1Threshold);

  /**
   * \brief Get the Cca Mode1 Threshold of the Wifi channel
   *
   * \return the Cca Mode1 Threshold of the Wifi channel
   */
  double GetCcaMode1Threshold (void) const;

  /**
   * \brief Set the minimum available transmission level
   *
   * \param txPowerStart the minimum available transmission level
   */
  void SetTxPowerStart (double txPowerStart);

  /**
   * \brief Get the minimum available transmission level
   *
   * \return the minimum available transmission level
   */
  double GetTxPowerStart (void) const;

  /**
   * \brief Set the maximum available transmission level
   *
   * \param txPowerEnd the maximum available transmission level
   */
  void SetTxPowerEnd (double txPowerEnd);

  /**
   * \brief Get the maximum available transmission level
   *
   * \return the maximum available transmission level
   */
  double GetTxPowerEnd (void) const;

  /**
   * \brief Set the channel number
   *
   * \param channelNumber the Wifi channel number
   */
  void SetChannelNumber (unit32_t channelNumber);

  /**
   * \brief Get the channel number used
   *
   * \return the Wifi channel number used
   */
  uint32_t GetChannelNumber (void) const;

  /**
   * \brief Set the reception gain of the Wifi channel
   *
   * \param rxGain the reception gain of the Wifi channel
   */
  void SetRxGain (double rxGain);

  /**
   * \brief Get the reception gain of the Wifi channel
   *
   * \return the reception gain of the Wifi channel
   */
  double GetRxGain (void) const;
};

protected:
  uint32_t m_rtsCtsThreshold;
  double m_energyDetectionThreshold;
  double m_ccaMode1Threshold;
  double m_txPowerStart;
  double m_TxPowerEnd;
  uint32_t m_channelNumber;
  double m_rxGain;
}

#endif /* CONFIGURE_TOPOLOGY_H */
