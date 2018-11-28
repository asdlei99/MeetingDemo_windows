/*##############################################################################
 * �ļ���ConfigParser.h
 * �����������ļ�����������
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include <string>


namespace demo
{

struct ClientConfig
{
	std::string strAppId;
	std::string strAppSecret;
	std::string strServerAddr;
	bool bUserDefine;
	std::string strUserAppId;
	std::string strUserAppSecret;
};

class CConfigParser
{
public:
	static CConfigParser& GetInstance();

	bool Init();
	void Serialize();

	ClientConfig& GetClientConfig() { return m_ClientConfig; }

private:
	// ���������͸�ֵ
	CConfigParser() {}
	CConfigParser(const CConfigParser&) {}
	CConfigParser& operator=(const CConfigParser&) {}

private:
	ClientConfig m_ClientConfig;
	std::string m_config_file;
};

}