#include "RockPCH.h"

#include	<windows.h>
#include	"quadlist.h"

void	node_idx::cleanup()
{
	pprev->pnext = pnext;
	if ( pnext )
		pnext->pprev	= pprev;
	else
		pprev->pnext	= NULL;

	pprev	= NULL;
	pnext	= NULL;
}

node_idxlist::node_idxlist()
{
}

void	node_idxlist::init()
{
	pheader	= SAFE_NEW( node_idx );	
	pheader->pnext	= NULL;
	pheader->pprev	= NULL;
}

node_idxlist::~node_idxlist()
{
}

void	node_idxlist::cleanup()
{
	node_idx *	plink	= pheader;
	node_idx *	ptemp;

	while ( plink )
	{
		ptemp	= plink->pnext;
		plink->pprev	= NULL;
		if ( plink )
		{
			SAFE_DELETE( plink );			
		}
		plink	= ptemp;
	}
}

void	node_idxlist::addtail(node_idx* node)
{
	node_idx *	plink	= pheader;
	
	while ( plink )
	{
		if ( plink->pnext == NULL )
		{
			plink->pnext	= node;
			node->pprev		= plink;
			node->pnext		= NULL;
			break;
		}
		plink	= plink->pnext;
	}
}
