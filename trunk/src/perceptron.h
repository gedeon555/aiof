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
  Perceptron ();
  Perceptron (string);
  ~Perceptron ();

  void init();
  void learn (vector < string >);
  float recognize (vector < float >);

  //void save (fstream &, string);
  void save (string);
  void load (string);

  string get_classname () const;

	int get_input_size() const;
	void set_input_size(int);
	void set_hidden_size(int);
	void set_epsilon_initial(float);
	void set_epsilon_drop(float);
	void set_nb_iter_epsilon(int);
	void set_nb_iter(int);

private:
  float compute_difference ();
  void randomize_weights();
  void process (vector < float >&);

private:
  typedef vector < Neuron > neurons;

  neurons inputs_;
  neurons hiddens_;
  neurons outputs_;
	
	// Neural Network Parameters
	int input_size;
	int hidden_size;
	float epsilon_initial;
	float epsilon_drop;
	int nb_iter_epsilon;
	int nb_iter;

  string classname_;
  Recognition_engine* parent_;
};

#endif /* !PERCEPTRON_H_ */
