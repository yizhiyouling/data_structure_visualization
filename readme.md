# 数据结构可视化实验室

## 项目简介

“数据结构可视化实验室”是一个基于 Qt6 和 C++20（向下兼容 C++17）的桌面应用程序，用于演示和交互式学习常见数据结构及其操作，包括：

- **单链表**  
- **双向链表**  
- **二叉树**  
- **树的遍历**（前序、中序、后序、层序）  
- **图（待开发）**  

通过可视化节点和指针/边，帮助用户直观理解数据结构的插入、删除、遍历等基本操作过程。

---

## 环境要求

- 操作系统：Windows / macOS / Linux  
- CMake ≥ 3.16  
- C++ 编译器，支持 C++17 或以上（推荐 C++20）  
- Qt6（或 Qt5）开发库与工具链  
- Qt Creator（可选，推荐用于快速调试与编辑）

---

## 构建与运行

1. **克隆仓库**  
   ```bash
   git clone https://github.com/yizhiyouling/data_structure_visualization.git
   cd data_structure_visualization

1. **创建构建目录**

   ```bash
   mkdir build
   cd build
   ```

2. **生成项目文件**

   ```bash
   cmake .. 
   # 如果使用 Qt5，可添加 -DCMAKE_PREFIX_PATH=/path/to/Qt5
   ```

3. **编译并运行**

   ```bash
   cmake --build . --config Release
   ./data_structure_visualization   # macOS/Linux
   data_structure_visualization.exe # Windows
   ```

------

## 项目结构

```
data_structure_visualization/
├── CMakeLists.txt
├── main.cpp
├── MainWindow.h/.cpp
├── NodeItem.h/.cpp
├── ListNodeItem.h/.cpp
├── ArrowItem.h
├── SinglyLinkedListWidget.h/.cpp
├── DoublyLinkedListWidget.h/.cpp
├── BinaryTreeWidget.h/.cpp
├── TreeTraversalWidget.h/.cpp
├── GraphWidget.h/.cpp
└── README.md
```

- **main.cpp** & **MainWindow**
   应用程序入口，创建 `QStackedWidget` 并管理各个模块页面及菜单切换。
- **NodeItem** & **ListNodeItem**
   通用的图形节点类，用于链表和树节点的绘制。
- **ArrowItem**
   自定义箭头，用于指针/边的可视化。
- **SinglyLinkedListWidget**
   单链表模块：支持尾部插入、尾部删除、指定节点后插入、指定节点删除、清空。
- **DoublyLinkedListWidget**
   双向链表模块：支持尾部插入、尾部删除、指定节点后插入、指定节点删除、清空，并展示双向指针。
- **BinaryTreeWidget**
   二叉树模块：支持节点动态添加、删除与场景自动布局。
- **TreeTraversalWidget**
   树的遍历模块：预构建 15 个节点的完全二叉树，支持前序、中序、后序、层序遍历并高亮动画。
- **GraphWidget**
   图模块（待开发）。

------

## 使用说明

1. 启动程序后，默认显示“单链表”模块。
2. 通过菜单栏切换到不同数据结构模块。
3. 在各模块中，使用提供的按钮完成对应操作，查看动画和节点布局变化。
4. 在“树的遍历”模块中，点击遍历按钮，即可看到节点和边的高亮动画，并在右侧日志中显示访问路径。



------

## 致谢

感谢 Qt 社区提供的优秀框架和工具，期望本项目能帮助更多同学理解数据结构的动态演示与交互式学习。