#!/usr/bin/python
import os
import sys
import subprocess

CXX      = 'g++'
AR       = 'ar'
MAKE     = 'make'
CXXFLAGS = '-std=c++14 -ggdb3 -O0 -Wall -Werror'

TLD = os.path.dirname(sys.argv[0])+'/'
PWD = os.getcwd()+'/'

class Build:
    def __init__(self):
        self.input_files = []
        self.output_file = ''
        self.src_dir = ''
        self.dependencies = []
        self.target_type = 0
        self.cxxflags = ''
        self.linkflags = ''
    def add_include_paths(self, p):
        self.cxxflags = self.cxxflags + ' ' +' '.join(['-I'+TLD+i for i in p])
    def set_cxxflags(self, f):
        self.cxxflags = f
    def set_linkflags(self, f):
        self.linkflags = f
    def set_src_dir(self, d):
        self.src_dir = d
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
            output = output + '\t'+ CXX + ' ' + ' '.join(objects) + ' ' + ' '.join(self.dependencies) + ' ' + self.linkflags +  ' -o ' + self.output_file + '\n'
        else:
            output = output + '\t'+ AR + ' rcs ' + self.linkflags + ' ' + self.output_file + ' ' + ' '.join(objects) + '\n'

        # object rule
        for i in range(len(self.input_files)):
            output = output + objects[i] + ':' + srcs[i] + '\n'
            output = output +'\t@mkdir -p ' + os.path.dirname(objects[i]) + '\n'
            output = output +'\t@echo Building '+objects[i]+'..\n'
            output = output +'\t@'+ CXX + ' -MMD ' + self.cxxflags + ' -c ' + srcs[i] + ' -o ' + objects[i] + '\n'

        return output;

def clean_filenames(a):
    return [i.strip().replace('./','') for i in a]

print 'configuring for testing'

print 'TLD is ' + TLD
print 'PWD is ' + PWD

TEST_SOURCES = []
CFG_SOURCES  = []

p = subprocess.Popen('cd '+TLD+'/test  && find . | egrep \'\.cpp$\'', shell=True, stdout=subprocess.PIPE)

TEST_SOURCES = clean_filenames(p.stdout.readlines())

print "TEST_SOURCES", TEST_SOURCES

tests = Build()
tests.set_cxxflags(CXXFLAGS)
tests.add_include_paths(['gtest'])
tests.add_include_paths(['src'])
tests.add_include_paths(['.'])
tests.add_include_paths(['generated'])
tests.set_src_dir(TLD+'test/')
tests.add_src_files(TEST_SOURCES)
tests.add_dependencies(['gtest.a'])
tests.set_linkflags('-lpthread')
tests.target_executable('tests')

tests_coverage = Build()
tests_coverage.set_cxxflags(CXXFLAGS+" --coverage")
tests_coverage.add_include_paths(['gtest'])
tests_coverage.add_include_paths(['src'])
tests_coverage.add_include_paths(['.'])
tests_coverage.add_include_paths(['generated'])
tests_coverage.set_src_dir(TLD+'test/')
tests_coverage.add_src_files(TEST_SOURCES)
tests_coverage.add_dependencies(['gtest.a'])
tests_coverage.set_linkflags('-lpthread -lgcov --coverage')
tests_coverage.target_executable('tests_coverage')

gtest = Build()
gtest.set_cxxflags(CXXFLAGS)
gtest.set_src_dir(TLD+'gtest/')
gtest.add_src_files(['gtest.cpp'])
gtest.add_include_paths(['gtest'])
gtest.target_archive('gtest.a')

with open('Makefile','w+') as mf:
    mf.write(gtest.generate_make())
    mf.write(tests.generate_make())
    mf.write(tests_coverage.generate_make())
