#ifndef __CTHREEWAYDISPLAY_H
#define __CTHREEWAYDISPLAY_H

#include "CSquenceBase.h"

class CThreeWayDisplay : public CSquenceBase
{
public:
	 
			 CThreeWayDisplay();
    virtual ~CThreeWayDisplay() {};

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
	virtual int Squence09();

private:

	void SetCameraFront(float Range);
	void SetCameraRight(float Range);
	void SetCameraLeft(float Range);

	void SetStartScene(void);
	void SetEndScene(void);

private:
	
	CAMERA_SAVE_VALUE	m_SaveCamera;

	float				m_fHeight;	
	int					m_AniIndexList[ 4 ];
	int					m_WeaponType;
	
public:

	virtual int SquenceCheck00();
	virtual int SquenceCheck01();
	virtual int SquenceCheck08();
	virtual int SquenceCheck09();
};

#endif