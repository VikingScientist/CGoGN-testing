#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>
#include "JSON.h"
#include "Topology/map/embeddedMap2.h"
#include "Topology/generic/parameters.h"
#include "Geometry/vector_gen.h"
#include "Algo/Tiling/Surface/square.h"

using namespace std;
using namespace util;
using namespace CGoGN;
// using namespace CGoGN::Geom;

struct PFP : public PFP_STANDARD {
  typedef EmbeddedMap2 MAP;
};

typedef PFP::MAP  MAP;
typedef PFP::VEC3 VEC3;

void sewAndPhi1(MAP& map, Dart& d1, Dart &d2) {
  map.sewFaces(d1,d2);
  d1 = map.phi1(d1);
  d2 = map.phi1(d2);
}

void initIcosahedron(MAP& map) {
  Dart face[20];
  for(int i=0; i<20; i++)
    face[i] = map.newFace(3);
  sewAndPhi1(map,face[ 1], face[ 6]); // bottom face
  sewAndPhi1(map,face[ 1], face[13]);
  sewAndPhi1(map,face[ 1], face[10]);
  sewAndPhi1(map,face[10], face[17]); // lvl 1
  sewAndPhi1(map,face[10], face[ 4]);
  sewAndPhi1(map,face[ 6], face[ 8]);
  sewAndPhi1(map,face[ 6], face[ 9]);
  sewAndPhi1(map,face[13], face[19]);
  sewAndPhi1(map,face[13], face[12]);
  sewAndPhi1(map,face[12], face[18]); // lvl 2
  sewAndPhi1(map,face[12], face[17]);
  sewAndPhi1(map,face[17], face[15]);
  sewAndPhi1(map,face[ 4], face[ 5]);
  sewAndPhi1(map,face[ 4], face[ 8]);
  sewAndPhi1(map,face[ 8], face[ 2]);
  sewAndPhi1(map,face[ 9], face[ 3]);
  sewAndPhi1(map,face[ 9], face[19]);
  sewAndPhi1(map,face[19], face[16]);
  sewAndPhi1(map,face[16], face[11]); // lvl 3
  sewAndPhi1(map,face[16], face[18]);
  sewAndPhi1(map,face[18], face[14]);
  sewAndPhi1(map,face[14], face[ 0]); // aaaand messed up
  sewAndPhi1(map,face[14], face[15]);
  sewAndPhi1(map,face[15], face[ 5]);
  sewAndPhi1(map,face[ 5], face[ 7]);
  face[0] = map.phi1(face[0]);
  sewAndPhi1(map,face[ 7], face[ 0]);
  sewAndPhi1(map,face[ 7], face[ 2]);
  sewAndPhi1(map,face[ 2], face[ 3]);
  sewAndPhi1(map,face[ 3], face[11]);
  face[0] = map.phi1(face[0]);
  sewAndPhi1(map,face[11], face[ 0]);
}


int main(int argc, char **argv) {
    
  MAP map;
  initIcosahedron(map);
  
  VertexAttribute<VEC3, MAP> position       = map.addAttribute<VEC3, VERTEX, MAP>("position");
  VertexAttribute<int, MAP>  vertex_index   = map.addAttribute<int, VERTEX, MAP>("vi");
  EdgeAttribute<int, MAP>    line_index     = map.addAttribute<int, EDGE, MAP>("ei");
  FaceAttribute<int, MAP>    face_index     = map.addAttribute<int, FACE, MAP>("fi");

  // CellMarker<MAP, VERTEX> cm(map);
  DartMarker<MAP> dm(map);

  int k = 0;
  for(Dart d=map.begin(); d!=map.end(); map.next(d)) {
    cout << "dart #" << k++ << " is edge? ";
    for(int i=0; i<3; i++) {
      cout << map.isBoundaryEdge(d) << " " ;
      d = map.phi1(d);
    }
    cout << endl;
    dm.unmark(d);
  }

  // create global index enumeration
  int globI = 0;
  for(Dart d=map.begin(); d!=map.end(); map.next(d)) {
    if( !dm.isMarked(d) ) {
      cout << "New vertex #" << globI << "( on dart #" << d << " )" << endl;
      vertex_index[d] = globI++;
      dm.markOrbit<VERTEX>(d);
    }
  }
  for(Dart d=map.begin(); d!=map.end(); map.next(d))
    dm.unmark(d);

  // create global edge enumeration
  globI = 0;
  for(Dart d=map.begin(); d!=map.end(); map.next(d)) {
    if( !dm.isMarked(d) ) {
      cout << "New edge #" << globI << "( on dart #" << d << " )" << endl;
      line_index[d] = globI++;
      dm.markOrbit<EDGE>(d);
    }
  }
  for(Dart d=map.begin(); d!=map.end(); map.next(d))
    dm.unmark(d);

  // create global face enumeration
  globI = 0;
  for(Dart d=map.begin(); d!=map.end(); map.next(d)) {
    if( !dm.isMarked(d) ) {
      cout << "New face #" << globI << "( on dart #" << d << " )" << endl;
      face_index[d] = globI++;
      dm.markOrbit<FACE>(d);
    }
  }

  cout << "(F,E,V)" << endl;
  for(Dart d = map.begin(); d!=map.end(); map.next(d))
    cout << "(" << face_index[d] << ", " << line_index[d] << ", " << vertex_index[d] << ")" << endl;



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

  // ofstream out( "test.json" );
  // out << json;
  // out.close();
} 
