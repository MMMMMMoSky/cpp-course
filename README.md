# 武汉大学 高级语言程序设计实验

> 2017级 计科1班 吴昊 2017301500015

## 2.1 自回避随机行走问题

### 问题描述

给定一个 N x N 的网格地图, 初始在地图中心.

每次随机向**四个相邻位置中还没有走过的点**移动, 如果无路可走则陷入死胡同.

编程**模拟**以上过程, 进行多轮尝试, 分析:

1. 陷入死胡同的概率
2. 行走的平均长度
3. 成功逃出时的平均路径长度
4. 陷入死胡同时的平均路径长度
5. N与陷入死胡同的概率的关系

### 核心函数

随机行走, 传入引用以实现多个返回值, ok表示是否成功逃出, length表示路径长度

```C++
void randomWalk(int N, bool &ok, int &length);
```

### Random类

用于获取随机数, 对rand()和srand()的一个封装.

*累计用时 15min*

## 2.2 表达式树的建立和遍历

### 问题描述

根据输入的中缀表达式建立二叉树, 然后对二叉树进行遍历, 再生成中缀表达式和后缀表达式

### 主流程

- 输入中缀表达式
  - 格式处理: 去掉空格
  - 合法性检查: 括号匹配; 运算符是否相邻/首尾
- 将中缀表达式拆分, 递归建立二叉树
  - 设计建树算法
  - 以某种形式打印二叉树
- 对二叉树进行遍历, 得到中缀表达式
- 对二叉树进行遍历, 得到后缀表达式

### 数据结构

使用枚举标识二叉树的一个节点是什么类型的

```C++
enum AtomicExpression {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVISION,
    NUMBER
};
```

储存表达式的二叉树节点

```C++
class ExpressionBinaryTreeNode {
    string val;                         // 该节点的值(可能是数字, 也可能是运算符)
    AtomicExpression type;              // 该节点的类型
    ExpressionBinaryTreeNode *left;     // 左子节点指针
    ExpressionBinaryTreeNode *right;    // 右子节点指针
};
```

### 建树算法

递归建树(保证传入的表达式是合法的, 可以建树的)

```C++
ExpressionBinaryTreeNode* createFromInfix(string& expression);
```

函数流程:

1. 如果expression两端含有括号则先去掉括号
2. 如果expression只含数字, 即到达叶子节点, 新建节点并返回
3. **从后往前**找到第一个不在括号内的 +- 当前节点为该运算符, 然后以两侧子串递归建立左右子树
4. 如果3没有找到, 则再次寻找第一个不在括号内的 */

### 遍历得到中缀表达式

由表达式二叉树中序遍历即可得到中缀表达式, 途中需要考虑加括号的问题.

```C++
string toInfixExpression(ExpressionBinaryTreeNode* rt);
```

传入二叉树的根节点, 返回该根节点代表的子树的中缀表达式

1. 如果当前节点为叶子节点, 直接返回
2. 递归获取左右子树对应的表达式
3. 对左右子树的表达式加括号
4. 相加得到当前子树代表的表达式, 返回

加括号策略:

- 定义优先级: 加减 \< 乘除 \< 数字
- 左子节点优先级 \< 当前节点优先级 则需要给左子树的表达式加括号
- 右子节点优先级 \<= 当前节点优先级 则需要给右子树的表达式加括号

### 遍历得到后缀表达式

注意: 后缀表达式每一个元素之间需要用空格分隔

较简单, 直接后序遍历即可

## 3.1 图的深度优先遍历

### 问题描述

给定某个图的结构数据和起点位置, 创建出图后, 再采用深度优先遍历算法, 输出所有的遍历节点序列.

### 简要分析

DFS基础题目. 

### 图结构的存储

使用 vector 式的邻接表, 每一个节点使用一个 vector 储存它可以到达的点.

每个节点由唯一的编号标识, 从 0 开始.

图类 `DirectedGraph`, 储存一个图, 封装添加边, 获取子节点列表的操作

```C++
class DirectedGraph {
private:
    int nodeNum, edgeNum;                       // 节点和边的数量
    vector<vector<int>> linkedNodes;            // linkedNodes[i]表示编号为i点节点可以到达的点
public:
    void addEdge(int u, int v);                 // 添加一条从 u 到 v 的有向边   
    const vector<int> &getLinkedNodes(int u);   // 获取 u 可以到达的点的列表
};
```

### RangeIO 类

封装带范围的读入, 方便规定读入的范围. 实现方法是从给定的流读入一个 string, 然后手动判断是否包含非法字符, 在 try 块中调用 stoi 转换成 int, 捕获异常.

如果监测到非法字符, 或者捕获到异常, 则输出错误信息, 并提醒用户重新输入. 如果成功转换, 再判断范围, 超出范围也提醒用户重新输入.

`readInt` 与 `readIntFromLine` 的不同仅在于读入的时候是直接使用 `in >> {string}` 还是 `getline(in, {string})`

```C++
// 静态方法
readInt(istream &in);
readInt(istream &in, int maxVal);   // 读入一个[0, maxVal]的整数
readInt(istream &in, int minVal, int maxVal);

// 读入一行
readIntFromLine(istream &in);
readIntFromLine(istream &in, int maxVal);
readIntFromLine(istream &in, int minVal, int maxVal);
```

### Depth-First Search

递归地访问图中所有的节点, 使用一个 bool 数组标识每个点是否访问过. 

`getDfsPath` 是dfs的过程, dfs遍历图, 获取路径.

```C++
/**
 * @param now (int): 当前正在访问的节点编号
 * @param g (DirectedGraph): 当前dfs遍历的图
 * @param visited (vector<bool>): visited[i] 表示编号为i的点是否已经访问过
 * @param path (vector<int>): 记录dfs的路径
 * @return: void
 */
void getDfsPath(int now, DirectedGraph &g, vector<bool> &visited, vector<int> &path);
```

`printDfsPath(startNode, g, visited)` 则是直接打印以 startNode 为起点的dfs路径, 其中会调用 `getDfsPath` 来获取路径. 

这样封装是为了方便 "当图不是联通的时候, 需要多次dfs", 每次都要重新选择起点, 而这些dfs要共享 visited 数组.


### 程序主要流程

1. 由用户输入图的大小(节点数量和边的数量)
2. 用户选择手动添加边或者随机生成边
3. 用户选定起点
4. 对图 dfs
5. 将dfs的路径打印

## 3.2 象棋中马的遍历

### 问题描述

在 N * N 的棋盘上的任意一个位置放置一个马, 选择一套合适的移动路线使得马按照 "马走日" 的移动规则不重复地遍历棋盘上的每一个点.

扩展: 设定 "蹩马腿" 的棋子

### 简要分析

这是一道比较简单的搜索题目. DFS遍历整个搜索树, 找到一条可行的路径即可.

### 回溯法

核心函数

```C++
bool traceBack(
    int x, int y, int N, int M, int left,
    vector<vector<bool>> &visited,
    vector<int> &path);
```

- (x, y) 表示马当前的位置
- N * M 表示地图的大小
- left 表示还剩下多少个点没有访问过
- visited[i][j] 表示点(i, j)是否访问过
- paths 表示到目前为止的路径, 以0-7表示马可以走的8种路径
- 如果找到路径, 则返回true

函数处理流程:

1. 如果 left 为 0 则返回 true
2. 把当前点标识为已访问过
3. 枚举8个方向, 如果对应的移动落点还没有被访问过
  1. 将这个方向放入 path 中
  2. 向这个方向跳一步, 即递归调用 (注意 left - 1)
  3. 如果上面的递归调用返回了 true, 代表找到了路线, 也直接返回true
  4. 反之如果返回了false, 应该弹出刚刚放入path的方向
4. 返回 false (当前的状态下, 往哪个方向走都找不到路)

### 扩展要求

对于设定的蹩马腿的棋子, 直接对 visited 数组的初始值进行设定即可.

### 程序主要流程

1. 由用户输入棋盘大小, 并选择是否设定蹩马腿的棋子
2. 初始化 visited, path 数组
3. 调用函数 traceBack 回溯法寻找解
4. 若找到解, 打印输出