# lr-core-vector

用 C 语言手写一个 `std::vector`：你要做的事只有一件：根据 `include/vector.h` 的描述**把 `src/vector.c` 里的空壳函数填成能用的实现，让 `make test` 全绿**。

[vector 原理可视化](https://lingrui-studio.github.io/vector-playground/)

本题实现的是只存储 `int` 的教学版动态数组，具体约定以 [include/vector.h](include/vector.h) 为准。

## 目录结构

```
lr-core-vector/
├── readme.md         本文件
├── Makefile          构建脚本（不用改）
├── .gitignore        列举 git 需要忽视的文件
├── .clang-format     格式化要求
├── include/
│   └── vector.h      接口声明 + 函数注释（不用改，但要读懂）
├── src/
│   └── vector.c      ★ 你要实现的地方
└── tests/
    └── test.c        单元测试（不用改）
```

## 自检

补全 [include/vector.c](include/vector.c) 中的函数实现后，项目根目录运行 `make test`，若最后输出结果如下即表示你完成了本项目（本项目只有未完成和已完成两种状态，不存在中间值）：

```bash
== 通过 3393 项，失败 0 项 ==
全部通过，可以 commit & push 了
```

测试始终开启 ASan + UBSan，检测到错误会以失败状态退出，不提供关闭开关。常见错误会被直接指出来，例如：

```
ERROR: AddressSanitizer: heap-buffer-overflow on address 0x... at pc 0x...
READ of size 4 at 0x... thread T0
    #0 0x... in get src/vector.c:52
```

行号会直接指到出问题的那一行，看不懂的把完成代码和报错信息复制给 AI 问一下。

## 提交

- 完成下面的`实现思路`一节，简要说明你的各个函数是如何实现的，尤其注意内存管理的说明
- 把所有修改 commit 并 push 到 GitHub 上自己的 vector 仓库
- 在个人仓库的 Actions 页面手动触发一次自动评分工作流

## 实现思路

我的函数一共有 14 个，如下

1. `vector_init`
   需要思考的部分如下
   `capacity` 为 `0` 时不分配内存，因为 `malloc(0)` 为实现定义行为
   `capacity > SIZE_MAX / sizeof(int)` 不能转化成乘法，不然可能回绕
   后文不再阐述类似的问题
2. `vector_destroy`
   记得先 `free(v->data)`
3. `size`
   先特判空指针
   然后 `size` 就是 `v->data` 到 `v->end` 的距离，记得转化类型为 `size_t`
4. `capacity`
   类似 `size`，将 `v->end` 改为 `v->cap`
5. `empty`
   判断 `size` 是否为 `0`
6. `get`
   特判 `index >= size(v)`
7. `set`
   和 `get` 差不多
8. `front`
   先特判 `empty`，然后 `get` 第 `0` 个元素
9. `back`
   和 `front` 差不多，`get` 第 `size(v)-1` 个元素
10. `push_back`
    这里要用到后面的 `reserve`
    注意 `old_cap == 0` 时，将 `new_cap` 赋值为 `1`
    内存管理的关键点在后文的 `reserve` 中
11. `pop_back`
    特判 `empty`
    先用 `back` 获得最后一个元素，然后 `v->end--;` 去掉最后一个元素
12. `reserve`
    唯二有技术含量的函数之一
    注意 `capacity` 变量重名了，得改
    先特判 `new_cap` 的大小是否不符合条件
    在 `realloc` 时，可以注意到必须新建指针 `*p` 防止内存管理中遇到 `realloc` 失败并返回空指针，无法释放原本地址的内存
13. `shrink_to_fit`
    唯二有技术含量的函数之一
    如果 `size(v) == 0` 则 `vector_destroy(v);`
    若 `size == capacity(v)` 则无需操作
    否则直接 `realloc` 并修改 `*v` 的其他成员，参见 `reserve` 中的注意事项
14. `clear`
    将 `size` 清零只需将 `v->end` 赋值为 `v->data`
