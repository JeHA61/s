#!/usr/bin/env python3
"""Basic audio regression checks for MixCopilot MVP.

Checks matching WAV files in --baseline and --input directories for:
1) low-mid energy ratio reduction (processed <= baseline + tolerance)
2) loudness delta bounds (absolute dBFS delta <= threshold)
"""

from __future__ import annotations

import argparse
import math
import pathlib
import struct
import sys
import wave


def read_wav_mono(path: pathlib.Path) -> tuple[list[float], int]:
    with wave.open(str(path), "rb") as wf:
        channels = wf.getnchannels()
        sample_width = wf.getsampwidth()
        sample_rate = wf.getframerate()
        frame_count = wf.getnframes()
        frames = wf.readframes(frame_count)

    if sample_width != 2:
        raise ValueError(f"{path}: only 16-bit PCM WAV is supported")

    ints = struct.unpack("<" + "h" * (len(frames) // 2), frames)
    out: list[float] = []
    for i in range(0, len(ints), channels):
        sample_sum = 0.0
        for ch in range(channels):
            sample_sum += ints[i + ch] / 32768.0
        out.append(sample_sum / channels)
    return out, sample_rate


def rms_dbfs(samples: list[float]) -> float:
    if not samples:
        return -120.0
    energy = sum(s * s for s in samples) / len(samples)
    if energy <= 0.0:
        return -120.0
    return 20.0 * math.log10(math.sqrt(energy))


def low_mid_ratio(samples: list[float], sample_rate: int, fft_size: int = 2048) -> float:
    if len(samples) < 2:
        return 0.0
    fft_size = min(fft_size, len(samples))
    half = fft_size // 2
    total = 0.0
    low_mid = 0.0
    for k in range(half):
        real = 0.0
        imag = 0.0
        for n in range(fft_size):
            angle = 2.0 * math.pi * k * n / fft_size
            sample = samples[n]
            real += sample * math.cos(angle)
            imag -= sample * math.sin(angle)
        power = real * real + imag * imag
        freq = k * sample_rate / fft_size
        total += power
        if 150.0 <= freq <= 500.0:
            low_mid += power
    if total <= 0.0:
        return 0.0
    return low_mid / total


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, help="Directory with processed WAV files")
    parser.add_argument("--baseline", required=True, help="Directory with baseline WAV files")
    parser.add_argument("--ratio-tolerance", type=float, default=0.03)
    parser.add_argument("--max-loudness-delta-db", type=float, default=2.0)
    args = parser.parse_args()

    input_dir = pathlib.Path(args.input)
    baseline_dir = pathlib.Path(args.baseline)

    if not input_dir.exists() or not baseline_dir.exists():
        print("SKIP: input/baseline directory missing.")
        return 0

    input_files = {p.name: p for p in input_dir.glob("*.wav")}
    baseline_files = {p.name: p for p in baseline_dir.glob("*.wav")}
    common = sorted(set(input_files) & set(baseline_files))

    if not common:
        print("SKIP: no matching WAV pairs found.")
        return 0

    failures = 0
    for name in common:
        processed_samples, processed_sr = read_wav_mono(input_files[name])
        baseline_samples, baseline_sr = read_wav_mono(baseline_files[name])
        if processed_sr != baseline_sr:
            print(f"FAIL {name}: sample rates differ ({processed_sr} vs {baseline_sr})")
            failures += 1
            continue

        processed_ratio = low_mid_ratio(processed_samples, processed_sr)
        baseline_ratio = low_mid_ratio(baseline_samples, baseline_sr)
        processed_db = rms_dbfs(processed_samples)
        baseline_db = rms_dbfs(baseline_samples)

        ratio_ok = processed_ratio <= (baseline_ratio + args.ratio_tolerance)
        loudness_ok = abs(processed_db - baseline_db) <= args.max_loudness_delta_db

        print(
            f"{name}: low_mid processed={processed_ratio:.4f} baseline={baseline_ratio:.4f}, "
            f"loudness processed={processed_db:.2f}dB baseline={baseline_db:.2f}dB"
        )

        if not ratio_ok:
            print(f"FAIL {name}: low-mid energy not reduced within tolerance ({args.ratio_tolerance})")
            failures += 1
        if not loudness_ok:
            print(f"FAIL {name}: loudness delta exceeds {args.max_loudness_delta_db} dB")
            failures += 1

    if failures:
        print(f"Result: FAIL ({failures} issues)")
        return 1

    print("Result: PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
