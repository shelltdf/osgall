
# mkdir rootfs
# mkdir rootfs/bin

# cp -a /usr/bin/bash rootfs/bin
# chmod 777 rootfs/bin

# cp -a /usr rootfs/
# cp -a /lib rootfs/
# cp -a /lib64 rootfs/

# mkdir rootfs
# mkdir rootfs/usr
# cp -a /bin rootfs/
# cp -a /usr rootfs/
# cp -a /etc rootfs/

# /usr/share/debootstrap/scripts
# sudo debootstrap --arch=i386 trusty rootfs
# sudo debootstrap --arch=amd64 trusty rootfs
# sudo debootstrap --arch=arm64 trusty rootfs
# sudo debootstrap --arch=powerpc trusty rootfs

sudo debootstrap trusty rootfs

# sudo chroot rootfs
