// TATLControl.cpp : Implementation of CTATLControl
#include "stdafx.h"
#include "TATLControl.h"


// CTATLControl

STDMETHODIMP CTATLControl::get_Sides(short* pVal)
{
	// method added to wizards generated
	*pVal = m_nSides;

	return S_OK;
}

STDMETHODIMP CTATLControl::put_Sides(short newVal)
{
	// method added to wizards generated
	if (2 < newVal && newVal < 101)
	{
		m_nSides = newVal;
		return S_OK;
	}
	else
	{
		return Error(_T("Shape must have between 3 and 100 sides"));
	}
}