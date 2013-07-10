/// MACRO FOR ANALYSING RECO AND TRUTH JETS
/// Note that everything is stored in MeV in the data files,
/// but I convert to GeV when building my four momentums --
/// I'll come up with a nicer way to do this later.

#include <Utils.hh>
#include <FourMomentum.hh>
#include <Selectors.hh>
#include <fstream>
#include <string.h>

using namespace std;
using namespace Analysis;

int main(int argc,char *argv[]) {


//Retrieve the TTrees


    TFile *recoFile = new TFile( "/afs/phas.gla.ac.uk/user/k/knordstrom/data/ttbar_bb.root", "READ" );
    TTree *recoTree = (TTree*)recoFile->Get("mini");

    TChain *truthTree = new TChain("truth");
    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._*.pool.root");
//    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.01147152._*.pool.root");

//    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.01147152._000601.pool.root");
//    truthTree->Add("/afs/phas.gla.ac.uk/user/k/knordstrom/data/truth/ntup/EVNT.00983679._000001.pool.root");

    TFile *outputFile = new TFile("~/output/comparedata.root", "RECREATE");
    outputFile->cd(); // WHY IS THIS NEEDED ROOT IS SO CONFUSING

//Set up the addresses for reading out the branches

    UInt_t EventNumber_truth;

    UInt_t jet_n;

    Int_t  EventNumber_reco;

    Float_t jet_pt_reco[40], jet_eta_reco[40], jet_phi_reco[40], jet_E_reco[40];
    Float_t lep_pt_reco, lep_eta_reco, lep_phi_reco, lep_E_reco;

    vector<float> *jet_pt_truth = 0, *jet_eta_truth = 0, *jet_phi_truth = 0, *jet_E_truth = 0;
    vector<float> *part_pt_truth = 0, *part_eta_truth = 0, *part_phi_truth = 0, *part_E_truth = 0;
    vector<int> *pdgId_truth = 0, *status_truth = 0;

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

// Declare histos

    TH1D *pt_truth = new TH1D("pt_truth", "pT for all truth jets", 50, 0, 250);
    TH1D *pt_reco = new TH1D("pt_reco", "pT for all reco jets", 50, 0, 250);
    TH1D *eta_truth = new TH1D("eta_truth", "eta for all truth jets", 30, -3, 3);
    TH1D *eta_reco = new TH1D("eta_reco", "eta for all reco jets", 30, -3, 3);

    TH1D *pt_truth_1 = new TH1D("pt_truth_1", "pT for leading truth jet", 50, 0, 250);
    TH1D *pt_reco_1 = new TH1D("pt_reco_1", "pT for leading reco jet", 50, 0, 250);
    TH1D *eta_truth_1 = new TH1D("eta_truth_1", "eta for leading truth jet", 30, -3, 3);
    TH1D *eta_reco_1 = new TH1D("eta_reco_1", "eta for leading reco jet", 30, -3, 3);

    TH1D *pt_truth_2 = new TH1D("pt_truth_2", "pT for subleading truth jet", 50, 0, 250);
    TH1D *pt_reco_2 = new TH1D("pt_reco_2", "pT for subleading reco jet", 50, 0, 250);
    TH1D *eta_truth_2 = new TH1D("eta_truth_2", "eta for subleading truth jet", 30, -3, 3);
    TH1D *eta_reco_2 = new TH1D("eta_reco_2", "eta for subleading reco jet", 30, -3, 3);

    TH1D *ht_truth = new TH1D("ht_truth", "HT for all truth jets", 50, 0, 1500);
    TH1D *ht_reco = new TH1D("ht_reco", "HT for all reco jets", 50, 0, 1500);
    TH1D *n_truth = new TH1D("n_truth", "n_{jet}^{truth}", 10, 0, 10);
    TH1D *n_reco = new TH1D("n_reco", "n_{jet}^{reco}", 10, 0, 10);

    TH1D *dpT_jet = new TH1D("dpT_jet", "dpT_jet (positive is reco is greater)", 50, -50, 50);
    TH1D *dR_jet = new TH1D("dR_jet", "dR_jet", 30, 0, 0.3);
    TH1D *dpT_jet_ave = new TH1D("dpT_jet_ave", "Average dPt as function of truth pT", 50, 0, 250);
    TH1D *dpT_jet_ave_new = new TH1D("dpT_jet_ave_new", "Average dPt as function of truth pT", 50, 0, 250);
    TH1D *dpT_jet_ave_entries = new TH1D("dpT_jet_ave_entries", "Average dPt as function of truth pT", 50, 0, 250);

    TH1D *dR_jet_closest = new TH1D("dR_jet_closest", "dR to closest in dR (unmatched)", 10, 0, 1.0);
    TH1D *dpT_jet_closest = new TH1D("dpT_jet_closest", "dpT to closest in dR (unmatched)", 25, 0, 250);
    TH1D *pT_unmatched_reco = new TH1D("pT_unmatched_reco", "pT unmatched reco", 25, 0, 500);
    TH1D *pT_unmatched_truth = new TH1D("pT_unmatched_truth", "pT unmatched truth", 25, 0, 500);

    TH1D *n_matches_truth = new TH1D("n_matches_truth", "number of reco jets matched to truth jet", 5, 0, 5);
    TH1D *n_matches_truth_real = new TH1D("n_matches_truth_real", "number of reco jets matched to truth jet", 5, 0, 5);
    TH1D *n_matches_reco = new TH1D("n_matches_reco", "number of truth jets matched to reco jet", 5, 0, 5);
    TH2D *pT_truth_vs_reco = new TH2D("pT_truth_vs_reco", "pT_truth_vs_reco (x axis is truth)", 50, 0, 250, 50, 0, 250);
    TH2D *unmatched_dpt_vs_dr = new TH2D("unmatched_dpt_vs_dr", "unmatched dR versus dPt to closest in dR", 20, 0, 1.0, 20, 0, 50);
    TH2D *dpt_vs_dr = new TH2D("dpt_vs_dr", "dR versus dPt to closest in dR", 20, 0, 1.0, 20, 0, 50);

// Helpers for histograms
    AverageHisto average_dpt(dpT_jet_ave);

//Read out all events

    Int_t event_match = 0; //number of events matched by event number
    Int_t jet_match_local = 0; //reco -> truth jets matched in *this* event
    Int_t jet_match_total = 0; //reco -> truth jets matched in total
    Int_t jet_reco_total = 0; //total number of reco jets
    Int_t jet_truth_total = 0; //total number of truth jets
    Int_t truth_lax_pt = 0; // truth matched with lax pt condition
    Int_t truth_lax_dr = 0; // truth matched with lax dR condition

    /// This gives the conditions used for matching two jets
    Comparison MatchJets;
    MatchJets.deltaRWithin(0.2);
    MatchJets.ptWithin(15, 0.2);

    /// Check if within dR=0.2 of the selected reco lepton
    Comparison LeptonClose;
    LeptonClose.deltaRWithin(0.2);

    /// This is to find truth jets which pass all the cuts to consider them
    /// "matchable" but which can't be matched
    Selector notMatched;
    notMatched.pdgIdCut(Equal, 1);
    notMatched.statusCut(Equal, 0);

    /// This is to find truth jets which are considered matchable
    /// when combined with the LeptonCut above
    Selector TruthSelector;
    TruthSelector.ptCut(More, 25);
    TruthSelector.etaCut(Less, 2.5);
    TruthSelector.etaCut(More, -2.5);

    /// This gives the pT conditions used for matching two jets
    /// with lax dR condition
    Comparison MatchPt;
    MatchPt.ptWithin(15, 0.2);
    MatchPt.deltaRWithin(0.5);

    /// This gives the dR conditions used for matching two jets
    /// with lax pT condition
    Comparison MatchdR;
    MatchdR.deltaRWithin(0.2);
    MatchdR.ptWithin(50, 0.2);


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

            Int_t jet_truth_local = 0;
            Int_t jet_reco_local = 0;
            Int_t lax_pt_local = 0;
            Int_t lax_dr_local = 0;

            ofstream myfile;
            char name[80];

            if(argc > 1) {
                char number[10];
                sprintf(number, "%d", (int)EventNumber_truth);
                strcpy (name, argv[1]);
                strcat (name, number);
                myfile.open(name);
            }

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
            vector<FourMomentum> tparts = makeVectors((*part_pt_truth), (*part_eta_truth), (*part_phi_truth), (*part_E_truth), (*pdgId_truth), (*status_truth));
            vector<FourMomentum> tleps;

            /// Extracting leptons from truth particles, with correct phase space cuts
            /// (reducing the pT cut to 20 GeV to take detector effects + dressing into consideration)
            foreach(FourMomentum particle, tparts) {
                if( particle.pt() > 20 && abs(particle.pdgId()) > 11 && abs(particle.pdgId()) < 19 && particle.status() == 1 && abs(particle.eta() < 2.5) ) { //is a final state muon
                    tleps.push_back(particle);
                }
                if( particle.pt() > 20 && abs(particle.pdgId()) == 11 && particle.status() == 1 && ( (abs(particle.eta()) < 2.47 && abs(particle.eta()) > 1.52 ) || (abs(particle.eta()) < 1.37)) ) { // electron
                    tleps.push_back(particle);
                }
            }

            /// Dressing leptons
            foreach( FourMomentum &tlep, tleps) {
                foreach( FourMomentum photon, tparts ) {
                    if( abs(photon.pdgId()) == 22 && photon.status() == 1 && tlep.deltaR(photon) < 0.1 ) {
                        tlep += photon; //dress lepton with photon
                    }
                }
            }

            jet_match_local = 0;


            /// Only look at 6 leading reco jets. Ignored for now.
//            // Only interested in the 6 leading reco jets
//            if(reco_jets.size() > 6) {
//                reco_jets.erase(reco_jets.begin() + 6, reco_jets.end());
//            }

            ///At this point it gets a bit confusing:
            ///The pdgId (which is set to zero before this by the default constructor)
            ///is used to track the truth jets which "should" be matched to a reco jet
            /// -- their pdgId is set to 1. The status is used to track which truth
            ///jet have been matched to a reco jet (status is how many time they have been
            ///matched).

            foreach(FourMomentum &tjet, truth_jets) {
                bool pass = true;
                foreach(FourMomentum tlep, tleps) {
                    if(LeptonClose.pass(tjet, rlep)) pass = false;
                }
                if(TruthSelector.pass(tjet) && pass) {
                    jet_truth_total++;   // only count these truth jets
                    jet_truth_local++;
                    tjet.setPdgId(1); // as above
                }
            }

            /// Plot pt, eta for all, leading, and subleading jets
            /// and n_jet

            Int_t leading = 0;

            foreach(FourMomentum tjet, truth_jets) {
                if(tjet.pdgId() != 1)continue;
                if(leading == 1) {pt_truth_2->Fill(tjet.pt()); eta_truth_2->Fill(tjet.eta()); leading++;}
                if(leading == 0) {pt_truth_1->Fill(tjet.pt()); eta_truth_1->Fill(tjet.eta()); leading++;}
                pt_truth->Fill(tjet.pt());
                eta_truth->Fill(tjet.eta());
            }

            leading = 0;

            foreach(FourMomentum rjet, reco_jets) {
                if(leading == 1) {pt_reco_2->Fill(rjet.pt()); eta_reco_2->Fill(rjet.eta()); leading++;}
                if(leading == 0) {pt_reco_1->Fill(rjet.pt()); eta_reco_1->Fill(rjet.eta()); leading++;}
                pt_reco->Fill(rjet.pt());
                eta_reco->Fill(rjet.eta());
            }


            n_truth->Fill(jet_truth_local);
            n_reco->Fill(reco_jets.size());

            /// Get & fill HT (scalar sum of pT)
            double truth_ht = 0;
            double reco_ht = 0;

            foreach(FourMomentum tjet, truth_jets) {
                if(tjet.pdgId() == 1) truth_ht += tjet.pt();
            }
            foreach(FourMomentum rjet, reco_jets) {
                reco_ht += rjet.pt();
            }

            ht_truth->Fill(truth_ht);
            ht_reco->Fill(reco_ht);

            /// Need to iterate by reference all the time to be able to change the values
            foreach(FourMomentum &rjet, reco_jets) {
                foreach(FourMomentum &tjet, truth_jets) {
                     if( MatchJets.pass(tjet, rjet) ) { //matched!
                        tjet.setStatus(tjet.status() + 1);
                        rjet.setStatus(rjet.status() + 1);
                        jet_match_local++;
                        dpT_jet->Fill(rjet.deltaPt(tjet));
                        dR_jet->Fill(tjet.deltaR(rjet));
                        average_dpt.add(tjet.pt(), abs(rjet.deltaPt(tjet)));
                        dpT_jet_ave_new->Fill(tjet.pt(), abs(rjet.deltaPt(tjet)));
                        dpT_jet_ave_entries->Fill(tjet.pt(), 1);
                        pT_truth_vs_reco->Fill(tjet.pt(), rjet.pt());
                        break;
                    }
                }
            }

            vector <double> dRs;

            foreach(FourMomentum tjet, truth_jets) {
                dRs.clear();
                foreach(FourMomentum rjet, reco_jets) {
                    dRs.push_back(tjet.deltaR(rjet));
                }
                if(dRs.size() == 0)continue;
                sort(dRs.begin(), dRs.end());

                foreach(FourMomentum rjet, reco_jets) {
                    if(areSimilar(dRs[0], tjet.deltaR(rjet), 0.000000001, 0.) && tjet.pdgId() == 1) {
                        dpt_vs_dr->Fill(tjet.deltaR(rjet), abs(tjet.deltaPt(rjet)));
                        break;
                    }
                }

                n_matches_truth->Fill(tjet.status());
                if(tjet.pdgId() == 1)n_matches_truth_real->Fill(tjet.status());
                bool tmp = true;
                if(notMatched.pass(tjet)) {
                    foreach(FourMomentum rjet, reco_jets) {
                        if(MatchPt.pass(tjet, rjet) && tmp){truth_lax_dr++; lax_dr_local++; tmp = false;}
                        if(MatchdR.pass(tjet, rjet) && tmp){truth_lax_pt++; lax_pt_local++; tmp = false;}
                        if(areSimilar(dRs[0], tjet.deltaR(rjet), 0.000000001, 0.)) {
                            dR_jet_closest->Fill(tjet.deltaR(rjet));
                            dpT_jet_closest->Fill(rjet.deltaPt(tjet));
                            pT_unmatched_truth->Fill(tjet.pt());
                            unmatched_dpt_vs_dr->Fill(tjet.deltaR(rjet), abs(tjet.deltaPt(rjet)));
                        }
                    }
                }
            }

            foreach(FourMomentum rjet, reco_jets) {
                n_matches_reco->Fill(rjet.status());
                if(rjet.status() == 0) {
                    pT_unmatched_reco->Fill(rjet.pt());
                }
            }


            jet_match_total += jet_match_local;
            jet_reco_total += reco_jets.size();
            jet_reco_local += reco_jets.size();

            //cout << "Jets matched in this event: " << jet_match_local << "/" << reco_jets.size() << endl;

            if(argc > 1) {
                myfile << "Total number of truth jets which pass all criteria to be matched: " << jet_truth_local << "<br>" << "\n";
                myfile << "Total number of reco jets: " << jet_reco_local << "<br>" << "\n";
                myfile << "Total number of matches: " << jet_match_local << "<br>" << "\n";
                myfile << "Number of extra matches with lax dR condition: " << lax_dr_local << "<br>" << "\n";
                myfile << "Number of extra matches with lax pT condition: " << lax_pt_local << "<br>" << "\n";

                myfile << "\n";
                myfile.close();
                cout << "Event: " << EventNumber_truth << " printed to file: " << name << endl;
            }

            break;

        }
    }

    average_dpt.fillHisto();

    cout << "Number of matching events: " << event_match << endl;
    cout << "Number of total jets matched (reco -> truth): " << jet_match_total << "/" << jet_reco_total << endl;
    cout << "(Fraction: " << (float)jet_match_total/jet_reco_total  << " )" << endl;
    cout << "Number of total jets matched (truth -> reco): " << jet_match_total << "/" << jet_truth_total << endl;
    cout << "(Fraction: " << (float)jet_match_total/jet_truth_total  << " )" << endl;
    cout << "Number of truth jets failed to match: " << jet_truth_total-jet_match_total << endl;
    cout << "Number of truth failed truth jets matched with lax pT condition: " << truth_lax_pt << " (Fraction: " << (float)truth_lax_pt/(jet_truth_total-jet_match_total) << ")" << endl;
    cout << "Number of truth failed truth jets matched with lax dR condition: " << truth_lax_dr << " (Fraction: " << (float)truth_lax_dr/(jet_truth_total-jet_match_total) << ")" << endl;


    //Finalize


    recoFile->Close();

    outputFile->Write();
    outputFile->Close();

    return 0;


}//end
