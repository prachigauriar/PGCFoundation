#!/bin/sh

[ "${GENERATE_DOCUMENTATION}" = "NO" ] && exit 0

[ -z "${SRCROOT}" ] && SRCROOT="${HOME}/Developer/PGCFoundation"
[ -z "${TARGET_NAME}" ] && TARGET_NAME="PGCFoundation"
[ -z "${DOCUMENTATION_ROOT}" ] && DOCUMENTATION_ROOT="${SRCROOT}/${TARGET_NAME}/Documentation/Headerdoc"

rm -rf "${DOCUMENTATION_ROOT}"

headerdoc2html -q -o "${DOCUMENTATION_ROOT}" "${SRCROOT}/${TARGET_NAME}/Documentation/${TARGET_NAME}.hdoc" > /dev/null 2>&1 
find "${SRCROOT}" -name "*.h" -print0 | xargs -0 headerdoc2html -q -o "${DOCUMENTATION_ROOT}" > /dev/null 2>&1 
gatherheaderdoc "${DOCUMENTATION_ROOT}" > /dev/null 2>&1 
