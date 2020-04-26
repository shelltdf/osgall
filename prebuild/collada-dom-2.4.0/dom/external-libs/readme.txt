In order to preserve compatibility with different compilation methods, different versions of the same library are also included:

old: minizip
new: minizip-1.1

old: pcre
new: pcre-8.02

new: zlib-1.2.5

The cmake build system uses the latest libraries. If it becomes mainstream and the other Makefiles are removed, then it will be safe to remove the old versions of these.
