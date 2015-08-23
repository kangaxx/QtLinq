#include "sqlfunctions.h"
//IFactory SqlFct


//Factory Mysql
//Mysql Factory
using namespace std;
MysqlFactory::MysqlFactory(Connections Conn)
{
    m_Conn = Conn;
    m_db = new QSqlDatabase;
    *m_db = QSqlDatabase::addDatabase("QMYSQL",m_Conn.ConnName);
    m_db->setHostName(Conn.Server);
    m_db->setUserName(Conn.User);
    m_db->setPassword(Conn.Passwd);
    m_db->setDatabaseName(Conn.Database);
}

MysqlFactory::~MysqlFactory()
{
    cout << "Do sqlfac desctruct!" << endl;
    if (m_db != 0){
        if (m_db->isOpen())
            m_db->close();
        m_db->removeDatabase(m_Conn.ConnName);
        delete m_db;
        m_db = 0;
    }
}


ISqlConnect * MysqlFactory::sqlConnectFct()
{
    return new MysqlConnect;
}


ISqlErrorMsg * MysqlFactory::sqlErrFct()
{
    return new MysqlErrorMsg;
}

ISqlQuery * MysqlFactory::sqlQryFct()
{
    return new MysqlQuery(m_db);
}


//Factory Sqllite
ISqlConnect * SqlliteFactory::sqlConnectFct()
{
    return new SqlliteConnect;
}

ISqlErrorMsg * SqlliteFactory::sqlErrFct()
{
    return new SqlliteErrorMsg;
}

ISqlQuery * SqlliteFactory::sqlQryFct()
{
    return new SqlliteQuery;
}


//Factory XML
XmlFactory::XmlFactory(Connections Conn)
{
    m_Conn = Conn;
}

XmlFactory::~XmlFactory()
{

}

ISqlConnect * XmlFactory::sqlConnectFct()
{
    return new XmlConnect;
}

ISqlErrorMsg * XmlFactory::sqlErrFct()
{
    return new XmlErrorMsg;
}

ISqlQuery * XmlFactory::sqlQryFct()
{
    return new XmlQuery(m_Conn);
}


/////////////////////////////////////////////////////
// Sql base factory system below                   //
// create by gxx 2013 10 14                        //
/////////////////////////////////////////////////////
/// \brief SqlFunctions::SqlFunctions

SqlFunctions::SqlFunctions()
{
    //donothing
}


SqlFunctions::~SqlFunctions()
{

}


FSqlFactory & SqlFunctions::Create(eFctType type, Connections Conn, FSqlFactory** SqlDest)
{
    FSqlFactory * result;
    switch (type) {
    case MYSQL:
        *SqlDest = new MysqlFactory(Conn);
        break;
    case SQLLITE:
        *SqlDest = new SqlliteFactory;
        break;
    case XML:
        *SqlDest = new XmlFactory(Conn);
        break;

    default:
        *SqlDest = new MysqlFactory(Conn);
        break;
    }
    result = *SqlDest;

    return *result;
}

void SqlFunctions::GetSqlLink(QString iniFilePath, QString iniFileName, QString , QString connName, Connections &conn)
{
    string fileName = iniFilePath.toStdString() + "/" + iniFileName.toStdString();
    conn.Server = QString::fromStdString(BaseFunctions::GetParaByLine(fileName,1));
    conn.User = QString::fromStdString(BaseFunctions::GetParaByLine(fileName,2));
    conn.Passwd = QString::fromStdString(BaseFunctions::GetParaByLine(fileName,3));
    conn.Database = QString::fromStdString(BaseFunctions::GetParaByLine(fileName,4));
    conn.ConnName = connName;
}

