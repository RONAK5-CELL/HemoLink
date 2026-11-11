#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QDate>
#include <QSpinBox>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSet>
#include <QStringList>

// FIND DONOR
static bool findDonation(QString id,QString &name,QString &blood)
{
    QFile file(QCoreApplication::applicationDirPath()+"/donors.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QStringList d=in.readLine().split('|');
        if(d.size()==8 && d[0].trimmed()==id)
        {
            name=d[1].trimmed();
            blood=d[3].trimmed();
            return true;
        }
    }
    return false;
}

// STATISTICS CARD
static QFrame* donationCard(QString title,QString icon,
                            QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setObjectName("donationCard");
    f->setMinimumHeight(110);
    f->setStyleSheet(
        "QFrame#donationCard{background:white;"
        "border:1px solid #E2E8F0;border-radius:12px;}"
        "QFrame#donationCard:hover{border:1px solid #94A3B8;}"
    );

    QVBoxLayout *v=new QVBoxLayout(f);
    QLabel *t=new QLabel(icon+"  "+title);
    value=new QLabel("0");

    t->setStyleSheet("color:#64748B;font-size:12px;"
                     "font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+
                         ";font-size:28px;font-weight:bold;border:none;");

    v->addWidget(t);
    v->addWidget(value);
    return f;
}

// FIELD LABEL
static QLabel* fieldLabel(QString text)
{
    QLabel *l=new QLabel(text);
    l->setStyleSheet("color:#334155;font-size:12px;font-weight:600;");
    return l;
}

// MAIN PAGE
QWidget* createDonationHistoryPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *layout=new QVBoxLayout(page);
    layout->setContentsMargins(20,15,20,15);
    layout->setSpacing(13);

    QString path=QCoreApplication::applicationDirPath()+"/";

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:13px;"
    );

    QHBoxLayout *hl=new QHBoxLayout(header);
    QVBoxLayout *ht=new QVBoxLayout;

    QLabel *title=new QLabel("Donation History");
    QLabel *sub=new QLabel("Track donor contributions and manage donation records.");

    title->setStyleSheet("color:white;font-size:25px;font-weight:bold;");
    sub->setStyleSheet("color:#CBD5E1;font-size:13px;");

    QLabel *badge=new QLabel("DONATION MANAGEMENT");
    badge->setStyleSheet("background:#DCFCE7;color:#15803D;"
                         "padding:9px;border-radius:7px;font-weight:bold;");

    ht->addWidget(title);
    ht->addWidget(sub);
    hl->addLayout(ht);
    hl->addStretch();
    hl->addWidget(badge);

    // STATISTICS
    QLabel *total,*totalUnits,*unique;

    QHBoxLayout *stats=new QHBoxLayout;
    stats->setSpacing(12);

    stats->addWidget(donationCard("TOTAL DONATIONS","●",total,"#2563EB"));
    stats->addWidget(donationCard("TOTAL BLOOD UNITS","✚",totalUnits,"#059669"));
    stats->addWidget(donationCard("UNIQUE DONORS","♟",unique,"#7C3AED"));

    // DONATION FORM
    QFrame *form=new QFrame;
    form->setObjectName("donationForm");
    form->setStyleSheet(
        "QFrame#donationForm{background:white;"
        "border:1px solid #E2E8F0;border-radius:12px;}"
    );

    QVBoxLayout *fl=new QVBoxLayout(form);
    fl->setContentsMargins(18,15,18,15);
    fl->setSpacing(12);

    QHBoxLayout *formHeading=new QHBoxLayout;
    QLabel *formTitle=new QLabel("Record New Donation");
    QLabel *newEntry=new QLabel("NEW ENTRY");

    formTitle->setStyleSheet("font-size:19px;font-weight:bold;border:none;");
    newEntry->setStyleSheet("background:#DBEAFE;color:#1D4ED8;"
                            "padding:7px 10px;border-radius:6px;"
                            "font-size:10px;font-weight:bold;");

    formHeading->addWidget(formTitle);
    formHeading->addStretch();
    formHeading->addWidget(newEntry);

    // INPUT FIELDS
    QLineEdit *id=new QLineEdit;
    id->setPlaceholderText("Enter Donor ID");

    QLineEdit *name=new QLineEdit;
    name->setPlaceholderText("Donor Name");
    name->setReadOnly(true);

    QLineEdit *blood=new QLineEdit;
    blood->setPlaceholderText("Blood Group");
    blood->setReadOnly(true);

    QDateEdit *date=new QDateEdit(QDate::currentDate());
    date->setCalendarPopup(true);
    date->setMaximumDate(QDate::currentDate());

    QSpinBox *units=new QSpinBox;
    units->setRange(1,5);
    units->setSuffix(" Units");

    QLineEdit *hospital=new QLineEdit;
    hospital->setPlaceholderText("Enter Hospital Name");

    QString inputStyle=
        "QLineEdit,QDateEdit,QSpinBox{"
        "background:#F8FAFC;color:#172554;padding:9px;"
        "border:1px solid #CBD5E1;border-radius:7px;}"
        "QLineEdit:focus,QDateEdit:focus,QSpinBox:focus{"
        "border:1px solid #2563EB;background:white;}";

    id->setStyleSheet(inputStyle);
    name->setStyleSheet(inputStyle);
    blood->setStyleSheet(inputStyle);
    date->setStyleSheet(inputStyle);
    units->setStyleSheet(inputStyle);
    hospital->setStyleSheet(inputStyle);

    // FETCH BUTTON
    QPushButton *fetch=new QPushButton("Fetch Donor");
    fetch->setCursor(Qt::PointingHandCursor);
    fetch->setStyleSheet(
        "QPushButton{background:#0F766E;color:white;padding:9px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#115E59;}"
    );

    QHBoxLayout *idRow=new QHBoxLayout;
    idRow->addWidget(id,1);
    idRow->addWidget(fetch);

    QWidget *idWidget=new QWidget;
    idWidget->setLayout(idRow);

    // FORM GRID
    QGridLayout *grid=new QGridLayout;
    grid->setHorizontalSpacing(13);
    grid->setVerticalSpacing(9);

    grid->addWidget(fieldLabel("Donor ID"),0,0);
    grid->addWidget(fieldLabel("Donor Name"),0,1);
    grid->addWidget(fieldLabel("Blood Group"),0,2);

    grid->addWidget(idWidget,1,0);
    grid->addWidget(name,1,1);
    grid->addWidget(blood,1,2);

    grid->addWidget(fieldLabel("Donation Date"),2,0);
    grid->addWidget(fieldLabel("Units Donated"),2,1);
    grid->addWidget(fieldLabel("Hospital Name"),2,2);

    grid->addWidget(date,3,0);
    grid->addWidget(units,3,1);
    grid->addWidget(hospital,3,2);

    // SAVE BUTTON
    QPushButton *save=new QPushButton("Save Donation Record");
    save->setMinimumHeight(41);
    save->setCursor(Qt::PointingHandCursor);
    save->setStyleSheet(
        "QPushButton{background:#2563EB;color:white;padding:10px;"
        "border-radius:7px;font-size:13px;font-weight:bold;}"
        "QPushButton:hover{background:#1D4ED8;}"
        "QPushButton:pressed{background:#1E40AF;}"
    );

    fl->addLayout(formHeading);
    fl->addLayout(grid);
    fl->addWidget(save);

    // DONATION RECORDS TABLE
    QFrame *tableBox=new QFrame;
    tableBox->setStyleSheet("background:white;border-radius:12px;"
                            "border:1px solid #E2E8F0;");

    QVBoxLayout *tl=new QVBoxLayout(tableBox);

    QHBoxLayout *tableHeading=new QHBoxLayout;
    QLabel *tableTitle=new QLabel("Donation Records");
    QLabel *recordInfo=new QLabel("All recorded donations");

    tableTitle->setStyleSheet("font-size:18px;font-weight:bold;border:none;");
    recordInfo->setStyleSheet("color:#64748B;font-size:11px;border:none;");

    tableHeading->addWidget(tableTitle);
    tableHeading->addStretch();
    tableHeading->addWidget(recordInfo);

    QTableWidget *table=new QTableWidget(0,6);
    table->setHorizontalHeaderLabels({
        "Donor ID","Donor Name","Blood Group",
        "Donation Date","Units","Hospital"
    });

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);
    table->setAlternatingRowColors(true);
    table->setMinimumHeight(220);

    table->setStyleSheet(
        "QTableWidget{background:white;border:none;"
        "color:#172554;font-size:12px;}"
        "QHeaderView::section{background:#172554;color:white;"
        "padding:11px;border:none;font-weight:bold;}"
        "QTableWidget::item{padding:8px;}"
        "QTableWidget::item:selected{background:#DBEAFE;color:#172554;}"
    );

    // REFRESH DATA
    auto refresh=[=]()
    {
        table->setRowCount(0);

        QFile file(path+"donation_history.txt");
        int count=0,amount=0;
        QSet<QString> donors;

        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream in(&file);

            while(!in.atEnd())
            {
                QStringList d=in.readLine().split('|');
                if(d.size()!=6) continue;

                int r=table->rowCount();
                table->insertRow(r);

                for(int i=0;i<6;i++)
                {
                    QTableWidgetItem *item=new QTableWidgetItem(d[i]);
                    item->setTextAlignment(Qt::AlignCenter);
                    table->setItem(r,i,item);
                }

                table->setRowHeight(r,42);
                count++;
                amount+=d[4].toInt();
                donors.insert(d[0]);
            }
            file.close();
        }

        total->setText(QString::number(count));
        totalUnits->setText(QString::number(amount));
        unique->setText(QString::number(donors.size()));
        recordInfo->setText(QString::number(count)+" records found");
    };

    refresh();

    // FETCH DONOR
    QObject::connect(fetch,&QPushButton::clicked,page,[=]()
    {
        QString n,b;

        if(findDonation(id->text().trimmed(),n,b))
        {
            name->setText(n);
            blood->setText(b);
        }
        else
            QMessageBox::warning(page,"Error","Donor not found!");
    });

    // CLEAR DETAILS WHEN ID CHANGES
    QObject::connect(id,&QLineEdit::textChanged,page,[=]()
    {
        name->clear();
        blood->clear();
    });

    // SAVE DONATION
    QObject::connect(save,&QPushButton::clicked,page,[=]()
    {
        QString donorId=id->text().trimmed();
        QString place=hospital->text().trimmed();
        QString n,b;

        if(donorId.isEmpty() || place.isEmpty() || name->text().isEmpty())
        {
            QMessageBox::warning(page,"Missing Details",
                                 "Please fill all details and fetch donor!");
            return;
        }

        if(!findDonation(donorId,n,b))
        {
            QMessageBox::warning(page,"Error","Donor not found!");
            return;
        }

        place.replace("|"," ");

        QFile file(path+"donation_history.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text))
        {
            QMessageBox::warning(page,"Error","Cannot save record!");
            return;
        }

        QTextStream out(&file);
        out<<donorId<<"|"<<n<<"|"<<b<<"|"
           <<date->date().toString("dd-MM-yyyy")<<"|"
           <<units->value()<<"|"<<place<<"\n";

        file.close();
        refresh();

        id->clear();
        name->clear();
        blood->clear();
        hospital->clear();
        units->setValue(1);
        date->setDate(QDate::currentDate());

        QMessageBox::information(page,"Success",
                                 "Donation record saved successfully!");
    });

    // FOOTER
    QLabel *footer=new QLabel(
        "HEMOLINK  |  Every donation creates an opportunity to save a life."
    );
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color:#64748B;font-size:11px;padding:5px;");

    tl->addLayout(tableHeading);
    tl->addWidget(table);

    layout->addWidget(header);
    layout->addLayout(stats);
    layout->addWidget(form);
    layout->addWidget(tableBox,1);
    layout->addWidget(footer);

    return page;
}