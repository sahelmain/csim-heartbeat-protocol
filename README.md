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

More detailed instructions will be added as the project develops.

---
*This project is part of the CS5331 coursework at Texas Tech University.* 