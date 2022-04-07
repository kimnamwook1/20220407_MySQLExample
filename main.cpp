#include <iostream>
#include <Windows.h>


#include "jdbc/mysql_connection.h"
#include "jdbc/cppconn/driver.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/resultset.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/prepared_statement.h"

#ifdef _DEBUG
#pragma comment(lib, "debug/mysqlcppconn.lib")
#else
#pragma comment(lib, "mysqlcppconn.lib")
#endif

using namespace std;

std::string Utf8ToMultiByte(std::string utf8_str);

int main()
{
	try
	{
		sql::Driver* driver; //workbench
		sql::Connection* connection; //접속
		sql::Statement* statement; //query
		sql::ResultSet* resultset; //결과화면
		sql::PreparedStatement* preparedStatement;

		//work벤치 켜기
		driver = get_driver_instance();

		//연결 버튼 누르기
		connection = driver->connect("tcp://work.junios.net:33306", "ygl", "ygl123");

		if (connection == nullptr)
		{
			cout << "connect failed" << endl;
			exit(-1);
		}

		//사용 데이터베이스 선정(use)
		connection->setSchema("ygl");

		//preparedStatement = connection->prepareStatement("insert into guestbook ( name, memo ) value ( ?, ? )");
		//preparedStatement->setString(1, "junios");
		//preparedStatement->setString(2, "hello");
		//preparedStatement->execute();

		//query
		statement = connection->createStatement();
		resultset = statement->executeQuery("select * from guestbook");

		for (; resultset->next();)
		{
			cout << resultset->getInt("idx") << " : " <<
				Utf8ToMultiByte(resultset->getString("name")) << " : " <<
				Utf8ToMultiByte(resultset->getString("memo")) << endl;
		}

		delete resultset;
		delete statement;
		delete connection;
	}
	catch (sql::SQLException e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}



	return 0;
}


/* * @brief 웹에서 사용하는 utf-8 인코딩을 window에서 사용하는 ANSI 인코딩으로 변경 합니다. */
std::string Utf8ToMultiByte(std::string utf8_str)
{
	std::string resultString; char* pszIn = new char[utf8_str.length() + 1];
	strncpy_s(pszIn, utf8_str.length() + 1, utf8_str.c_str(), utf8_str.length());
	int nLenOfUni = 0, nLenOfANSI = 0; wchar_t* uni_wchar = NULL;
	char* pszOut = NULL;
	// 1. utf8 Length
	if ((nLenOfUni = MultiByteToWideChar(CP_UTF8, 0, pszIn, (int)strlen(pszIn), NULL, 0)) <= 0)
		return nullptr;
	uni_wchar = new wchar_t[nLenOfUni + 1];
	memset(uni_wchar, 0x00, sizeof(wchar_t)*(nLenOfUni + 1));
	// 2. utf8 --> unicode
	nLenOfUni = MultiByteToWideChar(CP_UTF8, 0, pszIn, (int)strlen(pszIn), uni_wchar, nLenOfUni);
	// 3. ANSI(multibyte) Length
	if ((nLenOfANSI = WideCharToMultiByte(CP_ACP, 0, uni_wchar, nLenOfUni, NULL, 0, NULL, NULL)) <= 0)
	{
		delete[] uni_wchar; return 0; 
	}
	pszOut = new char[nLenOfANSI + 1];
	memset(pszOut, 0x00, sizeof(char)*(nLenOfANSI + 1));
	// 4. unicode --> ANSI(multibyte)
	nLenOfANSI = WideCharToMultiByte(CP_ACP, 0, uni_wchar, nLenOfUni, pszOut, nLenOfANSI, NULL, NULL);
	pszOut[nLenOfANSI] = 0;
	resultString = pszOut;
	delete[] uni_wchar;
	delete[] pszOut;
	return resultString;
}

