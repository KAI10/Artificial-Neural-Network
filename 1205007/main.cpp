/*
 * main.cpp
 * 
 * Created by: Ashik <ashik@KAI10>
 * Created on: Sun, 02 Apr 2017
 */


#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define mem(list, val) memset(list, (val), sizeof(list))
#define pb push_back

typedef vector<vector<double>> Matrix;

Matrix data;
vector<string> dataClass, className;
map<string, int> classIndex; 

int numberOfClasses, numberOfFeatures, numberOfSamples;
int numberOfLayers;
vector<int> layerSize;

#include "objects.hpp"
#include "NeuralNetwork.hpp"
#include "utilities.hpp"


int main()
{
    //freopen("test.txt", "r", stdin);

	readData("trainNN.txt", data, false);
	readParameters("layer_configuration.txt");

	normalize(data);

	//checkDataRead();

	init();

	//checkNetworkStatus();double error = 0;
	for(int iteration = 0; iteration < 100; iteration++){
		double error = 0;
		for(int i=0; i<numberOfSamples; i++){
			Vector prediction = NN.forward(data[i], i, 0);

			Vector orig = getPattern(classIndex[dataClass[i]]);

			//prediction.show();
			//orig.show();

			//cout << endl;

			error += (prediction - orig).norm();
		}

		error *= 0.5;
		printf("%.7f\n", error);

		if(error < 60) break;
		//cout << error*0.5 << endl;
		backward();

		//getchar();

		//cout << value[0][0][0] << endl;
		//cout << Y[0][0][0] << endl;

		updateWeight();
	}

	/*
	int correct = 0;
	for(int i=0; i<numberOfSamples; i++){
		Vector prediction = NN.forward(data[i], i, 0);
		Vector orig = getPattern(classIndex[dataClass[i]]);

		if(orig.vec[prediction.maxIndex()] == 1.0) correct++; 
	}

	cout << correct << endl;
	*/

	//Matrix testData;
	//readData("testNN.txt", testData, true);

	//normalize(testData);

	vector<int> correctlyDeclared(numberOfClasses, 0);
	vector<int> declared(numberOfClasses, 0);
	vector<int> actual(numberOfClasses, 0);

	FILE *fout = fopen("output.txt", "w");

	int correct = 0;
	for(int i=0; i<numberOfSamples; i++){
		//Vector prediction = NN.forward(testData[i], i, 0);

		for(int j=0; j<data[i].size(); j++) fprintf(fout, "%lf\t", data[i][j]);
		fprintf(fout, "%s\t", dataClass[i].c_str());
		
		Vector prediction = NN.forward(data[i], i, 0);
		Vector orig = getPattern(classIndex[dataClass[i]]);

		declared[prediction.maxIndex()]++;
		actual[classIndex[dataClass[i]]]++;
		if(orig.vec[prediction.maxIndex()] == 1.0){
			correct++;
			correctlyDeclared[prediction.maxIndex()]++;
		}

		fprintf(fout, "%s\n", className[prediction.maxIndex()].c_str());
		//prediction.show();
		//cout << prediction.maxIndex() << endl;
	}

	fprintf(fout, "accuracy: %lf\n", 100.0*correct/numberOfSamples);
	
	fprintf(fout, "Precision:\n");
	for(int i=0; i<numberOfClasses; i++){
		fprintf(fout, "Class %s: %lf\n", className[i].c_str(), 100.0*correctlyDeclared[i]/declared[i]);
	}

	fprintf(fout, "Recall:\n");
	for(int i=0; i<numberOfClasses; i++){
		fprintf(fout, "Class %s: %lf\n", className[i].c_str(), 100.0*correctlyDeclared[i]/actual[i]);
	}
	

	fclose(fout);
	



    return 0;
}
