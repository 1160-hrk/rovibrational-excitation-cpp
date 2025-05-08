# test/test_psi.py
import numpy as np
import psi                                      # bindings/psi.cpp で生成されたモジュール

def _make_pauli_system():
    """2 次元・外場無しでも回転する系（sigma_z をエネルギーギャップに）。"""
    sigma_x = np.array([[0, 1], [1, 0]],  dtype=np.complex128)
    sigma_y = np.array([[0, -1j], [1j, 0]], dtype=np.complex128)
    sigma_z = np.array([[1, 0], [0, -1]],  dtype=np.complex128)

    H0   = 0.5 * sigma_z                     # 固有エネルギー ±½
    mu_x = sigma_x                           # 双極子
    mu_y = sigma_y

    return H0, mu_x, mu_y

def test_rk4_psi_and_traj():
    H0, mu_x, mu_y = _make_pauli_system()

    # --- 外場：矩形 1 を 2*steps+1 点並べる（dt=0.1 のとき t_max≈steps*dt）
    steps = 20
    Ex = np.ones(2*steps+1, dtype=np.complex128)
    Ey = np.zeros_like(Ex)

    # --- 初期状態：|0>, |1>
    psi0 = np.array([1.0, 0.0], dtype=np.complex128)
    psi1 = np.array([0.0, 1.0], dtype=np.complex128)
    psi_list = [psi0.copy(), psi1.copy()]

    # ---- 実行：ノルムが保存されるか？ ------------------------
    dt = 0.05
    psi.rk4_psi(psi_list, H0, mu_x, mu_y, Ex, Ey, dt, steps)

    for p in psi_list:
        assert np.allclose(np.vdot(p, p).real, 1.0, atol=1e-12)

    # ---- 軌跡付き版：サンプル数と形状を確認 -------------------
    stride   = 5
    traj = psi.rk4_psi_traj([psi0, psi1], H0, mu_x, mu_y, Ex, Ey,
                            dt, steps, sample_stride=stride)

    n_sample_expected = steps // stride + 1
    assert len(traj) == 2                              # 2 状態
    assert len(traj[0]) == n_sample_expected           # サンプル数
    assert traj[0][0].shape == (2,)                    # 各ベクトルの形

    # 先頭サンプル（t=0）は初期状態と一致するはず
    assert np.allclose(traj[0][0], psi0)
    assert np.allclose(traj[1][0], psi1)
