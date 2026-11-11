#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QHash>
#include <QFrame>
#include <QTimer>

namespace
{
QLabel* makeCell(QString text, QString color)
{
    QLabel *l = new QLabel(text);
    l->setAlignment(Qt::AlignCenter);
    l->setStyleSheet("background:"+color+
        ";padding:7px;border-radius:5px;color:#172554;font-weight:600;");
    return l;
}

QFrame* makeCard(QString name, QLabel *&value, QString color)
{
    QFrame *f = new QFrame;
    f->setStyleSheet("background:white;border-radius:10px;");
    QVBoxLayout *v = new QVBoxLayout(f);
    QLabel *t = new QLabel(name);
    value = new QLabel("0");
    t->setStyleSheet("color:#64748B;border:none;");
    value->setStyleSheet("font-size:25px;font-weight:bold;color:"+color+";");
    v->addWidget(t);
    v->addWidget(value);
    return f;
}

bool updateStatus(QString path, QString id, QString status)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) return false;

    QStringList records;
    bool found=false;
    QTextStream in(&file);

    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()==7 && d[0]==id)
        {
            d[6]=status;
            found=true;
        }
        records<<d.join('|');
    }
    file.close();

    if(!found) return false;

    QSaveFile save(path);
    if(!save.open(QIODevice::WriteOnly|QIODevice::Text)) return false;

    QTextStream out(&save);
    out<<records.join('\n');
    out.flush();
    return save.commit();
}
}

QWidget* createEmergencyRequestsPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");
    QVBoxLayout *main=new QVBoxLayout(page);
    main->setSpacing(12);

    QString path=QCoreApplication::applicationDirPath()+"/";

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet("background:#172554;border-radius:12px;");
    QVBoxLayout *hl=new QVBoxLayout(header);

    QLabel *title=new QLabel("Emergency Blood Requests");
    QLabel *sub=new QLabel("Manage requests and track fulfilment");
    title->setStyleSheet("color:white;font-size:24px;font-weight:bold;");
    sub->setStyleSheet("color:#CBD5E1;");
    hl->addWidget(title);
    hl->addWidget(sub);

    // STATISTICS
    QLabel *total,*pending,*emergency,*totalUnits;
    QHBoxLayout *stats=new QHBoxLayout;
    stats->addWidget(makeCard("Total Requests",total,"#2563EB"));
    stats->addWidget(makeCard("Pending",pending,"#D97706"));
    stats->addWidget(makeCard("Active Emergencies",emergency,"#DC2626"));
    stats->addWidget(makeCard("Active Units",totalUnits,"#059669"));

    // REQUEST FORM
    QFrame *form=new QFrame;
    form->setStyleSheet("background:white;border-radius:10px;");
    QVBoxLayout *fl=new QVBoxLayout(form);

    QLabel *formTitle=new QLabel("Create New Request");
    formTitle->setStyleSheet("font-size:19px;font-weight:bold;");

    QComboBox *hospital=new QComboBox;
    QComboBox *group=new QComboBox;
    QComboBox *priority=new QComboBox;
    QSpinBox *units=new QSpinBox;

    hospital->addItem("Select Hospital");
    group->addItems({"Select Group","A+","A-","B+","B-","AB+","AB-","O+","O-"});
    priority->addItems({"Select Priority","Normal","Urgent","Emergency"});
    units->setRange(0,100);
    units->setSpecialValueText("Select Units");

    QString style="QComboBox,QSpinBox{padding:9px;background:#F8FAFC;"
                  "border:1px solid #CBD5E1;border-radius:6px;}";
    hospital->setStyleSheet(style);
    group->setStyleSheet(style);
    priority->setStyleSheet(style);
    units->setStyleSheet(style);

    QFile hf(path+"hospitals.txt");
    if(hf.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&hf);
        while(!in.atEnd())
        {
            QStringList d=in.readLine().split('|');
            if(d.size()>=3) hospital->addItem(d[1]+" - "+d[2],d[0]);
        }
    }

    QGridLayout *fields=new QGridLayout;
    fields->addWidget(new QLabel("Hospital"),0,0);
    fields->addWidget(new QLabel("Blood Group"),0,1);
    fields->addWidget(hospital,1,0);
    fields->addWidget(group,1,1);
    fields->addWidget(new QLabel("Units"),2,0);
    fields->addWidget(new QLabel("Priority"),2,1);
    fields->addWidget(units,3,0);
    fields->addWidget(priority,3,1);

    QPushButton *submit=new QPushButton("Submit Request");
    submit->setStyleSheet("background:#DC2626;color:white;padding:11px;"
                          "border-radius:6px;font-weight:bold;");
    fields->addWidget(submit,4,1);
    fl->addWidget(formTitle);
    fl->addLayout(fields);

    // REQUEST TABLE
    QFrame *box=new QFrame;
    box->setStyleSheet("background:white;border-radius:10px;");
    QVBoxLayout *bl=new QVBoxLayout(box);

    QLabel *tableTitle=new QLabel("Request Management");
    tableTitle->setStyleSheet("font-size:19px;font-weight:bold;");
    QPushButton *refreshBtn=new QPushButton("Refresh");

    QHBoxLayout *top=new QHBoxLayout;
    top->addWidget(tableTitle);
    top->addStretch();
    top->addWidget(refreshBtn);

    QTableWidget *table=new QTableWidget(0,7);
    table->setHorizontalHeaderLabels({
        "Request ID","Hospital","Blood Group","Units",
        "Priority","Status","Update Status"
    });
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    table->setStyleSheet("QHeaderView::section{background:#172554;color:white;"
                         "padding:9px;border:none;font-weight:bold;}"
                         "QTableWidget{border:none;background:white;}");

    // LOAD REQUESTS
    auto refresh=[=]()
    {
        table->setRowCount(0);
        QHash<QString,QString> names;

        QFile h(path+"hospitals.txt");
        if(h.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&h);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()>=3) names[d[0]]=d[1]+" - "+d[2];
            }
        }

        int t=0,p=0,e=0,u=0;
        QFile f(path+"requests.txt");

        if(f.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&f);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()!=7) continue;

                t++;
                QString status=d[6];
                bool active=(status!="Fulfilled" && status!="Cancelled");

                if(status=="Pending") p++;
                if(active) u+=d[3].toInt();
                if(active && d[5]=="Emergency") e++;

                int r=table->rowCount();
                table->insertRow(r);

                QStringList row={d[0],names.value(d[1],d[1]),
                                 d[2],d[3],d[5],status};

                for(int c=0;c<6;c++)
                {
                    QString color="#F8FAFC";
                    if(c==4) color=d[5]=="Emergency"?"#FECACA":
                                    d[5]=="Urgent"?"#FED7AA":"#DCFCE7";
                    if(c==5) color=status=="Fulfilled"?"#DCFCE7":
                                    status=="Cancelled"?"#E2E8F0":
                                    status=="In Progress"?"#DBEAFE":
                                    status=="Matched"?"#CCFBF1":"#FEF3C7";
                    table->setCellWidget(r,c,makeCell(row[c],color));
                }

                QWidget *manage=new QWidget;
                QHBoxLayout *ml=new QHBoxLayout(manage);
                ml->setContentsMargins(2,2,2,2);

                QComboBox *statusBox=new QComboBox;
                statusBox->addItems({"Pending","Matched","In Progress",
                                     "Fulfilled","Cancelled"});
                statusBox->setCurrentText(status);
                statusBox->setStyleSheet(style);

                QPushButton *updateBtn=new QPushButton("Update");
                updateBtn->setStyleSheet("background:#2563EB;color:white;"
                                         "padding:7px;border-radius:5px;");

                ml->addWidget(statusBox);
                ml->addWidget(updateBtn);
                table->setCellWidget(r,6,manage);

                QString id=d[0],oldStatus=status;

                QObject::connect(updateBtn,&QPushButton::clicked,page,[=]()
                {
                    QString next=statusBox->currentText();
                    if(next==oldStatus) return;

                    if(next=="Fulfilled" &&
                       QMessageBox::question(page,"Confirm",
                       "Has the hospital received the required blood?",
                       QMessageBox::Yes|QMessageBox::No)!=QMessageBox::Yes)
                        return;

                    if(next=="Cancelled" &&
                       QMessageBox::question(page,"Cancel Request",
                       "Cancel this request?",
                       QMessageBox::Yes|QMessageBox::No)!=QMessageBox::Yes)
                        return;

                    if(updateStatus(path+"requests.txt",id,next))
                    {
                        QMessageBox::information(page,"Updated","Status saved.");
                        QTimer::singleShot(0,page,[=](){ refreshBtn->click(); });
                    }
                    else
                        QMessageBox::warning(page,"Error","Status update failed.");
                });

                table->setRowHeight(r,52);
            }
        }

        total->setText(QString::number(t));
        pending->setText(QString::number(p));
        emergency->setText(QString::number(e));
        totalUnits->setText(QString::number(u));
    };

    QObject::connect(refreshBtn,&QPushButton::clicked,page,refresh);
    refresh();

    // SAVE REQUEST
    QObject::connect(submit,&QPushButton::clicked,page,[=]()
    {
        if(hospital->currentIndex()==0 || group->currentIndex()==0 ||
           priority->currentIndex()==0 || units->value()==0)
        {
            QMessageBox::warning(page,"Error","Fill all details!");
            return;
        }

        QFile f(path+"requests.txt");
        if(!f.open(QIODevice::Append|QIODevice::Text))
        {
            QMessageBox::warning(page,"Error","Cannot save request!");
            return;
        }

        QTextStream out(&f);
        out<<"R"<<QDateTime::currentMSecsSinceEpoch()<<"|"
           <<hospital->currentData().toString()<<"|"<<group->currentText()
           <<"|"<<units->value()<<"|"
           <<QDate::currentDate().toString("dd-MM-yyyy")<<"|"
           <<priority->currentText()<<"|Pending\n";

        f.close();
        hospital->setCurrentIndex(0);
        group->setCurrentIndex(0);
        priority->setCurrentIndex(0);
        units->setValue(0);
        refreshBtn->click();
        QMessageBox::information(page,"Success","Request saved!");
    });

    bl->addLayout(top);
    bl->addWidget(table);
    main->addWidget(header);
    main->addLayout(stats);
    main->addWidget(form);
    main->addWidget(box,1);

    return page;
}