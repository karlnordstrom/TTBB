#include <Utils.hh>
#include <FourMomentum.hh>
#include <Selectors.hh>

using namespace std;
using namespace Analysis;

int main() {

//Retrieve the TTree

    TFile *recoFile = new TFile( "/afs/phas.gla.ac.uk/user/k/knordstrom/data/ttbar_bb.root", "READ" );

    TTree *recoTree = (TTree*)recoFile->Get("mini");
    TChain *truthTree = new TChain("truth");

    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._*.pool.root");
//    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._000001.pool.root");

    TFile *outputFile = new TFile("~/output/comparedata.root", "RECREATE");

//Set up the addresses for reading out the branches

    UInt_t EventNumber_truth;
    Int_t  EventNumber_reco;
    Float_t lep_pt_reco, lep_eta_reco, lep_phi_reco, lep_E_reco;
    vector<float> *part_pt_truth = 0, *part_eta_truth = 0, *part_phi_truth = 0, *part_E_truth = 0;
    vector<int> *pdgId_truth = 0, *status_truth = 0;

//Associate the addresses with the TTree

    recoTree->SetBranchStatus("*", 0);

    recoTree->SetBranchStatus("eventNumber", 1);
    recoTree->SetBranchAddress("eventNumber",&EventNumber_reco);

    recoTree->SetBranchStatus("lep_pt", 1);
    recoTree->SetBranchStatus("lep_eta", 1);
    recoTree->SetBranchStatus("lep_phi", 1);
    recoTree->SetBranchStatus("lep_E", 1);

    recoTree->SetBranchAddress("lep_pt",&lep_pt_reco);
    recoTree->SetBranchAddress("lep_eta",&lep_eta_reco);
    recoTree->SetBranchAddress("lep_phi",&lep_phi_reco);
    recoTree->SetBranchAddress("lep_E",&lep_E_reco);


    truthTree->SetBranchStatus("*", 0);

    truthTree->SetBranchStatus("EventNumber", 1);
    truthTree->SetBranchAddress("EventNumber",&EventNumber_truth);

    truthTree->SetBranchStatus("mc_pt", 1);
    truthTree->SetBranchStatus("mc_eta", 1);
    truthTree->SetBranchStatus("mc_phi", 1);
    truthTree->SetBranchStatus("mc_E", 1);
    truthTree->SetBranchStatus("mc_pdgId", 1);
    truthTree->SetBranchStatus("mc_status", 1);

    truthTree->SetBranchAddress("mc_pdgId",&pdgId_truth);
    truthTree->SetBranchAddress("mc_status",&status_truth);
    truthTree->SetBranchAddress("mc_pt",&part_pt_truth);
    truthTree->SetBranchAddress("mc_eta",&part_eta_truth);
    truthTree->SetBranchAddress("mc_phi",&part_phi_truth);
    truthTree->SetBranchAddress("mc_E",&part_E_truth);


//Read out all events

    Comparison SimpleCut;
    SimpleCut.ptWithin(1.);
    SimpleCut.deltaRWithin(0.1);

    Int_t event_match = 0; //number of events matched by event number
    Int_t lepton_match = 0; // number of events matched by lepton

    Int_t eventIndex;

    for ( eventIndex = 0; eventIndex < (Int_t) truthTree->GetEntries(); eventIndex++ ) {

        truthTree->GetEntry(eventIndex);
        if(eventIndex % 100 == 0)cout << "Processing Event: " << eventIndex << endl;

        for(Int_t recoIndex = 0; recoIndex < (Int_t) recoTree->GetEntries(); recoIndex++) {

            recoTree->GetEntry(recoIndex);
            if((int)EventNumber_truth != (int)EventNumber_reco)continue;

            assert((int)EventNumber_truth == (int)EventNumber_reco);
            event_match++; //event matched by event number

            /// Create FourMomentum for convenience
            FourMomentum rlep = makeVector(lep_pt_reco, lep_eta_reco, lep_phi_reco, lep_E_reco);
            vector<FourMomentum> tparts = makeVectors((*part_pt_truth), (*part_eta_truth), (*part_phi_truth), (*part_E_truth), (*pdgId_truth), (*status_truth));

            /// First find the leptons, then dress them, then compare to
            /// reco lepton to try to find match
            foreach( FourMomentum tlep, tparts) {

                if( abs(tlep.pdgId()) > 10 && abs(tlep.pdgId()) < 19 && tlep.status() == 1 ) { //is a final state lepton

                    foreach( FourMomentum photon, tparts) {
                        if( abs(photon.pdgId()) == 22 && photon.status() == 1 && tlep.deltaR(photon) < 0.1 ) {
                            tlep += photon; //dress lepton with photon
                        }
                    }

                    if( SimpleCut.pass(tlep, rlep) ) {
                        lepton_match++;
                        //cout << "Matched lepton." << endl;
                        break;
                    }
                }
            }
            break;
        }

    }

    cout << "Number of matching events: " << event_match << endl;
    cout << "Number of matching lepton events: " << lepton_match << endl;
    cout << "(Fraction: " << (float)lepton_match/event_match << " )" << endl;

    //Finalize

    recoFile->Close();

    outputFile->Write();
    outputFile->Close();

    return 0;

}//end
