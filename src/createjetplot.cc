#include <Utils.hh>
#include <FourMomentum.hh>
#include <fstream>
#include <string.h>

using namespace std;

int main(int argc,char *argv[]) {


//Retrieve the TTrees


    TFile *recoFile = new TFile( "/afs/phas.gla.ac.uk/user/k/knordstrom/data/ttbar_bb.root", "READ" );
    TTree *recoTree = (TTree*)recoFile->Get("mini");

    TChain *truthTree = new TChain("truth");
    //truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._*.pool.root");
    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._000001.pool.root");

//Set up the addresses for reading out the branches

    UInt_t EventNumber_truth;
    UInt_t jet_n;
    Int_t  EventNumber_reco;
    Float_t jet_pt_reco[40], jet_eta_reco[40], jet_phi_reco[40], jet_E_reco[40];
    Float_t lep_pt_reco, lep_eta_reco, lep_phi_reco, lep_E_reco;
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

    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_pt", 1);
    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_eta", 1);
    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_phi", 1);
    truthTree->SetBranchStatus("jet_AntiKt4TruthJets_E", 1);

    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_pt",&jet_pt_truth);
    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_eta",&jet_eta_truth);
    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_phi",&jet_phi_truth);
    truthTree->SetBranchAddress("jet_AntiKt4TruthJets_E",&jet_E_truth);

    Int_t eventIndex;
    Int_t num_of_events = 0;

    Selector LeptonCut;
    LeptonCut.deltaRCut(0.2);

    for ( eventIndex = 0; eventIndex < (Int_t) truthTree->GetEntries(); eventIndex++ ) {

        truthTree->GetEntry(eventIndex);

        if(eventIndex % 100 == 0)cout << "Processing Event: " << eventIndex << endl;
        for(Int_t recoIndex = 0; recoIndex < (Int_t) recoTree->GetEntries(); recoIndex++) {

            recoTree->GetEntry(recoIndex);
            if((int)EventNumber_truth != (int)EventNumber_reco) {continue;}

            assert((int)EventNumber_truth == (int)EventNumber_reco); // extra assert to kill any bullshit
            num_of_events++;

            ofstream myfile;

            char number[10];
            sprintf(number, "%d", (int)EventNumber_truth);

            char name[80];
            strcpy (name, argv[1]);
            strcat (name, number);
            myfile.open(name);

            //Need to create vectors because reco jets stored as C arrays (YAY)
            vector<float> pts, etas, phis, Es;
            for(unsigned int rjet = 0; rjet < jet_n; rjet++) {
                pts.push_back(jet_pt_reco[rjet]);
                etas.push_back(jet_eta_reco[rjet]);
                phis.push_back(jet_phi_reco[rjet]);
                Es.push_back(jet_E_reco[rjet]);
            }

            vector<FourMomentum> reco_jets = makeVectors(pts, etas, phis, Es);
            vector<FourMomentum> truth_jets = makeVectors((*jet_pt_truth), (*jet_eta_truth), (*jet_phi_truth), (*jet_E_truth) );
            FourMomentum rlep = makeVector(lep_pt_reco, lep_eta_reco, lep_phi_reco, lep_E_reco);

            // Only interested in the 6 leading reco jets
            if(reco_jets.size() > 6) {
                reco_jets.erase(reco_jets.begin() + 6, reco_jets.end());
            }

            myfile << "R" << "  " << "-" << " " << 0 << "  " << rlep.pt() << " " << rlep.eta() << "    " << rlep.phi() << "    " << rlep.energy() << "  " << 0 <<"\n";

            foreach(FourMomentum rjet, reco_jets) {
                myfile << "R" << "  " << "-" << " " << 3 << "  " << rjet.pt() << " " << rjet.eta() << "    " << rjet.phi() << "    " << rjet.energy() << "  " << 0 <<"\n";
            }

            foreach(FourMomentum tjet, truth_jets) {
                if(tjet.pt() < 20)myfile << "T3" << "  " << "-" << "    " << 1 << "  " << tjet.pt() << " " << tjet.eta() << "    " << tjet.phi() << "    " << tjet.energy() << "  " << 0 <<"\n";
                else if(abs(tjet.eta()) > 2.25)myfile << "T3" << "  " << "-" << "    " << 2 << "  " << tjet.pt() << " " << tjet.eta() << "    " << tjet.phi() << "    " << tjet.energy() << "  " << 0 <<"\n";
                else if(LeptonCut.pass(tjet, rlep))myfile << "T3" << "  " << "-" << "    " << 3 << "  " << tjet.pt() << " " << tjet.eta() << "    " << tjet.phi() << "    " << tjet.energy() << "  " << 0 <<"\n";
                else myfile << "T3" << "  " << "-" << "    " << 0 << "  " << tjet.pt() << " " << tjet.eta() << "    " << tjet.phi() << "    " << tjet.energy() << "  " << 0 <<"\n";
            }
            myfile << "\n";
            myfile.close();
            cout << "Event: " << EventNumber_truth << " printed to file: " << name << endl;
            break;
        }
    }

    //Finalize
    recoFile->Close();

    return 0;

}//end

