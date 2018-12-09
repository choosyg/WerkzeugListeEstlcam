#include "EstlcamFile.h"

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace boost::iostreams;
using namespace boost::uuids;

namespace {

union ConvertDouble {
    double d;
    char bytes[sizeof( double )];
};

double strToDouble( const std::string& str, size_t& pos ) {
    ConvertDouble value;
    memcpy( value.bytes, &str[pos], 8 );
    pos += 8;
    return value.d;
}

std::string doubleToStr( double d ) {
    ConvertDouble value;
    value.d = d;

    std::string str;
    str.resize( 8 );
    memcpy( &str[0], value.bytes, 8 );
    return str;
}

union ConvertInt {
    unsigned int d;
    char bytes[sizeof( unsigned int )];
};

unsigned int strToInt( const std::string& str, size_t& pos ) {
    ConvertInt value;
    memcpy( value.bytes, &str[pos], 4 );
    pos += 4;
    return value.d;
}

std::string intToStr( int d ) {
    ConvertInt value;
    value.d = d;

    std::string str;
    str.resize( 4 );
    memcpy( &str[0], value.bytes, 4 );
    return str;
}

} // namespace

Parameter::Parameter( std::string n, double v, std::string u ) : name( n ), value( v ), unit( u ) {
}

std::string Parameter::toString() const {
    return name + ": " + std::to_string( value ) + unit;
}


EstlcamTool::EstlcamTool() {
    parameters[0] = Parameter( "Durchmesser", 0.0, "mm" );
    parameters[1] = Parameter( "Zustellung", 0.0, "mm" );
    parameters[2] = Parameter( "Eintauchwinkel", 0.0, "°" );
    parameters[3] = Parameter( "Vorschub", 0.0, "mm/s" );
    parameters[4] = Parameter( "Eintauchgeschwindigkeit", 0.0, "mm/s" );
    parameters[5] = Parameter( "Drehzahl", 0.0, "upm" );
    parameters[6] = Parameter( "Schlicht-Zustellung", 0.0, "mm" );
    parameters[7] = Parameter( "Schlicht-Vorschub", 0.0, "mm/s" );
    parameters[8] = Parameter( "schlichtenEintauchgeschwindigkeit", 0.0, "mm/s" );
    parameters[9] = Parameter( "schlichtenDrehzahl", 0.0, "upm" );
    parameters[10] = Parameter( "raumzustellung", 0.0, "%" );
    parameters[11] = Parameter( "wirbelzustellung", 0.0, "%" );
    parameters[12] = Parameter( "wirbelbreite", 0.0, "%" );
    parameters[13] = Parameter( "wirbelOszillation", 0.0, "mm" );
    parameters[14] = Parameter( "schneidenwinkel", 0.0, "°" );
    parameters[15] = Parameter( "mittenversatz", 0.0, "mm" );
    parameters[16] = Parameter( "spizenversatz", 0.0, "mm" );
    parameters[17] = Parameter( "kantenradius", 0.0, "mm" );
    parameters[18] = Parameter( "Unbekannnt", 0.0, "mm" );
}

std::string EstlcamTool::toString() const {
    std::string str = name;

    for( const auto& p : parameters ) {
        str += "\n" + p.toString();
    }

    return str;
}

EstlcamFile::EstlcamFile( const std::string& file ) : file_( file ) {
}

EstlcamToolList EstlcamFile::read() {
    std::ifstream is( file_.c_str(), std::ios_base::in | std::ios_base::binary );
    filtering_streambuf< input > in;
    in.push( gzip_decompressor() );
    in.push( is );

    std::stringstream ss;
    copy( in, ss );
    std::string str = ss.str();

    size_t offset = 0;
    int estlcamVersion = strToInt( str, offset );
    int count = strToInt( str, offset );
    // std::cout << "Estlcam Version " << estlcamVersin / 1000.0 << std::endl;

    EstlcamToolList list;
    while( offset < str.size() ) {
        EstlcamTool tool;

        int idlength = str[offset];
        offset += 1;

        std::string uuidStr
            = str.substr( offset, idlength ).insert( 8, "-" ).insert( 13, "-" ).insert( 18, "-" ).insert( 23, "-" );
        tool.uuid = boost::lexical_cast< uuid >( uuidStr );
        offset += idlength;

        tool.nummer = strToDouble( str, offset );

        int namelength = str[offset];
        offset += 1;

        tool.name = str.substr( offset, namelength );
        offset += namelength;

        for( auto& p : tool.parameters ) {
            p.value = strToDouble( str, offset );
        }

        // End bit = 1
        offset += 1;
        list.push_back( tool );
    }
    return list;
}

void EstlcamFile::write( const EstlcamToolList& etl ) {
    std::ofstream os( file_.c_str(), std::ios_base::out | std::ios_base::binary );
    filtering_streambuf< output > out;
    out.push( gzip_compressor() );
    out.push( os );

    std::string str = intToStr( 11040 ); // Estlcam Version
    str += intToStr( etl.size() );
    for( const auto& tool : etl ) {
        str += (char)32;

        std::string uuid = boost::lexical_cast< std::string >( tool.uuid );
        uuid.erase( 23, 1 ).erase( 18, 1 ).erase( 13, 1 ).erase( 8, 1 );
        std::transform( uuid.begin(), uuid.end(), uuid.begin(), ::toupper );
        str += uuid;

        str += doubleToStr( tool.nummer );

        str += (char)tool.name.size();
        str += tool.name;

        for( auto& p : tool.parameters ) {
            str += doubleToStr( p.value );
        }

        str += (char)1;
    }

    std::stringstream ss;
    ss << str;
    copy( ss, out );
}
