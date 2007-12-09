//
// perceptron.cc
//  
// Made by jaydehei
// Login   <jaydehei@jaydehei-laptop>
// 
// Started on  Sat Sep 22 14:54:00 2007 jaydehei
// Started on  Sat Sep 22 14:54:00 2007 jaydehei
//

#include <iostream>
#include "perceptron.h"
#include "time.h"
#include "utils.h"
#include "zernike.h"
#include "recognition_engine.h"
#include "interface.h"

Perceptron::Perceptron ()
{
  this->input_size = 30;
  this->hidden_size = 10;
  this->epsilon_initial = 2.;
  this->epsilon_drop = 0.9;
  this->nb_iter_epsilon = 30;
  this->nb_iter = 5000;
}

Perceptron::Perceptron (string classname)
{
  classname_ = classname;

  this->input_size = 30;
  this->hidden_size = 10;
  this->epsilon_initial = 2.;
  this->epsilon_drop = 0.9;
  this->nb_iter_epsilon = 30;
  this->nb_iter = 5000;
}

void Perceptron::init()
{
  int i;

  for (i = 0; i < input_size; ++i)
  {
    inputs_.push_back (*new Neuron);
    outputs_.push_back (*new Neuron);
  }

  for (i = 0; i < hidden_size; ++i)
  {
    hiddens_.push_back (*new Neuron);
  }

  randomize_weights ();
}

void Perceptron::randomize_weights ()
{
  neurons::iterator it, jt;

  // Initialisation aléatoire des poids entre la couche d'entrée et la couche cachée
  for (jt = hiddens_.begin (); jt != hiddens_.end (); ++jt)
  {
    for (it = inputs_.begin (); it != inputs_.end (); ++it)
      it->set_weight_with (jt->get_id(), random (-0.5, 0.5));

    // Initialise aussi le seuil des neurones de la couche cachée
    jt->set_threshold(random (-0.5, 0.5));
  }

  // Initialisation aléatoire des poids entre la couche cachée et la couche de sortie
  for (jt = outputs_.begin (); jt != outputs_.end (); ++jt)
  {
    for (it = hiddens_.begin (); it != hiddens_.end (); ++it)
      it->set_weight_with (jt->get_id(), random (-0.5, 0.5));

    // Initialise aussi le seuil des neurones de la couche de sortie
    jt->set_threshold(random (-0.5, 0.5));

  }
}

Perceptron::~Perceptron ()
{

}

void Perceptron::learn (vector < string > files)
{
  vector < vector < float > >moments;
  vector < string >::iterator file_it;
  vector < float >::iterator moment_it;
  map < int,float >deltas;
  neurons::iterator neuron_it, neuron_jt;
  int i = 0, k;
  int j = 0;
  int num_example;
  float sum;
  float v, w;
  double epsilon = epsilon_initial;
  int local_progress_range = nb_iter / 100;
  float progression = 0;

  progress_bar_set_total_value(0);
  for (file_it = files.begin (); file_it != files.end (); ++file_it)
  {
    progression = ++i;
    progress_bar_set_local_value(0);
    moments.push_back (Zernike::compute_moments (*file_it, input_size));
    progress_bar_set_local_value(1);
    progress_bar_set_total_value(progression / (1 + files.size()));
  }

  progress_bar_set_local_value(0);
  progress_bar_set_current_text("Learning");
  progression = 0;
  for (i = 0; i < nb_iter; ++i)
  {
    if (i % local_progress_range == 0)
    {
      progression += 0.01;
      progress_bar_set_local_value(progression);
    }

    if (j++ == nb_iter_epsilon)
    {
      epsilon *= epsilon_drop;
      j = 0;
    }

    // Sélection aléatoire d'un exemple
    num_example = (int) random (0, moments.size () - 1);

    // Calcul de la sortie du réseau
    process (moments[num_example]);

    // Calcul des deltas pour chaque neurone de sortie
    for (neuron_it = outputs_.begin (), neuron_jt = inputs_.begin();
         neuron_it != outputs_.end (); ++neuron_it, ++neuron_jt)
      {
        v = neuron_it->get_value ();
        deltas[neuron_it->get_id()] = v * (1 - v) * (neuron_jt->get_value() - v);
      }

    // Calcul des deltas pour chaque neurone de la couche cachée
    for (neuron_it = hiddens_.begin (); neuron_it != hiddens_.end ();
         ++neuron_it)
    {
      sum = 0;

      for (neuron_jt = outputs_.begin (); neuron_jt != outputs_.end ();
           ++neuron_jt)
        sum += deltas[neuron_jt->get_id()] * neuron_it->get_weight_with (neuron_jt->get_id());

      v = neuron_it->get_value ();
      deltas[neuron_it->get_id()] = v * (1 - v) * sum;
    }

    // Mise à jour des poids entre la couche d'entrée et la couche cachée
    for (neuron_jt = hiddens_.begin (); neuron_jt != hiddens_.end ();
         ++neuron_jt)
    {
      for (neuron_it = inputs_.begin (); neuron_it != inputs_.end ();
           ++neuron_it)
      {
        w = neuron_it->get_weight_with (neuron_jt->get_id());
        w += epsilon * deltas[neuron_jt->get_id()] * neuron_it->get_value ();
        neuron_it->set_weight_with (neuron_jt->get_id(), w);
      }

      w = neuron_jt->get_threshold();
      w += epsilon * deltas[neuron_jt->get_id()];
      neuron_jt->set_threshold(w);
    }

    // Mise à jour des poids entre la couche cachée et la couche de sortie
    for (neuron_jt = outputs_.begin (); neuron_jt != outputs_.end ();
         ++neuron_jt)
    {
      for (neuron_it = hiddens_.begin (); neuron_it != hiddens_.end ();
           ++neuron_it)
      {
        w = neuron_it->get_weight_with (neuron_jt->get_id());
        w += epsilon * deltas[neuron_jt->get_id()] * neuron_it->get_value ();
        neuron_it->set_weight_with (neuron_jt->get_id(), w);
      }

      w = neuron_jt->get_threshold();
      w += epsilon * deltas[neuron_jt->get_id()];
      neuron_jt->set_threshold(w);
    }
  }

  progress_bar_set_local_value(1);
  progress_bar_set_total_value(1);
}

void Perceptron::process (vector < float >&moments)
{
  neurons::iterator it, jt;
  vector < float >::iterator kt;
	int i;
  float sum;

  // Copie des valeurs d'entrées sur la couche de neurones d'entrée
  for (kt = moments.begin (), it = inputs_.begin (), i = 0; 
       i < input_size; ++kt, ++it, ++i)
    it->set_value (*kt);

  // Calcul des valeurs des neurones dans la couche cachée
  for (jt = hiddens_.begin (); jt != hiddens_.end (); ++jt)
  {
    sum = jt->get_threshold();
    for (it = inputs_.begin (); it != inputs_.end (); ++it)
    {
      sum += it->get_weight_with (jt->get_id()) * it->get_value ();
    }
    jt->set_value (sum);
  }

  // Calcul des valeurs des neurones dans la couche de sortie
  for (jt = outputs_.begin (); jt != outputs_.end (); ++jt)
  {
    sum = jt->get_threshold();
    for (it = hiddens_.begin (); it != hiddens_.end (); ++it)
    {
      sum += it->get_weight_with (jt->get_id()) * it->get_value ();
    }
    jt->set_value (sum);
  }
}


float Perceptron::recognize (vector < float >moments)
{
  progress_bar_set_current_text(classname_);
  process (moments);

  return (compute_difference ());
}

string Perceptron::get_classname () const
{
  return classname_;
}

int Perceptron::get_input_size() const
{
	return this->input_size;
}

void Perceptron::set_input_size(int n)
{
	this->input_size = n;
}

void Perceptron::set_hidden_size(int n)
{
	this->hidden_size = n;
}

void Perceptron::set_epsilon_initial(float f)
{
	this->epsilon_initial = f;
}

void Perceptron::set_epsilon_drop(float f)
{
	this->epsilon_drop = f;
}

void Perceptron::set_nb_iter_epsilon(int n)
{
	this->nb_iter_epsilon = n;
}

void Perceptron::set_nb_iter(int n)
{
	this->nb_iter = n;
}

float Perceptron::compute_difference ()
{
  float res = 0.;
  float tmp = 0.;

  neurons::iterator in, on;

  for (on = outputs_.begin (), in =
       inputs_.begin (); on != outputs_.end (); ++on, ++in)
  {
    tmp = on->get_value () - in->get_value ();
    res += (tmp >= 0 ? tmp : -tmp);
  }

  return (res);
}

void Perceptron::save (string path)
{
  neurons::iterator it, jt;
  fstream out (path.c_str (), fstream::out);

  out << classname_ << endl 
			<< this->input_size << " " << this->hidden_size << endl
			<< this->epsilon_initial << " " << this->epsilon_drop << endl
			<< this->nb_iter_epsilon << " " << this->nb_iter << endl << endl;
  
  if (out.good() == true)
  {
    // Ecriture des poids entre la couche d'entrée et la couche cachée
      for (it = inputs_.begin(); it != inputs_.end(); ++it)
      {
        for (jt = hiddens_.begin(); jt != hiddens_.end(); ++jt)
          out << it->get_weight_with(jt->get_id()) << " ";

        out << endl;
      }

      // Ecriture des poids entre la couche cachée et la couche de sortie
      for (it = hiddens_.begin(); it != hiddens_.end(); ++it)
      {
        out << it->get_threshold() << " ";

        for (jt = outputs_.begin(); jt != outputs_.end(); ++jt)
          out << it->get_weight_with(jt->get_id()) << " ";
        
        out << endl;
      }
    
      for (it = outputs_.begin(); it != outputs_.end(); ++it)
        out << it->get_threshold() << " ";
        
      out.close ();
  }
}

void Perceptron::load (string filename)
{
  fstream in (filename.c_str (), fstream::in);
  neurons::iterator it;
  neurons::iterator jt;
  Neuron* neuronA;
  Neuron* neuronB;
  float w;
  int input_size, hidden_size;
  int i;
  char str[128];

  in.getline(str, 128);
  
  this->classname_ = str;
  in >> this->input_size >> this->hidden_size;
	in >> this->epsilon_initial >> this->epsilon_drop;
	in >> this->nb_iter_epsilon >> this->nb_iter;

  init();

  for (it = inputs_.begin(); it != inputs_.end(); ++it)
  {
    for (jt = hiddens_.begin(); jt != hiddens_.end(); ++jt)
    {
      in >> w;
      it->set_weight_with(jt->get_id(), w);
    }
  }

  for (it = hiddens_.begin(); it != hiddens_.end(); ++it)
  {
    in >> w;
    it->set_threshold(w);

    for (jt = outputs_.begin(); jt != outputs_.end(); ++jt)
    {
      
      in >> w;
      it->set_weight_with(jt->get_id(), w);
    }
  }

  for (it = outputs_.begin(); it != outputs_.end(); ++it)
  {
    in >> w;
    it->set_threshold(w);
  }
  
  in.close ();
}
