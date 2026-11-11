#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QStringList>

// PAGE FUNCTIONS
QWidget* createDonationHistoryPage();
QWidget* createNotificationCenterPage();
QWidget* createLiveEmergencyRadarPage();

MainWindow::MainWindow()
{
    setWindowTitle("HemoLink - Blood Bank Management System");
    resize(1200,750);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // SIDEBAR
    QWidget *sidebar = new QWidget;
    sidebar->setFixedWidth(230);
    sidebar->setStyleSheet("background-color:#172554;");

    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(15,25,15,20);

    // LOGO
    QLabel *logo = new QLabel("🩸 HEMOLINK:");

    logo->setStyleSheet(
        "color:white;font-size:23px;font-weight:bold;"
        "font-family:'Segoe UI Emoji','Segoe UI';"
    );

    QLabel *subtitle = new QLabel(
        "INTELLIGENT BLOOD BANK\n"
        "AND EMERGENCY MATCHING SYSTEM"
    );

    subtitle->setWordWrap(true);
    subtitle->setStyleSheet(
        "color:#bfdbfe;font-size:10px;font-weight:bold;"
    );

    sideLayout->addWidget(logo);
    sideLayout->addWidget(subtitle);
    sideLayout->addSpacing(30);

    // MENU NAMES
    QStringList menuItems = {
        "Dashboard",
        "Donor Management",
        "Blood Inventory",
        "Emergency Requests",
        "Donor Matching",
        "Reports",
        "Donation History",
        "Notification Center",
        "Live Emergency Radar",
        "Admin Panel"
    };

    // MENU SYMBOLS
    QStringList menuIcons = {
        "⌂","♟","✚","⚠","⌕",
        "▥","◷","🔔","◉","⚙"
    };

    // CREATE SIDEBAR BUTTONS
    for(int i=0;i<menuItems.size();i++)
    {
        QPushButton *btn = new QPushButton(
            menuIcons[i]+"    "+menuItems[i]
        );

        btn->setMinimumHeight(45);
        btn->setCheckable(true);

        btn->setStyleSheet(
            "QPushButton{color:white;background:transparent;"
            "text-align:left;padding-left:15px;border:none;"
            "border-radius:6px;font-size:14px;"
            "font-family:'Segoe UI Symbol','Segoe UI';}"
            "QPushButton:hover{background-color:#1e40af;}"
            "QPushButton:checked{background-color:#2563eb;"
            "font-weight:bold;}"
        );

        sideLayout->addWidget(btn);
        navButtons.append(btn);

        connect(btn,&QPushButton::clicked,this,[this,i]()
        {
            changePage(i);
        });
    }

    sideLayout->addStretch();

    QLabel *footer = new QLabel("HemoLink v1.0");
    footer->setStyleSheet("color:#93c5fd;");
    sideLayout->addWidget(footer);

    // PAGES
    pages = new QStackedWidget;

    createDashboard();  // 0

    pages->addWidget(createDonorPage()); // 1

    pages->addWidget(createInfoPage(
        "Blood Inventory",
        "Check available blood groups and units."
    )); // 2

    pages->addWidget(createInfoPage(
        "Emergency Requests",
        "Manage emergency blood requests from hospitals."
    )); // 3

    pages->addWidget(createInfoPage(
        "Donor Matching",
        "Find suitable donors for emergency requirements."
    )); // 4

    pages->addWidget(createInfoPage(
        "Reports",
        "View blood bank and donor activity reports."
    )); // 5

    pages->addWidget(createDonationHistoryPage()); // 6

    pages->addWidget(createNotificationCenterPage()); // 7

    pages->addWidget(createLiveEmergencyRadarPage()); // 8

    pages->addWidget(createInfoPage(
        "Admin Panel",
        "Manage HemoLink system and patient requests."
    )); // 9

    // MAIN LAYOUT
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(pages);

    navButtons[0]->setChecked(true);
}

// CHANGE PAGE
void MainWindow::changePage(int index)
{
    pages->setCurrentIndex(index);

    for(int i=0;i<navButtons.size();i++)
        navButtons[i]->setChecked(i==index);
}