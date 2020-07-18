#!/usr/bin/python
import os
import sys
import subprocess

CXX      = 'g++'
AR       = 'ar'
MAKE     = 'make'
CXXFLAGS = '-std=c++17 -ggdb3 -O0 -Wall -Werror -pg'

TLD = os.path.dirname(sys.argv[0])+'/'
PWD = os.getcwd()+'/'

class Build:
    def __init__(self):
        self.input_files = []
        self.output_file = ''
        self.src_dir = ''
        self.dependencies = []
        self.external_dependencies = []
        self.target_type = 0
        self.cxxflags = ''
        self.includePaths = ''
        self.linkflags = ''
    def add_include_paths(self, p):
        self.includePaths = self.includePaths + ' ' +' '.join(['-I'+TLD+i for i in p])
    def set_cxxflags(self, f):
        self.cxxflags = f
    def set_linkflags(self, f):
        self.linkflags = f
    def set_src_dir(self, d):
        self.src_dir = TLD + d
    def add_src_files(self, f):
        self.name = f
        self.input_files.extend(f)
    def target_executable(self, f):
        self.name = f +"_build"
        self.output_file = f
        self.target_type = 0
    def target_archive(self, f):
        self.name = f +"_build"
        self.output_file = f
        self.target_type = 1
    def add_dependencies(self, d):
        self.dependencies.extend(d)
    def add_external_dependencies(self, d):
        self.external_dependencies.extend(d)
    def generate_make(self):
        output = ''
        objects = [self.name+'/'+i+'.o' for i in self.input_files]
        deps    = [self.name+'/'+i+'.d' for i in self.input_files]
        srcs    = [self.src_dir+i for i in self.input_files]
        print objects
        print deps
        print srcs
        output = output + '-include '+' '.join(deps) + '\n'
        output = output + self.output_file + ':' + ' '.join(self.dependencies)+' '+' '.join(objects) + '\n'
        # target rule
        if (self.target_type == 0):
            output = output + '\t'+ CXX + ' ' + ' '.join(objects) + ' ' + ' '.join(self.dependencies) + ' ' + ' '.join([TLD+i for i in self.external_dependencies]) + ' ' + self.linkflags + ' ' + self.cxxflags + ' -o ' + self.output_file + '\n'
        else:
            output = output + '\t'+ AR + ' rcs ' + self.linkflags + ' ' + self.output_file + ' ' + ' '.join(objects) + '\n'

        # object rule
        for i in range(len(self.input_files)):
            output = output + objects[i] + ':' + srcs[i] + '\n'
            output = output +'\t@mkdir -p ' + os.path.dirname(objects[i]) + '\n'
            output = output +'\t@echo Building '+objects[i]+'..\n'
            output = output +'\t@'+ CXX + ' -MMD ' + self.cxxflags + ' ' + self.includePaths + ' -c ' + srcs[i] + ' -o ' + objects[i] + '\n'

        return output;

def clean_filenames(a):
    return [i.strip().replace('./','') for i in a]

print 'configuring for testing'

print 'TLD is ' + TLD
print 'PWD is ' + PWD

CLIENT_TEST_SOURCES = []
CLIENT_SRC_SOURCES  = []
SERVER_TEST_SOURCES = []
SERVER_SRC_SOURCES  = []

p = subprocess.Popen('cd '+TLD+'/ConfigTree/src && find . | egrep \'\.cpp$\' | grep -v main.cpp', shell=True, stdout=subprocess.PIPE)
q = subprocess.Popen('cd '+TLD+'/FlyItf/src     && find . | egrep \'\.cpp$\' | grep -v main.cpp', shell=True, stdout=subprocess.PIPE)
r = subprocess.Popen('cd '+TLD+'/FlyItf/test    && find . | egrep \'\.cpp$\'', shell=True, stdout=subprocess.PIPE)

CONFIGTREE_SRC_SOURCES = clean_filenames(p.stdout.readlines())
FLYITF_SRC_SOURCES     = clean_filenames(q.stdout.readlines())
FLYITF_TEST_SOURCES    = clean_filenames(r.stdout.readlines())

print "CONFIGTREE_SRC_SOURCES",CONFIGTREE_SRC_SOURCES
print "FLYITF_SRC_SOURCES",FLYITF_SRC_SOURCES
print "FLYITF_TEST_SOURCES",FLYITF_TEST_SOURCES

includePathsCommon = ['.', 'Logless/include/', 'BFC/include/', 'ConfigTree/include', 'propertytree/client/include', 'propertytree/', 'propertytree/cum', 'ConfgTree/include']

configtree = Build()
configtree.set_cxxflags(CXXFLAGS)
configtree.add_include_paths(includePathsCommon)
configtree.set_src_dir('client/src/')
configtree.add_src_files(CONFIGTREE_SRC_SOURCES)
configtree.target_archive('configtree.a')

flyitf = Build()
flyitf.set_cxxflags(CXXFLAGS)
flyitf.add_include_paths(['FlyItf/src'])
flyitf.add_include_paths(includePathsCommon)
flyitf.set_src_dir('FlyItf/src/')
flyitf.add_src_files(FLYITF_SRC_SOURCES)
flyitf.target_archive('flyitf.a')

flyitf_bin = Build()
flyitf_bin.set_cxxflags(CXXFLAGS)
flyitf_bin.add_include_paths(['.', 'FlyItf/src/'])
flyitf_bin.add_include_paths(includePathsCommon)
flyitf_bin.set_src_dir('FlyItf/src/')
flyitf_bin.add_src_files(["main.cpp"])
flyitf_bin.add_dependencies(['flyitf.a', 'configtree.a'])
flyitf_bin.add_external_dependencies(['propertytree/build/client.a', 'Logless/build/logless.a'])
flyitf_bin.set_linkflags('-lpthread')
flyitf_bin.target_executable('flyitf')

with open('Makefile','w+') as mf:
    mf.write(configtree.generate_make())
    mf.write(flyitf.generate_make())
    mf.write(flyitf_bin.generate_make())

