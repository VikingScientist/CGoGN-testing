#include <iostream>
#include <stdlib.h>
#include <string>
#include "JSON.h"

using namespace std;
using namespace util;

int main(int argc, char **argv) {
		JSONObject json;
		JSONObject& data = json.upsert( "data", VARIANT_ARRAY );
		data.push_back( JSONObject() );
		JSONObject& plot = data[ 0 ];

		plot.upsert( "type", JSONObject( "scatter3d" ) );
		plot.upsert( "mode", JSONObject( "lines+markers" ) );

		JSONObject& x = plot.upsert( "x", VARIANT_ARRAY );
		JSONObject& y = plot.upsert( "y", VARIANT_ARRAY );
		JSONObject& z = plot.upsert( "z", VARIANT_ARRAY );
		JSONObject& text = plot.upsert( "text", VARIANT_ARRAY );
		JSONObject& size = plot.upsert( "marker" ).upsert( "size", VARIANT_ARRAY );

		x.push_back( 0 );
		y.push_back( 0 );
		z.push_back( 0 );
		text.push_back( "0" );
		size.push_back( 10 );

		x.push_back( 1 );
		y.push_back( 1 );
		z.push_back( 1 );
		text.push_back( "1" );
		size.push_back( 0 );

		x.push_back( JSONObject::Null() );
		y.push_back( JSONObject::Null() );
		z.push_back( JSONObject::Null() );
		size.push_back( 0 );
		text.push_back( JSONObject::Null() );

		x.push_back( 1 );
		y.push_back( 0 );
		z.push_back( 0 );
		text.push_back( "2" );
		size.push_back( 10 );

		x.push_back( 0 );
		y.push_back( 1 );
		z.push_back( 0.5 );
		text.push_back( "3" );
		size.push_back( 0 );

		ofstream out( "test.json" );
		out << json;
		out.close();
} 
