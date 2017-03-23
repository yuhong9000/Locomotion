#include "Skin.h"


Skin::Skin()
{
}


Skin::~Skin()
{
	delete[] vertex;
	delete[] normal;
	delete[] skinweight;
	delete[] dvertex;
	delete[] dnormal;
	delete[] binding;
	delete[] triangle;
}

void Skin::setSkel(Skeleton* s)
{
	skel = s;
	numjoint = s->numJoint();
}

void Skin::load(const char* file)
{
	Tokenizer token;
	token.Open(file);

	char tmp[256];

	while (1){
		token.GetToken(tmp);
	
		if (strcmp(tmp, "positions") == 0){
			int n = token.GetInt();
			numvertex = n;

			token.FindToken("{");

			vertex = new Vector3[n];
			dvertex = new Vector3[n];

			for (int i = 0; i < n; i++)
			{
				vertex[i].x = token.GetFloat();
				vertex[i].y = token.GetFloat();
				vertex[i].z = token.GetFloat();
			}
		}
		else if (strcmp(tmp, "normals") == 0){
			int n = token.GetInt();
			token.FindToken("{");

			normal = new Vector3[n];
			dnormal = new Vector3[n];

			for (int i = 0; i < n; i++)
			{
				normal[i].x = token.GetFloat();
				normal[i].y = token.GetFloat();
				normal[i].z = token.GetFloat();
				normal[i].Normalize();
			}
		}
		else if (strcmp(tmp, "skinweights") == 0){
			int n = token.GetInt();
			int jn = skel->numJoint(); // total number of joints
			token.FindToken("{");

			skinweight = new float[n*jn];

			using namespace std;

			for (int i = 0; i < n; i++){
				int m = token.GetInt();
				
				// initialize all skinweight to zero
				for (int j = 0; j < jn; j++){
					skinweight[i*jn+j] = 0.0;
				}

				// loading skin weight for vertex i with joint index---index
				for (int j = 0; j < m; j++){
					int index = token.GetInt();
					skinweight[i*jn+index] = token.GetFloat();
				}
			}
		}
		else if (strcmp(tmp, "triangles") == 0){
			int n = token.GetInt();
			numtriangle = n;
			token.FindToken("{");

			triangle = new int[n * 3];

			for (int i = 0; i < n; i++){
				for (int j = 0; j < 3; j++){
					triangle[i*3 + j] = token.GetInt();
				}
			}
		}
		else if (strcmp(tmp, "bindings") == 0){
			int n = token.GetInt();
			token.FindToken("{");

			binding = new Matrix34[n];

			for (int i = 0; i < n; i++){
				token.FindToken("matrix");
				token.FindToken("{");

				float x, y, z;
				
				x = token.GetFloat();
				y = token.GetFloat();
				z = token.GetFloat();
				binding[i].a = Vector3(x, y, z);
				
				x = token.GetFloat();
				y = token.GetFloat();
				z = token.GetFloat();
				binding[i].b = Vector3(x, y, z);
				
				x = token.GetFloat();
				y = token.GetFloat();
				z = token.GetFloat();
				binding[i].c = Vector3(x, y, z);

				x = token.GetFloat();
				y = token.GetFloat();
				z = token.GetFloat();
				binding[i].d = Vector3(x, y, z);

				binding[i].Inverse();
				token.FindToken("}");
			}
			
			break;
		}
		token.FindToken("}");
	}

	token.Close();
}

void Skin::update()
{
	for (int i = 0; i < numvertex; i++){
		// temporary vectors for calculating weighted sum

		Vector3 sum(0, 0, 0);
		Vector3 nsum(0, 0, 0);

		for (int j = 0; j < numjoint; j++){
			Vector3 tmp;
			Vector3 ntmp;
			// if weight of a joint is not zero
			// computer the weighted product
			if (skinweight[i*numjoint + j] != 0){
				Matrix34 M;
				
				// M = W*B^-1//////////////////////////////
				M.Dot(*(skel->worldlist[j]),binding[j]);

				M.Transform(vertex[i], tmp);
				M.Transform3x3(normal[i], ntmp);
				///////////////////////////////////////////
			
				sum += skinweight[i*numjoint + j] * tmp;
				ntmp.Normalize();
				nsum += skinweight[i*numjoint + j] * ntmp;

			}
		}

		dvertex[i] = sum;
		dnormal[i] = nsum;
		dnormal[i].Normalize();
	}
}

void Skin::draw()
{
	glMatrixMode(GL_MODELVIEW);
	Matrix34 glmatrix;
	glmatrix.Identity();
	glLoadMatrixf(glmatrix);
	for (int i = 0; i < numtriangle; i++){
		int a, b, c; // indices of triangle vertex

		a = triangle[i*3];
		b = triangle[i*3+1];
		c = triangle[i*3+2];
		
		glBegin(GL_TRIANGLES);
		glNormal3f(dnormal[a].x, dnormal[a].y, dnormal[a].z);
		glVertex3f(dvertex[a].x, dvertex[a].y, dvertex[a].z);

		glNormal3f(dnormal[b].x, dnormal[b].y, dnormal[b].z);
		glVertex3f(dvertex[b].x, dvertex[b].y, dvertex[b].z);

		glNormal3f(dnormal[c].x, dnormal[c].y, dnormal[c].z);
		glVertex3f(dvertex[c].x, dvertex[c].y, dvertex[c].z);
		glEnd();
	}
}