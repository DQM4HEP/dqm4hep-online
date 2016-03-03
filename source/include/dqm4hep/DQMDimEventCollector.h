/*
 *
 * DQMDimEventCollector.h header template automatically generated by a class generator
 * Creation date : mer. sept. 9 2015
 *
 * This file is part of DQM4HEP libraries.
 * 
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#ifndef DQMDIMEVENTCOLLECTOR_H
#define DQMDIMEVENTCOLLECTOR_H

// -- dqm4hep headers
#include "dqm4hep/DQMEventCollectorImp.h"
#include "dqm4hep/DQMStatisticsService.h"

// -- xdrstream headers
#include "xdrstream/xdrstream.h"

// -- dim headers
#include "dis.hxx"

namespace dqm4hep
{

class DQMDimEventCollector;

/** DimEventRequestRpc class
 */
class DimEventRequestRpc : public DimRpc
{
public:
	/** Constructor
	 */
	DimEventRequestRpc(DQMDimEventCollector *pCollector);

	/** The rpc handler
	 */
	void rpcHandler();

private:
	// the collector
	DQMDimEventCollector        *m_pCollector;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

/** DQMDimEventCollector class
 */
class DQMDimEventCollector : public DQMEventCollectorImp, public DimServer
{
//	friend class DimEventReceptionRpc;
	friend class DimEventRequestRpc;
 public:
	/** Constructor
	 */
	DQMDimEventCollector();

	/** Destructor
	 */
	virtual ~DQMDimEventCollector();

	/** Set the collector name
	 */
	StatusCode setCollectorName(const std::string &collectorName);

	/** Get the collector name
	 */
	const std::string &getCollectorName() const;

	/** Whether the collector server is running
	 */
	bool isRunning() const;

	/** Start the collector server
	 */
	StatusCode startCollector();

	/** Stop the collector server
	 */
	StatusCode stopCollector();

	/** Set the event streamer to serialize/deserialize the in/out-coming events
	 */
	void setEventStreamer(DQMEventStreamer *pEventStreamer);

	/** Get the event streamer
	 */
	DQMEventStreamer *getEventStreamer() const;

private:
	/** Dim command handler
	 */
	void commandHandler();

	/** Dim client exit handler
	 */
	void clientExitHandler();

	/** Client class
	 *
	 *  Represent a client instance from the point of
	 *  view of the service (id and update booleans)
	 */
	class Client
	{
	public:
		int           m_clientId;       ///< The client id (dim client id)
		bool          m_updateMode;    ///< Whether the client uses an update mode
		std::string    m_subEventIdentifier;   ///< The sub event identifier received from the client from
	};

	/**
	 */
	void handleEventReception(DimCommand *pDimCommand);

	/**
	 */
	void handleEventRequest(DimEventRequestRpc *pDimRpc);

	/** Get a client by id. Create it if not registered
	 */
	Client &getClient(int clientId);

	/** Update the event service for clients
	 *  that have specified an update mode
	 */
	void updateEventService();

	/** Remove a client from the map
	 */
	void removeClient(int clientId);

	/** Configure the buffer. Allocate the ptr is needed and set
	 *  the buffer to read only and owner of the buffer. The buffer
	 *  is copied since event updates need to keep track of the buffer
	 */
	xdrstream::BufferDevice *configureBuffer(char *pBuffer, xdrstream::xdr_size_t bufferSize);

private:

	typedef std::map<int, Client> ClientMap;

	std::string              m_collectorName;
	bool                    m_isRunning;
	int                     m_state;
	int                     m_clientRegisteredId;

	// services
	DimService              *m_pServerStateService;
	DimService              *m_pClientRegisteredService;
	DimService              *m_pEventUpdateService;
	DQMStatisticsService    *m_pStatisticsService;

	// commands
	DimCommand              *m_pCollectEventCommand;
	DimCommand              *m_pUpdateModeCommand;
	DimCommand              *m_pSubEventIdentifierCommand;
	DimCommand              *m_pClientRegitrationCommand;

	// remote procedure call
	DimEventRequestRpc      *m_pEventRequestRpc;

	xdrstream::BufferDevice *m_pBuffer;
	xdrstream::BufferDevice *m_pSubEventBuffer;

	DQMEventStreamer        *m_pEventStreamer;
	DQMEvent                *m_pCurrentEvent;

	ClientMap                m_clientMap;
}; 

} 

#endif  //  DQMDIMEVENTCOLLECTOR_H
