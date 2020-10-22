#ifndef		_____BRAVO_CAMERA_____
#define		_____BRAVO_CAMERA_____

#include "CFunctor.h"												// ī�޶� ��ũ���� By wxywxy
#include "brother.h"
#include "rocklibs\RockInterface\CharSelectWndProc.h"

#define		FRUSTUM_RIGHT		0
#define		FRUSTUM_LEFT		1
#define		FRUSTUM_BOTTOM		2
#define		FRUSTUM_TOP			3
#define		FRUSTUM_BACK		4
#define		FRUSTUM_FRONT		5

#define		INIT_RADIUS			55

struct		camthrough
{
	int x;
	int y;
};

typedef enum e_Camera_State_Walking
{

	CAMERA_STOP      ,
	CAMERA_ANIMATION ,

}CAMERA_STATE_WALKING;

// ī�޶� ���� ���� By wxywxy
typedef struct s_CameraSaveValue
{
	D3DXVECTOR3 vEye;
	D3DXVECTOR3 vLookAt;
	D3DXVECTOR3	vUp;

	float		CameraYaw;
	float		CameraPitch;
	float		CameraRadius;
	
	D3DXMATRIX	matView;
	D3DXMATRIX	matProj;

	void operator =(const s_CameraSaveValue &TempValue)
	{
		vEye         = TempValue.vEye;
		vLookAt      = TempValue.vLookAt;
		vUp          = TempValue.vUp;

		CameraYaw	 = TempValue.CameraYaw;	
		CameraPitch  = TempValue.CameraPitch; 
		CameraRadius = TempValue.CameraRadius;
		matView		 = TempValue.matView;		
		matProj		 = TempValue.matProj;		
	}
	

}CAMERA_SAVE_VALUE,*LPCAMERA_SAVE_VALUE;


typedef struct 
{

	D3DXVECTOR3			pos;
	D3DXQUATERNION		quat;

}ANI_TRANS_DATA;

typedef struct 
{
	float	fov;
	float   near_plane;
	float   far_plane;
	int     end_frame;
	float   frame_rate;

	std::vector <ANI_TRANS_DATA> ani_list;

}ANI_WALK_DATA;


class		CBravoCamera
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pDevice;
#else
	LPDIRECT3DDEVICE8		m_pDevice;
#endif // DIRECT_VERSION_9_MJH

public:
	BOOL					m_ISSkip;
	
	D3DXMATRIX				m_matWorld;// ���� ���
	D3DXMATRIX				m_matView;// ī�޶� ���
	D3DXMATRIX				m_matProj;// ���� ���
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9			m_ViewPort;// ����Ʈ
#else
	D3DVIEWPORT8			m_ViewPort;// ����Ʈ
#endif // DIRECT_VERSION_9_MJH
	

	D3DXMATRIX				m_matChrView;

	D3DXVECTOR3				m_vEye;// ī�޶��� ���� ��ġ
	D3DXVECTOR3				m_vLookat;// ī�޶� �ٶ󺸴� ��
	D3DXVECTOR3				m_vUp;// ī�޶��� ��溤��(Y��)

	D3DXVECTOR3				m_vEyeTemp;
	D3DXVECTOR3				m_vLookatTemp;

	D3DXMATRIX				m_matRot;
	D3DXMATRIX				m_matPos;
	D3DXMATRIX				m_matTrans;

#ifdef HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)
	
	float					m_fFOV;// �þ߰�(m_fAspect�� ���� �ٸ�)
	float					m_fAspect;// ������� ��Ⱦ��(�ʺ� / ����)
	
#endif// #ifdef HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)

	float					m_fRot;
	float					m_fPos;
	float					m_fRadian;

	// ĳ������ ���� ��ġ(���� ��ġ)
	float					m_fEye_x;
	float					m_fEye_y;
	float					m_fEye_z;

	FLOAT					m_fCameraYaw;// ī�޶��� y�� ȸ����
	FLOAT					m_fCameraPitch;// ī�޶��� x,z�� ȸ���� (-1.57< m_fCameraPitch < 1.57
	FLOAT					m_fCameraRadius;// ī�޶��� �Ÿ���

//	FLOAT					m_FirstPosX;
//	FLOAT					m_FirstPosY;
	FLOAT					m_FirstPosHeight;

	float					m_fDeltaX;
	float					m_fDeltaY;
	float					m_fDeltaZ;

	float					m_fNearDistance;

//	bool					m_bViewPoint;		//1��Ī ������ ���ؼ�. ( ������ ��� ����.)

	D3DXMATRIX				GetView()			{ return m_matView; };
	void					MatchingViewPoint();
	long					m_unique;

	float					m_fCamRadMax;	//ī�޶� �Ÿ�
	

	//-----------------------------------------------------------------------------
	// kstar79
	//-----------------------------------------------------------------------------
	
	//-----------------------------------------------------------------------------

public:	// �ڿ������� ī�޶� �������� ���Ͽ�.
	void					CamSpeed( float fTimeD );
	void					Reset(void);

	float					m_fSpeed;
	float					m_fAngularSpeed;

	float					m_fRadiusVelocity;
	float					m_fYawVelocity;
	float					m_fPitchVelocity;

	FLOAT					m_fLastRadius;
	FLOAT					m_fNewPitch;

public:
	void					ClearCamThrough();
	void					SetCamThrough();
	bool					IsOneTrue( float f );
	
	camthrough				CamThrough[512];

	int						PickCamThrough();
	int						m_iFrontBack;

public:	
#ifdef DIRECT_VERSION_9_MJH
	void					Create( LPDIRECT3DDEVICE9 pDevice );//....sh

	HRESULT					Init(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 eye, D3DXVECTOR3 lookat, D3DXVECTOR3 up);
	HRESULT					Init(LPDIRECT3DDEVICE9 pDevice, DWORD tileX, DWORD tileY);
#else
	void					Create( LPDIRECT3DDEVICE8 pDevice );//....sh

	HRESULT					Init(LPDIRECT3DDEVICE8 pDevice, D3DXVECTOR3 eye, D3DXVECTOR3 lookat, D3DXVECTOR3 up);
	HRESULT					Init(LPDIRECT3DDEVICE8 pDevice, DWORD tileX, DWORD tileY);
#endif // DIRECT_VERSION_9_MJH
	HRESULT					Update();
	void					CameraPosUpdate();
	void					Transform();

	void					UpdateMovCam();

	//-----------------------------------------------------------------------------
	// kstar79
	//-----------------------------------------------------------------------------
	D3DXMATRIX				m_matInvView;			// matView�� �����
	D3DXMATRIX				m_matRotY;				// ����Y���� �ݴ밪
	D3DXVECTOR3				m_vDir;					// ���� ���� ����
	bool					m_bFog;					// FOG ���?
	bool					m_bProj;				// �������ǳ���?
	bool					m_bRotBySort;			// ȸ���Ͽ���?
	D3DXMATRIX				GetBillboardRot() { return m_matRotY; }
	D3DXVECTOR3				GetAlphaSortRot() { return m_vDir; }
	void					ProjUpdate();
	//-----------------------------------------------------------------------------	

	bool					SetZoom( float rad );
	bool					SetRotate( float Yaw, float Pitch, bool bYaw = true );
	void					SetCameraPos(float Yaw, float Pitch, float Radius); 
	void					SetCameraClear();
	void					ReloadProjTM();
	
	void					IsCamMapOut();
	bool					IsCamOk();

	void					SetSaveVlue(CAMERA_SAVE_VALUE Data);
	CAMERA_SAVE_VALUE		GetSaveVlue(void);
	int						SetCamera(D3DXVECTOR3 vEye,D3DXVECTOR3 vLookAt,D3DXVECTOR3 vUp,BOOL ISTerrainUpdate = TRUE);


public:// ����ü �ø�(3D ������ �����ϴµ� �־ ���� �߿��� �ӵ� ���� ��� ���� �ϳ�)

	void					DrawFrustumLine(void);

	FLOAT					m_Frustum[6][4];// ���������� �����ϴ� 6���� ���
	void					CalculateFrustum();// �������� ���
	void					NormalizePlane(float Frustum[6][4], int side);///-- ����� ����ȭ�ϴ� �Լ�
	bool					PointInFrustum(float x, float y, float z);
	bool					CubeInFrustum(float x, float y, float z, float size);
	bool					CubeInFrustum(float x, float y, float z, float xsize, float ysize, float zsize);
	bool					PlaneInFrustum(float x, float y, float xsize, float ysize);
	bool					NodeInFrustum(float x, float y, float z, float xsize, float ysize, float zsize);

	// New Frustum ---------------------------------------------------------------------------------------------------
public:			
	D3DXVECTOR3				theVtx[8];			/// ���������� ������ ���� 8��
	D3DXPLANE				thePlane[6];			/// ���������� �����ϴ� 6���� ���(������)
	//----------------------------------------------------------------------------------------------------------------


public:

	bool					theIsDraw;

	float					m_fDist;

	void					MovCamera();
	int						tempcamera;
	float					m_fLife;
	long					m_lPrevTime;
	BYTE					m_byLifeFirst;

	void					Delaytime();
	float					m_fDelayLife;
	long					m_lDelayPrevTime;

	D3DXVECTOR3				From2Dto3D( int x, int y );

	//............................................................................................................
	// ī�޶� ��ũ By wxywxy
	//............................................................................................................
public:

	 int LoadWalk(char *pFile);
	 ///- by simwoosung
	 void LoadInvWalk();
	 
	 int SetAnimation(int DataNum);

#ifdef DIRECT_VERSION_9_MJH
	 int SetStartFrame(int DataNum,LPDIRECT3DDEVICE9 pDevice);
	 int SetEndFrame(int DataNum,LPDIRECT3DDEVICE9 pDevice);
	 
	 int UpdateWalk(LPDIRECT3DDEVICE9 pDevice);
#else
	 int SetStartFrame(int DataNum,LPDIRECT3DDEVICE8 pDevice);
	 int SetEndFrame(int DataNum,LPDIRECT3DDEVICE8 pDevice);
	 
	 int UpdateWalk(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	 CMSGFunctor <CCharSelectWnd>m_NextFunc;											// ���� ���۵� �Լ� ����

	 void	SetLoginCamera(void);

public:

	CAMERA_STATE_WALKING m_State;


private:

#ifdef DIRECT_VERSION_9_MJH
	int SetFrame(int DataNum,int StartFrame,float Range,LPDIRECT3DDEVICE9 pDevice);
#else
	int SetFrame(int DataNum,int StartFrame,float Range,LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	int SetWalk(D3DXVECTOR3 vEye,D3DXVECTOR3 vLookAt,D3DXVECTOR3 vUp,D3DXQUATERNION quat);

private:
	
	std::vector <ANI_WALK_DATA> m_List;
	DWORD				 m_PreTime;
	int					 m_SelectAniNum;
	int					 m_Ani_Frame;
	//............................................................................................................



#ifdef BOUND_BOX_DISPLAY

public:

	BOOL					m_ISBoundCamera;

#endif


public:
	CBravoCamera();
	~CBravoCamera();

};

extern	CBravoCamera		g_Camera;

typedef	CBravoCamera		CCamera;	/// sooree terrain


#endif	