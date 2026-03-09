#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
SKIP_BUILD=0
SKIP_TESTS=0
SKIP_REGRESSION=0
SKIP_AUVAL=0
INSTALL_AU=0

print_help() {
  cat <<'EOF'
Usage: scripts/release/run-validation.sh [options]

Options:
  --build-dir PATH    Override build directory (default: ./build)
  --skip-build        Skip cmake configure/build step
  --skip-tests        Skip unit tests
  --skip-regression   Skip audio regression step
  --skip-auval        Skip auval check
  --install-au        Temporarily install AU component before auval check
  -h, --help          Show this help message

Flow:
  1) configure + build (unless --skip-build)
  2) run tests (unless --skip-tests)
  3) optional audio regression (unless --skip-regression)
  4) optional auval (unless --skip-auval)
EOF
}

log() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $*"
}

run_step() {
  local label="$1"
  shift
  log "$label"
  "$@"
}

has_command() {
  command -v "$1" >/dev/null 2>&1
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --build-dir)
      [[ $# -lt 2 ]] && { echo "Missing argument: --build-dir PATH" >&2; exit 1; }
      BUILD_DIR="$2"
      shift 2
      ;;
    --skip-build)
      SKIP_BUILD=1
      shift
      ;;
    --skip-tests)
      SKIP_TESTS=1
      shift
      ;;
    --skip-regression)
      SKIP_REGRESSION=1
      shift
      ;;
    --skip-auval)
      SKIP_AUVAL=1
      shift
      ;;
    --install-au)
      INSTALL_AU=1
      shift
      ;;
    -h|--help)
      print_help
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      print_help
      exit 1
      ;;
  esac
done

run_step "Step 1/4: configure + build"
if [[ "$SKIP_BUILD" == "1" ]]; then
  if [[ ! -d "$BUILD_DIR" ]]; then
    echo "Build dir '$BUILD_DIR' not found. Remove --skip-build or run build first." >&2
    exit 1
  fi
else
  run_step "  cmake configure" cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DBUILD_TESTING=ON
  run_step "  cmake build" cmake --build "$BUILD_DIR" -j
fi

if [[ "$SKIP_TESTS" == "0" ]]; then
  run_step "Step 2/4: test"
  if has_command ctest; then
    run_step "  ctest" ctest --test-dir "$BUILD_DIR" --output-on-failure
  elif [[ -x "$BUILD_DIR/mixcopilot_tests" ]]; then
    run_step "  test binary" "$BUILD_DIR/mixcopilot_tests"
  else
    echo "No test runner available. Install CTest (or keep previous build outputs)." >&2
    exit 1
  fi
else
  log "Step 2/4: tests skipped"
fi

if [[ "$SKIP_REGRESSION" == "0" ]]; then
  run_step "Step 3/4: audio regression"
  REGRESSION_SCRIPT="$ROOT_DIR/scripts/verify/audio_regression.py"
  if [[ -f "$REGRESSION_SCRIPT" && -d "$ROOT_DIR/fixtures" && -d "$ROOT_DIR/baselines" ]]; then
    run_step "  run regression script" python3 "$REGRESSION_SCRIPT" --input "$ROOT_DIR/fixtures" --baseline "$ROOT_DIR/baselines"
  else
    log "  skipped (missing script or fixtures/baselines directories)"
  fi
else
  log "Step 3/4: regression skipped"
fi

if [[ "$SKIP_AUVAL" == "0" ]]; then
  run_step "Step 4/4: auval"
  COMPONENT_DIR="$BUILD_DIR/MixCopilotAU_artefacts/AU/MixCopilot.component"
  if has_command auval && [[ -d "$COMPONENT_DIR" ]]; then
    if [[ "$INSTALL_AU" == "1" ]]; then
      INSTALL_DIR="$HOME/Library/Audio/Plug-Ins/Components"
      run_step "  install to user AU directory" mkdir -p "$INSTALL_DIR" && cp -R "$COMPONENT_DIR" "$INSTALL_DIR/"
      run_step "  validate (auval)" auval -v aufx MXC1 MXCP
      run_step "  cleanup temp install" rm -rf "$INSTALL_DIR/MixCopilot.component"
    else
      run_step "  validate (auval)" auval -v aufx MXC1 MXCP
    fi
  else
    log "  skipped (auval binary or component not available)"
  fi
else
  log "Step 4/4: auval skipped"
fi

log "Validation complete"
