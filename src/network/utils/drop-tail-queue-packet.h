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

#ifndef DROPTAIL_PACKET_H
#define DROPTAIL_PACKET_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"

namespace ns3 {

class TraceContainer;

/**
 * \ingroup queue
 *
 * \brief A FIFO packet queue that drops tail-end packets on overflow
 */
class DropTailQueuePacket : public Queue <Packet> {
public:
  static TypeId GetTypeId (void);
  /**
   * \brief DropTailQueuePacket Constructor
   *
   * Creates a droptail queue with a maximum size of 100 packets by default
   */
  DropTailQueuePacket ();

  virtual ~DropTailQueuePacket();

  /**
   * Place an item into the Queue (each subclass defines the position)
   * \param item item to enqueue
   * \return True if the operation was successful; false otherwise
   */
  bool Enqueue(Ptr<Packet> item);

  /**
   * Remove an item from the Queue (each subclass defines the position),
   * counting it and tracing it as dequeued
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<Packet> Dequeue();

  /**
   * Remove an item from the Queue (each subclass defines the position),
   * counting it and tracing it as both dequeued and dropped
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<Packet> Remove();

  /**
   * Get a copy of an item in the queue (each subclass defines the position)
   * without removing it
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<const Packet> Peek() const;

  /**
   * Set the operating mode of this device.
   *
   * \param mode The operating mode of this device.
   *
   */
  void SetMode (DropTailQueuePacket::QueueMode mode);

  /**
   * Get the encapsulation mode of this device.
   *
   * \returns The encapsulation mode of this device.
   */
  DropTailQueuePacket::QueueMode GetMode (void);

private:
  virtual bool DoEnqueue (Ptr<Packet> p);
  virtual Ptr<Packet> DoDequeue (void);
  virtual Ptr<const Packet> DoPeek (void) const;

  std::queue<Ptr<Packet> > m_packets;
  NS_LOG_TEMPLATE_DECLARE; //!< the log component
  uint32_t m_maxPackets;
  uint32_t m_maxBytes;
  uint32_t m_bytesInQueue;
  QueueMode m_mode;

protected:
  TracedCallback<Ptr<const Packet> > m_traceEnqueue;
  TracedCallback<Ptr<const Packet> > m_traceDequeue;
  TracedCallback<Ptr<const Packet> > m_traceDrop;
};

} // namespace ns3

#endif /* DROPTAIL_PACKET_H */
