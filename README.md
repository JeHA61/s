# MixCopilot AU

Logic-first AU plugin scaffold for the conversational mixing copilot MVP.

## Build

```bash
export PATH="$HOME/Library/Python/3.14/bin:$PATH"
cmake -S . -B build
cmake --build build -j
```

## Verification

```bash
ctest --test-dir build --output-on-failure
python3 scripts/verify/audio_regression.py --input fixtures --baseline baselines
```

For AU host validation details, see:
- `docs/testing/logic-au-validation.md`
