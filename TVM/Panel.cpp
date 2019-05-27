#include "stdafx.h"
#include "Panel.h"
#include "basedlg.h"
#include "Bag.h"
#include "BitmapManager.h"

IMPLEMENT_DYNAMIC(CPanel,CStatic)
BEGIN_MESSAGE_MAP(CPanel,CStatic)
	 ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
/**
@brief      Panel���캯��

@param      (i)UIINFO& uIInfo ���ؼ���ʽ

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPanel::CPanel(UIINFO& uIInfo)
:CStatic()
{
	ASSERT( uIInfo.m_pParentWnd!=NULL);
	m_pBag = new CBag();
	this->m_UIInfo= uIInfo;
	calculatedFrameRect = false;
	m_ActualSize = DEFAULT_SIZE_EDIT;
	m_ActualLocation = CPoint(0,0);//uIInfo.m_Location;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Panel���캯��

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPanel::~CPanel()
{
	delete m_pBag;
	m_pBag = NULL;
}

CBag* CPanel::GetBag()
{
	return m_pBag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��Panel�ؼ�

@param      (i)Func<void (CPanel*)> * creating �ṩ�������ߵ�һ���ص������������ڵ���Create֮�����Panel�����Եȡ�

@retval     BOOL
				TRUE	�ɹ�
				FALSE	ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPanel::Create(Func<void (CPanel*)> * creating)
{
	if(creating!=NULL){
		creating->Invoke(this);
	}
	return Create();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��Panel�ؼ�

@param      void

@retval     BOOL
			TRUE	�ɹ�
			FALSE	ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPanel::Create()
{
	CalculateFrameRect(m_UIInfo.m_pParentWnd,m_UIInfo.m_DockStyle,m_UIInfo.m_Location);
	DWORD dwStyle = WS_CHILD  | ES_UPPERCASE | SS_OWNERDRAW;
	if(m_UIInfo.m_Visible)
	{
		dwStyle |= WS_VISIBLE;
	}
	if(!m_UIInfo.m_Enabled)
	{
		dwStyle |= WS_DISABLED;
	}
	return CStatic::Create(_T(""),dwStyle,GetFrameRect(),m_UIInfo.m_pParentWnd);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Childview������ɫ(��ֹ���Ƶ�ɫ)

@param      (i)CDC* pDC

@retval     BOOL    TRUE:��ֹ���Ƶ�ɫ; FALSE:�������Ƶ�ɫ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPanel::OnEraseBkgnd(CDC* pDC)
{
	BOOL bret=TRUE;;
	CRect  borderRect = GetContentRect();
	pDC->SetBkMode( m_UIInfo.m_BKMode);
	if(m_UIInfo.m_BKMode!=TRANSPARENT && m_UIInfo.m_pBackgroundImage)
	{
		CRect contentRect = borderRect;
		contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
		m_UIInfo.m_pBackgroundImage->Draw(pDC->m_hDC,contentRect);
		/*int nSaveDC = pDC->SaveDC();
		CDC pSrcDC;
		pSrcDC.CreateCompatibleDC(pDC);
		pSrcDC.SelectObject(m_UIInfo.m_pBackgroundImage);
		::StretchBlt(pDC->GetSafeHdc() , contentRect.left, contentRect.top,contentRect.Width(), contentRect.Height(), 
			pSrcDC.GetSafeHdc(), 0, 0,  m_UIInfo.m_pBackgroundImage->GetWidth(),m_UIInfo.m_pBackgroundImage->GetHeight() , SRCCOPY);
		pDC->RestoreDC(nSaveDC);*/
		bret=TRUE;
	}
	else if(m_UIInfo.m_BKMode!=TRANSPARENT && m_UIInfo.m_pBackgroundImage!=NULL)
	{
		CRect contentRect = borderRect;
		contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
		
		//CImage backgroundImage;
		//backgroundImage.Load(m_UIInfo.m_strBackgroundImage); 
		//if(!backgroundImage.IsNull()){
		//	ConvertPng(&backgroundImage);
		//	backgroundImage.Draw(pDC->m_hDC,contentRect);
		//}
		CImage* bkimage = m_UIInfo.m_pBackgroundImage;
		if (bkimage != NULL)
		{
			//ConvertPng(bkimage);
			bkimage->Draw(pDC->m_hDC,contentRect);
		}
		bret=TRUE;
	}
	else if(m_UIInfo.m_BKMode!=TRANSPARENT)
	{
		pDC->SelectStockObject(NULL_PEN);
		CBrush brush(m_UIInfo.m_BackColor);
		pDC->SelectObject(&brush);
		//pDC->Rectangle(borderRect);
		bret=TRUE;
	}
	return bret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�Panel����ʽ

@param      void

@retval     UIINFO ��ʽ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UIINFO CPanel::GetUIInfo()
{
	return m_UIInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Panel����ʽ

@param      (i)UIINFO& uIInfo ��������ʽ

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::SetUIInfo(UIINFO& uIInfo)
{
	if(m_UIInfo == uIInfo)
	{
		return;
	}
	calculatedFrameRect = false;
	m_UIInfo = uIInfo;
	if(!IsWindow(m_hWnd))
	{
		return;
	}

	this->DestroyWindow();
	this->Create();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ؼ����ƺ���

@param      (i)LPDRAWITEMSTRUCT lpDrawItemStruct 

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect =  lpDrawItemStruct->rcItem;
	CRect borderRect = rect;
	borderRect.left += m_UIInfo.m_Margins.cxLeftWidth;
	borderRect.right-=m_UIInfo.m_Margins.cxRightWidth;
	borderRect.top += m_UIInfo.m_Margins.cyTopHeight;
	borderRect.bottom -= m_UIInfo.m_Margins.cyBottomHeight;
	
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SetBkMode( m_UIInfo.m_BKMode);
	pDC->SetTextColor(m_UIInfo.m_ForeColor);
	CFont cFont;
	cFont.CreatePointFontIndirect(&m_UIInfo.m_Font);
	pDC->SelectObject(cFont);

	if (m_UIInfo.m_nParentType==1){
		CPen pen(PS_SOLID, 2,RGB(255,0,0));
		pDC->SelectObject(&pen);
	}
	else{
		CPen pen(PS_SOLID, m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderColor);
		if(m_UIInfo.m_BorderWidth>0)
		{
			pDC->SelectObject(&pen);
		}
		else
		{
			pDC->SelectObject(GetStockObject(NULL_PEN));
		}
	}
	pDC->SelectStockObject(NULL_BRUSH);
	//pDC->Rectangle(borderRect);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ÿؼ��Ŀ������

@param      void

@retval     CRect �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRect CPanel::GetFrameRect()
{
	return CRect(m_ActualLocation,m_ActualSize);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����DOCKSTYLE������ؼ���ʵ�ʴ�С��λ��

@param      void

@retval     CRect �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::CalculateFrameRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint location)
{
	if(calculatedFrameRect)
	{
		return;
	}

	calculatedFrameRect = true;
	//������ʾ��Ҫ��ȥͷ�����ֳ���
	m_ActualSize = dockStyle == TOPBOTTOM?CSize(m_UIInfo.m_Size.cx-170,m_UIInfo.m_Size.cy):m_UIInfo.m_Size;
	m_ActualLocation = location;
	CRect parentRectSrc;
	CRect parentContentRect;

	if(m_UIInfo.m_pParentWnd->IsKindOf(RUNTIME_CLASS(CPanel)))
	{
		CPanel* panel = reinterpret_cast<CPanel*>(m_UIInfo.m_pParentWnd);
		parentContentRect = panel->GetContentRect();
	}
	else{
		m_UIInfo.m_pParentWnd->GetWindowRect(&parentRectSrc);
		parentContentRect.left = 0;
		parentContentRect.top = 0;
		parentContentRect.right = parentRectSrc.Width();
		parentContentRect.bottom = parentRectSrc.Height();
	}
	
	
	
	switch(m_UIInfo.m_DockStyle)
	{
	case LEFT:
		{
			m_ActualLocation.x = parentContentRect.left + location.x;
			m_ActualLocation.y = parentContentRect.top;
			m_ActualSize.cy = parentContentRect.Height();
			m_ActualSize.cx > parentContentRect.Width() ? parentContentRect.Width() : m_ActualSize.cx;
			break;
		}
	case TOP:
		{
			m_ActualLocation.y = parentContentRect.top + location.y;
			m_ActualLocation.x = parentContentRect.left;
			m_ActualSize.cx = parentContentRect.Width();
			m_ActualSize.cy > parentContentRect.Height() ? parentContentRect.Height() : m_ActualSize.cx;
			break;
		}
	case RIGHT:
		{
			m_ActualLocation.x = parentContentRect.right - location.x - m_UIInfo.m_Size.cx;
			m_ActualLocation.y = parentContentRect.top;
			m_ActualSize.cy = parentContentRect.Height();
			m_ActualSize.cx > parentContentRect.Width() ? parentContentRect.Width() : m_ActualSize.cx;
			break;
		}
	case BOTTOM:
		{
			m_ActualLocation.y = parentContentRect.bottom - location.y - m_UIInfo.m_Size.cy;
			m_ActualLocation.x = parentContentRect.left;
			m_ActualSize.cx = parentContentRect.Width();
			m_ActualSize.cy > parentContentRect.Height() ? parentContentRect.Height() : m_ActualSize.cx;
			break;
		}
	case FILL:
		{
			m_ActualLocation = CPoint(parentContentRect.left ,parentContentRect.top);
			m_ActualSize = CSize(parentContentRect.Width(),parentContentRect.Height());
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ݿؼ�ʵ�ʻ������ݵ����򣨳����ⲹ�ף��ڲ��ף����߿�

@param      void

@retval     CRect ��������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRect CPanel::GetContentRect()
{
	CRect contentRect = CRect(CPoint(0,0),m_ActualSize);
	
	contentRect.left	+= (m_UIInfo.m_Margins.cxLeftWidth + m_UIInfo.m_Paddings.cxLeftWidth + m_UIInfo.m_BorderWidth);  
	contentRect.right	-= (m_UIInfo.m_Margins.cxRightWidth + m_UIInfo.m_Paddings.cxRightWidth + m_UIInfo.m_BorderWidth);  
	contentRect.top += (m_UIInfo.m_Margins.cyTopHeight + m_UIInfo.m_Paddings.cyTopHeight + m_UIInfo.m_BorderWidth);
	contentRect.bottom -= (m_UIInfo.m_Margins.cyBottomHeight + m_UIInfo.m_Paddings.cyBottomHeight + m_UIInfo.m_BorderWidth);
	return contentRect;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��д��Ϣ���������������ǰ��Ϣ������������ֵΪ-1ʱ����Ϣ����Ҫ·�ɵ������壬�����������Ϣ·�ɵ������塣

@param      (i)UINT message ��ϢID
@param      (i)WPARAM wParam ��Ϣ����
@param      (i)LPARAM lParam ��Ϣ����

@retval     LRESULT

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = __super::WindowProc(message,wParam,lParam);
	if(message>WM_USER && result>=0 &&  IsWindow(m_hWnd) && GetParent()!=NULL)
	{
		CWnd *pParent = GetParent();

		if(pParent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
		{
			((CBaseDlg*)pParent)->CallWndProc(message,wParam,lParam);
		}
		else if(pParent->IsKindOf(RUNTIME_CLASS(CPanel)))
		{
			((CPanel*)pParent)->CallWndProc(message,wParam,lParam);
		}
		else
		{
			pParent->SendMessage(message,wParam,lParam);
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ø����壨CBaseDlg���ͣ�

@param      void

@retval     CBaseDlg* ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBaseDlg* CPanel::GetParentDlg()
{
	if(IsWindow(this->m_hWnd))
	{
		CWnd* parent = GetParent();
		while(parent!=NULL && !parent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
		{
			parent = parent->GetParent();
		}
		return (CBaseDlg*)parent;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô��ڴ����������ƹ�Windows��Ϣ����

@param      (i)UINT message ��ϢID
@param      (i)WPARAM wParam ��Ϣ����
@param      (i)LPARAM lParam ��Ϣ����

@retval     LRESULT

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPanel::CallWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowProc(message,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ͼƬ͸������

@param      CImage* image

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::ConvertPng(CImage* image)
{
	for(int i = 0; i < image->GetWidth(); i++){  
		for(int j = 0; j < image->GetHeight(); j++){  
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(image->GetPixelAddress(i , j));  
			pucColor[0] = pucColor[0] * pucColor[3] / 255;  
			pucColor[1] = pucColor[1] * pucColor[3] / 255;  
			pucColor[2] = pucColor[2] * pucColor[3] / 255;  
		}  
	}  
}
BOOL CPanel::DrawTransprentDlg(HWND hWnd,BYTE transprt)
{
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//SetWindowsLong����������Ϊ�㼶����
	DWORD dwExStyle = GetWindowLong(hWnd,GWL_EXSTYLE);//��ȡ������Ϣ
	SetWindowLong(hWnd,GWL_EXSTYLE,dwExStyle|0x80000);//ȥ����ǰ����һ������,ԭ���Ժ�

	//����͸��ɫ��
	//��SetLayeredWindowAttributes����͸��ɫΪ0������UpdateLayeredWindow��ʹ��Ҫ��Щ
	HMODULE hInst= LoadLibrary(_T("User32.DLL"));//���ض�̬���ӿ� 

	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);//���庯��ָ������ 
	MYFUNC SetLayeredWindowAttributes = NULL;//����һ���ú���ָ�����͵ı��� 

	//�õ���̬���ӿ��SetLayeredWindowAttributes��������SetLayeredWindowAttributesָ���� 
	SetLayeredWindowAttributes = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");

	SetLayeredWindowAttributes(hWnd,0,transprt,2);//����SetLayeredWindowAttributes���� 
	//SetLayeredWindowAttributes(hWnd,0,transprt,ULW_ALPHA);
	FreeLibrary(hInst);//�ͷ�DLL 
	return true;
}