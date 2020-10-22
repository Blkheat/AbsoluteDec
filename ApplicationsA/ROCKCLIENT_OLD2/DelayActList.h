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
	// �н� ��ų ó���� ���ؼ�...������ �� ó�� 
	list <SDelayAppearPC*>		m_DelayAppearPCList;
	
	// �ż� ��Ÿ���� ����Ʈ ó�� ���ؼ� ������ ó��
	list <SDelayAppearNPC*>		m_DelayAppearNPCList;

	// �� ��Ÿ���� ����Ʈ ó�� ���ؼ� ������ ó��
	list <SDelayAppearPet*>		m_DelayAppearPetList;

	// �� ������� ����Ʈ ó�� ���ؼ� ������ ó��
	list < SDelayDisappearPet * >	m_DelayDisappearPetList;
};

extern CDelayActList g_DelayActList;

#endif