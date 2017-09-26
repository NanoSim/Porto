#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os
import sys
import re
import math
import warnings
import json
import ast

from vasp_outcar import CONTCAR, OUTCAR

thisdir = os.path.abspath(os.path.dirname(__file__))
entitydir = os.path.join(thisdir, '../entities')

try:
    import softpy
    HAVE_SOFT = True
    BaseDFTData = softpy.entity(
        open(os.path.join(entitydir, 'eu.nanosim.vasp.dftdata.json')))
except ImportError:
    warnings.warn('SOFT is not available')
    HAVE_SOFT = False
    BaseDFTData = object

if sys.version_info.major >= 3:
    basestring = str


class VASP_DATA(object):
    """
    Read data from OUTCAR-files.
    The root directory with subdirectories must be set up in as specific
    manner, for info see manual.
    The CONTCAR and OUTCAR file must be in the corresponding directories.
    The output data is placed in a text file, which can be read by RMC_rate.

    Parameters
    ----------
    rootdir: string
        Path to the main directory
    outcarfile: string
        The data is read from an OUTCAR-file (VASP-format) named OUTCAR
    contcarfile: string
        The data is read from a CONTCAR-file (VASP-format) named CONTCAR
    phononpyfile: string
        The vibrational data is read from a phononpy file named 'mash.yaml' if
        present
    """
    def __init__(self, rootdir, gasphase_directory):
        self.rootdir = rootdir
        self.gasphase_directory= gasphase_directory
        self.outcarfile = 'OUTCAR'
        self.contcarfile = 'CONTCAR'
        self.phononpyfile = 'mesh.yaml'

        # This makes this class a SOFT entity object
        #if HAVE_SOFT:
        #    self.__soft_entity__ = softpy.entity_t(
        #        get_meta_name='vaspdata',
        #        get_meta_version='0.1',
        #        get_meta_namespace='eu.nanosim.vasp',
        #        get_dimensions=[],
        #        get_dimension_size=lambda e, label: 0,
        #        store=self.store,
        #        load=self.load,
        #        id=None,
        #        user_data=self,
        #    )

        self.gasphases = [
            DFTData(*ph.split('\t')) for ph in self.get_gasphaselist()]


#    def store(self, e, datamodel):
#        """Stores self into datamodel.
#
#        Confirms to eu.nanosim.vasp.vaspdata version 0.1
#
#        This function is used by softpy.Storage and normally not
#        called directly.
#
#        The current use of string lists is not useful.  The data layout will
#        drastically change in future version.
#        """
#        systems = self.sysdir_check()
#        surface_name = systems[0] if isinstance(systems[0], str) else ''
#        solid_name = systems[1] if isinstance(systems[1], str) else ''
#
#        softpy.datamodel_append_string(datamodel, 'surface_name', surface_name)
#        softpy.datamodel_append_string(datamodel, 'solid_name', solid_name)
#
#        gasphaselist = self.get_gasphaselist()
#        gasdata = PhaseData('gasphase', gasphaselist)
#        ## gassphase
#        #surface_names = []
#        #compositions = []
#        #species_names = []
#        #states = []
#        #site_names = []
#        #total_energies = []
#        #freq_list = []
#        #cells = []
#        #pos_list = []
#        #info_list = []
#        #for (surface_name, composition, species_name,
#        #     state, site_name, total_energy, frequencies,
#        #     cell, positions, info) in self.get_gasphaselist():
#        #    surface_names.append(surface_name)
#        #    compositions.append(composition)
#        #    species_names.append(species_name)
#        #    states.append(state)
#        #    site_names.append(site_name)
#        #    total_energies.append(total_energy)
#        #    freq_list.append(frequencies)
#        #    cells.append(cell)
#        #    pos_list.append(positions)
#        #    info_list(info)
#        #print('*** gassphase ***')
#        #print(surface_names)
#        #softpy.datamodel.append_string_list('gassphase_surface_names'
#
#        softpy.datamodel_append_string_list(
#            datamodel, 'gasphase', gasphaselist)
#
#        if systems[0]:
#            surface = self.get_systemlist(system='surface')
#            adsorbate = self.get_statelist(system='surface')
#            surface_ts = self.get_tslist(system='surface')
#        else:
#            surface = ['']
#            adsorbate = ['']
#            surface_ts = ['']
#        softpy.datamodel_append_string_list(datamodel, 'surface', surface)
#        softpy.datamodel_append_string_list(datamodel, 'adsorbate', adsorbate)
#        softpy.datamodel_append_string_list(datamodel, 'surface_ts', surface_ts)
#
#        if systems[1]:
#            solid = self.get_systemlist(system='solid')
#            bulk = self.get_statelist(system='solid')
#            solid_ts = self.get_tslist(system='solid')
#        else:
#            solid = ['']
#            bulk = ['']
#            solid_ts = ['']
#        softpy.datamodel_append_string_list(datamodel, 'solid', solid)
#        softpy.datamodel_append_string_list(datamodel, 'bulk', bulk)
#        softpy.datamodel_append_string_list(datamodel, 'solid_ts', solid_ts)
#
#    def load(self, e, datamodel):
#        """Loads datamodel into self.
#
#        This function is used by softpy.Storage and normally not
#        called directly.
#        """
#        raise NotImplementedError('loading data is not supported')


    def sysdir_check(self):
        """
        Checks if directories "surface" and/or "solid " with vasp
        calculations are present under rootdir
        Returns a list of directories
        """

        rootdir = self.rootdir
        surface_dir = os.path.join(rootdir,'surface')
        solid_dir = os.path.join(rootdir, 'solid')

        systems =[surface_dir, solid_dir]
        fsystems = []

        for s in systems:
            if os.path.isdir(s):
                clean_dir = os.path.join(s, 'clean')
                if os.path.isdir(clean_dir):
                    if os.path.isfile(
                        os.path.join(
                            clean_dir, self.contcarfile)) and os.path.isfile(
                                os.path.join(clean_dir, self.outcarfile)):
                        fsystems.append(clean_dir)
                    else:
                        raise MissingFileError(
                            '--- No  CONTCAR and or OUTCAR file found for ' +
                            clean_dir)
                else:
                    raise MissingDirError(
                        '--- the "clean" subdirectory must exists under '
                        'directory: ' + s)
            else:
                fsystems.append(None)

        if fsystems[0] is None and systems[1] is None:
            # exit the program if there is no surface directory is found
            raise MissingDirError(
                'the "surface" or "solid" subdirectory must exists under '
                'rootdirectory: ' + rootdir)

        else:
            return fsystems



    def gasphasedir_check(self):
        """
        Checks if directory with calculations of the gas species is present.
        Returns a directory, and a liste of gas phase species.
        gasphase_directory: string
        molecules: list
        """

        molecules = []

        gasphase_dir= self.gasphase_directory

        if os.path.isdir(gasphase_dir):
            names = os.listdir(gasphase_dir)

            # append directories under gasphase_directory to list of molecules
            for i in range(0, len(names)):
                if os.path.isdir(os.path.join(gasphase_dir,str(names[i]))):
                    molecules.append(names[i])
            print('\nDirectories containing gasphase species: ')
            print(str(molecules))

        # if no directories containing gasphase species is found
        # return empty string and list
        else:
            gasphase_dir = False
            print('\nWarning --- No directory with gasphase species found.')
        return gasphase_dir, molecules


    def statedir_check(self, system=None):
        """
        Checks if directories with adsorbate/bulk structure calculations are
        present.
        Return a list of adsorbate directories.
        The root directory with subdirectories  must be set correctly, for
        info see manual.
        """
        state_directory = os.path.join(self.rootdir, system)
        names = os.listdir(state_directory)
        states = []

        # Remove directories containing calculations of clean surface,
        # gas phase species and transition states
        for i in range(0, (len(names))):
            if os.path.isdir(os.path.join(state_directory, str(names[i]))):
                states.append(str(names[i]))

        if 'clean' in os.listdir(state_directory):
            states.remove('clean')
        #if 'gasphase' in os.listdir(adsorbate_directory):
        #    adsorbates.remove('gasphase')
        if 'ts' in os.listdir(state_directory):
            states.remove('ts')

        print('\nDirectories containing adsorbed/bulk structures found: ')
        print(str(states))

        return state_directory, states


    def tsdir_check(self, ts_directory = 'ts', system=None):
        """
        Checks if directory 'ts' with transition state calculations are present.
        Return the transition states directory and list of transition states.
        ts_directory: string
        transition_states : list
        """

        transition_states = []

        if ts_directory in os.listdir(os.path.join(self.rootdir, system)):
            ts_dir = os.path.join(self.rootdir, system, ts_directory)
            names = os.listdir(ts_dir)

            for i in range(0, (len(names))):
                if os.path.isdir(os.path.join(ts_dir, str(names[i]))):
                    transition_states.append(str(names[i]))
            print('\nDirectories containing transition states found: ')
            print(str(transition_states))

            self.tscheck(self.rootdir, system=system)


        else:
            print('\nWarning --- No directory with transition state found')
            ts_dir = False
        return ts_dir, transition_states


    def tscheck(self, verbose=False, system=None):
        """Walks through the "ts" subdirectory of `rootdir` and checks that
        needed directories exists.
        """
        rootdir = os.path.join(self.rootdir, system)

        #if verbose:
        #    print('Checking for rootdir: ' + str(rootdir))
        #if not os.path.isdir(rootdir):
        #    raise MissingDirError('missing root directory: ' + str(rootdir))

        tsdir = os.path.join(rootdir, 'ts')
        #if verbose:
        #    print('Checking for ts directory: ' + str(tsdir))
        #if not os.path.isdir(tsdir):
        #    raise MissingDirError(
        #        'the "ts" subdirectory must exists under root directory: ' +
        #        rootdir)

        for transition in os.listdir(tsdir):
            transitiondir = os.path.join(tsdir, transition)


            # Skip subdirs containing a file named "SKIP"
            if os.path.isfile(os.path.join(transitiondir, 'SKIP')):
                if verbose:
                    print("Skipping subdir: " + transitiondir)
                    continue

            states = transition.split('-')

            if len(states) != 2:
                raise DirNameError(
                    'ts subdirs must contain 1 hyphen, separating the names '
                    'of the initial and final states: ' + transition)
            if verbose:
                print('\nChecking dependencies for transition: ' + transition)

            for stateno, state in enumerate(states):
                statedir = os.path.join(rootdir, state)

                if not os.path.isdir(statedir):
                    raise MissingDirError(
                        'Missing directory for state %s of transition %s: %s' %
                        (state, transition, statedir))

                # CHECK RULES for compnumdir
                components = state.split('+')
                compnumdir = os.path.join(
                    statedir, '+'.join(['1'] * len(components)))


                if not os.path.isdir(compnumdir):
                    compnumdir = os.path.join(
                        statedir, str(len(components)))


                for tssites in os.listdir(transitiondir):
                    tssitesdir = os.path.join(transitiondir, tssites)
                    if len(tssites.split('-')) != 2:
                        raise DirnameError(
                            'transition sites directories must contain 1 '
                            'hyphen, '
                            'separating the sites of the initial and final '
                            'state: %s' % tssitesdir)
                    site = tssites.split('-')[stateno]
                    sitedir = os.path.join(compnumdir, site)

                    if not os.path.isdir(sitedir):
                        raise MissingDirError(
                            'Missing site directory for transition %s: %s' %
                            (transition, sitedir))
                    if verbose:
                        print('   found sites directory for state %s: %s' %
                              (state, sitedir))
            if verbose:
                print('    --- OK')



    def get_gasphaselist(self):
        ''' Return data for gas phase species.
        molecule_list : list
        '''

        print('\n************ GAS PHASE SPECIES ****************')

        molecule_directory, molecules = self.gasphasedir_check()

        molecule_list = []

        # end if there is no elements in gasphase directory
        if len(molecules) == 0 and molecule_directory is not False:
            print('\n Warning --- no gasphase species found under ' +
                  molecule_directory)
            return molecule_list
        elif molecule_directory is False:
            return molecule_list

        # Data for each gasphase species is placed in molecule_list

        for molecule in molecules:
            mol_directory = os.path.join(molecule_directory, molecule)

            # get the variable for gas phase species
            variables = self.get_variables(mol_directory, gasphase=True)
            variables_text = '\t'.join([str(w) for w in variables])
            molecule_list.append(variables_text)
        return molecule_list



    def get_systemlist(self, system=None, systemdir=None):
        """
        Returns data for the surface
        surface_list : list
        """

        sysdir = self.sysdir_check()

        if system == 'surface':
            print("\n************ SYSTEM - surface *******************")
            surface_list = []
            variables = self.get_variables(sysdir[0], surface=True)
            variables_text = '\t'.join([str(w) for w in variables])
            surface_list.append(variables_text)
            return surface_list
        if system == 'solid':
            print("\n************ SYSTEM - solid *******************")
            solid_list = []
            variables = self.get_variables(sysdir[1], solid=True)
            variables_text = '\t'.join([str(w) for w in variables])
            solid_list.append(variables_text)
            return solid_list


    def get_statelist(self, system=None):
        """
        Returns a list of data from calculations of adsorbed/bulk species
        on the surface/solid

        """

        if system == 'surface':
            print("\n************ ADSORBATE STRUCTURES ****************")
        if system == 'solid':
            print("\n************ BULK STRUCTURES ****************")


        #get all folder in the gasphase directory
        state_directory, states = self.statedir_check(system=system)

        # end if there is no elements in adsorbate_directory
        if len(states) == 0:
            print("\n Warning --- No adsorbed/bulk structures found")
            return

        state_list = []

        # Go through the subdirectories
        for state in states:
            n_state_dir = os.path.join(state_directory, state)
            n_state = os.listdir(n_state_dir)

            for n in n_state:
                site_dir = os.path.join(n_state_dir, n)
                if os.path.isdir(site_dir):
                    sites = os.listdir(site_dir)

                    for site in sites:
                        state_dir = os.path.join(site_dir, site)
                        if os.path.isdir(os.path.join(site_dir, site)): #config_dir + '/' + config):

                            # get the variable for the adsorbed species
                            variables = self.get_variables(
                                state_dir, adsorbate=True,
                                species = state, config=site,
                                system=system)
                            variables_text = '\t'.join(
                                [str(w) for w in variables])
                            state_list.append(variables_text)
        return state_list


    def get_tslist(self, system=None):
        """
        Returns the calculated data regarding the transition states.
        Transition states data calculated using (CI-)NEB or dimer method can
        extracted
        Some updated are needed.
        """

        print("\n************ TRANSITION STATES ****************")

        # get the surface atom species and numbers
        #surface_atoms = self.get_surface_atoms()

        transition_state_dir, transition_states = self.tsdir_check(
            system=system)
        transition_state_list = []

        for transition_state in transition_states:
            # Get list of transition state configurations for ts
            config_dir = os.path.join(transition_state_dir, transition_state)
            configs = os.listdir(config_dir)

            for config in configs:
                ts_directory = os.path.join(config_dir, config)
                if os.path.isdir(ts_directory):
                    names = []

                    for i in os.listdir(ts_directory):
                        if os.path.isdir(ts_directory + '/' + i):
                            names.append(i)


                    # Check if dimer method is used
                    # If dim is present, results from dimer method will be used
                    if 'dim' in names:
                        if os.path.isfile(os.path.join(
                                ts_directory, 'dim', self.outcarfile)):
                            ts_dir = os.path.join(ts_directory, 'dim')
                        else:
                            print(
                                '    WARNING --- No OUTCAR/CONTCAR file found')
                            break

                    # Check if CI-NEB method is used
                    elif len(names) > 1:
                        # make a list with image number and energies
                        # the image with the highest energy is treated as the
                        # transition stat
                        # this can be changede
                        image_list = []
                        energy_list = []

                        for image in names:
                            if (image.startswith('0') or
                                image.startswith('1') or
                                image.startswith('2')):
                                # Make script for OUTCAR is .gz file

                                if os.path.isfile(os.path.join(
                                        ts_directory, image, self.outcarfile)):
                                    outcar = OUTCAR(os.path.join(
                                        ts_directory, image, self.outcarfile))
                                else:
                                    print('    WARNING --- No OUTCAR/CONTCAR '
                                          'file found')
                                    break

                                tot_energy = outcar.read_energy()
                                energy_list.append(tot_energy)
                                image_list.append(image)

                        # Find transition state energy and image number
                        index = energy_list.index(max(energy_list))

                        # Transition state image
                        ts_dir = os.path.join(ts_directory, image_list[index])
                    # The transisition state calculation is directly under
                    # the config_dir
                    elif os.path.isfile(
                            os.path.join(ts_directory, self.outcarfile)):
                        ts_dir = ts_directory

                    # No transition state is found
                    else:
                        print('    Warning --- no transition state files '
                              'found for ' + str(ts_directory))
                        break

                # get the variable for the adsorbed species
                variables = self.get_variables(
                    ts_dir, ts=True, species=transition_state, config=config,
                    system=system)

                variables_text = '\t'.join([str(w) for w in variables])
                transition_state_list.append(variables_text)

        return transition_state_list


    def get_system_atoms(self, system=None):
        """
        Returns a list of the atom kinds and number of atoms of the surface '
        'or solid system
        atom_kind : list
        atom_number : list
        """

        if system == 'surface':
            system_directory = self.sysdir_check()[0]
        if system == 'solid':
            system_directory = self.sysdir_check()[1]

        contcar = CONTCAR(os.path.join(system_directory, self.contcarfile))

        atom_kind = contcar.read_atomkinds()
        atom_number = contcar.read_atomnumbers()

        return atom_kind, atom_number


    def get_frequencies(self, current_directory, gasphase = False):
        ''' Returns a list of vibrational frequcencies
        Vibrational frequency calculations are placed in a subdirectory
        named 'vib'
        Calculations from VASP or PHONONPY can be extracted
        If no vibrational frequencies is found an empty list is returned
        frequencies : list
        '''

        frequencies = []

        # Check if directory 'vib' exists and get frequencies
        if 'vib' in os.listdir(current_directory):
            print("   'vib' directory found")
            if os.path.isfile(os.path.join(
                    current_directory, 'vib', self.outcarfile)):
                viboutcar = OUTCAR(os.path.join(
                    current_directory, 'vib', self.outcarfile))
                frequencies = viboutcar.read_vibrational_frequencies(
                    gasphase=gasphase)
            elif os.path.isfile(os.path.join(
                    current_directory, 'vib', self.phononpyfile)):
                from vasp_outcar import PHONONPY
                viboutfile = PHONONPY(
                    os.path.join(current_directory, 'vib', self.phononpyfile))
                frequencies = viboutfile.read_vibrational_frequencies_phononpy()
        return frequencies


    def get_variables(self, current_directory, gasphase=False, surface=False,
                      solid=False, adsorbate=False, ts=False, species=None,
                      config=None, system=None):
        '''Get the variables surface_name, composition, species_name, state,
        site_name, total_energy, frequencies, cell, positions, info
        from the VASP calculations
        '''

        system_name = self.rootdir.split('/')[-1]

        print('\nChecking --- ' + str(current_directory))



        # Check if outcarfile and contcarfile ie in the current_directory
        if (os.path.isfile(os.path.join(
                current_directory, self.outcarfile)) and
            os.path.isfile(os.path.join(
                current_directory, self.contcarfile))):
            outcar = OUTCAR(os.path.join(current_directory, self.outcarfile))
            contcar = CONTCAR(os.path.join(current_directory, self.contcarfile))

            # Checks if the OUTCAR file has completed -- will be
            # substituted with outcar.check_convergence
            finished = outcar.outcar_check()
            if finished is False:
                if surface:
                    print("   ERROR --- OUTCAR is not complete --- Exiting")
                    sys.exit()
                else:
                    print("    Warning --- OUTCAR file is not complete "
                          "--- This element is skipped")
                    variables = []
                return variables

        # if no contcar or outcar is in current_directory move to next
        else:
            print("    Warning --- No OUTCAR and/or CONTCAR file found "
                  "--- This element is skipped")
            variables = []
            return variables


        # Check if calculation has converged
        convergence = outcar.check_convergence()
        if convergence:
            print('    OUTCAR file - converged')
        #else:
            #print('WARNING --- Check convergency')


        # get chemical composition
        composition = contcar.read_composition(ordered=True)
        # get the total energy of the system
        total_energy = outcar.read_energy()

        # get the atomic super cell
        cell = contcar.read_cell()
        # get the atomic position of all elements
        positions = contcar.read_atoms_and_positions()
        # info is set as the given directory
        info = current_directory

        if gasphase:
            # check if atom/molecule is in defined molecules!!!! Add this

            # Set surface, site, species names and bulk_structure for
            # gasphase species
            surface_name = 'gasphase'
            species_name = contcar.read_composition(ordered=True, name=True)
            site_name = 'None'
            state = 'gasphase'

        if surface:
            # Set surface, site, species names and bulk_structure for
            # gasphase species
            surface_name = system_name
            site_name = 'None'
            species_name = contcar.read_composition(ordered=True)
            state = 'surface'

        if solid:
            # Set surface, site, species names and bulk_structure for
            # gasphase species
            surface_name = system_name
            site_name = 'None'
            species_name = contcar.read_composition(ordered=True)
            state = 'solid'

        if adsorbate:
            surface_name = system_name
            site_name = config
            surface_atoms = self.get_system_atoms(system=system)
            species_name = contcar.read_composition(
                adsorbed=True, surface=surface_atoms, name=True)
            state = species

        if ts:
            surface_name = system_name
            site_name = config
            surface_atoms = self.get_system_atoms(system=system)
            species_name = contcar.read_composition(
                adsorbed=True, surface=surface_atoms, name=True)

            state = species


            image = []
            for i in range(0,40):
                if i < 10:
                    image.append(str(0)+str(i))
                else:
                    image.append(str(i))

            if 'dim' in current_directory.split('/'):
                print('    Dimer method - used')
            elif current_directory.split('/')[-1] in image:
                print('    (CI)-NEB method - used - image ' +
                      str(current_directory.split('/')[-1]) +
                      ' set as transition state '
                      '--- Check that this is the correct transition state')
            else:
                print('    Method for finding transition state not given')


        frequencies = self.get_frequencies(
            current_directory, gasphase = gasphase)

        # Check the number of imaginary frequencies
        j = 0
        for f in frequencies:
            if isinstance(f, complex):
                j = j + 1

        if j > 0 and ts is False:
            print("    WARNING --- " + str(j) + " imaginary frequencies found")

        elif j > 1 and ts is True:
            print("    WARNING --- " + str(j) +
                  " imaginary frequencies found - is this a true "
                  "transition state?")

        # make list containing information about surface species, species,
        # state, site, energy,
        # frequencies, cell, positions and info
        variables = [surface_name, composition, species_name, state, site_name,
                         total_energy, frequencies,
                         cell, positions, info]

        print('    ' + str(variables[0:6]))

        return variables


    def write_outputdata(self, uri, driver='txt', options=None):
        """Writes VASP data to `uri` using `driver`

        Valid drivers are all drivers supported by SOFT in addition to "txt".
        Additional options to the driver can be provided via `options`.

        The "txt" driver is decoupled from SOFT and uses `uri` as the
        output base name.  If `options` is "short", less details are written.
        """
        if driver == 'txt':
            if options is None:
                self.write_outputdata_txt(uri)
            elif options == 'short':
                self.write_outputdata_txt_short(uri)
            else:
                raise ValueError(
                    'Only option supported by the txt driver is "short", '
                    'got %r' % (options, ))
        else:
            with softpy.Storage(driver, uri, options) as s:
                s.save(self)


    def write_outputdata_txt(self, file_name):
        """
        Writes VASP data to a txt-file
        """

        # check the system
        systems = self.sysdir_check()

        #Create header
        headings = '\t'.join(['system_name',
                              'total_composition',
                              'species_name',
                              'state',
                              'site_name',
                              'total_energy',
                              'frequencies',
                              'cell',
                              'posisitions',
                              'info'])

        # Get all information for the surface and different gasphase
        # and adsorbed species
        gasphase_list = self.get_gasphaselist()

        outputdata_list = [headings] + gasphase_list


        if  systems[0] is not None:
            surface_list = self.get_systemlist(system='surface')
            adsorbate_list = self.get_statelist(system='surface')
            surfts_list = self.get_tslist(system='surface')
            outputdata_list_surface = (
                outputdata_list +  surface_list + adsorbate_list + surfts_list)

            # Put all the information togheter in a file
            #outputdata_list = [headings] + gasphase_list + surface_list +
            # adsorbate_list + transition_list
            out_surface = '\n'.join(outputdata_list_surface)

            file_basename = os.path.splitext(file_name)[0]

            # Write the information to an output file
            ofile_surf = open(file_basename + '_surface.txt', 'w')
            ofile_surf.write(out_surface)
            ofile_surf.close()

        if  systems[1] is not None:
            solid_list = self.get_systemlist(system='solid')
            bulk_list = self.get_statelist(system='solid')
            solidts_list = self.get_tslist(system='solid')
            outputdata_list_solid = (
                outputdata_list +  solid_list + bulk_list + solidts_list)

            # Put all the information togheter in a file
            #outputdata_list = [headings] + gasphase_list + surface_list +
            # adsorbate_list + transition_list
            out_solid = '\n'.join(outputdata_list_solid)

            # Write the information to an output file
            ofile_solid = open(file_basename + '_solid.txt', 'w')
            ofile_solid.write(out_solid)
            ofile_solid.close()




    def write_outputdata_txt_short(self, file_name):
        """
        Writes VASP data to a txt-file
        """

        # check the system
        systems = self.sysdir_check()


        #Create header
        headings = '\t'.join(['surface_name', 'species_name', 'state',
                            'total_energy', 'frequencies'])
        output = []
        output.append(headings)

        # Get all information for the surface and different gasphase and
        # adsorbed species
        gasphase_list = self.get_gasphaselist()


        for i in range(0, len(gasphase_list)):
            item = gasphase_list[i].split("\t")
            output.append("\t".join([item[0], item[2], item[3],
                                     item[4], item[5], item[6]]))


        if  systems[0] is not None:
            out_surf = output
            surface_list = self.get_systemlist(system='surface')
            adsorbate_list = self.get_statelist(system='surface')
            surfts_list = self.get_tslist(system='surface')

            for i in range(0, len(surface_list)):
                item = surface_list[i].split("\t")
                out_surf.append("\t".join([item[0], item[2], item[3],
                                           item[4], item[5], item[6]]))

            for i in range(0, len(adsorbate_list)):
                item = adsorbate_list[i].split("\t")
                out_surf.append("\t".join([item[0], item[2], item[3],
                                           item[4], item[5], item[6]]))

            for i in range(0, len(surfts_list)):
                item = surfts_list[i].split("\t")
                out_surf.append("\t".join([item[0], item[2], item[3],
                                           item[4], item[5], item[6]]))

            outdata_surf = '\n'.join(out_surf)

            # Write the information to an output file
            ofile_surf = open(file_name+'_surface_short.txt', 'w')
            ofile_surf.write(outdata_surf)
            ofile_surf.close()

        if  systems[1] is not None:
            out_solid = output
            solid_list = self.get_systemlist(system='solid')
            bulk_list = self.get_statelist(system='solid')
            solidts_list = self.get_tslist(system='solid')

            for i in range(0, len(solid_list)):
                item = solid_list[i].split("\t")
                out_solid.append("\t".join([item[0], item[2], item[3],
                                            item[4], item[5], item[6]]))

            for i in range(0, len(bulk_list)):
                item = bulk_list[i].split("\t")
                out_solid.append("\t".join([item[0], item[2], item[3],
                                            item[4], item[5], item[6]]))

            for i in range(0, len(solidts_list)):
                item = solidts_list[i].split("\t")
                out_solid.append("\t".join([item[0], item[2], item[3],
                                            item[4], item[5], item[6]]))

            outdata_solid = '\n'.join(out_solid)

            # Write the information to an output file
            ofile_solid = open(file_name+'_solid_short.txt', 'w')
            ofile_solid.write(outdata_solid)
            ofile_solid.close()



class DFTData(BaseDFTData):
    """DFT results for a molecule/structure.

    Arguments
    ---------
    phase_name : string
    composition : string
    species_name : string
    state : string
    site_name : string
    total_energy : string | float
    frequencies : string | sequence
    cell : 3x3 floats
    coords : sequence
        Sequence of (symbol, [x, y, z])-tuples.
    info : string
    """
    def __init__(self, phase_name, composition, species_name, state,
                 site_name, total_energy, frequencies, cell, coords, info):
        super(DFTData, self).__init__()

        fm = lambda s: ast.literal_eval(s) if isinstance(s, basestring) else s

        self.phase_name = phase_name
        self.composition = composition
        self.species_name = species_name
        self.state = state
        self.site_name = site_name
        self.total_energy = fm(total_energy)
        self.frequencies = fm(frequencies)
        self.cell = fm(cell)
        self.coords = fm(coords)
        self.symbols = [p[0] for p in self.coords]
        self.positions = [p[1:][0] for p in self.coords]
        self.info = info

    def get_atoms(self):
        """Returns an Atoms object."""
        pass



class TSCheckError(OSError):
    """Baseclass for errors in this module."""
    pass


class MissingDirError(TSCheckError):
    """Exception raised when a directory is missing."""
    pass


class DirNameError(TSCheckError):
    """Exception raised for non-conforming directory names."""
    pass


class MissingFileError(TSCheckError):
    """Exception raised when a directory is missing."""
    pass
