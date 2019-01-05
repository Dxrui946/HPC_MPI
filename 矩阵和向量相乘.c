#include<stdio.h>                                                           
#include<mpi.h>
#include<stdlib.h>

void Read_matrix(double local_a[],int local_m,int m,int n,char matrix_name[],int my_rank,MPI_Comm comm)         /*��ȡ���ַ�����*/
{

        double* a;
        if(my_rank==0)
        {
            a=malloc((m*n)*sizeof(double));
            printf("������%d��%d�еķ��� %s\n",m,n,matrix_name);
            for(int i=0;i<m;i++)
            {
               for(int j=0;j<n;j++)
                    scanf("%lf",&a[i*n+j]);
            }

            MPI_Scatter(a,local_m*n,MPI_DOUBLE,local_a,local_m*n,MPI_DOUBLE,    0,comm);
            free(a);
        }
        else{

            MPI_Scatter(a,local_m*n,MPI_DOUBLE,local_a,local_m*n,MPI_DOUBLE,    0,comm);
        }                   /*����Ϊ0 �ַ����� �������� ��������*/
}

void Print_matrix(double local_b[],int local_m,int m,int n,char title[],int my_rank,MPI_Comm comm)/*���������з����ۼ���0�Ž���*/
{
    double* b;
    if(my_rank==0)
    {   
        b=malloc(m*sizeof(double));
        MPI_Gather(local_b,local_m,MPI_DOUBLE,b,local_m,MPI_DOUBLE,0,comm);
        printf("%s\n",title);
        for(int i=0;i<m;i++)
           printf("%lf ",b[i]);
        printf("\n");
        
        free(b);
    }
    else
    {
    MPI_Gather(local_b,local_m,MPI_DOUBLE,b,local_m,MPI_DOUBLE,0,comm);
    }
}
void matrix_multi(double local_x[],double x[],double local_y[],int local_m,i    nt n)
{
    int i,j;
    for(i=0;i<local_m;i++)
    {
        local_y[i]=0;
        for(j=0;j<n;j++)
            local_y[i]+=local_x[i*n+j]*x[j];
    }
}
void matrix_multi(double local_x[],double x[],double local_y[],int local_m,i    nt n)
{
    int i,j;
    for(i=0;i<local_m;i++)
    {
        local_y[i]=0;
        for(j=0;j<n;j++)
            local_y[i]+=local_x[i*n+j]*x[j];
    }
}
int main(void)
{
    int n,m,local_m,my_rank,comm_sz;
    double *local_a,*local_b,*x;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if(my_rank==0)
    {
    printf("����������ά��: \n");
    scanf("%d %d",&m,&n);   
    }
    MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
     
     
     local_m= m/comm_sz;     /*ÿ���������õ�������*/
     local_a=(double*)malloc(local_m*n*sizeof(double));/*ÿ�����̷���õ��ľ����С*/
     local_b=(double*)malloc(local_m*sizeof(double));
 
     x=malloc(n*sizeof(double));
 
     if(my_rank == 0)
     {
         printf("��������:\n");
         for(int i=0;i<n;i++)
         {
             scanf("%lf",&x[i]);
         }
     }

