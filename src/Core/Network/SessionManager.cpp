/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/SessionManager.h"
#include "Core/Network/Peer.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

SessionManager::SessionManager()
{
}

//-----------------------------------//

SessionManager::~SessionManager()
{
}

//-----------------------------------//

void SessionManager::addSession(const SessionPtr& session)
{
	if( !session ) return;
	const PeerPtr& peer = session->getPeer();
	
	sessions[peer] = session;

	onSessionAdded(session);
}

//-----------------------------------//

void SessionManager::removeSession(const SessionPtr& session)
{
	if( !session ) return;
	const PeerPtr& peer = session->getPeer();

	SessionsMap::iterator it = sessions.find(peer);
	
	if( it == sessions.end() )
		return;

	sessions.erase(it);

	onSessionRemoved(session);
}

//-----------------------------------//

SessionPtr SessionManager::getSession(const PeerPtr& peer)
{
	SessionsMap::iterator it = sessions.find(peer);

	if( it == sessions.end() )
		return nullptr;

	return it->second;
}

//-----------------------------------//

NAMESPACE_CORE_END