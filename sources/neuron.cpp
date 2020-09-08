#include "utils.h"

Neuron::Neuron()
{
	layer = 0;
	weights.clear();
	add_value = 0.;
	output = 0.;
}

Neuron::Neuron(const Neuron& neuron)
{
	layer = neuron.layer;
	weights = neuron.weights;
	add_value = neuron.add_value;
	output = neuron.output;
}

Neuron::Neuron(int layer_index, int previous_layer_size)
{
	layer = layer_index;
	weights.clear();
	add_value = random_factor();
	output = 0.;

	for (int i = 0; i < previous_layer_size; i++)
		weights.push_back(random_factor());
}

void Neuron::operator=(const Neuron& neuron)
{
	layer = neuron.layer;
	weights = neuron.weights;
	add_value = neuron.add_value;
	output = neuron.output;
}

void Neuron::update_output(const Network& network)
{
	output = 0.;

	for (int i = 0; i < network.layers[layer - 1].size(); i++)
		output += network.layers[layer - 1][i].output * weights[i];

	output += add_value;
	output = sigmoid(output);
}

void Neuron::mutate(const double& level)
{
	for (int i = 0; i < weights.size(); i++)
	{
		weights[i] += level * random_factor();

		if (weights[i] > 1.)
			weights[i] = 1.;

		if (weights[i] < -1.)
			weights[i] = -1.;
	}

	add_value += level * random_factor();
}
