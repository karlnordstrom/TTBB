#include <FourMomentum.hh>

using namespace std;

namespace Analysis {

FourMomentum::FourMomentum(TLorentzVector& mom, int pdgid, int status)
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

void FourMomentum::setPdgId(const int pdgid) {
    _pdgId = pdgid;
}

void FourMomentum::setStatus(const int status) {
    _status = status;
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

} // end namespace
