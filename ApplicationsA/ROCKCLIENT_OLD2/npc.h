#ifndef		_____MAP_NPC_H_____
#define		_____MAP_NPC_H_____

const int c_Milena_Guad_Point_Npc( 4805 );
const int c_Rain_Gaud_Point_Npc( 4806 );


class		CBravoNpc
{
protected:
	DWORD		m_dwCount;
	char**		m_szNameList;

public:
	bool		Create();
	void		FinalCleanup();

	DWORD		GetCount()					{ return m_dwCount; };
	char*		GetName(DWORD i)			{ return m_szNameList[i]; };
	DWORD		GetIndexByName(char* name);
	bool		LoadNpcTeleport(char* name);

	CBravoNpc();
	~CBravoNpc();
};

extern	CBravoNpc	g_Npc;
#endif
