SocialNet Simulator (COL106)
============================

This is a command-line simulator for a social network backend, built for the COL106 Data Structures course. It handles user management, friend graphs, and content posting using custom-built graph and AVL tree data structures.

How to Compile and Run
----------------------

The project includes compile scripts for Windows and POSIX systems.

**On Linux/macOS:**Make the script executable, then run it.

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   $chmod +x compile.sh$ ./compile.sh  Compilation successful. Running program...  Welcome!   `

**On Windows:**Run the batch file from your terminal.

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   C:\Project> compilewin.bat  Compilation successful. Running program...  Welcome!   `

System Architecture: Core Data Structures
-----------------------------------------

The system is built on three main data structures, plus helpers from the standard library.

1.  **Graph (Adjacency List for Friendships)**
    
    *   **Implementation:** std::vector\> friends
        
    *   **Design:** This is an adjacency list where each user's integer ID is their index in the vector. A hash map links the username string to this ID.
        
    *   **Why:** The std::set at each index stores that user's friends. This automatically keeps the friend list sorted alphabetically, which makes the LIST\_FRIENDS command very simple.
        
2.  **AVL Tree (Per-User Post Storage)**
    
    *   **Implementation:** A custom AVLTree class (from AVL\_tree.h).
        
    *   **Design:** Each user has their own AVLTree instance, mapped from their username. The tree nodes store a std::pair, using the time\_t timestamp as the key.
        
    *   **Why:** This guarantees posts are sorted by time. As a self-balancing tree, all post operations (inserting, searching) are kept at O(log P) time, where P is the number of posts for that user.
        
3.  **Hash Maps (Primary Lookups)**
    
    *   **Implementation:** Two std::unordered\_maps for O(1) average-time lookups.
        
    *   std::unordered\_map index: The main user index. It maps a username string to its integer ID (the index in the friends vector).
        
    *   std::unordered\_map posts: This map links a username string straight to that user's AVLTree object for their posts.
        
4.  **Priority Queue (Friend Suggestions)**
    
    *   **Implementation:** std::priority\_queue used in the SUGGEST\_FRIENDS function.
        
    *   **Design:** A max-heap ranks all "friends of friends." A custom comparator struct defines the ranking:
        
        1.  Primary: Number of mutual friends (most first).
            
        2.  Secondary: Username (alphabetical) for tie-breaking.
            
    *   **Why:** This avoids sorting a whole list. We just find all candidates, count their mutual friends (using temporary hashmaps), and push them onto the heap. Then we pop the top N users.
        

Key Algorithms
--------------

*   **Custom Priority Queue Comparator**
    
    *   The comparator in SocialNet.h is built for the max-heap. a.first < b.first orders by friend count. a.second > b.second is used for tie-breaking, giving "alice" a higher priority than "bob".
        
*   **BFS with Visited Array**
    
    *   DEGREES\_OF\_SEPARATION uses a Breadth-First Search (BFS) to find the shortest path in the friend graph.
        
    *   A std::vector vis (indexed by user ID) is used to track visited nodes. This prevents the search from looping and ensures the path is the shortest.
        
*   **Hashing in SUGGEST\_FRIENDS**
    
    *   This function uses two temporary std::unordered\_maps to work:
        
    *   std::unordered\_map seen: Tracks which "friend of a friend" has been found to avoid duplicates.
        
    *   std::unordered\_map mutuals: Counts mutual friends for each suggestion before adding them to the priority queue.
        

Project Structure
-----------------

*   **mainfile.cpp**: Holds the main program loop. It reads and parses commands from stdin using stringstream and calls the SocialNet methods. Also contains the normalize() function for case-insensitivity.
    
*   **SocialNet.h**: Defines the main SocialNet class. This class holds the graph (friends) and the main lookup maps (index, posts).
    
    *   ADD\_USER(string)
        
    *   ADD\_FRIEND(string, string)
        
    *   LIST\_FRIENDS(string)
        
    *   SUGGEST\_FRIENDS(string, int)
        
    *   DEGREES\_OF\_SEPARATION(string, string)
        
    *   ADD\_POST(string, string, time\_t)
        
    *   OUTPUT\_POSTS(string, int)
        
*   **AVL\_tree.h**: Defines the Node struct and the AVLTree class.
    
    *   insert(pair): Public function to add a post.
        
    *   insertHelper(Node\*, pair): Private, recursive function that inserts and re-balances.
        
    *   rightRotate(Node\*) & leftRotate(Node\*): Standard AVL rotation functions.
        
    *   Ngreatest(int N): Implements an iterative, stack-based reverse in-order traversal to get the N most recent posts (newest first).
        

System Features and Robustness
------------------------------

*   **Case-Insensitivity:** All usernames are normalized, so ADD\_USER Alice and ADD\_FRIEND alice work on the same user.
    
*   **Pathfinding:** DEGREES\_OF\_SEPARATION uses BFS, the best algorithm for finding the shortest path in this kind of graph.
    
*   **Handles Same-Second Posts:** The system is designed to handle posts made in the same second. Because time(nullptr) only has a 1-second resolution, multiple posts can have an identical timestamp. The AVL tree is built to correctly insert these "duplicate" keys (placing them in the right subtree), ensuring no posts are lost and that they are still retrieved in the correct order.
    
*   **Smart N=-1 Logic:** OUTPUT\_POSTS with N=-1 (for all posts) is handled smartly. The Ngreatest(N) function's loop if(count == N) never becomes true, so the loop naturally runs to completion and gets all posts without needing a separate if block.
    

### Error Handling

The system is designed to handle bad input without crashing.

*   **Unknown Users:** All commands that take a username (ADD\_FRIEND, LIST\_FRIENDS, etc.) use try...catch blocks. If a user doesn't exist, it prints a clean error message, not a segfault.
    
*   **No Path:** DEGREES\_OF\_SEPARATION returns -1 if there's no path or if a user doesn't exist.
    
*   **N=0:** SUGGEST\_FRIENDS 0 and OUTPUT\_POSTS 0 correctly print nothing.
    
*   **Invalid Friending:** ADD\_FRIEND checks if name1 == name2 and stops a user from friending themselves.
    

Command Reference
-----------------

All arguments are case-insensitive.

*   **ADD\_USER**
    
    *   **Action:** Creates a new user.
        
    *   **Details:** Adds the user to the index map, resizes the friends graph, and creates an empty AVLTree for them.
        
    *   **Edge Case:** Prints an error if the user already exists.
        
*   **ADD\_FRIEND**
    
    *   **Action:** Creates a two-way friendship.
        
    *   **Details:** Adds each user to the other's std::set in the friends graph.
        
    *   **Edge Cases:**
        
        *   Prints an error if either user doesn't exist.
            
        *   Prints an error if username1 == username2.
            
        *   Does nothing if they are already friends.
            
*   **LIST\_FRIENDS**
    
    *   **Action:** Prints the user's friend list.
        
    *   **Details:** Reads from the user's std::set, which is already sorted alphabetically.
        
    *   **Edge Cases:**
        
        *   Prints an error if username doesn't exist.
            
        *   Prints a blank line if the user has no friends.
            
*   **SUGGEST\_FRIENDS**
    
    *   **Action:** Recommends up to N new friends.
        
    *   **Details:** Uses the "friends of friends" logic, ranked by a priority queue.
        
    *   **Edge Cases:**
        
        *   Prints nothing if N = 0 or no suggestions are found.
            
        *   Prints an error if username doesn't exist.
            
*   **DEGREES\_OF\_SEPARATION**
    
    *   **Action:** Finds the shortest friendship path length.
        
    *   **Details:** Runs a BFS from username1 to username2.
        
    *   **Edge Cases:**
        
        *   Returns 0 if username1 == username2.
            
        *   Returns -1 if either user doesn't exist or no path is found.
            
*   **ADD\_POST**
    
    *   **Action:** Adds a post to the user's timeline.
        
    *   **Details:** Gets the current time and inserts the post into the user's AVLTree.
        
    *   **Edge Cases:**
        
        *   Prints an error if username doesn't exist.
            
        *   Handles empty or space-only posts.
            
*   **OUTPUT\_POSTS**
    
    *   **Action:** Prints the N most recent posts.
        
    *   **Details:** Calls Ngreatest(N) on the user's AVLTree for a reverse in-order traversal.
        
    *   **Edge Cases:**
        
        *   If N = -1, all posts are printed.
            
        *   If N = 0, nothing is printed.
            
        *   If user has fewer than N posts, it prints all available posts.
            
        *   Prints an error if username doesn't exist.
            
*   **EXIT**
    
    *   **Action:** Quits the program.
        
    *   **Details:** Breaks the main input loop.
        

Sample Usage
------------

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   ADD_USER alice  ADD_USER bob  ADD_USER charlie  ADD_USER diana  ADD_FRIEND alice bob  ADD_FRIEND bob charlie  ADD_FRIEND alice charlie  ADD_FRIEND diana charlie  LIST_FRIENDS alice  > bob charlie  DEGREES_OF_SEPARATION alice diana  > 2  SUGGEST_FRIENDS alice 1  > diana  ADD_POST alice Hello world  ADD_POST alice This is my second post  OUTPUT_POSTS alice 1  > this is my second post  OUTPUT_POSTS alice -1  > this is my second post  > hello world  ADD_FRIEND alice non_existent_user  > Error: User "non_existent_user" does NOT exist!  ADD_FRIEND bob bob  > Error: Cannot add yourself as a friend.  EXIT  > Exiting program  > Bye!   `