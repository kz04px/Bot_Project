#include "defs.h"

double **make_2d_array(int x_size, int *y_sizes) // Variable column heights, since the number of layers is fixed but nodes/layer is not
{
  int i;
  double **new_Array = (double **) malloc(x_size*sizeof(double *));
  for(i = 0; i < x_size; ++i)
  {
    new_Array[i] = (double *) malloc(y_sizes[i]*sizeof(double));
  }
  return new_Array;
}

double ***make_3d_array(int x_size, int *y_sizes) // 3rd dimension is equal to the number of connections backwards
{
  int i, j;
  double ***new_Array = (double ***) malloc(x_size*sizeof(double **));
  for(i = 0; i < x_size; ++i)
  {
    new_Array[i] = (double **) malloc(y_sizes[i]*sizeof(double *));
  for(j = 0; j < y_sizes[i] && i > 0; ++j)
    {
      new_Array[i][j] = (double *) malloc(y_sizes[i-1]*sizeof(double));
    }
  }
  return new_Array;
}

int nn_mutate(neural_network* network)
{
  int i, j, k;
  for(i = 1; i < network->num_layers; ++i)
  {
    for(j = 0; j < network->layer_sizes[i]; ++j)
    {
      for(k = 0; k < network->layer_sizes[i-1]; ++k)
      {
        network->weights[i][j][k] += RAND_BETWEEN(-0.1, 0.1);
      }
    }
  }
  return 0;
}

void nn_create(neural_network* nn)
{
  nn->eye_offset = 1;
  nn->spike_offset = nn->eye_offset + 3*4;
  nn->num_inputs = nn->layer_sizes[0];
  nn->num_outputs = nn->layer_sizes[nn->num_layers-1];
  nn->input = make_2d_array(nn->num_layers, nn->layer_sizes);
  nn->output = make_2d_array(nn->num_layers, nn->layer_sizes);
  nn->weights = make_3d_array(nn->num_layers, nn->layer_sizes);
}

void nn_random_weights(neural_network* our_nn, double min, double max)
{
  int i, j, k;
  for(i = 1; i < our_nn->num_layers; ++i)
  {
    for(j = 0; j < our_nn->layer_sizes[i]; ++j)
    {
      for(k = 0; k < our_nn->layer_sizes[i-1]; ++k)
      {
        our_nn->weights[i][j][k] = RAND_BETWEEN(min, max);
      }
    }
  }
}

void nn_set_inputs(bot* our_bot)
{
  our_bot->nn.input[0][0] = RAND_BETWEEN(0.0, 1.0);

  // eyes
  int e;
  for(e = 0; e < our_bot->num_eyes; ++e)
  {
    our_bot->nn.input[0][our_bot->nn.eye_offset + 4*e+0] = our_bot->eyes[e].in_strength;
    our_bot->nn.input[0][our_bot->nn.eye_offset + 4*e+1] = our_bot->eyes[e].in_red;
    our_bot->nn.input[0][our_bot->nn.eye_offset + 4*e+2] = our_bot->eyes[e].in_green;
    our_bot->nn.input[0][our_bot->nn.eye_offset + 4*e+3] = our_bot->eyes[e].in_blue;
  }
}

void nn_Basic(neural_network* our_nn)
{
  our_nn->output[our_nn->num_layers-1][0] = 1;
  our_nn->output[our_nn->num_layers-1][1] = 0.5;
  our_nn->output[our_nn->num_layers-1][2] = 0;
}

void nn_Intermediate(neural_network* our_nn)
{
  if(our_nn->input[0][our_nn->eye_offset] > our_nn->input[0][our_nn->eye_offset + 4]) // First eye > Second eye
  {
    our_nn->output[our_nn->num_layers-1][0] = 0.25;
    our_nn->output[our_nn->num_layers-1][1] = 1; // FIXME: 1
    our_nn->output[our_nn->num_layers-1][2] = 0;
  }
  else if(our_nn->input[0][our_nn->eye_offset] < our_nn->input[0][our_nn->eye_offset + 4]) // First eye < Second eye
  {
    our_nn->output[our_nn->num_layers-1][0] = 0.25;
    our_nn->output[our_nn->num_layers-1][1] = 0; // FIXME: 0
    our_nn->output[our_nn->num_layers-1][2] = 0;
  }
  else
  {
    our_nn->output[our_nn->num_layers-1][0] = 1.0;
    our_nn->output[our_nn->num_layers-1][1] = 0.5;
    our_nn->output[our_nn->num_layers-1][2] = 0;
  }
}

void nn_feedforward(neural_network* our_nn)
{
  int i, j, k;
  for(i = 0; i < our_nn->layer_sizes[0]; ++i) // First layer is extracted as it has no weights leading to it
  {
    our_nn->output[0][i] = our_nn->input[0][i]; // FIXME: Who knows which this should be
    //our_nn->output[0][i] = SIGMOID(our_nn->input[0][i]);
  }

  for(i = 1; i < our_nn->num_layers; ++i)
  {
    for(j = 0; j < our_nn->layer_sizes[i]; ++j)
    {
      our_nn->input[i][j] = 0;
      for(k = 0; k < our_nn->layer_sizes[i-1]; ++k) // input value is equal to the sum of the connection weights * the output of the connected node
      {
        our_nn->input[i][j] += our_nn->output[i-1][k] * our_nn->weights[i][j][k];
      }
      our_nn->output[i][j] = SIGMOID(our_nn->input[i][j]);
    }
  }
}

void update_bots_nns(world* our_world)
{
  int b;
  for(b = 0; b < our_world->num_bots; ++b)
  {
    if(our_world->bots[b].dead)
      continue;

    nn_set_inputs(&our_world->bots[b]);
    //nn_Basic(&our_world->bots[b].nn);
    //nn_Intermediate(&our_world->bots[b].nn);
    nn_feedforward(&our_world->bots[b].nn);
  }
}
