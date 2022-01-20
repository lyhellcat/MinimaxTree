#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8        // board size
#define ROWS_WITH_PIECES 3  // number of initial rows with pieces
#define CELL_EMPTY '.'      // empty cell character
#define CELL_BPIECE 'b'     // black piece character
#define CELL_WPIECE 'w'     // white piece character
#define CELL_BTOWER 'B'     // black tower character
#define CELL_WTOWER 'W'     // white tower character
#define COST_PIECE 1        // one piece cost
#define COST_TOWER 3        // one tower cost
#define TREE_DEPTH 3        // minimax tree depth
#define COMP_ACTIONS 10     // number of computed actions
#define MAX_CHILD 100       // max child num  8 * 12
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

// Define action from (x1, y1) to (x2, y2)
typedef struct {
    int x1, y1;
    int x2, y2;
} action_t;

// Define desion tree node
typedef struct node {
    board_t node_board;
    board_t decision_board;
    int decision_cost;
    int children_cnt;
    struct node* child[MAX_CHILD];
    action_t action;
    action_t decision;
} node_t;

void OutputBoard(board_t board) {
    puts("     A   B   C   D   E   F   G   H");
    for (int i = 0; i < BOARD_SIZE; i++) {
        puts("   +---+---+---+---+---+---+---+---+");
        printf(" %d", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" | ");
            printf("%c", board[i][j]);
        }
        printf(" |\n");
    }
    puts("   +---+---+---+---+---+---+---+---+");
}

int CalcBoardCost(board_t board) {
    // Board cost = b + 3B - w - 3W
    int cost = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'b') cost += 1;
            if (board[i][j] == 'B') cost += 3;
            if (board[i][j] == 'w') cost -= 1;
            if (board[i][j] == 'W') cost -= 3;
        }
    }
    return cost;
}

// Set the board
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

void checkingAction(int x1, int y1, int x2, int y2, int turn, board_t board) {
    turn = turn % 2;
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8) {
        puts("ERROR: Source cell is outside of the board.");
        exit(0);
    }
    if (x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        puts("ERROR: Target cell is outside of the board.");
        exit(0);
    }
    if (board[x1][y1] == '.') {
        puts("ERROR: Source cell is empty.");
        exit(0);
    }
    if (board[x2][y2] != '.') {
        puts("ERROR: Target cell is not empty.");
        exit(0);
    }
    if (turn && (board[x1][y1] == 'w' || board[x1][y1] == 'W')) {
        puts("ERROR: Source cell holds opponent\'s piece/tower.");
        exit(0);
    }
    if (!turn && (board[x1][y1] == 'b' || board[x1][y1] == 'B')) {
        puts("ERROR: Source cell holds opponent\'s piece/tower.");
        exit(0);
    }
    if (!check(x1, y1, x2, y2, board)) {
        puts("ERROR: Illegal action.");
        exit(0);
    }
}

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

void doingAction(int x1, int y1, int x2, int y2, board_t board) {
    // move
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = '.';
    // If action is capture
    if (abs(x1 - x2) == 2) {
        int mid_x = x1 + (x2 - x1) / 2, mid_y = y1 + (y2 - y1) / 2;
        board[mid_x][mid_y] = '.';
    }
    // Convert to Tower
    if (board[x2][y2] == 'b' && x2 == 0) board[x2][y2] = 'B';
    if (board[x2][y2] == 'w' && x2 == BOARD_SIZE - 1) board[x2][y2] = 'W';
}

node_t* CreateNode(node_t* parent, action_t action) {
    node_t* new_node = calloc(1, sizeof(node_t));
    new_node->action = action;

    // parent apply
    memcpy(new_node->node_board, parent->node_board, sizeof(new_node->node_board));
    doingAction(action.x1, action.y1, action.x2, action.y2, new_node->node_board);
    new_node->decision_cost = CalcBoardCost(new_node->node_board);

    return new_node;
}

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

// turn == 1 -> black go
// dfs
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

// create root
node_t* CreateTree(board_t board) {
    node_t* root = malloc(sizeof(node_t));
    root->children_cnt = 0;
    // root->nod_board = b;  nod_board = b
    memcpy(root->node_board, board, sizeof(root->node_board));
    memcpy(root->decision_board, board, sizeof(root->node_board));
    root->decision_cost = CalcBoardCost(root->node_board);

    return root;
}

void FreeMinimaxTree(node_t* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->children_cnt; i++)
        FreeMinimaxTree(node->child[i]);

    free(node);
}

void ReadData(board_t board) {
    char command[20];
    int turn = 0;
    while (~scanf("%s", command)) {
        turn++;
        if (command[0] == 'A') {
            // TODO Stage1
            node_t* root = CreateTree(board);
            DFSTree(root, turn % 2, turn % 2, 0);

            if (root->decision_cost == INT_MAX ||
                root->decision_cost == INT_MIN) {
                if (turn % 2 == 1)
                    puts("WHITE WIN");
                else
                    puts("BLACK WIN");
            }
            puts("=====================================");
            if (turn % 2)
                printf("*** BLACK ");
            else
                printf("*** WHITE ");
            // printf("ROOT %d\n")
            printf("ACTION #%d: %c%d-%c%d\n", turn, root->decision.y1 + 'A',
                   root->decision.x1 + 1, root->decision.y2 + 'A',
                   root->decision.x2 + 1);
            printf("BOARD COST: %d\n", CalcBoardCost(root->decision_board));
            OutputBoard(root->decision_board);
            FreeMinimaxTree(root);
        } else if (command[0] == 'P') {
            // TODO Stage2
            node_t* root = CreateTree(board);
            DFSTree(root, turn % 2, turn % 2, 0);
            board_t back_board;
            for (int i = 0; i < 10; i++) {
                if (root->decision_cost == INT_MAX ||
                    root->decision_cost == INT_MIN) {
                    if (turn % 2 == 1)
                        puts("WHITE WIN");
                    else
                        puts("BLACK WIN");
                }
                puts("=====================================");
                if (turn % 2)
                    printf("*** BLACK ");
                else
                    printf("*** WHITE ");
                printf("ACTION #%d: %c%d-%c%d\n", turn, root->decision.y1 + 'A',
                       root->decision.x1 + 1, root->decision.y2 + 'A',
                       root->decision.x2 + 1);
                printf("BOARD COST: %d\n", CalcBoardCost(root->decision_board));
                OutputBoard(root->decision_board);

                turn++;
                memcpy(back_board, root->decision_board, sizeof(back_board));
                FreeMinimaxTree(root);
                root = CreateTree(back_board);
                DFSTree(root, turn % 2, turn % 2, 0);
            }
            FreeMinimaxTree(root);
        } else {
            char c1, c2;
            int a1, a2;
            sscanf(command, "%c%d-%c%d", &c1, &a1, &c2, &a2);
            int x1 = a1 - 1, y1 = c1 - 'A';
            int x2 = a2 - 1, y2 = c2 - 'A';
            checkingAction(x1, y1, x2, y2, turn % 2, board);
            doingAction(x1, y1, x2, y2, board);
            puts("=====================================");
            if (turn % 2)
                printf("BLACK ");
            else
                printf("WHITE ");
            printf("ACTION #%d: %s\n", turn, command);
            printf("BOARD COST: %d\n", CalcBoardCost(board));
            OutputBoard(board);
        }
    }
}

int main(int argc, char* argv[]) {
    // IMPLEMENTATION OF STAGES 0-2
    board_t board;
    Init(board);
    ReadData(board);

    return EXIT_SUCCESS;  // exit program with the success code
}
