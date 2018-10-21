#pragma once

#include "ToolModel.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

private:
    Ui::MainWindow* ui;

    ToolModel model_;
};
