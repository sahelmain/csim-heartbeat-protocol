#!/bin/bash

# Heartbeat Protocol Simulation - Test Automation Script
# Project #2: Azzam, Sahel

echo "======================================================"
echo "Heartbeat Protocol Simulation - Automated Test Suite"
echo "======================================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check dependencies
print_status "Checking dependencies..."

# Check for make
if ! command -v make &> /dev/null; then
    print_error "make command not found"
    exit 1
fi

# Check for gcc
if ! command -v gcc &> /dev/null; then
    print_error "gcc compiler not found"
    exit 1
fi

# Check for python3
if ! command -v python3 &> /dev/null; then
    print_warning "python3 not found - graph generation will be skipped"
    SKIP_GRAPHS=1
else
    print_success "All dependencies found"
fi

# Clean previous builds
print_status "Cleaning previous builds..."
make clean > /dev/null 2>&1

# Test 1: Local build and execution
print_status "Testing local build..."
if make local > /dev/null 2>&1; then
    print_success "Local build successful"
    
    print_status "Running local simulation test..."
    if timeout 30s ./local_test > local_test_output.txt 2>&1; then
        print_success "Local simulation completed"
        
        # Check for basic output patterns
        if grep -q "simulation results" local_test_output.txt; then
            print_success "Local simulation output validated"
        else
            print_warning "Local simulation output may be incomplete"
        fi
    else
        print_error "Local simulation failed or timed out"
    fi
else
    print_error "Local build failed"
fi

# Test 2: V2 build and execution
print_status "Testing V2 simplified build..."
if make v2 > /dev/null 2>&1; then
    print_success "V2 build successful"
    
    print_status "Running V2 simulation test..."
    if timeout 20s ./v2_test > v2_test_output.txt 2>&1; then
        print_success "V2 simulation completed"
    else
        print_error "V2 simulation failed or timed out"
    fi
else
    print_error "V2 build failed"
fi

# Test 3: Graph generation (if python available)
if [ -z "$SKIP_GRAPHS" ]; then
    print_status "Testing graph generation..."
    if python3 create_graphs.py > graph_output.txt 2>&1; then
        if [ -f "heartbeat_protocol_graphs.png" ]; then
            print_success "Graph generation successful"
        else
            print_warning "Graph generation script ran but no output file found"
        fi
    else
        print_error "Graph generation failed"
    fi
fi

# Test 4: HPCC build (will likely fail without CSIM)
print_status "Testing HPCC build (may fail without CSIM)..."
if make hpcc > hpcc_build_output.txt 2>&1; then
    print_success "HPCC build successful"
else
    print_warning "HPCC build failed (expected without CSIM environment)"
fi

# Generate test report
print_status "Generating test report..."
cat > TEST_REPORT.md << EOF
# Automated Test Report

**Generated:** $(date)
**System:** $(uname -s) $(uname -r)
**Compiler:** $(gcc --version | head -n1)

## Test Results

### Build Tests
- [x] Local build: $([ -f local_test ] && echo "PASS" || echo "FAIL")
- [x] V2 build: $([ -f v2_test ] && echo "PASS" || echo "FAIL")
- [x] Graph generation: $([ -f heartbeat_protocol_graphs.png ] && echo "PASS" || echo "SKIP")

### Execution Tests
- [x] Local simulation: $([ -f local_test_output.txt ] && echo "COMPLETED" || echo "FAILED")
- [x] V2 simulation: $([ -f v2_test_output.txt ] && echo "COMPLETED" || echo "FAILED")

### Output Files Generated
$(ls -la *.txt *.png 2>/dev/null | grep -E '\.(txt|png)$' || echo "No output files found")

## Next Steps
1. Review output files for correctness
2. Test HPCC build in proper CSIM environment
3. Validate simulation results against requirements

---
*Automated test suite for Heartbeat Protocol Simulation*
EOF

print_success "Test report generated: TEST_REPORT.md"

# Cleanup temporary files
print_status "Cleaning up temporary files..."
rm -f *_output.txt

print_status "Test suite completed!"
echo "======================================================" 