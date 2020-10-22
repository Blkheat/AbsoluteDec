
#ifndef		__ANIMSEQUENCE_H__
#define		__ANIMSEQUENCE_H__


#include "Timer.h"
#include "BASE\\D3DBASE.H"

class CAnimFrame
{
public:
  CAnimFrame()			{ m_pTexture = NULL; }
  virtual ~CAnimFrame()	{ SAFE_RELEASE(m_pTexture); }
#ifdef DIRECT_VERSION_9_MJH
  LPDIRECT3DTEXTURE9	m_pTexture;
#else
  LPDIRECT3DTEXTURE8	m_pTexture;
#endif // DIRECT_VERSION_9_MJH
  float					m_fTime;
};



class CAnimSequence
{
public:

	 CAnimSequence();
	~CAnimSequence(); 

	void		LoadAni(char* dir);

public:
	
	float		m_fTimeGap;
	
	bool		AddFrame(const char *strFilename, float fTime);
	void		DeleteFrame(int index);
	void		ClearAllFrames();
	int			GetCurFrame(CTimer &timer);

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 GetCurFrameTexture(CTimer &timer)
#else
	LPDIRECT3DTEXTURE8 GetCurFrameTexture(CTimer &timer)
#endif // DIRECT_VERSION_9_MJH
	{
		try
		{
			int iCurFrame = GetCurFrame(timer);
			if (iCurFrame >= (int)m_vFrames.size())
				return NULL;

			return(m_vFrames[iCurFrame]->m_pTexture);
		}
		catch(...) { }
		return(NULL);
	}

	int			GetSize() { return m_vFrames.size(); }


protected:
	
	std::vector<CAnimFrame *> m_vFrames;  

};



#endif