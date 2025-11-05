# SocialNet Simulator (COL106)

This is a command-line simulator for a social network backend, built for the **COL106 Data Structures** course.  
It handles user management, friend graphs, and content posting using custom-built graph and AVL tree data structures.

---

## How to Compile and Run

The project includes compile scripts for both Windows and POSIX systems.

### On Linux/macOS
Make the script executable, then run it:
```bash
chmod +x compile.sh
./compile.sh
```
Output:
```
Compilation successful. Running program...
Welcome!
```

### On Windows
Run the batch file from your terminal:
```bat
compilewin.bat
```
Output:
```
Compilation successful. Running program...
Welcome!
```

---

## System Architecture: Core Data Structures

The system is built on three main data structures, plus helpers from the standard library.

###  Graph (Adjacency List for Friendships)

**Implementation:** `std::vector<std::set<std::string>> friends`  
**Design:** Each user is assigned an integer ID (their index in the vector).  
A hash map links usernames to these IDs.

**Why:** Using `std::set` keeps each friend list alphabetically sorted, simplifying `LIST_FRIENDS`.

---

###  AVL Tree (Per-User Post Storage)

**Implementation:** Custom `AVLTree` class (from `AVL_tree.h`)  
**Design:** Each user has an `AVLTree` mapped to their username.  
Each node stores a `(timestamp, content)` pair, ordered by `time_t`.

**Why:** Ensures posts remain sorted by time. AVL balancing keeps insertion and retrieval at **O(log P)**, where *P* = number of posts per user.

---

### Hash Maps (Primary Lookups)

Two unordered maps support O(1) average lookups:

- `std::unordered_map<std::string, int> index` → maps username → user ID  
- `std::unordered_map<std::string, AVLTree> posts` → maps username → their post tree

---

### Priority Queue (Friend Suggestions)

**Implementation:** `std::priority_queue` with a custom comparator.  
Used in `SUGGEST_FRIENDS()` to rank “friends of friends.”

**Ranking logic:**
1. More mutual friends = higher priority  
2. Alphabetical order used as tiebreaker

This avoids sorting entire lists and keeps friend recommendation efficient.

---

## Key Algorithms

### Custom Comparator
Used in the friend-suggestion heap.  
`a.first < b.first` sorts by friend count, `a.second > b.second` breaks ties alphabetically.

###  BFS (Degrees of Separation)
`DEGREES_OF_SEPARATION` runs a **Breadth-First Search** on the friend graph to find the shortest path.  
A `std::vector<bool>` tracks visited users to prevent cycles.

###  Hashing in Suggestions
Temporary hash maps in `SUGGEST_FRIENDS` ensure no duplicate candidates and count mutual friends before pushing to the heap.

---

## Project Structure

| File | Purpose |
|------|----------|
| **mainfile.cpp** | Reads commands, parses them using `stringstream`, and calls SocialNet methods. Also defines `normalize()` for case-insensitive usernames. |
| **SocialNet.h** | Core class implementing all features. Manages users, friends, and posts. |
| **AVL_tree.h** | Implements AVL tree for per-user post management. |
| **compile.sh / compilewin.bat** | Scripts for building and running the program. |

---

## Main Functions (in SocialNet.h)

- `ADD_USER(string)`  
- `ADD_FRIEND(string, string)`  
- `LIST_FRIENDS(string)`  
- `SUGGEST_FRIENDS(string, int)`  
- `DEGREES_OF_SEPARATION(string, string)`  
- `ADD_POST(string, string, time_t)`  
- `OUTPUT_POSTS(string, int)`  

### AVLTree Methods
- `insert(pair<time_t,std::string>)`
- `rightRotate()`, `leftRotate()`
- `Ngreatest(int N)` → returns N most recent posts (reverse in-order traversal)

---

## System Features and Robustness

- **Case-insensitive usernames** (via `normalize()`)  
- **Shortest-path search** using BFS  
- **Handles same-second posts** safely AVL insert allows equal timestamps  
- **Smart OUTPUT_POSTS logic** N = -1 automatically prints all posts  
- **Error-resilient** every user operation checked before lookup or insert  

---

## Error Handling

| Issue | Behavior |
|--------|-----------|
| Duplicate user | Prints error, does not overwrite |
| Invalid username | Prints `"User does NOT exist!"` |
| Same-name friend request | Prints `"Illegal operation"` |
| Unknown command | Prints `"Error: Unknown command <op>"` |
| N = 0 in SUGGEST_FRIENDS/OUTPUT_POSTS | Prints nothing |
| Empty or space-only posts | Ignored safely |

---

## Command Reference

All `<username>` arguments are case-insensitive.

### **ADD_USER <username>**
Creates a user. Error if user already exists.

### **ADD_FRIEND <username1> <username2>**
Creates a two-way friendship.  
Rejects self-friending or duplicates.

### **LIST_FRIENDS <username>**
Prints sorted friend list. Empty line if no friends.

### **SUGGEST_FRIENDS <username> <N>**
Shows up to N friend suggestions ranked by mutuals.

### **DEGREES_OF_SEPARATION <username1> <username2>**
Returns shortest connection length, or `-1` if no path.

### **ADD_POST <username> <content>**
Adds a timestamped post to user’s AVL tree.

### **OUTPUT_POSTS <username> <N>**
Prints N most recent posts.  
`N = -1` → all posts.

### **EXIT**
Terminates the program gracefully.

---

## Example Session

```text
ADD_USER alice
ADD_USER bob
ADD_USER charlie
ADD_USER diana

ADD_FRIEND alice bob
ADD_FRIEND bob charlie
ADD_FRIEND diana charlie

LIST_FRIENDS alice
> bob

DEGREES_OF_SEPARATION alice diana
> 2

SUGGEST_FRIENDS alice 1
> diana

ADD_POST alice Hello world
ADD_POST alice This is my second post

OUTPUT_POSTS alice 1
> this is my second post

OUTPUT_POSTS alice -1
> this is my second post
> hello world

EXIT
> Exiting program
> Bye!
```

---

## Why the System Is Robust

- Every major container protected with error checks.  
- No invalid memory access (all lookups done via `.at()` + try–catch).  
- Input parsing handles extra spaces and empty lines.  
- BFS ensures no infinite loops in friend graph.  
- AVL guarantees balanced insertions for post storage.

---

**End of README**
