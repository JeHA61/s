#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
INSTALL_DIR="$HOME/Library/Audio/Plug-Ins/Components"
COMPONENT_NAME="MixCopilot.component"
OPEN_LOGIC=0
SKIP_BUILD=0
SKIP_TESTS=1
SKIP_AUVAL=1
JOBS=""

print_help() {
  cat <<'USAGE'
Usage: scripts/dev/install-for-logic.sh [options]

Installs (or reinstalls) the AU component into the user component folder for Logic.

Options:
  --build-dir PATH    Override build directory (default: ./build)
  --skip-build        Skip cmake configure/build step
  --run-tests         Run unit tests before install
  --verify-au         Run auval after install
  --open-logic        Open Logic after install
  --jobs N            Parallel build jobs (cmake default if omitted)
  --help              Show this help message
USAGE
}

log() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $*"
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
    --run-tests)
      SKIP_TESTS=0
      shift
      ;;
    --verify-au)
      SKIP_AUVAL=0
      shift
      ;;
    --open-logic)
      OPEN_LOGIC=1
      shift
      ;;
    --jobs)
      [[ $# -lt 2 ]] && { echo "Missing argument: --jobs N" >&2; exit 1; }
      JOBS="$2"
      shift 2
      ;;
    --help|-h)
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

COMPONENT_DIR="$BUILD_DIR/MixCopilotAU_artefacts/AU/$COMPONENT_NAME"

if [[ "$SKIP_BUILD" == "0" ]]; then
  cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DBUILD_TESTING=$( [[ "$SKIP_TESTS" == "0" ]] && echo ON || echo OFF )
  if [[ -n "$JOBS" ]]; then
    cmake --build "$BUILD_DIR" -j"$JOBS"
  else
    cmake --build "$BUILD_DIR" -j
  fi
else
  if [[ ! -d "$BUILD_DIR" ]]; then
    echo "Build dir '$BUILD_DIR' not found. Remove --skip-build or run build first." >&2
    exit 1
  fi
fi

if [[ ! -d "$COMPONENT_DIR" ]]; then
  echo "Component not found: $COMPONENT_DIR" >&2
  exit 1
fi

mkdir -p "$INSTALL_DIR"
if [[ -d "$INSTALL_DIR/$COMPONENT_NAME" ]]; then
  rm -rf "$INSTALL_DIR/$COMPONENT_NAME"
fi
cp -R "$COMPONENT_DIR" "$INSTALL_DIR/"
log "Installed AU component to $INSTALL_DIR"

if [[ "$SKIP_AUVAL" == "0" ]]; then
  if command -v auval >/dev/null 2>&1; then
    auval -v aufx MXC1 MXCP
  else
    echo "auval command not available." >&2
    exit 1
  fi
fi

log "Plugin installed. Open Logic and rescan plugins to pick up the new AU component."
log "Installed bundle: $INSTALL_DIR/$COMPONENT_NAME"

if [[ "$OPEN_LOGIC" == "1" ]]; then
  open -a "Logic Pro"
fi
