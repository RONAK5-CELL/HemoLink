#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QHash>
#include <QTimer>
#include <QDateTime>
#include <QAbstractItemView>
#include <QStringList>
#include <algorithm>

// TABLE CELL
static QLabel* radarCell(QString text,QString bg,QString fg="#172554")
{
    QLabel *l=new QLabel(text);
    l->setAlignment(Qt::AlignCenter);
    l->setWordWrap(true);
    l->setStyleSheet("background:"+bg+";color:"+fg+
        ";padding:7px;border-radius:6px;font-size:12px;font-weight:600;");
    return l;
}

// SUMMARY CARD
static QFrame* radarCard(QString name,QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setObjectName("radarCard");
    f->setMinimumHeight(90);
    f->setStyleSheet(
        "QFrame#radarCard{background:white;border:1px solid #E2E8F0;"
        "border-radius:12px;}"
        "QFrame#radarCard:hover{border:1px solid #94A3B8;}"
    );

    QVBoxLayout *v=new QVBoxLayout(f);
    QLabel *t=new QLabel(name);
    value=new QLabel("0");

    t->setStyleSheet("color:#64748B;font-size:11px;font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+";font-size:27px;font-weight:bold;border:none;");

    v->addWidget(t);
    v->addWidget(value);
    return f;
}

// INSIGHT CARD
static QFrame* insightCard(QString name,QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setStyleSheet("background:white;border:1px solid #E2E8F0;border-radius:10px;");
    QVBoxLayout *v=new QVBoxLayout(f);

    QLabel *t=new QLabel(name);
    value=new QLabel("0");

    t->setStyleSheet("color:#64748B;font-size:11px;font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+";font-size:23px;font-weight:bold;border:none;");

    v->addWidget(t);
    v->addWidget(value);
    return f;
}

static int radarPriority(QString p)
{
    if(p.compare("Emergency",Qt::CaseInsensitive)==0 ||
       p.compare("Critical",Qt::CaseInsensitive)==0) return 0;
    if(p.compare("Urgent",Qt::CaseInsensitive)==0) return 1;
    return 2;
}

// MAIN PAGE
QWidget* createLiveEmergencyRadarPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *layout=new QVBoxLayout(page);
    layout->setContentsMargins(20,15,20,15);
    layout->setSpacing(11);

    QString path=QCoreApplication::applicationDirPath()+"/";

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:13px;"
    );

    QHBoxLayout *hl=new QHBoxLayout(header);
    QVBoxLayout *ht=new QVBoxLayout;

    QLabel *title=new QLabel("Live Emergency Radar");
    QLabel *sub=new QLabel("Hospital emergency monitoring and response dashboard");

    title->setStyleSheet("color:white;font-size:24px;font-weight:bold;");
    sub->setStyleSheet("color:#CBD5E1;font-size:12px;");

    QLabel *live=new QLabel("● LIVE MONITORING");
    live->setStyleSheet("background:#DCFCE7;color:#15803D;"
                        "padding:9px;border-radius:7px;font-weight:bold;");

    ht->addWidget(title);
    ht->addWidget(sub);
    hl->addLayout(ht);
    hl->addStretch();
    hl->addWidget(live);

    // SUMMARY CARDS
    QLabel *critical,*urgent,*active,*fulfilled;
    QHBoxLayout *cards=new QHBoxLayout;
    cards->setSpacing(11);

    cards->addWidget(radarCard("CRITICAL REQUESTS",critical,"#DC2626"));
    cards->addWidget(radarCard("URGENT REQUESTS",urgent,"#EA580C"));
    cards->addWidget(radarCard("ACTIVE REQUESTS",active,"#2563EB"));
    cards->addWidget(radarCard("FULFILLED",fulfilled,"#059669"));

    // TABLE HEADER
    QHBoxLayout *top=new QHBoxLayout;
    QLabel *heading=new QLabel("Emergency Request Monitor");
    QLabel *updated=new QLabel("Updated: --");

    heading->setStyleSheet("font-size:19px;font-weight:bold;");
    updated->setStyleSheet("color:#64748B;font-size:12px;");

    QPushButton *refreshBtn=new QPushButton("↻ Refresh Radar");
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setStyleSheet(
        "QPushButton{background:#2563EB;color:white;padding:9px 15px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#1D4ED8;}"
    );

    top->addWidget(heading);
    top->addStretch();
    top->addWidget(updated);
    top->addWidget(refreshBtn);

    // TABLE
    QFrame *box=new QFrame;
    box->setStyleSheet("background:white;border-radius:12px;");
    QVBoxLayout *bl=new QVBoxLayout(box);

    QTableWidget *table=new QTableWidget(0,9);
    table->setHorizontalHeaderLabels({
        "Request ID","Hospital","Location","Blood",
        "Required","Available","Shortage","Priority","Status"
    });

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setShowGrid(false);
    table->setStyleSheet(
        "QTableWidget{background:white;border:none;font-size:12px;}"
        "QHeaderView::section{background:#172554;color:white;"
        "padding:11px 4px;border:none;font-weight:bold;}"
    );

    // LIVE RESPONSE INSIGHTS
    QFrame *insights=new QFrame;
    insights->setStyleSheet("background:#E8EEF9;border-radius:12px;");
    QVBoxLayout *iv=new QVBoxLayout(insights);

    QLabel *insightTitle=new QLabel("Live Response Insights");
    insightTitle->setStyleSheet("font-size:17px;font-weight:bold;color:#172554;");

    QLabel *shortageCount,*coveredCount,*requiredUnits;
    QHBoxLayout *ib=new QHBoxLayout;
    ib->setSpacing(10);

    ib->addWidget(insightCard("REQUESTS WITH SHORTAGE",shortageCount,"#DC2626"));
    ib->addWidget(insightCard("NO CURRENT STOCK GAP",coveredCount,"#059669"));
    ib->addWidget(insightCard("OPEN UNITS REQUIRED",requiredUnits,"#2563EB"));

    QHBoxLayout *legend=new QHBoxLayout;
    legend->addWidget(radarCell("● Emergency","#FEE2E2","#B91C1C"));
    legend->addWidget(radarCell("● Urgent","#FFEDD5","#C2410C"));
    legend->addWidget(radarCell("● Normal","#DCFCE7","#15803D"));
    legend->addWidget(radarCell("● Fulfilled","#DBEAFE","#1D4ED8"));

    QLabel *note=new QLabel("Stock gap is an indicative comparison with current inventory.");
    note->setStyleSheet("color:#64748B;font-size:10px;border:none;");

    iv->addWidget(insightTitle);
    iv->addLayout(ib);
    iv->addLayout(legend);
    iv->addWidget(note);

    // REFRESH DATA
    auto refresh=[=]()
    {
        table->clearContents();
        table->setRowCount(0);

        QHash<QString,QStringList> hospitals;
        QHash<QString,int> stock;

        QFile hf(path+"hospitals.txt");
        if(hf.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&hf);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()>=3) hospitals[d[0]]={d[1],d[2]};
            }
        }

        QFile sf(path+"blood_stock.txt");
        if(sf.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&sf);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()==2) stock[d[0].trimmed()]=d[1].toInt();
            }
        }

        QList<QStringList> requests;
        QFile rf(path+"requests.txt");
        if(rf.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&rf);
            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()==7) requests.append(d);
            }
        }

        std::stable_sort(requests.begin(),requests.end(),
            [](const QStringList &a,const QStringList &b)
            { return radarPriority(a[5])<radarPriority(b[5]); });

        int c=0,u=0,a=0,f=0,gapCount=0,covered=0,totalRequired=0;

        for(const QStringList &d:requests)
        {
            bool done=d[6].compare("Fulfilled",Qt::CaseInsensitive)==0;

            if(done) f++;
            else
            {
                a++;
                if(radarPriority(d[5])==0) c++;
                if(radarPriority(d[5])==1) u++;
            }

            QStringList h=hospitals.value(d[1]);
            QString hospital=h.isEmpty()?d[1]:h[0];
            QString city=h.size()>1?h[1]:"Unknown";

            int required=d[3].toInt();
            int available=stock.value(d[2],0);
            int shortage=qMax(0,required-available);

            if(!done)
            {
                totalRequired+=required;
                if(shortage>0) gapCount++;
                else covered++;
            }

            QStringList row={d[0],hospital,city,d[2],
                QString::number(required),QString::number(available),
                QString::number(shortage),d[5],d[6]};

            int r=table->rowCount();
            table->insertRow(r);

            for(int i=0;i<9;i++)
            {
                QString bg=r%2?"#F8FAFC":"#FFFFFF",fg="#172554";

                if(i==7)
                {
                    bg=radarPriority(d[5])==0?"#FEE2E2":
                       radarPriority(d[5])==1?"#FFEDD5":"#DCFCE7";
                    fg=radarPriority(d[5])==0?"#B91C1C":
                       radarPriority(d[5])==1?"#C2410C":"#15803D";
                }
                if(i==6 && shortage>0){bg="#FEE2E2";fg="#B91C1C";}
                if(i==8)
                {
                    bg=done?"#DCFCE7":d[6]=="Matched"?"#DBEAFE":"#FEF3C7";
                    fg=done?"#15803D":d[6]=="Matched"?"#1D4ED8":"#92400E";
                }

                table->setCellWidget(r,i,radarCell(row[i],bg,fg));
            }
            table->setRowHeight(r,45);
        }

        critical->setText(QString::number(c));
        urgent->setText(QString::number(u));
        active->setText(QString::number(a));
        fulfilled->setText(QString::number(f));

        shortageCount->setText(QString::number(gapCount));
        coveredCount->setText(QString::number(covered));
        requiredUnits->setText(QString::number(totalRequired));

        if(requests.isEmpty())
        {
            table->insertRow(0);
            table->setSpan(0,0,1,9);
            table->setCellWidget(0,0,
                radarCell("No emergency requests available","#DBEAFE"));
        }

        updated->setText("Updated: "+
            QDateTime::currentDateTime().toString("hh:mm:ss AP"));
    };

    QObject::connect(refreshBtn,&QPushButton::clicked,page,refresh);

    QTimer *timer=new QTimer(page);
    QObject::connect(timer,&QTimer::timeout,page,refresh);
    timer->start(5000);

    bl->addWidget(table);

    layout->addWidget(header);
    layout->addLayout(cards);
    layout->addLayout(top);
    layout->addWidget(box,4);
    layout->addWidget(insights,2);

    refresh();
    return page;
}