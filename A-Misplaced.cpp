#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Size of the puzzle
const int N = 3;

// Structure to represent a state of the puzzle
struct PuzzleState {
    vector<vector<int>> board;
    int heuristic;  // Misplaced tiles heuristic value
    int cost;       // Cost to reach this state

    // Constructor
    PuzzleState(const vector<vector<int>>& b, int c) : board(b), cost(c) {
        heuristic = calculateHeuristic();
    }

    // Calculate the number of misplaced tiles
    int calculateHeuristic() const {
        int misplaced = 0;
        int targetValue = 0;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != targetValue) {
                    ++misplaced;
                }
                ++targetValue;
            }
        }

        return misplaced;
    }

    // Check if the current state is the goal state
    bool isGoal() const {
        int targetValue = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != targetValue) {
                    return false;
                }
                ++targetValue;
            }
        }
        return true;
    }

    // Check if two puzzle states are equal
    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }
};

// Hash function for PuzzleState (used for unordered_set)
struct PuzzleStateHash {
    size_t operator()(const PuzzleState& state) const {
        size_t hashValue = 0;
        for (const auto& row : state.board) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Compare function for priority_queue
struct ComparePuzzleState {
    bool operator()(const PuzzleState& lhs, const PuzzleState& rhs) const {
        return (lhs.cost + lhs.heuristic) > (rhs.cost + rhs.heuristic);
    }
};

// Function to print the puzzle state
void printPuzzleState(const PuzzleState& state) {
    for (const auto& row : state.board) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic: " << state.heuristic << " Cost: " << state.cost << endl;
    cout << "-----------------" << endl;
}

// Function to perform the A* search
void aStarSearch(const PuzzleState& initial) {
    priority_queue<PuzzleState, vector<PuzzleState>, ComparePuzzleState> pq;
    unordered_set<PuzzleState, PuzzleStateHash> visited;

    pq.push(initial);

    while (!pq.empty()) {
        PuzzleState current = pq.top();
        pq.pop();

        if (current.isGoal()) {
            cout << "Goal state reached!" << endl;
            printPuzzleState(current);
            return;
        }

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            printPuzzleState(current);

            // Generate possible next states
            // In this implementation, only left, right, up, and down moves are considered
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Find the position of the zero (empty space)
                        for (int i = 0; i < N; ++i) {
                            for (int j = 0; j < N; ++j) {
                                if (current.board[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Check if the new position is within the bounds
                        if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                            // Create a new state by swapping the zero and the adjacent tile
                            vector<vector<int>> newBoard = current.board;
                            swap(newBoard[newX][newY], newBoard[newX - dx][newY - dy]);

                            PuzzleState nextState(newBoard, current.cost + 1);

                            // Add the new state to the priority queue
                            pq.push(nextState);
                        }
                    }
                }
            }
        }
    }

    cout << "Goal state not reachable!" << endl;
}

int main() {
    // Example initial state
    vector<vector<int>> initialState = {
        {8, 0, 6},
        {5, 4, 7},
        {2, 3, 1}
    };

    PuzzleState initialPuzzleState(initialState, 0);

    cout << "Initial state:" << endl;
    printPuzzleState(initialPuzzleState);
    cout << "Starting A* search..." << endl;
    aStarSearch(initialPuzzleState);
    return 0;
}
