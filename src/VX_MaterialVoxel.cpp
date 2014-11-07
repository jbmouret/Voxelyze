/*******************************************************************************
Copyright (c) 2010, Jonathan Hiller (Cornell University)
If used in publication cite "J. Hiller and H. Lipson "Dynamic Simulation of Soft Heterogeneous Objects" In press. (2011)"

This file is part of Voxelyze.
Voxelyze is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Voxelyze is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
See <http://www.opensource.org/licenses/lgpl-3.0.html> for license details.
*******************************************************************************/

#include "VX_MaterialVoxel.h"
#include <assert.h>


CVX_MaterialVoxel::CVX_MaterialVoxel(double nominalSize, float youngsModulus, float density) : CVX_Material(youngsModulus, density)
{
	nomSize = nominalSize;
	gravMult = 0.0f;
	updateDerived();

}

//CVX_MaterialVoxel::~CVX_MaterialVoxel(void)
//{
//
//}

CVX_MaterialVoxel& CVX_MaterialVoxel::operator=(const CVX_MaterialVoxel& vIn)
{
	CVX_Material::operator=(vIn); //set base CVX_Material class variables equal

	nomSize=vIn.nomSize;
	gravMult=vIn.gravMult;
	_eHat = vIn._eHat;
	_mass=vIn._mass;
	_massInverse=vIn._massInverse;
	_sqrtMass=-vIn._sqrtMass;
	_firstMoment=vIn._firstMoment;
	_momentInertia=vIn._momentInertia;
	_momentInertiaInverse=vIn._momentInertiaInverse;
	_2xSqMxExS=vIn._2xSqMxExS;
	_2xSqIxExSxSxS=vIn._2xSqIxExSxSxS;

	return *this;
}

bool CVX_MaterialVoxel::updateDerived() 
{
	CVX_Material::updateDerived(); //update base CVX_Material class derived variables

	double volume = nomSize*nomSize*nomSize;
	_mass = (float)(volume*rho); 
	_momentInertia = (float)(_mass*nomSize*nomSize / 6.0f); //simple 1D approx
	_firstMoment = (float)(_mass*nomSize / 2.0f);

	if (volume==0 || _mass==0 || _momentInertia==0){
		_massInverse = _sqrtMass = _momentInertiaInverse = _2xSqMxExS = _2xSqIxExSxSxS = 0.0f; //zero everything out
		return false;
	}


	_massInverse = 1.0f / _mass;
	_sqrtMass = sqrt(_mass);
	_momentInertiaInverse = 1.0f / _momentInertia;
	_2xSqMxExS = (float)(2.0f*sqrt(_mass*E*nomSize));
	_2xSqIxExSxSxS = (float)(2.0f*sqrt(_momentInertia*E*nomSize*nomSize*nomSize));

	return true;
}


bool CVX_MaterialVoxel::setNominalSize(double size)
{
	if (size <= 0) size = FLT_MIN;
	nomSize=size;
	return updateDerived(); //update derived quantities
}