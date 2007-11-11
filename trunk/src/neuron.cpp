//
// neuron.cc
//  
// Made by jaydehei
// Login   <jaydehei@jaydehei-laptop>
// 
// Started on  Sat Sep 22 15:16:22 2007 jaydehei
// Started on  Sat Sep 22 15:16:22 2007 jaydehei
//

#include <iostream>
#include <time.h>
#include "utils.h"
#include "neuron.h"

/*
**
*/
Neuron::Neuron ()
{
  value_ = 0.;
  id_ = Neuron::generate_id();
}

/*
**
*/
Neuron::~Neuron ()
{

}

float Neuron::transfert (float value)
{
  return 1. / (1. + exp (-value));
}

float Neuron::get_value() const
{
    return value_;
}

void Neuron::set_value(float value)
{
    value_ = transfert(value);
}

float Neuron::get_weight_with(int neuron)
{
    return weights_[neuron];
}

void Neuron::set_weight_with(int neuron, float value)
{
    weights_[neuron] = value;
}

float Neuron::get_threshold()
{
    return weights_[0];
}

void Neuron::set_threshold(float value)
{
    weights_[0] = value;
}

int Neuron::get_id() const
{
    return id_;
}

int Neuron::generate_id()
{
    static int id = 1;
    
    return id++;
}

