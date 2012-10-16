#ifndef SmfObj_H
#define SmfObj_H

#include <vector>
#include <fstream> 
#include <sstream>

#include "vertex.h"
#include "vector.h"
using namespace std;

class SmfObj
{
	public:
	SmfObj();
	~SmfObj();
	SmfObj(char const * const filename);		//read file to construct
	int ObjRead(char  const * const filename);	//read a file
	
	vector< vector<unsigned> * > triangles; //indices	//stores the faces information, each face consists of 3 indices
	vector< Vertex * > vertices; 			//vertices information, each index consists of x,y,z cordinates
	vector< vector<unsigned> * > quads;
	
	vector< Vector3D > triNorms;	//stores normal for every face
	vector< Vector3D > quadNorms;	//stores normal for every face
	bool enableRGB;
	bool enableVectexNorm;
	bool enableFaceNorm;
	
	void draw();
	
	void getFaceNormals();
	
	
	private:
	void readSmf(std::ifstream  *  const input);
	void readPly(std::ifstream  * const input);
};








#endif
