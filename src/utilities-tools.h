
/*******************************************************************************
This file is part of tools_piccante.

tools_piccante is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
tools_piccante is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with tools_piccante.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

#ifndef UTILITIESTOOLS_H
#define UTILITIESTOOLS_H

#include "Wheader.h"

void swap_endian(float* in_f, uint64_t n);
void swap_endian(int* in_i, uint64_t n);
void swap_endian(double* in_d, uint64_t n);
int is_big_endian();


#endif // UTILITIESTOOLS_H
