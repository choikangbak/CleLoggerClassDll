#pragma once
#include <string>

class Log
{
private:
	std::string m_timestamp;
	std::string m_level;
	std::string m_message;

public:
	Log() {}
	Log(std::string timestamp, std::string level, std::string message) { m_timestamp = timestamp; m_level = level; m_message = message; }

	std::string getTimestamp() { return m_timestamp; }
	std::string getLevel() { return m_level; }
	std::string getMessage() { return m_message; }

	void setTimestamp(std::string timestamp) { m_timestamp = timestamp; }
	void setLevel(std::string level) { m_level = level; }
	void setMessage(std::string message) { m_message = message; }
	void setLog(std::string timestamp, std::string level, std::string message) { m_timestamp = timestamp; m_level = level; m_message = message; }
};