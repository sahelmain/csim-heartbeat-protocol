# CSIM Heartbeat Protocol Simulation

**CS5331: Mobile Data Management and Privacy - Project #2**  
**Author:** Azzam, Sahel  
**University:** Texas Tech University

## 📋 Project Overview

This project implements a heartbeat protocol simulation using the CSIM discrete event simulation library. The simulation models inter-process communication between 5 network nodes using Hello and Hello_Ack control packets.

## 🎯 Objectives

- Simulate a 5-node fully connected network
- Implement Hello/Hello_Ack packet exchange protocol
- Model packet loss scenarios (0.1 - 0.5 probability)
- Analyze transmission success/failure rates
- Generate performance visualizations

## 🔧 Technical Specifications

- **Simulation Time:** 1000 seconds
- **Inter-arrival Time:** Exponential distribution (mean 5.0s)
- **Transmission Time:** 0.2 seconds
- **Processing Delay:** 0.1 seconds  
- **Timeout Period:** 2.5 seconds
- **Retry Attempts:** Single retransmission allowed

## 📊 Performance Metrics

The simulation tracks and analyzes:
- Percentage of successful transmissions
- Percentage of failed transmissions
- Impact of varying packet loss probabilities
- Event snapshots for detailed analysis

## 🚀 Getting Started

### Quick Start
```bash
# Clone the repository
git clone https://github.com/sahelmain/csim-heartbeat-protocol.git
cd csim-heartbeat-protocol

# View available build options
make help

# For HPCC (with CSIM)
make hpcc
./proj2_azzam_sahel

# For local testing
make local
make run-local

# Generate performance graphs
make graphs
```

### 📁 Project Files
- `proj2_azzam_sahel.c` - Main CSIM implementation
- `local.c` - Local testing version with mock CSIM
- `V2.c` - Simplified testing framework
- `create_graphs.py` - Performance visualization
- `Makefile` - Build system and automation

### 📊 Results
The simulation generates:
- Success/failure transmission percentages
- Performance graphs across packet loss scenarios
- Event snapshots for detailed analysis
- Comprehensive performance metrics

For detailed setup instructions, see [Installation Guide](docs/INSTALLATION.md).

---
*This project is part of the CS5331 coursework at Texas Tech University.* 