/*
** perceptron.h
** 
** Made by jaydehei
** Login   <jaydehei@jaydehei-laptop>
** 
** Started on  Sat Sep 22 14:51:57 2007 jaydehei
Last update Sat Oct  6 16:37:41 2007 jaydehei
*/

#ifndef   	PERCEPTRON_H_
# define   	PERCEPTRON_H_
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include "classname.h"
# include "neuron.h"

class Recognition_engine;

using namespace std;

class Perceptron
{
public:
  Perceptron (Recognition_engine*, string);
  ~Perceptron ();

  void learn (vector < string >);
  float recognize (vector < float >);

  //void save (fstream &, string);
  void save (string);
  void load (string);

  string get_classname () const;

private:
  float compute_difference ();
  void randomize_weights();
  void process (vector < float >&);

private:
  typedef vector < Neuron > neurons;

  neurons inputs_;
  neurons hiddens_;
  neurons outputs_;

  string classname_;
  Recognition_engine* parent_;
};

#endif /* !PERCEPTRON_H_ */
