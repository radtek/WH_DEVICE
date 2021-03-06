#include "stdafx.h"
#include "SCSequence.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief     上位序列基类构造函数 

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCSequence::CSCSequence():CCommand()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCSequence::~CSCSequence()
{

}

//long CSCSequence:: Start()
//{
//	return SP_SUCCESS;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断该序列能否处理特定的命令

@param	CSCCommand *command 上位命令     

@retval     bool 是否能处理

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CSCSequence::CanAcceptSCCommand(CSCCommand *command)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      当特定的上位命令完成时的处理函数

@param      CSCCommand *command完成的命令

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSCSequence::OnSCCommandComplete(CSCCommand *command)
{

}