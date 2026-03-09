# MixCopilot AU

[![CI](https://github.com/JeHA61/s/actions/workflows/ci.yml/badge.svg)](https://github.com/JeHA61/s/actions/workflows/ci.yml)

Conversational mixing co-pilot AU plugin (Logic-first MVP).

This plugin helps home recordists and indie artists make mixing decisions faster by translating sensory language (예: "먹먹함", "몽글몽글") into practical, explainable DSP suggestions.

## Project Status
- Target: AU plugin for Logic users
- Workflow goal: 2-minute assisted pass, 5-minute hard limit
- Current mode: MVP of analysis + intent parsing + rationale UI + safe rollback and bypass compare
- Style presets: Warm Pop / Modern Pop / Indie Acoustic (MVP)

## Quickstart

### Prerequisites
- macOS with Xcode Command Line Tools
- CMake and a C++ toolchain
- JUCE submodules/tooling available via CMake

### Build

```bash
export PATH="$HOME/Library/Python/3.14/bin:$PATH"
scripts/release/run-validation.sh
```

The script builds the project, runs tests, and optionally runs audio regression + auval checks.

The build output is written to `build/` and includes: `build/mixcopilot_tests`, AU binary artefacts, and plugin bundle output.

### Verification

```bash
# quick validation
scripts/release/run-validation.sh

# optional audio regression check (requires fixtures + baselines)
python3 scripts/verify/audio_regression.py --input fixtures --baseline baselines
```

If `ctest` is unavailable in your shell environment, run the built test binary directly: `./build/mixcopilot_tests`.

To validate only parts:

```bash
scripts/release/run-validation.sh --skip-regression --skip-auval
```

For AU host validation and in-DAW smoke checks, see:
- `docs/testing/logic-au-validation.md`

### Logic Rapid Iteration

```bash
# Build and install AU only (fastest loop)
scripts/dev/install-for-logic.sh

# Run tests too, validate with auval, and launch Logic
scripts/dev/install-for-logic.sh --run-tests --verify-au --open-logic
```

This script installs/reinstalls:
- `build/MixCopilotAU_artefacts/AU/MixCopilot.component` -> `~/Library/Audio/Plug-Ins/Components`

Use `--skip-build` when you only want to reinstall an existing build output.

### Style Presets

Select a preset from the UI dropdown to shape the suggestion style:

- Warm Pop: cleaner low-mid control with gentle top lift
- Modern Pop: tighter transients and brighter presence
- Indie Acoustic: softer, tape-like movement and controlled air

## Usage Notes
- Start with natural-language intent text in the plugin UI (Korean/English phrases supported in MVP).
- Suggestions include a rationale block explaining why each action is recommended.
- Use rollback and Bypass controls to compare assisted vs original states safely.

## Git

Remote: `git@github.com:JeHA61/s.git`

```bash
git push -u origin main
```

## What’s Next
- Add artist-profile presets and style references
- Expand explanation fidelity and confidence scoring
- Improve parameter mapping quality without increasing interaction latency
