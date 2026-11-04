#!/bin/sh
# Full build inside a Docker container whose repo is a Windows bind mount at /dw4
# (Docker on Windows). EE-GCC is 32-bit and its stat() fails with EOVERFLOW on the mount (large
# inode numbers), so build in a container-local copy (/work), synced from /dw4.
# Needs: /opt/venv (requirements.txt), ninja, libc6:i386, EE-GCC copied to /opt/ee-gcc2.96.
# Usage: docker exec dw4 sh /dw4/tools/ninja_container.sh [ninja args]
set -e
command -v rsync >/dev/null || { apt-get install -y -qq rsync >/dev/null 2>&1; }
mkdir -p /work/disc
rsync -a --delete \
    --exclude .git/ --exclude /disc/ --exclude /build/ --exclude /asm/ --exclude /assets/ \
    --exclude /tools/ee-gcc2.96/ --exclude /build.ninja --exclude /.ninja_log --exclude /.ninja_deps \
    --exclude /objdiff.json --exclude /.splat/ \
    /dw4/ /work/
[ -f /work/disc/SLUS_208.36 ] || cp /dw4/disc/SLUS_208.36 /work/disc/
[ -x /work/tools/ee-gcc2.96/bin/ee-gcc ] || cp -r /opt/ee-gcc2.96 /work/tools/ee-gcc2.96
cd /work
/opt/venv/bin/python configure.py > /tmp/configure.log 2>&1 || { tail -30 /tmp/configure.log; exit 1; }
tail -1 /tmp/configure.log
ninja "$@" > /tmp/ninja.log 2>&1 && r=0 || r=$?
tail -15 /tmp/ninja.log
exit $r
