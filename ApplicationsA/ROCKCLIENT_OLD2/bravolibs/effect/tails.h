
#ifndef		__TAILS_H__
#define		__TAILS_H__

#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
#include	<OBJ\\Define.h>

#include	"EffectVertex.h"

#define		MAX_TAILS	256

//-----------------------------------------------------------------------------
// Desc: 窒葬
//-----------------------------------------------------------------------------
class CTails
{
public:
	float			m_fAllStart[MAX_TRACE_COUNT][3];
	float			m_fAllEnd[MAX_TRACE_COUNT][3];

	float			m_fStart[TOTAL_POINTS][3];
	float			m_fEnd[TOTAL_POINTS][3];

	D3DXVECTOR3		vTop;						// max高
	D3DXVECTOR3		vBottom;					// min高
	int				m_count;					// 酈高
	int				m_TotalTraceCnt;			// 識 薄偎熱
	int				m_Step;						// 醞除曖 爾除高

	TAILS_VERTEX	m_TraceVBuf[MAX_TRACE_COUNT*2];	

	void	Clear()
	{	
		m_count = -1;
		vTop    = D3DXVECTOR3( 0.0f, 0.3f, 1.0f );
		vBottom = D3DXVECTOR3( 0.0f, -0.3f, -1.0f );
		m_Step	= 3;
	}
	void	SetTails( D3DXVECTOR3& topv, D3DXVECTOR3& bottomv ) { vTop = topv; vBottom = bottomv; }
	void	DisPlay();
	void	AddPoint( D3DXVECTOR3& v, float width = 2.0f);
	void	AddPoint2( D3DXVECTOR3& v, float width = 2.0f);
	
	CTails() { Clear(); }
	~CTails() {}
};


class	CTailsManager
{
public:
	CTails			m_Tails[ MAX_TAILS ];
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_TexTail[2];
#else
	LPDIRECT3DTEXTURE8	m_TexTail[2];
#endif // DIRECT_VERSION_9_MJH
	int				texnum;

	bool			tex;

	void			LoadTex( char* dir );

	void			Render();
	int				FindNextTail();
	
	CTailsManager();
	~CTailsManager();
};



#endif