//................................................................................................................
//
// CFunctor.h																By wxywxy
//
//................................................................................................................
#ifndef __CFUNCTOR_H
#define __CFUNCTOR_H


class CFunctor
{
public:

	virtual void operator ()()=0;
};

template<class T>
class CMSGFunctor : public CFunctor
{

protected:
	
	T       *m_pObj;
	typedef void (T::*pFunc)();

	pFunc m_pFunc;

public:

	  CMSGFunctor() 
	  {
		m_pObj  = NULL;
		m_pFunc = NULL;
	  }

	 ~CMSGFunctor()
	 {
	 }

	void  operator ()()
	{
		if( m_pObj == NULL || m_pFunc == NULL  )
		{
			return;
		}
		
		T		*pTempObj  = m_pObj;
		pFunc    pTempFunc = m_pFunc;

		m_pObj  = NULL;
		m_pFunc = NULL;

		(pTempObj->*pTempFunc)();
	}

	void  Insert(T *pObj,pFunc pf)
	{
		m_pObj  = pObj;
		m_pFunc = pf;
	}

};

#endif