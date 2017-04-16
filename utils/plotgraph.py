#!/usr/bin/python
import os
import numpy as np
import matplotlib.pyplot as plt
directory = "raa-eval-output/"
filenames = []
for filename in os.listdir(directory):
	if filename.endswith(".txt"):
		filenames.append(filename)
for filename in filenames:
	f = open(directory + filename)
	lines = f.readlines()
	xdata = []
	ydata = []
	plt.title(filename.split('.')[0])
	plt.xlabel('Time (msec)')
	plt.ylabel('Average Througput (Mbit/sec)')
	for line in lines:
		xdata.append(line.split(', ')[0])
		ydata.append(line.split(', ')[1])
	plt.plot(xdata,ydata);
	plt.savefig("raa-eval-graph/"+filename.split('.')[0]+"-graph.png")
	f.close()
