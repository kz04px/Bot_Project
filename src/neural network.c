#include "defs.h"

double **Make_2D_Array(int X_Size, int *Y_Sizes) // Variable column heights, since the number of layers is fixed but nodes/layer is not
{
int i;
double **New_Array = (double **) malloc(X_Size*sizeof(double *));
for(i = 0; i < X_Size; ++i)
  {
  New_Array[i] = (double *) malloc(Y_Sizes[i]*sizeof(double));
  }
return New_Array;
}

double ***Make_3D_Array(int X_Size, int *Y_Sizes) // 3rd dimension is equal to the number of connections backwards
{
int i, j;
double ***New_Array = (double ***) malloc(X_Size*sizeof(double **));
for(i = 0; i < X_Size; ++i)
  {
  New_Array[i] = (double **) malloc(Y_Sizes[i]*sizeof(double *));
  for(j = 0; j < Y_Sizes[i] && i > 0; ++j)
    {
    New_Array[i][j] = (double *) malloc(Y_Sizes[i-1]*sizeof(double));
    }
  }
return New_Array;
}

int NN_Mutate(neural_network* Network)
{
int i, j, k;
for(i = 1; i < Network->Num_Layers; ++i)
  {
  for(j = 0; j < Network->Layer_Sizes[i]; ++j)
    {
    for(k = 0; k < Network->Layer_Sizes[i-1]; ++k)
      {
      Network->Weights[i][j][k] += RAND_BETWEEN(-0.1, 0.1);
      }
    }
  }
return 0;
}

void NN_Create(neural_network* NN)
{
  NN->Eye_Offset = 1;
  NN->Spike_Offset = NN->Eye_Offset + 3*4;
  NN->Num_Inputs = NN->Layer_Sizes[0];
  NN->Num_Outputs = NN->Layer_Sizes[NN->Num_Layers-1];
  NN->Input = Make_2D_Array(NN->Num_Layers, NN->Layer_Sizes);
  NN->Output = Make_2D_Array(NN->Num_Layers, NN->Layer_Sizes);
  NN->Weights = Make_3D_Array(NN->Num_Layers, NN->Layer_Sizes);
}

void NN_Random_Weights(neural_network* Our_NN, double Min, double Max)
{
  int i, j, k;
  for(i = 1; i < Our_NN->Num_Layers; ++i)
  {
    for(j = 0; j < Our_NN->Layer_Sizes[i]; ++j)
    {
      for(k = 0; k < Our_NN->Layer_Sizes[i-1]; ++k)
      {
        Our_NN->Weights[i][j][k] = RAND_BETWEEN(Min, Max);
      }
    }
  }
}

void NN_Set_Inputs(bot* Our_Bot)
{
  Our_Bot->NN.Input[0][0] = RAND_BETWEEN(0.0, 1.0);

  // Eyes
  int e;
  for(e = 0; e < Our_Bot->Num_Eyes; ++e)
  {
    Our_Bot->NN.Input[0][Our_Bot->NN.Eye_Offset + 4*e+0] = Our_Bot->Eyes[e].In_Strength;
    Our_Bot->NN.Input[0][Our_Bot->NN.Eye_Offset + 4*e+1] = Our_Bot->Eyes[e].In_Red;
    Our_Bot->NN.Input[0][Our_Bot->NN.Eye_Offset + 4*e+2] = Our_Bot->Eyes[e].In_Green;
    Our_Bot->NN.Input[0][Our_Bot->NN.Eye_Offset + 4*e+3] = Our_Bot->Eyes[e].In_Blue;
  }
}

void NN_Basic(neural_network* Our_NN)
{
  Our_NN->Output[Our_NN->Num_Layers-1][0] = 1;
  Our_NN->Output[Our_NN->Num_Layers-1][1] = 0.5;
  Our_NN->Output[Our_NN->Num_Layers-1][2] = 0;
}

void NN_Intermediate(neural_network* Our_NN)
{
  if(Our_NN->Input[0][Our_NN->Eye_Offset] > Our_NN->Input[0][Our_NN->Eye_Offset + 4]) // First eye > Second eye
    {
    Our_NN->Output[Our_NN->Num_Layers-1][0] = 0.25;
    Our_NN->Output[Our_NN->Num_Layers-1][1] = 1; // FIXME: 1
    Our_NN->Output[Our_NN->Num_Layers-1][2] = 0;
    }
  else if(Our_NN->Input[0][Our_NN->Eye_Offset] < Our_NN->Input[0][Our_NN->Eye_Offset + 4]) // First eye < Second eye
    {
    Our_NN->Output[Our_NN->Num_Layers-1][0] = 0.25;
    Our_NN->Output[Our_NN->Num_Layers-1][1] = 0; // FIXME: 0
    Our_NN->Output[Our_NN->Num_Layers-1][2] = 0;
    }
  else
    {
    Our_NN->Output[Our_NN->Num_Layers-1][0] = 1.0;
    Our_NN->Output[Our_NN->Num_Layers-1][1] = 0.5;
    Our_NN->Output[Our_NN->Num_Layers-1][2] = 0;
    }
}

void NN_Feedforward(neural_network* Our_NN)
{
  int i, j, k;
  for(i = 0; i < Our_NN->Layer_Sizes[0]; ++i) // First layer is extracted as it has no weights leading to it
  {
    Our_NN->Output[0][i] = Our_NN->Input[0][i]; // FIXME: Who knows which this should be
    //Our_NN->Output[0][i] = SIGMOID(Our_NN->Input[0][i]);
  }

  for(i = 1; i < Our_NN->Num_Layers; ++i)
  {
    for(j = 0; j < Our_NN->Layer_Sizes[i]; ++j)
    {
      Our_NN->Input[i][j] = 0;
      for(k = 0; k < Our_NN->Layer_Sizes[i-1]; ++k) // Input value is equal to the sum of the connection weights * the output of the connected node
      {
        Our_NN->Input[i][j] += Our_NN->Output[i-1][k] * Our_NN->Weights[i][j][k];
      }
      Our_NN->Output[i][j] = SIGMOID(Our_NN->Input[i][j]);
    }
  }
}

void Update_Bots_NNs(world* Our_World)
{
  int b;
  for(b = 0; b < Our_World->Num_Bots; ++b)
  {
    if(Our_World->Bots[b].Dead == TRUE)
      continue;

    NN_Set_Inputs(&Our_World->Bots[b]);
    //NN_Basic(&Our_World->Bots[b].NN);
    //NN_Intermediate(&Our_World->Bots[b].NN);
    NN_Feedforward(&Our_World->Bots[b].NN);
  }
}
