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
#ifndef ZERNIKE_H
#define ZERNIKE_H

#include <iostream>
#include <vector>
#include <Imlib2.h>
#include "CImg.h"

using namespace cimg_library;

#define PI 3.1411592654
#define RED(x) x / (256 * 256) % 256
#define GREEN(x) x / 256 % 256
#define BLUE(x) x % 256
#define GREY(x) (RED(x) + GREEN(x) + BLUE(x)) / 3.

using namespace std;

/**
    @author Benjamin Cohen Solal,,, <benji@BENJI-DESKTOP>
*/
class Zernike
{
public:
  Zernike ();
  ~Zernike ();

  static vector < float >compute_moments (string, int);

private:
  static float compute_zernike_moment (CImg<>&/*DATA32 **/, int, int);
  static float compute_zernike_radial (int, int, float);
  static std::pair < float, float >compute_zernike_polynom (int, int, float,
                                                            float);
};

#endif
