# Rovibrational Excitation Propagator (C++ + OpenMP + pybind11)

高速な振動回転状態の時間発展計算ライブラリ  
Liouville方程式に基づく密度行列の時間発展を、C++ と OpenMP により並列化し、高速に計算します。  
Python から簡単に利用可能です。

---

## 🚀 Features
- Runge-Kutta 4th order (RK4) 時間発展
- 遷移双極子行列と時間依存電場を使った計算
- OpenMP によるマルチコア並列
- Python から呼び出せる pybind11 バインディング
- Eigen による簡潔な行列操作
- Docker で簡単に開発環境を構築可能

---

## 🧰 Usage Example

```python
from fast_propagator import rk4_propagate
import numpy as np

# 例: 2x2行列
H0 = np.array([[0.0, 0.0], [0.0, 1.0]], dtype=np.complex128)
mu_x = np.array([[0.0, 1.0], [1.0, 0.0]], dtype=np.complex128)
mu_y = np.array([[0.0, -1j], [1j, 0.0]], dtype=np.complex128)
Efield_x = np.ones(1000, dtype=np.complex128)
Efield_y = np.zeros(1000, dtype=np.complex128)
rho0 = np.array([[1.0, 0.0], [0.0, 0.0]], dtype=np.complex128)

rk4_propagate(H0, mu_x, mu_y, Efield_x, Efield_y, rho0, dt=0.1, steps=1000)
print(rho0)
```

---

## 🐳 Development with Docker

### Build the environment:
```bash
docker-compose build
```

### Start the development container:
```bash
./dev.sh
```

Inside the container:
```bash
pip install .
pytest test/
```

---

## ⚡ Dependencies
- C++17
- OpenMP
- Eigen3
- pybind11
- Python (scikit-build, numpy, pytest)

---

## 📝 License
[MIT License](./LICENSE)

---

## 🧑‍💻 Author
Hiroki Tsusaka (tsusaka@iis.u-tokyo.ac.jp)