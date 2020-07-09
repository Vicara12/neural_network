#!/usr/bin/python3

import matplotlib.pyplot as plt
from os import remove as rm_file

file_name = 'error_list_to_plot'

def fileReader ():
   file = open(file_name, 'r')

   error_list = []
   line = file.readline()
   minimum = float(line)
   maximum = float(line)

   while line != '':
      error_list.append(float(line))
      if (float(line) < minimum): minimum = float(line)
      if (float(line) > maximum): maximum = float(line)
      line = file.readline()
   
   if minimum > 0:
      minimum = 0
   
   return error_list, minimum, maximum


def plotter (error_list, minimum, maximum):
   plt.plot(error_list, label = 'linear')

   low_bound = minimum

   if (low_bound != 0):
      low_bound *= 1.05

   axes = plt.gca()
   axes.set_ybound([low_bound, maximum*1.1])
   plt.xlabel("iteration")
   plt.ylabel("error")
   plt.title("training results")
   plt.show()


def main ():
   error_list, minimum, maximum = fileReader()
   plotter(error_list, minimum, maximum)
   rm_file(file_name)


main()