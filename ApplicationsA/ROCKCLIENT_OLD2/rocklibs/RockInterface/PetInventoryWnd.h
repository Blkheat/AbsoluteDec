// PetInventoryWnd.h: interface for the CPetInventoryWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__RUI_PETINVENTORY_WND_H__)
#define __RUI_PETINVENTORY_WND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/********************************************************************
	���κ�â������ �ܼ��� ������ ��Ŀ�� ���� ���� �޾Ƽ� ó���ؾ��Ѵ� .. 
	������ �ƴ� ���������� ó���Ѵ� .
	int m_CurSlotIndex;
*********************************************************************/


#include "WndProc.h"



class CPetInventoryWnd   : public CWndProc  
{
private:
	// �Ҹ�ǰ ������ ���� �Է½� �ʿ� 
	CSlot*				m_SrcSlot;			
	CSlot*				m_TargetSlot;

public:

	
	int c_MaxPetInvenNum;
	
	CFrameWnd* FrameWnd;
	CButton*		CencelBtn;
	CSlot*			PetInvenSlot[4];

	
	SPcDataInven*	PcInven;


	
	void SetSrcSlot(CSlot*		Src);
	void SetTargetSlot(CSlot*	Target);

	int m_CurSlotIndex;
	

	//==============================================================================
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	//==============================================================================


	void Open();
	void Close();

	
	
	void UpdatePetPcItem();
	

	void PetInvenSlotFocus( SEventMessage* EMsg );
	void PetInvenSlotClick( SEventMessage* EMsg );
	void PetInvenSlotDBClick( SEventMessage* EMsg );
	void PetInvenSlotDrag( SEventMessage* EMsg );
	void PetInvenSlotDrop( SEventMessage* EMsg );

	
	void CloseNumberInputWnd( int aNumItem ); 


	CPetInventoryWnd(){ Init();}
	virtual ~CPetInventoryWnd(){}

};


extern CPetInventoryWnd g_PetInventoryWnd;


#endif __RUI_PETINVENTORY_WND_H__
