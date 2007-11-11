/*
** neuron.h
** 
** Made by jaydehei
** Login   <jaydehei@jaydehei-laptop>
** 
** Started on  Sat Sep 22 15:16:42 2007 jaydehei
** Last update Sat Sep 22 15:16:42 2007 jaydehei
*/

#ifndef   	NEURON_H_
# define   	NEURON_H_
# include <iostream>
# include <vector>
# include <map>
# include "math.h"

using namespace std;
class Layer;
class Neuron
{
public:
  Neuron ();
   ~Neuron ();

  float transfert (float);

  float get_value() const;
  void set_value(float);
  float get_weight_with(int);
  void set_weight_with(int, float);
  void set_threshold(float);
  float get_threshold();
  int get_id() const;

  static int generate_id();

private:
  int id_;
  float value_;
  map < int,float > weights_;
};

#endif /* !NEURON_H_ */
