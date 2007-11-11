/***************************************************************************
 *   Copyright (C) 2007 by Benjamin Cohen Solal,,,   *
 *   benji@BENJI-DESKTOP   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "utils.h"

/**
 * Computes the factorial of its parameter
 * For each number less than 32, stock the result in order to not compute it again in the future
 *
 * @param n
 * @return
 */
int factorial (int n)
{
  static int res[32];
  static bool first = true;
  int i;

  if (first == true)
  {
    for (i = 0; i < 32; ++i)
      res[i] = 0;

    first = false;
  }

  if (n < 32 && n > 0)
  {
    if (res[n] == 0)
      res[n] = n * factorial (n - 1);

    return res[n];
  }
  else if (n == 0)
    return 1;
  else
    return n * factorial (n - 1);
}

/*
**
*/
float random (float min, float max)
{
  return (min + ((float) rand () * (max - min) / RAND_MAX));
}

/*
**
*/
vector < float > get_input_from_file (string filename, int size)
{
  vector < float >res;

  std::ifstream file (filename.c_str ());
  int nb = 0;
  int i = 0;

  if (file)
  {
    for (i = 0; file && i < size; ++i)
    {
      file >> nb;
      res.push_back (nb);
    }
  }
  if (i != size)
    std::cerr << "FICHIER D'ENTREE INVALIDE taille = " << i << std::endl;
  return (res);
}

/*
**
*/
vector < string > get_files_from_dir (string pathname)
{
  vector < string > filenames;
  DIR *dp;
  dirent *ep;
  string str;

  if ((dp = opendir (pathname.c_str ())))
  {
    while ((ep = readdir (dp)))
    {
      if (strcmp (ep->d_name, ".") && strcmp (ep->d_name, ".."))
      {
        str = pathname + '/' + ep->d_name;
        filenames.push_back (str.c_str ());
      }
    }
  }
  else
    std::cerr << "error" << std::endl;
  return (filenames);
}
