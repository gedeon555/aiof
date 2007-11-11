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
  void read_conf_file(string = "/etc/aiof/aiof.conf");
  void learn (string, string);
  void learn (vector < string >, string);
  void save (string, string);
  string load (string = "~/.aiof/");
  t_percents recognize (string);

  int get_input_size() const;
  int get_hidden_size() const;
  double get_epsilon() const;
  double get_k() const;
  int get_nb_iter_epsilon() const;
  int get_nb_iter() const;

private:
  Recognition_engine ();

  void add_example (string filename, string classname);

private:
  vector < Perceptron > classes_;

  int input_size_;
  int hidden_size_;
  double epsilon_;
  double k_;
  int nb_iter_epsilon_;
  int nb_iter_;
};

#endif /* !RECOGNITION_ENGINE_H_ */
