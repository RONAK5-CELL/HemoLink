#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QList>
#include <QTableWidget>
#include "Inventory.h"
#include "Donormanager.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    InventoryManager inventoryManager;

    QStackedWidget *pages;
    QList<QPushButton*> navButtons;

    DonorManager donorManager;
    QTableWidget *donorTable;

    QWidget* createInventoryPage();
    QWidget* createDonorPage();

    QWidget* createInfoPage(
        QString title,
        QString description
    );

    void createDashboard();
    void refreshDonors();
    void changePage(int index);
};

#endif