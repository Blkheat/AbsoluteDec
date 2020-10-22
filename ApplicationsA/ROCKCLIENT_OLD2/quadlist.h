
#ifndef	_____node______
#define	_____node______
/****************************************************************************************************
	* static node
****************************************************************************************************/

class	node_idx
{
public:
	short		nidx;
	short		nsidx;
	node_idx *	pprev;
	node_idx *	pnext;
	void		cleanup();
};

class	node_idxlist
{
public:
	node_idx *	pheader;

	void		addtail(node_idx* node);
	void		cleanup();
	void		init();

	node_idxlist();
	~node_idxlist();
};


#endif