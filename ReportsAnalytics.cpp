#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QProgressBar>
#include <QScrollArea>
#include <QCoreApplication>
#include <QDateTime>
#include <QFont>
#include <QColor>

// BLOOD GROUPS
static QStringList groups={"A+","A-","B+","B-","AB+","AB-","O+","O-"};
static QStringList colors={"#2563EB","#0891B2","#059669","#D97706",
                           "#7C3AED","#DB2777","#EA580C","#DC2626"};

// FILE OPEN
static bool reportFile(QFile &f,QString name)
{
    f.setFileName(QCoreApplication::applicationDirPath()+"/"+name);
    if(f.open(QIODevice::ReadOnly|QIODevice::Text)) return true;
    f.setFileName(name);
    return f.open(QIODevice::ReadOnly|QIODevice::Text);
}

// READ DONORS
static void reportDonors(int a[])
{
    QFile f;
    if(!reportFile(f,"donors.txt")) return;
    QTextStream in(&f);

    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()==8 && d[7].trimmed().compare("Yes",Qt::CaseInsensitive)==0)
            for(int i=0;i<8;i++)
                if(d[3].trimmed().compare(groups[i],Qt::CaseInsensitive)==0)
                    a[i]++;
    }
}

// READ STOCK
static void reportStock(int a[])
{
    QFile f;
    if(!reportFile(f,"blood_stock.txt")) return;
    QTextStream in(&f);

    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()==2)
            for(int i=0;i<8;i++)
                if(d[0].trimmed()==groups[i])
                    a[i]+=d[1].toInt();
    }
}

// READ REQUESTS
static void reportRequests(int a[],int &pending,int &emergency,
                           int &urgent,int &normal)
{
    QFile f;
    if(!reportFile(f,"requests.txt")) return;
    QTextStream in(&f);

    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()!=7) continue;

        for(int i=0;i<8;i++)
            if(d[2].trimmed()==groups[i])
                a[i]+=d[3].toInt();

        if(d[6].compare("Pending",Qt::CaseInsensitive)==0)
        {
            pending++;
            if(d[5]=="Emergency") emergency++;
            else if(d[5]=="Urgent") urgent++;
            else normal++;
        }
    }
}

// BAR GRAPH
class ReportChart : public QWidget
{
    int values[8]={};

public:
    ReportChart(int a[],QWidget *parent=nullptr):QWidget(parent)
    {
        setMinimumHeight(210);
        setValues(a);
    }

    void setValues(int a[])
    {
        for(int i=0;i<8;i++) values[i]=a[i];
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int max=1;
        for(int i=0;i<8;i++) if(values[i]>max) max=values[i];

        int left=38,bottom=height()-38,top=20;
        int chartH=bottom-top;
        int barW=(width()-left-15)/8;

        for(int i=0;i<=4;i++)
        {
            int y=bottom-i*chartH/4;
            p.setPen(QColor("#E2E8F0"));
            p.drawLine(left,y,width()-10,y);
            p.setPen(QColor("#64748B"));
            p.drawText(0,y-8,32,18,Qt::AlignRight,
                       QString::number(max*i/4));
        }

        for(int i=0;i<8;i++)
        {
            int h=values[i]*chartH/max;
            int x=left+i*barW;

            p.setPen(Qt::NoPen);
            p.setBrush(QColor(colors[i]));
            p.drawRoundedRect(x+4,bottom-h,barW-9,qMax(2,h),5,5);

            p.setPen(QColor("#172554"));
            p.setFont(QFont("Arial",9,QFont::Bold));
            p.drawText(x,bottom-h-19,barW,18,Qt::AlignCenter,
                       QString::number(values[i]));
            p.drawText(x,bottom+7,barW,20,Qt::AlignCenter,groups[i]);
        }
    }
};

// STAT CARD
static QFrame* reportMetric(QString title,QString icon,
                            QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                     "border-radius:12px;");
    QVBoxLayout *v=new QVBoxLayout(f);

    QLabel *name=new QLabel(icon+"  "+title);
    value=new QLabel("0");

    name->setStyleSheet("color:#64748B;font-size:11px;"
                        "font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+
                         ";font-size:29px;font-weight:bold;border:none;");

    v->addWidget(name);
    v->addWidget(value);
    return f;
}

// CHART PANEL
static QFrame* chartPanel(QString title,ReportChart *&chart,int data[])
{
    QFrame *f=new QFrame;
    f->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                     "border-radius:12px;");
    QVBoxLayout *v=new QVBoxLayout(f);

    QLabel *name=new QLabel(title);
    name->setStyleSheet("font-size:16px;font-weight:bold;color:#172554;border:none;");

    chart=new ReportChart(data);
    v->addWidget(name);
    v->addWidget(chart);
    return f;
}

// INSIGHT CARD
static QFrame* insightBox(QString title,QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                     "border-radius:10px;");
    QVBoxLayout *v=new QVBoxLayout(f);

    QLabel *name=new QLabel(title);
    value=new QLabel("--");

    name->setStyleSheet("color:#64748B;font-size:11px;"
                        "font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+
                         ";font-size:19px;font-weight:bold;border:none;");

    v->addWidget(name);
    v->addWidget(value);
    return f;
}

// MAIN REPORT PAGE
QWidget* createReportsAnalyticsPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *outer=new QVBoxLayout(page);
    QScrollArea *scroll=new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *content=new QWidget;
    QVBoxLayout *layout=new QVBoxLayout(content);
    layout->setContentsMargins(20,15,20,15);
    layout->setSpacing(13);

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:13px;");

    QHBoxLayout *hl=new QHBoxLayout(header);
    QVBoxLayout *ht=new QVBoxLayout;

    QLabel *title=new QLabel("Reports & Analytics");
    QLabel *sub=new QLabel("Visual insights into blood bank performance.");

    title->setStyleSheet("color:white;font-size:25px;font-weight:bold;border:none;");
    sub->setStyleSheet("color:#CBD5E1;font-size:12px;border:none;");

    QLabel *badge=new QLabel("ANALYTICS DASHBOARD");
    badge->setStyleSheet("background:#DCFCE7;color:#15803D;"
                         "padding:9px;border-radius:7px;font-weight:bold;");

    QPushButton *refresh=new QPushButton("↻ Refresh");
    refresh->setStyleSheet("background:#FFFFFF;color:#1D4ED8;"
                           "padding:9px 13px;border-radius:7px;font-weight:bold;");

    ht->addWidget(title);
    ht->addWidget(sub);
    hl->addLayout(ht);
    hl->addStretch();
    hl->addWidget(badge);
    hl->addWidget(refresh);

    // STATISTICS
    QLabel *donorValue,*stockValue,*pendingValue,*emergencyValue;
    QHBoxLayout *stats=new QHBoxLayout;
    stats->setSpacing(10);

    stats->addWidget(reportMetric("AVAILABLE DONORS","♟",donorValue,"#2563EB"));
    stats->addWidget(reportMetric("BLOOD UNITS","✚",stockValue,"#059669"));
    stats->addWidget(reportMetric("PENDING REQUESTS","◷",pendingValue,"#D97706"));
    stats->addWidget(reportMetric("EMERGENCY","⚠",emergencyValue,"#DC2626"));

    // DATA
    int donors[8]={},stock[8]={},demand[8]={};
    int pending=0,emergency=0,urgent=0,normal=0;

    reportDonors(donors);
    reportStock(stock);
    reportRequests(demand,pending,emergency,urgent,normal);

    // CHARTS
    QLabel *donorChartTitle=new QLabel("Visual Performance");
    donorChartTitle->setStyleSheet("font-size:19px;font-weight:bold;");

    ReportChart *donorChart,*stockChart;
    QHBoxLayout *charts=new QHBoxLayout;
    charts->setSpacing(12);

    charts->addWidget(chartPanel("Donors by Blood Group",donorChart,donors));
    charts->addWidget(chartPanel("Inventory by Blood Group",stockChart,stock));

    // DEMAND DISTRIBUTION
    QFrame *demandPanel=new QFrame;
    demandPanel->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                               "border-radius:12px;");
    QVBoxLayout *dv=new QVBoxLayout(demandPanel);

    QLabel *dh=new QLabel("Blood Demand Distribution");
    dh->setStyleSheet("font-size:17px;font-weight:bold;border:none;");

    QProgressBar *bars[8];
    QGridLayout *demandGrid=new QGridLayout;

    int maxDemand=1;
    for(int i=0;i<8;i++) if(demand[i]>maxDemand) maxDemand=demand[i];

    for(int i=0;i<8;i++)
    {
        QVBoxLayout *item=new QVBoxLayout;
        QLabel *name=new QLabel(groups[i]+"  |  "+QString::number(demand[i])+" units");
        name->setStyleSheet("font-size:11px;font-weight:bold;color:#475569;");

        bars[i]=new QProgressBar;
        bars[i]->setRange(0,maxDemand);
        bars[i]->setValue(demand[i]);
        bars[i]->setTextVisible(false);
        bars[i]->setFixedHeight(10);
        bars[i]->setStyleSheet(
            "QProgressBar{background:#E2E8F0;border:none;border-radius:5px;}"
            "QProgressBar::chunk{background:"+colors[i]+";border-radius:5px;}"
        );

        item->addWidget(name);
        item->addWidget(bars[i]);
        demandGrid->addLayout(item,i/4,i%4);
    }

    dv->addWidget(dh);
    dv->addLayout(demandGrid);

    // PRIORITY OVERVIEW
    QHBoxLayout *priority=new QHBoxLayout;
    priority->setSpacing(10);

    priority->addWidget(insightBox("EMERGENCY",donorChartTitle,"#DC2626"));
    // Separate priority labels
    QLabel *urgentValue,*normalValue;
    priority->addWidget(insightBox("URGENT REQUESTS",urgentValue,"#EA580C"));
    priority->addWidget(insightBox("NORMAL REQUESTS",normalValue,"#2563EB"));

    // INVENTORY INSIGHTS
    int totalDonors=0,totalStock=0,low=0,topD=0,topR=0;
    QString lowGroups;

    for(int i=0;i<8;i++)
    {
        totalDonors+=donors[i];
        totalStock+=stock[i];

        if(stock[i]<10)
        {
            low++;
            if(!lowGroups.isEmpty()) lowGroups+=", ";
            lowGroups+=groups[i];
        }

        if(donors[i]>donors[topD]) topD=i;
        if(demand[i]>demand[topR]) topR=i;
    }

    QLabel *mostDonors,*mostDemand,*lowStock;
    QHBoxLayout *insights=new QHBoxLayout;

    insights->addWidget(insightBox("TOP DONOR GROUP",mostDonors,"#2563EB"));
    insights->addWidget(insightBox("HIGHEST DEMAND",mostDemand,"#7C3AED"));
    insights->addWidget(insightBox("LOW STOCK GROUPS",lowStock,"#DC2626"));

    QLabel *watch=new QLabel;
    watch->setWordWrap(true);
    watch->setStyleSheet("background:#FFF7ED;color:#9A3412;"
                         "padding:12px;border-radius:8px;font-weight:bold;");

    // REFRESH DATA
    auto update=[=]()
    {
        int d[8]={},s[8]={},r[8]={};
        int p=0,e=0,u=0,n=0;

        reportDonors(d);
        reportStock(s);
        reportRequests(r,p,e,u,n);

        int td=0,ts=0,lowCount=0,bD=0,bR=0;
        QString lowNames;

        for(int i=0;i<8;i++)
        {
            td+=d[i];
            ts+=s[i];

            if(s[i]<10)
            {
                lowCount++;
                if(!lowNames.isEmpty()) lowNames+=", ";
                lowNames+=groups[i];
            }

            if(d[i]>d[bD]) bD=i;
            if(r[i]>r[bR]) bR=i;

            bars[i]->setValue(r[i]);
            QString label=groups[i]+"  |  "+QString::number(r[i])+" units";
            // Progress bar values refresh with current demand
            bars[i]->setRange(0,qMax(1,*std::max_element(r,r+8)));
        }

        donorValue->setText(QString::number(td));
        stockValue->setText(QString::number(ts));
        pendingValue->setText(QString::number(p));
        emergencyValue->setText(QString::number(e));

        urgentValue->setText(QString::number(u));
        normalValue->setText(QString::number(n));

        mostDonors->setText(td?groups[bD]+"  ("+QString::number(d[bD])+")":"No Data");
        mostDemand->setText(r[bR]?groups[bR]+"  ("+QString::number(r[bR])+" units)":"No Data");
        lowStock->setText(QString::number(lowCount)+" / 8");

        donorChart->setValues(d);
        stockChart->setValues(s);

        watch->setText(lowCount?
            "⚠ LOW STOCK ALERT: "+lowNames:
            "✓ Inventory status: All blood groups have 10 or more units.");
    };

    // INITIAL VALUES
    update();

    // REFRESH BUTTON
    QObject::connect(refresh,&QPushButton::clicked,page,update);

    // FINAL LAYOUT
    layout->addWidget(header);
    layout->addLayout(stats);
    layout->addWidget(donorChartTitle);
    layout->addLayout(charts);
    layout->addWidget(demandPanel);
    layout->addLayout(priority);
    layout->addWidget(new QLabel("Operational Insights"));
    layout->addLayout(insights);
    layout->addWidget(watch);
    layout->addStretch();

    scroll->setWidget(content);
    outer->addWidget(scroll);

    return page;
}