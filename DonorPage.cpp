#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QByteArray>
#include <QIntValidator>
#include <QStringList>
#include <cstring>
#include <vector>

// FIELD LABEL
static QLabel* donorLabel(QString text)
{
    QLabel *l=new QLabel(text);
    l->setStyleSheet("color:#334155;font-size:12px;font-weight:600;");
    return l;
}

// MAIN DONOR PAGE
QWidget* MainWindow::createDonorPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *main=new QVBoxLayout(page);
    main->setContentsMargins(20,15,20,15);
    main->setSpacing(12);

    // PAGE HEADER
    QHBoxLayout *header=new QHBoxLayout;
    QVBoxLayout *headingBox=new QVBoxLayout;

    QLabel *title=new QLabel("Donor Management");
    QLabel *sub=new QLabel("Register and manage blood donor information.");

    title->setStyleSheet("font-size:27px;font-weight:bold;color:#172554;");
    sub->setStyleSheet("font-size:13px;color:#64748B;");

    QLabel *status=new QLabel("● DONOR MANAGEMENT");
    status->setStyleSheet("background:#DCFCE7;color:#15803D;"
                          "padding:9px;border-radius:7px;font-weight:bold;");

    headingBox->addWidget(title);
    headingBox->addWidget(sub);
    header->addLayout(headingBox);
    header->addStretch();
    header->addWidget(status);

    // DONOR REGISTRATION FORM
    QFrame *form=new QFrame;
    form->setObjectName("donorForm");
    form->setStyleSheet(
        "QFrame#donorForm{background:white;border:1px solid #E2E8F0;"
        "border-radius:13px;}"
    );

    QVBoxLayout *formLayout=new QVBoxLayout(form);
    formLayout->setContentsMargins(18,14,18,14);
    formLayout->setSpacing(12);

    // FORM HEADING
    QHBoxLayout *formHeader=new QHBoxLayout;

    QLabel *formTitle=new QLabel("Register New Donor");
    QLabel *badge=new QLabel("NEW REGISTRATION");

    formTitle->setStyleSheet("font-size:19px;font-weight:bold;border:none;");
    badge->setStyleSheet("background:#DBEAFE;color:#1D4ED8;"
                         "padding:7px 10px;border-radius:6px;"
                         "font-size:10px;font-weight:bold;");

    formHeader->addWidget(formTitle);
    formHeader->addStretch();
    formHeader->addWidget(badge);

    QLabel *formSub=new QLabel(
        "Enter accurate donor details to register a new donor."
    );
    formSub->setStyleSheet("color:#64748B;font-size:12px;border:none;");

    // INPUT FIELDS
    QLineEdit *id=new QLineEdit;
    QLineEdit *name=new QLineEdit;
    QLineEdit *age=new QLineEdit;
    QComboBox *blood=new QComboBox;
    QLineEdit *phone=new QLineEdit;
    QLineEdit *city=new QLineEdit;
    QLineEdit *date=new QLineEdit;
    QComboBox *available=new QComboBox;

    id->setPlaceholderText("Enter Donor ID");
    name->setPlaceholderText("Full Name");
    age->setPlaceholderText("Age (18-100)");
    phone->setPlaceholderText("Phone Number");
    city->setPlaceholderText("Enter City");
    date->setPlaceholderText("DD-MM-YYYY");

    age->setValidator(new QIntValidator(18,100,age));

    blood->addItems({"Select Group","A+","A-","B+","B-",
                     "AB+","AB-","O+","O-"});

    available->addItems({"Select Availability","Yes","No"});

    QString inputStyle=
        "QLineEdit,QComboBox{background:#F8FAFC;color:#172554;"
        "padding:10px;border:1px solid #CBD5E1;border-radius:7px;"
        "font-size:12px;min-height:19px;}"
        "QLineEdit:focus,QComboBox:focus{border:1px solid #2563EB;"
        "background:white;}"
        "QComboBox QAbstractItemView{background:white;color:#172554;"
        "selection-background-color:#2563EB;selection-color:white;}";

    QWidget *inputs[]={id,name,age,blood,phone,city,date,available};

    for(QWidget *w:inputs)
        w->setStyleSheet(inputStyle);

    // FOUR COLUMN FORM
    QStringList labels={
        "Donor ID","Full Name","Age","Blood Group",
        "Phone Number","City","Last Donation Date","Availability"
    };

    QGridLayout *grid=new QGridLayout;
    grid->setHorizontalSpacing(13);
    grid->setVerticalSpacing(9);

    for(int i=0;i<8;i++)
    {
        int row=i/4;
        int col=i%4;

        grid->addWidget(donorLabel(labels[i]),row*2,col);
        grid->addWidget(inputs[i],row*2+1,col);
    }

    for(int i=0;i<4;i++)
        grid->setColumnStretch(i,1);

    // ADD BUTTON
    QPushButton *add=new QPushButton("+  Register Donor");
    add->setMinimumSize(190,42);
    add->setCursor(Qt::PointingHandCursor);

    add->setStyleSheet(
        "QPushButton{background:#2563EB;color:white;padding:10px;"
        "border:none;border-radius:7px;font-size:13px;font-weight:bold;}"
        "QPushButton:hover{background:#1D4ED8;}"
        "QPushButton:pressed{background:#1E40AF;}"
    );

    QHBoxLayout *actions=new QHBoxLayout;
    QLabel *hint=new QLabel("All fields are required.");
    hint->setStyleSheet("color:#94A3B8;font-size:11px;border:none;");

    actions->addWidget(hint);
    actions->addStretch();
    actions->addWidget(add);

    formLayout->addLayout(formHeader);
    formLayout->addWidget(formSub);
    formLayout->addLayout(grid);
    formLayout->addLayout(actions);

    // SEARCH SECTION
    QHBoxLayout *search=new QHBoxLayout;

    QLabel *tableTitle=new QLabel("Registered Donors");
    tableTitle->setStyleSheet("font-size:19px;font-weight:bold;color:#172554;");

    QLineEdit *searchID=new QLineEdit;
    searchID->setPlaceholderText("Search Donor ID");
    searchID->setMaximumWidth(200);
    searchID->setStyleSheet(inputStyle);

    QPushButton *find=new QPushButton("Search");
    QPushButton *show=new QPushButton("Show All");

    QString btnStyle=
        "QPushButton{background:#2563EB;color:white;padding:9px 15px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#1D4ED8;}";

    find->setStyleSheet(btnStyle);
    show->setStyleSheet(
        "QPushButton{background:#E2E8F0;color:#172554;padding:9px 15px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#CBD5E1;}"
    );

    search->addWidget(tableTitle);
    search->addStretch();
    search->addWidget(searchID);
    search->addWidget(find);
    search->addWidget(show);

    // DONOR TABLE
    donorTable=new QTableWidget(0,8);

    donorTable->setHorizontalHeaderLabels({
        "Donor ID","Name","Age","Blood Group",
        "Phone","City","Last Donation","Available"
    });

    donorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    donorTable->verticalHeader()->hide();
    donorTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    donorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    donorTable->setAlternatingRowColors(true);
    donorTable->setShowGrid(false);

    donorTable->setStyleSheet(
        "QTableWidget{background:white;border:1px solid #E2E8F0;"
        "border-radius:9px;color:#172554;font-size:12px;}"
        "QHeaderView::section{background:#172554;color:white;"
        "padding:11px;border:none;font-weight:bold;}"
        "QTableWidget::item{padding:7px;}"
        "QTableWidget::item:selected{background:#DBEAFE;color:#172554;}"
    );

    // REGISTER DONOR
    connect(add,&QPushButton::clicked,this,[=]()
    {
        if(id->text().trimmed().isEmpty() ||
           name->text().trimmed().isEmpty() ||
           age->text().isEmpty() || age->text().toInt()<18 ||
           phone->text().trimmed().isEmpty() ||
           city->text().trimmed().isEmpty() ||
           date->text().trimmed().isEmpty() ||
           blood->currentIndex()==0 || available->currentIndex()==0)
        {
            QMessageBox::warning(this,"Missing Information",
                                 "Please fill all donor details correctly.");
            return;
        }

        Donor d{};

        auto copy=[](char *dest,QString value,int size)
        {
            QByteArray data=value.trimmed().toUtf8();
            std::strncpy(dest,data.constData(),size-1);
            dest[size-1]='\0';
        };

        copy(d.donorID,id->text(),sizeof(d.donorID));
        copy(d.name,name->text(),sizeof(d.name));
        copy(d.bloodGroup,blood->currentText(),sizeof(d.bloodGroup));
        copy(d.phone,phone->text(),sizeof(d.phone));
        copy(d.city,city->text(),sizeof(d.city));
        copy(d.lastDonation,date->text(),sizeof(d.lastDonation));
        copy(d.availability,available->currentText(),sizeof(d.availability));

        d.age=age->text().toInt();

        if(donorManager.addDonorRecord(d))
        {
            QMessageBox::information(this,"Success",
                                     "Donor registered successfully!");

            id->clear();
            name->clear();
            age->clear();
            blood->setCurrentIndex(0);
            available->setCurrentIndex(0);
            phone->clear();
            city->clear();
            date->clear();

            refreshDonors();
        }
        else
            QMessageBox::warning(this,"Failed",
                                 "Donor ID already exists or file error.");
    });

    // SEARCH DONOR
    connect(find,&QPushButton::clicked,this,[=]()
    {
        QString value=searchID->text().trimmed();

        if(value.isEmpty())
        {
            QMessageBox::warning(this,"Search","Enter Donor ID.");
            return;
        }

        Donor result{};
        QByteArray key=value.toUtf8();

        if(donorManager.findDonorByID(key.constData(),result))
        {
            refreshDonors();

            for(int i=0;i<donorTable->rowCount();i++)
                donorTable->setRowHidden(
                    i,donorTable->item(i,0)->text()!=value);
        }
        else
        {
            QMessageBox::information(this,"Result","Donor not found.");
            refreshDonors();
        }
    });

    // SHOW ALL
    connect(show,&QPushButton::clicked,this,[=]()
    {
        searchID->clear();
        refreshDonors();
    });

    main->addLayout(header);
    main->addWidget(form);
    main->addLayout(search);
    main->addWidget(donorTable,1);

    refreshDonors();
    return page;
}

// REFRESH DONOR TABLE
void MainWindow::refreshDonors()
{
    std::vector<Donor> donors=donorManager.getAllDonors();
    donorTable->setRowCount(0);

    for(const Donor &d:donors)
    {
        int row=donorTable->rowCount();
        donorTable->insertRow(row);

        QStringList values={
            d.donorID,d.name,QString::number(d.age),d.bloodGroup,
            d.phone,d.city,d.lastDonation,d.availability
        };

        for(int col=0;col<8;col++)
        {
            QTableWidgetItem *item=new QTableWidgetItem(values[col]);
            item->setTextAlignment(Qt::AlignCenter);
            donorTable->setItem(row,col,item);
        }

        donorTable->setRowHeight(row,40);
    }
}