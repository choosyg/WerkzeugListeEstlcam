#include "EstlcamFile.h"

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>
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

union ConvertInt {
    unsigned int d;
    char bytes[sizeof( unsigned int )];
};

unsigned int strToInt( const std::string& str, size_t& pos ) {
    ConvertInt value;
    memcpy( value.bytes, &str[pos], 8 );
    pos += 8;
    return value.d;
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
    parameters[3] = Parameter( "Vorschub", 0.0, "mm/min" );
    parameters[4] = Parameter( "Eintauchgeschwindigkeit", 0.0, "mm/min" );
    parameters[5] = Parameter( "Schlicht-Zustellung", 0.0, "mm" );
    parameters[6] = Parameter( "Schlicht-Vorschub", 0.0, "mm/min" );
    parameters[7] = Parameter( "schlichtenEintauchgeschwindigkeit", 0.0, "mm/min" );
    parameters[8] = Parameter( "schlichtenDrehzahl", 0.0, "1/min" );
    parameters[9] = Parameter( "raumzustellung", 0.0, "mm" );
    parameters[10] = Parameter( "wirbelzustellung", 0.0, "mm" );
    parameters[11] = Parameter( "wirbelbreite", 0.0, "mm" );
    parameters[12] = Parameter( "wirbelOszillation", 0.0, "mm" );
    parameters[13] = Parameter( "schneidenwinkel", 0.0, "°" );
    parameters[14] = Parameter( "mittenversatz", 0.0, "mm" );
    parameters[15] = Parameter( "spizenversatz", 0.0, "mm" );
    parameters[16] = Parameter( "kantenradius", 0.0, "mm" );
    parameters[17] = Parameter( "Unbekannnt", 0.0, "mm" );
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
    int estlcamVersin = strToInt( str, offset );
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
}
