#include<iostream>
#include<cstdlib>
#include<fstream>
#include<sys/time.h>
#include<pthread.h>
#include<unistd.h>

using std::cout;
using std::endl;
using std::cin;

float  a=0.7;
float  deltaX=1;
//int f=0;
float deltaTime=0.0;
float fullTime=0.0;
float** data;

int zForce=1;
int force=1;

float* timeMass;
pthread_barrier_t bp;





float  Zij(int prevZ,int nextZ,int currentZ,int prevT,int currentX,int currentTime)
{
		float part1=0.0;
		if(currentTime==2)
		{
				if(currentX==zForce)
				part1 =a *a*(nextZ-2*currentZ+prevZ)+force*deltaTime*deltaTime;
				else
				part1 =a *a*(nextZ-2*currentZ+prevZ);
		}
		else
		part1 =a *a*(nextZ-2*currentZ+prevZ);

		float znam=deltaX*deltaX;
		float result=part1/znam+2*currentZ-prevT;
		return result;

}

struct solve_args
{
		int nodeCount;

		int maxThreadZ;
		int minThreadZ;
		

		int threadId;
};





void * solveEquation(void* arg_p)
{


		solve_args args=*((solve_args*)arg_p);
/*
		cout<<"Thread number  "<<args.threadId<cout<<endl;
		cout<<"maxThreadZ  "<<args.maxThreadZ<cout<<endl;
		cout<<"minThreadZ  "<<args.minThreadZ<cout<<endl;
		cout<<"node count   "<<args.nodeCount<cout<<endl;
*/
		int minZ=args.minThreadZ;
		int maxZ=args.maxThreadZ;
		int nodeCount=args.nodeCount;
		
		struct timeval tv1,tv2,dtv;
		struct timezone tz;


		int i=0,j=0;


		int steps=0;
		int minJ=0;
		steps=fullTime/deltaTime;
		cout<<"steps= "<<steps<<endl;


		if(args.threadId==0)
				minJ=minZ+1;
		else
				minJ=minZ;

		cout<<"minZ= "<<j<<endl;

		cout<<"ready"<<endl;

		gettimeofday(&tv1,&tz);

		for(i=2;i<steps;i++)
		{
				if(minZ==0)
						data[i][0]=0;

				if(maxZ==nodeCount-1)
						data[i][nodeCount-1]=0;


				for(j=minJ;j<=maxZ;j++)
				{
						data[i][j]=Zij(data[i-1][j-1],data[i-1][j+1],data[i-1][j],data[i-2][j],j,i);

				}

				pthread_barrier_wait(&bp);
		}
		gettimeofday(&tv2,&tz);
		dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;
		timeMass[args.threadId]=dtv.tv_usec;
		pthread_exit(0);
}


int main(int argc, char** argv)
{
		struct solve_args args;

        
		pthread_attr_t pattr;
        
		
		int ret;

		if(argc<4)
		{
				cout<<"Usage: ./prog_name thread_count node_count time_delta"<<endl;
				return -1;
		}
		
		cout<<"Please enter Z position where to apply the force"<<endl;
		cin>>zForce;
		cout<<"Please enter the value of force in point z= "<<zForce<<endl;
		cin>>force;
		
		cout<<"Please enter full time:"<<endl;
		cin>>fullTime;
		

		

		int nodeCount=atof(argv[2]);
		args.nodeCount=nodeCount;
        deltaTime=atof(argv[3]);
		cout<<"NODE COUNT= "<<nodeCount<<endl;
		
		int steps=fullTime/deltaTime;
		
		data = new float*[steps];
		for(int i=0;i<steps;i++)
				data[i]=new float[nodeCount];

		for(int j=0;j<2;j++)
		for(int i=0;i<nodeCount;i++)
				data[j][i]=0;
        

		int threadCount=atoi(argv[1]);
		
        timeMass=new float[threadCount];


		int delta;
		int deltaLast;
		bool UnEqual=false;

		if(nodeCount%threadCount==0)
				delta=(int)nodeCount/threadCount;
		else
		{
				UnEqual=true;
				delta=(int)nodeCount/threadCount;
				int closest=threadCount*delta;
				if(closest-nodeCount>0)
                     deltaLast=closest-nodeCount;
				else
				    deltaLast=nodeCount-closest;

				
		}



        pthread_barrier_init(&bp,NULL,threadCount);

		pthread_t threads[threadCount];

		pthread_attr_init(&pattr);
		pthread_attr_setscope(&pattr,PTHREAD_SCOPE_SYSTEM);
		pthread_attr_setdetachstate(&pattr,PTHREAD_CREATE_JOINABLE);
		
		for(int i=0;i<threadCount;i++)
		{
				args.threadId=i;
				if(!UnEqual)
				{
				args.maxThreadZ=(i+1)*delta-1;
				args.minThreadZ=i*delta;
				}
				else
				{
						args.maxThreadZ=(i+1)*delta-1;
						if(i==threadCount-1)
								args.maxThreadZ=deltaLast+(i+1)*delta-1;
				                
						args.minThreadZ=i*delta;
				}
				if(ret=pthread_create(&threads[i],&pattr,solveEquation,(solve_args*)&args))
						perror("pthread_create");
				sleep(1);
		}
		

		pthread_join(threads[threadCount-1],NULL);
		 
		pthread_barrier_destroy(&bp);

		std::ofstream file;

	
		
		
		
		//data=solveEquation(nodeCount,zeroZ,lengthZ);	
		
		

		
		
		file.open("tstring.data",std::ios::out);

/*
        for(int i=0;i<steps;i++)
		{
				for(int j=0;j<nodeCount;j++)
						cout<<data[i][j]<<"\t";

		cout<<endl;
		}
	
*/
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

		file.open("tstring.gp",std::ios::out);
		if(file.is_open())
		{
				cout<<"String.gp was opened"<<endl;
				file<<"set xrange[0:"<<nodeCount<<"]"<<endl;
				file<<"set yrange["<<-20<<":"<<20<<"]"<<endl;
				for(int i=0;i<steps;i++)
				{
						file<<"plot \"tstring.data\" index "<<i<<" with lines"<<endl;
						file<<"pause(0.1)"<<endl;
				}
		}
		else
		{
				cout<<"Can't open file tstring.gp"<<endl;
		}
		file.close();
		if(fork()==0)
		{
				system("gnuplot tstring.gp");
				return 0;
		}
		




		float maxTime=0.0;

		 for(int i=0;i<threadCount;i++)
				 maxTime+=timeMass[i];

		cout<<"Time= "<<maxTime<<endl;

		pthread_exit(0);

}
