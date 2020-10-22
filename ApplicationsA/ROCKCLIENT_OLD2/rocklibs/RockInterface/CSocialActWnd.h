#ifndef __CSOCIALACTWND_H
#define __CSOCIALACTWND_H


#include "WndProc.h"
#include "FrameWnd.h"
#include "ScrollBar.h"
#include "RockClient.h"

const int c_Max_SocialActSlot_Num( 19 );

#ifdef ADD_MOTION_DANCE
#define MOTION_DANCE
#endif



#ifdef MOTION_DANCE
	const int c_Max_Draw_SocialActSlot_Num(19);
#else
	const int c_Max_Draw_SocialActSlot_Num(18);
#endif

  
const int c_Max_Draw_RideActSlot_Num(3);

typedef struct SSocialActSlot
{
	CImageBox*		ActImg;
	CButton*		ActBtn;
	int				ActInfoIdx;
	SSocialActSlot()
	{
		Init();
	}
	void Init()
	{
		ActImg = NULL;
		ActBtn = NULL;
		ActInfoIdx =0 ; 
	}


} SSocialActSlot; 


typedef struct SSocialActInfo
{
	int				ActIdx;				// ������ ����ϴ� �米���� �ε��� 
	int				ImgTID;				// Image TID 
	int				HelpID;				// Help Tip 

	SSocialActInfo()
	{
		Init();
	}

	void Init()
	{
		ActIdx = 0;
		ImgTID = 0;
		HelpID = 0;
	}

} SSocialActInfo; 


typedef enum NSocialActIdx
{
	n_ActIdx_Request = 0,				// 1 ��û
	n_ActIdx_Bow,						// 2 ���
	n_ActIdx_Rudeness,					// 3 �޷�
	n_ActIdx_Lie,						// 4 ����
	n_ActIdx_Clap,						// 5 �ڼ�
	n_ActIdx_HandGreeting,				// 6 �ȳ�
	n_ActIdx_Shy,						// 7 ����
	n_ActIdx_Cry,						// 8 ����		
	n_ActIdx_Compliments,				// 9 �λ�
	n_ActIdx_There,						// 10 ����
	n_ActIdx_Dance,						// 11 ��
	n_ActIdx_Kiss,						// 12 Ű��
	n_ActIdx_HaHa,						// 13 ���� 
	n_ActIdx_Thank,						// 14 ���� 
	n_ActIdx_Attack,					// 15 ���� 
	n_ActIdx_Nod,						// 16 �� 
	n_ActIdx_No,						// 17 �ƴ� 
	n_ActIdx_Wait,						// 18 ��� 
	n_ActIdx_SitDown, 					// 19 �ɱ� 
	 
	


} NSocialActIdx; 

typedef enum NRideActIdx
{
	n_ActIdx_Ride_Sudden2 = 0,			// 22 ������ ���� .. 
	n_ActIdx_Ride_None1,
	n_ActIdx_Ride_None2,

}NRideActIdx;


class CSocialActWnd : public CWndProc
{
public:
	 CSocialActWnd();
	~CSocialActWnd();


public:	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void	Open(void);
	void	Close(void);

	void	SetActSlot();


private:

	CFrameWnd*		m_pFrameWnd;
	
	CImageBox9*		m_BGImg;

	SSocialActInfo	m_ActInfo[c_Max_SocialActSlot_Num];
	SSocialActSlot	m_ActSlot[c_Max_Draw_SocialActSlot_Num];


	//���̵� ���� dongs �̸�� 
	SSocialActSlot	m_RideActSlot[c_Max_Draw_RideActSlot_Num];


	CButton*		m_pPrevPageBtn;
	CButton*		m_pNextPageBtn;


	//���̵� ���� 
	SSocialActInfo	m_RideActInfo[c_Max_Draw_RideActSlot_Num];

	
	int				m_StartDrawSlotNum;
};


#define			TID_GAMESTART_NORMAL_IMG				1832
#define			TID_GAMESTART_NORMAL_IMG				1832
#define			TID_GAMESTART_NORMAL_IMG				1832





extern	CSocialActWnd		g_SocialActWnd;

#endif
