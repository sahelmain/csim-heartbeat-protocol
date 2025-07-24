# Project Requirements - Heartbeat Protocol Simulation

## 📋 Assignment Overview

**Course:** CS5331 - Mobile Data Management and Privacy  
**Project:** #2 - Heartbeat Protocol Implementation  
**Due Date:** July 23, 2025  
**Points:** 20 total

## 🎯 Core Requirements

### 1. Network Configuration
- **Nodes:** 5 fully connected nodes
- **Topology:** Each node can communicate with all others
- **Protocol:** Hello/Hello_Ack packet exchange

### 2. Timing Parameters
- **Inter-arrival Time:** Exponential distribution (mean = 5.0 seconds)
- **Transmission Time:** 0.2 seconds per packet
- **Processing Delay:** 0.1 seconds before reply
- **Timeout Period:** 2.5 seconds
- **Simulation Duration:** 1000 seconds

### 3. Packet Loss Scenarios
Test with the following packet loss probabilities:
- 0.1 (10% loss)
- 0.2 (20% loss) 
- 0.3 (30% loss)
- 0.4 (40% loss)
- 0.5 (50% loss)

### 4. Retransmission Logic
- If no Hello_Ack received within timeout → retransmit once
- If still no response after retry → mark as failed transmission
- Track both successful and failed transmission rates

### 5. Event Logging
- Generate event snapshot for 0.3 loss probability
- Format: "node.X sends a Hello to node.Y at Z.Z seconds"
- Include all major events (sends, receives, retransmissions, packet losses)

## 📊 Performance Analysis

### Required Metrics
1. **Percentage of successful transmissions** vs packet loss probability
2. **Percentage of failed transmissions** vs packet loss probability

### Deliverable Graphs
- Graph 1: Success rate across all loss probabilities
- Graph 2: Failure rate across all loss probabilities

## 📝 Submission Requirements

### Source Code
- File name: `proj2_azzam_sahel.c`
- Header comment: `/* Project #2; Azzam, Sahel */`
- Compilation: `./csim.gcc proj2_azzam_sahel.c -o proj2_azzam_sahel`

### Report Documentation
- File name: `proj2_azzam_sahel.pdf`
- Include: Instructions, parameters, graphs, implementation issues
- TA will compile and verify results

## 🔧 Technical Implementation

### CSIM Library Usage
- Discrete event simulation framework
- Mailbox-based message passing
- Facility-based resource management
- Built-in statistical functions

### Key Functions Required
- Message creation and routing
- Timeout handling with retransmission
- Packet loss simulation
- Performance metric collection
- Event logging system

---
*Requirements based on CS5331 Project #2 assignment specifications* 