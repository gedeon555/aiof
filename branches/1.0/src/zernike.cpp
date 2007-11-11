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
#include <config.h>

#include <math.h>
#include "CImg.h"
#include "zernike.h"
#include "utils.h"
#include "interface.h"

Zernike::Zernike ()
{
}


Zernike::~Zernike ()
{
}


/*
 * Compute Zernike moments of a picture
 * Warning: Problems with scaling
 *
 * @param filename The filename of the picture
 * @param nb The number of wanted Zernike moments
 * @return A vector of computated Zernike moments
 */
vector < float >Zernike::compute_moments (string filename, int nb)
{
  CImg<> image(filename.c_str());
  vector < float >moments;
  vector < float >::iterator it;
  int i = 0, j = 0, n = 0;
  float moment0;
  float progression;

  progress_bar_set_current_text(filename);
  while (n < nb)
  {
    if ((i + j) % 2 == 0) // if i + j even
    {
      progression = n;
      progression /= nb;
      progress_bar_set_local_value(progression);

      moments.push_back (compute_zernike_moment (image/*datas*/, i, j));
      ++n;
      ++j;

      if (j > i)
      {
        j = 0;
        ++i;
      }
    }
    else
    {
      ++j;
    }
  }
  progress_bar_set_local_value(1);

  moment0 = moments[0];
  for (it = moments.begin(); it != moments.end(); ++it)
  {
    *it /= moment0;
  }

  return moments;
}

float Zernike::compute_zernike_moment (CImg<>& image /*DATA32 * datas*/, int n, int m)
{
  unsigned int i, j;
  unsigned int width, height;
  float x, y;
  float r, teta;
  float sum_re = 0, sum_im = 0;
  float grey;
  std::pair < float, float >polynom;

  width = image.width;
  height = image.height;
	/*
  width = imlib_image_get_width ();
  height = imlib_image_get_height ();
    */
  for (i = 0; i < width; ++i)
  {
    for (j = 0; j < height; ++j)
    {
      // Mise à l'échelle
      x = 2. * (i - width / 2.) / width;
      y = 2. * (j - height / 2.) / height;

      // Calcul du rayon
      r = sqrt (x * x + y * y);

      if (r > 1)
        continue;

      // Calcul de teta
      if (x > 0 && y < 0)
        teta = atan (y / x) + 2 * PI;
      else if (x < 0)
        teta = atan (y / x) + PI;
      else if (x == 0 && y > 0)
        teta = PI / 2;
      else if (x == 0 && y < 0)
        teta = 3 * PI / 2;
      else
        teta = 0;

      polynom = Zernike::compute_zernike_polynom (n, m, r, teta);

      grey = (image (i, j) + image (i, j, 0, 1) + image (i, j, 2)) / 3.;
	  //grey = GREY (datas[j * width + i]);
      sum_re += grey * polynom.first;
      sum_im += grey * polynom.second;
    }
  }

  sum_re = (n + 1) * sum_re / PI;
  sum_im = (n + 1) * sum_im / PI;

  // @todo: Trouver un moyen de renvoyer un nombre compris entre 0 et 1
  return sqrt (sum_re * sum_re + sum_im * sum_im);
}

float Zernike::compute_zernike_radial (int n, int m, float r)
{
  static float radials[16][16][16];
  static bool first = true;
  float sum = 0;
  int s, i, j;

  // Initialisation du vecteur Radials
  if (first)
  {
    for (i = 0; i < 16; ++i)
      for (j = 0; j < 16; ++j)
        for (s = 0; s < 16; ++s)
          radials[i][j][s] = 0;

    first = false;
  }

  for (s = 0; s <= (n - (m > 0 ? m : -m)) / 2; ++s)
  {
    if (radials[n][m][s] == 0)
    {
      radials[n][m][s] = pow ((float) -1, s) * factorial (n - s);
      radials[n][m][s] /= factorial (s);
      radials[n][m][s] /= factorial (((n + (m > 0 ? m : -m)) / 2 - s));
      radials[n][m][s] /= factorial (((n - (m > 0 ? m : -m)) / 2 - s));
    }

    sum += radials[n][m][s] * pow (r, n - 2 * s);
  }

  return sum;
}

std::pair < float, float >
  Zernike::compute_zernike_polynom (int n, int m, float r, float teta)
{
  float re, im;
  float radial;

  radial = Zernike::compute_zernike_radial (n, m, r);
  re = radial * cos (m * teta);
  im = radial * sin (m * teta);
  return std::pair < float, float >(re, im);
}
