#include "stdafx.h"
#include "SCConnectRequestReply.h"
#include "HeaderManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������֤������Ӧ����캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCConnectRequestReply::CSCConnectRequestReply():CSCResponseCommand()
{
	m_IsRemote = TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCConnectRequestReply::~CSCConnectRequestReply()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSCConnectRequestReply::IsValidCommand()
{
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������֤������
@param      ��
@retval     long     0 �ɹ�   < 0 ʧ��
*/
//////////////////////////////////////////////////////////////////////////
long CSCConnectRequestReply::ExecuteCommand()
{
	return __super::ExecuteCommand();

}


