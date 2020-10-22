#ifndef		_____CHAR_ITEM_____
#define		_____CHAR_ITEM_____

class		CBravoItem
{
protected:

public:
	bool		Create();
	void		FinalCleanup();

	CBravoItem();
	~CBravoItem();
};

extern	CBravoItem	g_Item;


#endif