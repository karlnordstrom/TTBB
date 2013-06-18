#ifndef FourMomentum_HH
#define FourMomentum_HH

#include <Utils.hh>

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

/// Helper functions for creating FourMomentum out of
/// vectors containing the pt, eta, phi, and E values
vector<FourMomentum> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E);

FourMomentum makeVector(float pt, float eta, float phi, float E);

vector<FourMomentum> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E, vector<int> pdgId, vector<int> status);

FourMomentum makeVector(float pt, float eta, float phi, float E, int pdgId, int status);

/// Helper class for building sets of selections
/// on FourMomentums to make it easier to make
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


#endif
