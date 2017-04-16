#!/usr/bin/python
import os
import numpy as np
import matplotlib.pyplot as plt
main_directory = "raa-eval-output/"
subdirectories = [x[0] for x in os.walk(main_directory)]
print "Plotting Graphs";
for directory in subdirectories[1:]:
	filenames = []
	for filename in os.listdir(directory):
		if filename.endswith(".txt"):
			filenames.append(filename)
	for filename in filenames:
		plt.clf()
		f = open(directory + "/" +filename)
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
		plt.savefig("raa-eval-graph/"+ directory.split("/")[1] + "/" + filename.split('.')[0]+"-graph.png")
		f.close()
