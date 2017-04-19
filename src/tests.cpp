#include "tests.h"


/*******************************************
 * STATE PREDICTOR TESTS
 *******************************************/

bool test_compute_augmented_sigma(){

  double SMALL_POSITIVE_VALUE = 1.e-05;
  VectorXd WEIGHTS = VectorXd(NSIGMA);
  WEIGHTS << w0, w, w, w, w, w, w, w, w, w, w, w, w, w, w;
  StatePredictor statePredictor(WEIGHTS);

  VectorXd x = VectorXd(NX);
  x << 5.7441,
       1.3800,
       2.2049,
       0.5015,
      0.3528;

  MatrixXd P = MatrixXd(NX, NX);
  P <<  0.0043,   -0.0013,    0.0030,   -0.0022,   -0.0020,
       -0.0013,    0.0077,    0.0011,    0.0071,    0.0060,
        0.0030,    0.0011,    0.0054,    0.0007,    0.0008,
       -0.0022,    0.0071,    0.0007,    0.0098,    0.0100,
       -0.0020,    0.0060,    0.0008,    0.0100,    0.0123;

  MatrixXd sigma_x = statePredictor.compute_augmented_sigma(x, P);

  MatrixXd expected_sigma_x = MatrixXd(NAUGMENTED, NSIGMA);

  expected_sigma_x <<
     5.7441,  5.85768,  5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,  5.63052,  5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,
     1.38,    1.34566,  1.52806,  1.38,     1.38,     1.38,     1.38,     1.38,    1.41434,  1.23194,  1.38,     1.38,     1.38,     1.38,     1.38,
     2.2049,  2.28414,  2.24557,  2.29582,  2.2049,   2.2049,   2.2049,   2.2049,  2.12566,  2.16423,  2.11398,  2.2049,   2.2049,   2.2049,   2.2049,
     0.5015,  0.44339,  0.631886, 0.516923, 0.595227, 0.5015,   0.5015,   0.5015,  0.55961,  0.371114, 0.486077, 0.407773, 0.5015,   0.5015,   0.5015,
     0.3528,  0.299973, 0.462123, 0.376339, 0.48417,  0.418721, 0.3528,   0.3528,  0.405627, 0.243477, 0.329261, 0.22143,  0.286879, 0.3528,   0.3528,
     0,       0,        0,        0,        0,        0,        0.34641,  0,       0,        0,        0,        0,        0,       -0.34641,  0,
     0,       0,        0,        0,        0,        0,        0,        0.34641, 0,        0,        0,        0,        0,        0,       -0.34641;

  //cout << "sigma_x" << endl;
  //cout << sigma_x << endl;
  //cout << "expected_sigma_x" << endl;
  //cout << expected_sigma_x << endl;

  bool r =  (expected_sigma_x - sigma_x).norm() < SMALL_POSITIVE_VALUE;
  return r;
}

bool test_predict_sigma_x(){

  double SMALL_POSITIVE_VALUE = 1.e-04;
  VectorXd WEIGHTS = VectorXd(NSIGMA);
  WEIGHTS << w0, w, w, w, w, w, w, w, w, w, w, w, w, w, w;
  StatePredictor statePredictor(WEIGHTS);

  MatrixXd augmented_sigma_x = MatrixXd(NAUGMENTED, NSIGMA);
  augmented_sigma_x <<
    5.7441,  5.85768,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.63052,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,
      1.38,  1.34566,  1.52806,     1.38,     1.38,     1.38,     1.38,     1.38,   1.41434,  1.23194,     1.38,     1.38,     1.38,     1.38,     1.38,
    2.2049,  2.28414,  2.24557,  2.29582,   2.2049,   2.2049,   2.2049,   2.2049,   2.12566,  2.16423,  2.11398,   2.2049,   2.2049,   2.2049,   2.2049,
    0.5015,  0.44339, 0.631886, 0.516923, 0.595227,   0.5015,   0.5015,   0.5015,   0.55961, 0.371114, 0.486077, 0.407773,   0.5015,   0.5015,   0.5015,
    0.3528, 0.299973, 0.462123, 0.376339,  0.48417, 0.418721,   0.3528,   0.3528,  0.405627, 0.243477, 0.329261,  0.22143, 0.286879,   0.3528,   0.3528,
         0,        0,        0,        0,        0,        0,  0.34641,        0,         0,        0,        0,        0,        0, -0.34641,        0,
         0,        0,        0,        0,        0,        0,        0,  0.34641,         0,        0,        0,        0,        0,        0, -0.34641;

  MatrixXd predicted_sigma_x = statePredictor.predict_sigma(augmented_sigma_x, 0.1);

  MatrixXd expected_predicted_sigma_x = MatrixXd(NX, NSIGMA);
  expected_predicted_sigma_x <<
  5.93553, 6.06251,  5.92217,  5.9415,   5.92361,  5.93516,  5.93705, 5.93553,  5.80832,  5.94481,  5.92935,  5.94553,  5.93589,  5.93401, 5.93553,
  1.48939, 1.44673,  1.66484,  1.49719,  1.508,    1.49001,  1.49022, 1.48939,  1.5308,   1.31287,  1.48182,  1.46967,  1.48876,  1.48855, 1.48939,
  2.2049,  2.28414,  2.24557,  2.29582,  2.2049,   2.2049,   2.23954, 2.2049,   2.12566,  2.16423,  2.11398,  2.2049,   2.2049,   2.17026, 2.2049,
  0.53678, 0.473387, 0.678098, 0.554557, 0.643644, 0.543372, 0.53678, 0.538512, 0.600173, 0.395462, 0.519003, 0.429916, 0.530188, 0.53678, 0.535048,
  0.3528,  0.299973, 0.462123, 0.376339, 0.48417,  0.418721, 0.3528,  0.387441, 0.405627, 0.243477, 0.329261, 0.22143,  0.286879, 0.3528,  0.318159;

  //cout << "predicted_sigma" << endl;
  //cout << predicted_sigma_x << endl;
  //cout << "expected_predicted_sigma" << endl;
  //cout << expected_predicted_sigma_x << endl;

  bool r =  (expected_predicted_sigma_x - predicted_sigma_x).norm() < SMALL_POSITIVE_VALUE;
  return r;
}

bool test_predict_x_P(){

  double SMALL_POSITIVE_VALUE = 1.e-5;
  VectorXd WEIGHTS = VectorXd(NSIGMA);
  WEIGHTS << w0, w, w, w, w, w, w, w, w, w, w, w, w, w, w;
  StatePredictor statePredictor(WEIGHTS);

  MatrixXd predicted_sigma_x = MatrixXd(NX, NSIGMA);
  predicted_sigma_x <<
       5.9374,  6.0640,   5.925,  5.9436,  5.9266,  5.9374,  5.9389,  5.9374,  5.8106,  5.9457,  5.9310,  5.9465,  5.9374,  5.9359,  5.93744,
         1.48,  1.4436,   1.660,  1.4934,  1.5036,    1.48,  1.4868,    1.48,  1.5271,  1.3104,  1.4787,  1.4674,    1.48,  1.4851,    1.486,
        2.204,  2.2841,  2.2455,  2.2958,   2.204,   2.204,  2.2395,   2.204,  2.1256,  2.1642,  2.1139,   2.204,   2.204,  2.1702,   2.2049,
       0.5367, 0.47338, 0.67809, 0.55455, 0.64364, 0.54337,  0.5367, 0.53851, 0.60017, 0.39546, 0.51900, 0.42991, 0.530188,  0.5367, 0.535048,
        0.352, 0.29997, 0.46212, 0.37633,  0.4841, 0.41872,   0.352, 0.38744, 0.40562, 0.24347, 0.32926,  0.2214, 0.28687,   0.352, 0.318159;

  VectorXd expected_x = VectorXd(NX);
  expected_x << 5.93637, 1.49035, 2.20528, 0.536853, 0.353577;

  MatrixXd expected_P = MatrixXd(NX, NX);
  expected_P <<
    0.00543425, -0.0024053,  0.00341576, -0.00348196, -0.00299378,
   -0.0024053,   0.010845,   0.0014923,   0.00980182,  0.00791091,
    0.00341576,  0.0014923,  0.00580129,  0.000778632, 0.000792973,
   -0.00348196,  0.00980182, 0.000778632, 0.0119238,   0.0112491,
   -0.00299378,  0.00791091, 0.000792973, 0.0112491, 0.0126972;

  VectorXd x = statePredictor.predict_x(predicted_sigma_x);
  MatrixXd P = statePredictor.predict_P(predicted_sigma_x, x);

  //cout << "x" << endl;
  //cout << x << endl;
  //cout << "expected_x" << endl;
  //cout << expected_x << endl;
  //cout << "P" << endl;
  //cout << P << endl;
  //cout << "expected_P" << endl;
  //cout << expected_P << endl;

  bool a =  (expected_x - x).norm() < SMALL_POSITIVE_VALUE;
  bool b =  (expected_P - P).norm() < SMALL_POSITIVE_VALUE;

  return (a && b);
}

/*******************************************
 * RADAR PREDICTOR TESTS
 *******************************************/

bool radar_predictor_process_test(){

  double SMALL_POSITIVE_VALUE = 1.e-4;
  VectorXd WEIGHTS = VectorXd(NSIGMA);
  WEIGHTS << w0, w, w, w, w, w, w, w, w, w, w, w, w, w, w;
  RadarPredictor radarPredictor(WEIGHTS);

  MatrixXd sigma_x = MatrixXd(NX, NSIGMA);
  sigma_x <<
         5.9374,  6.0640,   5.925,  5.9436,  5.9266,  5.9374,  5.9389,  5.9374,  5.8106,  5.9457,  5.9310,  5.9465,  5.9374,  5.9359,  5.93744,
           1.48,  1.4436,   1.660,  1.4934,  1.5036,    1.48,  1.4868,    1.48,  1.5271,  1.3104,  1.4787,  1.4674,    1.48,  1.4851,    1.486,
          2.204,  2.2841,  2.2455,  2.2958,   2.204,   2.204,  2.2395,   2.204,  2.1256,  2.1642,  2.1139,   2.204,   2.204,  2.1702,   2.2049,
         0.5367, 0.47338, 0.67809, 0.55455, 0.64364, 0.54337,  0.5367, 0.53851, 0.60017, 0.39546, 0.51900, 0.42991, 0.530188,  0.5367, 0.535048,
          0.352, 0.29997, 0.46212, 0.37633,  0.4841, 0.41872,   0.352, 0.38744, 0.40562, 0.24347, 0.32926,  0.2214, 0.28687,   0.352, 0.318159;

  radarPredictor.process(sigma_x);
  VectorXd z = radarPredictor.getz();
  MatrixXd S = radarPredictor.getS();
  MatrixXd sigma_z = radarPredictor.get_sigma();


  VectorXd expected_z = VectorXd(NZ_RADAR);
  expected_z << 6.12155, 0.245993, 2.10313;

  MatrixXd expected_S = MatrixXd(NZ_RADAR, NZ_RADAR);
  expected_S <<
     0.0946171,   -0.000139448,    0.00407016,
    -0.000139448,  0.000617548,   -0.000770652,
     0.00407016,  -0.000770652,    0.0180917;

  MatrixXd expected_sigma_z = MatrixXd(NZ_RADAR, NSIGMA);
  expected_sigma_z <<
      6.11908,  6.23346,  6.15315,  6.12835,  6.11436,  6.11908,  6.12218,  6.11908,  6.00792,  6.08839,  6.11255,  6.12488,  6.11908,  6.11886,  6.12057,
     0.244289,  0.23371, 0.273165, 0.246166, 0.248461, 0.244289, 0.245307, 0.244289, 0.257001, 0.216927, 0.244336, 0.241934, 0.244289, 0.245157, 0.245239,
      2.11044,  2.21881,  2.06391,   2.1875,  2.03413,  2.10616,  2.14509,  2.10929,  2.00166,   2.1298,  2.03466,  2.16518,  2.11454,  2.07862,  2.11295;

  /*
  cout << "z" << endl;
  cout << z << endl;
  cout << "expected_z" << endl;
  cout << expected_z << endl;

  cout << "S" << endl;
  cout << S << endl;
  cout << "expected_S" << endl;
  cout << expected_S << endl;

  cout << "sigma_z" << endl;
  cout << sigma_z << endl;
  cout << "expected_sigma_z" << endl;
  cout << expected_sigma_z << endl;
  */
  bool a =  (expected_z - z).norm() < SMALL_POSITIVE_VALUE;
  bool b =  (expected_S - S).norm() < SMALL_POSITIVE_VALUE;
  bool c =  (expected_sigma_z - sigma_z).norm() < SMALL_POSITIVE_VALUE;

  return (a && b && c);
}
