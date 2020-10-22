#include "RockPCH.h"


#include	<NETWORK\\Protocol.h>
#include	"BASE\\D3DBASE.H"
#include	"..\\quadlist.h"
#include	"..\\PC.h"

#include	"..\\RockClient.h"
#include	"Cloak.h"

#define AFC			0.05f
#define	MAX_LENGTH1		0.7f
#define	MAX_LENGTH2		0.4f
inline float Distance( float x , float z ) { return (float)sqrt((double)(x*x + z*z)); }