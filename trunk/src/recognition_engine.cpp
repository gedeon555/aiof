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

  input_size_ = 25;
  hidden_size_ = 5;
  epsilon_ = 2;
  k_ = 0.9;
  nb_iter_epsilon_ = 20;
  nb_iter_ = 5000;  
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


void Recognition_engine::read_conf_file(string filename)
{
  std::ifstream file (filename.c_str ());
  int i;

  if (file.is_open() == true)
  {
    std::string str;
    std::string param;
    double nb;

    while (file.good() == true)
    {
      int pos;

      str.clear();
      param.clear();
      file >> str;

      pos = str.find ("=", 0);
      param = str.substr (0, pos);

      if (param != "")
      {
        istringstream iss (str.substr (pos + 1, str.size ()));

        iss >> nb;
        if (param == "input_size")
          input_size_ = (int) nb;
        else if (param == "hidden_size")
          hidden_size_ = (int) nb;
        else if (param == "eps")
          epsilon_ = nb;
        else if (param == "k")
          k_ = nb;
        else if (param == "nb_iter_eps")
          nb_iter_epsilon_ = (int) nb;
        else if (param == "nb_iter")
          nb_iter_ = (int) nb;
      }
    }

    file.close ();
  }
  else
  {
    std::cerr << "FICHIER INEXISTANT : " << filename;
    exit (1);
  }
}

/*
**
*/
void Recognition_engine::learn (string dirname, string classname)
{
  vector < Perceptron >::const_iterator classe;

  std::cout << "  --> Apprend la classe " << classname << "..." << std::endl;
  // cherche si la class classname est deja existante
  for (classe = classes_.begin ();
       classe != classes_.end () && classname != classe->get_classname ();
       ++classe)
    ;

  if (classe == classes_.end ())
  {
    Perceptron new_class (this, classname);

    new_class.learn (get_files_from_dir (dirname));
    classes_.push_back (new_class);
  }
  else
    ///@todo que faire si la classe existe deja hein benjamin?
    ;
  std::cout << "  <-- A appris la classe " << classname << "..." << std::endl;
}

void Recognition_engine::learn (vector < string > files, string classname)
{
  vector < Perceptron >::const_iterator classe;

  std::cout << "  --> Apprend la classe " << classname << "..." << std::endl;
  // cherche si la class classname est deja existante
  for (classe = classes_.begin ();
       classe != classes_.end () && classname != classe->get_classname ();
       ++classe)
    ;

  if (classe == classes_.end ())
  {
    Perceptron new_class (this, classname);

    new_class.learn (files);
    classes_.push_back (new_class);
  }
  else
    ///@todo que faire si la classe existe deja hein benjamin?
    ;
  std::cout << "  <-- A appris la classe " << classname << "..." << std::endl;
}

/*
**
*/
void Recognition_engine::save (string class_name, string path)
{
  vector < Perceptron >::iterator it;

  for (it = classes_.begin (); it != classes_.end (); ++it)
  {
    if (it->get_classname() == class_name)
        it->save (path);
  }
}

string Recognition_engine::load (string filename)
{
  fstream in (filename.c_str (), fstream::in);
  string class_name;
  char c[128];

  in.getline(c, 128);
  in.close ();
  class_name = c;
  Perceptron perceptron(this, class_name);

  perceptron.load (filename);
  classes_.push_back (perceptron);
  
  return class_name;
}

t_percents Recognition_engine::recognize (string filename)
{
  vector < Perceptron >::iterator it;
  vector < float > moments;
  t_percents results;
  float difference = 0.;
  float sum = 0.;
  float sum2 = 0.;

  moments = Zernike::compute_moments(filename, input_size_);
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

int Recognition_engine::get_input_size() const
{
    return input_size_;
}

int Recognition_engine::get_hidden_size() const
{
    return hidden_size_;
}

double Recognition_engine::get_epsilon() const
{
    return epsilon_;
}

double Recognition_engine::get_k() const
{
    return k_;
}

int Recognition_engine::get_nb_iter_epsilon() const
{
    return nb_iter_epsilon_;
}

int Recognition_engine::get_nb_iter() const
{
    return nb_iter_;
}
