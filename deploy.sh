#!/bin/bash

# Exit script immediately if any compilation or execution task fails
set -e

echo "=========================================================="
echo "HERCULES PASSIVE DAEMON PRODUCTION PIPELINE"
echo "=========================================================="

echo -e "\n[Step 1/3] Compiling optimized native system daemon binary..."
rm -f hercules-daemon
g++ -O3 -std=c++17 src/main.cpp -I include -o hercules-daemon

echo -e "\n[Step 2/3] Triggering real-time system defense simulations..."
./hercules-daemon

echo -e "\n[Step 3/3] Staging core modules and synchronizing to GitHub..."
git add src/main.cpp include/HerculesCore.hpp docs/ROADMAP.md docs/ARCHITECTURE.md README.md deploy.sh .gitignore .gitattributes tests/

# Only issue commits if new modification deltas are active
if ! git diff-index --quiet HEAD --; then
    git commit -m "pipeline: final Phase 3 automation deploy (Native daemon stability locked)"
    git push origin main
else
    echo "No modifications detected. Codebase is completely aligned."
fi

echo -e "\n=========================================================="
echo "SUCCESS! HERCULES IS SIGNED, SEALED, AND ONLINE."
echo "=========================================================="
