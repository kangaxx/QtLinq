#include "sqlfunctions.h"

//--------------------------------------------------------------------------------------//
/******************                 Linq codes Begin              ***********************/






/////////////////////////////////////////////////////
// LiFields class function below                   //
// create by gxx 2013 11 27                        //
/////////////////////////////////////////////////////
LiField::LiField()
{
    pTable = NULL;
    eAON = AND;
}

LiField::LiField(QString FieldName)
{
    pTable = NULL;
    sFieldName = FieldName;
    eAON = AND;
}



QString LiField::GetName() const
{
    return sFieldName;
}

QString LiField::GetTitle() const
{
    return sTitle;
}

QString LiField::GetValue() const
{
    return sValue;
}

eFType LiField::GetType() const
{
    return eFieldType;
}

int LiField::GetLen() const
{
    return iLength;
}

int LiField::GetPcs() const
{
    return iPrecision;
}

QString LiField::GetTableName() const
{
    if (pTable == NULL)
        return NULL;
    return pTable->GetName();
}

eAndOrNot LiField::GetAON() const
{
    return eAON;
}

eLiSqlOperate LiField::GetSO() const
{
    return eSO;
}

bool LiField::IsIdentity() const
{
    return bIsIdentity;
}

bool LiField::IsPrime() const
{
    return bIsPrime;
}

bool LiField::IsUnique() const
{
    return bIsUnique;
}

void LiField::SetIdentity(bool IsIdentity)
{
    bIsIdentity = IsIdentity;
}

void LiField::SetLen(int Len)
{
    iLength = Len;
}

void LiField::SetName(QString FieldName)
{
    if (FieldName == NULL||FieldName.trimmed().isEmpty())
        throw "Param is NULL, SetName error!";
    sFieldName = FieldName;
}

void LiField::SetPrecision(int Pcs)
{
    iPrecision = Pcs;
}

void LiField::SetPrime(bool IsPrime)
{
    bIsPrime = IsPrime;
}

void LiField::SetTitle(QString Title)
{
    sTitle = Title;
}

void LiField::SetType(eFType Type)
{
    eFieldType = Type;
}

void LiField::SetUnique(bool IsUnique)
{
    bIsUnique = IsUnique;
}

void LiField::SetAON(eAndOrNot AON)
{
    eAON = AON;
}

void LiField::SetSo(eLiSqlOperate SO)
{
    eSO = SO;
}

void LiField::SetValue(QString Value)
{
    if (Value == NULL||Value.trimmed().isEmpty())
        throw "Param is NULL, SetValue error!";
    sValue = Value;
}

void LiField::Set(eFType Type, QString Value, int Len, int Pcs, QString Title)
{
    SetType(Type);
    SetValue(Value);
    SetLen(Len);
    SetPrecision(Pcs);
    SetTitle(Title);
}

void LiField::Set(QString FieldName, eFType Type, int Len, int Pcs)
{
    SetName(FieldName);
    SetType(Type);
    SetLen(Len);
    SetPrecision(Pcs);
}

LiTable * LiField::SetTableLink(LiTable *Table)
{
    LiTable* result;
    result = Table;
    this->pTable = Table;
    return result;
}

/////////////////////////////////////////////////////
// LiRecords class function below                  //
// create by gxx 2013 11 27                        //
/////////////////////////////////////////////////////
LiRecords::LiRecords()
{
    plRecords = new QList<QSqlRecord>;
    iTotalCount = 0;
    iIndex = -1;
    _LiRecords=this;
}

LiRecords::LiRecords(QList<QSqlRecord> &Records)
{
    _LiRecords = this;
    if (Records.count()<0)
        throw "Can not Create a Empty LiRecords!";
    if (Records.count()==0)
    {
        plRecords = new QList<QSqlRecord>;
        iTotalCount = 0;
        iIndex = -1;
    }
    else
    {
        plRecords = new QList<QSqlRecord>;
        *plRecords = Records;
        iTotalCount = plRecords->count();
        iIndex = -1;
    }

}

//Copy Create function , should use in LiTable
LiRecords::LiRecords(LiRecords &other)
{
    _LiRecords = &other;
    if (other.GetCount()<0)
        throw "Can not Create a Empty LiRecords!";
    if (other.GetCount()==0)
    {
        plRecords = new QList<QSqlRecord>;
        iTotalCount = 0;
        iIndex = other.iIndex;
    }
    else
    {
        plRecords = new QList<QSqlRecord>;
        *plRecords = other.GetRecords();
        iTotalCount = plRecords->count();
        iIndex = other.iIndex;
    }
}

LiRecords::~LiRecords()
{
    if (plRecords != NULL)
    {
        delete plRecords;
        plRecords = NULL;
    }
}

QList<QSqlRecord> LiRecords::GetRecords() const
{
    return *plRecords;
}

int LiRecords::GetCount() const
{
    return iTotalCount;
}

QSqlRecord LiRecords::First()
{
    if (iTotalCount == 0||plRecords->isEmpty())
        throw "No Record,pls check SQL command!";
    _LiRecords->iIndex=0;
    return plRecords->first();
}

QSqlRecord LiRecords::Last()
{
    if (iTotalCount == 0||plRecords->isEmpty())
        throw "No Record,pls check SQL command!";
    _LiRecords->iIndex = iTotalCount-1;
    return plRecords->last();
}

QSqlRecord LiRecords::Next()
{
    if (iTotalCount == 0||plRecords->isEmpty())
        throw "No Record,pls check SQL command!";
    if (iIndex >= iTotalCount-1)
    {

        QSqlRecord Ret;
        return Ret; //with out new ,it is a null point be return , means we will get a NULL when we do next after last point of list. gxx 2013 11 16

    }
    else
    {
        _LiRecords->iIndex ++;
        return plRecords->at(_LiRecords->iIndex);
    }
}

QStringList LiRecords::NextFields(QStringList FieldNames)
{
    if (iTotalCount == 0||plRecords->isEmpty())
        throw "No Record,pls check SQL command!";
    if (FieldNames.count()<= 0)
        throw "No field name input,pls check codes!";
    QStringList result;
    _LiRecords->iIndex ++;
    if (_LiRecords->iIndex >= iTotalCount)
    {
        return result;
    }
    else
    {
        QSqlRecord rcd;

        rcd = plRecords->at(_LiRecords->iIndex);
        foreach (QString sName, FieldNames)
        {
            result<<rcd.field(sName).value().toString();
        }
        return result;
    }

}


void LiRecords::SetRecords(QList<QSqlRecord> * Records)
{
    if (plRecords != NULL)
        throw "Records already set, don't do it twice!";
    if (Records->isEmpty())
        throw "No Record,pls check SQL command!";
    plRecords = Records;
    iTotalCount = plRecords->count();
    iIndex = -1;
}



bool LiRecords::AddRecord(QSqlRecord rcd)
{
    try {
        if (plRecords)
        {
            plRecords->append(rcd);
            iTotalCount ++;
            return true;
        }
        return false;
    } catch (...) {
        return false;
    }
}

/////////////////////////////////////////////////////
// LiTable class function below                    //
// create by gxx 2013 11 27                        //
/////////////////////////////////////////////////////
LiTable::LiTable()
{
    iCopyNum = 0;
    iRecordsCount = 0;
    plRecords = new LiRecords;
}

LiTable::LiTable(QString TableName)
{
    iCopyNum = 0;
    SetName(TableName);
    plRecords = new LiRecords;
}

LiTable::~LiTable()
{
    int i = 0;
}

void LiTable::SetName(QString TableName)
{
    sTableName= TableName;
}

QString LiTable::GetName() const
{
    return sTableName;
}

LiRecords LiTable::GetRecords()
{
    if (plRecords == NULL)
        throw "Has not set up Records yet,do it before GetRecords,GetRecords error!";
    return *plRecords;
}

void LiTable::SetRecords(LiRecords *Records)
{
    delete plRecords;
    plRecords = Records;
    iRecordsCount = plRecords->GetCount();
}

bool LiTable::AppendRecord(QSqlRecord rcd)
{
    return plRecords->AddRecord(rcd);
}

/////////////////////////////////////////////////////
// LiDataContext class function below              //
// create by gxx 2013 11 27                        //
// Most import class for QtLinq                    //
/////////////////////////////////////////////////////
LiDataContext::LiDataContext()
{
    lpTables = new QList<LiTable*>;
    lFields = new QList<LiField>;
    lConditions = new QList<LiField>;
    bSameTable = false;
}

LiDataContext::~LiDataContext()
{
    if (lpTables != NULL)
    {
        delete lpTables;
        lpTables = NULL;
    }
    if (lFields != NULL)
    {
        delete lFields;
        lFields = NULL;
    }
    if (lConditions != NULL)
    {
        delete lConditions;
        lConditions = NULL;
    }
}

//SetTableName functions:
//give same TableName to all Fields , and this DataContext will run on all of it.
//WARNING: if already have two or more tables , throw exception.
//AND : After SetTableName,  we won't allow add table count , but still allow change table name
//create by gxx 2013 11 27
void LiDataContext::SetTableName(QString TableName)
{
    if (TableName == NULL||TableName.trimmed().isEmpty())
        throw "TableName set to empty,SetTableName error!";

    if (GetTableNum()>1)
        throw "Already have two or more tables, SetTableName fail!";
    else if (GetTableNum()==0)
    {
        AddTable(TableName);
    }
    else if (GetTableNum()==1)
    {
        if ((*(this->lpTables))[0]->GetRecords().GetCount()>0)
            throw "Can not RESET a reocreds of table,SetTableName fail!";
        else
            (*(this->lpTables))[0]->SetName(TableName);
    }
    bSameTable=true;
}

//AddTable for SetTableName when there is no Table, so must Add one
LiTable * LiDataContext::AddTable(QString TableName)
{
    LiTable * Table = new LiTable(TableName);
    (*lpTables).append(Table);
    return Table;
}


//Normally add Field and Condition will return true , otherwise tablename conflict the order .(see comment About SetTableName) gxx 2013 11 27
bool LiDataContext::AddField(LiField * Field)
{
    if ((*lpTables).count() == 0 && Field->GetTableName()== NULL)
        throw "Can not add a Field without any table be set, Add Field fail!";
    if ((*lpTables).count()>0 && Field->GetTableName()==NULL)
        Field->SetTableLink((*lpTables)[0]);
    foreach (LiField fld, (*lFields))
    {
        if (fld.GetName() == Field->GetName() && fld.GetTableName() != NULL && Field->GetTableName() != NULL && fld.GetTableName() == Field->GetTableName())
            throw "Already have a same field , Add Field Error!";
    }
    (*lFields).append(*Field);
    return true;
}

bool LiDataContext::AddField(QString & FieldName, eFType FieldType,QString Value,int Len, int Pcs)
{
    LiField * Field = new LiField(FieldName);
    Field->SetType(FieldType);
    if (Value != NULL &&    ( !Value.trimmed().isEmpty() )   )
    {
        Field->SetValue(Value);
    }

    Field->SetLen(Len);
    Field->SetPrecision(Pcs);
    AddField(Field);
    return true;
}



bool LiDataContext::AddField(QString & FieldName, eFType FieldType,int Len,int Pcs,QString Title)
{
    LiField * Field = new LiField(FieldName);
    Field->SetType(FieldType);
    Field->SetLen(Len);
    Field->SetPrecision(Pcs);
    Field->SetTitle(Title);
    AddField(Field);
    return true;
}


//wait to complete
//bool LiDataContext::AddField(LiTable * pTable, QString & FieldName, eFType FieldType,int Len,int Pcs,QString Title,QString Value);
//{
//    //wait to do
//}


bool LiDataContext::AddCondition(LiField * CondField)
{
    if ((*lpTables).count() == 0 && CondField->GetTableName()== NULL)
        throw "Can not add a Field without any table be set, Add Field fail!";
    if ((*lpTables).count()>0 && CondField->GetTableName()==NULL)
        CondField->SetTableLink((*lpTables)[0]);
    (*lConditions).append(*CondField);
    return true;
}


bool LiDataContext::AddConditions(QList<LiField*> CondFields, eAndOrNot GroupAON)
{
    LiField * pField = new LiField("");
    MakeGroupCondsToCond(CondFields,GroupAON,pField);
    return AddCondition(pField);
    // It will delete ptr PField later by :void qDeleteAll ( ForwardIterator begin, ForwardIterator end )  gxx 2013 11 29
    // example:       QList<Employee *> list;
    //                list.append(new Employee("Blackpool", "Stephen"));
    //                list.append(new Employee("Twist", "Oliver"));
    //                qDeleteAll(list.begin(), list.end());
    //                list.clear();
}

LiField* LiDataContext::MakeGroupCondsToCond(QList<LiField *> GroupCond, eAndOrNot GroupAON, LiField *result)
{
    //NOT COMPLETE ,DO IT LATER GXX 2013 11 29
    return result;
}


int LiDataContext::GetTableNum()
{
    return (*lpTables).count();
}

QStringList *LiDataContext::GetTableNames(QStringList &NameList) const
{
    QStringList* result;
    result = &NameList;
    foreach(LiTable *pTable,(*(this->lpTables)))
    {
        *result << pTable->GetName();
    }
    return result;
}

QList<LiField> LiDataContext::GetFieldsByTableName(QString & TableName)
{
    if (TableName.trimmed().isEmpty())
        throw "TableName is empty,GetFieldsByTableName Fail!";
    QList<LiField> result;
    foreach (LiField field, (*lFields))
    {
        if (field.GetTableName().trimmed().toUpper() == TableName.trimmed().toUpper())
            result.append(field);
    }
    return result;
}

QList<LiField> LiDataContext::GetConditionsByTableName(QString & TableName) const
{
    QList<LiField> result;
    if ((*lConditions).count()<= 0)
        return result;
    if (TableName.trimmed().isEmpty())
        throw "TableName is empty,GetConditionsByTableName Fail!";

    foreach (LiField field, (*lConditions))
    {
        if (field.GetTableName().trimmed().toUpper() == TableName.trimmed().toUpper())
            result.append(field);
    }
    return result;
}

/////////////////////////////////////////////////////
// XML file class function below                   //
// create by gxx 2013 11 27                        //
// Most import class for QtLinq                    //
/////////////////////////////////////////////////////
LiXmlLinker::LiXmlLinker(const QString &Database, const QString &FileName)
{
    m_FileName = FileName;
    m_Database = Database;
    if (FileName.trimmed().isEmpty())
        throw "Iniatialization Fail, please check the file path and name";
    m_pFile = new QFile;
    m_pFile->setFileName(FileName);
}

LiXmlLinker::LiXmlLinker(const LiXmlLinker &other)
{
    m_FileName =other.m_FileName;
    m_Database = other.m_Database;
    m_pFile = new QFile;
    m_pFile = other.GetFile();
}

LiXmlLinker&LiXmlLinker::operator =(const LiXmlLinker & Right)
{
    m_FileName =Right.m_FileName;
    m_Database = Right.m_Database;
    QFile *t = new QFile;
    t = Right.GetFile();
    delete m_pFile;
    m_pFile = t;
    return * this;
}

LiXmlLinker::LiXmlLinker(Connections Conn)
{
    m_FileName = Conn.ConnName;
    m_Database = Conn.Database;
    if (m_FileName.trimmed().isEmpty())
        throw "Iniatialization Fail, please check the file path and name";
    m_pFile = new QFile;
    m_pFile->setFileName(Conn.ConnName);


    //donothing;
}


LiXmlLinker::~LiXmlLinker()
{
    m_pFile->close();
    delete m_pFile;
    m_pFile = 0;
}

QFile * LiXmlLinker::GetFile() const
{
    return m_pFile;
}

QString LiXmlLinker::GetDbName()
{
    return m_Database;
}


/*****************************************    LINQ  END   *******************************************/
