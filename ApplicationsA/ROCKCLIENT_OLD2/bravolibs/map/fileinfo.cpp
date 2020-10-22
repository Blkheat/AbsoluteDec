#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	"Field.H"
#include	"..\\Camera.h"
#include	"FileInfo.h"
#include	<stdio.h>


bool	CFileInfo::Store(char* filename)
{
	return true;
}

bool	CFileInfo::Load(char* filename)
{
	FILE	* fp	= NULL;

	fp	= fopen(filename, "rb");
	if ( !fp )
	{
		return false;
	}
	fclose( fp );
	return true;
}
