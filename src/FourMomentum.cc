#include <FourMomentum.hh>

using namespace std;

FourMomentum::FourMomentum(TLorentzVector mom, int pdgid, int status)
    : _tvector(mom), _pdgId(pdgid), _status(status)
    {}

FourMomentum::FourMomentum()
    : _pdgId(0), _status(0)  {}

FourMomentum::~FourMomentum() {}

TLorentzVector FourMomentum::momentum() const {
    return _tvector;
}

int FourMomentum::pdgId() const {
    return _pdgId;
}

int FourMomentum::status() const {
    return _status;
}

float FourMomentum::deltaR(FourMomentum other) const {
    return _tvector.DeltaR(other.momentum());
}

float FourMomentum::deltaPt(FourMomentum other) const {
    return _tvector.Pt() - other.momentum().Pt();
}

float FourMomentum::pt() const {
    return _tvector.Pt();
}
float FourMomentum::eta() const {
    return _tvector.Eta();
}
float FourMomentum::phi() const {
    return _tvector.Phi();
}
float FourMomentum::energy() const {
    return _tvector.E();
}
float FourMomentum::mass() const {
    return _tvector.M();
}

void FourMomentum::setMomentum(const TLorentzVector& vec) {
    _tvector = vec;
}

FourMomentum FourMomentum::operator+ (const FourMomentum& other) const {
    FourMomentum result;
    TLorentzVector vec = this->momentum() + other.momentum();
    result.setMomentum(vec);
    return result;
}

void FourMomentum::operator+= (const FourMomentum& other) {
    TLorentzVector vec = this->momentum() + other.momentum();
    this->setMomentum(vec);
}

FourMomentum FourMomentum::operator- (const FourMomentum& other) const {
    FourMomentum result;
    TLorentzVector vec = this->momentum() - other.momentum();
    result.setMomentum(vec);
    return result;
}

void FourMomentum::operator-= (const FourMomentum& other) {
    TLorentzVector vec = this->momentum() - other.momentum();
    this->setMomentum(vec);
}

FourMomentum makeVector(float pt, float eta, float phi, float E) {
    TLorentzVector vector;
    vector.SetPtEtaPhiE(pt/1000., eta, phi, E/1000.); // convert to GeV
    FourMomentum mom(vector, 0, 0);
    return mom;
}


vector<FourMomentum> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E) {
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

FourMomentum makeVector(float pt, float eta, float phi, float E, int pdgId, int status) {
    TLorentzVector vector;
    vector.SetPtEtaPhiE(pt/1000., eta, phi, E/1000.); // convert to GeV
    FourMomentum mom(vector, pdgId, status);
    return mom;
}


vector<FourMomentum> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E, vector<int> pdgId, vector<int> status) {
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

Selector::Selector()
    :  _ptset(false), _massset(false), _deltarset(false), _energyset(false)
      {}

Selector::~Selector() {}

void Selector::ptCut(float absolute, float relative) {
    _ptabs = abs(absolute);
    _ptrel = abs(relative);
    _ptset = true;
}

void Selector::massCut(float absolute, float relative) {
    _massabs = abs(absolute);
    _massrel = abs(relative);
    _massset = true;
}

void Selector::deltaRCut(float absolute) {
    _deltarabs = abs(absolute);
    _deltarset = true;
}

void Selector::energyCut(float absolute, float relative) {
    _energyabs = abs(absolute);
    _energyrel = abs(relative);
    _energyset = true;
}

bool Selector::pass(FourMomentum reference, FourMomentum estimate) const {
    bool pt = (_ptset) ? areSimilar(reference.momentum().Pt(), estimate.momentum().Pt(), _ptabs, _ptrel) : true;
    bool mass = (_massset) ? areSimilar(reference.momentum().M(), estimate.momentum().M(), _massabs, _massrel) : true;
    bool energy = (_energyset) ? areSimilar(reference.momentum().E(), estimate.momentum().E(), _energyabs, _energyrel) : true;
    bool deltar = (_deltarset) ? reference.momentum().DeltaR(estimate.momentum()) < _deltarabs : true;
    return pt && mass && energy && deltar;
}

