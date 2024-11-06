#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>

size_t size = 2;

void copyDoubleArray(double* dest, double* src)
{
    for(int i = 0;i<size;i++)
    {
        dest[i] = src[i];
    }
}

double calculateDistance(double* first,double* second)
{
    float result = 0;
    for(int i = 0;i<size;i++)
    {
        result += pow((first[i]-second[i]),2);
    }
    result = sqrt(result);
    return result;
}

double function(double* x)
{
    return x[0] * (x[0]+10) + x[1] *x[1] + 10; //write func here
}

void derivativeOfFunc(double (*f)(double*), double* x, double* result)
{
    float h = (0.0001); // will change

    double xPlusH[size];
    double xMinusH[size];

    for (int j = 0; j < size; j++)
    {
        xPlusH[j] = x[j];
        xMinusH[j] = x[j];
    }

    for (int i = 0; i < size; i++)
    {
        xPlusH[i] += h;
        xMinusH[i] -= h;

        result[i] = ((*f)(xPlusH) - (*f)(xMinusH)) / (2 * h);

        xPlusH[i] -= h; //restore
        xMinusH[i] += h;
    }
}

//optimization functions

struct Data {
    double timeTaken;
    int iterations;
    double parameters[];
};

struct Data* createData()
{
    struct Data* data = (struct Data*)malloc(sizeof(struct Data) + size * sizeof(double));
    data->timeTaken = 0.0;
    data->iterations = 0;
    return data;
}

void freeData(struct Data* data)
{
    free(data);
}


struct Data* gradientDescent(double (*f)(double*),double* startingPoint,int maxIteration,float precision,float stepSize)
{
    clock_t startTime = clock();
    double xNew[size];
    double xOld[size];
    copyDoubleArray(xOld,startingPoint);
    int currentIteration = 0;
    while(currentIteration<maxIteration)
    {
        currentIteration++;
        double result[size];
        derivativeOfFunc(f,xOld,result);
        for(int i = 0;i<size;i++)
        {
            xNew[i] = xOld[i] - result[i]*stepSize;
        } 
        if(calculateDistance(xNew,xOld)<precision)
        {
            break;
        }
        copyDoubleArray(xOld,xNew);
    }
    struct Data* data;
    data = createData();
    copyDoubleArray(data->parameters,xNew);
    data->iterations = currentIteration;
    data->timeTaken = (double)(clock() - startTime) / CLOCKS_PER_SEC * 1000;
    return data;
}



int main()
{
    double at[] = { 3,2 };
    printf("%f\n", function(at));
    double result[size];
    derivativeOfFunc(function, at, result);
    for (int i = 0; i < size; i++)
    {
        printf("%f\n", result[i]);
    }
    struct Data* data;
    double startingPoint[] = {120,5000}; 
    data = gradientDescent(function,startingPoint,300,0.0000001,0.1);
    printf("%f\n", data->parameters[0]);
    printf("%i\n", data->iterations);
    printf("%f\n", function(data->parameters));
    printf("%f\n", data->timeTaken);
    freeData(data);
    return 0;
}