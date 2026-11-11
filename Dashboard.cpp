#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QStringList>

// COUNT RECORDS
static int countFile(QString name,bool match=false)
{
    QFile f(QCoreApplication::applicationDirPath()+"/"+name);
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text)) return 0;

    QTextStream in(&f);
    int count=0;

    while(!in.atEnd())
    {
        QString s=in.readLine().trimmed();
        if(!s.isEmpty() && (!match ||
           s.contains("Matched",Qt::CaseInsensitive) ||
           s.contains("Completed",Qt::CaseInsensitive)))
            count++;
    }
    return count;
}

// TOTAL BLOOD UNITS
static int getUnits()
{
    QFile f(QCoreApplication::applicationDirPath()+"/blood_stock.txt");
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text)) return 0;

    QTextStream in(&f);
    int total=0;

    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()>=2) total+=d.last().trimmed().toInt();
    }
    return total;
}

// PREMIUM STATISTICS CARD
static QFrame* dashboardCard(QString title,QString icon,
                             QString value,QString bg,QString color)
{
    QFrame *card=new QFrame;
    card->setObjectName("statCard");
    card->setMinimumHeight(130);

    card->setStyleSheet(
        "QFrame#statCard{background:"+bg+
        ";border:1px solid #E2E8F0;border-radius:14px;}"
        "QFrame#statCard:hover{border:2px solid "+color+";}"
    );

    QVBoxLayout *box=new QVBoxLayout(card);
    box->setContentsMargins(10,7,10,7);
    box->setSpacing(3);

    QLabel *symbol=new QLabel(icon);
    QLabel *name=new QLabel(title);
    QLabel *number=new QLabel(value);

    symbol->setAlignment(Qt::AlignCenter);
    name->setAlignment(Qt::AlignCenter);
    number->setAlignment(Qt::AlignCenter);

    symbol->setStyleSheet(
        "font-size:28px;color:"+color+";border:none;background:transparent;"
    );

    name->setStyleSheet(
        "font-size:15px;font-weight:bold;color:#334155;"
        "border:none;background:transparent;"
    );

    number->setStyleSheet(
        "font-size:40px;font-weight:bold;color:"+color+
        ";border:none;background:transparent;"
    );

    symbol->setAttribute(Qt::WA_TransparentForMouseEvents);
    name->setAttribute(Qt::WA_TransparentForMouseEvents);
    number->setAttribute(Qt::WA_TransparentForMouseEvents);

    box->addStretch();
    box->addWidget(symbol);
    box->addWidget(name);
    box->addWidget(number);
    box->addStretch();

    return card;
}

// QUICK ACTION BUTTON
static QPushButton* actionButton(QString text,QString color)
{
    QPushButton *b=new QPushButton(text);
    b->setMinimumHeight(48);
    b->setCursor(Qt::PointingHandCursor);

    b->setStyleSheet(
        "QPushButton{background:"+color+";color:white;"
        "border:none;border-radius:9px;font-size:13px;"
        "font-weight:bold;padding:8px;}"
        "QPushButton:hover{background:#172554;}"
        "QPushButton:pressed{background:#0F172A;}"
    );

    return b;
}

// MODULE CARD
static QPushButton* moduleCard(QString title,QString detail,
                               QString icon,QString color)
{
    QPushButton *b=new QPushButton;
    b->setMinimumHeight(76);
    b->setCursor(Qt::PointingHandCursor);
    b->setText(icon+"  "+title+"\n"+detail);

    b->setStyleSheet(
        "QPushButton{text-align:left;background:white;"
        "color:#172554;border:1px solid #E2E8F0;"
        "border-left:5px solid "+color+";"
        "border-radius:9px;padding:12px;"
        "font-size:13px;font-weight:bold;}"
        "QPushButton:hover{background:"+color+";color:white;}"
    );

    return b;
}

// MAIN DASHBOARD
void MainWindow::createDashboard()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *layout=new QVBoxLayout(page);
    layout->setContentsMargins(22,17,22,17);
    layout->setSpacing(13);

    // TOP HEADER
    QHBoxLayout *top=new QHBoxLayout;
    QVBoxLayout *headingBox=new QVBoxLayout;

    QLabel *title=new QLabel("Dashboard");
    QLabel *sub=new QLabel("Your blood bank management overview.");

    title->setStyleSheet("font-size:29px;font-weight:bold;color:#172554;");
    sub->setStyleSheet("font-size:13px;color:#64748B;");

    QLabel *status=new QLabel("● SYSTEM ACTIVE");
    status->setAlignment(Qt::AlignCenter);
    status->setStyleSheet(
        "background:#DCFCE7;color:#15803D;"
        "padding:10px 14px;border-radius:8px;font-weight:bold;"
    );

    headingBox->addWidget(title);
    headingBox->addWidget(sub);

    top->addLayout(headingBox);
    top->addStretch();
    top->addWidget(status);

    // WELCOME BANNER
    QFrame *banner=new QFrame;
    banner->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:15px;"
    );

    QHBoxLayout *bannerLayout=new QHBoxLayout(banner);
    bannerLayout->setContentsMargins(22,17,22,17);

    QVBoxLayout *bannerText=new QVBoxLayout;

    QLabel *welcome=new QLabel("HEMOLINK");
    QLabel *tagline=new QLabel("Connecting Donors. Saving Lives.");
    QLabel *description=new QLabel(
        "Intelligent Blood Bank & Emergency Matching System"
    );

    welcome->setStyleSheet("color:white;font-size:28px;font-weight:bold;border:none;");
    tagline->setStyleSheet("color:#DBEAFE;font-size:17px;font-weight:600;border:none;");
    description->setStyleSheet("color:#BFDBFE;font-size:12px;border:none;");

    bannerText->addWidget(welcome);
    bannerText->addWidget(tagline);
    bannerText->addWidget(description);

    // RIGHT BADGE
    QLabel *bannerTag=new QLabel("SMART\nBLOOD BANK");
    bannerTag->setAlignment(Qt::AlignCenter);
    bannerTag->setMinimumSize(125,65);
    bannerTag->setStyleSheet(
        "background:#0F172A;color:white;"
        "border:1px solid #93C5FD;border-radius:11px;"
        "padding:12px;font-size:12px;font-weight:bold;"
    );

    bannerLayout->addLayout(bannerText);
    bannerLayout->addStretch();
    bannerLayout->addWidget(bannerTag);

    // FOUR STATISTICS CARDS
    QGridLayout *grid=new QGridLayout;
    grid->setSpacing(13);

    grid->addWidget(dashboardCard(
        "TOTAL DONORS","♟",
        QString::number(countFile("donors.txt")),
        "#DBEAFE","#2563EB"),0,0);

    grid->addWidget(dashboardCard(
        "BLOOD UNITS","✚",
        QString::number(getUnits()),
        "#EDE9FE","#7C3AED"),0,1);

    grid->addWidget(dashboardCard(
        "EMERGENCY REQUESTS","⚠",
        QString::number(countFile("requests.txt")),
        "#FEE2E2","#DC2626"),0,2);

    grid->addWidget(dashboardCard(
        "SUCCESSFUL MATCHES","✓",
        QString::number(countFile("requests.txt",true)),
        "#DCFCE7","#059669"),0,3);

    // QUICK ACTIONS
    QLabel *quick=new QLabel("Quick Actions");
    quick->setStyleSheet("font-size:19px;font-weight:bold;color:#172554;");

    QHBoxLayout *buttons=new QHBoxLayout;
    buttons->setSpacing(12);

    QStringList names={
        "♟  Register Donor",
        "✚  Blood Inventory",
        "⚠  New Request"
    };

    QStringList colors={"#2563EB","#0891B2","#DC2626"};

    for(int i=0;i<3;i++)
    {
        QPushButton *btn=actionButton(names[i],colors[i]);

        connect(btn,&QPushButton::clicked,this,[this,i]()
        {
            changePage(i+1);
        });

        buttons->addWidget(btn);
    }

    // SYSTEM MODULES
    QLabel *moduleTitle=new QLabel("System Modules");
    moduleTitle->setStyleSheet("font-size:19px;font-weight:bold;color:#172554;");

    QGridLayout *modules=new QGridLayout;
    modules->setSpacing(11);

    QStringList moduleNames={
        "Donor Management","Blood Inventory",
        "Emergency Requests","Smart Donor Matching"
    };

    QStringList details={
        "Manage registered donors",
        "Monitor available blood stock",
        "Manage hospital requirements",
        "Find available blood donors"
    };

    QStringList icons={"♟","✚","⚠","⌕"};
    QStringList moduleColors={
        "#2563EB","#0891B2","#DC2626","#059669"
    };

    for(int i=0;i<4;i++)
    {
        QPushButton *b=moduleCard(
            moduleNames[i],details[i],icons[i],moduleColors[i]
        );

        connect(b,&QPushButton::clicked,this,[this,i]()
        {
            changePage(i+1);
        });

        modules->addWidget(b,i/2,i%2);
    }

    // FOOTER
    QFrame *footerBox=new QFrame;
    footerBox->setStyleSheet("background:#172554;border-radius:11px;");

    QHBoxLayout *footerLayout=new QHBoxLayout(footerBox);

    QLabel *footer=new QLabel(
        "HEMOLINK  |  Connecting Hospitals, Donors & Communities"
    );

    QLabel *version=new QLabel("PROJECT DASHBOARD  •  ACTIVE");

    footer->setStyleSheet("color:white;font-size:12px;font-weight:bold;border:none;");
    version->setStyleSheet("color:#BFDBFE;font-size:10px;border:none;");

    footerLayout->addWidget(footer);
    footerLayout->addStretch();
    footerLayout->addWidget(version);

    // FINAL LAYOUT
    layout->addLayout(top);
    layout->addWidget(banner);
    layout->addLayout(grid);
    layout->addWidget(quick);
    layout->addLayout(buttons);
    layout->addWidget(moduleTitle);
    layout->addLayout(modules);
    layout->addWidget(footerBox);

    pages->addWidget(page);
}