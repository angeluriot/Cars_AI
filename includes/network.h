#ifndef NETWORK_H
#define NETWORK_H
#include "neuron.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Network
{

public:

	std::vector<std::vector<Neuron>> layers;

	Network();
	Network(const Network& network);
	Network(std::vector<int> layers);

	void operator=(const Network& network);

	void update_outputs(const std::vector<double>& inputs);
	double get_max_output() const;
	bool is_positive(int index) const;
	std::vector<double> get_outputs() const;
	double get_output(int index) const;
	void mutate(const double& score);
};

#endif
