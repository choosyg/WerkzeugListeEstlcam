#pragma once

#include <estlcam/EstlcamFile.h>

#include <QAbstractTableModel>

class ToolModel : public QAbstractTableModel {
public:
    explicit ToolModel( QObject* parent = nullptr );

    void load( const QString& filename );
    void save( const QString& filename );

    void import( const QString& filename );
    void exportTo( const QString& filename );

    bool removeRows( int row, int count, const QModelIndex& parent ) override;
    bool insertRows( int row, int count, const QModelIndex& parent ) override;

    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    bool setData( const QModelIndex& index, const QVariant& value, int role ) override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    Qt::ItemFlags flags( const QModelIndex& index ) const override;

private:
    EstlcamToolList tools_;
};
