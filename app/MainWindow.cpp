#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QSortFilterProxyModel>

static const QString LAST_OPEN_DIR_KEY = "LastOpenDir";

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    sort_ = new QSortFilterProxyModel( this );
    sort_->setDynamicSortFilter( false );
    sort_->setSourceModel( &model_ );
    ui->tableView->setModel( sort_ );
    ui->tableView->sortByColumn( 1, Qt::AscendingOrder );

    connect( ui->tableView->selectionModel(),
             &QItemSelectionModel::currentChanged,
             this,
             &MainWindow::onCurrentChanged );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_openButton_clicked() {
    QSettings settings( "config.ini", QSettings::IniFormat );
    QString initDir = settings.value( LAST_OPEN_DIR_KEY ).toString();
    auto file = QFileDialog::getOpenFileName( this, "Öffnen", initDir, "*.tools" );
    settings.setValue( LAST_OPEN_DIR_KEY, QFileInfo( file ).absolutePath() );

    model_.load( file );
    sort_->invalidate();
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_saveButton_clicked() {
    QSettings settings( "config.ini", QSettings::IniFormat );
    QString initDir = settings.value( LAST_OPEN_DIR_KEY ).toString();
    auto file = QFileDialog::getSaveFileName( this, "Speichern", initDir, "*.tools" );
    settings.setValue( LAST_OPEN_DIR_KEY, QFileInfo( file ).absolutePath() );

    model_.save( file );
}


void MainWindow::on_importButton_clicked() {
    QSettings settings( "config.ini", QSettings::IniFormat );
    QString initDir = settings.value( LAST_OPEN_DIR_KEY ).toString();
    auto file = QFileDialog::getOpenFileName( this, "Öffnen", initDir, "*.tl" );
    settings.setValue( LAST_OPEN_DIR_KEY, QFileInfo( file ).absolutePath() );

    model_.import( file );
    sort_->invalidate();
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_exportButton_clicked() {
    QSettings settings( "config.ini", QSettings::IniFormat );
    QString initDir = settings.value( LAST_OPEN_DIR_KEY ).toString();
    auto file = QFileDialog::getSaveFileName( this, "Speichern", initDir, "*.tl" );
    settings.setValue( LAST_OPEN_DIR_KEY, QFileInfo( file ).absolutePath() );

    model_.exportTo( file );
}

void MainWindow::on_deleteButton_clicked() {
    auto index = ui->tableView->currentIndex();
    auto src = sort_->mapToSource( index );
    model_.removeRow( src.row(), src.parent() );
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::onCurrentChanged( const QModelIndex& current, const QModelIndex& prev ) {
    ui->deleteButton->setEnabled( current.isValid() );
}

void MainWindow::on_addButton_clicked() {
    model_.insertRow( 0, QModelIndex() );
    QModelIndex idx = model_.index( 0, 0 );
    auto viewIndex = sort_->mapFromSource( idx );
    ui->tableView->setCurrentIndex( viewIndex );
}

void MainWindow::on_reindexButton_clicked() {
    for( int i = 0; i < model_.rowCount( QModelIndex() ); ++i ) {
        auto index = sort_->index( i, 0 );
        sort_->setData( index, i + 1 );
    }
    sort_->invalidate();
}
