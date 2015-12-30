#include "defs.h"

double **make_2d_array(int x_size, int *y_sizes) // Variable column heights, since the number of layers is fixed but nodes/layer is not
{
  ASSERT(x_size > 0);
  ASSERT(y_sizes != NULL);
  
  int i;
  double **new_Array = (double **)malloc(x_size*sizeof(double *));
  for(i = 0; i < x_size; ++i)
  {
    new_Array[i] = (double *)malloc(y_sizes[i]*sizeof(double));
  }
  return new_Array;
}

double ***make_3d_array(int x_size, int *y_sizes) // 3rd dimension is equal to the number of connections backwards
{
  ASSERT(x_size > 0);
  ASSERT(y_sizes != NULL);
  
  int i, j;
  double ***new_Array = (double ***)malloc(x_size*sizeof(double **));
  for(i = 0; i < x_size; ++i)
  {
    new_Array[i] = (double **)malloc(y_sizes[i]*sizeof(double *));
  for(j = 0; j < y_sizes[i] && i > 0; ++j)
    {
      new_Array[i][j] = (double *)malloc(y_sizes[i-1]*sizeof(double));
    }
  }
  return new_Array;
}

int nn_mutate(s_neural_network* nn)
{
  ASSERT(nn != NULL);
  
  int i, j, k;
  for(i = 1; i < nn->num_layers; ++i)
  {
    for(j = 0; j < nn->layer_sizes[i]; ++j)
    {
      for(k = 0; k < nn->layer_sizes[i-1]; ++k)
      {
        nn->weights[i][j][k] += RAND_BETWEEN(-0.1, 0.1);
      }
    }
  }
  return 0;
}

void nn_create(s_neural_network* nn)
{
  ASSERT(nn != NULL);
  
  nn->eye_offset = 1;
  nn->spike_offset = nn->eye_offset + 3*4;
  nn->num_inputs = nn->layer_sizes[0];
  nn->num_outputs = nn->layer_sizes[nn->num_layers-1];
  nn->input = make_2d_array(nn->num_layers, nn->layer_sizes);
  nn->output = make_2d_array(nn->num_layers, nn->layer_sizes);
  nn->weights = make_3d_array(nn->num_layers, nn->layer_sizes);
}

void nn_random_weights(s_neural_network* nn, double min, double max)
{
  ASSERT(nn != NULL);
  
  int i, j, k;
  for(i = 1; i < nn->num_layers; ++i)
  {
    for(j = 0; j < nn->layer_sizes[i]; ++j)
    {
      for(k = 0; k < nn->layer_sizes[i-1]; ++k)
      {
        nn->weights[i][j][k] = RAND_BETWEEN(min, max);
      }
    }
  }
}

void nn_set_inputs(s_bot *bot)
{
  ASSERT(bot != NULL);
  ASSERT(bot->nn != NULL);
  
  bot->nn.input[0][0] = RAND_BETWEEN(0.0, 1.0);

  // eyes
  int e;
  for(e = 0; e < bot->num_eyes; ++e)
  {
    bot->nn.input[0][bot->nn.eye_offset + 4*e+0] = bot->eyes[e].in_strength;
    bot->nn.input[0][bot->nn.eye_offset + 4*e+1] = bot->eyes[e].in_red;
    bot->nn.input[0][bot->nn.eye_offset + 4*e+2] = bot->eyes[e].in_green;
    bot->nn.input[0][bot->nn.eye_offset + 4*e+3] = bot->eyes[e].in_blue;
  }
}

void nn_Basic(s_neural_network* nn)
{
  ASSERT(nn != NULL);
  
  nn->output[nn->num_layers-1][0] = 1;
  nn->output[nn->num_layers-1][1] = 0.5;
  nn->output[nn->num_layers-1][2] = 0;
}

void nn_Intermediate(s_neural_network* nn)
{
  ASSERT(nn != NULL);
  
  if(nn->input[0][nn->eye_offset] > nn->input[0][nn->eye_offset + 4]) // First eye > Second eye
  {
    nn->output[nn->num_layers-1][0] = 0.25;
    nn->output[nn->num_layers-1][1] = 1; // FIXME: 1
    nn->output[nn->num_layers-1][2] = 0;
  }
  else if(nn->input[0][nn->eye_offset] < nn->input[0][nn->eye_offset + 4]) // First eye < Second eye
  {
    nn->output[nn->num_layers-1][0] = 0.25;
    nn->output[nn->num_layers-1][1] = 0; // FIXME: 0
    nn->output[nn->num_layers-1][2] = 0;
  }
  else
  {
    nn->output[nn->num_layers-1][0] = 1.0;
    nn->output[nn->num_layers-1][1] = 0.5;
    nn->output[nn->num_layers-1][2] = 0;
  }
}

void nn_feedforward(s_neural_network* nn)
{
  ASSERT(nn != NULL);
  
  int i, j, k;
  for(i = 0; i < nn->layer_sizes[0]; ++i) // First layer is extracted as it has no weights leading to it
  {
    nn->output[0][i] = nn->input[0][i]; // FIXME: Who knows which this should be
    //nn->output[0][i] = SIGMOID(nn->input[0][i]);
  }

  for(i = 1; i < nn->num_layers; ++i)
  {
    for(j = 0; j < nn->layer_sizes[i]; ++j)
    {
      nn->input[i][j] = 0;
      for(k = 0; k < nn->layer_sizes[i-1]; ++k) // input value is equal to the sum of the connection weights * the output of the connected node
      {
        nn->input[i][j] += nn->output[i-1][k] * nn->weights[i][j][k];
      }
      nn->output[i][j] = SIGMOID(nn->input[i][j]);
    }
  }
}

void update_bots_nns(s_world *world)
{
  ASSERT(world != NULL);
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    if(world->bots[b].dead)
      continue;

    nn_set_inputs(&world->bots[b]);
    //nn_Basic(&world->bots[b].nn);
    //nn_Intermediate(&world->bots[b].nn);
    nn_feedforward(&world->bots[b].nn);
  }
}
