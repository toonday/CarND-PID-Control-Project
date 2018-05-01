#ifndef PID_H
#define PID_H
#include <vector>

class PID {
public:
  double prev_cte;
  double differential_cte;
  double integral_cte;
  std::vector<double> params;
  std::vector<double> delta_params;
  std::vector<int> twiddle_stage;
  double best_error;
  double curr_error;
  int frame_count;
  int param_index;
  
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
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
