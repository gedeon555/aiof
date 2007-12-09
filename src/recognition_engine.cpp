//
// recognition_engine.cc
//  
// Made by jaydehei
// Login   <jaydehei@jaydehei-desktop>
// 
// Started on  Sat Oct  6 14:56:43 2007 jaydehei
// Started on  Sat Oct  6 14:56:43 2007 jaydehei
//

#include <iostream>
#include <unistd.h>
#include "zernike.h"
#include "recognition_engine.h"
#include "interface.h"

/*
** Constructor
*/
Recognition_engine::Recognition_engine ()
{
  srand ((unsigned) time (NULL));  
}

/*
** Destructor
*/
Recognition_engine::~Recognition_engine ()
{
}


Recognition_engine* Recognition_engine::get_engine()
{
  static Recognition_engine* engine = 0;
  
  if (engine == 0)
    engine = new Recognition_engine();

  return engine;
}

Perceptron* Recognition_engine::get_perceptron(string classname)
{
  vector < Perceptron >::iterator classe;

  // cherche si la class classname est deja existante
  for (classe = classes_.begin (); classe != classes_.end (); ++classe)
    if (classname == classe->get_classname ())
      return &(*classe);

  Perceptron* new_class = new Perceptron (classname);
  classes_.push_back (*new_class);
  return new_class;
}

string Recognition_engine::load (string filename)
{
  Perceptron perceptron;

  perceptron.load (filename);
  classes_.push_back (perceptron);
  
  return perceptron.get_classname ();
}

t_percents Recognition_engine::recognize (string filename)
{
  vector < Perceptron >::iterator it;
  vector < float > moments;
  t_percents results;
  float difference = 0.;
  float sum = 0.;
  float sum2 = 0.;
	int nb_moments_max = 0;

	for (it = classes_.begin(); it != classes_.end (); ++it)
		if (it->get_input_size() > nb_moments_max)
			nb_moments_max = it->get_input_size();

  moments = Zernike::compute_moments(filename, nb_moments_max);
  for (it = classes_.begin(); it != classes_.end (); ++it)
  {
    difference = it->recognize (moments);
    sum += difference;
    results[it->get_classname()] = difference;
  }

  for (it = classes_.begin(); it != classes_.end (); ++it)
  {
    results[it->get_classname()] = sum - results[it->get_classname()];
    sum2 += results[it->get_classname()];
  }
  
  for (it = classes_.begin(); it != classes_.end (); ++it)
  {
    results[it->get_classname()] = 100. * results[it->get_classname()] / sum2;
  }

  return results;
}
