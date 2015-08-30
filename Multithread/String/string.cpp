#include<iostream>
#include<cstdlib>
#include<fstream>
#include<unistd.h>
#include<sys/time.h>

using std::cout;
using std::endl;
using std::cin;

float  a=0.7;
float  deltaX=1;
//int f=0;
float deltaTime=0.0;
float fullTime=0.0;

float force=0.0;
int zForce=0;



int f(int x)
{
		if(x==zForce)
				return force; 
		else
				return 0;
}



float  Zij(int prevZ,int nextZ,int currentZ,int prevT,int currentX,int currentTime)
{
		float part1=0.0;
		if(currentTime==2)
		part1 =a*a*(nextZ-2*currentZ+prevZ)+f(currentX)*deltaTime*deltaTime;
		else
		part1 =a*a*(nextZ-2*currentZ+prevZ);

		float znam=deltaX*deltaX;
		float result=part1/znam+2*currentZ-prevT;
		return result;

}


float ** solveEquation(int nodeCount)
{
		//int ** data=NULL;
		int i=0,j=0;

		int steps=0;
		steps=fullTime/deltaTime;

		//int data[steps][nodeCount];
		float** data = new float*[steps];
		for(i=0;i<steps;i++)
				data[i]=new float[nodeCount];

		for(j=0;i<2;i++)
		for(i=0;i<nodeCount;i++)
				data[0][i]=0;






		for(i=2;i<steps;i++)
		{
				data[i][0]=0;
				data[i][nodeCount-1]=0;
				for(j=1;j<nodeCount-1;j++)
				{
						data[i][j]=Zij(data[i-1][j-1],data[i-1][j+1],data[i-1][j],data[i-2][j],j,i); 

				}
		}



		return data;




}






int main(int argc, char** argv)
{

		struct timeval tv1,tv2,dtv;
		struct timezone tz;

		int nodeCount=0;
		float ** data=NULL;
		std::ofstream file;

		if(argc<3)
		{
				cout<<"Usage: ./prog_name node_count time_delta"<<endl;
				return -1;
		}
		
		cout<<"Please enter Z position where to apply the force"<<endl;
		cin>>zForce;
		cout<<"Please enter the value of force in point z= "<<zForce<<endl;
		cin>>force;
	
		
		cout<<"Please enter full time:"<<endl;
		cin>>fullTime;

		nodeCount=atof(argv[1]);
		deltaTime=atof(argv[2]);
		cout<<deltaTime<<endl;
	//	cout<<argv[2]<<endl;

		gettimeofday(&tv1,&tz);
		
		data=solveEquation(nodeCount);	
		
		gettimeofday(&tv2,&tz);
	
		file.open("string.data",std::ios::out);
		int steps=fullTime/deltaTime;

		cout<<"Steps = "<<steps<<endl;


		if(file.is_open())
		{
				cout<<"File was opened"<<endl;
				for(int i=0;i<steps;i++)
				{
						file<<i<<endl;
						for(int j=0;j<nodeCount;j++)
						{
								file<<j<<"\t";
								file<<data[i][j]<<"\n";
						}
						file<<"\n";
						file<<"\n";

				}


		}
		else
		{
				cout<<"Can't open file"<<endl;
		}
		file.close();

		file.open("string.gp",std::ios::out);
		if(file.is_open())
		{
				cout<<"String.gp was opened"<<endl;
				file<<"set xrange[0:"<<nodeCount<<"]"<<endl;
				file<<"set yrange["<<-20<<":"<<20<<"]"<<endl;
				for(int i=0;i<steps;i++)
				{
						file<<"plot \"string.data\" index "<<i<<" with lines"<<endl;
						file<<"pause(0.1)"<<endl;
				}
		}
		else
		{
				cout<<"Can't open file string.gp"<<endl;
		}
		file.close();
		if(fork()==0)
		{
		system("gnuplot string.gp");
		return(0);
		}

		dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;
		cout<<"Time = "<<dtv.tv_usec<<endl;



}
