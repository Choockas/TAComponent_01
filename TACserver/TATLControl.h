// TATLControl.h : Declaration of the CTATLControl
#pragma once
#include <math.h>
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TACserver_i.h"
#include "_ITATLControlEvents_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;



// CTATLControl
class ATL_NO_VTABLE CTATLControl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CTATLControl, ITATLControl>,
	public IOleControlImpl<CTATLControl>,
	public IOleObjectImpl<CTATLControl>,
	public IOleInPlaceActiveObjectImpl<CTATLControl>,
	public IViewObjectExImpl<CTATLControl>,
	public IOleInPlaceObjectWindowlessImpl<CTATLControl>,
	public IConnectionPointContainerImpl<CTATLControl>,
	public CProxy_ITATLControlEvents<CTATLControl>,
	public ISpecifyPropertyPagesImpl<CTATLControl>,
	public IProvideClassInfo2Impl<&CLSID_TATLControl, &__uuidof(_ITATLControlEvents), &LIBID_TACserverLib>,
	public CComCoClass<CTATLControl, &CLSID_TATLControl>,
	public CComControl<CTATLControl>
{
public:


	CTATLControl()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_TATLCONTROL)


BEGIN_COM_MAP(CTATLControl)
	COM_INTERFACE_ENTRY(ITATLControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CTATLControl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
#ifndef _WIN32_WCE
	PROP_ENTRY_TYPE("FillColor", DISPID_FILLCOLOR, CLSID_StockColorPage, VT_UI4)
#endif
	// Example entries
	// PROP_ENTRY_TYPE("Property Name", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CTATLControl)
	CONNECTION_POINT_ENTRY(__uuidof(_ITATLControlEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CTATLControl)
	CHAIN_MSG_MAP(CComControl<CTATLControl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ITATLControl
public:
	HRESULT OnDraw(ATL_DRAWINFO& di);

	OLE_COLOR m_clrFillColor;
	// members added to wizards generated
	short m_nSides;
	POINT m_arrPoint[100];
	STDMETHOD(get_Sides)(short* pval);
	STDMETHOD(put_Sides)(short newval);
	void OnFillColorChanged()
	{
		ATLTRACE(_T("OnFillColorChanged\n"));
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_nSides = 3;
		m_clrFillColor = RGB(0, 0xFF, 0);
		return S_OK;
	}

	void FinalRelease()
	{
	}
public:
		void CalcPoints(const RECT& rc);
};

OBJECT_ENTRY_AUTO(__uuidof(TATLControl), CTATLControl)

// added by hand
void CTATLControl::CalcPoints(const RECT& rc)
{
	const double pi = 3.14159265358979;
	POINT   ptCenter;
	double  dblRadiusx = (rc.right - rc.left) / 2;
	double  dblRadiusy = (rc.bottom - rc.top) / 2;
	double  dblAngle = 3 * pi / 2;          // Start at the top
	double  dblDiff = 2 * pi / m_nSides;   // Angle each side will make
	ptCenter.x = (rc.left + rc.right) / 2;
	ptCenter.y = (rc.top + rc.bottom) / 2;

	// Calculate the points for each side
	for (int i = 0; i < m_nSides; i++)
	{
		m_arrPoint[i].x = (long)(dblRadiusx * cos(dblAngle) + ptCenter.x + 0.5);
		m_arrPoint[i].y = (long)(dblRadiusy * sin(dblAngle) + ptCenter.y + 0.5);
		dblAngle += dblDiff;
	}
}

HRESULT CTATLControl::OnDraw(ATL_DRAWINFO& di)
{
	RECT& rc = *(RECT*)di.prcBounds;
	HDC hdc = di.hdcDraw;

	COLORREF    colFore;
	HBRUSH      hOldBrush, hBrush;
	HPEN        hOldPen, hPen;

	// Translate m_colFore into a COLORREF type
	OleTranslateColor(m_clrFillColor, NULL, &colFore);

	// Create and select the colors to draw the circle
	hPen = (HPEN)GetStockObject(BLACK_PEN);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);

	// Create and select the brush that will be used to fill the polygon
	hBrush = CreateSolidBrush(colFore);
	SelectObject(hdc, hBrush);

	CalcPoints(rc);
	Polygon(hdc, &m_arrPoint[0], m_nSides);

	// Select back the old pen and brush and delete the brush we created
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	return S_OK;
}

// generated implement
//HRESULT CTATLControl::OnDraw(ATL_DRAWINFO& di)
//{
//
//	RECT& rc = *(RECT*)di.prcBounds;
//	// Set Clip region to the rectangle specified by di.prcBounds
//	HRGN hRgnOld = nullptr;
//	if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
//		hRgnOld = nullptr;
//	bool bSelectOldRgn = false;
//
//	HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
//
//	if (hRgnNew != nullptr)
//	{
//		bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
//	}
//
//	Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
//	SetTextAlign(di.hdcDraw, TA_CENTER | TA_BASELINE);
//	SetTextColor(di.hdcDraw, RGB(120, 2, 4));
//	LPCTSTR pszText = _T("TATLControl");
//#ifndef _WIN32_WCE
//	TextOut(di.hdcDraw,
//		(rc.left + rc.right) / 2,
//		(rc.top + rc.bottom) / 2,
//		pszText,
//		lstrlen(pszText));
//#else
//	ExtTextOut(di.hdcDraw,
//		(rc.left + rc.right) / 2,
//		(rc.top + rc.bottom) / 2,
//		ETO_OPAQUE,
//		nullptr,
//		pszText,
//		ATL::lstrlen(pszText),
//		nullptr);
//#endif
//
//	if (bSelectOldRgn)
//		SelectClipRgn(di.hdcDraw, hRgnOld);
//
//	DeleteObject(hRgnNew);
//
//	return S_OK;
//}