#include <stdio.h>                                                          
#include <mpi.h>
 
void Build_mpi_type(double* a,double* b,int* n,MPI_Datatype* AA)
 {
     int array_of_blocklengths[3]={1,1,1};
     MPI_Datatype array_of_types[3]={MPI_DOUBLE,MPI_DOUBLE,MPI_INT};
     MPI_Aint a_addr,b_addr,n_addr;
     MPI_Aint array_of_displacement[3]={0};
     MPI_Get_address(a,&a_addr);
     MPI_Get_address(b,&b_addr);
     MPI_Get_address(n,&n_addr);
     array_of_displacement[1]=b_addr-a_addr;
     array_of_displacement[2]=n_addr-a_addr;
     MPI_Type_create_struct(3,array_of_blocklengths,array_of_displacement,arr    ay_of_types,AA);
     MPI_Type_commit(AA);
 }

void Get_input(int size,int rank,double* a,double* b,int* n)
 {
         MPI_Datatype AA;
         Build_mpi_type(a,b,n,&AA);
         if(rank==0){
             printf("请输入 a,b,n:\n");
             scanf("%lf%lf%d",a,b,n);
             }
         MPI_Bcast(a,1,AA,0,MPI_COMM_WORLD);
         MPI_Type_free(&AA);
 }

double f(double a)
 {
     return a*a;
 }
 
 double Trap(double local_a,double local_b,int local_n,double h)
 {
       double estimate, x;
 
       estimate = (f(local_a) + f(local_b))/2.0;
       for (int i = 1; i < local_n; i++)
       {
              x = local_a + i*h;
              estimate += f(x);
         }
       estimate = estimate*h;
       return estimate;
 }int main(int argc,char *argv[])
 {
     int my_rank,comm_sz;
     int n = 1024, local_n;
     double a=0.0,b=4.0,h,local_a,local_b;
     double local_s,total_s;
 
     MPI_Init(&argc, &argv);
     MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
     MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
 
     Get_input(comm_sz,my_rank,&a,&b,&n);
     h=(b-a)/n;  //每一个区间长度
     local_n = n/comm_sz;    //每个processor需要处理的梯形的数目
     local_a = a + my_rank*local_n*h;
     local_b = local_a + local_n*h;
     local_s = Trap(local_a, local_b, local_n, h);
 
     MPI_Reduce(&local_s,&total_s,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
 
     if(my_rank == 0)
     {
         printf("梯形面积为:\n");
         printf("%f\n",total_s);
     }
     MPI_Finalize();
     return 0;                                                               
 }
