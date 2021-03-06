#ifndef PID_H
#define PID_H
#include <vector>
#include <string>
#include <fstream>

class PID {
public:
  std::vector<double> params;
  std::vector<double> delta_params;
  std::vector<int> twiddle_stage;
  double best_error;
  double curr_error;
  double max_error;
  int frame_count;
  int param_index;
  bool stop_twiddle;
  int twiddle_frame_count;
  std::ofstream log_file;
  double init_delta_params;
  
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Write to log file.
  */
  void LogWrite(std::string s);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
