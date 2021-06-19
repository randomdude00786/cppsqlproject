/* Copyright 2008, 2010, Oracle and/or its affiliates.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

There are special exceptions to the terms and conditions of the GPL
as it is applied to this software. View the full text of the
exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
software distribution.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

int main(void)
{
    cout << endl;
    cout << "Let's have MySQL count from 10 to 1..." << endl;

    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
        /* Connect to the MySQL test database */
        if (con) {
            cout << "connected"<<endl;
        }
        
        stmt = con->createStatement();
        stmt->execute("USE sys");
        stmt->execute("DROP TABLE IF EXISTS test");
        stmt->execute("CREATE TABLE test(id INT,label VARCHAR(255))");
        delete stmt;

        /* '?' is the supported placeholder syntax */
        pstmt = con->prepareStatement("INSERT INTO test(id,label) VALUES (?,?)");
        for (int i = 1; i <= 10; i++){
            pstmt->setInt(1, i);
            pstmt->setString(2, string(1, char(i+64)));
            pstmt->executeUpdate();
        }
        delete pstmt;
        /*pstmt = con->prepareStatement("INSERT INTO test(label) VALUES (?)");
        for (char i = 'a'; i <= 'j'; i++) {
            pstmt->setString(2, string(1,i));
            pstmt->executeUpdate();
        }
        delete pstmt;*/

        /* Select in ascending order */
        pstmt = con->prepareStatement("SELECT id,label FROM test ORDER BY id ASC");
        res = pstmt->executeQuery();

        /* Fetch in reverse = descending order! */
        while (res->next()) {
            cout << "\t... MySQL counts: " << res->getInt("id") << "-" << res->getString("label") << endl;
        }
        delete res;
        /*pstmt = con->prepareStatement("SELECT label FROM test");
        res = pstmt->executeQuery();
        while (res->next()) {
            cout << res->getString(1);
        }
        delete res;*/
        delete pstmt;
        delete con;

    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<" )" << endl;
    }

    cout << endl;

    return EXIT_SUCCESS;
}