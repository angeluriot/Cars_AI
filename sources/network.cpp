#include "utils.h"

Network::Network()
{
	layers.clear();
}

Network::Network(const Network& network)
{
	layers = network.layers;
}

Network::Network(std::vector<int> layers)
{
	this->layers.clear();

	for (int i = 0; i < layers.size(); i++)
	{
		this->layers.push_back({});

		for (int j = 0; j < layers[i]; j++)
		{
			if (i == 0)
				this->layers[i].push_back(Neuron());

			else
				this->layers[i].push_back(Neuron(i, this->layers[i - 1].size()));
		}
	}
}

void Network::operator=(const Network& network)
{
	layers = network.layers;
}

void Network::update_outputs(const std::vector<double>& inputs)
{
	for (int i = 0; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			if (i == 0)
				layers[i][j].output = inputs[j];

			else
				layers[i][j].update_output(*this);
		}
	}
}

double Network::get_max_output() const
{
	double max_output = 0.;

	for (int i = 0; i < layers.back().size(); i++)
	{
		if (max_output < layers.back()[i].output)
			max_output = layers.back()[i].output;
	}

	return max_output;
}

bool Network::is_positive(int index) const
{
	return (layers.back()[index].output >= 0.5);
}

std::vector<double> Network::get_outputs() const
{
	std::vector<double> v;

	for (auto& out : layers.back())
		v.push_back(out.output);

	return v;
}

double Network::get_output(int index) const
{
	return layers.back()[index].output;
}

void Network::mutate(const double& score)
{
	double level = 1. / (double)std::pow(score * 50. + 1., 2);

	for (int i = 1; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
			layers[i][j].mutate(level);
	}
}