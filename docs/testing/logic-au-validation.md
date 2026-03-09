# Logic AU Validation Checklist

The quickest full check is:

```bash
scripts/release/run-validation.sh
```

개발용 빠른 실행:

```bash
scripts/dev/install-for-logic.sh --skip-build
scripts/dev/install-for-logic.sh --run-tests --verify-au --open-logic
```

You can also run only part of the pipeline:

```bash
scripts/release/run-validation.sh --skip-regression --skip-auval
```

## 1) Build AU component

```bash
export PATH="$HOME/Library/Python/3.14/bin:$PATH"
cmake -S . -B build
cmake --build build -j
```

## 2) Install to user component path

```bash
mkdir -p ~/Library/Audio/Plug-Ins/Components
cp -R build/MixCopilotAU_artefacts/AU/MixCopilot.component ~/Library/Audio/Plug-Ins/Components/
```

## 3) Validate with auval

```bash
auval -v aufx MXC1 MXCP
```

Expected:
- no crash
- validation passes

## 4) Confirm component appears in global list

```bash
auval -a | grep -i MixCopilot
```

## 5) Run automated checks

```bash
ctest --test-dir build --output-on-failure
python3 scripts/verify/audio_regression.py --input fixtures --baseline baselines
```
