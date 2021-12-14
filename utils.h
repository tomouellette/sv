#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <random>
#include <algorithm>

using namespace std;

template< class rng >
double sample_uniformr(rng &generator, double a, double b) 
{
	uniform_real_distribution<double> dist(a, b);
	return dist(generator);
}

template< class rng >
double sample_uniformi(rng &generator, int a, int b) 
{
	uniform_int_distribution<int> dist(a, b);
	return dist(generator);
}

template< class rng >
int sample_poisson(rng &generator, int u) 
{
	poisson_distribution<int> dist(u);
	return dist(generator);
}

template< class rng >
int sample_normal(rng &generator, float m, float s) 
{
	normal_distribution<int> dist(m, s);
	return dist(generator);
}

template< class rng >
int sample_bimodal(rng &generator, float m1, float s1, float m2, float s2) 
{
    float r = sample_uniformr(generator, 0, 1);
    if (r < 0.5) {
        return sample_normal(generator, m1, s1);
    } else {
        return sample_normal(generator, m2, s2);
    }
}

vector<int> index_sort_int(vector<int> indices, vector<int> sortee) {
	vector<int> sorted(sortee.size());
	for (int i=0; i < indices.size(); i++) {
		sorted[i] = sortee[indices[i]];
	}
	return sorted;
};

vector<string> index_sort_string(vector<int> indices, vector<string> sortee) {
	vector<string> sorted(sortee.size());
	for (int i=0; i < indices.size(); i++) {
		sorted[i] = sortee[indices[i]];
	}
	return sorted;
};

vector<string> remove_duplicates(vector<string> v) {
	vector<string> vd = v;
	sort(vd.begin(), vd.end());
	vd.erase(unique(vd.begin(), vd.end()), vd.end());
	return vd;
};
	
int dot_product(vector<int> v1, vector<int> v2) {
    int dp = 0;
    for (int i=0; i < v1.size(); i++) {
        dp += v1[i] * v2[i];
	}
    return dp;
};

vector<int> string_to_vec(string s) {
    vector<char> v(s.begin(), s.end());
    vector<int> convert;
    for (int i=0; i < v.size(); i++) {
        int c = v[i] - '0';
        convert.push_back(c);
    }
	return convert;
};

#endif