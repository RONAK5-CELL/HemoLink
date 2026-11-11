#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QTimer>
#include <QScrollArea>
#include <QDateTime>
#include <QLayoutItem>

// NOTIFICATION CARD
static QFrame* notificationBox(QString title,QString detail,QString color)
{
    QFrame *f=new QFrame;
    f->setStyleSheet("background:white;border:1px solid #E2E8F0;border-radius:11px;");

    QHBoxLayout *h=new QHBoxLayout(f);
    h->setContentsMargins(10,9,14,9);

    QFrame *line=new QFrame;
    line->setFixedWidth(5);
    line->setStyleSheet("background:"+color+";border-radius:3px;");

    QVBoxLayout *v=new QVBoxLayout;
    QLabel *t=new QLabel(title),*d=new QLabel(detail);

    t->setStyleSheet("font-size:13px;font-weight:bold;color:#172554;border:none;");
    d->setWordWrap(true);
    d->setStyleSheet("font-size:12px;color:#64748B;border:none;");

    v->addWidget(t);
    v->addWidget(d);
    h->addWidget(line);
    h->addLayout(v,1);
    return f;
}

// SUMMARY CARD
static QFrame* summaryCard(QString title,QString icon,QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setObjectName("summary");
    f->setStyleSheet(
        "QFrame#summary{background:white;border:1px solid #E2E8F0;border-radius:12px;}"
        "QFrame#summary:hover{border:1px solid "+color+";}"
    );

    QVBoxLayout *v=new QVBoxLayout(f);
    QLabel *t=new QLabel(icon+"  "+title);
    value=new QLabel("0");

    t->setStyleSheet("color:#64748B;font-size:11px;font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+";font-size:29px;font-weight:bold;border:none;");

    v->addWidget(t);
    v->addWidget(value);
    return f;
}

// HEALTH CARD
static QFrame* healthCard(QString title,QLabel *&value,
                          QLabel *&detail,QString color)
{
    QFrame *f=new QFrame;
    f->setStyleSheet("background:white;border:1px solid #E2E8F0;border-radius:11px;");

    QVBoxLayout *v=new QVBoxLayout(f);
    QLabel *t=new QLabel(title);
    value=new QLabel("--");
    detail=new QLabel("");

    t->setStyleSheet("color:#64748B;font-size:11px;font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+";font-size:19px;font-weight:bold;border:none;");
    detail->setStyleSheet("color:#64748B;font-size:11px;border:none;");

    v->addWidget(t);
    v->addWidget(value);
    v->addWidget(detail);
    return f;
}

// MAIN PAGE
QWidget* createNotificationCenterPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;");

    QVBoxLayout *layout=new QVBoxLayout(page);
    layout->setContentsMargins(20,15,20,15);
    layout->setSpacing(12);

    QString path=QCoreApplication::applicationDirPath()+"/";

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:13px;"
    );

    QHBoxLayout *hl=new QHBoxLayout(header);
    QVBoxLayout *ht=new QVBoxLayout;

    QLabel *title=new QLabel("Notification Center");
    QLabel *sub=new QLabel("Blood bank alerts and system activity monitoring.");
    QLabel *live=new QLabel("● LIVE MONITORING");

    title->setStyleSheet("color:white;font-size:25px;font-weight:bold;");
    sub->setStyleSheet("color:#CBD5E1;font-size:12px;");
    live->setStyleSheet("background:#DCFCE7;color:#15803D;padding:9px;"
                        "border-radius:7px;font-weight:bold;");

    ht->addWidget(title);
    ht->addWidget(sub);
    hl->addLayout(ht);
    hl->addStretch();
    hl->addWidget(live);

    // SUMMARY CARDS
    QLabel *total,*critical,*pending;

    QHBoxLayout *cards=new QHBoxLayout;
    cards->setSpacing(12);

    cards->addWidget(summaryCard("TOTAL ALERTS","●",total,"#2563EB"));
    cards->addWidget(summaryCard("CRITICAL ALERTS","!",critical,"#DC2626"));
    cards->addWidget(summaryCard("PENDING REQUESTS","◷",pending,"#D97706"));

    // FEED HEADER
    QHBoxLayout *top=new QHBoxLayout;
    QLabel *heading=new QLabel("Recent Notifications");
    QLabel *updated=new QLabel("Updated: --");

    QPushButton *refreshBtn=new QPushButton("↻ Refresh Alerts");

    heading->setStyleSheet("font-size:19px;font-weight:bold;color:#172554;");
    updated->setStyleSheet("color:#64748B;font-size:11px;");

    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setStyleSheet(
        "QPushButton{background:#2563EB;color:white;padding:9px 14px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#1D4ED8;}"
    );

    top->addWidget(heading);
    top->addStretch();
    top->addWidget(updated);
    top->addWidget(refreshBtn);

    // SCROLLABLE NOTIFICATIONS
    QScrollArea *scroll=new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setMinimumHeight(170);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background:transparent;border:none;");

    QWidget *content=new QWidget;
    QVBoxLayout *feed=new QVBoxLayout(content);
    feed->setSpacing(9);
    scroll->setWidget(content);

    // SYSTEM HEALTH
    QFrame *overview=new QFrame;
    overview->setStyleSheet("background:#E8EEF9;border-radius:12px;");

    QVBoxLayout *ov=new QVBoxLayout(overview);

    QLabel *ovTitle=new QLabel("System Health Overview");
    ovTitle->setStyleSheet("font-size:18px;font-weight:bold;color:#172554;");

    QLabel *stockValue,*stockDetail,*requestValue,*requestDetail;
    QLabel *donationValue,*donationDetail;

    QHBoxLayout *health=new QHBoxLayout;
    health->setSpacing(10);

    health->addWidget(healthCard("INVENTORY HEALTH",stockValue,stockDetail,"#059669"));
    health->addWidget(healthCard("REQUEST QUEUE",requestValue,requestDetail,"#D97706"));
    health->addWidget(healthCard("DONATION ACTIVITY",donationValue,donationDetail,"#2563EB"));

    ov->addWidget(ovTitle);
    ov->addLayout(health);

    // REFRESH DATA
    auto refresh=[=]()
    {
        while(QLayoutItem *item=feed->takeAt(0))
        {
            if(item->widget()) delete item->widget();
            delete item;
        }

        int all=0,urgent=0,waiting=0,low=0;
        int donations=0,donatedUnits=0,urgentRequests=0;

        auto add=[&](QString t,QString d,QString c)
        {
            feed->addWidget(notificationBox(t,d,c));
            all++;
        };

        // BLOOD STOCK ALERTS
        QFile stock(path+"blood_stock.txt");

        if(stock.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&stock);

            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()!=2) continue;

                int n=d[1].toInt();

                if(n<10)
                {
                    low++;

                    add("LOW STOCK ALERT  |  "+d[0],
                        QString::number(n)+" units currently available.",
                        n<5?"#DC2626":"#D97706");

                    if(n<5) urgent++;
                }
            }
            stock.close();
        }

        // EMERGENCY REQUESTS
        QFile requests(path+"requests.txt");

        if(requests.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&requests);

            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()!=7) continue;

                QString p=d[5].trimmed();
                QString s=d[6].trimmed();

                if(s.compare("Fulfilled",Qt::CaseInsensitive)==0)
                    continue;

                if(s.compare("Pending",Qt::CaseInsensitive)==0)
                    waiting++;

                if(p.compare("Emergency",Qt::CaseInsensitive)==0)
                {
                    urgent++;
                    urgentRequests++;

                    add("EMERGENCY REQUEST  |  "+d[0],
                        "Blood Group: "+d[2]+"  |  Required Units: "+d[3],
                        "#DC2626");
                }
                else if(p.compare("Urgent",Qt::CaseInsensitive)==0)
                    add("URGENT REQUEST  |  "+d[0],
                        "Blood Group: "+d[2]+"  |  Required Units: "+d[3],
                        "#EA580C");
                else if(s.compare("Pending",Qt::CaseInsensitive)==0)
                    add("PENDING REQUEST  |  "+d[0],
                        "Blood Group: "+d[2]+"  |  Required Units: "+d[3],
                        "#2563EB");
            }
            requests.close();
        }

        // DONATION HISTORY
        QFile history(path+"donation_history.txt");

        if(history.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&history);

            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');

                if(d.size()==6)
                {
                    donations++;
                    donatedUnits+=d[4].toInt();
                }
            }
            history.close();
        }

        if(donations)
            add("DONATION ACTIVITY UPDATED",
                QString::number(donations)+" records  |  "+
                QString::number(donatedUnits)+" units donated.",
                "#059669");

        if(all==0)
            feed->addWidget(notificationBox(
                "ALL CLEAR","No current alerts found.","#16A34A"));

        feed->addStretch();

        // UPDATE SUMMARY
        total->setText(QString::number(all));
        critical->setText(QString::number(urgent));
        pending->setText(QString::number(waiting));

        // UPDATE HEALTH CARDS
        stockValue->setText(low==0?"Healthy":"Needs Attention");
        stockDetail->setText(QString::number(low)+" groups below 10 units");

        requestValue->setText(QString::number(waiting)+" Pending");
        requestDetail->setText(QString::number(urgentRequests)+" emergency requests");

        donationValue->setText(QString::number(donatedUnits)+" Units");
        donationDetail->setText(QString::number(donations)+" donation records");

        updated->setText("Last Updated: "+
            QDateTime::currentDateTime().toString("hh:mm:ss AP"));
    };

    // MANUAL AND AUTOMATIC REFRESH
    QObject::connect(refreshBtn,&QPushButton::clicked,page,refresh);

    QTimer *timer=new QTimer(page);
    QObject::connect(timer,&QTimer::timeout,page,refresh);
    timer->start(5000);

    refresh();

    // FINAL LAYOUT
    layout->addWidget(header);
    layout->addLayout(cards);
    layout->addLayout(top);
    layout->addWidget(scroll,1);
    layout->addWidget(overview);

    return page;
}