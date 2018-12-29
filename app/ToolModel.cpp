#include "ToolModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

ToolModel::ToolModel( QObject* parent ) : QAbstractTableModel( parent ) {
}

void ToolModel::load( const QString& filename ) {
    beginResetModel();

    QFile file;
    file.setFileName( filename );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QJsonDocument document = QJsonDocument::fromJson( QString( file.readAll() ).toUtf8() );
    file.close();

    tools_.clear();

    QJsonArray array = document.object()["tools"].toArray();
    for( const auto& item : array ) {
        QJsonObject obj = item.toObject();
        EstlcamTool tool;
        tool.uuid = boost::lexical_cast< boost::uuids::uuid >( obj["uuid"].toString().toStdString() );
        tool.nummer = obj["number"].toDouble();
        tool.name = obj["name"].toString().toStdString();

        auto params = obj["parameters"].toArray();
        size_t idx = 0;
        for( const auto& p : params ) {
            auto po = p.toObject();
            tool.parameters[idx].name = po["name"].toString().toStdString();
            tool.parameters[idx].value = po["value"].toDouble();
            tool.parameters[idx].unit = po["unit"].toString().toStdString();
            idx++;
        }

        tools_.push_back( tool );
    }
    endResetModel();
}

void ToolModel::save( const QString& filename ) {
    QJsonArray array;
    for( const auto& tool : tools_ ) {
        QJsonObject toolObject;
        toolObject["uuid"] = boost::lexical_cast< std::string >( tool.uuid ).c_str();
        toolObject["number"] = tool.nummer;
        toolObject["name"] = tool.name.c_str();

        QJsonArray parameters;
        for( const auto& param : tool.parameters ) {
            QJsonObject paramObject;
            paramObject["name"] = param.name.c_str();
            paramObject["value"] = param.value;
            paramObject["unit"] = param.unit.c_str();
            parameters.append( paramObject );
        }
        toolObject["parameters"] = parameters;

        array.append( toolObject );
    }

    QJsonObject global;
    global["version"] = 1;
    global["tools"] = array;

    QJsonDocument document;
    document.setObject( global );

    QFile file;
    file.setFileName( filename );
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    file.write( document.toJson() );
    file.close();
}

void ToolModel::import( const QString& filename ) {
    beginResetModel();
    auto list = EstlcamFile( filename.toStdString() ).read();
    tools_.insert( tools_.end(), list.begin(), list.end() );
    endResetModel();
}

void ToolModel::exportTo( const QString& filename ) {
    EstlcamFile( filename.toStdString() ).write( tools_ );
}

bool ToolModel::removeRows( int row, int count, const QModelIndex& parent ) {
    beginRemoveRows( parent, row, row + count - 1 );
    tools_.erase( tools_.begin() + row, tools_.begin() + row + count );
    endRemoveRows();
    return true;
}

bool ToolModel::insertRows( int row, int count, const QModelIndex& parent ) {
    beginInsertRows( parent, row, row + count - 1 );
    tools_.insert( tools_.begin() + row, count, EstlcamTool() );
    endInsertRows();
    return true;
}

int ToolModel::rowCount( const QModelIndex& parent ) const {
    if( parent.isValid() ) {
        return 0;
    }

    return static_cast< int >( tools_.size() );
}

int ToolModel::columnCount( const QModelIndex& parent ) const {
    if( tools_.empty() || parent.isValid() ) {
        return 0;
    }

    return static_cast< int >( tools_[0].parameters.size() + 2 );
}

QVariant ToolModel::data( const QModelIndex& index, int role ) const {
    if( !index.isValid() ) {
        return QVariant();
    }

    if( role == Qt::DisplayRole ) {
        if( index.column() == 0 ) {
            return tools_[index.row()].nummer;
        }
        if( index.column() == 1 ) {
            return tools_[index.row()].name.c_str();
        }
        if( index.column() > 1 ) {
            auto p = tools_[index.row()].parameters[index.column() - 2];
            if( p.value <= 0 ) {
                return QVariant();
            }
            if( p.unit == "mm/s" ) {
                return QString::number( p.value * 60 ) + "mm/min";
            }
            return QString::number( p.value ) + p.unit.c_str();
        }
    }

    if( role == Qt::EditRole ) {
        if( index.column() == 0 ) {
            return tools_[index.row()].nummer;
        }
        if( index.column() == 1 ) {
            return tools_[index.row()].name.c_str();
        }
        if( index.column() > 1 ) {
            auto p = tools_[index.row()].parameters[index.column() - 2];
            if( p.unit == "mm/s" ) {
                return p.value * 60;
            }
            return p.value;
        }
    }
    if( role == Qt::ToolTipRole ) {
        return boost::lexical_cast< std::string >( tools_[index.row()].uuid ).c_str();
    }

    return QVariant();
}

bool ToolModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole ) {
        return false;
    }

    if( index.column() == 0 ) {
        tools_[index.row()].nummer = value.toDouble();
        return true;
    }
    if( index.column() == 1 ) {
        tools_[index.row()].name = value.toString().toStdString();
        return true;
    }
    if( index.column() > 1 ) {
        auto& param = tools_[index.row()].parameters[index.column() - 2];
        if( param.unit == "mm/s" ) {
            param.value = value.toDouble() / 60;
        } else {
            param.value = value.toDouble();
        }
        return true;
    }

    return false;
}

QVariant ToolModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole || orientation != Qt::Horizontal || tools_.empty() ) {
        return QVariant();
    }
    if( section == 0 ) {
        return "Nr.";
    }
    if( section == 1 ) {
        return "Name";
    }
    if( section > 1 ) {
        auto p = tools_[0].parameters[section - 2];
        return p.name.c_str();
    }
    return QVariant();
}

Qt::ItemFlags ToolModel::flags( const QModelIndex& index ) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
