#include "sqlfunctions.h"
//IFactory SqlFct


//Factory Mysql
//Mysql Factory
using namespace std;


MysqlFactory::MysqlFactory(Connections Conn)
{
    setConn(Conn);
}

MysqlFactory::~MysqlFactory()
{
    if (m_db != 0){
        m_db->close();
        delete m_db;
        QSqlDatabase::removeDatabase(m_Conn.ConnName);

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
    setConn(Conn);
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

FSqlFactory &SqlFunctions::Create(eFctType type, QString iniFile, QString Path)
{

    string filePath = BaseFunctions::combineFilePath(Path.toStdString(),iniFile.toStdString());
    return Create(type,filePath);

}

FSqlFactory &SqlFunctions::Create(eFctType type, string filePath)
{

    Connections conn;
    conn.Server = QString::fromStdString(BaseFunctions::GetParaByName(filePath,CHARS_CONNECTIONS_ELEMENTNAME_SERVER));
    conn.Database = QString::fromStdString(BaseFunctions::GetParaByName(filePath,CHARS_CONNECTIONS_ELEMENTNAME_DATABASE));
    conn.User =  QString::fromStdString(BaseFunctions::GetParaByName(filePath,CHARS_CONNECTIONS_ELEMENTNAME_USERNAME));
    conn.Passwd = QString::fromStdString(BaseFunctions::GetParaByName(filePath,CHARS_CONNECTIONS_ELEMENTNAME_PASSWORD));
    switch(type){
    case XML:
        conn.ConnName = QString::fromStdString(BaseFunctions::GetParaByName(filePath,CHARS_CONNECTIONS_ELEMENTNAME_CONNNAME));
        break;
    case MYSQL:
        conn.ConnName = CHARS_CONNECTIONS_STATIC_CONNATION_NAME;
        break;
    default:
        throw "ERROR: create sql link fail, unkown fct type!";
    }
    return Create(type,conn);
}

FSqlFactory &SqlFunctions::Create(eFctType type, Connections conn)
{
    static FSqlFactory *rst;
    switch(type){
    case XML:
        rst = new XmlFactory(conn);
        break;
    case MYSQL:
        rst = new MysqlFactory(conn);
        break;
    default:
        throw "ERROR: create sql link fail, unkown fct type!";
    }
    return *rst;
}


FSqlFactory & SqlFunctions::Create(eFctType type, Connections Conn, FSqlFactory** SqlDest)
{
//    FSqlFactory * result;
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
//    result = *SqlDest;

    return **SqlDest;
//    return *result;
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

LiResultList SqlFunctions::rcdToLiResult(LiTable &table,const QStringList &fieldName)
{
    LiResultList rst;
    int rcdCount = table.GetRecords().GetCount();
    if (rcdCount == 0)
        return rst;
    rst.setName(fieldName);
    for (int i = 0;i<rcdCount;i++){
        rst.insert(table.GetRecords().NextFields(fieldName));
    }
    return rst;
}

int SqlFunctions::listToFields(LiConditionList &values, LiField *fields)
{
    if (fields == NULL)
        throw QString("Error,condtionListToFields fail , arg fields is NULL");
    int i=0;
    for (; i < values.m_values.size()/2;i++){
        fields[i].SetAON(values.getAON(values,i));
        fields[i].SetSo(values.getOperator(values,i));
        fields[i].SetName(values.getFieldName(values,i));
        fields[i].SetValue(values.getFieldValue(values,i));
    }
    return i;// careful! i is m_values.size()/2 now
}


