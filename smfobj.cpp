#include <iostream> 


#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "smfobj.h"
#include "vertex.h"
#include <fstream> 
#include <sstream>


#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif




//constructor
SmfObj::SmfObj()
{
enableRGB = false;
enableVectexNorm= false;
enableFaceNorm=false;
//do nothing
//triangles = NULL;
//quads = NULL;
//vertices = NULL;


}

//destructor


SmfObj::SmfObj(char const *  const filename)
{
	
	ObjRead(filename);
}

SmfObj::~SmfObj()
{
	cout<<"SmfObj Destructor"<<endl;
	for (unsigned i=0; i < triangles.size();i++)
	{	
		if (triangles[i]!=NULL)
		{
			delete triangles[i];
		}
	}
	triangles.clear();
		
	for (unsigned i=0; i < vertices.size();i++)
	{
		if (vertices[i]!=NULL)
		{
			delete vertices[i];
		}
	}
	vertices.clear();


	for (unsigned i=0; i < quads.size();i++)
	{	
		if (quads[i]!=NULL)
		{
			delete quads[i];
		}
	}
	triangles.clear();
}



void SmfObj::readSmf(std::ifstream * const input)
{
//	std::string line;
	
	for( std::string line; getline( *input, line ); )
	{
//		float co[3];
//		int index[3];
		int i;
		if (!line.empty())
		{
			i=0;
			char * cstr;
			char * p;
			cstr = new char [line.size()+1];
			strcpy(cstr,line.c_str());
			
			p=strtok(cstr," ");
			
			if (*p == 'v')
			{
//				cout<<"v: ";
				p=strtok(NULL," ");
				Vertex * temp = new Vertex;
				
				while (p!=NULL)
				{
//					printf("%s ",p);

					
					temp->co[i]= atof(p);			
					
//					cout<<temp->co[i]<<" ";
					p=strtok(NULL," ");	
						
//					cout<<co[i]<<" ";
					i++;	
					if (i==3)
					{
						break;
					}				
				}
				vertices.push_back(temp);
			}
			else if (*p == 'f') //it is a face
			{
				p=strtok(NULL," ");
				
				vector<unsigned> * index = new vector<unsigned>;
				
//				cout<<"f ";
				
				while (p!=NULL)
				{
//					printf("%s ",p);
					int tempIndex = atoi(p) -1 ;
					if (tempIndex < -1)
					{
						cout<<"invalid face index"<<endl;
						abort();
					}
					
					index->push_back(tempIndex);
//					cout<<tempIndex<<" ";
					p=strtok(NULL," ");			
					i++;
					if (i==3)
					{
						break;
					}
										
				}
				
				triangles.push_back(index);
			}
			else
			{
				cout<<"ignore: "<<line<<endl;
				continue;
				delete [] cstr;				
			}
			
		
		
			if (i<3) //smf and not 3 vertices
			{
				cout<<"Invalid Vertex"<<endl;
				exit(0);
			}
			
			delete [] cstr;
		}
//		cout<<endl;
	    
	}
	
	cout<<"vertices size: "<<vertices.size()<<endl;
	cout<<"face size: "<<triangles.size()<<endl;





}

//*****************************
//*********READ PLY************
//*****************************
void SmfObj::readPly(std::ifstream * const input)
{
//	std::string line;
	
	int vertexLine =0;
	int faceLine=0;
	bool endofHeader = false;

	
	for( std::string line; getline( *input, line ); )
	{
		int i;
		if (!line.empty())
		{
			i=0;
			char * cstr;
			char * p;
			cstr = new char [line.size()+1];
			strcpy(cstr,line.c_str());
			
			p=strtok(cstr," ");
			
			
			if (strcmp(p,"element") ==0 )
			{
				
				p=strtok(NULL," ");
				
				if (strcmp(p,"face") ==0 )
				{
//					cout<<"element face: ";
					p=strtok(NULL," ");
					faceLine = atoi(p);
//					cout<<faceLine<<endl;			
				}
				else if (strcmp(p,"vertex")==0)
				{
//					cout<<"element vertex: ";
					p=strtok(NULL," ");
					vertexLine = atoi(p);
//					cout<<vertexLine<<endl;	
				}
				else
				{
					cout<<"**********some other element************"<<endl;
					printf("%s\n",p);
				}
				
				
				
			}
			else if (strcmp(p,"end_header") ==0 )
			{
				endofHeader=true;

				//START READING VERTICES				
				int linecount=0;
				while(linecount<vertexLine)
				{
					getline(*input,line);
					char * temp = new char[line.size()+1];
					strcpy(temp,line.c_str());
					
					char* curr =strtok(temp," ");
					
					Vertex *  ver= new Vertex;					
//					curr=strtok(NULL," ");				
					ver->co[0] = atof(curr);
					curr=strtok(NULL," ");				
					ver->co[1] = atof(curr);				
					curr=strtok(NULL," ");				
					ver->co[2] = atof(curr);
					curr=strtok(NULL," ");						
					
					
					
					
					if (curr!=NULL)
					{
						ver->norm[0] = atof(curr);
						curr=strtok(NULL," ");				
						ver->norm[1] = atof(curr);				
						curr=strtok(NULL," ");				
						ver->norm[2] = atof(curr);
						enableVectexNorm=true;
					}	
					
						
					
					curr=strtok(NULL," ");	
					if (curr!=NULL)
					{
//						cout<<"has rgb"<<endl;
						enableRGB=true;
						ver->r = atof(curr)/255;
						curr=strtok(NULL," ");
						ver->g = atof(curr)/255;
						curr=strtok(NULL," ");
						ver->b = atof(curr)/255;
						
					}						
					
					vertices.push_back(ver);
					

//					printf("%s \n",curr);
					
//					printf("%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n", 
//						ver->co[0],ver->co[1],ver->co[2],ver2->co[0],ver2->co[1],ver2->co[2]);
					
					linecount++;
				}
				
				
//				

				linecount=0;
				while (linecount<faceLine)
				{
					getline(*input,line);
					char * temp = new char[line.size()+1];
					strcpy(temp,line.c_str());
					
					char* curr =strtok(temp," ");
						
					int num = atoi(curr);
					
					unsigned i,j,k,l;
					if (num==3)
					{
						curr=strtok(NULL," ");
						i=atoi(curr);
						curr=strtok(NULL," ");
						j=atoi(curr);
						curr=strtok(NULL," ");
						k=atoi(curr);
						
						vector<unsigned> * vec = new vector<unsigned>;
						vec->push_back(i);
						vec->push_back(j);
						vec->push_back(k);
						triangles.push_back(vec);
							
//						printf("%d %d %d\n",i,j,k);
					}
					else if (num==4)
					{
						curr=strtok(NULL," ");
						i=atoi(curr);
						curr=strtok(NULL," ");
						j=atoi(curr);
						curr=strtok(NULL," ");
						k=atoi(curr);
						curr=strtok(NULL," ");
						l=atoi(curr);	
						
						vector<unsigned> * vec = new vector<unsigned>;
						vec->push_back(i);
						vec->push_back(j);
						vec->push_back(k);
						vec->push_back(l);
						quads.push_back(vec);						
						
//						printf("%d %d %d %d\n",i,j,k,l);
					
					}
					else
					{
					
					}
				
					linecount++;
				}
				
//				exit(0);
//				vertices.push_back(temp);
			}
			else
			{
				cout<<"somewhere else"<<endl;
			}
			
			
			delete [] cstr;
		}
//		cout<<endl;
	    
	}
	
	cout<<"vertices size: "<<vertices.size()<<endl;
	cout<<"face size: "<<triangles.size()<<endl;
	cout<<"PLY READING FINISHED"<<endl<<endl;




}

int SmfObj::ObjRead(char  const * const filename)
{

	cout<<"reading "<<filename<<endl;
	
	std::ifstream * input = new std::ifstream(filename);
	
	std::string line;
	getline(*input,line);
	
	char * cstr = new char [line.size()+1];
	strcpy(cstr,line.c_str());
	
	if ( strcmp( cstr ,"ply") == 0)
	{
		cout<<"PLY FILE"<<endl;
		readPly(input);
//		return 0;
//		exit(0);
	}
	else if ( strcmp( cstr ,"#$SMF 1.0") == 0)
	{
		readSmf(input);
	}
	else
	{
		cout<<"incorrect file type"<<endl;
		exit(0);
	}
	
	delete [] cstr;


	return 0;
}


void SmfObj::draw()
{
//	cout<<"size V F T: "<<vertices.size()<<" "<<triangles.size()<<" "<<quads.size()<<endl;
	int i=0;
	
	
	while(i<1)
	{
		if (i==0)
		{	
			glColor3f(0,0,0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			cout<<"fill"<<endl;
		}
		else 
		{	

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(0,0,0);
			glLineWidth(2);
		}
		i++;
		
		glBegin(GL_TRIANGLES);

		

			for (unsigned i=0; i<triangles.size();i++)
			{
				if (enableFaceNorm)
				{
					glNormal3f(triNorms[i][0],triNorms[i][1],triNorms[i][2]);
				}					
				
				if (enableRGB )
				{	
//					cout<<"rgb: "<<vertices[triangles[i]->at(0)]->r<<endl;	
					glColor3f(vertices[triangles[i]->at(0)]->r,
					vertices[triangles[i]->at(0)]->g,
					vertices[triangles[i]->at(0)]->b);
					glColor3f(0,0,0);
				}
				
				if (enableVectexNorm)
				{
					glNormal3f(	vertices[triangles[i]->at(0)]->norm[0],
							vertices[triangles[i]->at(0)]->norm[1],
							vertices[triangles[i]->at(0)]->norm[2]);
				}
				


					
				glVertex3f(vertices[triangles[i]->at(0)]->co[0],
					vertices[triangles[i]->at(0)]->co[1],
					vertices[triangles[i]->at(0)]->co[2]);	
				
				if (enableRGB )
				{
					glColor3f(vertices[triangles[i]->at(1)]->r,
					vertices[triangles[i]->at(1)]->g,
					vertices[triangles[i]->at(1)]->b);
				}					
					
				if (enableVectexNorm)
				{
					glNormal3f(	vertices[triangles[i]->at(1)]->norm[0],
							vertices[triangles[i]->at(1)]->norm[1],
							vertices[triangles[i]->at(1)]->norm[2]);
				}					
					
				glVertex3f(vertices[triangles[i]->at(1)]->co[0],
					vertices[triangles[i]->at(1)]->co[1],
					vertices[triangles[i]->at(1)]->co[2]);
				
				
				if (enableRGB )
				{
					glColor3f(	vertices[triangles[i]->at(2)]->r,
							vertices[triangles[i]->at(2)]->g,
							vertices[triangles[i]->at(2)]->b);
				}	
					
				if (enableVectexNorm)
				{
					glNormal3f(	vertices[triangles[i]->at(2)]->norm[0],
							vertices[triangles[i]->at(2)]->norm[1],
							vertices[triangles[i]->at(2)]->norm[2]);
				}
					
				glVertex3f(vertices[triangles[i]->at(2)]->co[0],
					vertices[triangles[i]->at(2)]->co[1],
					vertices[triangles[i]->at(2)]->co[2]);	
			}
		glEnd();
		
		glBegin(GL_QUADS);
			for (unsigned i=0; i<quads.size();i++)
			{
				if (enableFaceNorm)
				{
					glNormal3f(quadNorms[i][0],quadNorms[i][1],quadNorms[i][2]);
				}
				
				if (enableRGB )
				{	
//					cout<<"rgb: "<<vertices[quads[i]->at(0)]->r<<endl;	
					glColor3f(vertices[quads[i]->at(0)]->r,
					vertices[quads[i]->at(0)]->g,
					vertices[quads[i]->at(0)]->b);
					glColor3f(0,0,0);
				}	
				
				if (enableVectexNorm)
				{	
					glNormal3f(	vertices[quads[i]->at(0)]->norm[0],
							vertices[quads[i]->at(0)]->norm[1],
							vertices[quads[i]->at(0)]->norm[2]);
				}
					
				glVertex3f(vertices[quads[i]->at(0)]->co[0],
					vertices[quads[i]->at(0)]->co[1],
					vertices[quads[i]->at(0)]->co[2]);
					
					
				
				if (enableRGB )
				{
					glColor3f(vertices[quads[i]->at(1)]->r,
					vertices[quads[i]->at(1)]->g,
					vertices[quads[i]->at(1)]->b);
				}	
					
				if (enableVectexNorm)
				{	
					glNormal3f(	vertices[quads[i]->at(1)]->norm[0],
							vertices[quads[i]->at(1)]->norm[1],
							vertices[quads[i]->at(1)]->norm[2]);
				}		
				glVertex3f(vertices[quads[i]->at(1)]->co[0],
					vertices[quads[i]->at(1)]->co[1],
					vertices[quads[i]->at(1)]->co[2]);					
					
				
				if (enableRGB )
				{
					glColor3f(vertices[quads[i]->at(2)]->r,
					vertices[quads[i]->at(2)]->g,
					vertices[quads[i]->at(2)]->b);
				}	
				if (enableVectexNorm)
				{	
					glNormal3f(	vertices[quads[i]->at(2)]->norm[0],
							vertices[quads[i]->at(2)]->norm[1],
							vertices[quads[i]->at(2)]->norm[2]);
				};	
				glVertex3f(vertices[quads[i]->at(2)]->co[0],
					vertices[quads[i]->at(2)]->co[1],
					vertices[quads[i]->at(2)]->co[2]);
					
					
				
				if (enableRGB )
				{
					glColor3f(vertices[quads[i]->at(3)]->r,
					vertices[quads[i]->at(3)]->g,
					vertices[quads[i]->at(3)]->b);
				}	
				if (enableVectexNorm)
				{	
					glNormal3f(	vertices[quads[i]->at(3)]->norm[0],
							vertices[quads[i]->at(3)]->norm[1],
							vertices[quads[i]->at(3)]->norm[2]);
				}
				glVertex3f(vertices[quads[i]->at(3)]->co[0],
					vertices[quads[i]->at(3)]->co[1],
					vertices[quads[i]->at(3)]->co[2]);		
			}
		glEnd();	
	}
}

//this function calculates all the face normals if not given
void SmfObj::getFaceNormals()
{
	enableFaceNorm=true;
	unsigned triSize=triangles.size();	
	unsigned quadSize=quads.size();
	
	triNorms.resize(triSize);
	quadNorms.resize(quadSize);
	
	for (unsigned i=0;i<triSize;i++)
	{
		Vector3D x1 = {	vertices[triangles[i]->at(0)]->co[0],
				vertices[triangles[i]->at(0)]->co[1],
				vertices[triangles[i]->at(0)]->co[2]};
		Vector3D x2 = {	vertices[triangles[i]->at(1)]->co[0],
				vertices[triangles[i]->at(1)]->co[1],
				vertices[triangles[i]->at(1)]->co[2]};
		Vector3D x3 = {	vertices[triangles[i]->at(2)]->co[0],
				vertices[triangles[i]->at(2)]->co[1],
				vertices[triangles[i]->at(2)]->co[2]};
				
		Vector3D cr = cross(x2-x1,x3-x1);
		unitize(cr);
		triNorms[i]=cr;
		
//		cout<<cr<<endl;
		
	}
	
	for (unsigned i=0;i<quadSize;i++)
	{
		Vector3D x1 = {	vertices[quads[i]->at(0)]->co[0],
				vertices[quads[i]->at(0)]->co[1],
				vertices[quads[i]->at(0)]->co[2]};
		Vector3D x2 = {	vertices[quads[i]->at(1)]->co[0],
				vertices[quads[i]->at(1)]->co[1],
				vertices[quads[i]->at(1)]->co[2]};
		Vector3D x3 = {	vertices[quads[i]->at(2)]->co[0],
				vertices[quads[i]->at(2)]->co[1],
				vertices[quads[i]->at(2)]->co[2]};
				
		Vector3D cr = cross(x2-x1,x3-x1);
		unitize(cr);
		quadNorms[i]=cr;
	}
	
	
		
	
}
