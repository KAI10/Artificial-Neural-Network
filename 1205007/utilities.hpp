void readData(string fileName, Matrix &data, bool testData)
{
    ifstream fin;
    fin.open(fileName);

    string line, token;
    istringstream ss(line);

    vector<string> strTemp;
    vector<double> temp;
    
    numberOfClasses = 0;

    while(getline(fin, line)){

        ss.str(line);
       
        while(getline(ss,token,'\t')){
			strTemp.push_back(token);
		}

        for(int i=0; i<strTemp.size()-1; i++){
            temp.push_back(stod(strTemp[i]));
        }
        
        string name = strTemp.back();
        if(name.back() == '\r') name.pop_back();

        dataClass.push_back(name);
        data.push_back(temp);
        
        if(!testData){
            auto it = classIndex.find(name);
            if(it == classIndex.end()){
    			className.push_back(name);
    			classIndex[name] = numberOfClasses++;
    		}
        }

        ss.clear();
        temp.clear();
        strTemp.clear();

    }

    numberOfSamples = data.size();
    numberOfFeatures = data[0].size();
    numberOfClasses = className.size();

    fin.close();
}

void readParameters(string fileName)
{
    ifstream fin;
    fin.open(fileName);

    int siz;
    while(fin >> siz) layerSize.push_back(siz);
    layerSize.push_back(numberOfClasses);

    numberOfLayers = layerSize.size();

    fin.close();
}

void normalize(Matrix &Data)
{
    for(int i=0; i<data[0].size(); i++){
        double mx = 0;
        for(int j=0; j<data.size(); j++) mx = max(mx, data[j][i]);
        for(int j=0; j<data.size(); j++) data[j][i]/=mx;
    }
}