#!/bin/sh

cd ..
cd ..
cd ..
mkdir raa-eval-plots
for i in {0..9}
do
	./waf --run "raa-eval-dense --index=$i"
done
cd raa-eval-plots
touch plot.plt
echo 'set terminal png
set output "Raas.png"
set xlabel "Time (Sec)"
set ylabel "Average Throughput (Mbit/s)"

set autoscale
set title "Average Throughput of RAAs over time"
set colorsequence classic
set grid
set key left top
set style data lines
plot "ns3::ArfWifiManager Average Throughput.txt" using 1:2 title "Arf", "ns3::AarfWifiManager Average Throughput.txt" using 1:2 title "Aarf", "ns3::AarfcdWifiManager Average Throughput.txt"  using 1:2 title "Aarf-Cd", "ns3::AmrrWifiManager Average Throughput.txt"  using 1:2 title "Amrr", "ns3::CaraWifiManager Average Throughput.txt"  using 1:2 title "Cara", "ns3::IdealWifiManager Average Throughput.txt"  using 1:2 title "Ideal", "ns3::MinstrelWifiManager Average Throughput.txt"  using 1:2 title "Minstrel", "ns3::ParfWifiManager Average Throughput.txt"  using 1:2 title "Parf", "ns3::AparfWifiManager Average Throughput.txt"  using 1:2 title "Aparf"'>plot.plt
gnuplot plot.plt