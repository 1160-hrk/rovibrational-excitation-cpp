#!/bin/bash
# ============================================
# Development helper script for Docker container
# ============================================

set -e

SERVICE_NAME="dev"

# macでも動く nproc 相当
CPU_COUNT=$(getconf _NPROCESSORS_ONLN)

echo "🔧 Starting development container: ${SERVICE_NAME} with OMP_NUM_THREADS=${CPU_COUNT}"

docker-compose run --rm -e OMP_NUM_THREADS=${CPU_COUNT} ${SERVICE_NAME} bash -c "
    echo '📂 Moving into workspace...'
    cd /workspace

    echo '🏗️  Configuring and building with CMake...'
    mkdir -p build
    cd build
    cmake .. || { echo '❌ CMake configuration failed.'; exit 1; }
    make -j\$(nproc) || { echo '❌ Build failed.'; exit 1; }

    echo '✅ Build completed.'

    echo '🧪 Running pytest...'
    cd /workspace
    export PYTHONPATH=/workspace
    pytest test/ || echo '⚠️  Tests failed, entering shell for debugging.'

    echo '🌀 Entering interactive shell. You can rebuild or rerun tests manually.'
    exec bash
"
