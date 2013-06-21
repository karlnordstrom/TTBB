#ifndef FourMomentum_HH
#define FourMomentum_HH

#include <Utils.hh>

using namespace std;

namespace Analysis {

/// My own four momentum class to allow storage of
/// pdgid and status codes with the vector
/// (and other stuff in the future maybe)

class FourMomentum {

public:

    FourMomentum();
    FourMomentum(TLorentzVector& mom, int pdgid, int status);
    ~FourMomentum();
    TLorentzVector momentum() const;

    /// Setters
    void setMomentum(const TLorentzVector& vec);
    void setPdgId(const int pdgid);
    void setStatus(const int status);

    /// Accessers
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

template <class N, class M, class R, class T>
FourMomentum makeVector(N pt, M eta, R phi, T E) {
    TLorentzVector vector;
    vector.SetPtEtaPhiE(pt/1000., eta, phi, E/1000.); // convert to GeV
    FourMomentum mom(vector, 0, 0);
    return mom;
}

template <class N, class M, class R, class T>
FourMomentum makeVector(N pt, M eta, R phi, T E, int pdgId, int status) {
    TLorentzVector vector;
    vector.SetPtEtaPhiE(pt/1000., eta, phi, E/1000.); // convert to GeV
    FourMomentum mom(vector, pdgId, status);
    return mom;
}

template <class N, class M, class R, class T>
vector<FourMomentum> makeVectors(vector<N> pt, vector<M> eta, vector<R> phi, vector<T> E, vector<int> pdgId, vector<int> status) {
    assert(pt.size() == eta.size() && eta.size() == phi.size() && phi.size() == E.size() && E.size() == pdgId.size() && pdgId.size() == status.size());//make sure input makes sense
    vector<FourMomentum> vectors;
    TLorentzVector vector;
    for(unsigned int it = 0; it < pt.size(); it++) {
        vector.SetPtEtaPhiE(pt[it]/1000., eta[it], phi[it], E[it]/1000.); // convert to GeV
        FourMomentum mom(vector, pdgId[it], status[it]);
        vectors.push_back(mom);
    }
    return vectors;
}

template<class N, class M, class R, class T>
vector<FourMomentum> makeVectors(vector<N> pt, vector<M> eta, vector<R> phi, vector<T> E) {
    assert(pt.size() == eta.size() && eta.size() == phi.size() && phi.size() == E.size());//make sure input makes sense
    vector<FourMomentum> vectors;
    TLorentzVector vector;
    for(unsigned int it = 0; it < pt.size(); it++) {
        vector.SetPtEtaPhiE(pt[it]/1000., eta[it], phi[it], E[it]/1000.); // convert to GeV
        FourMomentum mom(vector, 0, 0);
        vectors.push_back(mom);
    }
    return vectors;
}



} // end namespace

#endif
