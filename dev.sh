#!/usr/bin/env bash
# dev.sh --- ローカル開発用ワンライナー
#!/usr/bin/env bash
set -euo pipefail

SERVICE_NAME="dev"
CPU_COUNT=$(getconf _NPROCESSORS_ONLN)

echo "🔧  Launch docker-compose service=${SERVICE_NAME} (OMP_NUM_THREADS=${CPU_COUNT})"

docker compose run --rm -T \
  -e OMP_NUM_THREADS="${CPU_COUNT}" \
  "${SERVICE_NAME}" bash -eu <<'EOSHELL'
# ===== inside container =====
echo "📂  cd /workspace"
cd /workspace

echo "🏗️   CMake configure / build"
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j"$(nproc)"

echo "✅  build done"

# 生成された Python 拡張を import 可能に
export PYTHONPATH="/workspace/build/bindings:${PYTHONPATH:-}"

echo "🧪  pytest"
pytest -q

echo "🎉  all tests passed"
EOSHELL
