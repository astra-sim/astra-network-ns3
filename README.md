# ASTRA-NETWORK-NS3
This repository contains an NS3-based network simulator that acts as a network backend for ASTRA-sim. 
ASTRA-sim is a distributed machine learning system simulator, developed as a joint collaboration between Georgia Tech, Meta, and Intel.
For more details on ASTRA-sim, please refer to [the ASTRA-sim repository](https://github.com/astra-sim/astra-sim)

This repository is extended from [https://github.com/alibaba-edu/High-Precision-Congestion-Control](https://github.com/alibaba-edu/High-Precision-Congestion-Control), and simulates various congestion control algorithms. 
With ASTRA-sim this repository was used to study the performance of different network congestion control algorithms in largescale collective communications in multi-gpu scenarios.
More details on the study can be found in our paper:

T. Khan, S. Rashidi, S. Sridharan, P. Shurpali, A. Akella and T. Krishna, "Impact of RoCE Congestion Control Policies on Distributed Training of DNNs," In proceedings of the IEEE Symposium on High-Performance Interconnects (HOTI), 2022 [[pdf]](https://arxiv.org/abs/2207.10898)

For information on how to use and build this tool, please visit our wiki [ASTRA-sim wiki](https://astra-sim.github.io/astra-sim-docs/getting-started/installing-astra-sim.html#compile-program)

## Contact Us
For any questions about using this network backend with ASTRA-sim, you can email the ASTRA-sim User Mailing List: astrasim-users@googlegroups.com

To join the mailing list, please fill out the following form: https://forms.gle/18KVS99SG3k9CGXm6
