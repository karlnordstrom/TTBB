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

#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <exception>
#include <boost/foreach.hpp>
#include <vector>

#include <exception>
#include <stdexcept>

#define foreach BOOST_FOREACH //just to clean up the loops a bit

using namespace std;

/// My own four momentum class to allow storage of
/// pdgid and status codes with the vector
/// (and other stuff in the future maybe)
class FourMomentum {

public:

    FourMomentum();
    FourMomentum(TLorentzVector mom, int pdgid, int status);
    ~FourMomentum();
    TLorentzVector momentum() const;
    void setMomentum(const TLorentzVector& vec);
    int pdgId() const;
    int status() const;
    float pt() const;
    float eta() const;
    float phi() const;
    float energy() const;
    float mass() const;

    float deltaR(FourMomentum other) const;
    float deltaPt(FourMomentum other) const;

    FourMomentum operator+ (const FourMomentum& other) const;
    void operator+= (const FourMomentum& other);
    FourMomentum operator- (const FourMomentum& other) const;
    void operator-= (const FourMomentum& other);


private:

    TLorentzVector _tvector;
    int _pdgId;
    int _status;

};



/// Class for throwing exceptions
class Exception : public std::runtime_error {

public:
    /// Constructor with error description string
    Exception(const std::string& what) : std::runtime_error(what) {}

};

/// Helper function for comparing two floats
bool areSimilar(float reference, float estimate, float absolute, float relative = 0.1);

/// Helper functions for creating FourMomentum out of
/// vectors containing the pt, eta, phi, and E values
vector<FourMomentum> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E);

FourMomentum makeVector(float pt, float eta, float phi, float E);

vector<FourMomentum> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E, vector<int> pdgId, vector<int> status);

FourMomentum makeVector(float pt, float eta, float phi, float E, int pdgId, int status);


/// Helper class for creating histos of average values:
/// much nicer than dividing two histos!
class AverageHisto {

public:

    AverageHisto(int array, float min, float max);
    AverageHisto(TH1D* histo);
    ~AverageHisto();
    void add(float x, float y);
    void setAverage();
    vector<float> getAverage();
    void fillHisto(TH1D* histo);
    void fillHisto();

private:

    float _min;
    float _max;
    float _bin;
    vector<float> _array;
    vector<int> _entries;
    vector<float> _averages;
    TH1D* _histogram;

};

/// Helper class for building sets of selections
/// on TLorentzVectors to make it easier to make
/// more complex combinations of selections
class Selector {

public:

    Selector();
    ~Selector();
    void ptCut(float absolute, float relative = 0.1);
    void massCut(float absolute, float relative = 0.1);
    void deltaRCut(float absolute);
    void energyCut(float absolute, float relative = 0.1);
    bool pass(FourMomentum reference, FourMomentum estimate) const;

private:

    float _ptabs, _ptrel;
    float _massabs, _massrel;
    float _deltarabs;
    float _energyabs, _energyrel;
    bool _ptset, _massset, _deltarset, _energyset;
};
