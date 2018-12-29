#pragma once

#include "ToolModel.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

private slots:
    void on_openButton_clicked();
    void on_deleteButton_clicked();

    void onCurrentChanged( const QModelIndex& current, const QModelIndex& prev );


    void on_importButton_clicked();

    void on_saveButton_clicked();

    void on_exportButton_clicked();

    void on_addButton_clicked();

    void on_reindexButton_clicked();

private:
    Ui::MainWindow* ui;

    ToolModel model_;
    QSortFilterProxyModel* sort_;
};
