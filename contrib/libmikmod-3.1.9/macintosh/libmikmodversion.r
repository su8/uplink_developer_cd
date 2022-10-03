/*	libmikmodversion.r
	NOTE: this file needs macintosh linefeeds (\r) to work with Rez!
*/

#ifndef __TYPES_R__
#include "Types.r"
#endif

#ifndef __BALLOONS_R__
#include "Balloons.r"
#endif

#define LIBMIKMOD_VERSION_MAJOR 		3
#define LIBMIKMOD_VERSION_MINOR 		1
#define LIBMIKMOD_REVISION     			8

#define LIBMIKMOD_STATE	    			release		/* development | alpha | beta | release */
#define LIBMIKMOD_RELEASE_NO     		0			/* number after letter, or zero for release */
#define LIBMIKMOD_COUNTRY				verSweden

#define LIBMIKMOD_VERSION_STRING 		"3.1.8"

#define LIBMIKMOD_NAME					"libmikmod"
#define LIBMIKMOD_SHORT_DESCRIPTION		"music module player"
#define LIBMIKMOD_LONG_DESCRIPTION		"A portable sound / music module playing shared library."

/*	Resources for libmikmod shared library : */

resource 'vers' (1) {
	LIBMIKMOD_VERSION_MAJOR,
	(LIBMIKMOD_VERSION_MINOR << 4) | LIBMIKMOD_REVISION,
	LIBMIKMOD_STATE,
	LIBMIKMOD_RELEASE_NO,
	LIBMIKMOD_COUNTRY,
	LIBMIKMOD_VERSION_STRING,
	LIBMIKMOD_VERSION_STRING
};

resource 'vers' (2) {
	LIBMIKMOD_VERSION_MAJOR,
	(LIBMIKMOD_VERSION_MINOR << 4) | LIBMIKMOD_REVISION,
	LIBMIKMOD_STATE,
	LIBMIKMOD_RELEASE_NO,
	LIBMIKMOD_COUNTRY,
	LIBMIKMOD_VERSION_STRING,
	LIBMIKMOD_SHORT_DESCRIPTION
};


	/* Extension Manager info */
data 'CCI�' (128) {		
	LIBMIKMOD_NAME "\n\n"
		LIBMIKMOD_LONG_DESCRIPTION
};

	/* Finder help balloon */
resource 'hfdr' (kHMHelpID) {
	HelpMgrVersion,
	hmDefaultOptions,
	0,
	0,
	{	
		HMStringItem
		{
			LIBMIKMOD_NAME "\n\n"
				LIBMIKMOD_LONG_DESCRIPTION
		}
	}
	
};

resource 'BNDL' (128) {
	'lmm!',
	0,
	{	'ICN#',
		{	0, 128
		},
		'FREF',
		{	0, 128
		}
	}
};

resource 'FREF' (128) {
	'shlb',
	0,
	""
};

	/* Owner resource */
data 'lmm!' (0) {
	$"00"                                                 
};



/*	nice custom icon for extension, drawn by Anders F Bj�rklund, follows: */

resource 'ICN#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"7FFF FFF0 8000 0008 8000 0008 8015 0008"
		$"8011 0008 8015 6008 8015 9008 8015 1008"
		$"A015 1008 D015 100A 9015 E00D 1000 0009"
		$"1000 5001 1000 1001 1166 5201 1199 5401"
		$"1111 5801 1111 5401 1111 5201 1111 5201"
		$"1000 0009 9000 004D D000 004A A598 E3C8"
		$"8665 1448 8445 1448 8445 1448 8445 14CE"
		$"8444 E34F 8000 000F 8000 000E 7FFF FFFC",
		/* [2] */
		$"7FFF FFF0 FFFF FFF8 FFFF FFF8 FFFF FFF8"
		$"FFFF FFF8 FFFF FFF8 FFFF FFF8 FFFF FFF8"
		$"FFFF FFF8 DFFF FFFA 9FFF FFFF 1FFF FFFF"
		$"1FFF FFFF 1FFF FFFF 1FFF FFFF 1FFF FFFF"
		$"1FFF FFFF 1FFF FFFF 1FFF FFFF 1FFF FFFF"
		$"1FFF FFFF 9FFF FFFF DFFF FFFA FFFF FFF8"
		$"FFFF FFF8 FFFF FFF8 FFFF FFF8 FFFF FFFE"
		$"FFFF FFFF FFFF FFFF FFFF FFFE 7FFF FFFC"
	}
};

resource 'icl8' (128) {
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 0000"
	$"FF00 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 00F6 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6C0 F6F6 F6C0"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"F6C0 C0F6 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"C0F6 F6C0 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FF00 F9F6 F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"F6F6 F6C0 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FF00 FFF9 F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"F6F6 F6C0 F6F6 F6F6 F6F6 F6F9 FF00 0000"
	$"FFFF 00FF F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"F6F6 F6C0 F6F6 F6F6 F6F6 F6F9 FF00 FF00"
	$"FF00 00FF F6F6 F6F6 F6F6 F6C0 F6C0 F6C0"
	$"C0C0 C0F6 F6F6 F6F6 F6F6 F6F9 FFFF 00FF"
	$"0000 00FF F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F9 FF00 F9FF"
	$"0000 00FF F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6C0 F6C0 F6F6 F6F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6C0 F6F6 F6F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6C0 F6C0 C0F6 F6C0 C0F6"
	$"F6C0 F6C0 F6F6 C0F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6C0 C0F6 F6C0 C0F6 F6C0"
	$"F6C0 F6C0 F6C0 F6F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6C0 F6F6 F6C0 F6F6 F6C0"
	$"F6C0 F6C0 C0F6 F6F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6C0 F6F6 F6C0 F6F6 F6C0"
	$"F6C0 F6C0 F6C0 F6F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6C0 F6F6 F6C0 F6F6 F6C0"
	$"F6C0 F6C0 F6F6 C0F6 F6F6 F6F6 F6F6 F9FF"
	$"0000 00FF F6F6 F6C0 F6F6 F6C0 F6F6 F6C0"
	$"F6C0 F6C0 F6F6 C0F6 F6F6 F6F6 F9F6 F9FF"
	$"0000 00FF F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F9 FFF9 F9FF"
	$"FF00 00FF F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6C0 F6F9 FFFF F9FF"
	$"FFFF 00FF F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6C0 F6F9 FF00 FF00"
	$"FF00 FF00 F6C0 F6C0 C0F6 F6C0 C0F6 F6F6"
	$"C0C0 C0F6 F6F6 C0C0 C0C0 F6F9 FF00 0000"
	$"FF00 0000 F6C0 C0F6 F6C0 C0F6 F6C0 F6C0"
	$"F6F6 F6C0 F6C0 F6F6 F6C0 F6F9 FF00 0000"
	$"FF00 F6F6 F6C0 F6F6 F6C0 F6F6 F6C0 F6C0"
	$"F6F6 F6C0 F6C0 F6F6 F6C0 F6F9 FF00 0000"
	$"FF00 F6F6 F6C0 F6F6 F6C0 F6F6 F6C0 F6C0"
	$"F6F6 F6C0 F6C0 F6F6 F6C0 F6F9 FF00 0000"
	$"FF00 F6F6 F6C0 F6F6 F6C0 F6F6 F6C0 F6C0"
	$"F6F6 F6C0 F6C0 F6F6 C0C0 F6F9 FFFC FC00"
	$"FF00 F6F6 F6C0 F6F6 F6C0 F6F6 F6C0 F6F6"
	$"C0C0 C0F6 F6F6 C0C0 F6C0 F6F9 FFFC FCFC"
	$"FFF6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F9 FFFC FCFC"
	$"FFF6 F9F9 F9F9 F9F9 F9F9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 F9F9 F9F9 F9F9 F9F9 FFFC FC00"
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FCFC"
};


resource 'ics#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"FFFE 8002 8002 8102 8102 C103 4001 4281"
		$"4541 4541 C003 8282 8542 8542 8002 FFFE",
		/* [2] */
		$"FFFE FFFE FFFE FFFE FFFE FFFF 7FFF 7FFF"
		$"7FFF 7FFF 7FFF FFFE FFFE FFFE FFFE FFFE"
	}
};

resource 'ics8' (128) {
	$"FBFF FFFF FFFF FFFF FFFF FFFF FFFF FB00"
	$"FF00 0000 0000 0000 0000 0000 00F6 FF00"
	$"FFF6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F8 FF00"
	$"FFF6 F6F6 F6F6 F6C0 F6F6 F6F6 F6F8 FF00"
	$"FFF6 F6F6 F6F6 F6C0 F6F6 F6F6 F6F8 FF00"
	$"FFFF 00F6 F6F6 F6C0 F6F6 F6F6 F6F8 FFFF"
	$"00FF 00F6 F6F6 F6F6 F6F6 F6F6 F6F6 F8FF"
	$"00FF 00F6 F6F6 C0F6 C0F6 F6F6 F6F6 F8FF"
	$"00FF 00F6 F6C0 F6C0 F6C0 F6F6 F6F6 F8FF"
	$"00FF 00F6 F6C0 F6C0 F6C0 F6F6 F6F6 F8FF"
	$"FFFF 00F6 F6F6 F6F6 F6F6 F6F6 F6F6 FFFF"
	$"FF00 00F6 F6F6 C0F6 C0F6 F6F6 F6F8 FF00"
	$"FF00 F6F6 F6C0 F6C0 F6C0 F6F6 F6F8 FF00"
	$"FF00 F6F6 F6C0 F6C0 F6C0 F6F6 F6F8 FF00"
	$"FFF6 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 FF00"
	$"FBFF FFFF FFFF FFFF FFFF FFFF FFFF FB"
};