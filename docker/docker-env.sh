#!/bin/bash
PROJ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../" > /dev/null && pwd)"
docker run -v"${PROJ_DIR}":/home/proj -w"/home/proj" --rm --device /dev/ttyUSB0 ${@:1} -ti env bash