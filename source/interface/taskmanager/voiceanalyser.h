

/*

  Tutorial program

	Sits on screen and tells you what to do 
	every now and again


  */

#ifndef _included_voiceanalyser_h
#define _included_voiceanalyser_h

// ============================================================================

#include "eclipse.h"

#include "interface/taskmanager/uplinktask.h"

#define VOICEANALYSER_NUMSAMPLES 25

#define VOICEANALYSER_STATUS_NONE			0
#define VOICEANALYSER_STATUS_WAITING		1
#define VOICEANALYSER_STATUS_RECORDING		2
#define	VOICEANALYSER_STATUS_ANALYSING		3
#define	VOICEANALYSER_STATUS_READY			4
#define	VOICEANALYSER_STATUS_REPRODUCING	5

// ============================================================================


class VoiceAnalyser : public UplinkTask
{

protected:

	static void CloseClick ( Button *button );
	static void TitleClick ( Button *button );

	static void MainTextDraw ( Button *button, bool highlighted, bool clicked );
	static void DrawAnalysis ( Button *button, bool highlighted, bool clicked );

	static void PlayDraw ( Button *button, bool highlighted, bool clicked );
	static void PlayClick ( Button *button );
	static void PlayMouseMove ( Button *button );
	static void PlayMouseDown ( Button *button );

protected:

	int STATUS;
	int sample[VOICEANALYSER_NUMSAMPLES];
	char personname [SIZE_PERSON_NAME];
	int timesync;
	int animsync;

public:

	VoiceAnalyser ();
	~VoiceAnalyser ();
	

	void Initialise ();     
	void Tick ( int n );           

	void MoveTo ( int x, int y, int time_ms );			

	void CreateInterface ();       
	void RemoveInterface ();
	void ShowInterface ();
	bool IsInterfaceVisible ();     

};



#endif