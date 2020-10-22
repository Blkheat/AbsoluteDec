#ifndef		_____DELAYACTLIST_____
#define		_____DELAYACTLIST_____

#include	"brother.h"
#include	<OBJ\\ObjBase.H>
#include	<OBJ\\Player.H>
#include	<OBJ\\NonPlayer.H>
#include	<Map\Field.h>
#include	"Map.H"
#include	"Bravolibs\\Obj\\SkillSystem.h"
#include	<list>

using namespace std;

typedef struct SDelayAppearPC
{
	long				DelayTime;
	SRpScAppearPC		AppearPC;
	
} SDelayAppearPC; 

typedef struct SDelayAppearNPC
{
	long				DelayTime;
	SRpScAppearNPC		AppearNPC;

} SDelayAppearNPC;

typedef struct SDelayAppearPet
{
	long				DelayTime;
	SRpScAppearPet		AppearPet;

} SDelayAppearPet;

typedef struct SDelayDisappearPet
{
	long				DelayTime;
	SRpScDisappearPet	DisappearPet;
} SDelayDisappearPet;

class CDelayActList
{

public:	
	// 분신 스킬 처리를 위해서...딜레이 후 처리 
	list <SDelayAppearPC*>		m_DelayAppearPCList;
	
	// 신수 나타날때 이펙트 처리 위해서 딜레이 처리
	list <SDelayAppearNPC*>		m_DelayAppearNPCList;

	// 펫 나타날때 이펙트 처리 위해서 딜레이 처리
	list <SDelayAppearPet*>		m_DelayAppearPetList;

	// 펫 사라질때 이펙트 처리 위해서 딜레이 처리
	list < SDelayDisappearPet * >	m_DelayDisappearPetList;
};

extern CDelayActList g_DelayActList;

#endif