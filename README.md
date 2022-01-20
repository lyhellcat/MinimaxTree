# 基于Minimax tree的跳棋博弈系统

## 背景介绍  

跳棋是一种由两名玩家玩的策略棋盘游戏。跳棋有很多变体。本系统的任务是实现一个智能决策的人机跳棋对战系统。

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image002.jpg)

跳棋具有多种多样的游戏规则。本系统的跳棋规则叙述如下：

（1）**设定**。一个 8x8 棋盘，有 12 个黑子和 12 个白子，初始位置如图 1a 所示。

（2）**游戏玩法**。每位棋手所下的棋子都是同一颜色的。黑方先走一步，然后白方走一步，然后双方轮流下棋。在一个回合中，棋手要么选择移动棋子，要么选择吃子。例如，图1a中的箭头表示黑棋从G6单元向F5单元移动。

（3）**移动**。棋子可以斜向前移动到一个空单元格（朝向对手，黑棋为北，白棋为南）一格。图 1b 中的箭头显示了黑白棋子的所有合法走法。

（4）**塔**。当一个棋子到达最远的一排（黑棋为最上一排，白棋为最下一排），它就成为一座塔。图1b中D7单元的白棋唯一的一步棋就把它提升到了塔中。A塔可以在对角线上向前或向后移动一个空格子。图1c中的箭头显示了黑、白塔的所有合法棋步。塔和普通棋子的区别在于，**塔可以向4个方向任意移动**。

（5）**吃子**。为了吃掉对方的棋子或塔，棋手的棋子或塔要跳过它并落在在另一边的直线对角线上。这个落子格必须是空的。当一个棋子或塔被吃掉后它就会从棋盘上移走。在一次跳跃中只能吃掉一个棋子或塔，而在我们的变体游戏中，只允许跳跃一次。在我们的游戏中，一个回合中只允许跳一次。因此，如果在第一次跳棋之后还有另一个棋子可以捕获，那么就不能选择在这一回合中吃子。此外，在游戏中，如果一个棋手可以选择出棋或吃子，他们可以任意决定完成哪个行为。棋子总是向前跳（朝向对手），而塔可以向前和向后移动。图1d中的箭头显示了双方棋手的所有合法吃子。

（6）**游戏结束**。如果轮到对手并且他们无法采取行动（移动棋子或吃子），则当前的玩家赢得游戏。

## 需求分析  

### **Stage 0 -**  **读取、分析和打印棋盘**

第0个阶段，程序应能够实现读取输入并打印初始的棋盘设定和执行所有的合法操作。预期的输出如下图所示。

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image004.jpg)

第1-21行输出棋盘的配置，并按照图1a进行初始设定。我们用'b'和'w'字符分别表示黑棋和白棋。然后，第22-42行打印由执行用户输入的第一步棋后棋盘的状态。每一步执行后，将会输出以37个'='字符的分隔线，然后接下来的两行打印所执行的动作和棋盘成本的信息；见第23和24行。

一个棋盘的成本计算为![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image006.png)，其中![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image008.png)和![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image010.png)分别是黑棋、白棋、白塔、黑塔的数量。棋盘上的黑棋、白棋、黑塔和白塔的数量；也就是说，一座塔的成本相当于三颗棋子。然后，程序应该打印出执行该动作后的棋盘状态。

如果用户输入了非法的状态，那么程序应该在接受用户输入后输出错误信息并终止。错误信息如下所示。

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image012.jpg)

错误的条件是不言自明的，应该按照消息列出的顺序进行判断。

###  **Stage 1 –** **计算并打印下一步预测的动作**

如果用户输入了‘A’，则应该通过minimax tree实现决策，预测出下一步应执行的动作。

首先，从当前状态（第0层）开始，构建所有可达到的棋盘状态的树，并达到要求的深度；如果同一棵树中同一棋盘可达到多次，必须复制相应的树节点。例如，黑棋在图3a中可以下两步棋：A6的塔可以走到B5（图3b），C8的棋可以走到D7（图3c）；见图2中的第1层。图2中的树明确显示了在深度为3的最小化树中，黑色回合和图3a中的棋盘的所有30个棋盘配置中，有15个节点是指这些配置。节点的标签指的是图3中显示的相应棋盘。例如，树的第二层中标签为(f)-(h)的节点分别指的是图3f至3h中的棋盘，它们是白棋通过在图3c的棋盘中下棋和吃子可以到达的所有棋盘。

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image014.jpg)

 

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image016.jpg)

第二步，计算所有棋盘的成本；见灰色背景的节点（第3级）。例如，从图3d中的棋盘可以到达树的第3层的六个棋盘状态。图3i和3j中的棋盘的成本为3，而通过棋盘（d）中B5单元的塔的所有棋步可以到达的四个棋盘（图中未显示）的成本为2。 直观地说，正的成本表明黑方获胜，负的成本说明白方获胜，而成本的大小表明一方对另一方的优势。

第三步，对于玩家的每一个可能的动作，我们检查对手所有可能的动作，并选择玩家的下一个动作作为从树根到玩家最大化的叶节点的路径上的第一个动作他们的收益，而对手（被认为是理性的）旨在最小化玩家的收益；参见图 2 中的红色路径。黑棋在树的第 2 层的棋盘上采取行动。在这一层，黑棋的目标是通过选择对成本最高的棋盘采取行动来最大化收益。对于棋盘（d）来说，朝向棋盘（i）和（j）的成本是3，朝向（k）的（e）是1，朝向（m）的（f）是0，朝向（n）的（g）是1，朝向（o）的（h）是1。第3层的节点显示的数字是当前棋盘的成本，第2层节点标记的数字是由下层节点传播而来的成本。白方的目标是使他们的收益最大化，也就是使黑方的收益最小化。因此，在第1层的每一个棋盘上，白方都会对第2层的棋盘选择一个具有从第3层传播的最低成本的行动。

对于棋盘（b）向（e）状态转化的成本是1，而对于棋盘（c）向（f）状态转化的成本是0。同样，可以观察从第2层到第1层的箭头和第1级的节点标签的成本。最后，为了使他们的收益最大化，黑方在对局中选择下一步行动，即导致在第1级传播成本最高的棋盘。对应决策树的路径为（k）-（a），对应的决策为"A6-B5"。假设白棋是理性的，则下一步应该下 "B7-A8"（转换到状态（e））。

为了计算白棋方的下一个动作，决策树中最大和最小层的顺序将被反转。如果根的几个子代具有相同的传播最大/最小费用，则必须选择最左边的那个子节点的行动。为了构建子节点，无论是黑棋还是白棋，棋盘都是按照行优先的顺序遍历的, 对于每个遇到的棋子或塔，都会探索所有可能的动作，从东北方向开始，顺时针进行。应按照构造顺序从左到右添加子项。

## **Stage 2 –** **机器游戏**

如果'P'命令跟在输入动作后面，你的程序应该执行10次下一个动作或是预测所有动作，直到游戏结束。如果游戏在接下来的十个回合内结束，则应该输出赢家。行动的决策和赢家的输出应遵循第1阶段的规则。

### 模块设计  

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image018.jpg)

系统设置了输出棋盘、计算棋盘的当前成本、验证用户输入是否合法、执行用户输入的动作以及构建Minimax决策树等模块。

部分函数原型及说明如下: 

```C
void InitBoard(board_t board)     // 初始化棋盘
void OutputBoard(board_t board)     // 用于输出当前棋盘
int CountCost(board_t board)      // 计算当前棋盘开销
// 检查从(x1, y1) -> (x2, y2)的移动是否合法
int checkMove(int x1, int y1, int x2, int y2, board_t board)
// 在棋盘b上执行(x1, y1) -> (x2, y2)移动棋子的动作
void doingAction(int x1, int y1, int x2, int y2, board_t b);
// 创建Minimax树的节点
node* CreateNode(node* parent, action_t action)
// 对Minimax决策树进行深度优先遍历
void DFSTree(node* root, int turn, int depth);
// 在Minimax决策树上进行反向传播, 传播节点的标签用以决策
void BackPropagate(node* root, int dep)
// 释放为决策树分配的内存
void FreeMinimaxTree(node* nd)
// 检查位置(x1, y1)的所有合法移动动作, 将其存储于参数acts中
int GetGotoActions(int x1, int y1, action_t* acts, board_t board)
```

###  数据流程图 

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image020.jpg)

 

## 模块实现  

### 初始化棋盘

```C 
void Init(board_t board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '.';
        }
    }
    for (int i = 0; i < 3; i++) {
        int j = 1;
        if (i == 1) j = 0;
        for (; j < BOARD_SIZE; j += 2) {
            board[i][j] = 'w';
        }
    }
    for (int i = 5; i < BOARD_SIZE; i++) {
        int j = 0;
        if (i == 6) j = 1;
        for (; j < BOARD_SIZE; j += 2) {
            board[i][j] = 'b';
        }
    }
    printf("BOARD SIZE: 8x8\n#BLACK PIECES: 12\n#WHITE PIECES: 12\n");
    OutputBoard(board);
}
```

 ### 检查移动是否合法

```C
int check(int x1, int y1, int x2, int y2, board_t board) {
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8) {
        return 0;
    }
    if (x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        return 0;
    }
    if (board[x1][y1] == '.') {
        return 0;
    }
    if (board[x2][y2] != '.') {
        return 0;
    }
    // move 1 cell AND captures
    if (board[x1][y1] == 'w') {
        // move
        if (x2 - x1 == 1 && abs(y1 - y2) == 1) return 1;
        // capture
        if (x2 - x1 == 2 && abs(y1 - y2) == 2) {
            int mid_x = x1 + (x2 - x1) / 2, mid_y = y1 + (y2 - y1) / 2;
            if (board[mid_x][mid_y] == 'b' || board[mid_x][mid_y] == 'B') return 1;
        }
    }
    if (board[x1][y1] == 'b') {
        // move
        if (x1 - x2 == 1 && abs(y1 - y2) == 1) return 1;
        // capture
        if (x1 - x2 == 2 && abs(y1 - y2) == 2) {
            int mid_x = x1 + (x2 - x1) / 2, mid_y = y1 + (y2 - y1) / 2;
            if (board[mid_x][mid_y] == 'w' || board[mid_x][mid_y] == 'W') return 1;
        }
    }
    if (board[x1][y1] == 'W' || board[x1][y1] == 'B') {
        // move
        if (abs(x1 - x2) == 1 && abs(y1 - y2) == 1) return 1;
        // captures
        if (abs(x1 - x2) == 2 && abs(y1 - y2) == 2) {
            int mid_x = x1 + (x2 - x1) / 2, mid_y = y1 + (y2 - y1) / 2;
            if (board[x1][y1] == 'W') {
                if (board[mid_x][mid_y] == 'b' || board[mid_x][mid_y] == 'B') return 1;
            }
            if (board[x1][y1] == 'B') {
                if (board[mid_x][mid_y] == 'w' || board[mid_x][mid_y] == 'W') return 1;
            }
        }
    }
    return 0;
}
```

### 获得所有可采取的合法动作

```c
int GetGotoActions(int x1, int y1, action_t* acts, board_t board) {
    int nums = 0;
    // Right up, left up
    int to_black[4][2] = {{-1, 1}, {-2, 2}, {-1, -1}, {-2, -2}};
    int to_white[4][2] = {{1, 1}, {2, 2}, {1, -1}, {2, -2}};
    // Right up, Right down, left down, left up
    int to_tower[8][2] = {{-1, 1}, {-2, 2}, {1, 1},   {2, 2},
                          {1, -1}, {2, -2}, {-1, -1}, {-2, -2}};
    if (isupper(board[x1][y1])) {
        for (int i = 0; i < 8; i++) {
            int x2 = x1 + to_tower[i][0];
            int y2 = y1 + to_tower[i][1];
            if (check(x1, y1, x2, y2, board)) {
                acts[nums].x1 = x1;
                acts[nums].y1 = y1;
                acts[nums].x2 = x2;
                acts[nums].y2 = y2;
                nums++;
            }
        }
    }
    if (board[x1][y1] == 'b') {
        for (int i = 0; i < 4; i++) {
            int x2 = x1 + to_black[i][0];
            int y2 = y1 + to_black[i][1];
            if (check(x1, y1, x2, y2, board)) {
                acts[nums].x1 = x1;
                acts[nums].y1 = y1;
                acts[nums].x2 = x2;
                acts[nums].y2 = y2;
                nums++;
            }
        }
    }
    if (board[x1][y1] == 'w') {
        for (int i = 0; i < 4; i++) {
            int x2 = x1 + to_white[i][0];
            int y2 = y1 + to_white[i][1];
            if (check(x1, y1, x2, y2, board)) {
                acts[nums].x1 = x1;
                acts[nums].y1 = y1;
                acts[nums].x2 = x2;
                acts[nums].y2 = y2;
                nums++;
            }
        }
    }
    return nums;
}
```



### 对Minimax tree执行深度优先遍历

```c
void DFSTree(node_t* root, int turn, int turn_back, int depth) {
    if (depth >= 3)
        return;
    char self = "wb"[turn % 2];
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (tolower(root->node_board[i][j]) == self) {
                // extend
                action_t acts[8];
                int actions_cnt = GetGotoActions(i, j, acts, root->node_board);
                // printf("======= -%d (%d %d) %c\n", actions_cnt, i, j,
                // root->n_board[i][j]);
                for (int k = 0; k < actions_cnt; k++) {
                    node_t* child = CreateNode(root, acts[k]);
                    root->child[root->children_cnt++] = child;
                }
            }
        }
    }

    for (int i = 0; i < root->children_cnt; i++) {
        DFSTree(root->child[i], turn + 1, turn_back, depth + 1);
    }
    BackPropagate(root, depth, turn_back);
}
```

### 传播节点的标签信息, 依据子节点进行决策

```c
void BackPropagate(node_t* root, int dep, int turn) {
    if (root->children_cnt == 0) {
        return;
    }

    if (turn % 2 == 1) {  // balck
        if (dep % 2 == 0) {
            int cur_cost = INT_MIN;
            for (int i = 0; i < root->children_cnt; i++) {
                if (root->child[i]->decision_cost > cur_cost) {
                    cur_cost = root->child[i]->decision_cost;
                    root->decision = root->child[i]->action;
                    memcpy(root->decision_board, root->child[i]->node_board,
                           sizeof(root->decision_board));
                }
            }
            root->decision_cost = cur_cost;
        }
        if (dep == 1) {  // min
            int cur_cost = INT_MAX;
            for (int i = 0; i < root->children_cnt; i++) {
                if (root->child[i]->decision_cost < cur_cost) {
                    cur_cost = root->child[i]->decision_cost;
                    root->decision = root->child[i]->action;
                    memcpy(root->decision_board, root->child[i]->node_board,
                           sizeof(root->decision_board));
                }
            }
            root->decision_cost = cur_cost;
        }
    } else {  // white
        if (dep == 1) {
            int cur_cost = INT_MIN;
            for (int i = 0; i < root->children_cnt; i++) {
                if (root->child[i]->decision_cost > cur_cost) {
                    cur_cost = root->child[i]->decision_cost;
                    root->decision = root->child[i]->action;
                    memcpy(root->decision_board, root->child[i]->node_board,
                           sizeof(root->decision_board));
                }
            }
            root->decision_cost = cur_cost;
        }
        if (dep == 0 || dep == 2) {  // min
            int cur_cost = INT_MAX;
            for (int i = 0; i < root->children_cnt; i++) {
                if (root->child[i]->decision_cost < cur_cost) {
                    cur_cost = root->child[i]->decision_cost;
                    root->decision = root->child[i]->action;
                    memcpy(root->decision_board, root->child[i]->node_board,
                           sizeof(root->decision_board));
                }
            }
            root->decision_cost = cur_cost;
        }
    }
}
```

### 运行&测试  

总共提供8组测试用例. 使用

 `gcc -std=c99 -Wall -g -fsanitize=leak ass2-skel.c -o ass2-skel` 命令进行编译. 

![image-20220120144417095](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/image-20220120144417095.png)

以test1.txt为例, 测试用例如下: 

![img](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/clip_image001.png)

程序需要读取到最后一行或是读取到字符`A`或`P`结束. 文件test1.out为标准的输出结果, 需要与之一致. 

![image-20220120144445134](https://raw.githubusercontent.com/lyhellcat/Pic/master/img/image-20220120144445134.png)

可使用**流重载**的方式测试程序的输入输出与标准结果是否一致. 对于所有测试用例, 本程序均可正确输出结果. 

## 总结

本系统主要实现了基于Minimax Tree的下棋决策。系统中的Minimax Tree深度为3，考虑了之后3层的所有可能情况，从而做出当前步骤的最佳下棋决策。“人生如下棋，多思则制胜”，对于生活中的人事物，我们也应该往后多思考几步，考虑多种情况，尽可能作出较优的决策。

