#!/usr/bin env python
import sys, math, numpy, re, os.path
################################################################################
#
# RMC-rate
#
# RATE CALCULATOR
#
# Part of the REMARC set of scripts for translation of (periodic) DFT
# (or similar) data into rate constants and thermodynamics.
#
# Specifically writes CHEMKIN-II and SURFACE CHEMKIN files
# (at present no thermodynamics data to these files)
#
# Currently handles surface reactions and (non-dissociative/associative)
# adsorption/desorption processes
#
# Input units: eV, Angstrom, cm-1
#
# Internal units: eV, Angstrom, amu
#
# Output units: kJ/mol, s, cm
#
# Version 1.0
# Stefan Andersson, 2015
#
################################################################################
### Physical constants
kB = 8.6173324E-5
hbar = 6.46538E-2
Rgas = 8.314462
mole = 6.02214129E23
### Time and frequency units
tu = 1.018046E-14
invcm2tau = 3.052025E-4
### Mass units
amu2kg = 1.660538921E-27
### Length units
cm = 1.0E8
cm3 = cm**3
ang2m = 1.0e-10
### Energy units
cm2ev = 8065.54
ev2K = 11604.505
ev2J = 1.602176565E-19
ev2kJ = 96.4853
J2kJ = 1.0E-3
### Pressure units
atm2Pa = 101325.0
Pprgm2Pa = ev2J*ang2m**-3
atm2Pprgm = atm2Pa/Pprgm2Pa

### Atomic masses
### Default is to use weighted atomic masses
masses = {
    'mass_H': 1.00794, 'mass_He': 4.002602, 'mass_Li': 6.941,
    'mass_Be': 9.012182, 'mass_B': 10.811, 'mass_C': 12.0107,
    'mass_N': 14.0067, 'mass_O': 15.9994, 'mass_F': 18.9984032,
    'mass_Ne': 20.1797, 'mass_Na': 22.98976928, 'mass_Mg': 24.3050,
    'mass_Al': 26.9815386, 'mass_Si': 28.0855, 'mass_P': 30.973762,
    'mass_S': 32.065, 'mass_Cl': 35.453, 'mass_Ar': 39.948, 'mass_K': 39.0983,
    'mass_Ca': 40.078, 'mass_Sc': 44.955912, 'mass_Ti': 47.867,
    'mass_V': 50.9415, 'mass_Cr': 51.9961, 'mass_Mn': 54.938045,
    'mass_Fe': 55.845, 'mass_Co': 58.933195, 'mass_Ni': 58.6934,
    'mass_Cu': 63.546, 'mass_Zn': 65.38, 'mass_Ga': 69.723, 'mass_Ge': 72.64,
    'mass_As': 74.92160, 'mass_Se': 78.96, 'mass_Br': 79.904, 'mass_Kr': 83.798,
    'mass_Rb': 85.4678, 'mass_Sr': 87.62, 'mass_Y': 88.90584, 'mass_Zr': 91.224,
    'mass_Nb': 92.90637, 'mass_Mo': 95.95, 'mass_Tc': 98., 'mass_Ru': 101.07,
    'mass_Rh': 102.90550, 'mass_Pd': 106.42, 'mass_Ag': 107.8682,
    'mass_Cd': 112.414, 'mass_In': 114.818, 'mass_Sn': 118.710,
    'mass_Sb': 121.760, 'mass_Te': 127.60, 'mass_I': 126.90447,
    'mass_Xe': 131.293, 'mass_Cs': 132.90545196, 'mass_Ba': 137.327,
    'mass_La': 138.90547, 'mass_Ce': 140.116, 'mass_Pr': 140.90766,
    'mass_Nd': 144.242, 'mass_Pm': 145., 'mass_Sm':  150.36, 'mass_Eu': 151.964,
    'mass_Gd': 157.25, 'mass_Tb': 158.92535, 'mass_Dy': 162.500,
    'mass_Ho': 164.93033, 'mass_Er': 167.259, 'mass_Tm': 168.93422,
    'mass_Yb': 173.054, 'mass_Lu': 174.9668, 'mass_Hf': 178.49,
    'mass_Ta': 180.94788, 'mass_W': 183.84, 'mass_Re': 186.207,
    'mass_Os': 190.23, 'mass_Ir': 192.217, 'mass_Pt': 195.084,
    'mass_Au': 196.966569, 'mass_Hg': 200.592, 'mass_Tl': 204.38,
    'mass_Pb': 207.2, 'mass_Bi': 208.98040, 'mass_Po': 209., 'mass_At': 210.
    }

### Default prefactors
pre_fix = 1.0E13
pre_fix_des = 1.0E13

def AtomPartFunc(elements_pf):
### Calculates translation partition function for gas-phase atoms    
    structraw = elements_pf[8].strip('[]')
    struct = [x.replace('[','').replace(']','') for x in structraw.split('],')]
    atomcoords = [y.split(',') for y in struct]
    atoms = [z[0].replace("'","").replace(' ','') for z in atomcoords]
    massnames = ['mass_'+str for str in atoms]
    masses_pf = [masses[i] for i in massnames]
    masstot = math.fsum(masses_pf)
### Translational partition function
    q_trans = [((2.0*math.pi*masstot*kB*Ti/((2.0*math.pi*hbar)**2))**1.5)
               *(kB*Ti/Pinp) for Ti in T]
    return(masstot,q_trans)

def TransRotPartFunc(elements_pf):
### Calculates translational and rotational partition functions
### for gas-phase molecules 
    structraw = elements_pf[8].strip('[]')
    struct = [x.replace('[','').replace(']','') for x in structraw.split('],')]
    atomcoords = [y.split(',') for y in struct]
    atoms = [z[0].replace("'","").replace(' ','') for z in atomcoords]
    xcoords = [float(row[1]) for row in atomcoords]
    ycoords = [float(row[2]) for row in atomcoords]
    zcoords = [float(row[3]) for row in atomcoords]
### Check for periodic boundary conditions
###    Transform atomic coordinates from Cartesian coordinates
###    to skew coordinates
    cellraw = elements_pf[7].strip('[]')
    cell = [x.replace('[','').replace(']','') for x in cellraw.split('],')]
    cellcoords = [y.split(',') for y in cell]
    a1_cell = cellcoords[0]
    a2_cell = cellcoords[1]
    a3_cell = cellcoords[2]
    a1_x = float(a1_cell[0])
    a1_y = float(a1_cell[1])
    a1_z = float(a1_cell[2])
    a2_x = float(a2_cell[0])
    a2_y = float(a2_cell[1])
    a2_z = float(a2_cell[2])
    a3_x = float(a3_cell[0])
    a3_y = float(a3_cell[1])
    a3_z = float(a3_cell[2])
    a1 = [a1_x, a1_y, a1_z]
    a2 = [a2_x, a2_y, a2_z]
    a3 = [a3_x, a3_y, a3_z]
    a1_len = (a1_x**2 + a1_y**2 + a1_z**2)**0.5
    a2_len = (a2_x**2 + a2_y**2 + a2_z**2)**0.5
    a3_len = (a3_x**2 + a3_y**2 + a3_z**2)**0.5

    a12 = numpy.dot(a1,a2)
    a13 = numpy.dot(a1,a3)
    a23 = numpy.dot(a2,a3)
    theta12 = math.acos(a12/(a1_len*a2_len))
    theta13 = math.acos(a13/(a1_len*a3_len))
    theta23 = math.acos(a23/(a2_len*a3_len))

    xaxis = [1., 0., 0.]
    yaxis = [0., 1., 0.]
    zaxis = [0., 0., 1.]
    a1x = numpy.dot(a1,xaxis)
    a1y = numpy.dot(a1,yaxis)
    a1z = numpy.dot(a1,zaxis)
    a2x = numpy.dot(a2,xaxis)
    a2y = numpy.dot(a2,yaxis)
    a2z = numpy.dot(a2,zaxis)
    a3x = numpy.dot(a3,xaxis)
    a3y = numpy.dot(a3,yaxis)
    a3z = numpy.dot(a3,zaxis)
    costh1x = a1x/a1_len
    costh1y = a1y/a1_len
    costh1z = a1z/a1_len
    costh2x = a2x/a2_len
    costh2y = a2y/a2_len
    costh2z = a2z/a2_len
    costh3x = a3x/a3_len
    costh3y = a3y/a3_len
    costh3z = a3z/a3_len

    Atrans = numpy.array([[costh1x, costh2x, costh3x],
                          [costh1y, costh2y, costh3y],
                          [costh1z, costh2z, costh3z]])
    Ainv = numpy.linalg.inv(Atrans)

    atom_a1 = []
    atom_a2 = []
    atom_a3 = []
    for i,xi in enumerate(xcoords):

        xyzvect = [xcoords[i],ycoords[i],zcoords[i]]
        a123vect = numpy.dot(Ainv,xyzvect)

        atom_a1.append(a123vect[0])
        atom_a2.append(a123vect[1])
        atom_a3.append(a123vect[2])
        
###    If interatomic distance is larger than half the cell coordinate:
###    correct coordinates!
###    Reference atom is first atom in list

    for j in range(1,len(atom_a1)):
       Transform = False
       dist_a1 = atom_a1[j] - atom_a1[0]
       if(dist_a1 > a1_len/2):
          atom_a1[j] = atom_a1[j] - a1_len
          Transform = True
       elif(dist_a1 < -a1_len/2):
          atom_a1[j] = atom_a1[j] + a1_len
          Transform = True
       dist_a2 = atom_a2[j] - atom_a2[0]
       if(dist_a2 > a2_len/2):
          atom_a2[j] = atom_a2[j] - a2_len
          Transform = True
       elif(dist_a2 < -a2_len/2):
          atom_a2[j] = atom_a2[j] + a2_len
          Transform = True
       dist_a3 = atom_a3[j] - atom_a3[0]
       if(dist_a3 > a3_len/2):
          atom_a3[j] = atom_a3[j] - a3_len
          Transform = True
       elif(dist_a3 < -a3_len/2):
          atom_a3[j] = atom_a3[j] + a3_len
          Transform = True

### Transform coordinates back to Cartesian coordinates

       if(Transform):
           xcoords[j] = (a1_x*atom_a1[j]/a1_len + a2_x*atom_a2[j]/a2_len
                         + a3_x*atom_a3[j]/a3_len)
           ycoords[j] = (a1_y*atom_a1[j]/a1_len + a2_y*atom_a2[j]/a2_len
                         + a3_y*atom_a3[j]/a3_len)
           zcoords[j] = (a1_z*atom_a1[j]/a1_len + a2_z*atom_a2[j]/a2_len
                         + a3_z*atom_a3[j]/a3_len)

### Assign masses to atoms    
    massnames = ['mass_'+str for str in atoms]
    masses_pf = [masses[i] for i in massnames]

    masstot = math.fsum(masses_pf)
### Translational partition function    
    q_trans = [((2.0*math.pi*masstot*kB*Ti/
                ((2.0*math.pi*hbar)**2))**1.5)*(kB*Ti/Pinp) for Ti in T]
### Calculation of moments of intertia
###    Calculation of center of mass
    xcom = numpy.dot(masses_pf,xcoords)/masstot
    ycom = numpy.dot(masses_pf,ycoords)/masstot
    zcom = numpy.dot(masses_pf,zcoords)/masstot

###    Calculation of coordinates relative to center of mass
    xrel = [x-xcom for x in xcoords]
    yrel = [y-ycom for y in ycoords]
    zrel = [z-zcom for z in zcoords]
###    Set up inertia matrix
    x2rel = numpy.array([x**2 for x in xrel])
    y2rel = numpy.array([y**2 for y in yrel])
    z2rel = numpy.array([z**2 for z in zrel])
    r2rel = x2rel + y2rel + z2rel
    r2x2 = r2rel - x2rel
    r2y2 = r2rel - y2rel
    r2z2 = r2rel - z2rel
    xy = numpy.multiply(xrel,yrel)
    xz = numpy.multiply(xrel,zrel)
    yz = numpy.multiply(yrel,zrel)
    mr2x2 = numpy.multiply(masses_pf,r2x2)
    mr2y2 = numpy.multiply(masses_pf,r2y2)
    mr2z2 = numpy.multiply(masses_pf,r2z2)
    mxy = numpy.multiply(masses_pf,xy)
    mxz = numpy.multiply(masses_pf,xz)
    myz = numpy.multiply(masses_pf,yz)
    Ixx = math.fsum(mr2x2)
    Iyy = math.fsum(mr2y2)
    Izz = math.fsum(mr2z2)
    Ixy = -math.fsum(mxy)
    Ixz = -math.fsum(mxz)
    Iyz = -math.fsum(myz)
    inert = numpy.array([[Ixx, Ixy, Ixz], [Ixy, Iyy, Iyz], [Ixz, Iyz, Izz]])
###     Calculate moments of inertia
    I = numpy.linalg.eigvalsh(inert)
###     Sort moments of inertia
    Isort = sorted(I)
    Is0 = Isort[0]
    Is1 = Isort[1]
    Is2 = Isort[2]
###     Calculate rotational temperatures
    Trot0 = hbar**2/(2*kB*Is0)
    Trot1 = hbar**2/(2*kB*Is1)
    Trot2 = hbar**2/(2*kB*Is2)
    print('Moments of inertia:')
    print("%(Is0)8.4f amu*Angstrom2" % vars())
    print("%(Is1)8.4f amu*Angstrom2" % vars())
    print("%(Is2)8.4f amu*Angstrom2" % vars())
    print('Rotational temperatures:')
    print("%(Trot0)8.2f K" % vars())
    print("%(Trot1)8.2f K" % vars())
    print("%(Trot2)8.2f K" % vars())
    out_file.write("Moments of inertia:\n")
    out_file.write("%(Is0)8.4f amu*Angstrom2" % vars())
    out_file.write("\n")
    out_file.write("%(Is1)8.4f amu*Angstrom2" % vars())
    out_file.write("\n")
    out_file.write("%(Is2)8.4f amu*Angstrom2" % vars())
    out_file.write("\n")
    out_file.write("Rotational temperatures:\n" % vars())
    out_file.write("%(Trot0)8.2f K" % vars())
    out_file.write("\n")
    out_file.write("%(Trot1)8.2f K" % vars())
    out_file.write("\n")
    out_file.write("%(Trot2)8.2f K" % vars())
    out_file.write("\n")

    if (Isort[0] < 0.01):
        print('This is a linear molecule')
        print('What is its symmetry number? Ex. H2 = 2, CO = 1, CO2 = 2, ' \
              'N2O = 1, C2H2 = 2')
        symnum = "12"
        out_file.write("This is a linear molecule\n")
        out_file.write("What is its symmetry number? Ex. H2 = 2, CO = 1, " \
              "CO2 = 2, N2O = 1, C2H2 = 2\n")
        out_file.write(symnum)
        out_file.write("\n")
        sym =float(symnum)
        q_rot = [2.0*Isort[1]*kB*Ti/(sym*hbar**2) for Ti in T]
        Linear = True
    else:
        print('This is a non-linear molecule')
        print('What is its symmetry number? Ex. H2O = 2, NH3 = 3, CH4 = 12')
        symnum = "12"
        out_file.write("This is a non-linear molecule\n")
        out_file.write("What is its symmetry number? Ex. H2O = 2, NH3 = 3, " \
                       "CH4 = 12\n")
        out_file.write(symnum)
        out_file.write("\n")
        sym =float(symnum)
        Iprsq = math.sqrt(I[0]*I[1]*I[2])
### Rotational partition function        
        q_rot = [math.sqrt(8.0*math.pi)*(Iprsq*(kB*Ti)**1.5)/(sym*hbar**3)
                 for Ti in T]
  	Linear = False
    return(masstot,q_trans,q_rot,Linear)

def CellArea(elements_ca):
### Calculates area of simulation cell
    cellraw = elements_ca[7].strip('[]')
    axes_cell = cellraw.replace('[','').replace(']','').split(',')
    aaxis_cell_raw = [axes_cell[x] for x in 0,1,2]
    aaxis_cell = [float(x) for x in aaxis_cell_raw]
    baxis_cell_raw = [axes_cell[x] for x in 3,4,5]
    baxis_cell = [float(x) for x in baxis_cell_raw]
    area_cell = numpy.linalg.norm(numpy.cross(aaxis_cell,baxis_cell))
    print("Area of simulation cell = %(area_cell)8.4f Angstrom2" % vars())
    out_file.write("Area of simulation cell = %(area_cell)8.4f" \
                   " Angstrom2" % vars())
    out_file.write("\n")
    
    return area_cell

def SiteDens(NSites,area_cell):
### Calculates site density for given type of site              
    n_sites = float(NSites)
    sites_cell = n_sites/area_cell
    site_dens = sites_cell*cm**2/mole
    print("Site density = %(site_dens)10.3e moles/cm2" % vars())
    out_file.write("Site density = %(site_dens)10.3e moles/cm2" % vars())
    out_file.write("\n")
    return site_dens

def AreaSite(NSites,area_cell):
### Calculates average surface area per adsorption site for
### given adsorbed species
    n_sites = float(NSites)
    area_site = area_cell/n_sites
    print("Average area per adsorption site = %(area_site)8.4f" \
          " Angstrom2" % vars())
    out_file.write("Average area per adsorption site =" \
                   " %(area_site)8.4f Angstrom2" % vars())
    out_file.write("\n")
    return area_site

def FreqCalc_gas(elements_fc,Freqs_gas):
### Extracts energy and frequencies and calculates vibrational temperatures
### and zero-point energies for gas-phase species 
    energy = float(elements_fc[5])
    freqs = []
    zpe = 0.0
    vibtemp = 0.0
    if (Freqs_gas == 'Y' or Freqs_gas == 'y'):
        freqraw = elements_fc[6].strip('[]')
        freqsall = [complex(x) for x in freqraw.split(',')]
        freqs = [x.real for x in freqsall]
        vibtemp = [ev2K*frq/cm2ev for frq in freqs]
        freqsum = math.fsum(freqs)
        zpe = 0.5*freqsum/cm2ev
        print("ZPE = %(zpe)8.4f eV" % vars())
        out_file.write("ZPE = %(zpe)8.4f eV" % vars())
        out_file.write("\n")
    return (energy,freqs,zpe,vibtemp)

def FreqCalc_ads(elements_fc,Freqs_ads):
### Extracts energy and frequencies and calculates vibrational temperatures and
### zero-point energies for surface slab with or without adsorbed species
    energy = float(elements_fc[5])
    freqs = []
    zpe = 0.0
    vibtemp = 0.0
    if (Freqs_ads == 'Y' or Freqs_ads == 'y'):
        no_atoms = re.findall(r'(\d+)',elements_fc[1])

        N_atoms = [int(nat) for nat in no_atoms]

        N_atomsum = sum(N_atoms)

        freqraw = elements_fc[6].strip('[]')

        freqsall = [complex(x) for x in freqraw.split(',')]

###     Check if all N atoms in slab are included in frequency calculation.
###     If so, 3 frequencies are related to translation of slab and number of
###     internal frequencies is 3N-3 rather than 3N.
        
        if(len(freqsall) == 3*N_atomsum):
            print("All %(N_atomsum)d atoms in slab are included in frequency" \
                  " calculation." %vars())
            print("Removing 3 lowest frequencies:")
            out_file.write("All %(N_atomsum)d atoms in slab are included in" \
                  " frequency calculation." % vars())
            out_file.write("\n")
            out_file.write("Removing 3 lowest frequencies:\n")
            for i in range(3):

                freqrm = freqsall.pop()
                print(freqrm)
                frqrm_str = str(freqrm)
                out_file.write(frqrm_str)
                out_file.write("\n")

        freqs = [x.real for x in freqsall]

        vibtemp = [ev2K*frq/cm2ev for frq in freqs]

        freqsum = math.fsum(freqs)
        zpe = 0.5*freqsum/cm2ev
        
        print("ZPE = %(zpe)8.4f eV" % vars())
        out_file.write("ZPE = %(zpe)8.4f eV" % vars())
        out_file.write("\n")
    return (energy,freqs,zpe,vibtemp)

def FreqCalc_ads_ts(elements_fc,Freqs_ads_ts):
### Extracts energy and frequencies and calculates vibrational temperatures and
### zero-point energies for transition state at surface slab
    energy = float(elements_fc[5])
    freqs = []
    zpe = 0.0
    vibtemp = 0.0
    if (Freqs_ads_ts == 'Y' or Freqs_ads_ts == 'y'):
        no_atoms = re.findall(r'(\d+)',elements_fc[1])
        N_atoms = [int(nat) for nat in no_atoms]
        N_atomsum = sum(N_atoms)
        freqraw = elements_fc[6].strip('[]')
        freqsall = [complex(x) for x in freqraw.split(',')]
### Calculate crossover temperature, below which quantum tunneling will dominate
### the rate of reaction.
        imfreq = abs(freqsall[-1])
        omega = 2*math.pi*invcm2tau*imfreq
        T_c = hbar*omega/(2.0*math.pi*kB)
###     Check if all N atoms in slab are included in frequency calculation.
###     If so, 3 frequencies are related to translation of slab and number of
###     internal frequencies is 3N-3 rather than 3N.
###     Remove imaginary frequency of unstable mode at
###     saddle point (transition state).
        if(len(freqsall) == 3*N_atomsum):
            del freqsall[-1]
            print("All %(N_atomsum)d atoms in slab are included in frequency " \
                  "calculation." %vars())
            print("Removing 3 lowest frequencies:")
            out_file.write("All %(N_atomsum)d atoms in slab are included in" \
                  " frequency calculation." % vars())
            out_file.write("\n")
            out_file.write("Removing 3 lowest frequencies:\n")
            for i in range(3):
                freqrm = freqsall.pop()
                print(freqrm)
                frqrm_str = str(freqrm)
                out_file.write(frqrm_str)
                out_file.write("\n")
                
        else:
            del freqsall[-1]
        freqs = [x.real for x in freqsall]
        vibtemp = [ev2K*frq/cm2ev for frq in freqs]
              
        print("Imaginary frequency = %(imfreq)9.2fi cm-1" % vars())
        print("Crossover temperature for tunneling (Tc) = %(T_c)6.2f K" % vars())
        out_file.write("Imaginary frequency = %(imfreq)9.2fi cm-1" % vars())
        out_file.write("\n")
        out_file.write("Crossover temperature for tunneling (Tc)" \
                       " = %(T_c)6.2f K" % vars())
        out_file.write("\n")        
        freqsum_ts = math.fsum(freqs)
        zpe = 0.5*freqsum_ts/cm2ev
        print("ZPE = %(zpe)8.4f eV" % vars())
        out_file.write("ZPE = %(zpe)8.4f eV" % vars())
        out_file.write("\n")
    return (energy,freqs,zpe,vibtemp)

def ThermoCalc_atom(q_trans):
### Calculates thermodynamics of atom in gas phase
    U_gas = []
    H_gas = []
    S_gas = []
    for i,Ti in enumerate(T):
        S_trans = Rgas*(math.log(q_trans[i]) + 2.5)
        S = S_trans
        U_trans = 1.5*Rgas*Ti
        U = U_trans
        H = U + Rgas*Ti
        U_gas.append(U)
        H_gas.append(H)
        S_gas.append(S)
    return (U_gas,H_gas,S_gas)

def ThermoCalc_gas(q_trans,q_rot,vibtemp,Linear):
### Calculates thermodynamics of molecular species in gas phase
    U_gas = []
    H_gas = []
    S_gas = []

    for i,Ti in enumerate(T):
        S_vib = (Rgas*(math.fsum([(vibt1/Ti)/(math.exp(vibt1/Ti)-1.0)
                                  -math.log(1.0-math.exp(-vibt1/Ti))
                                  for vibt1 in vibtemp])))
        S_rot = Rgas*(math.log(q_rot[i]) + 1.5)
        S_trans = Rgas*(math.log(q_trans[i]) + 2.5)
        S = S_vib + S_rot + S_trans
        U_vib = (Rgas*(math.fsum([vibt1*(0.5+1.0/(math.exp(vibt1/Ti)-1.0))
                                  for vibt1 in vibtemp])))
        if Linear:
            U_rot = Rgas*Ti
        else:
            U_rot = 1.5*Rgas*Ti
        U_trans = 1.5*Rgas*Ti
        U = U_vib + U_rot + U_trans
        H = U + Rgas*Ti
        U_gas.append(U)
        H_gas.append(H)
        S_gas.append(S)

    return (U_gas,H_gas,S_gas)

def ThermoCalc_ads(vibtemp):
### Calculates thermodynamics of surface slab with or without adsorbed species       
    U_ads = []
    H_ads = []
    S_ads = []
    for Ti in T:
        S = (Rgas*(math.fsum([(vibt1/Ti)/(math.exp(vibt1/Ti)-1.0)
                              -math.log(1.0-math.exp(-vibt1/Ti))
                              for vibt1 in vibtemp])))
        U = Rgas*(math.fsum([vibt1*(0.5+1.0/(math.exp(vibt1/Ti)-1.0))
                             for vibt1 in vibtemp]))
        H = U
        U_ads.append(U)
        H_ads.append(H)
        S_ads.append(S)
    return (U_ads,H_ads,S_ads)

def RateCalc_ads_fix(elements_a,elements_g,elements_s,Freqs_ads,Freqsurf_ads):
### Calculates desorption rate constant using fixed pre-factor
    kt_fix_des = []

    print("\nAdsorbed species: get energy")
    out_file.write("\nAdsorbed species: get energy\n")
    energy_ads,freqs_ads,zpe_ads,vibtemp_ads = FreqCalc_ads(elements_a,
                                                            Freqs_ads)

    print("\nClean surface: get energy")
    out_file.write("\nClean surface: get energy\n")
    energy_surf,freqs_surf,zpe_surf,vibtemp_surf = FreqCalc_ads(elements_s,
                                                                Freqs_ads)

    print("\nGas phase species: get energy")
    out_file.write("\nGas phase species: get energy\n")    
    energy_gas,freqs_gas,zpe_gas,vibtemp_gas = FreqCalc_gas(elements_g,
                                                            Freqs_ads)

    deltaedes = energy_gas + energy_surf - energy_ads
    deltaedeskj = ev2kJ*deltaedes

    print("\nDeltaE desorption = %(deltaedes)8.4f eV (%(deltaedeskj)8.2f" \
          " kJ/mol)" % vars())
    out_file.write("\nDeltaE desorption = %(deltaedes)8.4f eV" \
                   " (%(deltaedeskj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    beta = [1.0/(kB*Ti) for Ti in T]
    logexp_des = [-betai*deltaedes for betai in beta]
    logkt_fix_des = [math.log(pre_fix_des) + ledes for ledes in logexp_des]
    kt_fix_des = [math.exp(lkt_fix_des) for lkt_fix_des in logkt_fix_des]

    return (kt_fix_des)

def RateCalc_ads_qq(elements_a,elements_g,elements_s,Freqs_ads,Freqsurf_ads,
                    N_atomsum_gas):
### Calculates desorption and adsorption rate constants using
### quantum vibrational partition functions
    kt_qq_des = []
    kt_qq_ads = []
    K_reac = []

    logK_reac = []

### Gas phase
              
###     Atoms

    if N_atomsum_gas == 1:
        (masstot_g,q_trans_g) = AtomPartFunc(elements_g) 
        q_trans_ts = numpy.array([(masstot_g*kB*Ti*A_site)/(2*math.pi*hbar**2)
                                  for Ti in  T])
        logq_trans_ts = numpy.log(q_trans_ts)
        logq_rot_ts = 0.0
        Freqs_at = 'N'

        print("\nGas-phase atom: get energy")
        out_file.write("\nGas-phase atom: get energy\n")
        energy_gas,freqs_gas,zpe_gas,vibtemp_gas = FreqCalc_gas(elements_g,
                                                                Freqs_at)
        U_gas,H_gas,S_gas = ThermoCalc_atom(q_trans_g)
        
###     Molecular species
    else:    
        (masstot_g,q_trans_g,q_rot_g,Linear) = TransRotPartFunc(elements_g) 
        q_trans_ts = numpy.array([(masstot_g*kB*Ti*A_site)/(2*math.pi*hbar**2)
                                  for Ti in  T])
        logq_trans_ts = numpy.log(q_trans_ts)
        q_rot_ts = numpy.array(q_rot_g)
        logq_rot_ts = numpy.log(q_rot_ts)

        print("\nGas-phase molecular species: get energy and frequency/ies")
        out_file.write("\nGas-phase molecular species: get energy and"
                       " frequency/ies\n")
        energy_gas,freqs_gas,zpe_gas,vibtemp_gas = FreqCalc_gas(elements_g,
                                                                Freqs_ads)
        U_gas,H_gas,S_gas = ThermoCalc_gas(q_trans_g,q_rot_g,vibtemp_gas,Linear)

### Adsorbed species and clean surface

    print("\nAdsorbed species: get energy and frequencies")
    out_file.write("\nAdsorbed species: get energy and frequencies\n")
    energy_ads,freqs_ads,zpe_ads,vibtemp_ads = FreqCalc_ads(elements_a,
                                                            Freqs_ads)   

    if (Freqsurf_ads == 'N' or Freqsurf_ads == 'n'):
        Freqs_s = 'N'

        print("\nClean surface: get energy")
        out_file.write("\nClean surface: get energy\n")
        energy_surf,freqs_surf,zpe_surf,vibtemp_surf = FreqCalc_ads(elements_s,
                                                                    Freqs_s)
    else:

        print("\nClean surface: get energy and frequencies")
        out_file.write("\nClean surface: get energy and frequencies\n")
        energy_surf,freqs_surf,zpe_surf,vibtemp_surf = FreqCalc_ads(elements_s,
                                                                    Freqs_ads)

    deltaedes = energy_gas + energy_surf - energy_ads
    dezpedes = deltaedes + zpe_gas + zpe_surf - zpe_ads

    deltaedeskj = ev2kJ*deltaedes
    dezpedeskj = ev2kJ*dezpedes
    
    print("\nDeltaE desorption = %(deltaedes)8.4f eV (%(deltaedeskj)8.2f" \
          " kJ/mol)" % vars())
    print("DeltaE desorption incl. ZPE = %(dezpedes)8.4f eV" \
          " (%(dezpedeskj)8.2f kJ/mol)" % vars())

    out_file.write("\nDeltaE desorption = %(deltaedes)8.4f eV" \
                   " (%(deltaedeskj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("DeltaE desorption incl. ZPE = %(dezpedes)8.4f eV" \
                   " (%(dezpedeskj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    
### Reaction thermodynamics
    U_ads,H_ads,S_ads = ThermoCalc_ads(vibtemp_ads)
    if(Freqsurf_ads == 'N' or Freqsurf_ads == 'n'):
        U_surf = 0.0
        H_surf = 0.0
        S_surf = 0.0
    else:
        U_surf,H_surf,S_surf = ThermoCalc_ads(vibtemp_surf)

    U_rc = numpy.array(U_ads)
    H_rc = numpy.array(H_ads)
    S_rc = numpy.array(S_ads)
    U_g = numpy.array(U_gas)
    H_g = numpy.array(H_gas)
    S_g = numpy.array(S_gas)
    U_s = numpy.array(U_surf)
    H_s = numpy.array(H_surf)
    S_s = numpy.array(S_surf)

    U_pd = U_g + U_s
    H_pd = H_g + H_s
    S_pd = S_g + S_s

    DeltaU_reac = J2kJ*(U_pd - U_rc) + ev2kJ*deltaedes
    DeltaH_reac = J2kJ*(H_pd - H_rc) + ev2kJ*deltaedes               

    DeltaS_reac = S_pd - S_rc
    DeltaG_reac = DeltaH_reac - T*(J2kJ*DeltaS_reac)

### Equilibrium constant
    for i,Ti in enumerate(T):

        logK_rc = -(DeltaG_reac[i]/J2kJ)/(Rgas*Ti)

        K_rc = math.exp(logK_rc)
        K_reac.append(K_rc)
        logK_reac.append(logK_rc)

    K_des = K_reac
    logK_des = logK_reac
    
### Vibrations for partition functions
    wmads = [2*math.pi*invcm2tau*freqads for freqads in freqs_ads]
    if (N_atomsum_gas == 1):
        wts = [0.0]
    else:
        wts = [2*math.pi*invcm2tau*freqts for freqts in freqs_gas]
    if (Freqsurf_ads == 'Y' or Freqsurf_ads == 'y'):
        wmsurf = [2*math.pi*invcm2tau*freqsurf for freqsurf in freqs_surf]
        wts.extend(wmsurf)

### Rate constant calculation
    beta = [1.0/(kB*Ti) for Ti in T]

    for i,betai in enumerate(beta):
       a_ads = [-math.log(2*math.sinh(hbar*wm*betai/2)) for wm in wmads]
       logpreads_qq = math.fsum(a_ads)
       if (N_atomsum_gas == 1):
           logprets_qq = 0.0
       else:
           a_ts = [-math.log(2*math.sinh(hbar*wt*betai/2)) for wt in wts]
           logprets_qq = math.fsum(a_ts)
       logkt_qq_des = (math.log(1.0/(2*math.pi*hbar*betai)) + logq_trans_ts[i]
                       + logq_rot_ts[i] + logprets_qq - logpreads_qq
                       - betai*deltaedes - math.log(tu))
       logkt_qq_ads = logkt_qq_des - logK_des[i]
       kt_qq_des.append(math.exp(logkt_qq_des))
       kt_qq_ads.append(math.exp(logkt_qq_ads))

    return (kt_qq_des,kt_qq_ads,DeltaU_reac,DeltaH_reac,DeltaS_reac,DeltaG_reac,
            K_reac)
       
def RateCalc_surf_fix(elements_r1,elements_p1,elements_ts,Freqs_surf_fix):
### Calculates surface reaction rate constants using fixed pre-prefactor
    kt_fix_fwd = []
    kt_fix_rev = []

    print("\nReactant: get energy")
    out_file.write("\nReactant: get energy\n")
    energy_r1,freqs_r1,zpe_r1,vibtemp_r1 = FreqCalc_ads(elements_r1,
                                                        Freqs_surf_fix)

    print("\nProduct: get energy")
    out_file.write("\nProduct: get energy\n")
    energy_p1,freqs_p1,zpe_p1,vibtemp_p1 = FreqCalc_ads(elements_p1,
                                                        Freqs_surf_fix)

    print("\nTransition state: get energy")
    out_file.write("\nTransition state: get energy\n")
    energy_ts,freqs_ts,zpe_ts,vibtemp_ts = FreqCalc_ads_ts(elements_ts,
                                                           Freqs_surf_fix)
    deltaefwd = energy_ts - energy_r1
    deltaerev = energy_ts - energy_p1
    deltaereac = energy_p1 - energy_r1
    
    deltaefwdkj = ev2kJ*deltaefwd
    deltaerevkj = ev2kJ*deltaerev
    deltaereackj = ev2kJ*deltaereac

    print("\nBarrier height forward = %(deltaefwd)8.4f eV" \
          " (%(deltaefwdkj)8.2f kJ/mol)" % vars())
    print("Barrier height reverse = %(deltaerev)8.4f eV" \
          " (%(deltaerevkj)8.2f kJ/mol)" % vars())
    print("Reaction energy forward = %(deltaereac)8.4f eV" \
          " (%(deltaereackj)8.2f kJ/mol)" % vars())
    out_file.write("\nBarrier height forward = %(deltaefwd)8.4f eV" \
          " (%(deltaefwdkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Barrier height reverse = %(deltaerev)8.4f eV" \
          " (%(deltaerevkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Reaction energy forward = %(deltaereac)8.4f eV" \
          " (%(deltaereackj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    
    beta = [1.0/(kB*Ti) for Ti in T]
    logexp_fwd = [-betai*deltaefwd for betai in beta]
    logexp_rev = [-betai*deltaerev for betai in beta]
    logkt_fix_fwd = [math.log(pre_fix) + lefwd for lefwd in logexp_fwd]
    kt_fix_fwd = [math.exp(lkt_fix_fwd) for lkt_fix_fwd in logkt_fix_fwd]
    logkt_fix_rev = [math.log(pre_fix) + lerev for lerev in logexp_rev]
    kt_fix_rev = [math.exp(lkt_fix_rev) for lkt_fix_rev in logkt_fix_rev]
    return (kt_fix_fwd,kt_fix_rev)

def RateCalc_surf_cl(elements_r1,elements_p1,elements_ts,Freqs_surf_cl):
### Calculates surface reaction rate constants using
### classical vibrational partition functions
    kt_cl_fwd = []
    kt_cl_rev = []

    print("\nReactant: get energy and frequencies")
    out_file.write("\nReactant: get energy and frequencies\n")
    energy_r1,freqs_r1,zpe_r1,vibtemp_r1 = FreqCalc_ads(elements_r1,
                                                        Freqs_surf_cl)

    print("\nProduct: get energy and frequencies")
    out_file.write("\nProduct: get energy and frequencies\n")
    energy_p1,freqs_p1,zpe_p1,vibtemp_p1 = FreqCalc_ads(elements_p1,
                                                        Freqs_surf_cl)

    print("\nTransition state: get energy and frequencies")
    out_file.write("\nTransition state: get energy and frequencies\n")
    energy_ts,freqs_ts,zpe_ts,vibtemp_ts = FreqCalc_ads_ts(elements_ts,
                                                           Freqs_surf_cl)
    deltaefwd = energy_ts - energy_r1
    deltaerev = energy_ts - energy_p1
    deltaereac = energy_p1 - energy_r1

    deltaefwdkj = ev2kJ*deltaefwd
    deltaerevkj = ev2kJ*deltaerev
    deltaereackj = ev2kJ*deltaereac
    
    print("\nBarrier height forward = %(deltaefwd)8.4f eV" \
          " (%(deltaefwdkj)8.2f kJ/mol)" % vars())
    print("Barrier height reverse = %(deltaerev)8.4f eV" \
          " (%(deltaerevkj)8.2f kJ/mol)" % vars())
    print("Reaction energy forward = %(deltaereac)8.4f eV" \
          " (%(deltaereackj)8.2f kJ/mol)" % vars())
    out_file.write("\nBarrier height forward = %(deltaefwd)8.4f eV" \
                   " (%(deltaefwdkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Barrier height reverse = %(deltaerev)8.4f eV" \
                   " (%(deltaerevkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Reaction energy forward = %(deltaereac)8.4f eV" \
                   " (%(deltaereackj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
                   
    beta = [1.0/(kB*Ti) for Ti in T]
    logexp_fwd = [-betai*deltaefwd for betai in beta]
    logexp_rev = [-betai*deltaerev for betai in beta]
    wmfwd = [2*math.pi*invcm2tau*freqfwd for freqfwd in freqs_r1]
    wmrev = [2*math.pi*invcm2tau*freqrev for freqrev in freqs_p1]
    wts = [2*math.pi*invcm2tau*freqts for freqts in freqs_ts]
    logprefwd_cl = (-math.log(2*math.pi) + math.fsum([math.log(wm)
                                                      for wm in wmfwd])
                    - math.fsum([math.log(wt) for wt in wts]))
    logprerev_cl = (-math.log(2*math.pi) + math.fsum([math.log(wm)
                                                      for wm in wmrev])
                    - math.fsum([math.log(wt) for wt in wts]))
    logkt_cl_fwd = [logprefwd_cl + lefwd - math.log(tu) for lefwd in logexp_fwd]
    logkt_cl_rev = [logprerev_cl + lerev - math.log(tu) for lerev in logexp_rev]
    kt_cl_fwd = [math.exp(lkt_cl) for lkt_cl in logkt_cl_fwd]
    kt_cl_rev = [math.exp(lkt_cl) for lkt_cl in logkt_cl_rev]
    return (kt_cl_fwd,kt_cl_rev)

def RateCalc_surf_qq(elements_r1,elements_p1,elements_ts,Freqs_surf_qq):
### Calculates surface reaction rate constants using
### quantum vibrational partition functions
    kt_qq_fwd = []
    kt_qq_rev = []
    K_reac = []

    print("\nReactant: get energy and frequencies")
    out_file.write("\nReactant: get energy and frequencies\n")
    energy_r1,freqs_r1,zpe_r1,vibtemp_r1 = FreqCalc_ads(elements_r1,
                                                        Freqs_surf_qq)

    print("\nProduct: get energy and frequencies")
    out_file.write("\nProduct: get energy and frequencies\n")
    energy_p1,freqs_p1,zpe_p1,vibtemp_p1 = FreqCalc_ads(elements_p1,
                                                        Freqs_surf_qq)

    print("\nTransition state: get energy and frequencies")
    out_file.write("\nTransition state: get energy and frequencies\n")
    energy_ts,freqs_ts,zpe_ts,vibtemp_ts = FreqCalc_ads_ts(elements_ts,
                                                           Freqs_surf_qq)
    deltaefwd = energy_ts - energy_r1
    deltaerev = energy_ts - energy_p1
    deltaereac = energy_p1 - energy_r1
    
    dezpefwd = deltaefwd + zpe_ts - zpe_r1
    dezperev = deltaerev + zpe_ts - zpe_p1
    dezpereac = deltaereac + zpe_p1 - zpe_r1

    deltaefwdkj = ev2kJ*deltaefwd
    deltaerevkj = ev2kJ*deltaerev
    deltaereackj = ev2kJ*deltaereac

    dezpefwdkj = ev2kJ*dezpefwd
    dezperevkj = ev2kJ*dezperev
    dezpereackj = ev2kJ*dezpereac

    print("\nBarrier height forward = %(deltaefwd)8.4f eV" \
          " (%(deltaefwdkj)8.2f kJ/mol)" % vars())
    print("Barrier height forward incl. ZPE = %(dezpefwd)8.4f eV" \
          " (%(dezpefwdkj)8.2f kJ/mol)" % vars())
    print("\nBarrier height reverse = %(deltaerev)8.4f eV" \
          " (%(deltaerevkj)8.2f kJ/mol)" % vars())
    print("Barrier height reverse incl. ZPE = %(dezperev)8.4f eV" \
          " (%(dezperevkj)8.2f kJ/mol)" % vars())
    print("\nReaction energy forward = %(deltaereac)8.4f eV" \
          " (%(deltaereackj)8.2f kJ/mol)" % vars())
    print("Reaction energy forward incl. ZPE = %(dezpereac)8.4f eV" \
          " (%(dezpereackj)8.2f kJ/mol)" % vars())
    out_file.write("\nBarrier height forward = %(deltaefwd)8.4f eV" \
                   " (%(deltaefwdkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Barrier height forward incl. ZPE = %(dezpefwd)8.4f eV" \
                   " (%(dezpefwdkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("\nBarrier height reverse = %(deltaerev)8.4f eV" \
          " (%(deltaerevkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Barrier height reverse incl. ZPE = %(dezperev)8.4f eV" \
          " (%(dezperevkj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("\nReaction energy forward = %(deltaereac)8.4f eV" \
          " (%(deltaereackj)8.2f kJ/mol)" % vars())
    out_file.write("\n")
    out_file.write("Reaction energy forward incl. ZPE = %(dezpereac)8.4f eV" \
          " (%(dezpereackj)8.2f kJ/mol)" % vars())
    out_file.write("\n")

    beta = [1.0/(kB*Ti) for Ti in T]
    logexp_fwd = [-betai*deltaefwd for betai in beta]
    logexp_rev = [-betai*deltaerev for betai in beta]
### Vibrations for partition functions
    wmfwd = [2*math.pi*invcm2tau*freqfwd for freqfwd in freqs_r1]
    wmrev = [2*math.pi*invcm2tau*freqrev for freqrev in freqs_p1]
    wts = [2*math.pi*invcm2tau*freqts for freqts in freqs_ts]         

    for betai in beta:
             a_fwd = [-math.log(2*math.sinh(hbar*wm*betai/2)) for wm in wmfwd]
             a_rev = [-math.log(2*math.sinh(hbar*wm*betai/2)) for wm in wmrev]
             a_ts = [-math.log(2*math.sinh(hbar*wt*betai/2)) for wt in wts]
             logprefwd_qq = math.fsum(a_fwd)
             logprerev_qq = math.fsum(a_rev)
             logprets_qq = math.fsum(a_ts)
             logkt_qq_fwd = (math.log(1.0/(2*math.pi*hbar*betai)) + logprets_qq
                             - logprefwd_qq - betai*deltaefwd - math.log(tu))
             logkt_qq_rev = (math.log(1.0/(2*math.pi*hbar*betai)) + logprets_qq
                             - logprerev_qq - betai*deltaerev - math.log(tu))
             kt_qq_fwd.append(math.exp(logkt_qq_fwd))
             kt_qq_rev.append(math.exp(logkt_qq_rev))
### Reaction thermodynamics
    U_reac,H_reac,S_reac = ThermoCalc_ads(vibtemp_r1)
    U_prod,H_prod,S_prod = ThermoCalc_ads(vibtemp_p1)
    U_ts,H_ts,S_ts = ThermoCalc_ads(vibtemp_ts)

    U_rc = numpy.array(U_reac)
    H_rc = numpy.array(H_reac)
    S_rc = numpy.array(S_reac)
    U_pd = numpy.array(U_prod)
    H_pd = numpy.array(H_prod)
    S_pd = numpy.array(S_prod)
    U_t = numpy.array(U_ts)
    H_t = numpy.array(H_ts)
    S_t = numpy.array(S_ts)

    DeltaU_reac = J2kJ*(U_pd - U_rc) + ev2kJ*deltaereac
    DeltaH_reac = J2kJ*(H_pd - H_rc) + ev2kJ*deltaereac               
    DeltaS_reac = S_pd - S_rc
    DeltaG_reac = DeltaH_reac - T*(J2kJ*DeltaS_reac)

    DeltaU_fwd = J2kJ*(U_t - U_rc) + ev2kJ*deltaefwd
    DeltaH_fwd = J2kJ*(H_t - H_rc) + ev2kJ*deltaefwd
    DeltaS_fwd = S_ts - S_rc
    DeltaG_fwd = DeltaH_fwd - T*(J2kJ*DeltaS_fwd)

    DeltaU_rev = J2kJ*(U_t - U_pd) + ev2kJ*deltaerev
    DeltaH_rev = J2kJ*(H_t - H_pd) + ev2kJ*deltaerev
    DeltaS_rev = S_ts - S_pd
    DeltaG_rev = DeltaH_rev - T*(J2kJ*DeltaS_rev)

### Equilibrium constant    
    for i,Ti in enumerate(T):
        K_rc = math.exp(-(DeltaG_reac[i]/J2kJ)/(Rgas*Ti))
        K_reac.append(K_rc)
    
    return (kt_qq_fwd,kt_qq_rev,DeltaU_reac,DeltaH_reac,DeltaS_reac,DeltaG_reac,
            DeltaU_fwd,DeltaH_fwd,DeltaS_fwd,DeltaG_fwd,DeltaU_rev,DeltaH_rev,
            DeltaS_rev,DeltaG_rev,K_reac)

def fit_rate(k):
### Fits rate constants to Arrhenius-type parameters: A, B, E_a              
      x=numpy.array(T)
      y=numpy.array([numpy.log(kt) for kt in k])

      logx=numpy.array([numpy.log(xi) for xi in x])
      xminv=numpy.array([-1.0/xi for xi in x])

      A=numpy.vstack([numpy.ones(len(x)),logx,xminv]).T
      result = numpy.linalg.lstsq(A, y)[0]
      arrhenius = result.tolist()
      Ai = numpy.exp(arrhenius[0])
      Bi = arrhenius[1]
      Ei = arrhenius[2]
      Ei_kJ = Ei*ev2kJ/ev2K
      k_fit = [Ai*Ti**Bi*math.exp(-Ei/Ti) for Ti in T]
      return (Ai,Bi,Ei_kJ,k_fit)

prompt = 'REMARC>'

try:
    infilename = sys.argv[1]
except:
    print ("Usage:",sys.argv[0], "infile"); sys.exit(1)

### Open output file
out_file = open('remarc.out', 'w')

### Read input for rate calculations.
### Automatic if input file remarc.in exists, otherwise interactive.
if os.path.isfile("remarc.in"):
    reac_input = open('remarc.in', 'r')
    input_lines = reac_input.readlines()
    indata = []
    for il in input_lines:
       item=il.strip().split(' ')
       indata.append(item[0])
    T_s = float(indata[0])
    T_e = float(indata[1])
    DT = float(indata[2])
    E_unc = float(indata[3])
    E_unc_ads = float(indata[4])
    P_a = float(indata[5])
    Pinp = P_a*atm2Pprgm
    Freqs = indata[6]
    Freqsurf = indata[7]
    PartFunc = indata[8]
    pre_fix = float(indata[9])
    pre_fix_des = float(indata[10])
    reac_input.close()
    out_file.write("Read from input file remarc.in:\n")
    out_file.write("Start temperature (K):\n")
    out_file.write(indata[0])
    out_file.write("\n")
    out_file.write("End temperature (K):\n")
    out_file.write(indata[1])
    out_file.write("\n")
    out_file.write("Temperature step size (K):\n")
    out_file.write(indata[2])
    out_file.write("\n")
    out_file.write("Typical uncertainty in barrier height (eV):\n")
    out_file.write(indata[3])
    out_file.write("\n")
    out_file.write("Typical uncertainty in adsorption energy (eV):\n")
    out_file.write(indata[4])
    out_file.write("\n")
    out_file.write("Pressure (in atm):\n")
    out_file.write(indata[5])
    out_file.write("\n")
    out_file.write("Use frequencies for rate calculations? (Y or N)\n")
    out_file.write(indata[6])
    out_file.write("\n")
    out_file.write("Use frequencies for clean surfaces? (Y or N)\n")
    out_file.write(indata[7])
    out_file.write("\n")
    out_file.write("Quantum or classical partion functions for rate calculations?" \
              "(Q or C)\n")
    out_file.write(indata[8])
    out_file.write("\n")
    out_file.write("Fixed prefactor for surface reactions (Default: 1E13 s-1):\n")
    out_file.write(indata[9])
    out_file.write("\n")
    out_file.write("Fixed prefactor for desorption (Default: 1E13 s-1):\n")
    out_file.write(indata[10])
    out_file.write("\n") 
else:
    print("No input file remarc.in found!")
    out_file.write("No input file remarc.in found!\n")
    print("Interactive mode!")
    out_file.write("Interactive mode!\n")
    print("Start temperature (K)?")
    out_file.write("Start temperature (K)?\n")
    T_start = input(prompt)

    T_s = float(T_start)
    T_start_str = str(T_start)
    out_file.write(T_start_str)
    out_file.write("\n")
    print("End temperature (K)?")
    out_file.write("End temperature (K)?\n")
    T_end = input(prompt)
    T_e = float(T_end)
    T_end_str = str(T_end)
    out_file.write(T_end_str)
    out_file.write("\n")
    print("Temperature step size (K)?")
    out_file.write("Temperature step size (K)?\n")
    DeltaT = input(prompt)
    DT = float(DeltaT)
    DeltaT_str = str(DeltaT)
    out_file.write(DeltaT_str)
    out_file.write("\n")
    print("Typical uncertainty in barrier height (eV)?")
    out_file.write("Typical uncertainty in barrier height (eV)?\n")
    E_uncert = input(prompt)
    E_unc = float(E_uncert)
    E_uncert_str = str(E_uncert)
    out_file.write(E_uncert_str)
    out_file.write("\n")
    print("Typical uncertainty in adsorption energy (eV)?")
    out_file.write("Typical uncertainty in adsorption energy (eV)?\n")
    E_uncert_ads = input(prompt)
    E_unc_ads = float(E_uncert_ads)
    E_uncert_ads_str = str(E_uncert_ads)
    out_file.write(E_uncert_ads_str)
    out_file.write("\n")
    print("Pressure (in atm)?")
    out_file.write("Pressure (in atm)?\n")
    P_atm = raw_input(prompt)
    P_a = float(P_atm)
    P_atm_str = str(P_atm)
    out_file.write(P_atm_str)
    out_file.write("\n")
    Pinp = P_a*atm2Pprgm
    print("Use frequencies for rate calculations? (Y or N)")
    out_file.write("Use frequencies for rate calculations? (Y or N)\n")
    Freqs = raw_input(prompt)
    out_file.write(Freqs)
    out_file.write("\n")
    if (Freqs == 'N' or Freqs == 'n'):
        print("Use default prefactor for surface reactions (1E13 s-1)?" \
              " (Y or N)")
        out_file.write("Use default prefactor for surface reactions (1E13 s-1)?" \
              " (Y or N)\n")
        Prefact = raw_input(prompt)
        out_file.write(Prefact)
        out_file.write("\n")
        if (Prefact == 'N' or Prefact == 'n'):
            print("Enter prefactor:")
            out_file.write("Enter prefactor:\n")
            pre_user = raw_input(prompt)
            pre_user_str = str(pre_user)
            out_file.write(pre_user_str)
            pre_fix = float(pre_user)
        print("Use default prefactor for desorption (1E13 s-1)?" \
              " (Y or N)")
        out_file.write("Use default prefactor for desorption (1E13 s-1)?" \
              " (Y or N)\n")
        Prefact = raw_input(prompt)
        out_file.write(Prefact)
        out_file.write("\n")
        if (Prefact == 'N' or Prefact == 'n'):
            print("Enter prefactor:")
            out_file.write("Enter prefactor:\n")
            pre_user = raw_input(prompt)
            pre_user_str = str(pre_user)
            out_file.write(pre_user_str)
            out_file.write("\n")
            pre_fix_des = float(pre_user)
    else:
        print("Quantum or classical partion functions for rate calculations?" \
              "(Q or C)")
        out_file.write("Quantum or classical partion functions for rate "\
              "calculations? (Q or C)\n")
        PartFunc = raw_input(prompt)
        out_file.write(PartFunc)
        out_file.write("\n")
        print("Use frequencies for clean surfaces? (Y or N)")
        out_file.write("Use frequencies for clean surfaces? (Y or N)\n")
        Freqsurf = raw_input(prompt)
        out_file.write(Freqsurf)
        out_file.write("\n")

out_file.write("\nOutput from RMC-rate:\n")

NumT = int((T_e - T_s)/(DT)+1)
T_old = T_s - DT
T = []
for i in range(NumT):
    T.append(T_old + DT)
    T_old = T_old + DT

### Calculate estimated uncertainties in rate constants
f_unc = [math.exp(E_unc/(kB*Ti)) for Ti in T]
uncertainty = [(100. * (fi - 1.)) for fi in f_unc] 
f_unc_ads = [math.exp(E_unc_ads/(kB*Ti)) for Ti in T]
uncertainty_ads = [(100. * (fi - 1.)) for fi in f_unc_ads]

### Read information from file containing energy, frequency and structure data
initial = open( infilename, 'r')
lines = initial.readlines()
initial.close()

### Initialize
gasspec = []
elmts = []
sites = []
sitedens = []
surfspec = []
sitespec = []
reacspecs = []
surfreacs = []
surfarrA = []
surfarrB = []
surfarrE = []
desarrA = []
desarrB = []
desarrE = []
gasarrA = []
gasarrB = []
gasarrE = []

ispec = 0
print("\n====================================================")
print("SEARCHING FOR ADSORBED SPECIES AND SURFACE REACTIONS")
print("====================================================\n")
out_file.write("\n====================================================\n")
out_file.write("SEARCHING FOR ADSORBED SPECIES AND SURFACE REACTIONS")
out_file.write("\n====================================================\n")
out_file.write("\n")

for line in lines:
  elements = line.split("\t")
### Find gas phase species
  if (elements[0] == 'gasphase'):
     elements_g = elements
     Gas = elements_g[2]
     Gasup = Gas.upper()
     if Gasup not in gasspec:
        gasspec.append(Gasup)
     elmtsgas0 = re.split(r'(\d+)',elements_g[1])
     for e in elmtsgas0:
         if e.isalpha():
            eupgas=e.upper() 
            if eupgas not in elmts: 
               elmts.append(eupgas)
     no_atoms_gas = re.findall(r'(\d+)',elements_g[1])
     N_atoms_gas = [int(nat) for nat in no_atoms_gas]
     N_atomsum_gas = sum(N_atoms_gas)
              
### Find and process adsorbed states.
     for line in lines:
         elements_a = line.split("\t")
         if (elements_a[3] == Gas):
             Ads1 = elements_a[3]
             Site_a1 = elements_a[4]
             Surf_a1 = elements_a[0]
             Short_a1 = Site_a1[0] + Site_a1[-1]
             Short_a1 = Short_a1.upper()
             Ads1up = Ads1.upper()
             Name_ads1 = Ads1up + '(' + Short_a1 + ')'
             print("\nAdsorbed species: " + Ads1 + " at site " + Site_a1 + 
                   " at " + Surf_a1)
             out_file.write("\nAdsorbed species: " + Ads1 + " at site " +
                            Site_a1 + " at " + Surf_a1)
             out_file.write("\n")

             elmts0 = re.split(r'(\d+)',elements_a[1])
             for e in elmts0:
                 if e.isalpha():
                    eup=e.upper() 
                    if eup not in elmts: 
                       elmts.append(eup)
             print("\nHow many sites of type " + Site_a1 + " are there in the" \
                   " simulation cell?")
             NSites_a = "4"
             out_file.write("\nHow many sites of type " + Site_a1 + \
                   "  are there in the simulation cell?")
             out_file.write("\n")
             out_file.write(NSites_a)
             out_file.write("\n")
             area_cell_a = CellArea(elements_a)
             site_dens_a = SiteDens(NSites_a,area_cell_a)
             print("\nHow many adsorption sites for " + Ads1 +
                   " are there in total\nin the simulation cell for surface "
                   + Surf_a1 + " (all sites)?")
             NSites_a_tot = "4 "
             out_file.write("\nHow many adsorption sites for " + Ads1 +
                   " are there in total\nin the simulation cell for surface "
                   + Surf_a1 + " (all sites)?")
             out_file.write("\n")
             out_file.write(NSites_a_tot)
             out_file.write("\n")             
             A_site = AreaSite(NSites_a_tot,area_cell_a)
### Check if frequencies are available
### or if gas phase species is atomic
### and frequencies of adsorbed species are available
             FreqInfo = False
             if(elements_g[6].strip('[]') and elements_a[6].strip('[]')):
                 FreqInfo = True
             if(N_atomsum_gas == 1 and elements_a[6].strip('[]')):
                 FreqInfo = True
             for line in lines:
                 elements_s = line.split("\t")
                 if (elements_s[0] == Surf_a1 and elements_s[3] == 'surface'):
                    if ((Freqs != 'N' or Freqs != 'n') and FreqInfo):
### Desorption and adsorption rate constant calculation using
### quantum vibrational partition functions              
                      print("Quantum vibrational partition functions used for" \
                            " desorption and adsorption rate calculation!")
                      out_file.write("Quantum vibrational partition functions" \
                                     " used for desorption and adsorption rate" \
                                     " calculation!")
                      out_file.write("\n")

                      (k_des,k_ads,DeltaU_des,DeltaH_des,DeltaS_des,DeltaG_des,
                            K_des) = RateCalc_ads_qq(elements_a,elements_g,
                                                     elements_s,
                                                     Freqs,Freqsurf,N_atomsum_gas)
                      A_des,B_des,E_des,k_fit_des = fit_rate(k_des)
                      A_ads,B_ads,E_ads,k_fit_ads = fit_rate(k_ads)
                      surfarrA.append(A_des)
                      surfarrB.append(B_des)
                      surfarrE.append(E_des)
                      surfarrA.append(A_ads)
                      surfarrB.append(B_ads)
                      surfarrE.append(E_ads)
                    
### Print to rate constant file rate_des.out
     
                      Name_ads = '   ' + Gas +  ' => ' + Name_ads1 + '   '
                      Name_des = '   ' + Name_ads1 +  ' => ' + Gas + '   '
                      reacspecs.append(Gas)
                      reacspecs.append(Name_ads1)
                      surfreacs.append(Name_des)
                      surfreacs.append(Name_ads)
                      des_file = open('rate_des.out', 'a')
                      des_file.write(Name_des)
                      des_file.write("\n")
                      des_file.write("Arrhenius parameters A, B, E:   " \
                                     "%10.3e %7.2f %8.3f\n" % (A_des, B_des,
                                                               E_des))
                      des_file.write("  T/K     k_calc(s-1)      "
                                     "k_fit(s-1)       f_unc       "
                                     "uncertainty(%)\n")

                      for ides,Ti in enumerate(T):
                         des_file.write("%7.1f  %9.2e  %15.2e %15.4f %15.1f\n" \
                                        % (Ti, k_des[ides], k_fit_des[ides],
                                           f_unc_ads[ides],
                                           uncertainty_ads[ides]))

                      des_file.write("\n")
                      des_file.write(Name_ads)
                      des_file.write("\n")
                      des_file.write("Arrhenius parameters A, B, E:   " \
                                     "%10.3e %7.2f %8.3f\n" % (A_ads, B_ads,
                                                               E_ads))
                      des_file.write("  T/K     k_calc(s-1)      " \
                                     "k_fit(s-1)       f_unc       "\
                                     "uncertainty(%)\n")

                      for iads,Ti in enumerate(T):
                         des_file.write("%7.1f  %9.2e  %15.2e %15.4e %15.1f\n"
                                        % (Ti, k_ads[iads], k_fit_ads[iads],
                                           f_unc_ads[iads],
                                           uncertainty_ads[iads]))

                      des_file.write("\n")
                      des_file.close()
### Print to thermodynamics file thermo_des.out
                      thermo_file = open('thermo_des.out', 'a')
                      thermo_file.write(Name_des)
                      thermo_file.write("\n")
                      thermo_file.write("  T/K     DeltaU(kJ/mol)  " \
                                        "DeltaH(kJ/mol)  DeltaS(J/K mol)  "\
                                        "DeltaG(kJ/mol)     K\n")
                      for itherm,Ti in enumerate(T):
                         thermo_file.write("%7.1f  %15.2f  %14.2f %16.2f %15.2f "\
                                        "%9.2e\n"
                                        % (Ti, DeltaU_des[itherm],
                                           DeltaH_des[itherm], DeltaS_des[itherm],
                                           DeltaG_des[itherm],
                                           K_des[itherm]))
                      thermo_file.close()

                    else:
### Desorption rate constant calculation using fixed pre-factor
                      print("Lacking frequency information!\nUsing fixed " \
                            "prefactor for desorption rate calculation!")
                      out_file.write("Lacking frequency information!\nUsing " \
                            "fixed prefactor for desorption rate calculation!")
                      out_file.write("\n")
                      Freqs_d = 'N'
                      k_des = RateCalc_ads_fix(elements_a,elements_g,elements_s,
                                               Freqs_d,Freqsurf)
                      A_des,B_des,E_des,k_fit_des = fit_rate(k_des)
                      surfarrA.append(A_des)
                      surfarrB.append(B_des)
                      surfarrE.append(E_des)
                      Name_des = '   ' + Name_ads1 +  ' => ' + Gas + '   '
                      surfreacs.append(Name_des)
### Print to rate constant file rate_des.out
                      des_file = open('rate_des.out', 'a')
                      des_file.write(Name_des)
                      des_file.write("\n")
                      des_file.write("Arrhenius parameters A, B, E:   " \
                                     "%10.3e %7.2f %8.3f\n"
                                     % (A_des, B_des, E_des))
                      des_file.write("  T/K     k_calc(s-1)      " \
                                     "k_fit(s-1)       f_unc       " \
                                     "uncertainty(%)\n")

                      for ides,Ti in enumerate(T):
                         des_file.write("%7.1f  %9.2e  %15.2e %15.4e %15.1f\n"
                                        % (Ti, k_des[ides], k_fit_des[ides],
                                           f_unc_ads[ides],
                                           uncertainty_ads[ides]))

                      des_file.write("\n")
                      des_file.close()
             if Name_ads1 not in reacspecs:
                reacspecs.append(Name_ads1)
                    
             if Name_ads1 not in surfspec:
                surfspec.append(Name_ads1)
                Site_a1up = Site_a1.upper()
                if Site_a1up not in sites:
                    sites.append(Site_a1up)
                    sitedens.append(site_dens_a)
                    sitespec.append([])
                    sitespec[ispec].append(Name_ads1)
### For adsorption/desorption processes the empty site should be included
### in the list of species
                    if Short_a1 not in sites:
                        sitespec[ispec].append(Short_a1)
                    ispec = ispec + 1
                elif Site_a1up in sites:
                    jspec=sites.index(Site_a1up)
                    sitespec[jspec].append(Name_ads1)
                    if Short_a1 not in sites:
                        sitespec[jspec].append(Short_a1)

             print("\n")
             print("========================================================\n")
             out_file.write("\n")
             out_file.write("===============================================\n")
             out_file.write("\n")
                     
  if (elements[3] != 'gasphase' and elements[3] != 'surface'):
### Find and process transition state information for surface reactions.
### Identify reactant and product.
    if '-' in elements[3]:
     elements_ts = elements
     TS = elements[3]
     reacprod = elements[3].split("-")
     Reac = reacprod[0]
     Prod = reacprod[1]
     Surf = elements[0]
     Comp = elements[1]
     Site_ts = elements[4]
     Sites_reacprod = Site_ts.split("-")
     Site_r1 = Sites_reacprod[0]
     Site_p1 = Sites_reacprod[1]

     print("\nTransition state: " + TS + " at site " + Site_ts + " at " + Surf)
     print("Reactant: " + Reac + " at site " + Site_r1 + " at " + Surf)
     print("Product: " + Prod + " at site " + Site_p1 + " at " + Surf)
     out_file.write("\nTransition state: " + TS + " at site " + Site_ts +  \
                    " at " + Surf)
     out_file.write("\n")
     out_file.write("Reactant: " + Reac + " at site " + Site_r1 + " at " + Surf)
     out_file.write("\n")
     out_file.write("Product: " + Prod + " at site " + Site_p1 + " at " + Surf)
     out_file.write("\n")
     
     elmts0 = re.split(r'(\d+)',elements[1])
     for e in elmts0:
         if e.isalpha():
            eup=e.upper() 
            if eup not in elmts: 
               elmts.append(eup)
                     
### Find and process reactant information.
     for line in lines:
        elements_r = line.split("\t")
        if (elements_r[3] == Reac and elements_r[0] == Surf and
            elements_r[1] == Comp and elements_r[4] == Site_r1):
              Reac1 = Reac
              State_r1 = "ads"
              Surf_r1 = Surf
              Short_r1 = Site_r1[0] + Site_r1[-1]
              Short_r1 = Short_r1.upper()
              if ('+' not in Reac1):
                 Reac1up = Reac1.upper()
                 Name_reac1 = Reac1up + '(' + Short_r1 + ')'
                 if Name_reac1 not in reacspecs:
                    reacspecs.append(Name_reac1)

                 if Name_reac1 not in surfspec:
                    surfspec.append(Name_reac1)
                    Site_r1up = Site_r1.upper()
                    if Site_r1up not in sites:
                       sites.append(Site_r1up)
                       print("\nHow many sites of type " + Site_r1 +
                             " are there in the simulation cell?")
                       NSites_r = "4"
                       out_file.write("\nHow many sites of type " + Site_r1 +
                                      " are there in the simulation cell?")
                       out_file.write("\n")
                       out_file.write(NSites_r)
                       out_file.write("\n")
                       area_cell_r = CellArea(elements_r)
                       site_dens_r = SiteDens(NSites_r,area_cell_r)
                       sitedens.append(site_dens_r)
                       sitespec.append([])
                       sitespec[ispec].append(Name_reac1)
                       ispec = ispec + 1
                    elif Site_r1up in sites:
                       jspec=sites.index(Site_r1up)
                       sitespec[jspec].append(Name_reac1)
              elif ('+' in Reac1):
                 Sites_r1 = Site_r1.split('+')
                 Site_r1a = Sites_r1[0]
                 Site_r1b = Sites_r1[1]
                 Short_r1a = Site_r1a[0] + Site_r1a[-1]
                 Short_r1a = Short_r1a.upper()
                 Short_r1b = Site_r1b[0] + Site_r1b[-1]
                 Short_r1b = Short_r1b.upper()
                 Names_reac1 = Reac1.split('+')
                 Name_reac1a = Names_reac1[0]
                 Name_reac1a = Name_reac1a.upper()
                 Name_reac1a = Name_reac1a + '(' + Short_r1a + ')'
                 Name_reac1b = Names_reac1[1]
                 Name_reac1b = Name_reac1b.upper()
                 Name_reac1b = Name_reac1b + '(' + Short_r1b + ')'
                 Name_reac1 = Name_reac1a + ' + ' +  Name_reac1b
 
                 if Name_reac1a not in reacspecs:
                     reacspecs.append(Name_reac1a)
                 
                 if Name_reac1a not in surfspec:
                    surfspec.append(Name_reac1a)
                    Site_r1aup = Site_r1a.upper()
                    if Site_r1aup not in sites:
                       sites.append(Site_r1aup)
                       print("\nHow many sites of type " + Site_r1a +
                             " are there in the simulation cell?")
                       NSites_r = "4"
                       out_file.write("\nHow many sites of type " + Site_r1a +
                                      " are there in the simulation cell?")
                       out_file.write("\n")
                       out_file.write(NSites_r)
                       out_file.write("\n")
                       area_cell_r = CellArea(elements_r)
                       site_dens_r = SiteDens(NSites_r,area_cell_r)
                       sitedens.append(site_dens_r)
                       sitespec.append([])
                       sitespec[ispec].append(Name_reac1a)
                       ispec = ispec + 1
                    elif Site_r1aup in sites:
                       jspec=sites.index(Site_r1aup)
                       sitespec[jspec].append(Name_reac1a)
                 if Name_reac1b not in reacspecs:
                    reacspecs.append(Name_reac1b)

                 if Name_reac1b not in surfspec:
                    surfspec.append(Name_reac1b)
                    Site_r1bup = Site_r1b.upper()
                    if Site_r1bup not in sites:
                       sites.append(Site_r1bup)
                       print("\nHow many sites of type " + Site_r1b +
                             " are there in the simulation cell?")
                       NSites_r = "4"
                       out_file.write("\nHow many sites of type " + Site_r1b +
                                      " are there in the simulation cell?")
                       out_file.write("\n")
                       out_file.write(NSites_r)
                       out_file.write("\n")
                       area_cell_r = CellArea(elements_r)
                       site_dens_r = SiteDens(NSites_r,area_cell_r)
                       sitedens.append(site_dens_r)
                       sitespec.append([])
                       sitespec[ispec].append(Name_reac1b)
                       ispec = ispec + 1
                    elif Site_r1bup in sites:
                       jspec=sites.index(Site_r1bup)
                       sitespec[jspec].append(Name_reac1b)

              break

### Find and process product information.
     for line in lines:
        elements_p = line.split("\t")
        if (elements_p[3] == Prod and elements_p[0] == Surf and
            elements_p[1] == Comp and elements_p[4] == Site_p1):
              Prod1 = Prod

              Surf_p1 = Surf
              Short_p1 = Site_p1[0] + Site_p1[-1]
              Short_p1 = Short_p1.upper()
              if ('+' not in Prod1):
                Prod1up = Prod1.upper()
                Name_prod1 = Prod1up + '(' + Short_p1 + ')'
                if Name_prod1 not in reacspecs:
                   reacspecs.append(Name_prod1)

                if Name_prod1 not in surfspec:
                   surfspec.append(Name_prod1)
                   Site_p1up = Site_p1.upper()
                   if Site_p1up not in sites:
                      sites.append(Site_p1up)
                      print("\nHow many sites of type " + Site_p1 +
                            " are there in the simulation cell?")

                      NSites_p = raw_input(prompt)
                      out_file.write("\nHow many sites of type " + Site_p1 +
                                     " are there in the simulation cell?")
                      out_file.write("\n")
                      out_file.write(NSites_p)
                      out_file.write("\n")
                      area_cell_p = CellArea(elements_p)
                      site_dens_p = SiteDens(NSites_p,area_cell_p)
                      sitedens.append(site_dens_p)
                      sitespec.append([])
                      sitespec[ispec].append(Name_prod1)
                      ispec = ispec + 1
                   elif Site_p1up in sites:
                      jspec=sites.index(Site_p1up)
                      sitespec[jspec].append(Name_prod1)
              elif ('+' in Prod1):
                 Sites_p1 = Site_p1.split('+')
                 Site_p1a = Sites_p1[0]
                 Site_p1b = Sites_p1[1]
                 Short_p1a = Site_p1a[0] + Site_p1a[-1]
                 Short_p1a = Short_p1a.upper()
                 Short_p1b = Site_p1b[0] + Site_p1b[-1]
                 Short_p1b = Short_p1b.upper()
                 Names_prod1 = Prod1.split('+')
                 Name_prod1a = Names_prod1[0]
                 Name_prod1a = Name_prod1a.upper()
                 Name_prod1a = Name_prod1a + '(' + Short_p1a + ')'
                 Name_prod1b = Names_prod1[1]
                 Name_prod1b = Name_prod1b.upper()
                 Name_prod1b = Name_prod1b + '(' + Short_p1b + ')'
                 Name_prod1 = Name_prod1a + ' + ' +  Name_prod1b

                 if Name_prod1a not in reacspecs:
                    reacspecs.append(Name_prod1a)

                 if Name_prod1a not in surfspec:
                    surfspec.append(Name_prod1a)
                    Site_p1aup = Site_p1a.upper()
                    if Site_p1aup not in sites:
                       sites.append(Site_p1aup)
                       print("\nHow many sites of type " + Site_p1a +
                             " are there in the simulation cell?")
                       NSites_p = "4"
                       out_file.write("\nHow many sites of type " + Site_p1a +
                                      " are there in the simulation cell?")
                       out_file.write("\n")
                       out_file.write(NSites_p)
                       out_file.write("\n")
                       area_cell_p = CellArea(elements_p)
                       site_dens_p = SiteDens(NSites_p,area_cell_p)
                       sitedens.append(site_dens_p)
                       sitespec.append([])
                       sitespec[ispec].append(Name_prod1a)
                       ispec = ispec + 1
                    elif Site_p1aup in sites:
                       jspec=sites.index(Site_p1aup)
                       sitespec[jspec].append(Name_prod1a)
                 if Name_prod1b not in reacspecs:
                    reacspecs.append(Name_prod1b)

                 if Name_prod1b not in surfspec:
                    surfspec.append(Name_prod1b)
                    Site_p1bup = Site_p1b.upper()
                    if Site_p1bup not in sites:
                       sites.append(Site_p1bup)
                       print("\nHow many sites of type " + Site_p1b +
                             " are there in the simulation cell?")
                       NSites_p = "4"
                       out_file.write("\nHow many sites of type " + Site_p1b +
                                      " are there in the simulation cell?")
                       out_file.write("\n")
                       out_file.write(NSites_p)
                       out_file.write("\n")
                       area_cell_p = CellArea(elements_p)
                       site_dens_p = SiteDens(NSites_p,area_cell_p)
                       sitedens.append(site_dens_p)
                       sitespec.append([])
                       sitespec[ispec].append(Name_prod1b)
                       ispec = ispec + 1
                    elif Site_p1bup in sites:
                       jspec=sites.index(Site_p1bup)
                       sitespec[jspec].append(Name_prod1b)

              break

     print("\n")
     out_file.write("\n")

### Define reaction mechanism and calculate rate constants              
### Here begins the actual rate calculation

     FreqInfo = False
     if(elements_r[6].strip('[]') and elements_p[6].strip('[]') and
        elements_ts[6].strip('[]')):
         FreqInfo = True

     if ((Freqs == 'N' or Freqs == 'n') or not FreqInfo):
        print("Lacking frequency information!\nUsing fixed " \
              "prefactor for desorption rate calculation!")
        print("Fixed prefactor used for rate calculation!")
        out_file.write("Lacking frequency information!\nUsing fixed " \
              "prefactor for desorption rate calculation!")
        out_file.write("\n")
        out_file.write("Fixed prefactor used for rate calculation!")
        out_file.write("\n")
        k_fwd,k_rev = RateCalc_surf_fix(elements_r,elements_p,elements_ts,Freqs)
     elif (PartFunc == 'C' or PartFunc == 'c'):
        print("Classical vibrational partition functions used for " \
              "rate calculation!")
        out_file.write("Classical vibrational partition functions used for " \
              "rate calculation!")
        out_file.write("\n")
        k_fwd,k_rev = RateCalc_surf_cl(elements_r,elements_p,elements_ts,Freqs)
     else:
        print("Quantum vibrational partition functions used for " \
              "rate calculation!")
        out_file.write("Quantum vibrational partition functions used for " \
              "rate calculation!")
        out_file.write("\n")
        (k_fwd,k_rev,DeltaU_reac,DeltaH_reac,DeltaS_reac,DeltaG_reac,DeltaU_fwd,
             DeltaH_fwd,
             DeltaS_fwd,DeltaG_fwd,DeltaU_rev,DeltaH_rev,DeltaS_rev,DeltaG_rev,
             K_reac) = RateCalc_surf_qq(elements_r,elements_p,elements_ts,Freqs)

     A_fwd,B_fwd,E_fwd,k_fit_fwd = fit_rate(k_fwd)
     A_rev,B_rev,E_rev,k_fit_rev = fit_rate(k_rev)
     surfarrA.append(A_fwd)
     surfarrB.append(B_fwd)
     surfarrE.append(E_fwd)
     surfarrA.append(A_rev)
     surfarrB.append(B_rev)
     surfarrE.append(E_rev)
### Calculate reaction thermodynamics for reverse reaction
     DeltaU_reac_rev = -DeltaU_reac
     DeltaH_reac_rev = -DeltaH_reac
     DeltaS_reac_rev = -DeltaS_reac
     DeltaG_reac_rev = -DeltaG_reac
     K_reac_rev = [K_re**-1 for K_re in K_reac]

### Print to rate constant file rate_surf.out
     
     Name_fwd = '   ' + Name_reac1 +  ' => ' + Name_prod1
     Name_rev = '   ' + Name_prod1 +  ' => ' + Name_reac1
     surfreacs.append(Name_fwd)
     surfreacs.append(Name_rev)
     reac_file = open('rate_surf.out', 'a')
     reac_file.write(Name_fwd)
     reac_file.write("\n")
     reac_file.write("Arrhenius parameters A, B, E:   %10.3e %7.2f %8.3f\n"
                     % (A_fwd, B_fwd, E_fwd))
     reac_file.write("  T/K     k_calc(s-1)      k_fit(s-1)       f_unc       "\
                     "uncertainty(%)\n")

     for ifwd,Ti in enumerate(T):
         reac_file.write("%7.1f  %9.2e  %15.2e %15.4e %15.1f\n"
                         % (Ti, k_fwd[ifwd], k_fit_fwd[ifwd], f_unc[ifwd],
                            uncertainty[ifwd]))

     reac_file.write("\n")
     reac_file.write(Name_rev)
     reac_file.write("\n")
     reac_file.write("Arrhenius parameters A, B, E:   %10.3e %7.2f %8.3f\n"
                     % (A_rev, B_rev, E_rev))
     reac_file.write("  T/K     k_calc(s-1)      k_fit(s-1)       f_unc       "\
                     "uncertainty(%)\n")

     for irev,Ti in enumerate(T):
         reac_file.write("%7.1f  %9.2e  %15.2e %15.4e %15.1f\n"
                         % (Ti, k_rev[irev], k_fit_rev[irev], f_unc[irev],
                            uncertainty[irev]))

     reac_file.write("\n")
     reac_file.close()

### Print to thermodynamics file thermo_barrier.out
     thermo_file = open('thermo_barrier.out', 'a')
     thermo_file.write(Name_fwd)
     thermo_file.write("\n")
     thermo_file.write("  T/K     DeltaU(kJ/mol)  " \
                        "DeltaH(kJ/mol)  DeltaS(J/K mol)  "\
                        "DeltaG(kJ/mol)\n")
     for itherm,Ti in enumerate(T):
        thermo_file.write("%7.1f  %15.2f  %14.2f %16.2f "\
                        "%15.2f\n"
                        % (Ti, DeltaU_fwd[itherm],
                        DeltaH_fwd[itherm], DeltaS_fwd[itherm], 
                        DeltaG_fwd[itherm]))
     thermo_file.write("\n")
     thermo_file.write(Name_rev)
     thermo_file.write("\n")
     thermo_file.write("  T/K     DeltaU(kJ/mol)  " \
                        "DeltaH(kJ/mol)  DeltaS(J/K mol)  "\
                        "DeltaG(kJ/mol)\n")
     for itherm,Ti in enumerate(T):
        thermo_file.write("%7.1f  %15.2f  %14.2f %16.2f "\
                        "%15.2f\n"
                        % (Ti, DeltaU_rev[itherm],
                        DeltaH_rev[itherm], DeltaS_rev[itherm],
                        DeltaG_rev[itherm]))
     thermo_file.write("\n")
     thermo_file.close()
### Print to thermodynamics file thermo_reac.out
     thermo_file = open('thermo_reac.out', 'a')
     thermo_file.write(Name_fwd)
     thermo_file.write("\n")
     thermo_file.write("  T/K     DeltaU(kJ/mol)  " \
                        "DeltaH(kJ/mol)  DeltaS(J/K mol)  "\
                        "DeltaG(kJ/mol)     K\n")
     for itherm,Ti in enumerate(T):
        thermo_file.write("%7.1f  %15.2f  %14.2f %16.2f %15.2f "\
                        "%9.2e\n"
                        % (Ti, DeltaU_reac[itherm],
                        DeltaH_reac[itherm], DeltaS_reac[itherm],
                        DeltaG_reac[itherm],
                        K_reac[itherm]))
     thermo_file.write("\n")
     thermo_file.write(Name_rev)
     thermo_file.write("\n")
     thermo_file.write("  T/K     DeltaU(kJ/mol)  " \
                        "DeltaH(kJ/mol)  DeltaS(J/K mol)  "\
                        "DeltaG(kJ/mol)     K\n")
     for itherm,Ti in enumerate(T):
        thermo_file.write("%7.1f  %15.2f  %14.2f %16.2f %15.2f "\
                        "%9.2e\n"
                        % (Ti, DeltaU_reac_rev[itherm],
                        DeltaH_reac_rev[itherm], DeltaS_reac_rev[itherm],
                        DeltaG_reac_rev[itherm],
                        K_reac_rev[itherm]))
     thermo_file.write("\n")
     print("\n==============================================================\n")
     out_file.write("\n=====================================================\n")
     out_file.write("\n")
     
print("\n=========================================")
print("DESORPTION AND SURFACE REACTIONS FINISHED")
print("=========================================\n")
out_file.write("\n=========================================\n")
out_file.write("DESORPTION AND SURFACE REACTIONS FINISHED")
out_file.write("\n=========================================\n")
  
###  Print to CHEMKIN II file

ck_file = open('Chemkin.inp', 'w')
ck_file.write('! CHEMKIN II file produced by REMARC for T = %7.2f - %7.2f K ' \
              'and P = %6.2f atm\n'
              % (T_s, T_e, P_a))
ck_file.write('! Rate constant units are: s, cm, molecules, kJ/mole\n')
ck_file.write("   ELEMENTS ")
i=0
for el in elmts:
   i=i+1
   ck_file.write("%s " % el)
   if i==10:
      ck_file.write("\n")   
      i=0
ck_file.write("\n")   
ck_file.write("   END\n")
ck_file.write("   SPECIES\n")
i=0
for gspec in gasspec:
    i=i+1
    ck_file.write("   ")
    ck_file.write("%s " %gspec)
    if i==10:
        ck_file.write("\n")
        i=0
ck_file.write("\n")   
ck_file.write("   END\n")
ck_file.write("\n")   
ck_file.write("   REACTIONS  KJOULES/MOLE  MOLECULES\n")
ck_file.write("\n")

ck_file.write("   END \n")
ck_file.close()

###  Print to SURFACE CHEMKIN file

sck_file = open('SurfaceChemkin.inp', 'w')
sck_file.write('! SURFACE CHEMKIN file produced by REMARC for ' \
               'T = %7.2f - %7.2f K and P = %6.2f atm\n' \
               % (T_s, T_e, P_a))
sck_file.write('! Rate constant units are: s, kJ/mole\n')

for jsite,st in enumerate(sites):
   sck_file.write("   SITE ")
   sck_file.write("/ " + st + " / ")
   sck_file.write(" SDEN/" + "%9.2e" % sitedens[jsite])
   sck_file.write("/\n")
   sck_file.write("    ")
   i=0
   jspec=sites.index(st)
   for stsp in sitespec[jspec]:
      i=i+1
      sck_file.write("%s " % stsp)
      if i==10:
         sck_file.write("\n")   
         i=0
   sck_file.write("\n   END \n")

sck_file.write("\n")
sck_file.write("   ELEMENTS ")
i=0
for el in elmts:
   i=i+1
   sck_file.write("%s " % el)
   if i==10:
      sck_file.write("\n")   
      i=0
sck_file.write("\n")   
sck_file.write("   END\n")

sck_file.write("\n")
sck_file.write("   SPECIES\n")

i=0
sck_file.write("    ")
for spec in reacspecs:
    i=i+1
    sck_file.write("%s " %spec)
    if i==10:
        sck_file.write("\n")   
        i=0

sck_file.write("\n")   
sck_file.write("   END\n")
sck_file.write("\n")   

sck_file.write("\n")   
sck_file.write("   REACTIONS ! KJOULES/MOLE\n")
sck_file.write("\n")

for ireac,sr in enumerate(surfreacs):
    sck_file.write("   " + surfreacs[ireac]  + "%15.3e %7.2f %8.3f\n"
                   % (surfarrA[ireac], surfarrB[ireac], surfarrE[ireac]))

sck_file.write("   END \n")
sck_file.close()
