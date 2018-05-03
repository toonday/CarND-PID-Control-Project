#include "PID.h"
#include <iostream>
#include <limits>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {
	log_file.close();
}

void PID::Init(double Kp, double Ki, double Kd) {
	log_file.open("_log.txt");
	
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	
	best_error = std::numeric_limits<double>::max();
	curr_error = 0.0;
	max_error = 1.0;
	frame_count = 0;
	param_index = 0;
	stop_twiddle = true;
	twiddle_frame_count = 0;
	init_delta_params = 1.0;
	
	for( int i=0; i<3; ++i ) {
		params.push_back(0.0);
		delta_params.push_back(init_delta_params);
		twiddle_stage.push_back(-1);
	}

	// initialize delta params to values in the expected range of the variables
	delta_params[0] = 0.1;
	delta_params[1] = 20.0;
	delta_params[2] = 0.005;
}

void PID::LogWrite(std::string s) {
	if ( log_file.is_open() ) {
		log_file << s;
	}
}

void PID::UpdateError(double cte) {
	frame_count++;
	
	d_error = cte - p_error;
	i_error += cte;
	p_error = cte;
	
	int N = 10;
	double tolerance = 0.005;

	// stop twiddling when the best error is less than tolerance threshold!
	// this is my tweak to the twiddle algorithm explained by Seb
	// the algorithm stops twiddling when the sum of delta params is less than the tolerance threshold
	// however, I think this works better in this scenario (updates are done every frame and you cannot have a loop to simulate the future /more complicated approach/)
	stop_twiddle = (best_error < tolerance*1.0);
	if ( stop_twiddle ) {
		LogWrite("\n, ++++++++++++++++++++ TWIDDLING STOPPED: \n");
		return;
	}
	++twiddle_frame_count;

	bool update_twiddle = (twiddle_frame_count == (2*N)-1);
	if ( twiddle_frame_count >= N )
		curr_error += cte * cte / N;
	
	int i = param_index;
	if ( update_twiddle ) {
		
		LogWrite("\n, ******************** UPDATING: " + std::to_string(twiddle_frame_count));
		LogWrite(", stop_twiddle: " + std::to_string(stop_twiddle));
		LogWrite(", twiddle_frame_count: " + std::to_string(twiddle_frame_count));
		LogWrite(", frame_count: " + std::to_string(frame_count) + "\n");
		LogWrite(", p_error: " + std::to_string(p_error));
		LogWrite(", i_error: " + std::to_string(i_error));
		LogWrite(", d_error: " + std::to_string(d_error) + "\n");
		LogWrite(", param_index: " + std::to_string(param_index));
		LogWrite(", curr_error: " + std::to_string(curr_error));
		LogWrite(", best_error: " + std::to_string(best_error) + "\n");
		LogWrite(", Kp: " + std::to_string(Kp));
		LogWrite(", Ki: " + std::to_string(Ki));
		LogWrite(", Kd: " + std::to_string(Kd) + "\n");
		LogWrite(", delta_params[0]: " + std::to_string(delta_params[0]));
		LogWrite(", delta_params[1]: " + std::to_string(delta_params[1]));
		LogWrite(", delta_params[2]: " + std::to_string(delta_params[2]) + "\n");
		LogWrite(", params[0]: " + std::to_string(params[0]));
		LogWrite(", params[1]: " + std::to_string(params[1]));
		LogWrite(", params[2]: " + std::to_string(params[2]) + "\n");
		LogWrite(", BEFORE twiddle_stage[0]: " + std::to_string(twiddle_stage[0]));
		LogWrite("\n, BEFORE twiddle_stage[1]: " + std::to_string(twiddle_stage[1]));
		LogWrite("\n, BEFORE twiddle_stage[2]: " + std::to_string(twiddle_stage[2]));
		LogWrite("\n--------------------------------------------------");
	
		if ( twiddle_stage[i] == -1 ) {
			best_error = curr_error;
			twiddle_stage[i] = twiddle_stage[i] + 1;
		}
		
		if ( twiddle_stage[i] == 0 ) {
			params[i] += delta_params[i];
			twiddle_stage[i] = 1;
		}
		else if ( twiddle_stage[i] == 1 ) {
			if( curr_error < best_error ) {
				best_error = curr_error;
				delta_params[i] *= 1.1;
				twiddle_stage[i] = 0;
				param_index = (param_index+1) % 3;
				params[param_index] += delta_params[param_index];
				twiddle_stage[param_index] = 1;
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
				param_index = (param_index+1) % 3;
				params[param_index] += delta_params[param_index];
				twiddle_stage[param_index] = 1;
			}
			else {
				params[i] += delta_params[i];
				delta_params[i] *= 0.9;
				twiddle_stage[i] = 0;
				param_index = (param_index+1) % 3;
				params[param_index] += delta_params[param_index];
				twiddle_stage[param_index] = 1;
			}
		}	
		curr_error = 0.0;
		twiddle_frame_count = 0;
		
		LogWrite("\n, i: " + std::to_string(i));
		LogWrite("\n, params: " + std::to_string(params[i]));
		LogWrite("\n, delta_params: " + std::to_string(delta_params[i]));
		LogWrite("\n, AFTER twiddle_stage[0]: " + std::to_string(twiddle_stage[0]));
		LogWrite("\n, AFTER twiddle_stage[1]: " + std::to_string(twiddle_stage[1]));
		LogWrite("\n, AFTER twiddle_stage[2]: " + std::to_string(twiddle_stage[2]));
		LogWrite("\n-------------------------------------------------- \n-------------------------------------------------- \n");
	}
	else {		
		LogWrite("\n, ******************** NOT UPDATING: " + std::to_string(twiddle_frame_count) + "\n");
	}
}

double PID::TotalError() {
	Kp = params[0];
	Kd = params[1];
	Ki = params[2];
	
	double total_error = (-Kp * p_error) + (-Kd * d_error) + (-Ki * i_error);
	
	// clamp total_error to be between -1 and 1
	total_error = (total_error > 1.0) ? 1.0 : total_error;
	total_error = (total_error < -1.0) ? -1.0 : total_error;
		
	return total_error;
}

