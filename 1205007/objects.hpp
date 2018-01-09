random_device rd;
uniform_real_distribution<double> rng(-1, 1);

double getRandomDouble() { return rng(rd); }

struct Vector{
	vector<double> vec;

	Vector(){}
	Vector(int siz, double val = 0){vec.resize(siz, val);}
	Vector(vector<double> v){vec = v;}
	Vector(const Vector &p) {vec = p.vec;}
	
	void setSize(int siz, double val = 0){vec.resize(siz, val);}
	void set(vector<double> &p) {vec = p;}
	void setRandom(int siz){
		for(int i=0; i<siz; i++) vec.push_back(getRandomDouble());
	}
	int getDimension(){return vec.size();}

    Vector operator + (const Vector &p) const {
    	assert(vec.size() == p.vec.size());
    	vector<double> temp;
    	for(int i=0; i<vec.size(); i++) temp.push_back(vec[i] + p.vec[i]);
    	return Vector(temp); 
    }

    Vector operator - (const Vector &p) const {
    	assert(vec.size() == p.vec.size());
    	vector<double> temp;
    	for(int i=0; i<vec.size(); i++) temp.push_back(vec[i] - p.vec[i]);
    	return Vector(temp); 
    }

    double operator * (const Vector &p) const {
    	assert(vec.size() == p.vec.size());
    	double ret = 0;
    	for(int i=0; i<vec.size(); i++) ret += vec[i] * p.vec[i];
    	return ret; 
    }

    Vector operator + (double c) const {
    	vector<double> temp;
    	for(int i=0; i<vec.size(); i++) temp.push_back(vec[i] + c);
    	return Vector(temp); 
    }

    Vector operator - (double c) const {
    	vector<double> temp;
    	for(int i=0; i<vec.size(); i++) temp.push_back(vec[i] - c);
    	return Vector(temp); 
    }

    Vector operator * (double c) const {
    	vector<double> temp;
    	for(int i=0; i<vec.size(); i++) temp.push_back(vec[i] * c);
    	return Vector(temp); 
    }

    Vector operator / (double c) const {
    	vector<double> temp;
    	for(int i=0; i<vec.size(); i++) temp.push_back(vec[i] / c);
    	return Vector(temp); 
    }

    double norm(){
    	double ret = 0;
    	for(auto v: vec) ret += v*v;
    	return ret;
    }

    void show(){
    	for(auto v: vec) cout << v << ' ';
    	cout << endl;
    }

    Vector insertOne(){
    	vector<double> temp(1, 1);
    	temp.insert(temp.end(), vec.begin(), vec.end());
    	return Vector(temp);
    }

    int maxIndex(){
    	double mx = vec[0];
    	int ret = 0;

    	for(int i=1; i<vec.size(); i++){
    		if(mx < vec[i]){
    			mx = vec[i];
    			ret = i;
    		}
    	}
    	return ret;
    }
};

template<class T>
void  display(vector<T> vec){
    for(auto v: vec) cout << v << ' ';
    cout << endl;
}