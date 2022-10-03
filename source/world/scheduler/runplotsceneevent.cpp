
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/scheduler/runplotsceneevent.h"



RunPlotSceneEvent::RunPlotSceneEvent ()
{
	
	act = 0;
	scene = 0;

}

RunPlotSceneEvent::~RunPlotSceneEvent ()
{
}

void RunPlotSceneEvent::SetActAndScene ( int newact, int newscene )
{

	act = newact;
	scene = newscene;

}

void RunPlotSceneEvent::Run ()
{

    if ( act != -1 )
	    game->GetWorld ()->plotgenerator.Run_Scene ( act, scene );

    else
        game->GetWorld ()->demoplotgenerator.RunScene ( scene );

}


char *RunPlotSceneEvent::GetShortString ()
{
	
	return GetLongString ();

}

char *RunPlotSceneEvent::GetLongString ()
{

	char *result = new char [32];

    if ( act != -1 )
	    sprintf ( result, "Run Plot Act %d, Scene %d", act, scene );

    else
        sprintf ( result, "Run Demo Plot Scene %d", scene );

	return result;

}


void RunPlotSceneEvent::Load  ( FILE *file )
{

	LoadID ( file );

    UplinkEvent::Load ( file );

	fread ( &act, sizeof(act), 1, file );
	fread ( &scene, sizeof(scene), 1, file );

	LoadID_END ( file );

}

void RunPlotSceneEvent::Save  ( FILE *file )
{

	SaveID ( file );

    UplinkEvent::Save ( file );

	fwrite ( &act, sizeof(act), 1, file );
	fwrite ( &scene, sizeof(scene), 1, file );

	SaveID_END ( file );
}

void RunPlotSceneEvent::Print ()
{

    UplinkEvent::Print ();
	printf ( "%s\n", GetLongString () );

}

	
char *RunPlotSceneEvent::GetID ()
{
	
	return "EVT_PLOT";

}

int  RunPlotSceneEvent::GetOBJECTID ()
{
	
	return OID_RUNPLOTSCENEEVENT;

}


