
#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/agent.h"
#include "world/company/mission.h"
#include "world/scheduler/attemptmissionevent.h"


AttemptMissionEvent::AttemptMissionEvent ()
{
}

AttemptMissionEvent::~AttemptMissionEvent ()
{
}

void AttemptMissionEvent::Run ()
{

	Agent *agent = (Agent *) game->GetWorld ()->GetPerson ( agentname );
	UplinkAssert (agent);

	agent->AttemptMission ();

}

void AttemptMissionEvent::SetAgentName ( char *newagentname )
{

	UplinkAssert ( strlen (newagentname) < SIZE_PERSON_NAME );
	strcpy ( agentname, newagentname );

}

char *AttemptMissionEvent::GetShortString ()
{

	char *shortstring = new char [strlen(agentname) + 32];
	sprintf ( shortstring, "Attempt Mission for %s", agentname );
	return shortstring;

}

char *AttemptMissionEvent::GetLongString ()
{

	Agent *agent = (Agent *) game->GetWorld ()->GetPerson ( agentname );
	UplinkAssert (agent);
	
	Mission *m = agent->missions.GetData (0);
	UplinkAssert (m);

	ostrstream longstring;
	longstring << "Attempt Mission Event:\n"
			   << "Agent : " << agentname << "\n"
			   << "Agents next mission : " << m->description 
			   << '\x0';

	return longstring.str ();

}


void AttemptMissionEvent::Load ( FILE *file )
{

	LoadID ( file );

	UplinkEvent::Load ( file );

	fread ( agentname, sizeof(agentname), 1, file );

	LoadID_END ( file );

}

void AttemptMissionEvent::Save ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	fwrite ( agentname, sizeof(agentname), 1, file );

	SaveID_END ( file );

}

void AttemptMissionEvent::Print ()
{
	
	printf ( "AttemptMissionEvent : agentname=%s\n", agentname );

	UplinkEvent::Print ();

}
	
char *AttemptMissionEvent::GetID ()
{

	return "EVT_AME";

}

int AttemptMissionEvent::GetOBJECTID ()
{
	
	return OID_ATTEMPTMISSIONEVENT;

}