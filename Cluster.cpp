#include "Cluster.hpp"

#define _USE_MATH_DEFINES

Cluster::Cluster(double _g0, double _a, double _b, double _c, double _D0, double _kT, double _h, int _NTr)
				: g0(_g0), a(_a), b(_b), c(_c), D0(_D0), kT(_kT), h(_h), NTr(_NTr)
{
	g.push_back (g0);
	g_i = vector<double> (NTr, g.back());
	var.push_back (0.0);
	speed.push_back (0.0);

	phi=dphi=d2phi=D=dD=d2D = 0.0;
	g_k = 0.0;
	q = 1.0;
}

Cluster::~Cluster() {
	g.clear();
	g_i.clear();
	var.clear();
	speed.clear();
}

// ! походу неверно пересчитываются функционал-коэффициенты (они должны зависеть от функции распределения по размерам
// ! (мат ожидание размера на предыдущем шаге), а не от текущего размера)

double Cluster::f_phi() { return -a*g_k + b*pow(g_k,2.0/3)*(1-pow(g_k,-1.0/3)/3) + c*pow(g_k,-1.0/3); }
double Cluster::f_dphi() { return -a + b*(2.0/3*pow(g_k,-1.0/3)-1.0/9*pow(g_k,-2.0/3)) - c*(-1.0/3*pow(g_k,-4.0/3)); }
double Cluster::f_d2phi() { return b*(-2.0/27*pow(g_k,-5.0/3)-2.0/9*pow(g_k,-4.0/3)) + c*(4.0/9*pow(g_k,-7.0/3)); }
double Cluster::f_D() { return D0*pow(g_k,2.0/3); }
double Cluster::f_dD() { return D0*(2.0/3*pow(g_k,-1.0/3)); }
double Cluster::f_d2D() { return -D0*(2.0/9*pow(g_k,-4.0/3)); }

double Cluster::f_H() { return -1.0/kT*(dphi*D) - 1.0/2*dD; }
double Cluster::f_dH() { return -1.0/kT*(d2phi*D+dphi*dD) - 1.0/2*d2D; }
double Cluster::f_sigma() { return sqrt((2.0/q)*D); }
double Cluster::f_dsigma() { return 1.0/sqrt((2.0/q)*D)*dD/q; }

double Cluster::ksi() {
	random_device rd;
    	mt19937 gen(rd());
    	uniform_real_distribution<> dis(0, 1);
	return sqrt(-2*log(dis(gen)))*cos(2*M_PI*dis(gen));
}

void Cluster::change_coefs() {
	g_k = g.back();

	phi = f_phi();
	dphi = f_dphi();
	d2phi = f_d2phi();
	D = f_D();
	dD = f_dD();
	d2D = f_d2D();
}

void Cluster::skachek() {
	g_i.clear();
	double g_new = 0.0;
	double Eg = 0.0;
	g_k = g.back();
	for(int i=0; i<NTr; i++) {
		g_new = g_k + 1.0/(1.0-h/2*f_dH())*(h*f_H()+sqrt(h)*f_sigma()*ksi()+h/2*f_dsigma()*f_sigma()*ksi());
		g_i.push_back (g_new);
		if ((g_i.back()>300) && (i%15==0)) {
			double temp = g_i.back();
			g_i.pop_back();
			g_i.push_back(temp/2);
			g_i.push_back(temp/2);
		}
	}
	Eg = accumulate(g_i.begin(), g_i.end(), 0.0)/g_i.size();
	speed.push_back (Eg-g.back());
	g.push_back (Eg);

	vector<double> Eg_sq;
	for(unsigned int j=0; j<g_i.size(); j++)
		Eg_sq.push_back (pow(Eg-g_i[j],2));
		var.push_back (accumulate(Eg_sq.begin(),Eg_sq.end(),0.0)/NTr);
	}

void Cluster::traectories() {
	g_i.clear();
	double g_new = 0.0;

	double Eg = 0.0;

	g_k = g.back();
	for(int i=0; i<NTr; i++) {
		g_new = g_k + 1.0/(1.0-h/2*f_dH())*(h*f_H()+sqrt(h)*f_sigma()*ksi()+h/2*f_dsigma()*f_sigma()*ksi());
		g_i.push_back (g_new);
	}
	Eg = accumulate(g_i.begin(), g_i.end(), 0.0)/NTr;
	speed.push_back (Eg-g.back());
	g.push_back (Eg);

	vector<double> Eg_sq;
	for(unsigned int j=0; j<g_i.size(); j++)
		Eg_sq.push_back (pow(Eg-g_i[j],2));
	var.push_back (accumulate(Eg_sq.begin(),Eg_sq.end(),0.0)/NTr);
}


void Cluster::euler() {
	g_i.clear();
        double g_new = 0.0;

        double Eg = 0.0;

        g_k = g.back();
        for(int i=0; i<NTr; i++) {
                g_new = g_k + h*f_H() + sqrt(h)*f_sigma()*ksi() ;
                g_i.push_back (g_new);
        }
        Eg = accumulate(g_i.begin(), g_i.end(), 0.0)/NTr;
        speed.push_back (Eg-g.back());
        g.push_back (Eg);

        vector<double> Eg_sq;
        for(unsigned int j=0; j<g_i.size(); j++)
                Eg_sq.push_back (pow(Eg-g_i[j],2));
        var.push_back (accumulate(Eg_sq.begin(),Eg_sq.end(),0.0)/NTr);

}

void Cluster::evol_dump(const char* name, double t, int bins, double g_min, double g_max) {
	ofstream file;
	file.open(name, ios::out|ios::app);

	double g_step = (g_max-g_min)/bins;

	for(int i=0; i<bins; i++) {
		file << t << " " << g_min + g_step*i << " " << hist(g_i,bins,g_min,g_max)[i] << "\n";
	}
	file << "\n";

	file.close();
}

vector<double> Cluster::hist(vector<double> gi, int bins, double g_min, double g_max) {
	double g_step = (g_max-g_min)/bins;

	vector<double> hist_arr (bins,0);

	for(int k=0; k<bins; k++) {
		for(int i=0; i<NTr; i++) {
			if ( (gi[i]>(g_min + g_step*k)) && (gi[i]<=(g_min + g_step*(k+1))) )
				hist_arr[k]+=1.0/gi.size();
		}
	}

	return hist_arr;
	hist_arr.clear();
}
