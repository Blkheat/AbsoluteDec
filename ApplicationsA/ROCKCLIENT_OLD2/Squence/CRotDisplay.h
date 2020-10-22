#ifndef __CROTDISPLAY_H
#define __CROTDISPLAY_H

#include "CSquenceBase.h"

class CRotDisplay : public CSquenceBase
{
public:
	 
			 CRotDisplay();
    virtual ~CRotDisplay() {};

public:

	virtual int Squence00();
	virtual int Squence01();
	virtual int Squence02();
	virtual int Squence03();
	virtual int Squence04();
	virtual int Squence05();
	virtual int Squence06();
	virtual int Squence07();
	virtual int Squence08();

private:

	void SetCameraFront(float Range);
	void RotateCamera(void);

	void SetStartScene(void);
	void SetEndScene(void);

private:
	
	CAMERA_SAVE_VALUE	m_SaveCamera;
	float				m_fHeight;	
	float				m_fRangeAngle;
	float				m_OldTimer;
	float				m_RotTimer;
	float				m_fDistance;
	D3DXVECTOR3			m_vDir;

	int					m_AniIndex;
	BOOL				m_ISMan;
	
public:

	virtual int SquenceCheck00();
	virtual int SquenceCheck01();
	virtual int SquenceCheck03();
	virtual int SquenceCheck04();

	virtual int SquenceCheck07();
	virtual int SquenceCheck08();
};

#endif
