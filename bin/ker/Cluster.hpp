#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

class Cluster {
	public:
		Cluster(
			double _g0,
			double _a,
			double _b,
			double _c,
			double _D0,
			double _kT,
			double _h,
			int _NTr
		);

		~Cluster();

		double f_phi();
		double f_dphi();
		double f_d2phi();
		double f_D();
		double f_dD();
		double f_d2D();

		double f_H();
		double f_dH();
		double f_sigma();
		double f_dsigma();
		double ksi();

		void change_coefs();
		void traectories();
		void leap();

		// схема эйлера для сравнения устойчивости
		void euler();

		void data_dump(const char* name, double t, int bins, double g_min, double g_max);
		vector<double> hist(vector<double> g, int bins, double g_min, double g_max);


//	private:
		// переменные, в которых будут храниться значения функционал-коэффициентов
		double phi, dphi, d2phi;
		double D, dD, d2D;
		vector<double> g;

		// вектор для хранения переменных для построения эволюции функции распределения
		vector<double> g_i;
		vector<double> var;
		vector<double> speed;

		// коэффициенты для диффузии и Гиббса
		double g0, a, b, c, D0, kT;
		// текущее значение g; шаг по времени; кол-во траекторий; интенсивность белого шума
		double g_k, h, NTr, q;
};