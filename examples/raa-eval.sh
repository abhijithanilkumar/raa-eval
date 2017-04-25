#!/bin/sh

cd ..
cd ..
cd ..
mkdir raa-eval-plots
for i in {0..1}
do
	./waf --run "raa-eval-dense --index=$i"
done
cd raa-eval-plots
touch plotd.plt
echo 'set terminal png
set output "Raas-downlink.png"
set xlabel "Time (Sec)"
set ylabel "Average Throughput (Mbit/s)"

set autoscale
set title "Average Throughput of RAAs over time(downlink)"
set colorsequence classic
set grid
set key left top
set style data lines
plot "ns3::AarfWifiManager Average Throughput.txt" using 1:2 title "AARF", "ns3::AarfcdWifiManager Average Throughput.txt"  using 1:2 title "AARF-CD"'>plotd.plt
gnuplot plotd.plt
touch plotu.plt
echo 'set terminal png
set output "Raas-uplink.png"
set xlabel "Time (Sec)"
set ylabel "Average Throughput (Mbit/s)"

set autoscale
set title "Average Throughput of RAAs over time(uplink)"
set colorsequence classic
set grid
set key left top
set style data lines
plot "ns3::AarfWifiManager Average Throughput.txt" using 1:3 title "AARF", "ns3::AarfcdWifiManager Average Throughput.txt"  using 1:3 title "AARF-CD"'>plotu.plt
gnuplot plotu.plt