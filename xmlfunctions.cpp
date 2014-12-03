#include "QMessageBox"
#include "sqlfunctions.h"

xmlfunctions::xmlfunctions(QFile *File, QString Database)
{
    if (!File)
        throw "Memory address error,pls check the codes or contact us!";
    else if (File->fileName().isNull()||File->fileName().isEmpty())
        throw "Codes error , file name failed in Initialization!";
    else if (!File->exists())
        throw QString("File ["+File->fileName()+"] is not exists,pls check it!");
    else if (!File->open(QIODevice::ReadWrite))
        throw QString("File "+File->fileName()+"] open failed,pls check it!");
    //make sure if the Database is exists in the file
    QDomDocument Doc;

    QString ErrorStr;
    int ErrorLine;
    int ErrorCol;
    //read xml content from file.
    Doc.setContent(File,false,&ErrorStr,&ErrorLine,&ErrorCol);

    QDomNode Root;
    Root = Doc.firstChild();
    while(!Root.isNull())
    {
        if (Root.isElement())
        {
            if (Root.toElement().attribute("name").trimmed() == Database.trimmed() && Root.nodeName().trimmed() == "database")
            {
                m_DatabaseTree = Root;
                File->close();
                return;//ok get the database
            }
        }
        Root = Root.nextSibling();
    }
    //if codes goto below ,means database is not exists
    throw QString("Can not find database :[" +Database + "] in file:["
                  +File->fileName()+"] ,pls check it!");
}

//a record for search data not for UPDATE!!
int xmlfunctions::FltRecord(const QString &TableName,LiField Conditions[], int ConditionNum, QStringList FieldNames,LiTable &Dest, eRegularTableStruct FieldIntegrity)
{
    int Result = 0;

    QDomNode Table = GetTableByName(TableName,this->m_DatabaseTree);
    QDomNode Record = Table.firstChild();
    if (Record.isNull())
        return Result;

    while(!Record.isNull())
    {
        if (Record.isElement() && Record.nodeName() == "record" && AnalyseRecord(Record,Conditions,ConditionNum,FieldIntegrity))
        {
            AppendNode2LiTable(Record,FieldNames,Dest);
            Result++;
        }
        Record = Record.nextSibling();
    }
    return Result;

}


//updateRecord , which contains the Conditions.
int xmlfunctions::UpdRecord(QFile *File,const QString &TableName, LiField Conditions[], int ConditionNum, LiField Fields[], int FieldNum, eRegularTableStruct FieldIntegrity)
{
    int Result = 0;
    if (!File->open(QFile::ReadOnly | QFile::Text))
    {
        throw "File can not open,UpdRecord Record error!";
    }
    else
    {
        QDomDocument doc;
        QString errorStr;
        int errorLine;
        int errorCol;
        if (!doc.setContent(File,true,&errorStr,&errorLine,&errorCol))
        {
            File->close();
            throw "InsertRecord error, pls check the file!";
        }
        File->close();
        QDomElement root = doc.documentElement();
        QDomNode Table = root.firstChild();
        QDomNode NewTable;

        if (Table.isNull())
            throw "InsertRecord , file is EMPTY,pls check it!";

        //1 find Target table and create a copy : NewTable
        while (!Table.isNull())
        {
            if (Table.isElement() && Table.toElement().attribute("name")==TableName)
            {
                NewTable = Table;
                break;
            }
            Table = Table.nextSibling();
        }

        //2 to Update Record in NewTable

        QDomNode Record = NewTable.firstChild();
        QDomNode NewRecord;
        QDomNode Field;
        bool FieldNeedUpd;
        QString NewFieldValue;
        while(!Record.isNull())
        {
            NewRecord = Record;
            //2.1 Analyse nodes which tagname is record in NewTable,
            if (Record.isElement() && Record.toElement().tagName() =="record" && AnalyseRecord(Record,Conditions,ConditionNum,FieldIntegrity))
            {

                //2.2  analyse is true copy a new record but change the field value what need update
                Field = NewRecord.firstChild();

                while(!Field.isNull())
                {

                    QDomElement NewField;
                    NewField = doc.createElement("field");
                    FieldNeedUpd = false;
                    for(int i = 0; i < FieldNum ; i++)
                    {

                        if (Fields[i].GetName() == Field.toElement().attribute("name"))
                        {
                            FieldNeedUpd = true;
                            NewFieldValue = Fields[i].GetValue();
                        }
                    }
                    NewField.setAttribute(QString("name"),Field.toElement().attribute("name"));
                    if (FieldNeedUpd)
                        NewField.setAttribute(QString("value"),NewFieldValue);
                    else
                        NewField.setAttribute(QString("value"),Field.toElement().attribute("value"));
                    NewRecord.replaceChild(NewField,Field);
                    Field = NewField.nextSibling();

                }

            }

            NewTable.replaceChild(NewRecord,Record);
            //2.3 next Record
            Record = NewRecord.nextSibling();
        }




        //3 replace child in root and save file

        if (!NewTable.isNull())
            root.replaceChild(NewTable,Table);
        if (!File->open(QFile::WriteOnly | QFile::Truncate))
            throw "InsertRecord error , pls check it!";

        QTextStream out(File);
        out.reset();
        out.setCodec("UTF-8");
        root.save(out,4);
        return Result;
    }

}

int xmlfunctions::DelRecord(QFile *File, const QString &TableName, LiField Conditions[],int ConditionNum, eRegularTableStruct FieldIntegrity)
{
    int Result = 0;
    if (!File->open(QFile::ReadOnly | QFile::Text))
    {
        throw "File can not open,UpdRecord Record error!";
    }
    else
    {
        QDomDocument doc;
        QString errorStr;
        int errorLine;
        int errorCol;
        if (!doc.setContent(File,true,&errorStr,&errorLine,&errorCol))
        {
            File->close();
            throw "InsertRecord error, pls check the file!";
        }
        File->close();
        QDomElement root = doc.documentElement();
        QDomNode Table = root.firstChild();
        QDomNode NewTable;

        if (Table.isNull())
            throw "InsertRecord , file is EMPTY,pls check it!";

        //1 find Target table and create a copy : NewTable
        while (!Table.isNull())
        {
            if (Table.isElement() && Table.toElement().attribute("name")==TableName)
            {
                NewTable = Table;
                break;
            }
            Table = Table.nextSibling();
        }

        //2 to Update Record in NewTable
        QDomNode Record = Table.firstChild();
        QDomNode NewRecord = NewTable.firstChild();
        QDomNode t = NewTable.firstChild();
        QDomNode Field;

        while(!Record.isNull())
        {


            //2.1 Analyse nodes which tagname is record in NewTable,
            if (Record.isElement() && Record.toElement().tagName() =="record" && AnalyseRecord(Record,Conditions,ConditionNum,FieldIntegrity))
            {
                Record = Record.nextSibling();
                NewRecord = t;
                t = t.nextSibling();
                //2.2  analyse is true means we should remove it;
                NewTable.removeChild(NewRecord);

            }
            else
            {
                t = t.nextSibling();
                Record = Record.nextSibling();
            }
            //2.3 next Record


        }




        //3 replace child in root and save file

        if (!NewTable.isNull())
            root.replaceChild(NewTable,Table);
        if (!File->open(QFile::WriteOnly | QFile::Truncate))
            throw "InsertRecord error , pls check it!";

        QTextStream out(File);
        out.reset();
        out.setCodec("UTF-8");
        root.save(out,4);
        return Result;
    }
}


//this function for xmlquery :: insert
void xmlfunctions::InsertRecord(QFile *File, const QString &TableName, LiField Fields[],int FieldsNum)
{
    if (!File->open(QFile::ReadOnly | QFile::Text))
    {
        throw "File can not open,Insert Record error!";
    }
    else
    {
        QDomDocument doc;
        QString errorStr;
        int errorLine;
        int errorCol;
        if (!doc.setContent(File,true,&errorStr,&errorLine,&errorCol))
        {
            File->close();
            throw "InsertRecord error, pls check the file!";
        }
        File->close();
        QDomElement root = doc.documentElement();
        QDomNode Table = root.firstChild();
        QDomNode NewTable;

        if (Table.isNull())
            throw "InsertRecord , file is EMPTY,pls check it!";
        while (!Table.isNull())
        {
            if (Table.isElement() && Table.toElement().attribute("name")==TableName)
            {
                int LastNo;
                LastNo = Table.childNodes().count();
                NewTable = Table;

                QDomElement Record = doc.createElement("record");
                Record.setAttribute(QString("no"),LastNo);
                QStringList FieldNames;
                //begin to create new record and fill record with fields.
                for(int i=0;i<FieldsNum;i++)
                {
                    QDomElement Field = doc.createElement("field");
                    Field.setAttribute(QString("name"),Fields[i].GetName());
                    Field.setAttribute(QString("value"),Fields[i].GetValue());
                    FieldNames<<Fields[i].GetName();
                    Record.appendChild(Field);
                }
                //fill the default field into record if it not in params
                QDomNode DefaultRecord = Table.firstChild();
                QDomNodeList DefaultFields;
                while(!DefaultRecord.isNull())
                {
                    if (DefaultRecord.isElement() && DefaultRecord.toElement().tagName()=="default")
                    {
                        DefaultFields = DefaultRecord.childNodes();
                    }
                    DefaultRecord = DefaultRecord.nextSibling();
                }
                if (DefaultFields.count()>0)
                {
                    for(int j;j<DefaultFields.count();j++)
                    {
                        if (FieldNames.contains(DefaultFields.at(j).toElement().attribute("name")))
                            continue;
                        else
                        {
                            QDomElement Field = doc.createElement("field");
                            Field.setAttribute(QString("name"),DefaultFields.at(j).toElement().attribute("name"));
                            Field.setAttribute(QString("value"),DefaultFields.at(j).toElement().attribute("value"));
                            Record.appendChild(Field);
                        }
                    }
                }
                UpdateRcdWithPrompty(Record,doc,Table);
                NewTable.appendChild(Record);
                break;
            }
            Table = Table.nextSibling();
        }

        if (!NewTable.isNull())
            root.replaceChild(NewTable,Table);

        if (!File->open(QFile::WriteOnly | QFile::Truncate))
            throw "InsertRecord error , pls check it!";

        QTextStream out(File);
        out.reset();
        out.setCodec("UTF-8");
        root.save(out,4);
        return;
    }

}

QList<LiField> xmlfunctions::GetDefaultFields(const QString &TableName,const QDomNode &Table)
{
    QDomNode DefaultFields;
    QList<LiField> Result;

    DefaultFields = Table.firstChild();
    if (DefaultFields.isNull()) //emtpy table !!
        return Result;

    while (!DefaultFields.isNull())
    {
        DefaultFields = DefaultFields.nextSibling();
    }
}

void xmlfunctions::UpdateXML(QFile *File)
{
    if (!File->open(QIODevice::ReadWrite))
        throw QString("File "+File->fileName()+"] update failed,pls check it!");
    QTextStream out(File);
    out.setCodec("UTF-8");
    this->m_DatabaseTree.toDocument().save(out,4);
}

//function for query::query  ,anyrecord adpter the conditions then we should input it's field which in the field names into the Dest Table

void xmlfunctions::AppendNode2LiTable(const QDomNode &Record,QStringList FieldNames, LiTable &Dest)
{
    QSqlRecord NewRecord;

    QDomNode Field;

    Field = Record.firstChild();
    if (Field.isNull())
        return;
    while(!Field.isNull())
    {
        if (Field.isElement() && FieldNames.contains(Field.toElement().attribute("name")))
        {

            QSqlField T(Field.toElement().attribute("name"));
            T.setValue(Field.toElement().attribute("value"));
            NewRecord.append(T);

        }
        Field = Field.nextSibling();
    }
    Dest.AppendRecord(NewRecord);
}

void xmlfunctions::UpdateRcdWithPrompty(QDomNode &Record, QDomDocument Doc, const QDomNode &Table)
{
    QDomNode Prompty;
    QList<LiField> Result;

    Prompty = Table.firstChild();

    while (!Prompty.isNull())
    {
        if (Prompty.isElement() && Prompty.toElement().tagName()=="promptys")
        {
            //found prompty,now lets go
            QDomNode t;
            t = Prompty.firstChild();
            while(!t.isNull())
            {
                if (t.isElement())
                {
                    //identity field
                    if (t.toElement().tagName()=="prompty" && t.toElement().attribute("type")=="identity" )
                    {
                        //identity field could not update or insert by Sql Cmd ,could only update by Database service
                        QString FieldName = t.toElement().attribute("value");
                        int MaxValue = GetMaxValueByFieldName(FieldName,Table);
                        MaxValue ++;
                        UpdateOrInsertFieldToRecord(Record,FieldName,MaxValue,Doc);
                    }
                    //unique field
                    if (t.toElement().tagName()=="prompty" && t.toElement().attribute("type")=="unique")
                    {
                        //unique field must , oh .... unique .
                    }
                }
                t = t.nextSibling();
            }
        }

        Prompty = Prompty.nextSibling();

    }
}

int xmlfunctions::GetMaxValueByFieldName(const QString FieldName, const QDomNode &Table)
{
    int Result = 0;
    QDomNode Record;
    Record = Table.firstChild();
    while(!Record.isNull())
    {
        if (Record.isElement() && Record.toElement().tagName()=="record")
        {
            QDomNode Field = Record.firstChild();
            while(!Field.isNull())
            {
                if (Field.isElement() && Field.toElement().tagName()=="field" && Field.toElement().attribute("name")== FieldName)
                {
                    QString value = Field.toElement().attribute("value");
                    bool *IsInt = new bool;
                    int t = value.toInt(IsInt);
                    if (!(*IsInt))  //means value is not a int
                        t = -1;
                    if (t > Result)
                        Result = t;
                }
                Field = Field.nextSibling();
            }

        }
        Record = Record.nextSibling();
    }
    return Result;
}

void xmlfunctions::UpdateOrInsertFieldToRecord(QDomNode &Record, const QString FieldName,int FieldValue, QDomDocument Doc)
{
    QDomNode FieldExists = Record.firstChild();
    bool IsExists = false;
    while(!FieldExists.isNull())
    {
        if (FieldExists.isElement() && FieldExists.toElement().tagName() == "record" && FieldExists.toElement().attribute("name") == FieldName)
        {
            IsExists = true;
            FieldExists.toElement().setAttribute("value",FieldValue);
        }
        FieldExists = FieldExists.nextSibling();
    }

    if (!IsExists)
    {
        QDomElement Field = Doc.createElement("field");
        Field.setAttribute(QString("name"),FieldName);
        Field.setAttribute(QString("value"),FieldValue);
        Record.appendChild(Field);
    }
}

//still not finished yet
bool xmlfunctions::AnalyseCMDWithPrompty(const QDomNode &Table)
{
    return true;
}

//sub function for FltRecord
bool xmlfunctions::AnalyseRecord(QDomNode &Record, LiField Conditions[],int ConditionNum,eRegularTableStruct Regular)
{
    if (ConditionNum < 1)
        return true;
    bool Result = (bool&) Regular;
    QDomNode Field;
    for (int i = 0;i<ConditionNum;i++)
    {

        Field = Record.firstChild();
        if (Field.isNull())
            return false; //no child
        while(!Field.isNull())
        {
            if (!Field.isElement())
            {
                Field =Field.nextSibling();
                continue; //Carefully, jump one step
            }
            if(Conditions[i].GetAON()== OR || Conditions[i].GetAON() == NOTOR)  // OR condition
            {
                if(Field.toElement().attribute("name")== Conditions[i].GetName() && Field.toElement().attribute("value") == Conditions[i].GetValue())
                    return true;
            }
            else  //AND condition
            {
                if (Field.toElement().attribute("name") == Conditions[i].GetName() && Field.toElement().attribute("value") != Conditions[i].GetValue())
                    return false;
                else if (Field.toElement().attribute("name") == Conditions[i].GetName() && Field.toElement().attribute("value") == Conditions[i].GetValue())
                    Result = true;
            }


            Field = Field.nextSibling();
        }
    }
    return Result;
}

//if no table found this function will return a null node (Node isNull)
QDomNode xmlfunctions::GetTableByName(const QString &TableName, QDomNode &Database)
{
    QDomNode Table = Database.firstChild();

    while (!Table.isNull())
    {
        if (Table.isElement())
            if (Table.toElement().attribute("name")== TableName && Table.nodeName() == "table")
                return Table;

        Table = Table.nextSibling();
    }
    return Table;
}


