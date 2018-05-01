#include "PID.h"
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	prev_cte = 0;
	differential_cte = 0.0;
	integral_cte = 0.0;
	best_error = 10000000.0;
	curr_error = 0.0;
	frame_count = 0;
	param_index = 0;
	
	for( int i=0; i<3; ++i ) {
		params.push_back(0.0);
		delta_params.push_back(1.0);
		twiddle_stage.push_back(0);
	}
}

void PID::UpdateError(double cte) {
	differential_cte = cte - prev_cte;
	integral_cte += cte;
	
    std::cout << "\n\n==================================================" << "\n";
    std::cout << ", cte: " << cte;
    std::cout << ", prev_cte: " << prev_cte;
    std::cout << ", differential_cte: " << differential_cte;
    std::cout << ", integral_cte: " << integral_cte;
    std::cout << "\n--------------------------------------------------" << "\n";
	
	
	prev_cte = cte;
	
	p_error = (-Kp * cte);
	i_error = (-Ki * integral_cte);
	d_error = (-Kd * differential_cte);
	
	frame_count++;
	int N = 243;
	if ( frame_count % N == 0 ) {
		curr_error = 0.0;
	}
	curr_error += cte * cte / ((frame_count % N) + 1);
	
	
	
    std::cout << ", p_error: " << p_error;
    std::cout << ", i_error: " << i_error;
    std::cout << ", d_error: " << d_error;
    std::cout << ", frame_count: " << frame_count;
    std::cout << ", param_index: " << param_index;
    std::cout << ", curr_error: " << curr_error;
    std::cout << ", best_error: " << best_error;
    std::cout << "\n--------------------------------------------------" << "\n";
	
	int i = param_index;
	if ( twiddle_stage[i] == 0 ) {
		params[i] += delta_params[i];
		twiddle_stage[i] = 1;
	}
	else if ( twiddle_stage[i] == 1 ) {
		if( curr_error < best_error ) {
			best_error = curr_error;
			delta_params[i] *= 1.1;
			twiddle_stage[i] = 0;
			param_index = ++param_index % 3;
		}
		else {
			params[i] -= 2 * delta_params[i];
			twiddle_stage[i] = 2;
		}
	}
	else {
		if( curr_error < best_error ) {
			best_error = curr_error;
			delta_params[i] *= 1.1;
			twiddle_stage[i] = 0;
			param_index = ++param_index % 3;
		}
		else {
			params[i] += delta_params[i];
			delta_params[i] *= 0.9;
			twiddle_stage[i] = 0;
			param_index = ++param_index % 3;
		}
	}
	std::cout << "\n, i: " << i;
	std::cout << "\n, params: " << params[i];
	std::cout << "\n, delta_params: " << delta_params[i];
	std::cout << "\n, twiddle_stage: " << twiddle_stage[i];
    std::cout << "\n--------------------------------------------------" << "\n--------------------------------------------------" << "\n";
	
	if ( frame_count % N == 0 ) {
		Kp = params[0];
		Kd = params[1];
		Ki = params[2];
	}
}

double PID::TotalError() {
	return prev_cte * prev_cte;
}

