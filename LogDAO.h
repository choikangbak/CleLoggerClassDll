#pragma once
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <assert.h>
#include <pqxx/pqxx>
#include "Log.h"

class LogDAO
{	
private:
	pqxx::connection* m_pdbC;
	int AnsiToUTF8(const char* szSrc, char* strDest, int destSize);
	int UTF8ToAnsi(char* szSrc, char* strDest, int destSize);
	void createTypeLevelIfNotExists();
	void createTableLogIfNotExists();
	void alterDatabaseSetTimezoneToLocal();

public:
	LogDAO() {}
	bool initLogDao(std::string strPasswd);
	void insertLog(Log log);
	void closeConnection();
};