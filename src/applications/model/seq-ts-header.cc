/*
 * Copyright (c) 2009 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "seq-ts-header.h"

#include "ns3/assert.h"
#include "ns3/header.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("SeqTsHeader");

NS_OBJECT_ENSURE_REGISTERED(SeqTsHeader);

SeqTsHeader::SeqTsHeader()
    : m_seq(0),
      m_ts(Simulator::Now().GetTimeStep())
{
    NS_LOG_FUNCTION(this);
}

void
SeqTsHeader::SetSeq(uint32_t seq)
{
    NS_LOG_FUNCTION(this << seq);
    m_seq = seq;
}

uint32_t
SeqTsHeader::GetSeq() const
{
    NS_LOG_FUNCTION(this);
    return m_seq;
}

Time
SeqTsHeader::GetTs() const
{
    NS_LOG_FUNCTION(this);
    return TimeStep(m_ts);
}

void
SeqTsHeader::SetPG (uint16_t pg)
{
	m_pg = pg;
}

uint16_t
SeqTsHeader::GetPG (void) const
{
	return m_pg;
}

TypeId
SeqTsHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::SeqTsHeader")
                            .SetParent<Header>()
                            .SetGroupName("Applications")
                            .AddConstructor<SeqTsHeader>();
    return tid;
}

TypeId
SeqTsHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

void
SeqTsHeader::Print(std::ostream& os) const
{
    NS_LOG_FUNCTION(this << &os);
    os << "(seq=" << m_seq << " time=" << TimeStep(m_ts).As(Time::S) << ")";
}

uint32_t
SeqTsHeader::GetSerializedSize() const
{
    NS_LOG_FUNCTION(this);
	return GetHeaderSize();
}

uint32_t SeqTsHeader::GetHeaderSize(void){
	return 6 + IntHeader::GetStaticSize();
}

void
SeqTsHeader::Serialize(Buffer::Iterator start) const
{
    NS_LOG_FUNCTION(this << &start);
    Buffer::Iterator i = start;
    i.WriteHtonU32(m_seq);
    i.WriteHtonU16(m_pg);

    // write IntHeader
    ih.Serialize(i);
}

uint32_t
SeqTsHeader::Deserialize(Buffer::Iterator start)
{
    NS_LOG_FUNCTION(this << &start);
    Buffer::Iterator i = start;
    m_seq = i.ReadNtohU32();
    m_pg =  i.ReadNtohU16 ();

    // read IntHeader
    ih.Deserialize(i);
    return GetSerializedSize ();
}

} // namespace ns3
