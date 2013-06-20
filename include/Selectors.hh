#ifndef Selectors_HH
#define Selectors_HH

#include <Utils.hh>
#include <FourMomentum.hh>

using namespace std;

namespace Analysis {

enum Cut {More, Less, Equal, NotEqual, None};

/// Templated functions for actually performing cuts
template<class N, class M>
bool cutCheck(Cut cut, N test, M tobetested) {
    if(cut == None)return true;
    else if(cut == More)return (tobetested > test);
    else if(cut == Less)return (tobetested < test);
    else if(cut == Equal)return (tobetested == test);
    else if(cut == NotEqual)return (tobetested != test);
    else return true;
}
template<class N, class M>
bool cutCheck(vector<Cut> cut, vector<N> test, M tobetested) {
    assert(cut.size() == test.size());
    bool rtn = true;
    for(unsigned int it = 0; it < cut.size(); ++it) {
        bool tmp = cutCheck(cut[it], test[it], tobetested);
        rtn = rtn && tmp;
        if(!rtn)break;
    }
    return rtn;
}



/// Helper function for comparing two floats --
/// templated for the greater good.
template <class N, class M, class R, class T>
bool areSimilar(N reference, M estimate, R absolute, T relative = 0.1) {
    return (estimate > (reference - (absolute + relative * abs(reference)))) && (estimate < (reference + (absolute + relative * abs(reference))));
}


/// Helper class for building sets of selections
/// on FourMomentums to make it easier to make
/// more complex combinations of selections
class Comparison {

public:

    Comparison();
    ~Comparison();
    void ptWithin(float absolute, float relative = 0.1);
    void massWithin(float absolute, float relative = 0.1);
    void deltaRWithin(float absolute);
    void energyWithin(float absolute, float relative = 0.1);
    bool pass(FourMomentum reference, FourMomentum estimate) const;

private:

    float _ptabs, _ptrel;
    float _massabs, _massrel;
    float _deltarabs;
    float _energyabs, _energyrel;
    bool _ptset, _massset, _deltarset, _energyset;
};

class Selector {

public:

    Selector();
    ~Selector();
    void ptCut(Cut cut, float value);
    void etaCut(Cut cut, float value);
    void phiCut(Cut cut, float value);
    void massCut(Cut cut, float value);
    void energyCut(Cut cut, float value);
    void pdgIdCut(Cut cut, int value);
    void statusCut(Cut cut, int value);
    bool pass(FourMomentum vec) const;

private:

    vector<float> _pt, _eta, _phi, _mass, _energy;
    vector<int> _pdgid, _status;

    vector<Cut> _ptcut, _etacut, _phicut, _masscut,
        _energycut, _pdgidcut, _statuscut;

};

} // end namespace
#endif
