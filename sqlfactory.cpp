#include "sqlfunctions.h"
//IFactory SqlFct


//Factory Mysql
//Mysql Factory
MysqlFactory::MysqlFactory(Connections Conn)
{
    m_Conn = Conn;
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
    return new MysqlQuery(m_Conn);
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
