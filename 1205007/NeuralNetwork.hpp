const double a = 1;
double learning_rate = 0.0014;

vector<Matrix> value, Y, delta;
//value -> saves the calculated value
//Y -> saves logistic_function(V)
//delta ->
///hierarchy: layer -> dataset -> neuron => value

double logistic_function(double x){
	return 1/(1 + exp(-a*x));
}

double differentiate(double x){
	double val = logistic_function(x);
	return a * val * (1 - val);
}

Vector getPattern(int index){
	vector<double> ret(numberOfClasses, 0.0);
	ret[index] = 1.0;
	return Vector(ret);
}


struct Neuron{
	Vector W;
	int id, layerID;

	Neuron(){}
	void set(int id, int lid, int siz){
		this->id = id;
		layerID = lid;
		W.setRandom(siz);
	}

	void updateW(Vector deltaW) {W = W + deltaW;}
	double getSize(){return W.getDimension();}

	double calc(Vector input, int dataIndex){
		double v = W*input, ret = logistic_function(v);
		
		value[layerID][dataIndex][id] = v;
		Y[layerID][dataIndex][id] = ret;
		return ret;	
	}


};

struct Layer{
	vector<Neuron> neurons;
	int id;

	Layer(){}
	void set(int id, int numberOfNeurons, int dimension){
		this->id = id;
		neurons.resize(numberOfNeurons);
		for(int i=0; i<numberOfNeurons; i++) neurons[i].set(i, id, dimension);
	}

	int getSize(){return neurons.size();}
	int getDimension(){return neurons[0].getSize();}

	Vector calc(Vector Y, int dataIndex){
		vector<double> ret(neurons.size());
		for(int i=0; i<ret.size(); i++){
			ret[i] = neurons[i].calc(Y, dataIndex);
		}
		return Vector(ret);
	}

};

struct NeuralNetwork{
	vector<Layer> layers;

	NeuralNetwork(){}
	void set(int numberOfLayers, vector<int> layerSize){
		layers.resize(numberOfLayers);
		for(int i=0; i<numberOfLayers;i++){
			int dimension = (i == 0)? numberOfFeatures + 1: layerSize[i-1] + 1;
			layers[i].set(i, layerSize[i], dimension);
		}
	}

	double getWeight(int layerNO, int neuronNO, int wIndex){
		return layers[layerNO].neurons[neuronNO].W.vec[wIndex];
	}

	Vector forward(Vector, int, int);
};

Vector NeuralNetwork::forward(Vector X, int dataIndex, int depth)
{		
	//cout << "input size: " << X.getDimension() << endl;
	//vector<double> work = X.vec;
	//work.push_back(1);
	X = X.insertOne();

	Vector nx = layers[depth].calc(X, dataIndex);
	//cout << "output: " << nx.getDimension() << endl;

	if(depth == layers.size()-1) return nx;
	return forward(nx, dataIndex, depth+1);
}

NeuralNetwork NN;

void backward()
{
	///calculate delta for last layer
	for(int i=0; i<numberOfSamples; i++){
		for(int j=0; j<NN.layers.back().getSize(); j++){
			Vector orig = getPattern(classIndex[dataClass[i]]);
			delta[numberOfLayers-1][i][j] = differentiate(value[numberOfLayers-1][i][j]) * (Y[numberOfLayers-1][i][j] - orig.vec[j]);
		}
	}

	//display<double> (delta[numberOfLayers-1][0]);

	for(int r=numberOfLayers-2; r>=0; r--){
		for(int i=0; i<numberOfSamples; i++){
			for(int j=0; j<NN.layers[r].getSize(); j++){

				double sum = 0;
				for(int k=0; k<NN.layers[r+1].getSize(); k++){
					sum += delta[r+1][i][k] * NN.getWeight(r+1, k, j);
				}
				sum *= differentiate(value[r][i][j]);
				delta[r][i][j] = sum;

			}
		}
	}

	//display<double>(delta[0][0]);

}

void updateWeight()
{
	Vector deltaW;

	for(int r=0; r<numberOfLayers; r++){
		for(int j=0; j<NN.layers[r].getSize(); j++){

			int dimension = NN.layers[r].getDimension();
			deltaW.setSize(dimension, 0);

			for(int i=0; i<numberOfSamples; i++){
				Vector y;
				if(r == 0) y.set(data[i]);
				else y.set(Y[r-1][i]);

				y = y.insertOne();

				//cout << "y size: " << y.getDimension() <<endl;
				//cout << "deltaW size: " << deltaW.getDimension() << endl;

				y = y * delta[r][i][j];
				deltaW = deltaW - y;
			}

			deltaW = deltaW * learning_rate;
			NN.layers[r].neurons[j].updateW(deltaW);

		}
	}

}


///initialize the data structures
void init()
{
	value.resize(numberOfLayers);
	Y.resize(numberOfLayers);
	delta.resize(numberOfLayers);

	for(int i=0; i<numberOfLayers; i++){
		value[i].resize(numberOfSamples);
		Y[i].resize(numberOfSamples);
		delta[i].resize(numberOfSamples);
	}

	for(int i=0; i<numberOfLayers; i++){
		for(int j=0; j<numberOfSamples; j++){
			value[i][j].resize(layerSize[i]);
			Y[i][j].resize(layerSize[i]);
			delta[i][j].resize(layerSize[i]);
		}
	}

	NN.set(numberOfLayers, layerSize);
}