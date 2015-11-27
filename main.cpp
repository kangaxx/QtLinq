#include <iostream>
#include "sqlfunctions.h"
#include <QtCore>
#define CHARS_UNIT_TEST_INIFILE "sqlfunc_ut.ini"

#define CHARS_UNIT_TABLE_SIMPLE "ut_simple_tb"
#define CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID "id"
#define CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME "name"
#define CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE "value"
#define CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE "date"

using namespace std;

void ut_sqlfunction_create_mysql_mode2015_query01()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    QStringList fields;
    fields << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE;
    LiResultList rst = sqlfct->sqlQryFct()->query(fields,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    int i = 0;
    for (; i <rst.getCount(); i++){
        qDebug() << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID))
        << " | " << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE));
    }
    if (i == 0)
        qDebug() << QString("warning: no record!");
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_query02()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    QStringList fields;
    fields << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE;
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME << "1" << liIsNULL << AND;
    LiResultList rst = sqlfct->sqlQryFct()->query(fields,LiTableName(CHARS_UNIT_TABLE_SIMPLE),&cl);
    int i = 0;
    for (; i <rst.getCount(); i++){
        qDebug() << "query 2015_02(is null):" << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID))
        << " | " << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE));
    }
    if (i == 0)
        qDebug() << QString("warning: no record!");
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_query03()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    QStringList fields;
    fields << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE;
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME << "1" << liNotNULL << AND;
    LiResultList rst = sqlfct->sqlQryFct()->query(fields,LiTableName(CHARS_UNIT_TABLE_SIMPLE),&cl);
    int i = 0;
    for (; i <rst.getCount(); i++){
        qDebug() << "query 2015_03(is not null):" << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID))
        << " | " << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE));
    }
    if (i == 0)
        qDebug() << QString("warning: no record!");
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_query04()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    QStringList fields;
    fields  << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE
           << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE;
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME << "name%" << liLike << AND;
    LiResultList rst = sqlfct->sqlQryFct()->query(fields,LiTableName(CHARS_UNIT_TABLE_SIMPLE),&cl);
    int i = 0;
    for (; i <rst.getCount(); i++){
        qDebug() << "query 2015_04(like):" << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID))
        << " | " << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE))
        << " | "  << rst.at(i,QString::fromLatin1(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE));
    }
    if (i == 0)
        qDebug() << QString("warning: no record!");
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_insert01()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID << "1" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE << "2015-01-01" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME << "name01" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "1" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoInsert(cl,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_insert result is : " << rst;
    delete sqlfct;

}


void ut_sqlfunction_create_mysql_mode2015_insert02()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID << "2" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE << "2015-01-01" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME << "name02" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "2" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoInsert(cl,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_insert02 result is : " << rst;
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_insert03()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID << "3" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE << "2015-01-01" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME << "name03" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "3" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoInsert(cl,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_insert03 result is : " << rst;
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_insert04()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID << "4" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE << "2015-01-01" << liEqual << AND
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "4" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoInsert(cl,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_insert result is : " << rst;
    delete sqlfct;

}

void ut_sqlfunction_create_mysql_mode2015_delete01()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList cl;
    cl << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID << "1" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoDel(cl,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_delete01() result is : " << rst;
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_delete_all()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));

    int rst = sqlfct->sqlQryFct()->DoDel(LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_delete_all() result is : " << rst;
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_update01()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList value,cond;
    value << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "2" << liEqual << AND;
    cond << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "1" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoUpdate(value,cond,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_update01() result is : " << rst;
    delete sqlfct;
}

void ut_sqlfunction_create_mysql_mode2015_update_all()
{
    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));
    LiConditionList value;
    value << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE << "2" << liEqual << AND;
    int rst = sqlfct->sqlQryFct()->DoUpdate(value,LiTableName(CHARS_UNIT_TABLE_SIMPLE));
    qDebug() << "ut_sqlfunction_create_mysql_mode2015_update_all() result is : " << rst;
    delete sqlfct;
}




void ut_sqlfunction_create_mysql_mode2014_query01()
{

    FSqlFactory *sqlfct;

    sqlfct = &(SqlFunctions::Create(MYSQL,QString::fromLatin1(CHARS_UNIT_TEST_INIFILE),
                                    QString::fromStdString(BaseFunctions::getConfigPath())));

    LiField fieldName,fieldValue,fieldId,fieldDate;
    fieldName.SetName(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME);
    fieldValue.SetName(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE);
    fieldId.SetName(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID);
    fieldDate.SetName(CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_DATE);

    LiDataContext dc;
    dc.SetTableName(QString(CHARS_UNIT_TABLE_SIMPLE));
    dc.AddField(&fieldName);
    dc.AddField(&fieldValue);
    dc.AddField(&fieldId);
    dc.AddField(&fieldDate);

    LiTable rst;
    sqlfct->sqlQryFct()->query(dc,rst);
    QStringList ls;
    ls << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_ID << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_NAME
       << CHARS_UNIT_TABLE_SIMPLE_FIELDNAME_VALUE;
    for(int i = 0 ; i< rst.GetRecords().GetCount(); i++){
         qDebug() << rst.GetRecords().NextFields(ls);
    }

    delete sqlfct;
}



int main()
{
    try
    {
//        ut_sqlfunction_create_mysql_mode2015_insert01();
//        ut_sqlfunction_create_mysql_mode2015_query01();
//        ut_sqlfunction_create_mysql_mode2015_update01();
//        ut_sqlfunction_create_mysql_mode2015_query01();

//        ut_sqlfunction_create_mysql_mode2015_delete01();

//        ut_sqlfunction_create_mysql_mode2015_insert01();
//        ut_sqlfunction_create_mysql_mode2015_insert02();
//        ut_sqlfunction_create_mysql_mode2015_insert03();
//        ut_sqlfunction_create_mysql_mode2015_query01();
//        ut_sqlfunction_create_mysql_mode2015_update_all();
//        ut_sqlfunction_create_mysql_mode2015_query01();

//        ut_sqlfunction_create_mysql_mode2015_delete_all();
//        ut_sqlfunction_create_mysql_mode2015_query01();
//        ut_sqlfunction_create_mysql_mode2015_insert01();
//        ut_sqlfunction_create_mysql_mode2015_insert02();
//        ut_sqlfunction_create_mysql_mode2015_insert03();
//        ut_sqlfunction_create_mysql_mode2015_query01();

        /**************test query 2015_0[2-4]***************/
        ut_sqlfunction_create_mysql_mode2015_delete_all();
        ut_sqlfunction_create_mysql_mode2015_insert01();
        ut_sqlfunction_create_mysql_mode2015_insert02();
        ut_sqlfunction_create_mysql_mode2015_insert03();
        ut_sqlfunction_create_mysql_mode2015_insert04();
        ut_sqlfunction_create_mysql_mode2015_query02();
        ut_sqlfunction_create_mysql_mode2015_query03();
        ut_sqlfunction_create_mysql_mode2015_query04();
    }
    catch(QString err)
    {
        qDebug() << err;
    }
    catch(const char *err)
    {
        qDebug() << err;
    }
}
