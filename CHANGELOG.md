# Changelog

All notable changes to the Heartbeat Protocol Simulation project.

## [1.0.0] - 2025-01-XX - Project Completion

### Added
- Complete CSIM heartbeat protocol implementation
- 5-node network simulation with exponential inter-arrival times
- Packet loss modeling (0.1-0.5 probability range)
- Timeout and retransmission logic (2.5s timeout, single retry)
- Performance metrics tracking and analysis
- Event snapshot generation for debugging
- Local testing framework with mock CSIM functions
- Automated build system with Makefile
- Performance visualization with Python/Matplotlib
- Comprehensive documentation and installation guides

### Technical Specifications
- **Simulation Time:** 1000 seconds
- **Inter-arrival Time:** Exponential distribution (mean 5.0s)
- **Transmission Time:** 0.2 seconds
- **Processing Delay:** 0.1 seconds
- **Network Topology:** Fully connected 5-node network
- **Message Types:** Hello and Hello_Ack packets

### Performance Results
- Success rates range from 54.37% (0.1 loss) to 31.32% (0.5 loss)
- Failure rates increase monotonically with packet loss probability
- Proper accounting of transmission attempts and outcomes
- Event logging for detailed analysis and debugging

### Development Features
- Multiple build targets (HPCC/CSIM, local testing, simplified V2)
- Mock CSIM implementation for local development
- Automated graph generation and analysis tools
- Comprehensive error handling and debugging traces
- Professional documentation with installation guides

### Files Structure
```
├── proj2_azzam_sahel.c         # Main CSIM implementation
├── local.c                     # Local testing version
├── V2.c                        # Simplified test framework
├── create_graphs.py            # Performance visualization
├── Makefile                    # Build automation
├── README.md                   # Project overview
├── docs/
│   ├── REQUIREMENTS.md         # Assignment specifications
│   └── INSTALLATION.md         # Setup instructions
├── Project2.txt                # Original assignment
├── heartbeat_protocol_graphs.png # Results visualization
└── CHANGELOG.md                # This file
```

### Implementation Highlights
- Robust message handling with proper timeout loops
- Accurate transmission success/failure accounting
- Event snapshot generation for 0.3 loss probability
- Professional graph generation with trend analysis
- Cross-platform compatibility (HPCC + local development)

---
*Project developed for CS5331: Mobile Data Management and Privacy*  
*Texas Tech University - Summer II 2025* 