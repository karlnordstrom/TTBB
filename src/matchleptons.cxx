#include "library.h"

using namespace std;

float deltaR(float phi1, float phi2, float eta1, float eta2) {
    TLorentzVector vector1;
    TLorentzVector vector2;
    vector1.SetPtEtaPhiE(10, eta1, phi1, 15);
    vector2.SetPtEtaPhiE(10, eta2, phi2, 15);
    return vector1.DeltaR(vector2);
}

int main() {


//Retrieve the TTree


    TFile *recoFile = new TFile( "/afs/phas.gla.ac.uk/user/k/knordstrom/data/ttbar_bb.root", "READ" );

    TTree *recoTree = (TTree*)recoFile->Get("mini");
    TChain *truthTree = new TChain("truth");

    //truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._*.pool.root");
    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._000001.pool.root");

    TFile *outputFile = new TFile("~/output/comparedata.root", "RECREATE");
    //outputFile->cd();



//Set up the addresses for reading out the branches

    UInt_t EventNumber_truth;

    Int_t  EventNumber_reco;

    Float_t lep_pt_reco, lep_eta_reco, lep_phi_reco;

    vector<float> *part_pt_truth = 0, *part_eta_truth = 0, *part_phi_truth = 0;

    vector<int> *pdgId_truth = 0, *status_truth = 0;


//and others




//declare other analysis variables





//Associate the addresses with the TTree

    recoTree->SetBranchStatus("*", 0);

    recoTree->SetBranchStatus("eventNumber", 1);
    recoTree->SetBranchAddress("eventNumber",&EventNumber_reco);

    recoTree->SetBranchStatus("lep_pt", 1);
    recoTree->SetBranchStatus("lep_eta", 1);
    recoTree->SetBranchStatus("lep_phi", 1);

    recoTree->SetBranchAddress("lep_pt",&lep_pt_reco);
    recoTree->SetBranchAddress("lep_eta",&lep_eta_reco);
    recoTree->SetBranchAddress("lep_phi",&lep_phi_reco);


    truthTree->SetBranchStatus("*", 0);

    truthTree->SetBranchStatus("EventNumber", 1);
    truthTree->SetBranchAddress("EventNumber",&EventNumber_truth);

    truthTree->SetBranchStatus("mc_pt", 1);
    truthTree->SetBranchStatus("mc_eta", 1);
    truthTree->SetBranchStatus("mc_phi", 1);
    truthTree->SetBranchStatus("mc_pdgId", 1);
    truthTree->SetBranchStatus("mc_status", 1);

    truthTree->SetBranchAddress("mc_pdgId",&pdgId_truth);
    truthTree->SetBranchAddress("mc_status",&status_truth);
    truthTree->SetBranchAddress("mc_pt",&part_pt_truth);
    truthTree->SetBranchAddress("mc_eta",&part_eta_truth);
    truthTree->SetBranchAddress("mc_phi",&part_phi_truth);


//Read out all events



    Int_t event_match = 0; //number of events matched by event number
    Int_t lepton_match = 0; // number of events matched by lepton


    Int_t eventIndex;

    for ( eventIndex = 0; eventIndex < (Int_t) truthTree->GetEntries(); eventIndex++ ) {

        truthTree->GetEntry(eventIndex);

        if(eventIndex % 100 == 0)cout << "Processing Event: " << eventIndex << endl;

        for(Int_t recoIndex = 0; recoIndex < (Int_t) recoTree->GetEntries(); recoIndex++) {

            recoTree->GetEntry(recoIndex);
            //if(recoIndex == 1)cout << EventNumber_truth << "	" << EventNumber_reco << endl;
            if(EventNumber_truth != EventNumber_reco)continue;

            assert(EventNumber_truth == EventNumber_reco);
            event_match++; //event matched by event number
            //cout << "Matched events: " << EventNumber_truth << " and " << EventNumber_reco << endl;

            for(Int_t p = 0; p < (Int_t) (*pdgId_truth).size(); p++) {

                if(abs((*pdgId_truth)[p]) > 10 && abs((*pdgId_truth)[p]) < 19 && (*status_truth)[p] == 1) { //is a final state lepton

                    float lepton_pt = (*part_pt_truth)[p];
                    float lepton_phi = (*part_phi_truth)[p];
                    float lepton_eta = (*part_eta_truth)[p];


                    for(Int_t z = 0; z < (Int_t) (*pdgId_truth).size(); z++) {
                        if(abs((*pdgId_truth)[z]) == 22 && (*status_truth)[z] == 1 &&
                                deltaR(lepton_phi, (*part_phi_truth)[z], lepton_eta, (*part_eta_truth)[z]) < 0.1 ) {
                            //cout << (*part_pt_truth)[z] << "	" << lepton_pt << endl;
                            lepton_pt += (*part_pt_truth)[z];
                        }
                    }


                    if( areSimilar( lepton_pt, lep_pt_reco, 5000) &&
                            deltaR( lepton_phi, lep_phi_reco, lepton_eta, lep_eta_reco) < 0.1 ) {

                        lepton_match++;
                        //cout << "Matched lepton." << endl;
                        //cout << (*part_pt_truth)[p] << "	" << lep_pt_reco << endl;
                        //cout << (*part_phi_truth)[p] << "	" << lep_phi_reco << endl;
                        //cout << (*part_eta_truth)[p] << "	" << lep_eta_reco << endl;
                        break;
                    } //lepton matched
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
