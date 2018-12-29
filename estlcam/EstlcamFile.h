#pragma once

#include <boost/uuid/uuid.hpp>

#include <array>
#include <string>
#include <vector>

class Parameter {
public:
    Parameter( std::string n = "", double v = 0.0, std::string u = "" );
    std::string name;
    double value;
    std::string unit;
    std::string toString() const;
};

class EstlcamTool {
public:
    EstlcamTool();
    boost::uuids::uuid uuid;
    double nummer = 0.0;
    std::string name;

    std::array< Parameter, 19 > parameters;

    std::string toString() const;
};

typedef std::vector< EstlcamTool > EstlcamToolList;

class EstlcamFile {
public:
    EstlcamFile( const std::string& file );

    EstlcamToolList read();
    void write( const EstlcamToolList& etl );

private:
    const std::string file_;
};
