/*
   Simple event visualiser using Rivet (http://rivet.hepforge.org/).
   Needs two arguments - first is the input file, second is the plotting instructions.

   Input format is:
   [Stage (T1-3, R)]	[Particle (latex code accepted)] 	[Colour (0,1,2,3 - represents mother particle)] [pT]  [Eta]  [Phi]  [E]  [usedneutrino]

   Example particle:
   T2	$\bar{b}$	2	120	-1.9	2.5	160	0
   Use GeV for E and pT.

   Accepted plotting instructions are:
   T1, T2, T3 - only truth level one, two, or three. R - only reconstructed. TR - truth level three and reconstructed.

   Colour conventions are:
   0: Green
   1: Blue
   2: Red
   3: Black

   To use, redirect output into the file of your choice and then run
   make-plots (yourfilename)
   to generate a pdf file with your event.

   Sorry for the horrible coding, fast development was my only goal when writing this.
   Karl Nordstrom 2012.
*/



/*  COMPILING INSTRUCTIONS

Since I now use some ROOT stuff for my Lorentz vectors you will have to tell your compiler where to find ROOT (well, usually). Thankfully ROOT comes equipped with a small script called 'root-config' which will tell us everything we need to know. The following instructions work for me personally (on Ubuntu 12.04 and GCC 4.6.3):

g++ -c plotter.c `root-config --cflags`
g++ -o plotter plotter.o `root-config --libs`

I've heard rumours that the placement on the `root-config --libs` statement might depend on your GCC version, and older versions would prefer

g++ -o plotter `root-config --libs` plotter.o

but I have not tested this personally. Hopefully this helps!

*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <Math/Vector4D.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVector2.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TROOT.h>



void createplot(int type);
void createtags();
void createcircles();
void createcrosses();
const float pi = 3.141592;
float scale = 1;
bool truth = true;
bool check0 = false, check1 = false, check2 = false, check3 = false;
bool check4 = false, check5 = false, check6 = false, check7 = false, check8 = false, check9 = false, check10 = false, check11 = false;
int compare;
int quantity; // pT = 0, E = 1, ...
std::vector<std::string> particle;
std::vector<int> stage;
std::vector<int> color;
std::vector<float> E;
std::vector<float> eta;
std::vector<float> phi;
std::vector<float> pT;
std::vector<int> isplotted;
std::vector<int> neutrinochosen;
std::vector<TLorentzVector> lorentz;
std::string plots = "circles crosses neutrino tags dummy lines"; //note the order of these is important (drawing order)



int main(int argc, char *argv[]) {

	//check arguments are correct

	if(argc < 2) {

		std::cout << "Needs an input file (arg1) and plotting instructions (arg2)." << std::endl;
		std::cout << "Accepted plotting instructions are:" << std::endl;
		std::cout << "T1: truth level one." << std::endl;
		std::cout << "T2: truth level two." << std::endl;
		std::cout << "T3: truth level three." << std::endl;
		std::cout << "R: reconstructed." << std::endl;
		std::cout << "RT: reconstructed + truth level three (truth in dashed)." << std::endl;
		return(1);

	}

	if(argc > 4) {

		std::cout << "Give only one input file (arg1), one plotting instruction (arg2), and if needed the quantity to plot (arg3)." << std::endl;
		std::cout << "Accepted plotting instructions are:" << std::endl;
		std::cout << "T1: truth level one." << std::endl;
		std::cout << "T2: truth level two." << std::endl;
		std::cout << "T3: truth level three." << std::endl;
		std::cout << "R: reconstructed." << std::endl;
		std::cout << "RT: reconstructed + truth level three (truth in dashed)." << std::endl;
		return(1);

	}

	if(argc == 3) {

		std::string argument = argv[2];

		if(argument == "T1")compare = 0;
		else if(argument == "T2")compare = 1;
		else if(argument == "T3")compare = 2;
		else if(argument == "R")compare = 3;
		else if(argument == "RT" || argument == "TR" || argument == "RT3" || argument == "T3R")compare = 4;
		else {  std::cout << "Accepted plotting instructions (arg 2) are:" << std::endl;
			std::cout << "T1: truth level one." << std::endl;
			std::cout << "T2: truth level two." << std::endl;
			std::cout << "T3: truth level three." << std::endl;
			std::cout << "R: reconstructed." << std::endl;
			std::cout << "RT: reconstructed + truth level three (reconstructed in dashed)." << std::endl;
			return(1);
		}

		quantity = 0;

	}

	if(argc == 4) {

		std::string argument = argv[2];

		if(argument == "T1")compare = 0;
		else if(argument == "T2")compare = 1;
		else if(argument == "T3")compare = 2;
		else if(argument == "R")compare = 3;
		else if(argument == "RT" || argument == "TR" || argument == "RT3" || argument == "T3R")compare = 4;
		else {  std::cout << "Accepted plotting instructions (arg 2) are:" << std::endl;
			std::cout << "T1: truth level one." << std::endl;
			std::cout << "T2: truth level two." << std::endl;
			std::cout << "T3: truth level three." << std::endl;
			std::cout << "R: reconstructed." << std::endl;
			std::cout << "RT: reconstructed + truth level three (reconstructed in dashed)." << std::endl;
			return(1);

		}

		std::string mode = argv[3];

		if(mode == "pt")quantity = 0;
		else if(mode == "PT")quantity = 0;
		else if(mode == "pT")quantity = 0;
		else if(mode == "Pt")quantity = 0;
		else if(mode == "E")quantity = 1;
		else if(mode == "e")quantity = 1;
		else if(mode == "p")quantity = 2;
		else if(mode == "P")quantity = 2;
		else if(mode == "lngamma")quantity = 3;
		else if(mode == "b")quantity = 4;
		else if(mode == "B")quantity = 4;
		else if(mode == "gamma")quantity = 5;
		else if(mode == "constant") quantity = 6;
		else {
			std::cout << "## Not supported quantity used. Defaulting to constant size." << std::endl;
			quantity = 6;
		}

	}


	//read input file into particle vectors

	std::ifstream datafile(argv[1]);

	while (!datafile.eof())
	{

		std::string line;
		getline(datafile,line);

		std::stringstream ss(line);

		float tempE, tempeta, tempphi, temppT;
		int tempcolor, tempstage, tempneutrino;
		std::string tempparticle, tempstage1;
		TLorentzVector templorentz;

		ss >> tempstage1;
		if(tempstage1 == "R")tempstage = 3;
		if(tempstage1 == "T1")tempstage = 0;
		if(tempstage1 == "T2")tempstage = 1;
		if(tempstage1 == "T3")tempstage = 2;
		if(tempstage1 == "")continue;
		ss >> tempparticle;
		ss >> tempcolor;
		ss >> temppT;
		ss >> tempeta;
		ss >> tempphi;
		ss >> tempE;
		ss >> tempneutrino;
		templorentz.SetPtEtaPhiE(temppT, tempeta, tempphi, tempE);

		//these lines are used for deciding what needs to be plotted

		if(tempcolor == 3 && compare == tempstage) check0 = true;
		if(tempcolor == 0 && compare == tempstage) check1 = true;
		if(tempcolor == 1 && compare == tempstage) check2 = true;
		if(tempcolor == 2 && compare == tempstage) check3 = true;

		if(compare == 4)
		{
		if(tempcolor == 0 && 2 == tempstage) check4 = true;
		if(tempcolor == 1 && 2 == tempstage) check5 = true;
		if(tempcolor == 2 && 2 == tempstage) check6 = true;
		if(tempcolor == 3 && 2 == tempstage) check10 = true;
		if(tempcolor == 0 && 3 == tempstage) check7 = true;
		if(tempcolor == 1 && 3 == tempstage) check8 = true;
		if(tempcolor == 2 && 3 == tempstage) check9 = true;
		if(tempcolor == 3 && 3 == tempstage) check11 = true;
		}

		eta.push_back(tempeta);
		phi.push_back(tempphi);
		E.push_back(tempE);
		pT.push_back(temppT);
		particle.push_back(tempparticle);
		stage.push_back(tempstage);
		color.push_back(tempcolor);
		isplotted.push_back(0);
		neutrinochosen.push_back(tempneutrino);
		lorentz.push_back(templorentz);

	}







	//	!!!!!!!!!!!!!!	CHANGE SCALE HERE	!!!!!!!!!!!!!!!!!!!!!








	int d = 0;
	while(d!= (int)lorentz.size()) {

		//had some problems with unphysical boost factors, this line was a semi-temporary solution (I think the problem was in the input file rather than in rounding errors in the end). Can probably be removed.
		while(lorentz[d].Beta() > 0.99999) lorentz[d].SetE(lorentz[d].E() + 0.00001);

		if(quantity == 0) {
			if(pT[d] > scale*1.5)scale = pT[d]/1.5;
		}

		if(quantity == 1) {
			if(E[d] > scale*2)scale = E[d]/2;
		}

		if(quantity == 2) {
			if(lorentz[d].P() > scale*2)scale = lorentz[d].P()/2;
		}

		if(quantity == 3) {
			if(log(lorentz[d].Gamma()) > scale)scale = log(lorentz[d].Gamma());
		}

		if(quantity == 4) {
			if(lorentz[d].Beta() < scale)scale = lorentz[d].Beta()-0.1;
		}
		if(quantity == 5) {
			if(lorentz[d].Gamma() > scale/2)scale = 2*lorentz[d].Gamma();
		}

		d++;
	}

	/*

	Generate the basic Rivet plot header. Feel free to change these manually, you can find instructions at
	http://rivet.hepforge.org/make-plots.html. Be aware that changing the x and y ranges will mess up the
	scale and is therefore not adviced.

	*/

	std::cout << "# BEGIN PLOT" << std::endl;
	std::cout << "TITLE=" << argv[1] << std::endl;
	std::cout << "Legend=1" << std::endl;
	std::cout << "LegendYPos=1" << std::endl;
	std::cout << "LegendXPos=0" << std::endl;
	std::cout << "LegendOnly=lepton1 lepton higgs1 higgs hadron1  hadron" << std::endl;
	std::cout << "YMin=-3.141592" << std::endl;
	std::cout << "YMax=3.141592" << std::endl;
	std::cout << "XMin=-4" << std::endl;
	std::cout << "XMax=4" << std::endl;
	std::cout << "LogY=0" << std::endl;
	std::cout << "DrawOnly=" << plots << std::endl;
	std::cout << "XLabel=$" << "\\" << "eta$" << std::endl;
	std::cout << "YLabel=$" << "\\" << "phi$" << std::endl;
	std::cout << "XMinorTickMarks=0" << std::endl;
	std::cout << "YMinorTickMarks=0" << std::endl;
	std::cout << "XMajorTickMarks=10" << std::endl;
	std::cout << "YMajorTickMarks=10" << std::endl;
	std::cout << "DrawSpecialFirst=1" << std::endl;
	std::cout << "ColorSeries={rgb}{last}[rgb]{1,0.97,0.94}[rgb]{0.6,0.0,0.05}" << std::endl;

	std::cout << "# END PLOT" << std::endl;
	std::cout << std::endl;

	//Lines for phi = 0 and eta = 0 and the legend.

	std::cout << "# BEGIN SPECIAL lines " << std::endl;
	std::cout << "\\" << "psclip{" << "\\" << "psframe[linewidth=0, linestyle=none](0,0)(1,1)}" << std::endl;
//	std::cout << "\\" << "psline[linewidth=0.4pt,linecolor=black,linestyle=dashed]" << "\\" << "physicscoor( -4,0)" << "\\" << "physicscoor( 4,0)" << std::endl;
//	std::cout << "\\" << "psline[linewidth=0.4pt,linecolor=black,linestyle=dashed]" << "\\" << "physicscoor( 0," << -pi << ")" << "\\" << "physicscoor( 0," << pi << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=0.4pt,linecolor=red,linestyle=dashed]" << "\\" << "physicscoor( 2.5," << -pi << ")" << "\\" << "physicscoor( 2.5," << pi << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=0.4pt,linecolor=red,linestyle=dashed]" << "\\" << "physicscoor( -2.5," << -pi << ")" << "\\" << "physicscoor( -2.5," << pi << ")" << std::endl;
	std::cout << "\\" << "fontsize{8pt}{8.1pt}" << "\\" << "selectfont" << std::endl;
	//std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.65 << "," << 2.5 << "){"<< "\\" << "textcolor{blue}{hadronic}}" << std::endl;
	//std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.75 << "," << 2.2 << "){"<< "\\" << "textcolor{green}{leptonic}}" << std::endl;
	//std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -3 << "," << 1.9 << "){"<< "\\" << "textcolor{red}{higgs}}" << std::endl;
	std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.65 << "," << 2.5 << "){+ = truth}" << std::endl;
	if(quantity == 0) {
		std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.9 << "," << 2.8 << "){$" << "\\" << "sim$ $p_T$}" << std::endl;
	}
	if(quantity == 1) {
		std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.9 << "," << 2.8 << "){$" << "\\" << "sim$ E}" << std::endl;
	}
	if(quantity == 2) {
		std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.9 << "," << 2.8 << "){$" << "\\" << "sim$ P}" << std::endl;
	}
	if(quantity == 3) {
		std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.9 << "," << 2.8 << "){$" << "\\" << "sim$ $" << "\\" << "ln" << "\\" << "gamma$}" << std::endl;
	}
	if(quantity == 4) {
		std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.9 << "," << 2.8 << "){$" << "\\" << "sim$ $" << "\\" << "beta$}" << std::endl;
	}
	if(quantity == 5) {
		std::cout << "\\" << "uput{4pt}[180]{0}" << "\\" << "physicscoor(" << -2.9 << "," << 2.8 << "){$" << "\\" << "sim$ $" << "\\" << "gamma$}" << std::endl;
	}
	std::cout << "\\" << "endpsclip" << std::endl;
	std::cout << "# END SPECIAL" << std::endl;
	std::cout << std::endl;

	//Generate the "histograms" for the particles.

	if(compare != 4)
	{
	if(compare == 3)createcircles();
	else if(check0 || check1 || check2 || check3)createcrosses();
	}

	else
	{
	if(check7 || check8 || check9 || check11)createcircles();
	compare = 2;
	if(check4 || check5 || check6 || check10)createcrosses();
	}

	//Name boxes.

	createtags();

	//Tag neutrino/whatever else needs to be tagged.
	int j;

	bool plot = false;

    for(j = 0; j < (int)E.size(); ++j) {
		if(isplotted[j] == 1 && neutrinochosen[j] == 1)plot = true;
    }

    if(plot) {
    std::cout << std::endl;
    std::cout << "# BEGIN SPECIAL neutrino" << std::endl;
    std::cout << "\\" << "psclip{" << "\\" << "psframe[linewidth=0, linestyle=none](0,0)(1,1)}" << std::endl;

	for(j = 0; j < (int)E.size(); ++j) {
		if(isplotted[j] == 1 && neutrinochosen[j] == 1){
		std::cout << "\\" << "psdots[dotstyle=diamond,dotscale=1.5]" << "\\" << "physicscoor(" << eta[j] << "," << phi[j] << ")" << std::endl;
		}
    }
    std::cout << "\\" << "endpsclip" << std::endl;
    std::cout << "# END SPECIAL" << std::endl;
    std::cout << std::endl;
	}

	//Create dummy (needed for rivet to accept the file)

	std::cout << std::endl;
	std::cout << "# BEGIN HISTOGRAM dummy" << std::endl;
	std::cout << "LineColor=black" << std::endl;
	std::cout << "LineWidth=0pt" << std::endl;
	std::cout << "LineOpacity=0" << std::endl;
	std::cout << "LineStyle=none" << std::endl;
	std::cout << "ErrorBars=0" << std::endl;
	std::cout << "PolyMarker=" << std::endl;
	std::cout << "Title=dummy" << std::endl;
	std::cout << "0 0 0 0" << std::endl;

	return(0);

	}

void createtags() {

	//Generate pstricks code for Rivet to create name boxes for the particles.

	std::string special = "tags";


	std::cout << "# BEGIN SPECIAL " << special << std::endl;
	int i = 0;
	std::cout << "\\" << "psclip{" << "\\" << "psframe[linewidth=0, linestyle=none](0,0)(1,1)}" << std::endl;
	std::cout << "\\" << "fontsize{8pt}{8.1pt}" << "\\" << "selectfont" << std::endl;
	while(i != (int)E.size())
	{
	if(phi[i] > -pi+0.3 && isplotted[i] == 1 && stage[i] != 3)std::cout << "\\" << "rput[Bl]" << "\\" << "physicscoor(" << eta[i] + 0.03 << "," << phi[i] - 0.3 << "){" << particle[i] << "}" << std::endl;
	else if (isplotted[i] == 1 && stage[i] != 3)std::cout << "\\" << "rput[Bl]" << "\\" << "physicscoor(" << eta[i] + 0.03 << "," << phi[i] + 0.15 << "){" << particle[i] << "}" << std::endl;
	else if (isplotted[i] == 1 && stage[i] == 3)std::cout << "\\" << "rput[Br]" << "\\" << "physicscoor(" << eta[i] - 0.03 << "," << phi[i] + 0.15 << "){" << particle[i] << "}" << std::endl;
	i++;
	}
	std::cout << "\\" << "endpsclip" << std::endl;

	std::cout << "# END SPECIAL" << std::endl;

	}

void createcircles() {

	//Generate pstricks code for Rivet to create circles for reconstructed.

	std::string special = "circles";


	std::cout << "# BEGIN SPECIAL " << special << std::endl;
	int i = 0;
	std::string colour;
	std::cout << "\\" << "psclip{" << "\\" << "psframe[linewidth=0, linestyle=none](0,0)(1,1)}" << std::endl;
	std::cout << "\\" << "fontsize{8pt}{8.1pt}" << "\\" << "selectfont" << std::endl;
	while(i != (int)E.size())
	{
	if(color[i] == 0)colour = "green";
	if(color[i] == 1)colour = "blue";
	if(color[i] == 2)colour = "red";
	if(color[i] == 3)colour = "black";

	float opac = 0.5;
	if(colour == "black")opac = 0.25;

	if(stage[i] == 3 && quantity == 0){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << pT[i]/scale << "}" << std::endl;
	isplotted[i] = 1; }
	else if(stage[i] == 3 && quantity == 1){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << E[i]/scale << "}" << std::endl;
	isplotted[i] = 1; }
	else if(stage[i] == 3 && quantity == 2){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << lorentz[i].P()/scale << "}" << std::endl;
	isplotted[i] = 1; }
	else if(stage[i] == 3 && quantity == 3){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << log(lorentz[i].Gamma())/scale << "}" << std::endl;
	isplotted[i] = 1; }
	else if(stage[i] == 3 && quantity == 4){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << lorentz[i].Beta()-scale << "}" << std::endl;
	isplotted[i] = 1; }
	else if(stage[i] == 3 && quantity == 5){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << lorentz[i].Gamma()/scale << "}" << std::endl;
	isplotted[i] = 1; }
	else if(stage[i] == 3 && quantity == 6){std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << 0.4 << "}" << std::endl;
                                            std::cout << "\\" << "pscircle[linestyle=none,fillstyle=solid,fillcolor=" << colour << ",opacity=" << opac << "]" << "\\" << "physicscoor(" << eta[i] << "," << phi[i] << "){" << 0.2 << "}" << std::endl;
	isplotted[i] = 1; }
	i++;
	}
	std::cout << "\\" << "endpsclip" << std::endl;

	std::cout << "# END SPECIAL" << std::endl;
	std::cout << std::endl;

	}

void createcrosses() {

	//Generate pstricks code for Rivet to create crosses for truth.

	std::string special = "crosses";


	std::cout << "# BEGIN SPECIAL " << special << std::endl;
	int i = 0;
	std::string colour;
	std::cout << "\\" << "psclip{" << "\\" << "psframe[linewidth=0, linestyle=none](0,0)(1,1)}" << std::endl;
	std::cout << "\\" << "fontsize{8pt}{8.1pt}" << "\\" << "selectfont" << std::endl;
	while(i != (int)E.size())
	{
	if(color[i] == 0)colour = "green";
	if(color[i] == 1)colour = "blue";
	if(color[i] == 2)colour = "red";
	if(color[i] == 3)colour = "black";
	if(stage[i] == compare && quantity == 1){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-E[i]/scale << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+E[i]/scale << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-E[i]/(0.8*scale) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+E[i]/(0.8*scale) << ")" << std::endl;
	isplotted[i] = 1; }
	if(stage[i] == compare && quantity == 0){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-pT[i]/scale << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+pT[i]/scale << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-pT[i]/(0.8*scale) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+pT[i]/(0.8*scale) << ")" << std::endl;
	isplotted[i] = 1; }
	if(stage[i] == compare && quantity == 2){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-lorentz[i].P()/scale << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+lorentz[i].P()/scale << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-lorentz[i].P()/(0.8*scale) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+lorentz[i].P()/(0.8*scale) << ")" << std::endl;
	isplotted[i] = 1; }
	if(stage[i] == compare && quantity == 3){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-log(lorentz[i].Gamma())/scale << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+log(lorentz[i].Gamma())/scale << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-log(lorentz[i].Gamma())/(0.8*scale) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+log(lorentz[i].Gamma())/(0.8*scale) << ")" << std::endl;
	isplotted[i] = 1; }
	if(stage[i] == compare && quantity == 4){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-(lorentz[i].Beta()-scale) << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+(lorentz[i].Beta()-scale) << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-(lorentz[i].Beta()-(0.8*scale)) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+(lorentz[i].Beta()-(0.8*scale)) << ")" << std::endl;
	isplotted[i] = 1; }
	if(stage[i] == compare && quantity == 5){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-lorentz[i].Gamma()/scale << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+lorentz[i].Gamma()/scale << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-lorentz[i].Gamma()/(0.8*scale) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+lorentz[i].Gamma()/(0.8*scale) << ")" << std::endl;
	isplotted[i] = 1; }
    if(stage[i] == compare && quantity == 6){std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i]-0.2 << "," << phi[i] << ")" << "\\" << "physicscoor( " << eta[i]+0.2 << "," << phi[i] << ")" << std::endl;
	std::cout << "\\" << "psline[linewidth=1pt,linecolor=" << colour << ",linestyle=dashed]" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]-(0.2/0.8) << ")" << "\\" << "physicscoor( " << eta[i] << "," << phi[i]+(0.2/0.8) << ")" << std::endl;
	isplotted[i] = 1; }
	i++;
	}
	std::cout << "\\" << "endpsclip" << std::endl;

	std::cout << "# END SPECIAL" << std::endl;
	std::cout << std::endl;

	}
