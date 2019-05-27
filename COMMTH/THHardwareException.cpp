#include "StdAfx.h"
#include "THHardwareException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	�ⲿ��д��Ӳ���쳣��
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)long lDetailID,      ģ������ϸ����
@param      (i)CString sFileName    �쳣����ʱ�ļ���
@param      (i)long lLineNo         �쳣����ʱ�к�
@param      (i)CString sMessage     ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHHardwareException::CTHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage)
    :CSysException(MODULE_ID, lDetailID, sFileName, lLineNo, sMessage)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHHardwareException::~CTHHardwareException()
{
}