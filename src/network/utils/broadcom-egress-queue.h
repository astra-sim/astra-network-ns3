/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
* Copyright (c) 2006 Georgia Tech Research Corporation, INRIA
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

#ifndef BROADCOM_EGRESS_H
#define BROADCOM_EGRESS_H

#include <queue>
#include "ns3/packet.h"
#include "queue.h"
#include "drop-tail-queue-packet.h"
#include "red-queue.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/event-id.h"

namespace ns3 {

	class TraceContainer;

	class BEgressQueue : public Queue <Packet> {
	public:
		static TypeId GetTypeId(void);
		static const unsigned fCnt = 128; //max number of queues, 128 for NICs
		static const unsigned qCnt = 8; //max number of queues, 8 for switches
		BEgressQueue();
		virtual ~BEgressQueue();
		bool Enqueue(Ptr<Packet> p, uint32_t qIndex);
		Ptr<Packet> DequeueRR(bool paused[]);
		uint32_t GetNBytes(uint32_t qIndex) const;
		uint32_t GetNBytesTotal() const;
		uint32_t GetLastQueue();

		TracedCallback<Ptr<const Packet>, uint32_t> m_traceBeqEnqueue;
		TracedCallback<Ptr<const Packet>, uint32_t> m_traceBeqDequeue;

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

	private:
		bool DoEnqueue(Ptr<Packet> p, uint32_t qIndex);
		Ptr<Packet> DoDequeueRR(bool paused[]);
		//for compatibility
		virtual bool DoEnqueue(Ptr<Packet> p);
		virtual Ptr<Packet> DoDequeue(void);
		virtual Ptr<const Packet> DoPeek(void) const;
		double m_maxBytes; //total bytes limit
  		NS_LOG_TEMPLATE_DECLARE; //!< the log component
		uint32_t m_bytesInQueue[fCnt];
		uint32_t m_bytesInQueueTotal;
		uint32_t m_rrlast;
		uint32_t m_qlast;
		std::vector<Ptr<Queue> > m_queues; // uc queues

	protected:
		TracedCallback<Ptr<const Packet> > m_traceEnqueue;
		TracedCallback<Ptr<const Packet> > m_traceDequeue;
 		TracedCallback<Ptr<const Packet> > m_traceDrop;
	};

} // namespace ns3

#endif /* DROPTAIL_H */
