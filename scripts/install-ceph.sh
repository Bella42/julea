#!/bin/bash

SELF_PATH="$(readlink --canonicalize-existing -- "$0")"
SELF_DIR="${SELF_PATH%/*}"

. "${SELF_DIR}/common"

CEPH_DIR="$(get_directory "${SELF_DIR}/..")/dependencies/ceph"

git clone --recursive --single-branch https://github.com/ceph/ceph.git "${CEPH_DIR}"
cd "${CEPH_DIR}"
#./install-deps.sh
sed -i "s/  add_definitions(-DROCKSDB_MALLOC_USABLE_SIZE)/#  add_definitions(-DROCKSDB_MALLOC_USABLE_SIZE)/g" "${CEPH_DIR}/src/rocksdb/CMakeLists.txt"
./do_cmake.sh -DCMAKE_BUILD_TYPE=RelWithDebInfo -DWITH_BLUESTORE=ON -DWITH_BLUEFS=ON -DWITH_TESTS=OFF -DWITH_RADOSGW=OFF -DWITH_MGR=OFF
cd build
make -j48
