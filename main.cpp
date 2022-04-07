#include <iostream>



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

		preparedStatement = connection->prepareStatement("insert into guestbook ( name, memo ) value ( ?, ? )");
		preparedStatement->setString(1, "junios");
		preparedStatement->setString(2, "hello");
		preparedStatement->execute();

		//query
		statement = connection->createStatement();
		resultset = statement->executeQuery("select * from guestbook");

		for (; resultset->next();)
		{
			cout << resultset->getInt("idx") << " : " <<
				resultset->getString("name") << " : " << resultset->getString("memo") << endl;
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