/*##############################################################################
 * �ļ���ConfigParser.cpp
 * �����������ļ�������ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "ConfigParser.h"
#include <atlconv.h>
#include "tinyxml2.h"

using namespace demo;

#define MAX_CHAR_SIZE 128

namespace
{

TCHAR* GetModulePath(HMODULE hModule)
{
	TCHAR  szPath[MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];

	GetModuleFileName(hModule, szPath, MAX_PATH);

	_tsplitpath(szPath, szDrive, szDir, NULL, NULL);

	_stprintf(szPath, _T("%s%s"), szDrive, szDir);

	static TCHAR  szModuelPath[MAX_PATH];

	return _tcscpy(szModuelPath, szPath);
}

}

/*------------------------------------------------------------------------------
 * ��  ������̬��������ȡ��̬ʵ��
 * ��  ������
 * ����ֵ�����ý�����ʵ������
 ------------------------------------------------------------------------------*/
CConfigParser& CConfigParser::GetInstance()
{
	static CConfigParser configer;
	return configer;
}

/*------------------------------------------------------------------------------
 * ��  ������ʼ��
 * ��  ������
 * ����ֵ���ɹ�/ʧ��
 ------------------------------------------------------------------------------*/
bool CConfigParser::Init()
{
	WCHAR* module_path = GetModulePath(NULL);

	USES_CONVERSION;
	std::string strConfigFile = T2A(module_path) + std::string("appinfo.xml");

	tinyxml2::XMLDocument doc;
	doc.LoadFile(strConfigFile.c_str());
	if (doc.ErrorID() != 0)
		return false;

	const char* szAppId = doc.FirstChildElement("AppId")->GetText();
	if (!szAppId) 
		return false;
	else
		m_ClientConfig.strAppId = szAppId;

	tinyxml2::XMLElement* serverAddrElement = doc.FirstChildElement("ServerAddr");
	const char* szServerAddr = serverAddrElement ? serverAddrElement->GetText() : "";
	if (!szServerAddr) // ��������ServerAddr
		m_ClientConfig.strServerAddr = "";
	else
		m_ClientConfig.strServerAddr = szServerAddr;
	
	const char* szAppSecret = doc.FirstChildElement("AppSecret")->GetText();
	if (!szAppSecret)
		return false;
	else
		m_ClientConfig.strAppSecret = szAppSecret;

	return true;
}

