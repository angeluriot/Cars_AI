#ifndef NEURON_H
#define NEURON_H
#include <vector>

class Network;

class Neuron
{

public:

	int layer;
	std::vector<double> weights;
	double add_value;
	double output;

	Neuron();
	Neuron(const Neuron& neuron);
	Neuron(int layer_index, int previous_layer_size);

	void operator=(const Neuron& neuron);

	void update_output(const Network& network);
	void mutate(const double& level);
};

#endif