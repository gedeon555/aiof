//
// recognition_engine.h
//  
// Made by jaydehei
// Login   <jaydehei@jaydehei-desktop>
// 
// Started on  Sat Oct  6 14:56:43 2007 jaydehei
// Started on  Sat Oct  6 14:56:43 2007 jaydehei
//

#ifndef   	RECOGNITION_ENGINE_H_
# define   	RECOGNITION_ENGINE_H_

#include <vector>
#include "utils.h"
#include "perceptron.h"
#include "typedefs.h"

class Gaiof;

using namespace std;

class Recognition_engine
{
public:
  ~Recognition_engine ();

  static Recognition_engine* get_engine();
  Perceptron* get_perceptron(string);
  void learn (string, string);
  void learn (vector < string >, string);
  string load (string);
  t_percents recognize (string);

private:
  Recognition_engine ();

  void add_example (string filename, string classname);

private:
  vector < Perceptron > classes_;
};

#endif /* !RECOGNITION_ENGINE_H_ */
