#include "mainwindow.h"
#include "Inventory.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QByteArray>
#include <QStringList>
#include <QLayoutItem>
#include <vector>

// SUMMARY CARD
static QFrame* inventoryStat(QString title,QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setObjectName("stat");
    f->setMinimumHeight(95);
    f->setStyleSheet(
        "QFrame#stat{background:white;border:1px solid #E2E8F0;"
        "border-radius:11px;}"
        "QFrame#stat:hover{border:1px solid "+color+";}"
    );

    QHBoxLayout *h=new QHBoxLayout(f);
    QFrame *line=new QFrame;
    line->setFixedWidth(5);
    line->setStyleSheet("background:"+color+";border-radius:2px;");

    QVBoxLayout *v=new QVBoxLayout;
    QLabel *name=new QLabel(title);
    value=new QLabel("0");

    name->setStyleSheet("color:#64748B;font-size:11px;"
                        "font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+
                         ";font-size:27px;font-weight:bold;border:none;");

    v->addWidget(name);
    v->addWidget(value);
    h->addWidget(line);
    h->addSpacing(9);
    h->addLayout(v);
    h->addStretch();

    return f;
}

// BLOOD GROUP CARD
static QFrame* inventoryCard(QString group,int units,QString accent)
{
    QFrame *card=new QFrame;
    card->setObjectName("bloodCard");
    card->setMinimumHeight(125);

    card->setStyleSheet(
        "QFrame#bloodCard{background:white;border:1px solid #E2E8F0;"
        "border-radius:11px;}"
        "QFrame#bloodCard:hover{border:1px solid "+accent+";"
        "background:#FAFCFF;}"
    );

    QHBoxLayout *main=new QHBoxLayout(card);
    main->setContentsMargins(0,10,12,10);

    QFrame *line=new QFrame;
    line->setFixedWidth(5);
    line->setStyleSheet("background:"+accent+";border-radius:2px;");

    QVBoxLayout *v=new QVBoxLayout;
    v->setSpacing(6);

    QLabel *groupName=new QLabel(group);
    groupName->setAlignment(Qt::AlignCenter);
    groupName->setFixedSize(43,30);
    groupName->setStyleSheet(
        "background:#EFF6FF;color:#1E40AF;border-radius:6px;"
        "font-size:14px;font-weight:bold;"
    );

    QLabel *number=new QLabel(QString::number(units));
    number->setStyleSheet("font-size:29px;font-weight:bold;"
                          "color:#172554;border:none;");

    QLabel *unitText=new QLabel("UNITS IN STOCK");
    unitText->setStyleSheet("font-size:9px;font-weight:bold;"
                            "color:#94A3B8;border:none;");

    QString status=units==0?"OUT OF STOCK":
                   units<10?"LOW STOCK":"AVAILABLE";

    QString statusColor=units==0?"#DC2626":
                        units<10?"#D97706":"#15803D";

    QLabel *badge=new QLabel("●  "+status);
    badge->setStyleSheet("color:"+statusColor+
                         ";font-size:10px;font-weight:bold;border:none;");

    v->addWidget(groupName,0,Qt::AlignLeft);
    v->addWidget(number);
    v->addWidget(unitText);
    v->addWidget(badge);

    main->addWidget(line);
    main->addSpacing(9);
    main->addLayout(v);
    main->addStretch();

    return card;
}

// PAGE DECLARATIONS
QWidget* createEmergencyRequestsPage();
QWidget* createDonorMatchingPage();
QWidget* createReportsAnalyticsPage();
QWidget* createAdminPanelPage();

// MAIN INFO PAGE
QWidget* MainWindow::createInfoPage(QString title,QString description)
{
    if(title=="Emergency Requests")
        return createEmergencyRequestsPage();

    if(title=="Donor Matching")
        return createDonorMatchingPage();

    if(title=="Reports")
        return createReportsAnalyticsPage();

    if(title=="Admin Panel")
        return createAdminPanelPage();

    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *layout=new QVBoxLayout(page);
    layout->setContentsMargins(20,15,20,15);
    layout->setSpacing(13);

    if(title=="Blood Inventory")
    {
        // HEADER
        QFrame *header=new QFrame;
        header->setStyleSheet(
            "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
            "stop:0 #172554,stop:1 #2563EB);border-radius:13px;"
        );

        QHBoxLayout *hl=new QHBoxLayout(header);
        QVBoxLayout *ht=new QVBoxLayout;

        QLabel *heading=new QLabel("Blood Inventory");
        QLabel *sub=new QLabel("Blood stock monitoring and management");

        heading->setStyleSheet("color:white;font-size:25px;"
                               "font-weight:bold;border:none;");
        sub->setStyleSheet("color:#CBD5E1;font-size:12px;border:none;");

        QLabel *tag=new QLabel("STOCK CONTROL");
        tag->setStyleSheet("background:#DCFCE7;color:#15803D;"
                           "padding:9px;border-radius:7px;font-weight:bold;");

        ht->addWidget(heading);
        ht->addWidget(sub);
        hl->addLayout(ht);
        hl->addStretch();
        hl->addWidget(tag);

        // STATISTICS
        QLabel *total,*active,*low;
        QHBoxLayout *stats=new QHBoxLayout;
        stats->setSpacing(12);

        stats->addWidget(inventoryStat("TOTAL BLOOD UNITS",total,"#2563EB"));
        stats->addWidget(inventoryStat("AVAILABLE GROUPS",active,"#059669"));
        stats->addWidget(inventoryStat("LOW STOCK GROUPS",low,"#D97706"));

        // ADD STOCK FORM
        QFrame *form=new QFrame;
        form->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                            "border-radius:11px;");

        QVBoxLayout *fl=new QVBoxLayout(form);
        fl->setContentsMargins(16,13,16,13);

        QLabel *formTitle=new QLabel("Add Blood Stock");
        formTitle->setStyleSheet("font-size:17px;font-weight:bold;");

        QComboBox *group=new QComboBox;
        group->addItem("Select Blood Group");
        group->addItems({"A+","A-","B+","B-","AB+","AB-","O+","O-"});

        QSpinBox *units=new QSpinBox;
        units->setRange(0,1000);
        units->setSpecialValueText("Enter Units");

        QString inputStyle=
            "QComboBox,QSpinBox{background:#F8FAFC;color:#172554;"
            "padding:10px;border:1px solid #CBD5E1;border-radius:7px;}";

        group->setStyleSheet(inputStyle);
        units->setStyleSheet(inputStyle);

        QPushButton *add=new QPushButton("+  Add Stock");
        add->setMinimumHeight(40);
        add->setCursor(Qt::PointingHandCursor);
        add->setStyleSheet(
            "QPushButton{background:#2563EB;color:white;padding:10px;"
            "border-radius:7px;font-weight:bold;}"
            "QPushButton:hover{background:#1D4ED8;}"
        );

        QHBoxLayout *inputs=new QHBoxLayout;
        inputs->addWidget(group,2);
        inputs->addWidget(units,1);
        inputs->addWidget(add,1);

        fl->addWidget(formTitle);
        fl->addLayout(inputs);

        // STOCK OVERVIEW
        QHBoxLayout *section=new QHBoxLayout;
        QLabel *stockTitle=new QLabel("Blood Group Overview");
        stockTitle->setStyleSheet("font-size:19px;font-weight:bold;");

        QPushButton *refreshBtn=new QPushButton("↻ Refresh");
        refreshBtn->setCursor(Qt::PointingHandCursor);
        refreshBtn->setStyleSheet(
            "background:#E2E8F0;color:#172554;padding:9px 14px;"
            "border-radius:7px;font-weight:bold;"
        );

        section->addWidget(stockTitle);
        section->addStretch();
        section->addWidget(refreshBtn);

        QStringList groups={"A+","A-","B+","B-","AB+","AB-","O+","O-"};
        QStringList colors={"#2563EB","#7C3AED","#059669","#D97706",
                            "#DB2777","#0891B2","#EA580C","#DC2626"};

        QGridLayout *grid=new QGridLayout;
        grid->setSpacing(12);

        // BOTTOM INSIGHTS
        QHBoxLayout *insights=new QHBoxLayout;

        QLabel *alert=new QLabel;
        QLabel *best=new QLabel;

        alert->setWordWrap(true);
        best->setWordWrap(true);
        alert->setMinimumHeight(55);
        best->setMinimumHeight(55);

        insights->addWidget(alert,1);
        insights->addWidget(best,1);

        // REFRESH INVENTORY
        auto refresh=[=]()
        {
            while(QLayoutItem *item=grid->takeAt(0))
            {
                if(item->widget()) delete item->widget();
                delete item;
            }

            InventoryManager inventory;
            std::vector<BloodStock> stocks=inventory.getAllStock();

            int counts[8]={};
            int sum=0,activeGroups=0,lowGroups=0,bestIndex=-1;
            QString lowNames;

            for(const auto &s:stocks)
            {
                QString blood=QString::fromLatin1(s.bloodGroup).trimmed();

                for(int i=0;i<8;i++)
                    if(groups[i]==blood) counts[i]=s.units;
            }

            for(int i=0;i<8;i++)
            {
                int n=counts[i];
                sum+=n;

                if(n>0) activeGroups++;

                if(n<10)
                {
                    lowGroups++;
                    if(!lowNames.isEmpty()) lowNames+=", ";
                    lowNames+=groups[i];
                }

                if(bestIndex==-1 || n>counts[bestIndex])
                    bestIndex=i;

                grid->addWidget(inventoryCard(groups[i],n,colors[i]),
                                i/4,i%4);
            }

            total->setText(QString::number(sum));
            active->setText(QString::number(activeGroups));
            low->setText(QString::number(lowGroups));

            alert->setText(lowGroups?
                "⚠  LOW STOCK: "+lowNames:
                "✓  All groups have sufficient stock.");

            alert->setStyleSheet(
                QString("background:%1;color:%2;padding:12px;"
                        "border-radius:8px;font-weight:bold;")
                .arg(lowGroups?"#FEF3C7":"#DCFCE7")
                .arg(lowGroups?"#92400E":"#15803D")
            );

            best->setText(bestIndex>=0 && sum>0?
                "HIGHEST STOCK: "+groups[bestIndex]+"  |  "+
                QString::number(counts[bestIndex])+" units":
                "NO STOCK DATA AVAILABLE");

            best->setStyleSheet(
                "background:white;color:#1E40AF;padding:12px;"
                "border:1px solid #E2E8F0;border-radius:8px;font-weight:bold;"
            );
        };

        refresh();

        // ADD STOCK
        QObject::connect(add,&QPushButton::clicked,page,[=]()
        {
            if(group->currentIndex()==0 || units->value()<=0)
            {
                QMessageBox::warning(page,"Missing Details",
                                     "Select blood group and units!");
                return;
            }

            InventoryManager inventory;
            QByteArray blood=group->currentText().toLatin1();

            if(!inventory.addStockRecord(blood.constData(),units->value()))
            {
                QMessageBox::warning(page,"Error",
                                     "Unable to update blood stock!");
                return;
            }

            refresh();
            group->setCurrentIndex(0);
            units->setValue(0);

            QMessageBox::information(page,"Success",
                                     "Blood stock updated successfully!");
        });

        QObject::connect(refreshBtn,&QPushButton::clicked,page,refresh);

        layout->addWidget(header);
        layout->addLayout(stats);
        layout->addWidget(form);
        layout->addLayout(section);
        layout->addLayout(grid);
        layout->addLayout(insights);
        layout->addStretch();
    }
    else
    {
        QLabel *heading=new QLabel(title);
        QLabel *info=new QLabel(description);

        heading->setStyleSheet(
            "font-size:30px;font-weight:bold;color:#172554;"
        );

        info->setStyleSheet("font-size:14px;color:#64748B;");

        layout->addWidget(heading);
        layout->addWidget(info);
        layout->addStretch();
    }

    return page;
}