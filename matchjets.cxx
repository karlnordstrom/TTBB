/// MACRO FOR ANALYSING RECO AND TRUTH JETS

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
//#include <TROOT.h>

#define foreach BOOST_FOREACH //just to clean up the loops a bit

using namespace std;

bool areSimilar(float reference, float estimate, float absolute, float relative = 0.1) {
    return (estimate > (reference - (absolute + relative * abs(reference)))) && (estimate < (reference + absolute + relative * abs(reference)));
}

vector<TLorentzVector> makeVectors(vector<float> pt, vector<float> eta, vector<float> phi, vector<float> E) {
    assert(pt.size() == eta.size() && eta.size() == phi.size() && phi.size() == E.size());//make sure input makes sense
    vector<TLorentzVector> vectors;
    TLorentzVector vector;
    for(unsigned int it = 0; it < pt.size(); it++) {
        vector.SetPtEtaPhiE(pt[it]/1000., eta[it], phi[it], E[it]/1000.); // convert to GeV
        vectors.push_back(vector);
    }
    return vectors;
}


// Helper class for creating histograms with average values over
// x range since this apparently is hard.
class Average {

public:
    Average(int array, float min, float max)
        : _min(min), _max(max), _bin((float)(max-min)/array)
    {
        _array.resize(array);
        _entries.resize(array);
        _averages.resize(array);
    }

    ~Average() {}

    void add(float x, float y) {
        if(x > _max || x < _min)return;
        int index = round((x - _min)/_bin - 0.49);
        _array[index] += y;
        _entries[index]++;
    }

    void setAverage() {
        for(int i = 0; i < (int)_averages.size(); i++) {
            if (_entries[i] != 0)_averages[i] = (float)_array[i] / _entries[i];
        }
    }

    vector<float> getAverage() {
        setAverage();
        return _averages;
    }

    void fillHisto(TH1D* histo) {
        setAverage();
        for(int i = 0; i < (int)_averages.size(); i++) {
            histo->Fill(_min + round(i*_bin - 0.49), _averages[i]);
        }
    }

private:
    float _min;
    float _max;
    float _bin;
    vector<float> _array;
    vector<int> _entries;
    vector<float> _averages;

};

int main() {


//Retrieve the TTrees


    TFile *recoFile = new TFile( "/afs/phas.gla.ac.uk/user/k/knordstrom/data/ttbar_bb.root", "READ" );
    TTree *recoTree = (TTree*)recoFile->Get("mini");

    TChain *truthTree = new TChain("truth");
    //truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._*.pool.root");
    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._000001.pool.root");

    TFile *outputFile = new TFile("~/output/comparedata.root", "RECREATE");
    outputFile->cd(); // WHY IS THIS NEEDED ROOT IS SO CONFUSING

//Set up the addresses for reading out the branches

    UInt_t EventNumber_truth;

    UInt_t jet_n;

    Int_t  EventNumber_reco;

    Float_t jet_pt_reco[40], jet_eta_reco[40], jet_phi_reco[40], jet_E_reco[40];

    vector<float> *jet_pt_truth = 0, *jet_eta_truth = 0, *jet_phi_truth = 0, *jet_E_truth = 0;

//Associate the addresses with the TTree

    recoTree->SetBranchStatus("*", 0);

    recoTree->SetBranchStatus("eventNumber", 1);
    recoTree->SetBranchAddress("eventNumber",&EventNumber_reco);

    recoTree->SetBranchStatus("jet_pt", 1);
    recoTree->SetBranchStatus("jet_eta", 1);
    recoTree->SetBranchStatus("jet_phi", 1);
    recoTree->SetBranchStatus("jet_E", 1);
    recoTree->SetBranchStatus("jet_n", 1);

    recoTree->SetBranchAddress("jet_pt",&jet_pt_reco);
    recoTree->SetBranchAddress("jet_eta",&jet_eta_reco);
    recoTree->SetBranchAddress("jet_phi",&jet_phi_reco);
    recoTree->SetBranchAddress("jet_E",&jet_E_reco);
    recoTree->SetBranchAddress("jet_n",&jet_n);

    truthTree->SetBranchStatus("*", 0);

    truthTree->SetBranchStatus("EventNumber", 1);
    truthTree->SetBranchAddress("EventNumber",&EventNumber_truth);

    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_pt", 1);
    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_eta", 1);
    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_phi", 1);
    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_E", 1);

    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_pt",&jet_pt_truth);
    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_eta",&jet_eta_truth);
    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_phi",&jet_phi_truth);
    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_E",&jet_E_truth);

// Declare histos

    TH1D *dpT_jet = new TH1D("dpT_jet", "dpT_jet (positive is reco is greater)", 50, -50, 50);
    TH1D *dR_jet = new TH1D("dR_jet", "dR_jet", 30, 0, 0.3);
    TH1D *dpT_jet_ave = new TH1D("dpT_jet_ave", "dpT_jet_ave", 25, 0, 250);
    TH1D *n_matches_truth = new TH1D("n_matches_truth", "number of reco jets matched to truth jet", 5, 0, 5);

    TH2D *pT_truth_vs_reco = new TH2D("pT_truth_vs_reco", "pT_truth_vs_reco (x axis is truth)", 25, 0, 250, 25, 0, 250);


//Read out all events

    Int_t event_match = 0; //number of events matched by event number
    Int_t jet_match_local = 0; //reco -> truth jets matched in *this* event
    Int_t jet_match_total = 0; //reco -> truth jets matched in total
    Int_t jet_reco_total = 0; //total number of reco jets
    Int_t jet_truth_total = 0; //total number of truth jets

    Average average_dpt(25, 0, 250); // initialise helper for average histo

    Int_t eventIndex;

    for ( eventIndex = 0; eventIndex < (Int_t) truthTree->GetEntries(); eventIndex++ ) {

        truthTree->GetEntry(eventIndex);

        if(eventIndex % 100 == 0)cout << "Processing Event: " << eventIndex << endl;
        for(Int_t recoIndex = 0; recoIndex < (Int_t) recoTree->GetEntries(); recoIndex++) {

            recoTree->GetEntry(recoIndex);
            if((int)EventNumber_truth != (int)EventNumber_reco) {
                continue;   // loop over reco tree until the correct event is found
            }

            assert((int)EventNumber_truth == (int)EventNumber_reco); // extra assert to kill any bullshit
            event_match++; //event matched by event number

            // Uncomment to get continuous confirmation that at least something is working:
            //cout << "Matched events: " << EventNumber_truth << " and " << EventNumber_reco << endl;

            //Need to create vectors because reco jets stored as C arrays (YAY)
            vector<float> pts, etas, phis, Es;
            for(unsigned int rjet = 0; rjet < jet_n; rjet++) {
                pts.push_back(jet_pt_reco[rjet]);
                etas.push_back(jet_eta_reco[rjet]);
                phis.push_back(jet_phi_reco[rjet]);
                Es.push_back(jet_E_reco[rjet]);
            }

            vector<TLorentzVector> reco_jets = makeVectors(pts, etas, phis, Es);
            vector<TLorentzVector> truth_jets = makeVectors((*jet_pt_truth), (*jet_eta_truth), (*jet_phi_truth), (*jet_E_truth) );

            jet_match_local = 0;

	    // Only interested in the 6 leading reco jets
            if(reco_jets.size() > 6) {
                reco_jets.erase(reco_jets.begin() + 6, reco_jets.end());
            }

	    foreach(TLorentzVector tjet, truth_jets) {
		if( tjet.Pt() > 20 ){jet_truth_total++;} // only count truth jets over 20GeV
	    }

	    //This is all just to map how many reco jets get matched to
	    //each truth jet. A bit messy atm.
	    vector<int> mapped;
	    mapped.resize(truth_jets.size());

            foreach(TLorentzVector rjet, reco_jets) {

		int index = -1;

                foreach(TLorentzVector tjet, truth_jets) {
	
		    index++;

                    if( areSimilar( tjet.Pt(), tjet.Pt(), 50) && tjet.DeltaR(rjet) < 0.2 ) {
			mapped[index]++;
                        jet_match_local++;
                        dpT_jet->Fill(rjet.Pt() - tjet.Pt());
                        dR_jet->Fill(tjet.DeltaR(rjet));
                        average_dpt.add(tjet.Pt(), abs(rjet.Pt() - tjet.Pt()));
                        pT_truth_vs_reco->Fill(tjet.Pt(), rjet.Pt());
                        break;
                    }
                }
            }

	    foreach(int n_matches, mapped) {
	    	n_matches_truth->Fill(n_matches);
	    }

            jet_match_total += jet_match_local;
            jet_reco_total += reco_jets.size();

            //cout << "Jets matched in this event: " << jet_match_local << "/" << reco_jets.size() << endl;
            break;

        }

    }

    average_dpt.fillHisto(dpT_jet_ave);

    cout << "Number of matching events: " << event_match << endl;
    cout << "Number of total jets matched (reco -> truth): " << jet_match_total << "/" << jet_reco_total << endl;
    cout << "(Fraction: " << (float)jet_match_total/jet_reco_total  << " )" << endl;
    cout << "Number of total jets matched (truth -> reco): " << jet_match_total << "/" << jet_truth_total << endl;
    cout << "(Fraction: " << (float)jet_match_total/jet_truth_total  << " )" << endl;


    //Finalize


    recoFile->Close();




    outputFile->Write();
    outputFile->Close();

    return 0;


}//end
