/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */
#include <stdio.h>
#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "drop-tail-queue-packet.h"

NS_LOG_COMPONENT_DEFINE ("DropTailQueuePacket");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(DropTailQueuePacket);

TypeId DropTailQueuePacket::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DropTailQueuePacket")
    .SetParent<Queue> ()
    .AddConstructor<DropTailQueuePacket> ()
    .AddAttribute ("Mode",
                   "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (QUEUE_MODE_BYTES),
                   MakeEnumAccessor <QueueMode> (&DropTailQueuePacket::SetMode),
                   MakeEnumChecker (QUEUE_MODE_BYTES, "QUEUE_MODE_BYTES",
                                    QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
    .AddAttribute ("MaxPackets",
                   "The maximum number of packets accepted by this DropTailQueuePacket.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DropTailQueuePacket::m_maxPackets),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxBytes",
                   "The maximum number of bytes accepted by this DropTailQueuePacket.",
                   UintegerValue (30000 * 65535),
                   MakeUintegerAccessor (&DropTailQueuePacket::m_maxBytes),
                   MakeUintegerChecker<uint32_t> ())
  ;

  return tid;
}

DropTailQueuePacket::DropTailQueuePacket () :
  Queue (),
  m_packets (),
  m_bytesInQueue (0),
  NS_LOG_TEMPLATE_DEFINE("DropTailQueuePacket")
{
  NS_LOG_FUNCTION_NOARGS ();
}

DropTailQueuePacket::~DropTailQueuePacket ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
DropTailQueuePacket::SetMode (DropTailQueuePacket::QueueMode mode)
{
  NS_LOG_FUNCTION (mode);
  m_mode = mode;
}

DropTailQueuePacket::QueueMode
DropTailQueuePacket::GetMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mode;
}

bool
DropTailQueuePacket::Enqueue (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  //
  // If DoEnqueue fails, Queue::Drop is called by the subclass
  //
  bool retval = DoEnqueue (p);
  if (retval)
    {
      NS_LOG_LOGIC ("m_traceEnqueue (p)");
      m_traceEnqueue (p);

      uint32_t size = p->GetSize ();
      m_nBytes += size;
      m_nTotalReceivedBytes += size;

      m_nPackets++;
      m_nTotalReceivedPackets++;
    }
  return retval;
}

Ptr<Packet>
DropTailQueuePacket::Dequeue (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<Packet> packet = DoDequeue ();

  if (packet)
    {
      NS_ASSERT (m_nBytes >= packet->GetSize ());
      NS_ASSERT (m_nPackets > 0);

      m_nBytes -= packet->GetSize ();
      m_nPackets--;

      NS_LOG_LOGIC ("m_traceDequeue (packet)");
      m_traceDequeue (packet);
    }
  return packet;
}

Ptr<Packet>
DropTailQueuePacket::Remove (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<Packet> packet = DoDequeue ();

  if (packet)
    {
      NS_ASSERT (m_nBytes >= packet->GetSize ());
      NS_ASSERT (m_nPackets > 0);

      m_nBytes -= packet->GetSize ();
      m_nPackets--;

      NS_LOG_LOGIC ("m_traceDequeue (packet)");
      m_traceDequeue (packet);
      m_traceDrop (packet);
    }
  return packet;
}

Ptr<const Packet>
DropTailQueuePacket::Peek (void) const
{
  NS_LOG_FUNCTION (this);
  return DoPeek ();
}

bool
DropTailQueuePacket::DoEnqueue (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  if (m_mode == QUEUE_MODE_PACKETS && (m_packets.size () >= m_maxPackets))
    {
      NS_LOG_LOGIC ("Queue full (at max packets) -- droppping pkt");
      DropBeforeEnqueue (p);
      return false;
    }

  if (m_mode == QUEUE_MODE_BYTES && (m_bytesInQueue + p->GetSize () >= m_maxBytes))
    {
      NS_LOG_LOGIC ("Queue full (packet would exceed max bytes) -- droppping pkt");
      DropBeforeEnqueue (p);
      return false;
    }

  m_bytesInQueue += p->GetSize ();
  m_packets.push (p);

  NS_LOG_LOGIC ("Number packets " << m_packets.size ());
  NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

  ////std:://cout << "Buffer enqueue:" << p->GetSize() << "\n";

  return true;
}

Ptr<Packet>
DropTailQueuePacket::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);

  if (m_packets.empty ())
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }



  Ptr<Packet> p = m_packets.front ();
  m_packets.pop ();
  m_bytesInQueue -= p->GetSize ();

  NS_LOG_LOGIC ("Popped " << p);

  NS_LOG_LOGIC ("Number packets " << m_packets.size ());
  NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

  return p;
}

Ptr<const Packet>
DropTailQueuePacket::DoPeek (void) const
{
  NS_LOG_FUNCTION (this);

  if (m_packets.empty ())
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  Ptr<Packet> p = m_packets.front ();
  ////std:://cout << "Buffer peek:" << p->GetSize() << "\n";

  NS_LOG_LOGIC ("Number packets " << m_packets.size ());
  NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

  return p;
}

} // namespace ns3

