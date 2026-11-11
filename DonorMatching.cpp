#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QAbstractItemView>
#include <QFrame>
#include <QColor>

// SUMMARY CARD
static QFrame* matchCard(QString title,QLabel *&value,QString color)
{
    QFrame *f=new QFrame;
    f->setStyleSheet("QFrame{background:white;border:1px solid #E2E8F0;"
                     "border-radius:12px;}"
                     "QFrame:hover{border:1px solid #94A3B8;}");
    QVBoxLayout *v=new QVBoxLayout(f);
    QLabel *t=new QLabel(title);
    value=new QLabel("0");

    t->setStyleSheet("color:#64748B;font-size:11px;font-weight:bold;border:none;");
    value->setStyleSheet("color:"+color+";font-size:25px;font-weight:bold;border:none;");

    v->addWidget(t);
    v->addWidget(value);
    return f;
}

// MAIN PAGE
QWidget* createDonorMatchingPage()
{
    QWidget *page=new QWidget;
    page->setStyleSheet("background:#F1F5F9;color:#172554;");

    QVBoxLayout *layout=new QVBoxLayout(page);
    layout->setContentsMargins(20,15,20,15);
    layout->setSpacing(12);

    // HEADER
    QFrame *header=new QFrame;
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #172554,stop:1 #2563EB);border-radius:13px;");

    QVBoxLayout *hl=new QVBoxLayout(header);
    QLabel *title=new QLabel("Smart Donor Matching");
    QLabel *sub=new QLabel("Find available donors by blood group and location");

    title->setStyleSheet("color:white;font-size:25px;font-weight:bold;");
    sub->setStyleSheet("color:#CBD5E1;font-size:13px;");
    hl->addWidget(title);
    hl->addWidget(sub);

    // SUMMARY CARDS
    QLabel *matches,*selectedGroup,*selectedLocation;
    QHBoxLayout *stats=new QHBoxLayout;
    stats->setSpacing(12);

    stats->addWidget(matchCard("MATCHING DONORS",matches,"#2563EB"));
    stats->addWidget(matchCard("BLOOD GROUP",selectedGroup,"#DC2626"));
    stats->addWidget(matchCard("SEARCH LOCATION",selectedLocation,"#059669"));

    // SEARCH PANEL
    QFrame *form=new QFrame;
    form->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                        "border-radius:12px;");
    QVBoxLayout *fl=new QVBoxLayout(form);

    QLabel *formTitle=new QLabel("Find Available Donors");
    formTitle->setStyleSheet("font-size:18px;font-weight:bold;");

    QComboBox *group=new QComboBox;
    group->addItems({"Select Blood Group","A+","A-","B+","B-",
                     "AB+","AB-","O+","O-"});

    QLineEdit *location=new QLineEdit;
    location->setPlaceholderText("Enter city or location (optional)");

    QString style="QComboBox,QLineEdit{background:#F8FAFC;"
                  "color:#172554;padding:10px;"
                  "border:1px solid #CBD5E1;border-radius:7px;}"
                  "QComboBox:focus,QLineEdit:focus{border:1px solid #2563EB;}";

    group->setStyleSheet(style);
    location->setStyleSheet(style);

    QPushButton *search=new QPushButton("Search Donors");
    QPushButton *clear=new QPushButton("Clear Filters");

    search->setStyleSheet(
        "QPushButton{background:#2563EB;color:white;padding:11px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#1D4ED8;}");

    clear->setStyleSheet(
        "QPushButton{background:#E2E8F0;color:#172554;padding:11px;"
        "border-radius:7px;font-weight:bold;}"
        "QPushButton:hover{background:#CBD5E1;}");

    QHBoxLayout *filters=new QHBoxLayout;
    filters->addWidget(group,1);
    filters->addWidget(location,2);
    filters->addWidget(search);
    filters->addWidget(clear);

    fl->addWidget(formTitle);
    fl->addLayout(filters);

    // DONOR TABLE
    QFrame *box=new QFrame;
    box->setStyleSheet("background:white;border:1px solid #E2E8F0;"
                       "border-radius:12px;");
    QVBoxLayout *bl=new QVBoxLayout(box);

    QHBoxLayout *top=new QHBoxLayout;
    QLabel *tableTitle=new QLabel("Available Donors");
    QLabel *result=new QLabel("Search to find donors");

    tableTitle->setStyleSheet("font-size:18px;font-weight:bold;");
    result->setStyleSheet("color:#64748B;font-size:12px;");

    top->addWidget(tableTitle);
    top->addStretch();
    top->addWidget(result);

    QTableWidget *table=new QTableWidget(0,4);
    table->setHorizontalHeaderLabels(
        {"Donor Name","Blood Group","Location","Availability"});

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);
    table->setMinimumHeight(240);

    table->setStyleSheet(
        "QTableWidget{background:white;border:none;font-size:13px;}"
        "QHeaderView::section{background:#172554;color:white;"
        "padding:12px;border:none;font-weight:bold;}"
        "QTableWidget::item{padding:8px;color:#172554;}"
        "QTableWidget::item:selected{background:#DBEAFE;}");

    // SEARCH DONORS
    QObject::connect(search,&QPushButton::clicked,page,[=]()
    {
        table->setRowCount(0);

        if(group->currentIndex()==0)
        {
            QMessageBox::warning(page,"Select Group","Please select a blood group!");
            return;
        }

        QFile file(QCoreApplication::applicationDirPath()+"/donors.txt");

        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            file.setFileName("donors.txt");
            if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QMessageBox::warning(page,"Error","Donor file not found!");
                return;
            }
        }

        QString bloodGroup=group->currentText();
        QString city=location->text().trimmed();

        selectedGroup->setText(bloodGroup);
        selectedLocation->setText(city.isEmpty()?"All Cities":city);

        QTextStream in(&file);
        int count=0;

        while(!in.atEnd())
        {
            QStringList d=in.readLine().split('|');
            if(d.size()!=8) continue;

            QString name=d[1].trimmed();
            QString blood=d[3].trimmed();
            QString donorCity=d[5].trimmed();
            QString status=d[7].trimmed();

            if(blood.compare(bloodGroup,Qt::CaseInsensitive)==0 &&
               donorCity.contains(city,Qt::CaseInsensitive) &&
               status.compare("Yes",Qt::CaseInsensitive)==0)
            {
                int r=table->rowCount();
                table->insertRow(r);

                table->setItem(r,0,new QTableWidgetItem(name));
                table->setItem(r,1,new QTableWidgetItem(blood));
                table->setItem(r,2,new QTableWidgetItem(donorCity));

                QLabel *available=new QLabel("● Available");
                available->setAlignment(Qt::AlignCenter);
                available->setStyleSheet(
                    "background:#DCFCE7;color:#15803D;"
                    "padding:7px;border-radius:7px;font-weight:bold;");

                table->setCellWidget(r,3,available);
                table->setRowHeight(r,45);
                count++;
            }
        }

        file.close();
        matches->setText(QString::number(count));
        result->setText(QString::number(count)+" donors found");

        if(count==0)
            QMessageBox::information(page,"Result","No available donor found.");
    });

    // CLEAR FILTERS
    QObject::connect(clear,&QPushButton::clicked,page,[=]()
    {
        group->setCurrentIndex(0);
        location->clear();
        table->setRowCount(0);
        matches->setText("0");
        selectedGroup->setText("—");
        selectedLocation->setText("—");
        result->setText("Search to find donors");
    });

    // MATCHING GUIDE
    QFrame *guide=new QFrame;
    guide->setStyleSheet("background:#E0F2FE;border-radius:10px;");
    QVBoxLayout *gv=new QVBoxLayout(guide);

    QLabel *gt=new QLabel("How Donor Matching Works");
    QLabel *gd=new QLabel(
        "• Select the required blood group.\n"
        "• Filter available donors by city.\n"
        "• View matching donors and their availability."
    );

    gt->setStyleSheet("font-size:15px;font-weight:bold;color:#075985;");
    gd->setStyleSheet("color:#334155;font-size:12px;");
    gv->addWidget(gt);
    gv->addWidget(gd);

    layout->addWidget(header);
    layout->addLayout(stats);
    layout->addWidget(form);
    layout->addWidget(box,1);
    layout->addWidget(guide);

    return page;
}