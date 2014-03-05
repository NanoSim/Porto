/*---------------------------------------------------------------------------*\
    CFDEMcoupling - Open Source CFD-DEM coupling

    CFDEMcoupling is part of the CFDEMproject
    www.cfdem.com
                                Christoph Goniva, christoph.goniva@cfdem.com
                                Copyright 2009-2012 JKU Linz
                                Copyright 2012-     DCS Computing GmbH, Linz
                                Copyright 2013-     Graz University of Technology
-------------------------------------------------------------------------------
License
    This file is part of CFDEMcoupling.

    CFDEMcoupling is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CFDEMcoupling is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CFDEMcoupling; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Description
    This code is designed to realize coupled CFD-DEM simulations using LIGGGHTS
    and OpenFOAM(R). Note: this code is not part of OpenFOAM(R) (see DISCLAIMER).

    Beetstra et al. drag law
    based on Beetstra et al 2007,"Drag Force of Intermediate Reynolds Number Flow
    Past Mono- and Bidisperse Arrays of Spheres", AIChE Journal 53, No. 2.

    - including interpolation of the velocity to the exact position
    - including option for filtered drag coefficient model

Class
    BeetstraDrag

SourceFiles
    BeetstraDrag.C

\*---------------------------------------------------------------------------*/

#ifndef BeetstraDrag_H
#define BeetstraDrag_H

#include "forceModel.H"
#include "interpolationCellPoint.H"
#include "multiphaseFlowBasic.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class BeetstraDrag Declaration
\*---------------------------------------------------------------------------*/

class BeetstraDrag
:
    public forceModel
{
private:
    dictionary propsDict_;

    bool verbose_;

    word velFieldName_;

    const volVectorField& U_;

    word densityFieldName_;

    const volScalarField& rho_;

    word voidfractionFieldName_;

    const volScalarField& voidfraction_;

    word gravityFieldName_;

    #ifdef version21
        const uniformDimensionedVectorField& g_;    // ref to gravity
    #elif defined(version16ext) || defined(version15)
        const dimensionedVector& g_;    // ref to gravity
    #endif
    
    mutable scalar  dPrim_; //the  primary particle diameter
    
    scalar  rhoParticle_;   //the (reference) particle density

    bool interpolation_; // use interpolated field values

    mutable scalar scale_;
    
    bool useFilteredDragModel_; //use filtered drag model or not
    
    bool useParcelSizeDependentFilteredDrag_; //take the parcel size for the filtered drag into account
    
    scalar filtDragParamsK_; 
    scalar filtDragParamsALimit_;
    scalar filtDragParamsAExponent_;
    mutable scalar filtDragParamsLChar2_;
    
    mutable multiphaseFlowBasic basicCalculator_; 

    //Include filtered Drag Function Declarations
#   include "./filteredDragFunctions/filteredDragFunctions.H"

public:

    //- Runtime type information
    TypeName("BeetstraDrag");


    // Constructors

        //- Construct from components
        BeetstraDrag
        (
            const dictionary& dict,
            cfdemCloud& sm
        );

    // Destructor

        ~BeetstraDrag();


    // Member Functions
        void setForce() const;
        
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
