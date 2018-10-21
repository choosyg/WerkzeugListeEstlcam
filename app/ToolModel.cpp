#include "ToolModel.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

ToolModel::ToolModel( QObject* parent ) : QAbstractTableModel( parent ) {
}

void ToolModel::load( const QString& file ) {
    beginResetModel();
    list_ = EstlcamFile( file.toStdString() ).read();
    endResetModel();
}

int ToolModel::rowCount( const QModelIndex& parent ) const {
    if( parent.isValid() ) {
        return 0;
    }

    return list_.size();
}

int ToolModel::columnCount( const QModelIndex& parent ) const {
    if( list_.empty() || parent.isValid() ) {
        return 0;
    }

    return list_[0].parameters.size() + 2;
}

QVariant ToolModel::data( const QModelIndex& index, int role ) const {
    if( !index.isValid() ) {
        return QVariant();
    }

    if( role == Qt::DisplayRole || role == Qt::EditRole ) {
        if( index.column() == 0 ) {
            return list_[index.row()].nummer;
        }
        if( index.column() == 1 ) {
            return list_[index.row()].name.c_str();
        }
        if( index.column() > 1 ) {
            auto p = list_[index.row()].parameters[index.column() - 2];
            return p.value;
        }
    }

    return QVariant();
}

bool ToolModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() | role != Qt::EditRole ) {
        return false;
    }

    if( index.column() == 0 ) {
        list_[index.row()].nummer = value.toDouble();
        return true;
    }
    if( index.column() == 1 ) {
        list_[index.row()].name = value.toString().toStdString();
        return true;
    }
    if( index.column() > 1 ) {
        list_[index.row()].parameters[index.column() - 2].value = value.toDouble();
        return true;
    }

    return false;
}

QVariant ToolModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole || orientation != Qt::Horizontal || list_.empty() ) {
        return QVariant();
    }
    if( section == 0 ) {
        return "Nr.";
    }
    if( section == 1 ) {
        return "Name";
    }
    if( section > 1 ) {
        auto p = list_[0].parameters[section - 2];
        return p.name.c_str();
    }
}

Qt::ItemFlags ToolModel::flags( const QModelIndex& index ) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
