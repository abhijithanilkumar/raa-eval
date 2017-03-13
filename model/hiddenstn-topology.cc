/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 NITK Surathkal
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

#include <iostream>
#include <cstdlib>
#include <string>

#include "hiddenstn-topology.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("3 Node Hidden Station Topology");

NS_OBJECT_ENSURE_REGISTERED (HiddenstnTopology);

TypeId
HiddenstnTopology::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HiddenstnTopology")
    .setParent<ConfigureTopology> ()
    .setGroupName ("RaaEvaluationSuite")
  ;
  return tid;
}

}
