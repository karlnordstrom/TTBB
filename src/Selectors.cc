#include <Selectors.hh>

namespace Analysis {

Comparison::Comparison()
    :  _ptset(false), _massset(false), _deltarset(false), _energyset(false)
      {}

Comparison::~Comparison() {}

void Comparison::ptWithin(float absolute, float relative) {
    _ptabs = abs(absolute);
    _ptrel = abs(relative);
    _ptset = true;
}

void Comparison::massWithin(float absolute, float relative) {
    _massabs = abs(absolute);
    _massrel = abs(relative);
    _massset = true;
}

void Comparison::deltaRWithin(float absolute) {
    _deltarabs = abs(absolute);
    _deltarset = true;
}

void Comparison::energyWithin(float absolute, float relative) {
    _energyabs = abs(absolute);
    _energyrel = abs(relative);
    _energyset = true;
}

bool Comparison::pass(FourMomentum reference, FourMomentum estimate) const {
    bool pt = (_ptset) ? areSimilar(reference.momentum().Pt(), estimate.momentum().Pt(), _ptabs, _ptrel) : true;
    bool mass = (_massset) ? areSimilar(reference.momentum().M(), estimate.momentum().M(), _massabs, _massrel) : true;
    bool energy = (_energyset) ? areSimilar(reference.momentum().E(), estimate.momentum().E(), _energyabs, _energyrel) : true;
    bool deltar = (_deltarset) ? reference.momentum().DeltaR(estimate.momentum()) < _deltarabs : true;
    return pt && mass && energy && deltar;
}

Selector::Selector() {}

Selector::~Selector() {}

void Selector::ptCut(Cut cut, float value) {
    _pt.push_back(value);
    _ptcut.push_back(cut);
}

void Selector::etaCut(Cut cut, float value) {
    _eta.push_back(value);
    _etacut.push_back(cut);
}

void Selector::phiCut(Cut cut, float value) {
    _phi.push_back(value);
    _phicut.push_back(cut);
}

void Selector::massCut(Cut cut, float value) {
    _mass.push_back(value);
    _masscut.push_back(cut);
}

void Selector::energyCut(Cut cut, float value) {
    _energy.push_back(value);
    _energycut.push_back(cut);
}

void Selector::pdgIdCut(Cut cut, int value) {
    _pdgid.push_back(value);
    _pdgidcut.push_back(cut);
}

void Selector::statusCut(Cut cut, int value) {
    _status.push_back(value);
    _statuscut.push_back(cut);
}

bool Selector::pass(FourMomentum vec) const {

    bool pass = true;
    bool pt = cutCheck(_ptcut, _pt, vec.pt());
    pass = pass && pt;
    if (!pass)return pass;

    bool eta = cutCheck(_etacut, _eta, vec.eta());
    pass = pass && eta;
    if (!pass)return pass;

    bool phi = cutCheck(_phicut, _phi, vec.phi());
    pass = pass && phi;
    if (!pass)return pass;

    bool mass = cutCheck(_masscut, _mass, vec.mass());
    pass = pass && mass;
    if (!pass)return pass;

    bool energy = cutCheck(_energycut, _energy, vec.energy());
    pass = pass && energy;
    if (!pass)return pass;

    bool pdgid = cutCheck(_pdgidcut, _pdgid, vec.pdgId());
    pass = pass && pdgid;
    if (!pass)return pass;

    bool status = cutCheck(_statuscut, _status, vec.status());
    pass = pass && status;

    return pass;
}


} // end namespace
