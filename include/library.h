#include <Math/Vector4D.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVector2.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TChain.h>

#include <cmath>
#include <vector>
#include <iostream>
#include <exception>
#include <boost/foreach.hpp>
#include <vector>

#define foreach BOOST_FOREACH //just to clean up the loops a bit

using namespace std;

/// Helper function for comparing two floats
bool areSimilar(float reference, float estimate, float absolute, float relative = 0.1);

/// Helper function for creating TLorentzVectors out of
/// vectors containing the pt, eta, phi, and E values
vector<TLorentzVector> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E);


/// Helper class for creating histos of average values:
/// much nicer than dividing two histos!
class AverageHisto {

public:

    AverageHisto(int array, float min, float max);
    ~AverageHisto();
    void add(float x, float y);
    void setAverage();
    vector<float> getAverage();
    void fillHisto(TH1D* histo);

private:

    float _min;
    float _max;
    float _bin;
    vector<float> _array;
    vector<int> _entries;
    vector<float> _averages;

};
