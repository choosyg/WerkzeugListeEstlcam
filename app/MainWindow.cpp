#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QSortFilterProxyModel>

static const QString LAST_OPEN_DIR_KEY = "LastOpenDir";

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    auto* sort = new QSortFilterProxyModel( this );
    sort->setSourceModel( &model_ );
    ui->tableView->setModel( sort );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_toolButton_clicked() {
    QSettings settings( "config.ini", QSettings::IniFormat );
    QString initDir = settings.value( LAST_OPEN_DIR_KEY ).toString();
    auto file = QFileDialog::getOpenFileName( this, "Öffnen", initDir, "*.tl" );
    settings.setValue( LAST_OPEN_DIR_KEY, QFileInfo( file ).absolutePath() );

    model_.load( file );
    ui->tableView->resizeColumnsToContents();
}
