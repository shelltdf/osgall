#!/bin/bash
#
# Copyright (c) 2019-2023 Ruben Perez Hidalgo (rubenperez038 at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

set -e

BK=docs
IMAGE=build-docs
CONTAINER=builder-$IMAGE
FULL_IMAGE=ghcr.io/anarthal-containers/$IMAGE

docker start mysql
docker start $CONTAINER || docker run -dit \
    --name $CONTAINER \
    -v ~/workspace/mysql:/opt/boost-mysql \
    -v /var/run/mysqld:/var/run/mysqld \
    $FULL_IMAGE
docker network connect my-net $CONTAINER || echo "Network already connected"
docker exec $CONTAINER python /opt/boost-mysql/tools/ci.py --source-dir=/opt/boost-mysql --server-host=mysql \
    --build-kind=$BK \
    --build-shared-libs=1 \
    --valgrind=0 \
    --coverage=0 \
    --clean=0 \
    --toolset=clang \
    --cxxstd=20 \
    --variant=debug \
    --cmake-standalone-tests=1 \
    --cmake-add-subdir-tests=1 \
    --cmake-install-tests=1 \
    --stdlib=native

if [ "$BK" == "docs" ]; then
    usr=$(whoami)
    sudo chown "$usr" -R ~/workspace/mysql/doc/html
    sudo chgrp "$usr" -R ~/workspace/mysql/doc/html
fi
