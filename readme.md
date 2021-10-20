# 这是一个第三方库的自动化编译流程

这里存在两个版本：

1. 根目录下的脚本（早期版本）
2. csm目录中的脚本（改进版本）



# 后续主要维护改进版本

直接放弃了32位支持，除非必要。比如emsdk只有32bit。

改进版本支持情况：

1. windows 本地编译 x64  release debug dynamic static 四个组合。
2. ubuntu 本地编译 x64  release debug dynamic static 四个组合。
3. windows 交叉交叉编译 android 64位  release debug dynamic static 四个组合。
4. windows 交叉编译 emsdk  release debug 只有static版本。
5. 其他版本待续。



# 编译结果检查程序

提供多个检查编译结果正确性的程序，目前还没有清理，后续再说。





