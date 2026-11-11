#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QHeaderView>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QMessageBox>
#include <QHash>
#include <QTimer>
#include <QFrame>
#include <QColor>
#include <QStringList>

namespace
{
int countRecords(QString path,int fields)
{
    QFile f(path);
    int count=0;
    if(f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream in(&f);
        while(!in.atEnd())
            if(in.readLine().split('|').size()==fields) count++;
    }
    return count;
}

// DIFFERENT COLOUR STAT CARDS
QFrame* adminCard(QString name,QLabel *&value,
                  QString bg,QString color,QString icon)
{
    QFrame *card=new QFrame;
    card->setMinimumHeight(125);
    card->setStyleSheet(
        "QFrame{background:"+bg+";border:1px solid "+color+
        ";border-radius:14px;}"
    );

    QVBoxLayout *v=new QVBoxLayout(card);
    v->setSpacing(5);
    v->setAlignment(Qt::AlignCenter);

    QLabel *badge=new QLabel(icon);
    badge->setFixedSize(37,37);
    badge->setAlignment(Qt::AlignCenter);
    badge->setStyleSheet(
        "background:"+color+";color:white;"
        "border-radius:10px;font-size:18px;font-weight:bold;"
    );

    QLabel *title=new QLabel(name);
    value=new QLabel("0");

    title->setAlignment(Qt::AlignCenter);
    value->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(
        "color:#334155;font-size:11px;font-weight:bold;border:none;"
    );
    value->setStyleSheet(
        "color:"+color+";font-size:29px;font-weight:bold;border:none;"
    );

    v->addWidget(badge,0,Qt::AlignCenter);
    v->addWidget(title,0,Qt::AlignCenter);
    v->addWidget(value,0,Qt::AlignCenter);
    return card;
}

bool saveStatus(QString path,QString id,QString status)
{
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text)) return false;

    QStringList rows;
    bool found=false;
    QTextStream in(&f);

    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()==7 && d[0]==id)
        {
            d[6]=status;
            found=true;
        }
        rows<<d.join('|');
    }
    f.close();

    if(!found) return false;

    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)) return false;

    {
        QTextStream out(&file);
        out<<rows.join('\n');
        out.flush();
    }
    return file.commit();
}
}

QWidget* createAdminPanelPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *main=new QVBoxLayout(page);
    main->setContentsMargins(18,15,18,15);
    main->setSpacing(13);

    QString path=QCoreApplication::applicationDirPath()+"/";

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:14px;"
    );

    QHBoxLayout *hl=new QHBoxLayout(header);
    QVBoxLayout *hv=new QVBoxLayout;

    QLabel *title=new QLabel("Admin Control Center");
    QLabel *sub=new QLabel("HEMOLINK | SYSTEM OVERVIEW & MANAGEMENT");
    QLabel *badge=new QLabel("● ADMIN PANEL");

    title->setStyleSheet("color:white;font-size:25px;font-weight:bold;");
    sub->setStyleSheet("color:#BFDBFE;font-size:10px;font-weight:bold;");
    badge->setStyleSheet(
        "background:#DCFCE7;color:#15803D;"
        "padding:10px;border-radius:8px;font-weight:bold;"
    );

    hv->addWidget(title);
    hv->addWidget(sub);
    hl->addLayout(hv);
    hl->addStretch();
    hl->addWidget(badge);

    // COLOURFUL STATISTICS
    QGridLayout *grid=new QGridLayout;
    grid->setSpacing(13);

    QLabel *donors,*hospitals,*requests;
    QLabel *pending,*fulfilled,*bloodUnits;

    grid->addWidget(adminCard("TOTAL DONORS",donors,
        "#DBEAFE","#1D4ED8","D"),0,0);

    grid->addWidget(adminCard("HOSPITALS",hospitals,
        "#EDE9FE","#6D28D9","H"),0,1);

    grid->addWidget(adminCard("TOTAL REQUESTS",requests,
        "#CFFAFE","#0E7490","R"),0,2);

    grid->addWidget(adminCard("PENDING REQUESTS",pending,
        "#FEF3C7","#B45309","P"),1,0);

    grid->addWidget(adminCard("FULFILLED REQUESTS",fulfilled,
        "#D1FAE5","#047857","✓"),1,1);

    grid->addWidget(adminCard("BLOOD UNITS",bloodUnits,
        "#FFE4E6","#BE123C","B"),1,2);

    // REQUEST MANAGEMENT
    QFrame *box=new QFrame;
    box->setStyleSheet(
        "background:white;border:1px solid #E2E8F0;border-radius:12px;"
    );

    QVBoxLayout *bl=new QVBoxLayout(box);
    bl->setContentsMargins(14,14,14,12);

    QHBoxLayout *top=new QHBoxLayout;
    QLabel *heading=new QLabel("Patient Request Management");
    heading->setStyleSheet("font-size:18px;font-weight:bold;");

    QPushButton *refreshBtn=new QPushButton("↻ Refresh");
    refreshBtn->setStyleSheet(
        "background:#2563EB;color:white;padding:9px 14px;"
        "border-radius:7px;font-weight:bold;"
    );

    top->addWidget(heading);
    top->addStretch();
    top->addWidget(refreshBtn);

    QTableWidget *table=new QTableWidget(0,7);
    table->setHorizontalHeaderLabels({
        "Request ID","Hospital","Group","Units",
        "Priority","Status","Manage"
    });

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setShowGrid(false);

    table->setStyleSheet(
        "QHeaderView::section{background:#172554;color:white;"
        "padding:10px;border:none;font-weight:bold;}"
        "QTableWidget{background:white;border:none;"
        "alternate-background-color:#F8FAFC;}"
    );

    // UPDATE STATISTICS
    auto refreshStats=[=]()
    {
        int p=0,done=0,units=0;

        donors->setText(QString::number(
            countRecords(path+"donors.txt",8)));

        hospitals->setText(QString::number(
            countRecords(path+"hospitals.txt",5)));

        requests->setText(QString::number(
            countRecords(path+"requests.txt",7)));

        QFile f(path+"requests.txt");
        if(f.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&f);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()!=7) continue;
                if(d[6]=="Pending") p++;
                if(d[6]=="Fulfilled") done++;
            }
        }

        QFile stock(path+"blood_stock.txt");
        if(stock.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&stock);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()==2) units+=d[1].toInt();
            }
        }

        pending->setText(QString::number(p));
        fulfilled->setText(QString::number(done));
        bloodUnits->setText(QString::number(units));
    };

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
                if(d.size()>=3) names[d[0]]=d[1];
            }
        }

        QFile f(path+"requests.txt");
        if(f.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&f);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()!=7) continue;

                int r=table->rowCount();
                table->insertRow(r);

                QStringList row={
                    d[0],names.value(d[1],d[1]),
                    d[2],d[3],d[5],d[6]
                };

                for(int c=0;c<6;c++)
                {
                    QTableWidgetItem *item=new QTableWidgetItem(row[c]);
                    item->setTextAlignment(Qt::AlignCenter);

                    if(c==4)
                        item->setForeground(QColor(
                            d[5]=="Emergency"?"#DC2626":
                            d[5]=="Urgent"?"#D97706":"#15803D"));

                    if(c==5)
                        item->setForeground(QColor(
                            d[6]=="Fulfilled"?"#15803D":
                            d[6]=="Cancelled"?"#64748B":
                            d[6]=="In Progress"?"#2563EB":"#D97706"));

                    table->setItem(r,c,item);
                }

                QComboBox *status=new QComboBox;
                status->addItems({"Pending","Matched","In Progress",
                                  "Fulfilled","Cancelled"});
                if(status->findText(d[6])==-1) status->addItem(d[6]);
                status->setCurrentText(d[6]);

                QPushButton *update=new QPushButton("Update");
                update->setStyleSheet(
                    "background:#2563EB;color:white;padding:7px;"
                    "border-radius:5px;font-weight:bold;"
                );

                QWidget *cell=new QWidget;
                QHBoxLayout *cl=new QHBoxLayout(cell);
                cl->setContentsMargins(3,3,3,3);
                cl->addWidget(status);
                cl->addWidget(update);
                table->setCellWidget(r,6,cell);

                QString id=d[0],old=d[6];

                QObject::connect(update,&QPushButton::clicked,page,[=]()
                {
                    QString next=status->currentText();
                    if(next==old) return;

                    if(next=="Fulfilled" &&
                       QMessageBox::question(page,"Confirm",
                       "Has the hospital confirmed blood receipt?",
                       QMessageBox::Yes|QMessageBox::No)!=QMessageBox::Yes)
                        return;

                    if(next=="Cancelled" &&
                       QMessageBox::question(page,"Cancel Request",
                       "Are you sure you want to cancel this request?",
                       QMessageBox::Yes|QMessageBox::No)!=QMessageBox::Yes)
                        return;

                    if(saveStatus(path+"requests.txt",id,next))
                    {
                        QMessageBox::information(page,"Success","Status updated!");
                        QTimer::singleShot(0,page,[=](){refreshBtn->click();});
                    }
                    else
                        QMessageBox::warning(page,"Error","Update failed!");
                });

                table->setRowHeight(r,48);
            }
        }
        refreshStats();
    };

    QObject::connect(refreshBtn,&QPushButton::clicked,page,refresh);

    bl->addLayout(top);
    bl->addWidget(table);
    main->addWidget(header);
    main->addLayout(grid);
    main->addWidget(box,1);

    refresh();

    QTimer *timer=new QTimer(page);
    QObject::connect(timer,&QTimer::timeout,page,refreshStats);
    timer->start(5000);

    return page;
}