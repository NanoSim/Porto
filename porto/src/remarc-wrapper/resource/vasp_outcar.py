#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Implements three classes representing the information in CONTCAR and
OUTCAR files and frequenices calculated using phononpy.
"""
import re
import math
import warnings
import numpy as np


class CONTCAR(object):
    """POSCAR file parser.

    Attributes denoted as array below are actually nested lists.

    Attributes
    ----------
    comment       Comment
    scale         Scale
    cell          Unscaled supercell, 3x3 array, Å
    atomkinds     List of included atom kinds (chemical symbols)
    atomnumbers   Number of each atom kind
    symbols       List with the chemical symbols of each atom
    natoms        Number of atoms
    selective     bool, whether selective dynamics is selected
    cartesian     Whether coordinates are Cartesian or direct, bool
    coords        Coordinates, natoms x 3 array
    movable       Wether each coord can change, bool natoms x 3 array
    velocities    Velocity of each atom, natoms x 3 array, optional
    """
    def __init__(self, filename):
        with open(filename, 'r') as f:
            self.parse(f)

    def parse(self, f):
        """Parse POSCAR file and store results."""
        self.comment = f.readline()
        self.scale = float(f.readline())

        
        # Read cell
        vec1 = [float(v) for v in f.readline().split()]
        vec2 = [float(v) for v in f.readline().split()]
        vec3 = [float(v) for v in f.readline().split()]
        self.cell = [vec1, vec2, vec3]

        # Read number of each atom
        sixthline_tokens = f.readline().split()  # Test for vasp5 syntax
        try:
            int(sixthline_tokens[0])
        except ValueError:
            atomkinds = sixthline_tokens
            atomnumbers = [int(n) for n in f.readline().split()]
        else:
            atomnumbers = [int(n) for n in sixthline_tokens]
            tokens = self.comment.split()
            if len(tokens) == len(atomnumbers):
                atomkinds = tokens   # ASE convension
            else:
                atomkinds = ['at%d' % n for n in range(len(atomnumbers))]
        self.atomkinds = atomkinds
        self.atomnumbers = atomnumbers
        self.symbols = []
        for at, n in zip(atomkinds, atomnumbers):
            self.symbols.extend([at] * n)
        self.natoms = sum(atomnumbers)

        seventhline = f.readline()
        if seventhline[0].lower() == 's':
            seventhline = f.readline()
            self.selective = True
        else:
            self.selective = False
        self.cartesian = True if seventhline[0].lower() == 'c' else False

        # Read coordinates
        self.coords = []
        self.movable = []
        for i in range(self.natoms):
            tokens = f.readline().split()
            self.coords.append([float(tokens[i]) for i in range(3)])
            if self.selective:
                self.movable.append([tokens[i][0].upper() == 'T' 
                                       for i in range(3, 6)])
            else:
                self.movable.append([True, True, True])

        # Read velocities
        self.velocities = []
        if not f.readline():
            return
        for i in range(self.natoms):
            tokens = f.readline().split()
            if not tokens:
                return
            self.velocities.append([float(tokens[i]) for i in range(3)])

    def get_cell(self):
        """Returns the cell (including scaling) as a numpy array.
        Requires numpy."""
        import numpy as np
        
        return self.scale * np.array(self.cell)

    def get_positions(self):
        """Returns atom positions in Cartesian coordinates (Å) as a
        numpy array.  Requires numpy."""
        import numpy as np
        if self.cartesian:
            return np.array(self.coords)
        else:
            return np.dot(self.coords, self.get_cell())

    def get_scaled_positions(self):
        """Returns scaled atom positions. as a numpy array.  
        Requires numpy."""
        if self.cartesian:
            scaled = np.linalg.solve(self.get_cell().T, 
                                     np.array(self.coords).T).T
            scaled %= 1.0
            scaled %= 1.0
            return scaled
        else:
            return np.array(self.coords)
    
    def read_cell(self):
        """ Returns the cell 
        """
        import numpy
        cell = self.get_cell()
        
        cell_list = []
        for i in range(3):
            c = cell[i]
            cell_list.append([c[0], c[1], c[2]])
        return cell_list
    
    def read_atoms_and_positions(self):
        """ Returns atomkind and positions
        """
        import numpy as np

        atomlist = []

        symbols = self.symbols
        positions = self.get_positions()
        
        for i in range(self.natoms):

            s = symbols[i]
            p = positions[i]

            atomlist.append([s, [float(p[0]), float(p[1]), float(p[2])]])

        return atomlist

    def read_atomkinds(self):
        return self.atomkinds

    def read_atomnumbers(self):
        return self.atomnumbers

    def read_chemical_species(self):
        """ Returns the chemical formula 
        """
        a = self.atomkinds
        n = self.atomnumbers

        species = []
        for i in range(len(a)):
            species.append(str(a[i]))
            species.append(str(n[i]))
            chemical_species = ''.join(map(str, species))

        return chemical_species

    def read_composition(self, ordered=False, adsorbed=False, surface=None, name=False):
        """
        Returns the chemical composition/formula as a string 
        If ordered, the chemical compositoion is ordered alfabetically, but with H and C placed first.
        If adsorbate, the chemical composition of the surface atoms is removed from the formula. Requires that surface_atoms is given.
        """

        import numpy as np

        a = self.atomkinds
        n = self.atomnumbers

        index = list(range(len(a)))
        species = []
        numbers = []

        # If adsorbate, the surface atoms are removed 
        if adsorbed:
            surface_kind, surface_numbers = surface

            for i in index:
            # Remove the surface atoms from the list
                if a[i] in surface_kind:
                    species=species
                    numbers=numbers
                else:
                    species.append(str(a[i]))
                    numbers.append(str(n[i])) 
        else:
            for i in index:
                species.append(str(a[i]))
                numbers.append(str(n[i]))

        if ordered:
            # Sorting the atoms in alphabetical order
            index.sort(key = species.__getitem__)
            species[:] = [species[i] for i in index]
            numbers[:] = [numbers[i] for i in index]

        # Placing H first
            if "H" in species:
                i = species.index('H')
                species.insert(0, species.pop(i))
                numbers.insert(0, numbers.pop(i))

        # Placing C first
            if "C" in species:
                i = species.index('C')
                species.insert(0, species.pop(i))
                numbers.insert(0, numbers.pop(i))
    
        composition = ''
        for s, n in zip(species, numbers):
            composition += s
            if name: 
                if int(n) > 1:
                    composition += n
            else:
                composition += n
 
        return composition



 
class OUTCAR(object):
    """OUTCAR file

    Arguments
    ---------
    filename   Path to OUTCAR file
 
    """

    def __init__(self, filename):
        self.filename = filename
 
    def outcar_check(self):
        """
        Check if the outcar-file has completed. 
        """
        f = open(self.filename, 'r')
        finished = None
        
        while True:
            line = f.readline()
            if line.startswith(' General timing and accounting informations for this job'):
                finished = True
                return finished
            elif not line:
                finished = False
                return finished
                
 
    def read_energy(self):
        """
        Read the total energy from an OUTCAR file
        """

        f = open(self.filename, 'r')

        # Read through the file and find the cell
        while True:
            line = f.readline()
            if line.startswith('  energy  without entropy'):
                item = line.split()
                E0 = float(item[6])

            elif not line:
                break

        return E0


    def read_number_of_atoms(self):
         """
         Read the number of atoms from an OUTCAR file
         """

         f = open(self.filename, 'r')
         number_of_atoms = 0
         # Read through the file and find the cell
         while True:
             line = f.readline()
             if line.startswith('   ions per type ='):

                 item = line.split()
                 natoms =  item[4:]

                 for i in range(0,len(natoms)):
                     number_of_atoms += int(natoms[i])
             elif not line:
                 break
         return number_of_atoms
          
    def read_ion_positions(self):
        """       
        Returns the ion_positions and number of atoms
        """
        f = open(self.filename, 'r')
        
        natoms = self.read_number_of_atoms()
        cell = []
        ion_positions = []
        xcoords = []
        ycoords = []
        zcoords = []
        
        while True:
            line = f.readline()
            

            if line.startswith('      direct lattice vectors'):
                #f.readline()
 
                A1 = [float(x) for x in f.readline().split()[:3]]
                A2 = [float(x) for x in f.readline().split()[:3]]
                A3 = [float(x) for x in f.readline().split()[:3]]
                
                cell.append([A1, A2, A3])
  

            if line.startswith(' position of ions in cartesian coordinates'):
                #f.readline()
                for i in range(0, natoms):
                    line = f.readline()
                    item = line.split()

                    x = float(item[0])
                    y = float(item[1])
                    z = float(item[2])

                    pos = np.array([float(item[0]), float(item[1]), float(item[2])])
                    xcoords.append(x)
                    ycoords.append(y)
                    zcoords.append(z)

                    ion_positions.append(pos)


            elif not line:
                break
            
        return ion_positions, natoms
        
        
    def check_if_straight(self):
        """
        Check if a free/isolated molecule is straight consisting of three or four atoms.
        """
    
        pos, natoms = self.read_ion_positions()
     
        range1 = (179.5, 180.5)
        range2 = (0.0 , 0.5)
        range3 = (359.5, 360.0)

        straight = False
                  
        if natoms == 3:

            p0 = pos[0]
            p1 = pos[1]
            p2 = pos[2]

            vec1 = p0 - p1
            vec2 = p2 - p1

            n_vec1 = vec1/self.norm(vec1)
            n_vec2 = vec2/self.norm(vec2)

            dot_vec1_vec2 = np.dot(n_vec1, n_vec2)
            angle1 = math.acos(dot_vec1_vec2)*180./math.pi

            # Rewrite!!!!!
            if angle1 in range1:
                straight = True
            elif angle1 in range2:
                straight = True
            elif angle1 in range3:
                straight = True

        if natoms == 4:
  
            p0 = pos[0]
            p1 = pos[1]
            p2 = pos[2]
            p3 = pos[3]

            vec1 = p0 - p1
            vec2 = p2 - p1
            vec3 = p3 - p2

            n_vec1 = vec1/self.norm(vec1)
            n_vec2 = vec2/self.norm(vec2)
            n_vec3 = vec3/self.norm(vec3)

            dot_vec1_vec2 = np.dot(n_vec1, n_vec2)
            dot_vec2_vec3 = np.dot(n_vec2, n_vec3)

            angle1 = math.acos(dot_vec1_vec2)*180./math.pi
            angle2 = math.acos(dot_vec2_vec3)*180./math.pi

            straight1 = False
            straight2 = False

            # Rewrite!!!!!
            if angle1 in range1:
                straight1 = True
            elif angle1 in range2:
                straight1 = True
            elif angle1 in range3:
                straight1 = True
            if angle2 in range1:
                straight2 = True
            elif angle2 in range2:
                straight2 = True
            elif angle2 in range3:
                straight2 = True

            
            if straight1 and straight2 is True:
                straight = True

        #return ion_positions
        return straight



    def check_convergence(self):
        """
        Method checking if the calculations has converged
        To be added!
        """

        f = open(self.filename, 'r')

        converged = None
      
        return converged
   

    
    def read_vibrational_frequencies(self, meV=False, gasphase = False):
        """
        Reads the vibrational frequencies for VASP OUTCAR file
        Returns the frequencies in meV if meV = True otherwise in cm-1
        """

        frequencies = []
        f = open(self.filename, 'r')
        
        natoms = self.read_number_of_atoms()

        # Read through file and get vibrational frequencies
        while True:
            line = f.readline()
            if ' Eigenvectors and eigenvalues of the dynamical matrix' in line:
                for i in range(0,3):
                    f.readline() 

            # add frequencies to array
                for i in range(3 * natoms):
                    line = f.readline()
                    item = line.split()

                    if 'f  =' in line:
                        frequencies.append(float(item[7]))
                    elif 'f/i=' in line:
                        frequencies.append(complex(float(item[6]), 0j))

                    #skip the next n + 2 lines
                    for j in range(1 + natoms + 1): 
                        f.readline()
            elif not line:
                break

        f.close()    
        
        # if the molecule is in the gasphase only 3*n-6(5) frequencies will be returned
        if gasphase:
            if natoms == 1:
                frequencies = []
            elif natoms == 2:
                    frequencies = frequencies[0:1]
            elif natoms > 2:
                # Straight molecules 
                # Can only be checked for molecules with 3 or 4 atoms
                straight = self.check_if_straight()
                if straight:
                    frequencies = frequencies[0:(3*natoms-5)]
                    # Non-straight molecules
                else:
                    frequencies = frequencies[0:(3*natoms-6)]

             # Convert frequencies to meV if meV = True
        if meV:
            factor = 0.123984243 # factor for converting cm-1 to meV
            frequencies = np.multiply(frequencies, factor)
            
        return frequencies


    def norm(self, pos):
        return np.sqrt(np.sum(pos*pos))



class PHONONPY(object):
    """Return list of frequencies calculated using phononpy

    Arguments
    ---------
    filename   Path to mesh.yaml file
 
    """

    def __init__(self, filename):
        self.filename = filename

    def read_vibrational_frequencies_phononpy(self, meV=False, gasphase = False):
        """
        Reads the vibrational frequencies for PHONONPY calculation
        Returns the frequencies in meV if meV = True otherwise in wavenumbers
        file - mesh.yaml
        depend on yaml package
        """
        import yaml

        # Conversion parameter for THz to cm-1 taken from nist.gov data
        THz_cm_m1 = 1/0.02998

        # Read the yaml file
        with open(self.filename, 'r') as f:
            document = yaml.load(f.read())

            # Get the 'phonon' block of the yaml file
            data = document['phonon']
            item_phonon = data[0]

            # Get the 'band' block of the yaml file
            item_band = Structure(**item_phonon)

            # Get list of frequencies format [{frequency: value}]
            frequency_list  =  item_band.band

            # Get list of frequencies with values in THz converted to cm-1 only
            frequencies = []
            for item in frequency_list:
                freq = float(item['frequency']) * THz_cm_m1
                frequencies.append(freq)

            frequencies.sort(reverse = True)
 
            # make negative numbers - imaginary
            for i in range(0, len(frequencies)): 
                if frequencies[i] < 0.:
                    frequencies[i] = complex(0, abs(frequencies[i]))
        
            # Remove n-6(5) if molecule in gas phase
            if gasphase:
                if natoms == 1:
                    frequencies = []
                if natoms == 2:
                    frequencies = frequencies[0:1]
                else:
                    # Straight molecules 
                    # Can only be checked for molecules with 3 or 4 atoms
                    straight = self.check_if_straight()
                    if straight:
                        frequencies = frequencies[0:(3*natoms-5)]
                        # Non-straight molecules
                    else:
                        frequencies = frequencies[0:(3*natoms-6)]

            # Convert frequencies to meV if meV = True
            if meV:
                factor = 0.123984243 # factor for converting cm-1 to meV
                frequencies = np.multiply(frequencies, factor)
    
        return frequencies


class Structure:
    def __init__(self, **entries):
        self.__dict__.update(entries)


