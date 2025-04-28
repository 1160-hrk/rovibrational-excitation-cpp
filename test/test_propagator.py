import numpy as np
from fast_propagator import rk4_propagate

def test_rk4_runs():
    dim = 2
    steps = 10
    dt = 0.1

    H0 = np.array([[0.0, 0.0], [0.0, 1.0]], dtype=np.complex128)
    mu_x = np.array([[0.0, 1.0], [1.0, 0.0]], dtype=np.complex128)
    mu_y = np.array([[0.0, -1j], [1j, 0.0]], dtype=np.complex128)

    Efield_x = np.ascontiguousarray(np.ones(2 * steps + 1, dtype=np.complex128))
    Efield_y = np.ascontiguousarray(np.zeros(2 * steps + 1, dtype=np.complex128))

    rho0 = np.array([[1.0, 0.0], [0.0, 0.0]], dtype=np.complex128)

    rk4_propagate(H0, mu_x, mu_y, Efield_x, Efield_y, rho0, dt, steps)

    assert rho0.shape == (dim, dim)
    assert rho0.dtype == np.complex128
