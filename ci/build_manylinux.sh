#!/usr/bin/env bash
set -euxo pipefail

# Downgrade auditwheel
# see: https://github.com/pypa/auditwheel/issues/137
#/opt/_internal/cpython-3.7.7/bin/pip install auditwheel==2.0.0

SKIP_PLATFORMS=(cp27-cp27m cp27-cp27mu cp34-cp34m)

for PYROOT in /opt/python/*; do
  PYTAG=$(basename "${PYROOT}")
  echo "$PYTAG"
  # Check for platforms to be skipped
  # shellcheck disable=SC2199,SC2076
  if [[ " ${SKIP_PLATFORMS[@]} " =~ " ${PYTAG} " ]]; then
    echo "skipping deprecated platform $PYTAG"
    continue
  fi

  # Create and activate virtualenv
  PYBIN="${PYROOT}/bin"
  "${PYBIN}/pip" install virtualenv
  "${PYBIN}/virtualenv" -p "${PYBIN}/python" "venv_${PYTAG}"
  # shellcheck source=/dev/null
  source "venv_${PYTAG}/bin/activate"
  #pip install -U pip setuptools wheel

  # Clean the build dir
  BUILD_DIR="build_$PYTAG"
  rm -rf "${BUILD_DIR}"

  LIB="${PYROOT}/lib/libpython$(python -V | grep -o "[0-9]\+.[0-9]\+.[0-9]\+").so"
  touch "$LIB"
  cmake -S. "-B${BUILD_DIR}" \
    -DBUILD_PYTHON=ON \
    -DPython_FIND_VIRTUALENV=ONLY \
    -DPython_LIBRARY="${LIB}"

  cmake --build "${BUILD_DIR}" -v

  # Restore environment
  deactivate
done
