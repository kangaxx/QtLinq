#include "sqlfunctions.h"

/*********************************************************************************************************************************************************/
//                                                                 Factory's SQL Functions                                                               //
//                                                                          Begin                                                                        //
/*********************************************************************************************************************************************************/

//Mysql Functions
QSqlDatabase * MysqlConnect::iniDBConnect(const QString &Server, const QString &UserName, const QString &Password, const QString &Database, const QString &ConnName)
{
    if (ConnName.trimmed().isEmpty())
        throw "ConnName is Empty!";
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL",ConnName);
    db.setHostName(Server);
    db.setUserName(UserName);
    db.setPassword(Password);

    db.setDatabaseName(Database);

    return new QSqlDatabase(db);
}

bool MysqlErrorMsg::setMessage(const QString &ErrorMsg)
{
    this->pErrorMsg = &ErrorMsg;
    return true;
}

const QString * MysqlErrorMsg::lastError()
{
    return this->pErrorMsg;
}


MysqlQuery::MysqlQuery(Connections Conn)
{
    m_pDb = new QSqlDatabase;
    *m_pDb = QSqlDatabase::addDatabase("QMYSQL",Conn.ConnName);
    m_pDb->setHostName(Conn.Server);
    m_pDb->setUserName(Conn.User);
    m_pDb->setPassword(Conn.Passwd);
    m_pDb->setDatabaseName(Conn.Database);
}

QSqlQuery MysqlQuery::query(QSqlDatabase pDB, QString &SqlCmd, QString & sErr)
{
    try
    {
        pDB.open();
        if ((!pDB.isValid()) || (!pDB.isOpen()))
            throw pDB.lastError().text();
        QSqlQuery * query = new QSqlQuery(pDB);
        (*query).exec(SqlCmd);
        sErr = (*query).lastError().text();
        pDB.close();
        return (*query);
    }
    catch (QString exception)
    {
        throw exception;
    }
}

QSqlQuery MysqlQuery::query(QSqlDatabase pDB, QString &SqlCmd)
{
    pDB.open();
    QSqlQuery * query = new QSqlQuery(pDB);
    (*query).exec(SqlCmd);
    pDB.close();
    return (*query);
}

QSqlQuery MysqlQuery::query(QString &SqlCmd)
{
    QSqlQuery * query = new QSqlQuery;
    (*query).exec(SqlCmd);
    return (*query);
}

MysqlQuery::~MysqlQuery()
{
    if (m_pDb)
    {
        delete m_pDb;
        m_pDb = 0;
    }
}

LiTable& MysqlQuery::query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table)
{

    LiTable * result;   //should update later.
    result = &table;
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "SELECT ";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
            SqlCmd = SqlCmd + field.GetName() + ",";
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','
    SqlCmd = SqlCmd + " FROM " + c_TableName + " "; //add From Table subsentence
    //add WHERE subsentence to sqlcmd


    SqlCmd = SqlCmd + " WHERE 1=1 ";
    if (dc.GetConditionsByTableName(c_TableName).count()>0)
    {
        foreach (LiField field, dc.GetConditionsByTableName(c_TableName))
        {

            if (field.GetName().trimmed().isEmpty() || field.GetValue().trimmed().isEmpty())
                continue;

            if (field.GetAON() == OR)
                SqlCmd = SqlCmd + " OR " + field.GetName() + "="+ "'" + field.GetValue() + "'";
            else
                SqlCmd = SqlCmd + " AND "  + field.GetName() + "="+ "'" + field.GetValue() + "'";
        }
    }


    //Running Sqlcmd in Sql Database and return the result

    pDB.open();

    QSqlQuery Qry;
    Qry = query(pDB,SqlCmd);


    if (Qry.size()<0)
        throw QString("Run SQL query error , Command is : "+SqlCmd);


    QList<QSqlRecord> *lRcds = new QList<QSqlRecord>;

    while (Qry.next())
    {
        (*lRcds).append(Qry.record());
    }
    pDB.close();
    LiRecords * GetRcd = new LiRecords(*lRcds);

    table.SetRecords(GetRcd);

    return *result;
}

LiTable &MysqlQuery::query(LiDataContext &dc, LiTable &table)
{
    LiTable * result;   //should update later.
    result = &table;
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "SELECT ";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
            SqlCmd = SqlCmd + field.GetName() + ",";
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','
    SqlCmd = SqlCmd + " FROM " + c_TableName + " "; //add From Table subsentence
    //add WHERE subsentence to sqlcmd


    SqlCmd = SqlCmd + " WHERE 1=1 ";
    if (dc.GetConditionsByTableName(c_TableName).count()>0)
    {
        foreach (LiField field, dc.GetConditionsByTableName(c_TableName))
        {

            if (field.GetName().trimmed().isEmpty() || field.GetValue().trimmed().isEmpty())
                continue;

            if (field.GetAON() == OR)
                SqlCmd = SqlCmd + " OR " + field.GetName() + "="+ "'" + field.GetValue() + "'";
            else
                SqlCmd = SqlCmd + " AND "  + field.GetName() + "="+ "'" + field.GetValue() + "'";
        }
    }


    //Running Sqlcmd in Sql Database and return the result

    m_pDb->open();

    QSqlQuery Qry;
    Qry = query(*m_pDb,SqlCmd);


    if (Qry.size()<0)
        throw QString("Run SQL query error , Command is : "+SqlCmd);


    QList<QSqlRecord> *lRcds = new QList<QSqlRecord>;

    while (Qry.next())
    {
        (*lRcds).append(Qry.record());
    }
    m_pDb->close();
    LiRecords * GetRcd = new LiRecords(*lRcds);

    table.SetRecords(GetRcd);

    return *result;

}

int MysqlQuery::DoDel(QSqlDatabase pDB, LiDataContext &dc)
{
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "DELETE ";

    SqlCmd = SqlCmd + " FROM " + c_TableName + " "; //add From Table subsentence
    //add WHERE subsentence to sqlcmd


    SqlCmd = SqlCmd + " WHERE 1=1 ";
    if (dc.GetConditionsByTableName(c_TableName).count()>0)
    {
        foreach (LiField field, dc.GetConditionsByTableName(c_TableName))
        {

            if (field.GetName().trimmed().isEmpty() || field.GetValue().trimmed().isEmpty())
                continue;

            if (field.GetAON() == OR)
                SqlCmd = SqlCmd + " OR " + field.GetName() + "="+ "'" + field.GetValue() + "'";
            else
                SqlCmd = SqlCmd + " AND "  + field.GetName() + "="+ "'" + field.GetValue() + "'";
        }
    }


    //Running Sqlcmd in Sql Database and return the result

    pDB.open();

    QSqlQuery Qry;
    Qry = query(pDB,SqlCmd);

    pDB.close();

    return 1;
}

int MysqlQuery::DoDel(LiDataContext &dc)
{
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql Delete Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetConditionsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql Delete Error !";
    QString SqlCmd;
    SqlCmd = "DELETE ";

    SqlCmd = SqlCmd + " FROM " + c_TableName + " "; //add From Table subsentence
    //add WHERE subsentence to sqlcmd


    SqlCmd = SqlCmd + " WHERE 1=1 ";
    if (dc.GetConditionsByTableName(c_TableName).count()>0)
    {
        foreach (LiField field, dc.GetConditionsByTableName(c_TableName))
        {

            if (field.GetName().trimmed().isEmpty() || field.GetValue().trimmed().isEmpty())
                continue;

            if (field.GetAON() == OR)
                SqlCmd = SqlCmd + " OR " + field.GetName() + "="+ "'" + field.GetValue() + "'";
            else
                SqlCmd = SqlCmd + " AND "  + field.GetName() + "="+ "'" + field.GetValue() + "'";
        }
    }


    //Running Sqlcmd in Sql Database and return the result

    m_pDb->open();

    QSqlQuery Qry;
    Qry = query(*m_pDb,SqlCmd);

    m_pDb->close();

    return 1;
}

int MysqlQuery::DoInsert(LiDataContext &dc)
{
    int result=-1;
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "INSERT " + c_TableName + "(";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
            SqlCmd = SqlCmd + field.GetName() + ",";
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','
    SqlCmd = SqlCmd + ") VALUES(";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
            SqlCmd = SqlCmd + "'" + field.GetValue() + "',";
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','
    SqlCmd = SqlCmd + ")";
    //Running Sqlcmd in Sql Database and return the result

    m_pDb->open();

    QSqlQuery * Qry = new QSqlQuery(*m_pDb);
    if ((*Qry).exec(SqlCmd))
        result = 1;

    m_pDb->close();

    delete Qry;
    Qry = 0;

    return 1;
}

int MysqlQuery::DoInsert(QSqlDatabase pDB, LiDataContext &dc)
{
    int result=-1;
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "INSERT " + c_TableName + "(";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
            SqlCmd = SqlCmd + field.GetName() + ",";
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','
    SqlCmd = SqlCmd + ") VALUES(";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
            SqlCmd = SqlCmd + "'" + field.GetValue() + "',";
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','
    SqlCmd = SqlCmd + ")";
    //Running Sqlcmd in Sql Database and return the result

    pDB.open();

    QSqlQuery * Qry = new QSqlQuery(pDB);
    if ((*Qry).exec(SqlCmd))
        result = 1;

    pDB.close();

    delete Qry;
    Qry = 0;

    return 1;
}

int MysqlQuery::DoRemove(QSqlDatabase pDB, LiDataContext &dc)
{
    pDB.close();  //yes , I do this just because I hate warning.
    int result=0;

    return result;
}

int MysqlQuery::DoUpdate(QSqlDatabase pDB, LiDataContext &dc)
{
    int result=-1;
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "UPDATE " + c_TableName + " SET ";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if ( !field.GetName().trimmed().isEmpty())
        {
            SqlCmd = SqlCmd + field.GetName() + " = ";
            SqlCmd = SqlCmd + "'" + field.GetValue() + "',";
        }
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','

    SqlCmd = SqlCmd + " WHERE 1=1 ";
    if (dc.GetConditionsByTableName(c_TableName).count()>0)
    {
        foreach (LiField field, dc.GetConditionsByTableName(c_TableName))
        {

            if (field.GetName().trimmed().isEmpty() || field.GetValue().trimmed().isEmpty())
                continue;

            if (field.GetAON() == OR)
                SqlCmd = SqlCmd + " OR " + field.GetName() + "="+ "'" + field.GetValue() + "'";
            else
                SqlCmd = SqlCmd + " AND "  + field.GetName() + "="+ "'" + field.GetValue() + "'";
        }
    }
    //Running Sqlcmd in Sql Database and return the result
    pDB.open();

    QSqlQuery * Qry = new QSqlQuery(pDB);
    if ((*Qry).exec(SqlCmd))
        result = 1;
    pDB.close();

    delete Qry;
    Qry = 0;

    return 1;

}

int MysqlQuery::DoUpdate(LiDataContext &dc)
{
    int result=-1;
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString SqlCmd;
    SqlCmd = "UPDATE " + c_TableName + " SET ";
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if ( !field.GetName().trimmed().isEmpty())
        {
            SqlCmd = SqlCmd + field.GetName() + " = ";
            SqlCmd = SqlCmd + "'" + field.GetValue() + "',";
        }
    }
    SqlCmd = SqlCmd.left(SqlCmd.length()-1); // cut off last ch ','

    SqlCmd = SqlCmd + " WHERE 1=1 ";
    if (dc.GetConditionsByTableName(c_TableName).count()>0)
    {
        foreach (LiField field, dc.GetConditionsByTableName(c_TableName))
        {

            if (field.GetName().trimmed().isEmpty() || field.GetValue().trimmed().isEmpty())
                continue;

            if (field.GetAON() == OR)
                SqlCmd = SqlCmd + " OR " + field.GetName() + "="+ "'" + field.GetValue() + "'";
            else
                SqlCmd = SqlCmd + " AND "  + field.GetName() + "="+ "'" + field.GetValue() + "'";
        }
    }
    //Running Sqlcmd in Sql Database and return the result
    m_pDb->open();

    QSqlQuery * Qry = new QSqlQuery(*m_pDb);
    if ((*Qry).exec(SqlCmd))
        result = 1;
    m_pDb->close();

    delete Qry;
    Qry = 0;

    return 1;
}

//Sqllite Functions
QSqlDatabase * SqlliteConnect::iniDBConnect(const QString & Server,const QString & UserName,const QString & Password,const QString & Database,const QString & ConnName)
{
    throw "sqllite not finish!";
}

bool SqlliteErrorMsg::setMessage(const QString &ErrorMsg)
{
    throw "sqllite not finish!";
}

const QString * SqlliteErrorMsg::lastError()
{
    throw "sqllite not finish!";
}

QSqlQuery SqlliteQuery::query(QSqlDatabase pDB, QString &SqlCmd, QString &sErr)
{
    throw "sqllite not finish!";

}


QSqlQuery SqlliteQuery::query(QSqlDatabase pDB, QString &SqlCmd)
{
    throw "sqllite not finish!";
}

QSqlQuery SqlliteQuery::query(QString &SqlCmd)
{
    throw "sqllite not finish!";
}

LiTable &SqlliteQuery::query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table)
{
    throw "sqllite not finish!";
}

LiTable &SqlliteQuery::query(LiDataContext &dc, LiTable &table)
{
    throw "sqllite not finish!";
}



int SqlliteQuery::DoDel(QSqlDatabase pDB, LiDataContext &dc)
{
    throw "sqllite not finish!";
}

int SqlliteQuery::DoDel(LiDataContext &dc)
{
    throw "sqllite not finish!";
}

int SqlliteQuery::DoInsert(LiDataContext &dc)
{
    throw "sqllite not finish!";
}

int SqlliteQuery::DoInsert(QSqlDatabase pDB, LiDataContext &dc)
{
    throw "sqllite not finish!";
}

int SqlliteQuery::DoRemove(QSqlDatabase pDB, LiDataContext &dc)
{
    throw "sqllite not finish!";
}

int SqlliteQuery::DoUpdate(QSqlDatabase pDB, LiDataContext &dc)
{
    throw "sqllite not finish!";
}

int SqlliteQuery::DoUpdate(LiDataContext &dc)
{
    throw "SqlliteQuery not finish!";
}

// xml functions
QSqlDatabase * XmlConnect::iniDBConnect(const QString& Server, const QString &UserName, const QString & Password, const QString & Database, const QString & ConnName)
{
    return new LiXmlLinker(Database,ConnName);
}


bool XmlErrorMsg::setMessage(const QString &ErrorMsg)
{
    this->pErrorMsg = &ErrorMsg;
    return true;
}

const QString * XmlErrorMsg::lastError()
{
    return this->pErrorMsg;
}


XmlQuery::XmlQuery(Connections Conn)
{
    m_pErrMsg = new XmlErrorMsg;
    m_pLinker = new LiXmlLinker(Conn);
}

XmlQuery::XmlQuery(XmlErrorMsg *ErrMsg, LiXmlLinker *Linker)
{
    m_pErrMsg = ErrMsg;
    if (Linker)
        m_pLinker = Linker;
}

XmlQuery::~XmlQuery()
{
    if (m_pErrMsg)
    {
        delete m_pErrMsg;
        m_pErrMsg = 0;
    }
    if (m_pLinker)
    {
        delete m_pLinker;
        m_pLinker = 0;
    }
}


QSqlQuery XmlQuery::query(QSqlDatabase pDB, QString &SqlCmd)
{
    QSqlQuery query;
    return query;
}

QSqlQuery XmlQuery::query(QString &SqlCmd)
{
    QSqlQuery query;
    return query;
}

QSqlQuery XmlQuery::query(QSqlDatabase pDB, QString &SqlCmd, QString &sErr)
{
    QSqlQuery query;
    return query;
}

LiTable &XmlQuery::query(QSqlDatabase pDB, LiDataContext &dc, LiTable &table)
{
    throw "not finished this query redifinition now!";
}


LiTable& XmlQuery::query(LiDataContext &dc, LiTable &table)
{
    LiTable *Result;
    Result = &table;
    if (!m_pLinker || (!m_pLinker->GetFile()))
        throw "Linker error , function can not work!";
    xmlfunctions XmlFunc(m_pLinker->GetFile(),m_pLinker->GetDbName());

    //Analyse SQL command ,rount map: analyse sql conditions
    /***************************************************************************/
    //STEP1: Get table num and table name
    /***************************************************************************/
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,Mysql query Error !";
    QString TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(TableName).count() <= 0)
        throw "Sql parameters incomplete,mysql query Error !";
    /***************************************************************************/
    //STEP2 : if the table num is lesser than 2, go to step 3
    //        else , we should join all the records
    //        Can not allow any join now ,but we should finished inner join function soon.
    /***************************************************************************/
    if (TableNames.count() > 1)
        throw "The tables num is more than 1 ,pls do not it";



    /***************************************************************************/
    //STEP3 : Filter the Records by conditions
    /***************************************************************************/
    int ConditionsNum = dc.GetConditionsByTableName(TableName).count();
    LiField *Conditions = 0;
    if (ConditionsNum >= 1)
    {
        Conditions = new LiField[ConditionsNum];
        QList<LiField> ConditionList = dc.GetConditionsByTableName(TableName);
        int bg  = 0;
        int ed = ConditionsNum - 1;
        foreach (LiField t, ConditionList)
        {
            //OR conditions place at front of AND conditions
            if (t.GetAON()==OR || t.GetAON() == NOTOR) //OR VALUE
            {
                Conditions[bg] = t;
                bg ++ ;
            }
            else
            {
                Conditions[ed] = t;
                ed --;
            }
        }
    }

    QList<LiField> FieldList = dc.GetFieldsByTableName(TableName);
    QStringList FieldNames;
    foreach (LiField t, FieldList)
    {
        if (!t.GetName().isNull() && !t.GetName().trimmed().isEmpty())
            FieldNames << t.GetName();
    }
    XmlFunc.FltRecord(TableName,Conditions,ConditionsNum,FieldNames,*Result);

    /***************************************************************************/
    //STEP4 : Get the data with
    /***************************************************************************/
    return *Result;
}

int XmlQuery::DoInsert(QSqlDatabase pDB, LiDataContext &dc)
{
    return 0;// do nothing;
}

int XmlQuery::DoInsert(LiDataContext &dc)
{
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,DoInsert Error !";
    QString c_TableName = TableNames.at(0);
    if (dc.GetFieldsByTableName(c_TableName).count() <= 0)
        throw "Sql Parameters incomplete,DoInsert Error !";

    int FieldNum = dc.GetFieldsByTableName(c_TableName).count();
    if (FieldNum <= 0)
        return FieldNum;
    LiField Fields[FieldNum];
    int i = 0;
    foreach(LiField field,dc.GetFieldsByTableName(c_TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
        {
            Fields[i].SetName(field.GetName());
            Fields[i].SetValue(field.GetValue());
            i++;
        }
    }
    if (i != FieldNum)
        return -1;




    if (!m_pLinker || (!m_pLinker->GetFile()))
        throw "Linker error , function can not work!";
    xmlfunctions XmlFunc(m_pLinker->GetFile(),m_pLinker->GetDbName());
    XmlFunc.InsertRecord(m_pLinker->GetFile(),c_TableName,Fields,FieldNum);
    return 1;
}

int XmlQuery::DoDel(QSqlDatabase pDB, LiDataContext &dc)
{
    return 0;//do nothing;
}

int XmlQuery::DoDel(LiDataContext &dc)
{
    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,DoDel Error !";
    QString TableName = TableNames.at(0);
    if (dc.GetConditionsByTableName(TableName).count() <= 0)
        throw "Sql Parameters incomplete,DoDel Error !";
    int ConditionsNum = dc.GetConditionsByTableName(TableName).count();
    LiField *Conditions = 0;
    if (ConditionsNum >= 1)
    {
        Conditions = new LiField[ConditionsNum];
        QList<LiField> ConditionList = dc.GetConditionsByTableName(TableName);
        int bg  = 0;
        int ed = ConditionsNum - 1;
        foreach (LiField t, ConditionList)
        {
            //OR conditions place at front of AND conditions
            if (t.GetAON()==OR || t.GetAON() == NOTOR) //OR VALUE
            {
                Conditions[bg] = t;
                bg ++ ;
            }
            else
            {
                Conditions[ed] = t;
                ed --;
            }
        }
    }
    xmlfunctions XmlFunc(m_pLinker->GetFile(),m_pLinker->GetDbName());
    XmlFunc.DelRecord(m_pLinker->GetFile(),TableName,Conditions,ConditionsNum);
    return 1;
}

int XmlQuery::DoRemove(QSqlDatabase pDB, LiDataContext &dc)
{
    return 0;
}

int XmlQuery::DoUpdate(QSqlDatabase pDB, LiDataContext &dc)
{
    return 0;
}

int XmlQuery::DoUpdate(LiDataContext &dc)
{
    int Result = -1;

    QStringList TableNames;
    dc.GetTableNames(TableNames);
    if (TableNames.count()<=0)
        throw "Sql Parameters incomplete,DoUpdate Error !";
    QString TableName = TableNames.at(0);
    if (dc.GetConditionsByTableName(TableName).count() <= 0)
        throw "Sql Parameters incomplete,DoUpdate Error !";

    int FieldNum = dc.GetFieldsByTableName(TableName).count();
    if (FieldNum <= 0)
        return -1;
    LiField Fields[FieldNum];
    int i = 0;
    foreach(LiField field,dc.GetFieldsByTableName(TableName))
    {
        if (!field.GetName().trimmed().isEmpty())
        {
            Fields[i].SetName(field.GetName());
            Fields[i].SetValue(field.GetValue());
            i++;
        }
    }
    if (i != FieldNum)
        return -1;

    if (TableNames.count() > 1)
        throw "The tables num is more than 1 ,pls do not it";



    /***************************************************************************/
    //Filter the Records by conditions
    /***************************************************************************/
    int ConditionsNum = dc.GetConditionsByTableName(TableName).count();
    LiField *Conditions = 0;
    if (ConditionsNum >= 1)
    {
        Conditions = new LiField[ConditionsNum];
        QList<LiField> ConditionList = dc.GetConditionsByTableName(TableName);
        int bg  = 0;
        int ed = ConditionsNum - 1;
        foreach (LiField t, ConditionList)
        {
            //OR conditions place at front of AND conditions
            if (t.GetAON()==OR || t.GetAON() == NOTOR) //OR VALUE
            {
                Conditions[bg] = t;
                bg ++ ;
            }
            else
            {
                Conditions[ed] = t;
                ed --;
            }
        }
    }



    if (!m_pLinker || (!m_pLinker->GetFile()))
        throw "Linker error , function can not work!";
    xmlfunctions XmlFunc(m_pLinker->GetFile(),m_pLinker->GetDbName());
    Result = XmlFunc.UpdRecord(m_pLinker->GetFile(), TableName,Conditions,ConditionsNum,Fields,FieldNum);
    return Result;
}


/******************              Sql Factory codes END            ***********************/

