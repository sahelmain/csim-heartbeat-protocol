# Installation and Setup Guide

## 🖥️ HPCC Environment Setup

### Prerequisites
- Access to Texas Tech HPCC (login.hpcc.ttu.edu)
- eRaider credentials
- GlobalProtect VPN (for off-campus access)

### CSIM Installation
```bash
# Copy CSIM from shared location
cp /lustre/work/slim/CSIM.zip $HOME

# Extract and setup
unzip CSIM.zip
chmod +x ./csim20_linux/Linux/64-bit/gcc/csim.gcc

# Add to PATH (optional)
export PATH=$PATH:$HOME/csim20_linux/Linux/64-bit/gcc
```

### Compilation and Execution
```bash
# Navigate to CSIM compiler directory
cd ~/csim20_linux/Linux/64-bit/gcc/

# Compile the project
./csim.gcc proj2_azzam_sahel.c -o proj2_azzam_sahel

# Run simulation
./proj2_azzam_sahel
```

## 🏠 Local Development Setup

### Requirements
- GCC compiler
- Python 3.x
- Matplotlib library

### Local Testing
```bash
# Clone repository
git clone https://github.com/sahelmain/csim-heartbeat-protocol.git
cd csim-heartbeat-protocol

# Install Python dependencies
pip3 install matplotlib numpy

# Build local test version
make local

# Run local simulation
make run-local
```

## 📊 Graph Generation

### Prerequisites
```bash
# Install required Python packages
pip3 install matplotlib numpy
```

### Generate Performance Graphs
```bash
# Using Makefile
make graphs

# Or directly
python3 create_graphs.py
```

## 🔧 Makefile Options

| Command | Description |
|---------|-------------|
| `make help` | Show all available options |
| `make hpcc` | Build for HPCC (requires CSIM) |
| `make local` | Build local test version |
| `make v2` | Build simplified V2 version |
| `make graphs` | Generate performance graphs |
| `make clean` | Clean compiled files |
| `make run-local` | Build and run local test |
| `make install-deps` | Install Python dependencies |

## 🐛 Troubleshooting

### Common Issues

**CSIM Compiler Not Found:**
```bash
# Ensure you're in the correct directory
cd ~/csim20_linux/Linux/64-bit/gcc/
# Or use full path
~/csim20_linux/Linux/64-bit/gcc/csim.gcc proj2_azzam_sahel.c -o proj2_azzam_sahel
```

**Local Compilation Errors:**
```bash
# Ensure math library is linked
gcc -Wall -g local.c -o local_test -lm
```

**Graph Generation Issues:**
```bash
# Install missing dependencies
pip3 install --upgrade matplotlib numpy
# Or use conda
conda install matplotlib numpy
```

### File Permissions
```bash
# Make executable
chmod +x proj2_azzam_sahel
chmod +x local_test
```

## 📁 Project Structure
```
csim-heartbeat-protocol/
├── proj2_azzam_sahel.c     # Main CSIM implementation
├── local.c                 # Local testing version
├── V2.c                    # Simplified test version
├── create_graphs.py        # Performance visualization
├── Makefile               # Build system
├── README.md              # Project overview
├── docs/                  # Documentation
│   ├── REQUIREMENTS.md    # Assignment specifications
│   └── INSTALLATION.md    # This file
└── Project2.txt           # Original assignment document
```

---
*For additional support, refer to the CSIM documentation or contact the course TA.* 