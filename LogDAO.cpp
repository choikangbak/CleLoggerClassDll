#include "LogDAO.h"

void LogDAO::createTableLogIfNotExists()
{
	try
	{
		std::string_view query;
		query = "CREATE TABLE IF NOT EXISTS log"
			"("
			"id SERIAL PRIMARY KEY,"
			"timestamp TIMESTAMP NOT NULL,"
			"level LEVEL NOT NULL,"
			"message VARCHAR(255) NOT NULL,"
			"created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
			");";
		pqxx::work W(*m_pdbC);
		W.exec(query);
		W.commit();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void LogDAO::alterDatabaseSetTimezoneToLocal()
{
	try
	{
		std::string_view query;
		query = "ALTER DATABASE quickstartdb SET timezone TO 'Asia/Seoul';";
		pqxx::work W(*m_pdbC);
		W.exec(query);
		W.commit();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void LogDAO::createTypeLevelIfNotExists()
{
	try
	{
		std::string_view query;
		query = "DO $$ BEGIN IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'level') THEN "
			//"CREATE TYPE level AS ENUM('Trace', 'Debug', 'Info', 'Warning', 'Error', 'Critical', 'Off'); "
			"CREATE TYPE level AS ENUM('T', 'D', 'I', 'W', 'E', 'C', 'O'); "
			"END IF; END $$;";
		pqxx::work W(*m_pdbC);
		W.exec(query);
		W.commit();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void LogDAO::insertLog(Log log)
{
	try
	{
		int messageLength = log.getMessage().length() * 3;
		int queryLength = messageLength + 100;
		char* message = new char[messageLength];
		char* query = new char[queryLength];

		AnsiToUTF8(log.getMessage().c_str(), message, messageLength);

		sprintf_s(query, queryLength, "INSERT INTO log(timestamp, level, message) VALUES('%s', '%s', '%s');", log.getTimestamp().c_str(), log.getLevel().c_str(), message);

		pqxx::work W(*m_pdbC);
		W.exec(query);
		W.commit();

		delete[] message;
		delete[] query;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void LogDAO::closeConnection()
{
	if (m_pdbC) {
		m_pdbC->close();
		delete m_pdbC;
	}
}

int LogDAO::AnsiToUTF8(const char* szSrc, char* strDest, int destSize) // char* szSrc to const char* szSrc
{
	WCHAR* szUnicode = new WCHAR[destSize];
	char* szUTF8code = new char[destSize];

	int nUnicodeSize = MultiByteToWideChar(CP_ACP, 0, szSrc, (int)strlen(szSrc), szUnicode, destSize * sizeof(WCHAR)); 
	int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, szUnicode, nUnicodeSize, szUTF8code, destSize * sizeof(char), NULL, NULL);

	assert(destSize > nUTF8codeSize);

	memcpy(strDest, szUTF8code, nUTF8codeSize);
	strDest[nUTF8codeSize] = 0;
	delete[] szUnicode, szUTF8code;

	return nUTF8codeSize;
}

int LogDAO::UTF8ToAnsi(char* szSrc, char* strDest, int destSize)
{
	WCHAR szUnicode[255];
	char szAnsi[255];
	int nSize = MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, 0, 0);
	int nUnicodeSize = MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, szUnicode, nSize);
	int nAnsiSize = WideCharToMultiByte(CP_ACP, 0, szUnicode, nUnicodeSize, szAnsi, sizeof(szAnsi), NULL, NULL);
	assert(destSize > nAnsiSize);
	memcpy(strDest, szAnsi, nAnsiSize);
	strDest[nAnsiSize] = 0;
	return nAnsiSize;
}

bool LogDAO::initLogDao(std::string strPasswd)
{
	std::string connectionString;
	connectionString = "dbname =  postgres user = postgres password = " + strPasswd + " hostaddr = 127.0.0.1 port = 5432";
	m_pdbC = new pqxx::connection(connectionString);

	if (m_pdbC->is_open()) {
		std::cout << "pqxx::connection open succeed" << std::endl;
		createTypeLevelIfNotExists();
		createTableLogIfNotExists();
		alterDatabaseSetTimezoneToLocal();
		return true;
	}
	else {
		std::cout << "pqxx::connection open failed" << std::endl;
		return false;
	}
}