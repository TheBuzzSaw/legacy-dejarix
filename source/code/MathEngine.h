/**
 *  This file is part of Dejarix.
 *
 *  Dejarix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Dejarix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dejarix.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MATHENGINE_H
#define MATHENGINE_H

#include <boost/random.hpp>

#define PI 3.141592653589793238462643383
#define PI_HALVES (PI / 2.0)
#define TO_RADIANS(n) ((n) * PI / 180.0)
#define TO_DEGREES(n) ((n) * 180.0 / PI)
#define PSEUDO_ZERO 0.000001

class MathEngine
{
    public:
        template<class T>
        static T supremeRandom(T min, T max)
        {
            boost::uniform_real<T> u(min, max);
            boost::variate_generator< boost::mt19937&, boost::uniform_real<T> >
                outValue(mSeed, u);
            return outValue();
        }

    private:
        static boost::mt19937 mSeed;
};

#endif
